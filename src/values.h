#ifndef VALUES_H
#define VALUES_H

#include <string>
#include <vector>

enum PARAM_TYPE {
  PARAM_STRING,
  PARAM_NUMBER,
  PARAM_VAR
};

class PredicateParam {
  private:
    string value;
    enum PARAM_TYPE type;
  public:
    PredicateParam() {};
    PredicateParam(PARAM_TYPE ptype, string value) {
      this->type = ptype;
      this->value = value;
    };
    void set_type(PARAM_TYPE ptype) { this->type = ptype; };
    PARAM_TYPE get_type() { return this->type; };
    void set(string value) { this->value.assign(value); };
    string get() { return string(this->value); };
};

#endif /* VALUES_H */
