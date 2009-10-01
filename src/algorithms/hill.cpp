#include <limits.h>

#include <cassert>
#include <iomanip>
#include <list>
#include <queue>
#include <set>
#include <stack>
#include <string>

#include "./debug.h"
#include "./facts.h"
#include "./heuristic.h"
#include "./rules.h"
#include "./algorithms/hill.h"
#include "./algorithms/common.h"
#include "./parser/setblock.h"

using ::std::cout;
using ::std::endl;

using ::std::queue;
using ::std::stack;

/*
 * Inizializza e chiama l'algoritmo di Hill Climbing sulla working memory
 */
unsigned int algo_hill(RuleSet *ruleset,
                       Fact *initial,
                       Fact *final,
                       Options *options) {
  if (options->get("limit")->get_type() != ITEM_INT)
    throw WorkingMemoryError("missing or invalid 'limit' option");

  stringstream ss;
  ss << options->get("limit")->get_value();
  unsigned int limit;
  ss >> limit;
  if (limit == 0)
    throw WorkingMemoryError("missing or invalid 'limit' option");

  cout << "Limit: " << options->get("limit")->get_value() << endl;

  if (!options->has("heuristic") ||
      options->get("heuristic")->get_type() != ITEM_STRING)
    throw WorkingMemoryError(
        "no heuristic defined or its value is not valid");

  string heuristic_name = options->get("heuristic")->get_value();
  if (ruleset->heuristics.find(heuristic_name) == ruleset->heuristics.end())
    throw WorkingMemoryError(
        "no heuristic defined or its value is not valid");

  unsigned int expanded_nodes = 0;
  SearchNode *result = hill(ruleset, initial, final,
                            heuristic_name, limit, &expanded_nodes);

  if (!result) {
    cout << "No solution found" << endl;
  } else {
    cout << "Solution found!" << endl;
    cout << "Sequence: " << endl;

    /* la lista di passi torna in ordine inverso visto che partiamo dalle
     * foglie, mettiamo in una pila e mostriamo l'output solo successivamente
     */
    SearchNode *tmp = result;
    stack<string> steps;
    while (tmp->father) {
      steps.push(tmp->applied_rule->get_name());
      tmp = tmp->father;
    }

    /* ora possiamo mandare in output i passi */
    unsigned int i = 1;
    string rulename;
    while (!steps.empty()) {
      rulename = steps.top();
      steps.pop();

      cout << std::setw(3) << i++ << ". " << rulename << endl;
    }
  }

  return expanded_nodes;
};

/*
 * Implementazione vera e propria dell'algoritmo di Hill Climbing
 */
SearchNode *
hill(RuleSet *ruleset,
     Fact *initial,
     Fact *final,
     string heuristic,
     unsigned int depth_limit,
     unsigned int *expanded_nodes) {
  int result, min;
  list<RuleApplication> *applications;
  list<RuleApplication>::iterator app_iterator;

  queue<SearchNode *> cs_app_result;
  SearchNode *currnode, *tmpnode, *newnode;

  std::set<string> nodes_hash;
  string hash;

  assert(initial->equals_to(initial));

  SearchNode *root = new SearchNode();
  root->father = NULL;
  root->applied_rule = NULL;
  root->fact = initial;
  root->depth = 0;

  if (initial->equals_to(final)) return root;
  nodes_hash.insert(root->fact->hash());
  (*expanded_nodes)++;

  currnode = root;
  unsigned int depth = 0;
  while (depth < depth_limit) {
    applications = ruleset->get_conflict_set(currnode->fact);
    cs_app_result = apply_conflict_set(ruleset, applications,
                                       currnode, ++depth);

    depth = currnode->depth;
    applications = ruleset->get_conflict_set(currnode->fact);

    /* se non ci sono cammini da qui in poi abbandoniamo il processo */
    if (applications->empty()) {
      delete currnode;
      delete applications;
      return NULL;
    }

    /* Troviamo il massimo locale (tra i figli, quello con il minor costo
     * ottenuto dalla funzione euristica) e procediamo per quella strada
     */
    newnode = NULL;
    min = INT_MAX;
    while (!cs_app_result.empty()) {
      tmpnode = cs_app_result.front();
      cs_app_result.pop();

      /* abbiamo trovato una soluzione */
      if (tmpnode->fact->equals_to(final))
        return tmpnode;

      /* ignoriamo il nodo se e` uguale al suo genitore... */
      if (tmpnode->fact->equals_to(tmpnode->father->fact)) {
        delete tmpnode;
        continue;
      }

      /* ..o se e` un nodo che abbiamo gia` considerato */
      hash = tmpnode->fact->hash();
      if (nodes_hash.find(hash) != nodes_hash.end()) {
        delete tmpnode;
        continue;
      }

      /* applichiamo l'euristica e controlliamo se e` o meno un massimo */
      result = apply_heuristic(tmpnode->fact, final, ruleset, heuristic);
      if (result > min) continue;

      /* e` un massimo locale, sostituiamolo con il precedente */
      newnode = tmpnode;
      min = result;
    }

    /* se i nodi generati erano stati gia` tutti generati in precedenza, allora
     * non siamo riusciti a trovare una soluzione
     */
    if (!newnode) {
      delete applications;
      return NULL;
    }
    currnode = newnode;

    /* aggiungiamo il nuovo nodo alla lista di fatti gia` considerati */
    nodes_hash.insert(currnode->fact->hash());
    (*expanded_nodes)++;

    delete applications;
  }

  return NULL;
};

