#include <stdint.h>

#include <cmath>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

#include "./debug.h"
#include "./expression.h"
#include "./functions.h"
#include "./rules.h"
#include "./variables.h"
#include "./wm.h"
#include "./parser/language.h"
#include "./parser/parser.h"

using ::std::queue;

/*
 * Fa il parsing di un blocco racchiuso tra beginRule
 * e endRule (esclusi).
 * Aggiunge anche la regola alla working memory.
 */
void
Parser::parse_rule_block(list<Token> token_list) {
  list<Token>::iterator token_iterator = token_list.begin();
  list<Token> lhs, rhs;

  if (token_list.size() < 4)
    throw ParseError("minimum number of required tokens not satisfied for rule",
        token_iterator->get_line_no());

  /* Il primo parametro e` il nome della regola */
  if (token_iterator->get_type() != TKN_SYMBOL)
    throw ParseError("expected rule name", token_iterator->get_line_no());
  string name = token_iterator->get_value();
  ++token_iterator;

  debug(name);
  Rule *rule = new Rule(name);

  lhs = BUILD_TOKEN_BLOCK(token_iterator,
                          string(1, LNG_RULE_ACTION).c_str(),
                          TKN_OPERATOR,
                          token_list.end());

  rule->set_lhs(this->parse_rule_lhs(lhs));
  token_iterator = this->advance_iterator(token_iterator, lhs.size());
  ++token_iterator;

  while (token_iterator != token_list.end())
    rhs.push_back(*(token_iterator++));
  /* XXX:
   * La scrittura sotto *dovrebbe* essere assolutamente equivalente
   * a quella sopra, solo che per qualche motivo a me oscuro crasha
   * inesorabilmente. La domanda ora e`: PERCHE?
   *
   * copy(token_iterator, token_list.end(), rhs.begin());
   */

  rule->set_rhs(this->parse_rule_rhs(rhs));

  this->global->ruleset->add(rule);
};

/*
 * Fa il parsing della LHS della regola (si costruisce una lista di
 * Expression, controlla che la sequenza sia valida (es: operando
 * operatore operando operatore) e la manda a parse_expression)
 */
Expression *
Parser::parse_rule_lhs(list<Token> token_list) {
  return this->parse_expression(token_list);
};

/*
 * Fa il parsing della RHS della regola
 */
list<PredicateCall>
Parser::parse_rule_rhs(list<Token> token_list) {
  list<Token>::iterator token_iterator = token_list.begin();
  list<Token> predicate_tokens;
  list<PredicateCall> rhs;
  uint32_t read_tokens = 0;

  if (token_iterator == token_list.end())
    throw ParseError("empty right hand side");

  while (token_iterator != token_list.end()) {
    predicate_tokens = BUILD_TOKEN_BLOCK(token_iterator, string(1, LNG_STMT_END).c_str(), TKN_OPERATOR, token_list.end());
    Predicate pred = this->parse_predicate(predicate_tokens, predicate_tokens.begin(), &read_tokens);
    token_iterator = this->advance_iterator(token_iterator, read_tokens);
    ++token_iterator;
    read_tokens = 0;

    if ((token_iterator->get_type() == TKN_OPERATOR) &&
        (!token_iterator->get_value().compare(string(1, LNG_STMT_END)))) {
      ++token_iterator;
    } else if (token_iterator == token_list.end()) {
      throw ParseError("expected ; as predicate separator", (*(--token_iterator)).get_line_no());
    }

    PredicateCall call;
    call.set_name(pred.get_name());
    call.set_parameters(pred.get_parameters());
    rhs.push_back(call);
  }
  return rhs;
};

/******************
 * Classe RuleSet *
 ******************/

void
RuleSet::add(Rule *rule) {
#ifdef DEBUG
  string r("Adding rule ");
  r.append(rule->get_name());
  r.append(" to working memory");
  debug(r);
#endif
  this->rules.push_back(rule);
};

/*
 * Costruisce il conflict set delle regole applicabili sullo stato
 * attuale della working memory.
 */
