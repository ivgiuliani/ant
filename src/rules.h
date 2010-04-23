#ifndef RULES_H
#define RULES_H

#include <list>
#include <queue>
#include <stdint.h>

#include "debug.h"
#include "expression.h"
#include "facts.h"
#include "predicate.h"
#include "variables.h"

/* Una singola regola */
class Rule {
  private:
    string name;
    /* La parte sinistra della regola non e` altro che un expression tree */
    Expression *lhs;
    list<PredicateCall> rhs;
  
  public:
    /* E' una cache sulle variabili che sono presenti all'interno
     * della regola. Punta a NULL inizialmente, ma quando viene calcolata
     * bisogna far puntare il ptr al posto giusto, successivamente
     * si puo` riutilizzare durante l'unificazione
     */
    vector<string> *extracted_variables;

    Rule() {
      this->lhs = NULL;
      this->extracted_variables = NULL;
    };

    Rule(string name) {
      this->lhs = NULL;
      this->name.assign(name);
      this->extracted_variables = NULL;
    };

    void set_name(string name) { this->name.assign(name); };
    string get_name() { return this->name; }

    void set_lhs(Expression *exprtree) { this->lhs = exprtree; };
    void set_rhs(list<PredicateCall> rhs) { this->rhs = rhs; };
    Expression *get_lhs() { return this->lhs; };
    list<PredicateCall> get_rhs() { return this->rhs; };
};

/* Non e` altro che l'associazione di un set di variabili
 * gia` risolte ad una espressione
 */
class RuleApplication {
  public:
    Rule *rule;
    FuncMemory funcmem;
};

class RuleSet {
  private:
    list<Rule *> rules;

  public:
    map<string, list<PredicateCall> > heuristics;

    ~RuleSet() {
      list<Rule *>::iterator it = this->rules.begin();
      while (it != this->rules.end()) {
        delete *it;
      }
    };

    void add(Rule *);
    void add_heuristic(string, list<PredicateCall>);

    list<RuleApplication> *get_conflict_set(Fact *);
    vector<DTArrayItem> explode_var_values(Fact *);
    list<RuleApplication> solve_search_tree(vector<string> *, vector<DTArrayItem> *, Rule *, Fact *);

    /* possibile implementazione futura: ricerca delle regole
     * applicabili a una lista di fatti
     * list<Rule> get_conflict_set(list<Fact>);
     */
    uint32_t rules_number();

    void apply(RuleApplication *, Fact *);
};

void inline
__ATTR(__INLINE)
increment_idxs(vector<unsigned int> *, unsigned int, unsigned int, vector<Variable> *, vector<DTArrayItem> *);

#endif /* RULES_H */
