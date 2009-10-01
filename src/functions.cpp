#include <map>
#include <vector>

#include "functions.h"
#include "predicate.h"
#include "predicate_impl.h"
#include "values.h"

void
__INIT_LHS_PREDICATE_RESOLVER() {
  extern __FUNC_LHS_MAPPER __FUNC_LHS_MAP;
  __FUNC_LHS_MAP["equals"]    = __fn_equals;
  __FUNC_LHS_MAP["different"] = __fn_different;
  __FUNC_LHS_MAP["gt"]        = __fn_gt;
  __FUNC_LHS_MAP["lt"]        = __fn_lt;
  __FUNC_LHS_MAP["gte"]       = __fn_gte;
  __FUNC_LHS_MAP["lte"]       = __fn_lte;
  __FUNC_LHS_MAP["within"]    = __fn_within;
  __FUNC_LHS_MAP["true"]      = __fn_true;
  __FUNC_LHS_MAP["false"]     = __fn_false;
};

void
__INIT_RHS_PREDICATE_RESOLVER() {
  extern __FUNC_RHS_MAPPER __FUNC_RHS_MAP;
  __FUNC_RHS_MAP["abs"]       = __fn_abs;
  __FUNC_RHS_MAP["add"]       = __fn_add;
  __FUNC_RHS_MAP["sub"]       = __fn_sub;
  __FUNC_RHS_MAP["mul"]       = __fn_mul;
  __FUNC_RHS_MAP["define"]    = __fn_define;
  __FUNC_RHS_MAP["undefine"]  = __fn_undefine;
  __FUNC_RHS_MAP["find"]      = __fn_find;
  __FUNC_RHS_MAP["insert"]    = __fn_insert;
  __FUNC_RHS_MAP["remove"]    = __fn_remove;
  __FUNC_RHS_MAP["edit"]      = __fn_edit;
  __FUNC_RHS_MAP["equals"]    = __fn_rhs_equals;
  __FUNC_RHS_MAP["different"] = __fn_rhs_different;
  __FUNC_RHS_MAP["return"]    = __fn_return;
};

/*
 * Dato il nome di un predicato della LHS, lo risolve nel puntatore
 * alla funzione desiderata. Ritorna NULL se la funzione non esiste.
 */
__WM_LHS_FUNC
RESOLVE_LHS_PREDICATE(string name) {
  extern __FUNC_LHS_MAPPER __FUNC_LHS_MAP;
  return __FUNC_LHS_MAP[name.c_str()];
};

/*
 * Dato il nome di un predicato della RHS, lo risolve nel puntatore
 * alla funzione desiderata. Ritorna NULL se la funzione non esiste.
 */
__WM_RHS_FUNC
RESOLVE_RHS_PREDICATE(string name) {
  extern __FUNC_RHS_MAPPER __FUNC_RHS_MAP;
  return __FUNC_RHS_MAP[name.c_str()];
};

