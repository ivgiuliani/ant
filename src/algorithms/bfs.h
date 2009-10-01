#ifndef BFS_H
#define BFS_H

#include "facts.h"
#include "rules.h"
#include "wm.h"
#include "algorithms/common.h"
#include "parser/setblock.h"

uint32_t algo_bfs(RuleSet *, Fact *, Fact *, Options *);
SearchNode *bfs(RuleSet *, Fact *, Fact *, unsigned int, unsigned int *);

#endif /* BFS_H */
