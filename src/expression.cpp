#include <string>
#include <list>

#include "./debug.h"
#include "./facts.h"
#include "./functions.h"
#include "./expression.h"
#include "./predicate.h"
#include "./parser/parser.h"

/* Funzione di valutazione per l'and */
bool AndBooleanExpr::evaluate(FuncMemory *funcmem, Fact *fact) {
  return (this->cond1->evaluate(funcmem, fact) &&
          this->cond2->evaluate(funcmem, fact));
};

/* Funzione di valutazione per l'or */
bool OrBooleanExpr::evaluate(FuncMemory *funcmem, Fact *fact) {
  /* Usa la legge di de morgan per portare questa formula in AND */
  return (!this->cond1->evaluate(funcmem, fact) &&
          !this->cond2->evaluate(funcmem, fact));
};

/*
 * Effettua il parsing di un espressione e ritorna l'expression tree
 * relativo
 */
Expression *Parser::parse_expression(list<Token> token_list) {
  unsigned int read_tokens = 0, brace_count = 1;
  list<Token>::iterator token_iterator = token_list.begin();
  list<Token> new_token_list;

  AndBooleanExpr *andexpr;
  OrBooleanExpr *orexpr;
  Expression *root = NULL;
  Expression *operand = NULL;
  PredicateExpr *operand_pred = NULL;

  debug("expr left op recognition");
  new_token_list.clear();
  /* Riconosci l'operando sinistro */
  if ((token_iterator->get_type() == TKN_OPERATOR) &&
      !token_iterator->get_value().compare(string(1, LNG_EXPR_BLOCK_START))) {
    /* ritrova l'espressione tra parentesi e interpretala */
    ++token_iterator;

    brace_count = 1;
    debug("expr brace opened");
    while ((brace_count > 0) && (token_iterator != token_list.end())) {
      new_token_list.push_back(*token_iterator);

      if (!token_iterator->get_value().compare(
            string(1, LNG_EXPR_BLOCK_START))) {
        brace_count++;
      } else if (!token_iterator->get_value().compare(
            string(1, LNG_EXPR_BLOCK_END))) {
        /* elimina l'ultima parentesi dalla lista di token creata */
        if (--brace_count == 0) new_token_list.pop_back();
      }

      ++token_iterator;
    }
    debug("expr brace closed");

    if (brace_count != 0)
      throw ParseError("unbalanced parenthesis",
          ((*(--token_iterator)).get_line_no()));

    operand = this->parse_expression(new_token_list);
  } else {
    operand_pred = new PredicateExpr();

    Predicate orig_pred = this->parse_predicate(token_list,
                                                token_iterator,
                                                &read_tokens);

    operand_pred->set_name(orig_pred.get_name());
    operand_pred->set_parameters(orig_pred.get_parameters());

    operand = operand_pred;

    debug("predicate parsing done");
    token_iterator = this->advance_iterator(token_iterator, read_tokens);
    token_iterator++;
    read_tokens = 0;
  }
  if (token_iterator == token_list.end()) return operand;
  new_token_list.clear();

  debug("expr operator recognition");

  /* Riconosci l'operatore */
  if (token_iterator->get_type() != TKN_SYMBOL)
    throw ParseError("missing operator in expression",
        token_iterator->get_line_no());

  /* costruisci la nuova lista in anticipo perche` a meno che l'operatore
   * non sia non valido la dovremo utilizzare obbligatoriamente
   */
  debug("expr copy list tail");
  Token current_op = *token_iterator++;
  if (token_iterator == token_list.end())
    throw ParseError("missing r-value operand in expression",
        (*(--token_iterator)).get_line_no());
  while (token_iterator != token_list.end()) {
    new_token_list.push_back(*token_iterator);
    ++token_iterator;
  }

  switch (RESOLVE_BOOLEAN_OP_FROM_STRING(current_op.get_value())) {
    case BOOLEAN_AND:
      andexpr = new AndBooleanExpr();
      andexpr->set_cond1(operand);
      andexpr->set_cond2(this->parse_expression(new_token_list));

      root = andexpr;
      break;
    case BOOLEAN_OR:
      orexpr = new OrBooleanExpr();
      orexpr->set_cond1(operand);
      orexpr->set_cond2(this->parse_expression(new_token_list));

      root = orexpr;
      break;
    default:
      throw ParseError("invalid operator", current_op.get_line_no());
      break;
  }

  /* se l'espressione a dx non ha variabili spostiamola a sx per migliorare
   * l'efficienza durante la valutazione dell'espressione
   */
  BooleanExpr *bexpr = dynamic_cast<BooleanExpr *>(root);
  if (bexpr && extract_variables(bexpr->get_cond2()).empty()) {
    Expression *tmp = bexpr->get_cond1();
    bexpr->set_cond1(bexpr->get_cond2());
    bexpr->set_cond2(tmp);
  }

  return root;
};

