#ifndef PREDICATE_IMPL_H
#define PREDICATE_IMPL_H

#include <sstream>
#include "debug.h"
#include "functions.h"
#include "values.h"
#include "wm.h"

/* ATTENZIONE:
 * se aggiungi una funzione qui, devi aggiungere l'entry corrispondente
 * dentro __INIT_PREDICATE_RESOLVER, altrimenti non verranno riconosciute
 * all'interno dei sorgenti.
 */

/*
 * Definisci degli helper per la definizione delle implementazioni
 * dei predicati
 */
#define PREDICATE_LHS(_FnName, _Funcmem, _Fact, _Params) \
  bool \
  _FnName(FuncMemory* _Funcmem, Fact* _Fact, vector<PredicateParam> _Params)

#define PREDICATE_RHS(_FnName, _Funcmem, _Fact, _Params) \
  void \
  _FnName(FuncMemory* _Funcmem, Fact* _Fact, vector<PredicateParam> _Params)

/* Dichiarazioni delle funzioni per il motore inferenziale.
 * Tutte le funzioni devono avere la stessa identica firma.
 */
PREDICATE_LHS(__fn_equals, funcmem, fact, params);
PREDICATE_LHS(__fn_different, funcmem, fact, params);
PREDICATE_LHS(__fn_gt, funcmem, fact, params);
PREDICATE_LHS(__fn_lt, funcmem, fact, params);
PREDICATE_LHS(__fn_gte, funcmem, fact, params);
PREDICATE_LHS(__fn_lte, funcmem, fact, params);
PREDICATE_LHS(__fn_within, funcmem, fact, params);
PREDICATE_LHS(__fn_true, funcmem, fact, params);
PREDICATE_LHS(__fn_false, funcmem, fact, params);

PREDICATE_RHS(__fn_abs, funcmem, fact, params);
PREDICATE_RHS(__fn_add, funcmem, fact, params);
PREDICATE_RHS(__fn_sub, funcmem, fact, params);
PREDICATE_RHS(__fn_mul, funcmem, fact, params);
PREDICATE_RHS(__fn_define, funcmem, fact, params);
PREDICATE_RHS(__fn_undefine, funcmem, fact, params);
PREDICATE_RHS(__fn_find, funcmem, fact, params);
PREDICATE_RHS(__fn_insert, funcmem, fact, params);
PREDICATE_RHS(__fn_remove, funcmem, fact, params);
PREDICATE_RHS(__fn_edit, funcmem, fact, params);
PREDICATE_RHS(__fn_rhs_equals, funcmem, fact, params);
PREDICATE_RHS(__fn_rhs_different, funcmem, fact, params);
PREDICATE_RHS(__fn_return, funcmem, fact, params);

/* Funzioni interne per i predicati */
PREDICATE_RHS(__fn_find_attribute_substitution, funcmem, fact, params);
PREDICATE_RHS(__fn_find_search_attributes, funcmem, fact, params);

/***********************************
 * Funzioni helper per i predicati *
 ***********************************/

/*
 * Richiedi obbligatoriamente n parametri
 */
void inline
REQUIRE_N_PARAMS(string name, vector<PredicateParam> params, unsigned int n) {
  if (params.size() != n) {
    string err(name);
    err.append(" needs ");
    stringstream ss;
    ss << n;
    err.append(ss.str());
    err.append(" parameters");
    throw PredicateError(err);
  }
};

/*
 * Richiedi obbligatoriamente almeno n parametri
 */
void inline
REQUIRE_ATLEAST_N_PARAMS(string name, vector<PredicateParam> params, unsigned int n) {
  if (params.size() < n) {
    string err(name);
    err.append(" needs ");
    stringstream ss;
    ss << n;
    err.append(ss.str());
    err.append(" parameters");
    throw PredicateError(err);
  }
};

#endif /* PREDICATE_IMPL_LH */
