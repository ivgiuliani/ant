#ifndef ALGORITHMS_HILL_H_
#define ALGORITHMS_HILL_H_

#include "facts.h"
#include "rules.h"
#include "wm.h"
#include "algorithms/common.h"
#include "parser/setblock.h"

unsigned int algo_hill(RuleSet *, Fact *, Fact *, Options *);
SearchNode *hill(RuleSet *, Fact *, Fact *, string, unsigned int, unsigned int *);

#endif /* ALGORITHMS_HILL_H_ */
