#include <cstdlib>
#include <sstream>

#include "debug.h"
#include "predicate.h"
#include "predicate_impl.h"
#include "values.h"

/*
 * Qui seguono le implementazioni dei predicati sia per la LHS che per la RHS.
 * Per una descrizione della sintassi si puo` consultare la documentazione allegata
 */

/************************
 * Funzioni di supporto *
 ************************/

/*
 * Risolve una singola variabile
 * Nota: questa funzione e` critica in quanto e` una delle funzioni piu` chiamate
 */
static const void
__ATTR(__HOT)
RESOLVE_VARIABLE(const Fact* fact, FuncMemory* funcmem, PredicateParam *orig) {
  debug(orig->get());
  if (orig->get_type() != PARAM_VAR) return;

  /* salva la posizione dell'oggetto cosi` da non dover ripetere la ricerca
   * subito dopo
   */
  FuncMemory::iterator item = funcmem->find(orig->get());
  if (item == funcmem->end()) {
    string err("unresolved variable '");
    err.append(orig->get());
    err.append("'");
    throw WorkingMemoryError(err);
  }

  switch(item->second.get_type()) {
    case ITEM_INT:
      orig->set_type(PARAM_NUMBER);
      break;
    case ITEM_STRING:
      orig->set_type(PARAM_STRING);
      break;
    default:
      string err("invalid parameter type for ");
      err.append(orig->get());
      err.append(" (value: ");
      err.append((*funcmem)[orig->get()].get_value());
      err.append(")");
      throw WorkingMemoryError(err);
      break;
  }
  orig->set(item->second.get_value());
};

/*
 * Crea un nuovo vettore di parametri i cui elementi di tipo
 * variabile sono gia` risolti a costanti
 */
static const void
RESOLVE_PARAMS(const Fact* fact, FuncMemory* funcmem, vector<PredicateParam> *params) {
  vector<PredicateParam> v;
  unsigned int i;

  for (i = 0; i < params->size(); i++) {
    if ((*params)[i].get_type() == PARAM_VAR) {
      RESOLVE_VARIABLE(fact, funcmem, &(*params)[i]);
    }
  }
};



/************************
 * Predicati per la LHS *
 ************************/

/*
 * Predicato: 'equals'.
 * Sintassi: equals([nomeattributo], valore1, valore2, ..., valoreN)
 */
PREDICATE_LHS(__fn_equals, funcmem, fact, params) {
  REQUIRE_ATLEAST_N_PARAMS("equals", params, 2);
  RESOLVE_PARAMS(fact, funcmem, &params);

  PredicateParam attrname = params[0];
  /* caso del confronto semplice tra due valori */
  if ((params.size() == 2) && (attrname.get_type() == PARAM_STRING && !fact->has_attribute(attrname.get()))) {
    PredicateParam p1 = params[0];
    PredicateParam p2 = params[1];
    return (!p1.get().compare(p2.get()));
  }

  /* caso piu` complesso del confronto tra due array */
  if (attrname.get_type() != PARAM_STRING || !fact->has_attribute(attrname.get()))
    return false;

  /* se gli array non hanno la stessa lunghezza non procediamo con il confronto */
  DTArray *cmpval = fact->get_attribute(attrname.get());
  if (cmpval->length() != (params.size() - 1))
    return false;

  /* confrontiamo gli array */
  bool equals = true;
  unsigned int i;
  for (i = 1; i < params.size() && equals; i++) {
    if (params[i].get().compare((cmpval->at(i - 1))->get_value()))
      equals = false;
  }

  return equals;
};

/*
 * Predicato: 'different'.
 * Sintassi: different([nomeattributo], valore1, valore2, ..., valoreN)
 */
PREDICATE_LHS(__fn_different, funcmem, fact, params) {
  return !__fn_equals(funcmem, fact, params);
};

/*
 * Predicato: 'gt' (Greater Than).
 * Sintassi: gt(attributo1, attributo2)
 */
PREDICATE_LHS(__fn_gt, funcmem, fact, params) {
  REQUIRE_N_PARAMS("gt", params, 2);
  RESOLVE_PARAMS(fact, funcmem, &params);

  PredicateParam p1 = params[0];
  PredicateParam p2 = params[1];

  /* gt si applica solo sui numeri, se cosi` non fosse il
   * predicato non e` valutabile
   */
  if ((p1.get_type() != PARAM_NUMBER) || (p1.get_type() != p2.get_type())) {
    return false;
  }

  stringstream s1, s2;
  int n1, n2;
  s1 << p1.get();
  s2 << p2.get();

  s1 >> n1;
  s2 >> n2;

  return n1 > n2;
};

