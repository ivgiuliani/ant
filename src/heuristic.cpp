#include "parser/parser.h"

/* fa il parsing di una euristica */
void
Parser::parse_heuristic(list<Token> token_list) {
  list<Token>::iterator token_iterator = token_list.begin();
  list<Token> predlist;

  if (token_list.size() < 1)
    throw ParseError("minimum number of required tokens not satisfied for heuristic", token_iterator->get_line_no());

  /* il primo parametro e` il nome dell'euristica */
  if (token_iterator->get_type() != TKN_SYMBOL)
    throw ParseError("expected heuristic name", token_iterator->get_line_no());
  string name = token_iterator->get_value();
  ++token_iterator;

  while (token_iterator != token_list.end())
    predlist.push_back(*(token_iterator++));

  this->global->ruleset->add_heuristic(name, this->parse_rule_rhs(predlist));
}

/* Applica l'euristica specificata al fatto passato in input */
int
apply_heuristic(Fact *initial, Fact *final, RuleSet *ruleset, string heuristic_name) {
  list<PredicateCall> heuristic = ruleset->heuristics[heuristic_name];
  list<PredicateCall>::iterator predicate = heuristic.begin();
  FuncMemory funcmem = FuncMemory();

  __WM_RHS_FUNC tmp_func;
  while (predicate != heuristic.end()) {
    debug(predicate->get_name());

    tmp_func = RESOLVE_RHS_PREDICATE(predicate->get_name());
    if (!tmp_func) {
      string err(predicate->get_name());
      err.append(" is an unknown function");
      throw WorkingMemoryError(err);
    }

    tmp_func(&funcmem, initial, predicate->get_parameters());
    ++predicate;
  }

  if (funcmem.find("?") == funcmem.end())
    throw WorkingMemoryError("Heuristic doesn't define a return value");

  string result = funcmem["?"].get_value();
  stringstream ss;
  ss << result;
  int returnval;
  ss >> returnval;

  return returnval;
}

