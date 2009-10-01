#ifndef ASTAR_H
#define ASTAR_H

#include "facts.h"
#include "rules.h"
#include "values.h"
#include "wm.h"
#include "algorithms/common.h"
#include "parser/setblock.h"

class AStarSearchNode : public SearchNode {
	public:
		int32_t priority;
};

class _AStarSearchNodeComparison
{
	public:
		bool operator() (AStarSearchNode* &left, AStarSearchNode* &right) const {
			return left->priority > right->priority;
		}
};

typedef std::priority_queue<AStarSearchNode *, std::vector<AStarSearchNode *>, _AStarSearchNodeComparison> astar_priqueue;

uint32_t algo_astar(RuleSet *, Fact *, Fact *, Options *);
AStarSearchNode *astar(RuleSet *, Fact *, Fact *, string, int32_t, unsigned int *);

#endif /* ASTAR_H */
