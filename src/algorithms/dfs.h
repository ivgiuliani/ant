#ifndef DFS_H
#define DFS_H

#include "facts.h"
#include "rules.h"
#include "wm.h"
#include "algorithms/common.h"
#include "parser/setblock.h"

uint32_t algo_dfs(RuleSet *, Fact *, Fact *, Options *);
SearchNode *dfs(RuleSet *, Fact *, Fact *, unsigned int, unsigned int *);

#endif /* DFS_H */
