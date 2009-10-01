#include "debug.h"
#include "parser/parser.h"

/*
 * Fa il parsing di attributi all'interno di un blocco 'set'
 */
Options *
Parser::parse_set_block(list<Token> token_list) {
	list<Token>::iterator token_iterator = token_list.begin();
	list<Token> token_block;
	list<Token>::iterator token;

	debug("Creating new variable");

	Options *options = new Options();

	/* Fa il parsing dei singoli attributi del set block */
	while (token_iterator != token_list.end()) {
		token_block = BUILD_TOKEN_BLOCK(token_iterator, string(1, LNG_STMT_END).c_str(), TKN_OPERATOR, token_list.end());
		token = token_block.begin();

		if (token_block.size() != 2)
			throw ParseError("invalid number of operands for set assignment", token->get_line_no());

		if (token->get_type() != TKN_SYMBOL)
			throw ParseError("expected variable name", token->get_line_no());

		string set_name = token->get_value();
		token++;

		debug(set_name);
		debug(token->get_value());

		switch (token->get_type()) {
			case TKN_STRING:
				options->set(set_name, DTArrayItem(ITEM_STRING, token->get_value()));
				break;
			case TKN_NUMBER:
				options->set(set_name, DTArrayItem(ITEM_INT, token->get_value()));
				break;
			default:
				throw ParseError("option value must be either a string or a number", token->get_line_no());
				break;
		}

		token_iterator = this->advance_iterator(token_iterator, token_block.size());
		token_iterator++;
	}

	return options;
};
