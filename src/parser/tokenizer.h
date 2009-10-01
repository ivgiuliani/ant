#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <iostream>
#include <list>

using ::std::list;
using ::std::string;
using ::std::stringstream;

/* Gli stati ammissibile nell'automa a stati finiti */
enum TOK_STATE {
	TKN_NONE = 0,
	TKN_SYMBOL = 1,
	TKN_STRING = 2,
	TKN_OPERATOR = 3,
	TKN_NUMBER = 4,
};

class TokenError {
	public:
		string err;
		unsigned int line_no;
		TokenError(string err, unsigned int line_no) {
			this->err = err;
			this->line_no = line_no;
		}
};

class Token {
	private:
		enum TOK_STATE type;
		unsigned int line_no;
	public:
		union {
			int number;
			char *string;
			char *symbol;
			char op;
		} value;

		/* Utilizziamo l'overloading per creare diversi
		 * tipi di token (piuttosto che usare i template,
		 * cosi` possiamo usare le linked list di stl
		 */

		Token(int, unsigned int);
		Token(const char *, enum TOK_STATE, unsigned int);
		Token(char, unsigned int);

		enum TOK_STATE get_type(void);
		string get_value(void);
		unsigned int get_line_no(void);
};

class Tokenizer {
	private:
		char *source;
		unsigned int len_source;

		bool is_valid_symbol(char);
		bool is_valid_number(char);
		bool is_valid_operator(char);
		bool is_string_marker(char);
		bool is_ignorable(char);
		bool is_space(char);

	public:
		Tokenizer(const char *);
		~Tokenizer(void);
		void generate_tokens(list<Token> *);
};

#define COUNT_LINENO(counter, chr) if (chr == '\n') counter++;

#endif /* TOKENIZER_H */
