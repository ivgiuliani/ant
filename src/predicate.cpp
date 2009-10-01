#include <list>
#include <vector>

#include "debug.h"
#include "expression.h"
#include "facts.h"
#include "functions.h"
#include "predicate.h"
#include "values.h"
#include "parser/parser.h"

Predicate::Predicate(string name) {
  this->name = name;
};

void
Predicate::add_parameter(PredicateParam param) {
  this->parameters.push_back(param);
};

vector<PredicateParam>
Predicate::get_parameters() {
  return this->parameters;
};

void
Predicate::set_parameters(vector<PredicateParam> param_list) {
  this->parameters = param_list;
};

void
Predicate::set_name(string name) {
  this->name = name;
};

string
Predicate::get_name() {
  return this->name;
};

/*
 * Ritorna vero se la regola in questione si applica ai fatti correnti
 */
bool
PredicateExpr::evaluate(FuncMemory *funcmem, Fact *fact) {
  debug(this->get_name());

  /* evitiamo di risolvere ogni volta il nome del predicato nella funzione
   * relativa, piuttosto mettiamo in cache il ptr alla funzione relativa
   */
  if (!this->cache_predicate_func) {
    __WM_LHS_FUNC tmp_func = RESOLVE_LHS_PREDICATE(this->get_name());
    if (!tmp_func) {
      string err(this->get_name());
      err.append(" is an unknown function");
      throw WorkingMemoryError(err);
    }

    this->cache_predicate_func = tmp_func;
  }

  return this->cache_predicate_func(funcmem, fact, this->get_parameters());
};

/*
 * Fa il parsing di un predicato nella forma nome(par1, par2, ..., parN)
 */
Predicate
Parser::parse_predicate(list<Token> token_list, list<Token>::iterator token, unsigned int *read_tokens) {
  list<Token> parameters;

  if (token_list.empty())
    throw ParseError("expected a predicate, none found");

  if (token->get_type() != TKN_SYMBOL)
    throw ParseError("expected predicate name", token->get_line_no());

  string predicate_name = token->get_value();
  debug(predicate_name);
  Predicate predicate(predicate_name);
  token++;
  (*read_tokens)++;

  if ((token->get_type() != TKN_OPERATOR) &&
      (token->get_value().length() != 1) &&
      (token->get_value()[0] != LNG_PARAM_BEGIN))
    throw ParseError("expected beginning of parameter list", token->get_line_no());
  token++;
  (*read_tokens)++;
  
  parameters = BUILD_TOKEN_BLOCK(token, string(1, LNG_PARAM_END), TKN_OPERATOR, token_list.end());
  list<Token>::iterator param_iter = parameters.begin();
  (*read_tokens) += parameters.size();

  /* se il predicato non ha nessun parametro non parsiamo i parametri */
  for (; parameters.size() != 0;) {
    PredicateParam param;
    param.set(param_iter->get_value());

    switch(param_iter->get_type()) {
      case TKN_SYMBOL:
        param.set_type(PARAM_VAR);
        break;
      case TKN_NUMBER:
        param.set_type(PARAM_NUMBER);
        break;
      case TKN_STRING:
        param.set_type(PARAM_STRING);
        break;
      default:
        string err("invalid symbol type '");
        err.append(param_iter->get_value());
        err.append("'");
        throw ParseError(err, param_iter->get_line_no());
        break;
    }
    predicate.add_parameter(param);
    debug(param.get());

    if (++param_iter == parameters.end()) break;

    if (param_iter->get_value() != string(1, LNG_PARAM_SEP)) {
      debug(param_iter->get_value());
      throw ParseError("expected parameter separator or end of parameter list", param_iter->get_line_no());
    }
    param_iter++;
  }

  return predicate;
};

