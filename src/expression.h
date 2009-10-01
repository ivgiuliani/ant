#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <vector>

#include "facts.h"
#include "functions.h"
#include "values.h"

enum EXPR_TYPE_INFO {
  EXPR_UNARY,
  EXPR_BOOLEAN,
  EXPR_UNKNOWN
};

class ExpressionError {};

/*
 * Classe base per le espressioni. Non dovrebbe mai essere
 * usata direttamente, piuttosto bisogna utilizzare le sue
 * sottoclassi.
 */
class Expression {
  public:
    virtual ~Expression() {};

    virtual bool evaluate(FuncMemory *, Fact *) { throw ExpressionError(); return false; };
    enum EXPR_TYPE_INFO get_type() { throw ExpressionError(); return EXPR_UNKNOWN; };
};

class BooleanExpr : virtual public Expression {
  protected:
    Expression *cond1;
    Expression *cond2;

  public:
    ~BooleanExpr() { delete cond1; delete cond2; };
    void set_cond1(Expression *cond) { this->cond1 = cond; };
    void set_cond2(Expression *cond) { this->cond2 = cond; };
    Expression *get_cond1() { return this->cond1; }
    Expression *get_cond2() { return this->cond2; }

    enum EXPR_TYPE_INFO get_type() { return EXPR_BOOLEAN; };
    bool evaluate(FuncMemory *, Fact *) { return false; }
};

class AndBooleanExpr : public BooleanExpr {
  public:
    bool evaluate(FuncMemory *, Fact *);
};

class OrBooleanExpr : public BooleanExpr {
  public:
    bool evaluate(FuncMemory *, Fact *);
};

#endif /* EXPRESSION_H */
