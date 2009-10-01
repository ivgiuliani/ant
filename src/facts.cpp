#include <cstring>
#include <list>
#include <sstream>
#include <string>

#include "./debug.h"
#include "./wm.h"
#include "./ds/array.h"
#include "./parser/parser.h"

/*
 * Fa il parsing di un blocco racchiuso tra beginFact
 * e endFact (esclusi).
 * Aggiunge anche il fatto alla working memory.
 */
void
Parser::parse_fact_block(list<Token> token_list) {
  list<Token>::iterator token_iterator = token_list.begin();
  list<Token> token_block;

  if (token_iterator->get_type() != TKN_SYMBOL) {
    if (token_iterator == token_list.end()) {
      throw ParseError("expected fact name");
    } else {
      throw ParseError("expected fact name", (*token_iterator).get_line_no());
    }
  }

  string fact_name = token_iterator->get_value();
  token_iterator++;
  debug(fact_name);

  debug("Creating new fact");
  Fact *fact = new Fact(fact_name);

  /* Fa il parsing dei singoli attributi del fatto */
  while (token_iterator != token_list.end()) {
    token_block = BUILD_TOKEN_BLOCK(token_iterator,
                                    string(1, LNG_STMT_END).c_str(),
                                    TKN_OPERATOR,
                                    token_list.end());
    this->parse_fact_attribute(fact, token_block);
    token_iterator = this->advance_iterator(token_iterator, token_block.size());
    token_iterator++;
  }

  this->global->wm()->add_fact(Fact(*fact));
  delete fact;
};

/*
 * Fa il parsing di una tripla <attributo> '=' <valore>.
 * Aggiunge anche l'attributo parsato al fatto corrente.
 */
void
Parser::parse_fact_attribute(Fact *fact, list<Token> token_list) {
  list<Token>::iterator token = token_list.begin();

  /* Controlla che ci sia una tripla <nome> '=' <valore> valida */
  if (token->get_type() != TKN_SYMBOL)
    throw ParseError("expected symbol as attribute name", token->get_line_no());
  string attr_name = token->get_value();
  token++;

  if ((token->get_type() != TKN_OPERATOR) &&
      (token->get_value().length() != 1) &&
      (token->get_value()[0] != LNG_OP_ASSIGNMENT)) {
    throw ParseError("expected assignment after attribute declaration",
                     token->get_line_no());
  }
  token++;

  debug(attr_name);

  list<Token> token_block;
  while (token != token_list.end()) {
    token_block.push_back(*token);
    token++;
  }
  DTArray value = this->parse_fact_value(token_block);
  fact->add_attribute(attr_name, value);
};

/*
 * Fa il parsing del valore di un attributo. Se e` un valore atomico
 * lo converte in un array di un elemento
 */
DTArray
Parser::parse_fact_value(list<Token> token_list) {
  list<Token>::iterator token = token_list.begin();
  stringstream ss;
  int num;
  DTArray array;

  if (token->get_value().compare(string(1, LNG_ARRAY_BEGIN))) {
    debug(token->get_value());
    /* caso dell'elemento atomico */
    switch (token->get_type()) {
      case TKN_SYMBOL:
        array.at(0)->set_value(ITEM_VAR, token->get_value());
        break;
      case TKN_STRING:
        array.at(0)->set_value(ITEM_STRING, token->get_value());
        break;
      case TKN_NUMBER:
        ss << token->get_value();
        ss >> num;
        array.at(0)->set_value(num);
        break;
      default:
        throw ParseError("unexpected value type", token->get_line_no());
        break;
    }
  } else {
    list<Token> token_block = BUILD_TOKEN_BLOCK(token,
                                                string(1, LNG_ARRAY_END).c_str(),
                                                TKN_OPERATOR,
                                                token_list.end());
    token_block.push_back(*(--token_list.end()));
    array = parse_array(token_block);
  }

  return array;
};

/**************************
 ******* Classe Fact ******
 **************************/
void
Fact::set_name(string name) {
  this->name = name;
};

string
Fact::get_name(void) {
  return this->name;
};

/*
 * Aggiungi un attributo di tipo intero */
void
Fact::add_attribute(string name, int val) {
  DTArray tmp(1);
  tmp[0] = DTArrayItem(val);
  this->add_attribute(name, tmp);
};

/* Aggiungi un attributo di tipo stringa */
void
Fact::add_attribute(string name, string val) {
  DTArray tmp(1);
  tmp[0] = DTArrayItem(ITEM_STRING, val);
  this->add_attribute(name, tmp);
};

/* Aggiungi un attributo di tipo array */
void
Fact::add_attribute(string name, DTArray val) {
  debug(name);
  this->attributes[name] = val;
};

/*
 * Cancella un attributo del fatto
 */
void
Fact::del_attribute(string name) {
  attrmap::iterator it = this->attributes.find(name);

  if (it != this->attributes.end()) {
    this->attributes.erase(it);
  }
};

bool
Fact::has_attribute(string name) {
  attrmap::iterator it = this->attributes.find(name);
  return (it != this->attributes.end());
};

/*
 * Confronta l'istanza del fatto preso in esame con un altro fatto
 */
bool
Fact::equals_to(Fact *cmp) {
  /* se non hanno la stessa dimensione e` inutile procedere */
  attrmap *list = this->get_attribute_list();

  if (list->size() != cmp->get_attribute_list()->size())
    return false;

  attrmap::iterator attrs = list->begin();
  while (attrs != list->end()) {
    /* se l'attributo non esiste nemmeno e` inutile andare avanti */
    if (!cmp->has_attribute(attrs->first))
      return false;

    if (*(cmp->get_attribute(attrs->first)) != attrs->second)
      return false;

    attrs++;
  }

  return true;
};

DTArray *
Fact::get_attribute(string attr_name) {
  if (!this->has_attribute(attr_name)) {
    string err("Attribute named ");
    err.append(attr_name);
    err.append(" not found");
    throw WorkingMemoryError(err);
  }

  return &this->attributes[attr_name];
};

/* torna un semplice hash del fatto (e` molto semplicemente una
 * concatenazione delle stringhe delle coppie attributo-valore
 */
string
Fact::hash() {
  string res;
  unsigned int i;
  attrmap::iterator it = this->attributes.begin();

  while (it != this->attributes.end()) {
    res.append(it->first);
    for (i = 0; i < it->second.length(); i++)
      res.append(it->second[i].get_value());

    it++;
  }

  return res;
};

