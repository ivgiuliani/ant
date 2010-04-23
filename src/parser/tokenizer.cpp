/*
 * Questo insieme di classi prende in input il file sorgente e restituisce
 * una lista di tokens. Per esempio, dato in input:
 * beginFact fatto
 *   attributo = "valore"
 * endFact
 * restituira` una lista del tipo:
 * ['beginFact', 'fatto', 'attributo', 'valore', 'endFact']
 * Questa lista di tokens sara` poi analizzata dal parser che costruira` il
 * parsetree.
 */

#include <cstring>
#include <fstream>
#include <sstream>
#include <stdint.h>

#include "debug.h"
#include "parser/language.h"
#include "parser/tokenizer.h"

/*
 *****************************
 * METODI DELLA CLASSE TOKEN *
 *****************************
 */

Token::Token(int number, unsigned int line_no) {
	this->type = TKN_NUMBER;
	this->value.number = number;
	this->line_no = line_no;
};

Token::Token(const char *string, enum TOK_STATE type, unsigned int line_no) {
	this->type = type;
	this->line_no = line_no;
	switch(type) {
		case TKN_SYMBOL:
			this->value.symbol = new char[strlen(string) + 1];
			strcpy(this->value.symbol, string);
			break;
		case TKN_STRING:
			this->value.string = new char[strlen(string) + 1];
			strcpy(this->value.string, string);
			break;
		default:
			throw "Invalid token type";
			break;
	}
};

Token::Token(char c, unsigned int line_no) {
	this->type = TKN_OPERATOR;
	this->line_no = line_no;
	this->value.op = c;
}

enum
TOK_STATE Token::get_type(void) {
	return this->type;
}

string
Token::get_value(void) {
	string s;
	stringstream ss;
	switch(this->get_type()) {
		case TKN_SYMBOL:
			s = this->value.symbol;
			break;
		case TKN_STRING:
			s = this->value.string;
			break;
		case TKN_NUMBER:
			ss << this->value.number;
			s = ss.str();
			break;
		case TKN_OPERATOR:
			s = this->value.op;
			break;
		/* evitiamo i warnings di gcc */
		case TKN_NONE:
		default:
			break;
	}

	return s;
};

unsigned int
Token::get_line_no(void) {
	return this->line_no;
};

/*
 *********************************
 * METODI DELLA CLASSE TOKENIZER *
 *********************************
 */

/*
 * E' una classe che, preso in input il file sorgente,
 * restituisce una linked list di tokens
 */
Tokenizer::Tokenizer(const char *input_file) {
  std::ifstream file(input_file, std::ios::in|std::ios::ate);
	int32_t size;

	if (file.is_open()) {
		/* Leggi tutto il file in memoria e lascialo in this->source */
		size = file.tellg();
    this->source = new char[size];
		this->len_source = size;
		file.seekg(0, std::ios::beg);
		file.read(this->source, size);
		file.close();
	} else {
		throw input_file;
	}
};

Tokenizer::~Tokenizer(void) {
	delete[] this->source;
};

/*
 * Qui giace l'automa a stati finiti del tokenizer
 */