list<RuleApplication> *
RuleSet::get_conflict_set(Fact *fact) {
  list<RuleApplication> *conflict_set = new list<RuleApplication>();
  list<Rule *>::iterator rule = this->rules.begin();

  /* Risolvi le variabili in tutti i modi possibili e genera una
   * lista di regole risultate applicabili alla valutazione, con assieme
   * una determinata sequenza di variabili (valide) gia` sostituite
   *
   * (Il cuore di tutto si trova dentro solve_search_tree, che e`
   * ampiamente commentata)
   *
   * Purtroppo questo e` un problema NP-completo, e non abbiamo
   * altra soluzione che esplorare l'intero spazio di ricerca
   */

  vector<DTArrayItem> values = this->explode_var_values(fact);

  /* se il fatto non ha attributi nessuna regola sara` soddisfatta */
  if (fact->get_attribute_list()->size() == 0)
    return conflict_set;

  while (rule != this->rules.end()) {
    vector<string> *variables;

    /* una volta estratte le variabili, mantienile in una
     * sorta di cache all'interno della regola
     */
    if ((*rule)->extracted_variables) {
      variables = (*rule)->extracted_variables;
    } else {
      variables = new vector<string>(extract_variables((*rule)->get_lhs()));
      (*rule)->extracted_variables = variables;
    }

    list<RuleApplication> applications = this->solve_search_tree(variables, &values, *rule, fact);

    conflict_set->splice(conflict_set->end(), applications);
    ++rule;
  }

  return conflict_set;
};

uint32_t
RuleSet::rules_number() {
  return this->rules.size();
};

/*
 * Applica la parte destra della regola al fatto passato in input
 * Torna un *nuovo* fatto, risultato dell'applicazione della regola
 */
void
RuleSet::apply(RuleApplication *application, Fact *fact) {
  list<PredicateCall> rhs = application->rule->get_rhs();
  list<PredicateCall>::iterator rhs_iter = rhs.begin();
  FuncMemory funcmem = FuncMemory(application->funcmem);

  __WM_RHS_FUNC tmp_func;
  while (rhs_iter != rhs.end()) {
    debug(rhs_iter->get_name());
    tmp_func = RESOLVE_RHS_PREDICATE(rhs_iter->get_name());

    if (!tmp_func) {
      string err(rhs_iter->get_name());
      err.append(" is an unknown function");
      throw WorkingMemoryError(err);
    }

    tmp_func(&funcmem, fact, rhs_iter->get_parameters());
    ++rhs_iter;
  }
};

/*
 * Esplora lo spazio di ricerca per provare tutte le sostituzioni
 * possibili di variabili. Quando ha trovato una sostituzione valida
 * prova a valutare la regola, se e` soddisfatta la potremo aggiungere
 * al conflict set
 */
