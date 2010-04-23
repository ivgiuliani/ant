#ifndef FUNCTIONS_H
#define FUNCTIONS_H
/*
 * In questo file e` inserita l'implementazione del marchingegno che
 * permette ad ANT di risolvere i nomi di predicati nel sorgente
 * in vere e proprie funzioni.
 */

#include <map>
#include <string>
#include <vector>

#include "facts.h"
#include "values.h"

using ::std::vector;

/* Funzione per la comparazione delle chiavi del map di tipo const char */
struct __cmp_map_const_char
{
  bool operator()(const char *s1, const char *s2) const {
    return string(s1).compare(string(s2)) < 0;
  }
};

/*
 * Utilizzata per la variable substitution. E' un dizionario che contiene
 * tutte le variabili risolte
 */
typedef map<string, DTArrayItem> FuncMemory;

string inline DUMP_MEMORY(FuncMemory *mem) {
  string output;
  FuncMemory::iterator iter = mem->begin();
  while (iter != mem->end()) {
    output.append("  ");
    output.append(iter->first);
    output.append(": ");
    output.append(iter->second.get_value());
    output.append("\n");

    iter++;
  }
  return output;
}

/* Firma di una funzione che agisce sulla working memory (una qualsiasi
 * di quelle definite sotto)
 */
typedef bool(*__WM_LHS_FUNC)(FuncMemory *, Fact *, vector<PredicateParam>);
typedef void(*__WM_RHS_FUNC)(FuncMemory *, Fact *, vector<PredicateParam>);

typedef map<const char *, __WM_LHS_FUNC, __cmp_map_const_char> __FUNC_LHS_MAPPER;
typedef map<const char *, __WM_RHS_FUNC, __cmp_map_const_char> __FUNC_RHS_MAPPER;
static __FUNC_LHS_MAPPER __FUNC_LHS_MAP;
static __FUNC_RHS_MAPPER __FUNC_RHS_MAP;
void __INIT_LHS_PREDICATE_RESOLVER();
void __INIT_RHS_PREDICATE_RESOLVER();
__WM_LHS_FUNC __ATTR(__NOTHROW, __HOT) RESOLVE_LHS_PREDICATE(string);
__WM_RHS_FUNC __ATTR(__NOTHROW, __HOT) RESOLVE_RHS_PREDICATE(string);

#endif /* FUNCTIONS_H */