/*
 * Predicato: 'lt' (Less Than).
 * Sintassi: lt(attributo1, attributo2)
 */
PREDICATE_LHS(__fn_lt, funcmem, fact, params) {
  REQUIRE_N_PARAMS("lt", params, 2);
  RESOLVE_PARAMS(fact, funcmem, &params);

  PredicateParam p1 = params[0];
  PredicateParam p2 = params[1];

  /* lt si applica solo sui numeri, se cosi` non fosse il
   * predicato non e` valutabile
   */
  if ((p1.get_type() != PARAM_NUMBER) || (p1.get_type() != p2.get_type())) {
    return false;
  }

  stringstream s1, s2;
  int n1, n2;
  s1 << p1.get();
  s2 << p2.get();

  s1 >> n1;
  s2 >> n2;

  return n1 < n2;
};

/*
 * Predicato: 'gte' (Greater Than or Equals to).
 * Sintassi: gte(attributo1, attributo2)
 */
PREDICATE_LHS(__fn_gte, funcmem, fact, params) {
  REQUIRE_N_PARAMS("gte", params, 2);
  RESOLVE_PARAMS(fact, funcmem, &params);

  PredicateParam p1 = params[0];
  PredicateParam p2 = params[1];

  /* gte si applica solo sui numeri, se cosi` non fosse il
   * predicato non e` valutabile
   */
  if ((p1.get_type() != PARAM_NUMBER) || (p1.get_type() != p2.get_type())) {
    return false;
  }

  stringstream s1, s2;
  int n1, n2;
  s1 << p1.get();
  s2 << p2.get();

  s1 >> n1;
  s2 >> n2;

  return n1 >= n2;
};

/*
 * Predicato: 'lte'.
 * Sintassi: lte(attributo1, attributo2)
 */
PREDICATE_LHS(__fn_lte, funcmem, fact, params) {
  REQUIRE_N_PARAMS("lte", params, 2);
  RESOLVE_PARAMS(fact, funcmem, &params);

  PredicateParam p1 = params[0];
  PredicateParam p2 = params[1];

  /* lte si applica solo sui numeri, se cosi` non fosse il
   * predicato non e` valutabile
   */
  if ((p1.get_type() != PARAM_NUMBER) || (p1.get_type() != p2.get_type())) {
    return false;
  }

  stringstream s1, s2;
  int n1, n2;
  s1 << p1.get();
  s2 << p2.get();

  s1 >> n1;
  s2 >> n2;

  return n1 <= n2;
};

/*
 * Predicato: 'within'
 * Sintassi: within(attributo, valoremin, valoremax)
 */
PREDICATE_LHS(__fn_within, funcmem, fact, params) {
  REQUIRE_N_PARAMS("within", params, 3);
  RESOLVE_PARAMS(fact, funcmem, &params);

  PredicateParam cmp = params[0];
  PredicateParam min = params[1];
  PredicateParam max = params[2];

  if ((cmp.get_type() != PARAM_NUMBER) ||
      (min.get_type() != PARAM_NUMBER) ||
      (max.get_type() != PARAM_NUMBER)) return false;

  stringstream s_cmp, s_min, s_max;
  int n_cmp, n_min, n_max;

  s_cmp << cmp.get();
  s_min << min.get();
  s_max << max.get();
  s_cmp >> n_cmp;
  s_min >> n_min;
  s_max >> n_max;

  if (n_cmp >= n_min && n_cmp <= n_max) return true;
  return false;
};

/* Predicato: 'true'
 * Sintassi: true(...)
 */
PREDICATE_LHS(__fn_true, funcmem, fact, params) {
  return true;
};

/* Predicato: 'false'
 * Sintassi: true(...)
 */
PREDICATE_LHS(__fn_false, funcmem, fact, params) {
  return false;
};

/************************
 * Predicati per la RHS *
 ************************/

/* Notare che i predicati qui implementati POSSONO modificare sia la FuncMemory
 * che il fatto, percui all'interno del sorgente l'ordine conta
 */

/* Predicato: 'abs'.
 * Sintassi: abs(valore, nuovo_valore)
 */
