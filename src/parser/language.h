#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <list>

#include "parser/tokenizer.h"

#define LNG_STRING_MARKER '"'
#define LNG_ESCAPE_CHAR   '\\'
#define LNG_OP_ASSIGNMENT '='
#define LNG_STMT_END      ';'
#define LNG_RULE_ACTION   '>'
#define LNG_PARAM_BEGIN   '('
#define LNG_PARAM_END     ')'
#define LNG_PARAM_SEP     ','
#define LNG_ARRAY_BEGIN   '['
#define LNG_ARRAY_END     ']'
#define LNG_ARRAY_SEP     LNG_PARAM_SEP

#define LNG_EXPR_BLOCK_START '('
#define LNG_EXPR_BLOCK_END   ')'
#define LNG_EXPR_AND         "and"
#define LNG_EXPR_OR          "or"

enum BOOLEAN_OP {
	BOOLEAN_AND,
	BOOLEAN_OR,
	BOOLEAN_UNKNOWN
};

/*
 * Dato una stringa ritorna il valore dell'operatore booleano
 * associato o BOOLEAN_UNKNOWN se non e` un operatore valido.
 * *NON* togliere l'inline qui se non sai cosa stai facendo.
 */
inline enum BOOLEAN_OP
RESOLVE_BOOLEAN_OP_FROM_STRING(string oper) {
	if (!oper.compare(LNG_EXPR_AND)) return BOOLEAN_AND;
	if (!oper.compare(LNG_EXPR_OR))  return BOOLEAN_OR;
	return BOOLEAN_UNKNOWN;
};

/* Aggiungere una keyword significa andare a modificare di
 * conseguenza la enum KEYWORD e le funzioni
 * RESOLVE_KEYWORD_FROM_TOKEN e RESOLVE_STRING_FROM_KEYWORD
 */
#define LNG_FACT_BEGIN "beginFact"
#define LNG_FACT_END   "endFact"
#define LNG_RULE_BEGIN "beginRule"
#define LNG_RULE_END   "endRule"
#define LNG_SET_BEGIN  "set"
#define LNG_SET_END    "endSet"
#define LNG_HEUR_BEGIN "beginHeuristic"
#define LNG_HEUR_END   "endHeuristic"
#define LNG_SHOW_ANT   "showAnt"

/* Ricorda di aggiornare RESOLVE_KEYWORD_FROM_TOKEN e RESOLVE_STRING_FROM_KEYWORD
 * quando modifichi questa enum
 */
enum KEYWORD {
	KEYW_FACT_BEGIN,
	KEYW_FACT_END,
	KEYW_RULE_BEGIN,
	KEYW_RULE_END,
	KEYW_SET_BEGIN,
	KEYW_SET_END,
	KEYW_HEUR_BEGIN,
	KEYW_HEUR_END,
	KEYW_SHOW_ANT,
	KEYW_UNKNOWN, /* un error code per riconoscere una keyword non meglio identificata */
};

/*
 * Dato un token ritorna il valore relativo della keyword
 * o KEYW_UNKNOWN se non e` una keyword o e` un valore
 * sconosciuto.
 * Se modifichi questa funzione probabilmente devi andare a
 * modificare anche RESOLVE_STRING_FROM_KEYWORD.
 * *NON* togliere l'inline qui se non sai cosa stai facendo.
 */
inline const enum KEYWORD
RESOLVE_KEYWORD_FROM_TOKEN(Token t) {
	if (t.get_type() != TKN_SYMBOL) return KEYW_UNKNOWN;
	string s = t.get_value();

	/* XXX: la ricerca della keyword giusta qui e` fatta con O(n) confronti
	 * ed inoltre e` anche abbastanza poco elegante. Trovare un modo migliore
	 */
	if (!s.compare(LNG_FACT_BEGIN)) return KEYW_FACT_BEGIN;
	if (!s.compare(LNG_FACT_END))   return KEYW_FACT_END;
	if (!s.compare(LNG_RULE_BEGIN)) return KEYW_RULE_BEGIN;
	if (!s.compare(LNG_RULE_END))   return KEYW_RULE_END;
	if (!s.compare(LNG_HEUR_BEGIN)) return KEYW_HEUR_BEGIN;
	if (!s.compare(LNG_HEUR_END))   return KEYW_HEUR_END;
	if (!s.compare(LNG_SET_BEGIN))  return KEYW_SET_BEGIN;
	if (!s.compare(LNG_SET_END))    return KEYW_SET_END;
	if (!s.compare(LNG_SHOW_ANT))   return KEYW_SHOW_ANT;

	return KEYW_UNKNOWN;
};

/*
 * Dato una keyword ritorna la stringa che identifica quella kewyord.
 * Se modifichi questa funzione probabilmente devi andare a modificare
 * anche RESOLVE_KEYWORD_FROM_TOKEN.
 * *NON* togliere l'inline qui se non sai cosa stai facendo.
 */
inline string
RESOLVE_STRING_FROM_KEYWORD(enum KEYWORD keyword) {
	switch(keyword) {
		case KEYW_FACT_BEGIN:
			return string(LNG_FACT_BEGIN);
			break;
		case KEYW_FACT_END:
			return string(LNG_FACT_END);
			break;
		case KEYW_RULE_BEGIN:
			return string(LNG_RULE_BEGIN);
			break;
		case KEYW_RULE_END:
			return string(LNG_RULE_END);
			break;
		case KEYW_HEUR_BEGIN:
			return string(LNG_HEUR_BEGIN);
			break;
		case KEYW_HEUR_END:
			return string(LNG_HEUR_END);
			break;
		case KEYW_SET_BEGIN:
			return string(LNG_SET_BEGIN);
			break;
		case KEYW_SET_END:
			return string(LNG_SET_END);
			break;
		case KEYW_SHOW_ANT:
			return string(LNG_SHOW_ANT);
			break;
		default:
			return NULL;
			break;
	};
};

#endif /* LANGUAGE_H */
