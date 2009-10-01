#include <queue>
#include <set>
#include <vector>

#include "debug.h"
#include "predicate.h"
#include "values.h"
#include "variables.h"

using ::std::string;
using ::std::queue;

queue<Predicate> _extract_predicate(Expression *);
queue<string> _extract_variables(Predicate);

/*
 * Estrae una lista di variabili da un espressione
 */
vector<string>
extract_variables(Expression *expression) {
  /* estrai tutti i predicati dall'albero di espressione */
  queue<Predicate> predicates = _extract_predicate(expression);
  queue<string> pred_varlist;
  std::set<string> vars;

  /* estrae le variabili da ogni predicato in coda */
  while (!predicates.empty()) {
    Predicate p = predicates.front();

    pred_varlist = _extract_variables(p);
    while (!pred_varlist.empty()) {
      vars.insert(pred_varlist.front());
      pred_varlist.pop();
    }

    predicates.pop();
  }

  /* Converti il set in un vettore di variabili */
  vector<string> retlist;
  std::set<string>::iterator it = vars.begin();
  while (it != vars.end()) {
    debug(*it);
    retlist.push_back(*it); 
    it++;
  }

  return retlist;
};

/*
 * Estrare ricorsivamente i predicati dall'albero attraverso
 * una visita in post-ordine
 */
queue<Predicate>
_extract_predicate(Expression *expr) {
  queue<Predicate> q, tmp;

  BooleanExpr *b = dynamic_cast<BooleanExpr *>(expr);
  if (b) {
    tmp = _extract_predicate(b->get_cond1());
    while (!tmp.empty()) {
      q.push(tmp.front());
      tmp.pop();
    }

    tmp = _extract_predicate(b->get_cond2());
    while (!tmp.empty()) {
      q.push(tmp.front());
      tmp.pop();
    }
  };

  PredicateExpr *p = dynamic_cast<PredicateExpr *>(expr);
  if (p) {
    debug(p->get_name());
    q.push(*p);
  } 

  return q;
};

/* Estra i nomi delle variabili da un predicato */
queue<string>
_extract_variables(Predicate p) {
  vector<PredicateParam> parameters = p.get_parameters();
  queue<string> varnames;
  unsigned int i;

  for (i = 0; i < parameters.size(); i++) {
    if (parameters[i].get_type() == PARAM_VAR)
      varnames.push(parameters[i].get());
  }

  return varnames;
};
