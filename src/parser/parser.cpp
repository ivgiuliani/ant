/*
 * Contiene tutto il parser. Il file viene tutto parsato
 * prima di essere processato per ritrovare errori di sintassi.
 */

#include <list>
#include <sstream>

#include "debug.h"
#include "global.h"
#include "parser.h"
#include "tokenizer.h"

using ::std::cerr;
using ::std::endl;

Parser::Parser(Global *global) {
  this->global = global;
};

bool
Parser::start(const char *fname) {
  Tokenizer *t;
  try {
    t = new Tokenizer(fname);
  } catch (const char *exc) {
    cerr << "Can't open '" << exc << "' for reading" << endl;
    return false;
  }

  try {
    t->generate_tokens(&this->token_list);
  } catch (TokenError tok) {
    cerr << "Token error: " << tok.err << " at line " << tok.line_no << endl;
    delete t;
    return false;
  } catch (const char *exc) {
    cerr << exc << endl;
    delete t;
    return false;
  }

  try {
    this->parse();
  } catch (ParseError pexc) {
    cerr << "Parse error: " << pexc.err << endl;
    delete t;
    return false;
  }

  delete t;
  return true;
};

Parser::~Parser(void) {
  list<Token>::iterator it;

  /* Distruggi la lista di tokens alla fine */
  for (it = this->token_list.begin(); it != this->token_list.end(); it++) {
    /*
     * Gli unici tipi di token che allochiamo sono SYMBOL e STRING,
     * gli altri vengono semplicemente messi nell'heap percui non
     * serve deallocarli.
     */
    switch((*it).get_type()) {
      case TKN_SYMBOL:
        delete[] (*it).value.symbol;
        break;
      case TKN_STRING:
        delete[] (*it).value.string;
        break;
      /* evitiamo i warnings di GCC */
      case TKN_OPERATOR:
      case TKN_NUMBER:
      case TKN_NONE:
        break;
    }
  }
};

/* Costruisce la parse list */
void
Parser::parse(void) {
  enum KEYWORD keyword;
  list<Token>::iterator token;
  list<Token> token_block;
  token = this->token_list.begin();

  /*
   * Qui si identificano i macro blocchi, che vengono poi
   * successivamente parsati dalle specifiche routines. Per
   * esempio si individuano i blocchi di fatti (racchiusi tra
   * beginFact e endFact) e si passano al parser relativo
   * che riempira` la working memory.
   */
    while (token != this->token_list.end()) {
    debug(token->get_value());
    keyword = RESOLVE_KEYWORD_FROM_TOKEN(*token);
    switch(keyword) {
      case KEYW_FACT_BEGIN:
        /* Inizio di un fatto */
        token_block = BUILD_KEYW_TOKEN_BLOCK(++token, KEYW_FACT_END, this->token_list.end());
        this->parse_fact_block(token_block);
        token = this->advance_iterator(token, token_block.size());
        break;

      case KEYW_RULE_BEGIN:
        /* Inizio di una regola */
        token_block = BUILD_KEYW_TOKEN_BLOCK(++token, KEYW_RULE_END, this->token_list.end());
        this->parse_rule_block(token_block);
        token = this->advance_iterator(token, token_block.size());
        break;

      case KEYW_HEUR_BEGIN:
        /* Inizio di una euristica */
        token_block = BUILD_KEYW_TOKEN_BLOCK(++token, KEYW_HEUR_END, this->token_list.end());
        this->parse_heuristic(token_block);
        token = this->advance_iterator(token, token_block.size());
        break;

      case KEYW_SET_BEGIN:
        /* Inizio di istruzione set */
        token_block = BUILD_KEYW_TOKEN_BLOCK(++token, KEYW_SET_END, this->token_list.end());
        this->global->wm()->setopt(this->parse_set_block(token_block));
        token = this->advance_iterator(token, token_block.size());
        break;

      case KEYW_SHOW_ANT:
        /* Inizio istruzione show ant */
        this->parse_show_ant();
        break;

      case KEYW_UNKNOWN:
      default:
        throw ParseError("invalid symbol found, expected keyword", token->get_line_no());
        break;
    }
    
    if (token != this->token_list.end())
      token++;
  }
};