PREDICATE_RHS(__fn_abs, funcmem, fact, params) {
  REQUIRE_N_PARAMS("abs", params, 2);

  int tmpnum;
  PredicateParam p1 = params[0];
  PredicateParam p2 = params[1];

  if (p2.get_type() != PARAM_VAR)
    throw PredicateError("'abs' needs to save the value in a variable", funcmem, fact);

  if (p1.get_type() == PARAM_VAR)
    RESOLVE_VARIABLE(fact, funcmem, &p1);

  if (p1.get_type() == PARAM_STRING) {
    if (!fact->has_attribute(p1.get()))
      throw PredicateError("'abs' first value is not a valid fact attribute", funcmem, fact);
    if (fact->get_attribute(p1.get())->length() != 1 || fact->get_attribute(p1.get())->at(0)->get_type() != ITEM_INT)
      throw PredicateError("'abs' first value is a string or an array (needs a number)", funcmem, fact);

    switch(fact->get_attribute(p1.get())->at(0)->get_type()) {
      case ITEM_INT:
        p1.set_type(PARAM_NUMBER);
        break;
      case ITEM_STRING:
        p1.set_type(PARAM_STRING);
        break;
      default:
        throw PredicateError("invalid attribute type on 'add'", funcmem, fact);
        break;
    }

    p1.set(fact->get_attribute(p1.get())->at(0)->get_value());
  }

  stringstream ss;
  ss << p1.get();
  ss >> tmpnum;
  (*funcmem)[p2.get()] = abs(tmpnum);
};

/*
 * Predicato: 'add'.
 * Sintassi: add(attributo, valore)
 */
PREDICATE_RHS(__fn_add, funcmem, fact, params) {
  REQUIRE_N_PARAMS("add", params, 2);
  int tmpnum1, tmpnum2;

  PredicateParam p1 = params[0];
  PredicateParam p2 = params[1];

  if (p2.get_type() == PARAM_VAR)
    RESOLVE_VARIABLE(fact, funcmem, &p2);

  if (p2.get_type() != PARAM_NUMBER) {
    string err("'add' requires a number as second parameter (found '");
    err.append(p2.get());
    err.append("')");
    throw PredicateError(err, funcmem, fact);
  }

  stringstream adder;
  adder << p2.get();
  adder >> tmpnum2;

  /* se non e` una variabile controlliamo che sia un attributo del fatto */
  if (funcmem->find(p1.get()) == funcmem->end()) {
    if (!fact->has_attribute(p1.get())) {
      string err("'add' cannot find attribute or variable named ");
      err.append(p1.get());
      throw PredicateError(err, funcmem, fact);
    }

    DTArray *tmp = fact->get_attribute(p1.get());
    if (tmp->length() != 1)
      throw PredicateError("'add' cannot operate on arrays", funcmem, fact);

    if (tmp->at(0)->get_type() != ITEM_INT) {
      string err("'add' requires values \"");
      err.append(tmp->at(0)->get_value());
      err.append("\" to be a number");
      throw PredicateError(err, funcmem, fact);
    }

    stringstream ss;
    ss << tmp->at(0)->get_value();
    ss >> tmpnum1;
    tmp->at(0)->set_value(tmpnum1 + tmpnum2);
  } else {
    /* altrimenti incrementiamo la variabile se del tipo giusto */
    if (funcmem->find(p1.get())->second.get_type() != ITEM_INT)
      throw PredicateError("'add' requires first parameter to be a numeric value", funcmem, fact);

    stringstream ss;
    ss << (*funcmem)[p1.get()].get_value();
    ss >> tmpnum1;
    
    (*funcmem)[p1.get()].set_value(tmpnum1 + tmpnum2);
  }
};

/*
 * Predicato: 'sub'.
 * Sintassi: sub(attributo, valore)
 */
