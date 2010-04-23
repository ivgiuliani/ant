#ifndef FACTS_H
#define FACTS_H

#include <iostream>
#include <list>
#include <map>
#include <string>
#include <sstream>

#include "compiler.h"
#include "debug.h"

#include "ds/array.h"

using ::std::map;

typedef map<string, DTArray> attrmap;

/* Un singolo fatto */
class Fact {
  private:
    string name;
    /* la coppia <attributo>:<valore> */
    attrmap attributes;

  public:
    Fact() { };
    Fact(string name) {
      this->set_name(name);
    };

    void set_name(string);
    string get_name(void);

    void add_attribute(string, int);
    void add_attribute(string, string);
    void add_attribute(string, DTArray);

    void del_attribute(string);
    bool __ATTR(__NOTHROW, __HOT) has_attribute(string);

    DTArray* get_attribute(string);
    attrmap *get_attribute_list() { return &this->attributes; };

    bool equals_to(Fact *);
    string hash();
};

string inline DUMP_FACT(Fact *fact) {
  string output;
  unsigned int i;
  attrmap::iterator iter = fact->get_attribute_list()->begin();
  while (iter != fact->get_attribute_list()->end()) {
    output.append("  ");
    output.append(iter->first);
    output.append(": [");

    for (i = 0; i < iter->second.length(); i++) {
      output.append(iter->second[i].get_value());
      output.append(", ");
    }

    output.append("]\n");

    iter++;
  }

  return output;
};

#endif /* FACTS_H */
