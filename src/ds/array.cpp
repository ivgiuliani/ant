#include <queue>
#include <sstream>

#include "array.h"
#include "debug.h"
#include "parser/language.h"
#include "parser/parser.h"
#include "parser/tokenizer.h"

using ::std::queue;

/* Fa il parsing di un array */
DTArray parse_array(list<Token> token_list) {
	list<Token>::iterator token = token_list.begin();

	if ((token->get_type() != TKN_OPERATOR) ||
			(token->get_value().compare(string(1, LNG_ARRAY_BEGIN)))) {
		throw ParseError("expected array begin", token->get_line_no());
	}
	token++;

	queue<Token> token_queue;

	while (!((token->get_type() == TKN_OPERATOR) &&
			   (!token->get_value().compare(string(1, LNG_ARRAY_END))))) {
		debug(token->get_value());
		switch(token->get_type()) {
			case TKN_SYMBOL:
			case TKN_STRING:
			case TKN_NUMBER:
				token_queue.push(*token);
				break;
			default:
				throw ParseError("invalid value type", token->get_line_no());
				break;
		}

		token++;

		/* se il token non e` un separatore allora dovra` essere perforza
		 * il carattere di fine array
		 */
		if ((token->get_type() == TKN_OPERATOR) &&
				(!token->get_value().compare(string(1, LNG_ARRAY_SEP)))) {
			token++;
			continue;
		}
	}

	/* alla fine dobbiamo aver esaurito la lista di token */
	if (++token != token_list.end())
		throw ParseError("array misses closure operator", (--token)->get_line_no());

	/* costruisci il vero array a partire dalla coda creata precedentemente */
	DTArray array(token_queue.size());
	uint32_t counter = 0;
	stringstream ss;

	while (!token_queue.empty()) {
		Token t = token_queue.front();
		token_queue.pop();
		switch(t.get_type()) {
			case TKN_NUMBER:
				array.at(counter)->set_value(ITEM_INT, t.get_value());
				break;
			case TKN_STRING:
				array.at(counter)->set_value(ITEM_STRING, t.get_value());
				break;
			case TKN_SYMBOL:
				array.at(counter)->set_value(ITEM_VAR, t.get_value());
				break;
			default:
				throw ParseError("unexpected token type", token->get_line_no());
				break;
		}

		counter++;
	}

	return array;
};
