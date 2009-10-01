#ifndef PREDICATE_H
#define PREDICATE_H

#include <string>
#include <vector>

#include "compiler.h"
#include "facts.h"
#include "expression.h"
#include "values.h"

class PredicateError {
  public:
    string err;
    PredicateError(string err) { this->err = err; };
    PredicateError(string err, FuncMemory *mem) {
      this->err = err;
      this->err.append("\nMEMORY DUMP:\n");
      this->err.append(DUMP_MEMORY(mem));
    };
    PredicateError(string err, FuncMemory *mem, Fact *fact) {
      this->err = err;
      this->err.append("\nMEMORY DUMP:\n");
      this->err.append(DUMP_MEMORY(mem));

      this->err.append("\nFACT DUMP:\n");
      this->err.append(DUMP_FACT(fact));
    };
};

/*
 * Un predicato e` una chiamata a una funzione che accetta
 * un numero variabile di parametri
 */
class Predicate {
  protected:
    string name;
    vector<PredicateParam> parameters;
  public:
    Predicate() {};
    Predicate(string);
    /* TODO: bisogna gestire diversi tipi di parametri:
     * Stringhe, numeri e variabili
     */
    void add_parameter(PredicateParam);
    vector<PredicateParam> __ATTR(__FASTCALL) get_parameters();
    void set_parameters(vector<PredicateParam>);
    void set_name(string);
    string get_name();
};

/*
 * Un predicato che modifica la working memory.
 */
class PredicateCall : public Predicate {
  public:
    PredicateCall() {};
    PredicateCall(string name) { this->set_name(name); };
};

/*
 * Rappresenta una chiamata a un predicato in una expression.
 * Predicati di questo genere implementano la evaluate() e
 * ritornano vero o falso.
 */
class PredicateExpr : public Predicate, public Expression {
  protected:
    __WM_LHS_FUNC cache_predicate_func;
  public:
    PredicateExpr() { this->cache_predicate_func = NULL; };
    PredicateExpr(string name) {
      this->set_name(name);
      this->cache_predicate_func = NULL;
    };
    bool evaluate(FuncMemory *, Fact *);
};

#endif /* PREDICATE_H */