PREDICATE_RHS(__fn_sub, funcmem, fact, params) {
  REQUIRE_N_PARAMS("sub", params, 2);
  int tmpnum1, tmpnum2;

  PredicateParam p1 = params[0];
  PredicateParam p2 = params[1];

  if (p2.get_type() != PARAM_NUMBER)
    throw PredicateError("'sub' requires a number as second parameter", funcmem, fact);

  stringstream subber;
  subber << p2.get();
  subber >> tmpnum2;

  /* se non e` una variabile controlliamo che sia un attributo del fatto */
  if (funcmem->find(p1.get()) == funcmem->end()) {
    if (!fact->has_attribute(p1.get())) {
      string err("'sub' cannot find attribute or variable named ");
      err.append(p1.get());
      throw PredicateError(err, funcmem, fact);
    }

    DTArray *tmp = fact->get_attribute(p1.get());
    if (tmp->length() != 1)
      throw PredicateError("'sub' cannot operate on arrays", funcmem, fact);

    if (tmp->at(0)->get_type() != ITEM_INT) {
      string err("'sub' requires values \"");
      err.append(tmp->at(0)->get_value());
      err.append("\" to be a number");
      throw PredicateError(err, funcmem, fact);
    }

    stringstream ss;
    ss << tmp->at(0)->get_value();
    ss >> tmpnum1;
    tmp->at(0)->set_value(tmpnum1 - tmpnum2);
  } else {
    /* altrimenti decrementiamo la variabile se del tipo giusto */
    if (funcmem->find(p1.get())->second.get_type() != ITEM_INT)
      throw PredicateError("'sub' requires first parameter to be a numeric value", funcmem, fact);

    stringstream ss;
    ss << (*funcmem)[p1.get()].get_value();
    ss >> tmpnum1;
    
    (*funcmem)[p1.get()].set_value(tmpnum1 - tmpnum2);
  }
};

/*
 * Predicato: 'mul'.
 * Sintassi: mul(attributo, valore)
 */
PREDICATE_RHS(__fn_mul, funcmem, fact, params) {
  REQUIRE_N_PARAMS("mul", params, 2);
  int tmpnum1, tmpnum2;

  PredicateParam p1 = params[0];
  PredicateParam p2 = params[1];

  if (p2.get_type() != PARAM_NUMBER)
    throw PredicateError("'mul' requires a number as second parameter", funcmem, fact);

  stringstream subber;
  subber << p2.get();
  subber >> tmpnum2;

  /* se non e` una variabile controlliamo che sia un attributo del fatto */
  if (funcmem->find(p1.get()) == funcmem->end()) {
    if (!fact->has_attribute(p1.get())) {
      string err("'mul' cannot find attribute or variable named ");
      err.append(p1.get());
      throw PredicateError(err, funcmem, fact);
    }

    DTArray *tmp = fact->get_attribute(p1.get());
    if (tmp->length() != 1)
      throw PredicateError("'mul' cannot operate on arrays", funcmem, fact);

    if (tmp->at(0)->get_type() != ITEM_INT) {
      string err("'mul' requires values \"");
      err.append(tmp->at(0)->get_value());
      err.append("\" to be a number");
      throw PredicateError(err, funcmem, fact);
    }

    stringstream ss;
    ss << tmp->at(0)->get_value();
    ss >> tmpnum1;
    tmp->at(0)->set_value(tmpnum1 - tmpnum2);
  } else {
    /* altrimenti decrementiamo la variabile se del tipo giusto */
    if (funcmem->find(p1.get())->second.get_type() != ITEM_INT)
      throw PredicateError("'mul' requires first parameter to be a numeric value", funcmem, fact);

    stringstream ss;
    ss << (*funcmem)[p1.get()].get_value();
    ss >> tmpnum1;
    
    (*funcmem)[p1.get()].set_value(tmpnum1 * tmpnum2);
  }
};


/*
 * Predicato: 'define'.
 * Sintassi: define(variabile, valore1, valore2, ..., valoreN)
 */
PREDICATE_RHS(__fn_define, funcmem, fact, params) {
  REQUIRE_N_PARAMS("define", params, 2);

  PredicateParam varname = params[0];
  PredicateParam value = params[1];

  if (value.get_type() == PARAM_VAR)
    RESOLVE_VARIABLE(fact, funcmem, &value);

  /* se il valore e` il nome di un attributo, allora risolvilo */
  if ((value.get_type() == PARAM_STRING) && (fact->has_attribute(value.get()))) {
    if (fact->get_attribute(value.get())->length() > 1)
      throw PredicateError("'define' cannot operates on array attributes", funcmem, fact);
    switch(fact->get_attribute(value.get())->at(0)->get_type()) {
      case ITEM_INT:
        value.set_type(PARAM_NUMBER);
        break;
      case ITEM_STRING:
        value.set_type(PARAM_STRING);
        break;
      default:
        throw PredicateError("invalid attribute type on 'define'", funcmem, fact);
        break;
    }

    value.set(fact->get_attribute(value.get())->at(0)->get_value());
  }

  
  switch(value.get_type()) {
    case PARAM_NUMBER:
      (*funcmem)[varname.get()].set_value(ITEM_INT, value.get());
      break;
    case PARAM_STRING:
      (*funcmem)[varname.get()].set_value(ITEM_STRING, value.get());
      break;
    default:
      throw PredicateError("'define' needs second parameter to be constant", funcmem, fact);
      break;
  }
};

