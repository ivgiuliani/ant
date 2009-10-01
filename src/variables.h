#ifndef VARIABLES_H
#define VARIABLES_H

#include <string>
#include <vector>

#include "facts.h"
#include "expression.h"

/*
 * Un placeholder per le variabili
 */
class Variable {
  public:
    string varname;
    DTArrayItem value;
};

vector<string> extract_variables(Expression *);

#endif /* VARIABLES_H */
