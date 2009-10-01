#ifndef PARSER_H
#define PARSER_H

#include <list>
#include <sstream>

#include "expression.h"
#include "global.h"
#include "predicate.h"
#include "wm.h"
#include "ds/array.h"
#include "parser/language.h"
#include "parser/setblock.h"
#include "parser/tokenizer.h"

class ParseError {
	public:
		string err;
		ParseError(string s) {
			this->err = s;
		};
		ParseError(string s, unsigned int line_no) {
			string err = s;
			s.append(" at line ");
			stringstream i;
			i << line_no;
			s.append(i.str());
			this->err = s;
		};
};

/*
 * Costruisce una double linked list di token iterando su una
 * lista esistente finche` non trova la stringa di endblock
 * specificata (il token dovra` essere di tipo block_end_type).
 * Endpoint e` il limite su cui iterare.
 *
 * Nota: la lista costruita non include il carattere di block_end.
 * Nota2: *NON* togliere l'inline qui se non sai cosa stai facendo.
 */
inline list<Token>
BUILD_TOKEN_BLOCK(
		list<Token>::iterator tkn_iterator,
		string block_end,
		const enum TOK_STATE block_end_type,
		const list<Token>::iterator endpoint) {
    list<Token> tkn_list;

    while (tkn_iterator != endpoint) {
        if (tkn_iterator->get_type() == block_end_type &&
            !tkn_iterator->get_value().compare(block_end))
            return tkn_list;
        tkn_list.push_back(*tkn_iterator);
        tkn_iterator++;
    }

    throw ParseError("expected keyword, not found", (--tkn_iterator)->get_line_no());
};

/*
 * Costruisce una double linked list di token iterando su una lista
 * esistente finche` non trova la keyword 'keyword' specificata.
 * Endpoint e` il limite su cui iterare.
 * *NON* togliere l'inline qui se non sai cosa stai facendo.
 */

inline list<Token> BUILD_KEYW_TOKEN_BLOCK(
		list<Token>::iterator tkn_iterator,
		const enum KEYWORD keyword,
		const list<Token>::iterator endpoint) {
	return BUILD_TOKEN_BLOCK(tkn_iterator, RESOLVE_STRING_FROM_KEYWORD(keyword), TKN_SYMBOL, endpoint);
};

class Parser {
	private:
		Global *global;
		list<Token> token_list;

	protected:
		void parse(void);

		void parse_fact_block(list<Token>);
		void parse_fact_attribute(Fact *, list<Token>);
		DTArray parse_fact_value(list<Token>);
		void parse_rule_block(list<Token>);
		Expression *parse_rule_lhs(list<Token>);
		list<PredicateCall> parse_rule_rhs(list<Token>);
		Predicate parse_predicate(list<Token>, list<Token>::iterator, unsigned int *);
		void parse_show_ant();
		Expression *parse_expression(list<Token>);
		Options *parse_set_block(list<Token>);
		void parse_heuristic(list<Token>);

		/*
		 * Dato che gli iteratori per la lista STL non danno la possibilita'
		 * di muoversi avanti di un certo numero di passi, abbiamo bisogno
		 * di questa funzione. E' necessario mantenere l'implementazione nella
		 * definizione della classe per sfruttare l'inline.
		 */
		list<Token>::iterator advance_iterator(list<Token>::iterator iter, int32_t n) {
			while (n-- > 0) iter++;
			return iter;
		};

	public:
		Parser(Global *);
		~Parser(void);
		bool start(const char *);
};

#endif /* PARSER_H */