/*
 * Predicato: 'undefine'
 * Sintassi: undefine(variabile1, variabile2, ..., variabileN)
 */
PREDICATE_RHS(__fn_undefine, funcmem, fact, params) {
  REQUIRE_ATLEAST_N_PARAMS("undefine", params, 1);
  FuncMemory::iterator item;

  /* se la variabile non esiste ignorala */
  for (unsigned int i = 0; i < params.size(); i++) {
    if (params[i].get_type() != PARAM_VAR)
      throw PredicateError("'undefine' requires only variable values");
    item = funcmem->find(params[i].get());

    if (item != funcmem->end())
      funcmem->erase(item);
  }
};


/*
 * Predicato: 'find'.
 * Sintassi: find(attributo, valore1, valore2, ..., valoreN)
 *
 * Questo predicato e` abbastanza particolare in quanto effettua 
 * un approximate search degli attributi del fatto, ma si ferma
 * al primo che soddisfa le condizioni specificate.
 */
PREDICATE_RHS(__fn_find, funcmem, fact, params) {
  REQUIRE_ATLEAST_N_PARAMS("find", params, 2);
  unsigned int i;

  /* risolvi tutte le variabili precedentemente risolte e lascia irrisolte
   * quelle che dobbiamo trovare
   */
  vector<PredicateParam> cmp;
  for (i = 0; i < params.size(); i++) {
    if (params[i].get_type() != PARAM_VAR) {
      cmp.push_back(params[i]);
      continue;
    }

    try {
      RESOLVE_VARIABLE(fact, funcmem, &params[i]);
      cmp.push_back(params[i]);
    } catch (WorkingMemoryError) {
      /* variabile irrisolta, ignoriamo l'errore e
       * riaggiungiamo il parametro alla lista
       */
      cmp.push_back(params[i]);
    }
  }

  if (params[0].get_type() == PARAM_STRING)
    __fn_find_attribute_substitution(funcmem, fact, cmp);
  else __fn_find_search_attributes(funcmem, fact, cmp);
};

/* Usato da 'find': assume che il primo parametro sia una costante */
PREDICATE_RHS(__fn_find_attribute_substitution, funcmem, fact, params) {
  unsigned int i;
  PredicateParam attrname = params[0];

  if (!fact->has_attribute(attrname.get())) {
    string err("'find' cannot find the specified attribute '");
    err.append(attrname.get());
    err.append("'");
    throw PredicateError(err, funcmem, fact);
  }

  unsigned int val_no = params.size();
  DTArray *array = fact->get_attribute(attrname.get());

  if ((val_no - 1) != array->length()) {
    string err("'find' cannot find the specified attribute '");
    err.append(attrname.get());
    err.append("' (number of params doesn't match)");
    throw PredicateError(err, funcmem, fact);
  }

  /* tutto e` andato bene, sostituiamo le variabili rimaste */
  for (i = 1; i < val_no; i++) {
    if (params[i].get_type() != PARAM_VAR) continue;
    (*funcmem)[params[i].get()].set_value(array->at(i - 1)->get_type(), array->at(i - 1)->get_value());
  }
};

/* Usato da 'find': implementa l'approximate search */
PREDICATE_RHS(__fn_find_search_attributes, funcmem, fact, params) {
  unsigned int val_no = params.size();
  unsigned int i;
  attrmap::iterator attr = fact->get_attribute_list()->begin();

  bool equals = false;
  while (attr != fact->get_attribute_list()->end() && !equals) {
    if (attr->second.length() != val_no - 1) { ++attr; continue; }

    equals = true;
    for (i = 1; i < val_no && equals; i++) {
      if (params[i].get_type() == PARAM_VAR) continue;
      if (params[i].get().compare(attr->second.at(i - 1)->get_value())) {
        equals = false;
      }
    }
    if (equals) break;

    ++attr;
  }

  /* abbiamo trovato un match */
  if (equals) {
    /* sappiamo che il nome dell'attributo e` una variabile perche` altrimenti
     * avremmo usato la prima euristica
     */
    (*funcmem)[params[0].get()].set_value(ITEM_STRING, attr->first);

    DTArray *array = fact->get_attribute(attr->first);
    for (i = 1; i < val_no; i++) {
      if (params[i].get_type() != PARAM_VAR) continue;
      (*funcmem)[params[i].get()] = (*array)[i - 1];
    }
  } else {
    string err("'find' cannot find the specified attribute '");
    err.append(params[0].get());
    err.append("'");
    throw PredicateError(err, funcmem, fact);
  }
};