void
Tokenizer::generate_tokens(list<Token> *token_list) {
	register enum TOK_STATE current_state = TKN_NONE;
	char c = 0x00;
	uint32_t len = this->len_source, i = 0;
	uint32_t line_no = 1;
	string buffer;
	bool read = true;
	int32_t tmp_num;

	while (i < (len - 1)) {
        /* gestiamo il look ahead */
		if (read) {
			c = this->source[i++];
			COUNT_LINENO(line_no, c);
		} else read = true;

		switch(current_state) {
			case TKN_NONE:
				/* siamo all'inizio del file o tra un blocco e l'altro */
				buffer += c;
				if (is_valid_number(c)) {
					current_state = TKN_NUMBER;
				} else if (is_valid_symbol(c)) {
					current_state = TKN_SYMBOL;
				} else if (is_valid_operator(c)) {
					current_state = TKN_OPERATOR;
					read = false;
				} else if (is_string_marker(c)) {
					/* se abbiamo trovato il carattere di inizio stringa non
					 * aggiungiamolo al buffer, quello che interessa e` la
					 * stringa vera e propria
					 */
					buffer.clear();
					current_state = TKN_STRING;
				} else if (is_space(c) || is_ignorable(c)) {
					buffer.clear();
				} else {
					string err("Invalid char '");
					err += c;
					err.append("' found");
					throw TokenError(err, line_no);
				}
				break;
			case TKN_SYMBOL:
				if (!is_valid_symbol(c)) {
					token_list->push_back(Token(buffer.c_str(), TKN_SYMBOL, line_no));
					debug(buffer);
					buffer.clear();
					read = false;
					current_state = TKN_NONE;
				}
				buffer += c;
				break;
			case TKN_OPERATOR:
				/* il carattere prima era un operatore, non leggere il nuovo
				 * ma aggiungi il precedente ai token
				 */
				debug(buffer[0]);
				token_list->push_back(Token(buffer[0], line_no));
				buffer.clear();
				current_state = TKN_NONE;
				break;
			case TKN_STRING:
				if (is_string_marker(c)) {
					/* potrebbe essere un fine stringa o un marker all'interno
					 * della stringa preceduto da un carattere di escape
					 */
					if (buffer[buffer.length() - 1] == LNG_ESCAPE_CHAR) {
						buffer.erase(buffer.length() - 1);
						buffer += c;
					} else {
						/* e` effettivamente un fine stringa */
						token_list->push_back(Token(buffer.c_str(), TKN_STRING, line_no));
						debug(buffer);
						buffer.clear();

						current_state = TKN_NONE;
					}
				} else buffer += c;
				break;
			case TKN_NUMBER:
				if (is_valid_number(c)) {
					buffer += c;
				} else if ((is_valid_operator(c)) ||
									 (is_space(c))) {
					stringstream intstream(buffer);
					intstream >> tmp_num;
					token_list->push_back(Token(tmp_num, line_no));
					debug(tmp_num);

					read = false;
					buffer.clear();
					buffer += c;
					current_state = TKN_NONE;
				} else {
					string err("Invalid number near '");
					err += c;
					err.append("' found");
					throw TokenError(err, line_no);
				}
				break;
		}
	}

    /* mettiamo l'ultimo elemento (se presente) all'interno della token list */
    if (buffer.length() == 0) {
        buffer.clear();
        return;
    }
    debug(buffer);
    switch(current_state) {
        case TKN_NONE:
            break;
        case TKN_SYMBOL:
            token_list->push_back(Token(buffer.c_str(), TKN_SYMBOL, line_no));
            break;
        case TKN_OPERATOR:
            token_list->push_back(Token(buffer[0], line_no));
            break;
        case TKN_STRING:
            token_list->push_back(Token(buffer.c_str(), TKN_STRING, line_no));
            break;
        case TKN_NUMBER:
            stringstream intstream(buffer);
            intstream >> tmp_num;
            token_list->push_back(Token(tmp_num, line_no));
            break;
    }
    buffer.clear();
};

bool inline
Tokenizer::is_valid_symbol(char c) {
	return (isalnum(c) ||c == '_');
};

bool inline
Tokenizer::is_valid_number(char c) {
	return isdigit(c);
};

bool inline
Tokenizer::is_valid_operator(char c) {
	return (
		(c == LNG_OP_ASSIGNMENT)    ||
		(c == LNG_STMT_END)         ||
		(c == LNG_RULE_ACTION)      ||
		(c == LNG_PARAM_BEGIN)      ||
		(c == LNG_PARAM_END)        ||
		(c == LNG_PARAM_SEP)        ||
		(c == LNG_ARRAY_BEGIN)      ||
		(c == LNG_ARRAY_END)        ||
		(c == LNG_ARRAY_SEP)        ||
		(c == LNG_EXPR_BLOCK_START) ||
		(c == LNG_EXPR_BLOCK_END)
	);
};

bool inline
Tokenizer::is_string_marker(char c) {
	return (c == LNG_STRING_MARKER);
};

bool inline
Tokenizer::is_ignorable(char c) {
	return (c == '\n' || c == '\r');
};

bool inline
Tokenizer::is_space(char c) {
	return (c == ' ' || c == '\t');
};

