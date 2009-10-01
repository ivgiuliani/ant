#ifndef HEURISTIC_H
#define HEURISTIC_H

#include <list>
#include <string>

#include "predicate.h"
#include "rules.h"

int apply_heuristic(Fact *, Fact *, RuleSet *, string);

#endif /* HEURISTIC_H */