/*
 * Predicato: 'insert'.
 * Sintassi: insert(attributo, valore)
 */
PREDICATE_RHS(__fn_insert, funcmem, fact, params) {
  REQUIRE_ATLEAST_N_PARAMS("insert", params, 2);
  RESOLVE_PARAMS(fact, funcmem, &params);

  PredicateParam attrname = params[0];

  DTArray value(params.size() - 1);

  unsigned int i;
  for (i = 1; i < params.size(); i++) {
    switch(params[i].get_type()) {
      case PARAM_NUMBER:
        value[i - 1].set_value(ITEM_INT, params[i].get());
        break;
      case PARAM_STRING:
        value[i - 1].set_value(ITEM_STRING, params[i].get());
        break;
      default:
        throw PredicateError("'insert' needs constant values", funcmem, fact);
        break;
    }
  }

  fact->add_attribute(attrname.get(), value);
};


/*
 * Predicato: 'remove'.
 * Sintassi: remove(attributo, valore)
 */
PREDICATE_RHS(__fn_remove, funcmem, fact, params) {
  REQUIRE_N_PARAMS("remove", params, 1);
  RESOLVE_PARAMS(fact, funcmem, &params);

  string name = params[0].get();

  if (!fact->has_attribute(name)) {
    string err("'remove' cannot find attribute named ");
    err.append(name);
    throw PredicateError(err, funcmem, fact);
  }

  fact->del_attribute(name);
};


/*
 * Predicato: 'edit'.
 * Sintassi: edit(attributo, valore)
 */
PREDICATE_RHS(__fn_edit, funcmem, fact, params) {
  REQUIRE_ATLEAST_N_PARAMS("edit", params, 2);
  RESOLVE_PARAMS(fact, funcmem, &params);

  vector<PredicateParam> remove_params(1);
  remove_params[0] = params[0];

  __fn_remove(funcmem, fact, remove_params);
  __fn_insert(funcmem, fact, params);
};

/* Predicato: 'equals'.
 * Sintassi: equals([nomeattributo], valore1, valore2, ..., valoreN, risultato)
 */
PREDICATE_RHS(__fn_rhs_equals, funcmem, fact, params) {
  REQUIRE_ATLEAST_N_PARAMS("equals", params, 3);

  PredicateParam save_into = params[params.size() - 1];
  if (save_into.get_type() != PARAM_VAR)
    throw PredicateError("'equals' cannot save result into a constant value", funcmem, fact);

  vector<PredicateParam> tmpparams;
  for (unsigned int i = 0; i < (params.size() - 1); i++)
    tmpparams.push_back(params[i]);

  bool res = __fn_equals(funcmem, fact, tmpparams);
  (*funcmem)[save_into.get()].set_value(ITEM_INT, res ? "1" : "0");
};


/* Predicato: 'different'.
 * Sintassi: different([nomeattributo], valore1, valore2, ..., valoreN, risultato)
 */
PREDICATE_RHS(__fn_rhs_different, funcmem, fact, params) {
  REQUIRE_ATLEAST_N_PARAMS("different", params, 3);

  PredicateParam save_into = params[params.size() - 1];
  if (save_into.get_type() != PARAM_VAR)
    throw PredicateError("'different' cannot save result into a constant value", funcmem, fact);

  vector<PredicateParam> tmpparams;
  for (unsigned int i = 0; i < (params.size() - 1); i++)
    tmpparams.push_back(params[i]);

  bool res = __fn_different(funcmem, fact, tmpparams);
  (*funcmem)[save_into.get()].set_value(ITEM_INT, res ? "1" : "0");
};

/*
 * Predicato: 'return'.
 * Sintassi: return(valore);
 */
PREDICATE_RHS(__fn_return, funcmem, fact, params) {
  REQUIRE_N_PARAMS("return", params, 1);
  RESOLVE_PARAMS(fact, funcmem, &params);
  PredicateParam value = params[0];

  if (value.get_type() != PARAM_NUMBER)
    throw PredicateError("'return' accepts only numerical values", funcmem, fact);
  (*funcmem)["?"].set_value(ITEM_INT, value.get());
};
