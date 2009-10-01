#ifndef COMMON_H
#define COMMON_H

#include <list>
#include <queue>

#include "facts.h"
#include "rules.h"

/* Un nodo all'interno dell'albero di ricerca */
class SearchNode {
	public:
		SearchNode *father;
		Rule *applied_rule;
		Fact *fact;
		uint32_t depth;
};

std::queue<SearchNode *>
apply_conflict_set(RuleSet *, std::list<RuleApplication> *, SearchNode *, unsigned int);

#endif /* COMMON_H */
