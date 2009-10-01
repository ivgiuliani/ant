#include <list>
#include <queue>

#include "algorithms/common.h"

using ::std::queue;

/*
 * Applica tutte le regole generate da un conflict set a un fatto
 * Ricordarsi di liberare la memoria occupata dai fatti tornati in output
 */
queue<SearchNode *>
apply_conflict_set(RuleSet *ruleset, list<RuleApplication> *applications, SearchNode *father, unsigned int depth) {
	list<RuleApplication>::iterator application = applications->begin();
	queue<SearchNode *> search_queue;
	SearchNode *searchnode;

	while (application != applications->end()) {
		Fact *newfact = new Fact(*father->fact);

		ruleset->apply(&(*application), newfact);

		searchnode = new SearchNode();
		searchnode->father = father;
		searchnode->applied_rule = application->rule;
		searchnode->fact = newfact;
		searchnode->depth = depth;

		search_queue.push(searchnode);
		application++;
	}

	return search_queue;
};

