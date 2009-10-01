#include <limits.h>

#include <iomanip>
#include <list>
#include <queue>
#include <set>
#include <stack>
#include <string>

#include "./facts.h"
#include "./heuristic.h"
#include "./rules.h"
#include "./values.h"
#include "./algorithms/astar.h"
#include "./parser/setblock.h"

using ::std::queue;
using ::std::stack;

uint32_t
algo_astar(RuleSet *ruleset, Fact *initial, Fact *final, Options *options) {
  int limit;

  if (options->has("limit")) {
    if (options->get("limit")->get_type() != ITEM_INT)
      throw WorkingMemoryError("missing or invalid 'limit' option");

    stringstream ss;
    ss << options->get("limit")->get_value();
    ss >> limit;
    if (limit == 0)
      throw WorkingMemoryError("missing or invalid 'limit' option");
  } else {
    limit = INT_MAX;
  }

  cout << "Limit: " << options->get("limit")->get_value() << endl;

  if (!options->has("heuristic") ||
      options->get("heuristic")->get_type() != ITEM_STRING)
    throw WorkingMemoryError(
        "no heuristic defined or its value is not valid");

  string heuristic_name = options->get("heuristic")->get_value();
  if (ruleset->heuristics.find(heuristic_name) == ruleset->heuristics.end())
    throw WorkingMemoryError(
        "no heuristic defined or its value is not valid");

  cout << "Using heuristic: " << heuristic_name << endl;

  unsigned int expanded_nodes = 0;
  AStarSearchNode *result = astar(ruleset,
                                  initial,
                                  final,
                                  heuristic_name,
                                  limit,
                                  &expanded_nodes);

  if (!result) {
    cout << "No solution found" << endl;
  } else {
    cout << "Solution found!" << endl;
    cout << "Sequence: " << endl;

    /* la lista di passi torna in ordine inverso visto che partiamo dalle
     * foglie, mettiamo in una pila e mostriamo l'output solo successivamente
     */
    AStarSearchNode *tmp = result;
    stack<string> steps;
    while (tmp->father) {
      steps.push(tmp->applied_rule->get_name());
      tmp = static_cast<AStarSearchNode *>(tmp->father);
    }

    /* ora possiamo mandare in output i passi */
    unsigned int i = 1;
    string rulename;
    while (!steps.empty()) {
      rulename = steps.top();
      steps.pop();

      cout << std::setw(3) << i++ << ". " << rulename << endl;
    }

    delete result;
  }

  return expanded_nodes;
};

AStarSearchNode *
astar(RuleSet *ruleset,
      Fact *initial,
      Fact *final,
      string heuristic,
      int32_t limit,
      unsigned int *expanded_nodes) {
  int result, cost;
  int32_t node_count;

  list<RuleApplication> *applications;
  list<RuleApplication>::iterator app_iterator;

  /* questa corrisponde alla lista Open */
  astar_priqueue nodes, priqueue_tmp;
  queue<SearchNode *> cs_app_result;

  /* corrisponde alla lista Closed, che per noi e` un set di
   * fatti gia` generati
   */
  std::set<string> nodes_hash;
  string hash;

  AStarSearchNode *tmpnode;
  AStarSearchNode *current = new AStarSearchNode();
  current->fact = initial;
  current->applied_rule = NULL;
  current->father = NULL;
  current->depth = 0;

  nodes_hash.insert(current->fact->hash());
  nodes.push(current);

  (*expanded_nodes)++;

  while (!nodes.empty()) {
    current = nodes.top();
    nodes.pop();

    applications = ruleset->get_conflict_set(current->fact);
    cs_app_result = apply_conflict_set(ruleset,
                                       applications,
                                       current,
                                       ++(current->depth));

    while (!cs_app_result.empty()) {
      tmpnode = static_cast<AStarSearchNode *>(cs_app_result.front());
      cs_app_result.pop();

      result = apply_heuristic(tmpnode->fact, final, ruleset, heuristic);

      /* aggiustiamo l'euristica se non e` monotona */
      cost = tmpnode->depth + result;
      if (cost < current->priority)
        cost = current->priority;
      tmpnode->priority = cost;

      /* Se il nodo generato e` uguale al suo genitore non considerarlo */
      if (tmpnode->fact->equals_to(current->fact)) {
        delete tmpnode;
        continue;
      }

      /* abbiamo trovato una soluzione */
      if (tmpnode->fact->equals_to(final))
        return tmpnode;

      /* controlliamo se questo nodo e` gia` stato calcolato, solo se non lo e`
       * lo aggiungiamo alla lista di stati da considerare */
      if (nodes_hash.find(hash) == nodes_hash.end())
        priqueue_tmp.push(tmpnode);
    }

    /* svuotiamo la coda temporanea rispettando il limite imposto */
    node_count = 0;
    while (!priqueue_tmp.empty()) {
      tmpnode = priqueue_tmp.top();
      priqueue_tmp.pop();

      if (node_count++ < limit) {
        nodes_hash.insert(tmpnode->fact->hash());
        nodes.push(tmpnode);
        (*expanded_nodes)++;
      }
    }

    delete applications;
  }

  return NULL;
};