list<RuleApplication>
RuleSet::solve_search_tree(vector<string> *variables, vector<DTArrayItem> *values, Rule *rule, Fact *fact) {
  /* L'algoritmo seguito qui consta di due passi principali:
   *   1. Creiamo una sorta di cache per i possibili valori che
   *      le variabili possono assumere (gia` fatto in explode_var_values)
   *   2. Esplodiamo l'albero con tutte le possibili combinazioni
   *      variabile/valore. In realta` qui non utilizziamo un albero
   *      vero e proprio ma un vettore di tanti elementi quante sono
   *      i valori che le variabili possono assumere, pero` l'esplorazione
   *      avviene come se fosse un albero
   */

  Expression *expr = rule->get_lhs();
  list<RuleApplication> ret;
  FuncMemory funcmem;
  uint32_t i;
  uint32_t attribute_no = values->size();
  uint32_t var_no = variables->size();
  int32_t pos;

  if (variables->size() == 0) {
    /* se non ci sono variabili valutiamo la regola direttamente */
    if (expr->evaluate(&funcmem, fact)) {
      debug(rule->get_name());
      RuleApplication r;
      r.rule = rule;
      r.funcmem = funcmem;

      ret.push_back(r);
    }

    return ret;
  }

  vector<Variable> resolved_vars;
  vector<uint32_t> idx(var_no);
  for (i = 0; i < var_no; i++) {
    Variable tmp;
    tmp.varname = (*variables)[i];
    tmp.value = (*values)[0];
    resolved_vars.push_back(tmp);
    idx[i] = 0;
  }

  bool resolved = false;
  while (!resolved) {
    funcmem.clear();

    /* costruisci una FuncMemory con i valori puntati dagli indici delle
     * variabili. Tale FuncMemory sara` poi quella che verra` usata durante
     * la verifica della regola.
     */
    pos = var_no;
    while (pos-- > 0)
      funcmem[resolved_vars[pos].varname] = resolved_vars[pos].value;

    if (expr->evaluate(&funcmem, fact)) {
      /* la regola e` risultata applicabile, aggiungiamola alla lista di
       * applicazioni possibili
       */
      debug(rule->get_name());
      RuleApplication r;
      r.rule = rule;
      r.funcmem = funcmem;

      ret.push_back(r);
    }

    /* controlla se abbiamo finito */
    for (i = 0; i < var_no; i++) {
      resolved = (idx[i] == attribute_no - 1);
      if (!resolved) break;
    }

    if (resolved) break;

    /* altrimenti incrementa il contatore degli indici */
    increment_idxs(&idx, var_no, attribute_no, &resolved_vars, values);
  }

  return ret;
};

/*
 * Incrementa i valori del vettore di indici delle variabili
 */
void inline
__ATTR(__INLINE, __NOTHROW)
increment_idxs(vector<uint32_t> *idx,
               uint32_t var_no,
               uint32_t max,
               vector<Variable> *resolved_vars,
               vector<DTArrayItem> *values) {
  register int16_t tmpval = var_no - 1;

  (*idx)[tmpval] = ((*idx)[tmpval] + 1) % max;
  (*resolved_vars)[tmpval].value = (*values)[(*idx)[tmpval]];

  while ((*idx)[tmpval] == 0 && tmpval > 0) {
    tmpval = tmpval - 1;
    (*idx)[tmpval] = ((*idx)[tmpval] + 1) % max;
    (*resolved_vars)[tmpval].value = (*values)[(*idx)[tmpval]];
  }
};

/*
 * Ritorna un vettore con all'interno tutti i possibili valori
 * che le variabili possono assumere
 */
vector<DTArrayItem>
RuleSet::explode_var_values(Fact *fact) {
  vector<DTArrayItem> values;
  queue<DTArrayItem> values_queue;
  uint32_t i, len;
  DTArray *array_ptr;

  attrmap::iterator attr = fact->get_attribute_list()->begin();
  while (attr != fact->get_attribute_list()->end()) {
    /* anche il nome dell'attributo e` un possibile valore valido per
     * una variabile
     */
    values_queue.push(DTArrayItem(ITEM_STRING, attr->first));
    len = fact->get_attribute(attr->first)->length();

    for (i = 0; i < len; i++) {
      array_ptr = fact->get_attribute(attr->first);
      values_queue.push(*array_ptr->at(i));
    }

    ++attr;
  }

  /* Prima di iniziare, rimuoviamo eventuali valori duplicati cosi` da diminuire
   * la dimensione dell'albero di ricerca
   *
   * XXX: questo modo di ricerca e` tutt'altro che efficiente dato che e` O(n^2),
   * sarebbe meglio trovare un altro metodo
   */
  bool found = false;
  while (!values_queue.empty()) {
    DTArrayItem tmp = values_queue.front();
    values_queue.pop();

    found = false;
    for (i = 0; i < values.size(); i++) {
      if (!values[i].get_value().compare(tmp.get_value()))
        found = true;
    }

    if (!found) values.push_back(tmp);
  }

  return values;
};

void
RuleSet::add_heuristic(string name, list<PredicateCall> h) {
  this->heuristics[name] = h;
};
