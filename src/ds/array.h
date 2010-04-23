#ifndef ARRAY_H
#define ARRAY_H

#include <sstream>
#include <string>

#include "compiler.h"
#include "parser/tokenizer.h"

class ArrayIndexError {};

enum ITEM_TYPE {
	ITEM_INT = 0,
	ITEM_STRING,
	ITEM_VAR
};

/*
 * Rappresenta ogni elemento di un array
 */
class DTArrayItem {
	private:
		ITEM_TYPE type;
		string value;

	public:
		DTArrayItem() {};

		DTArrayItem(const DTArrayItem &orig) {
			type = orig.type;
			value.assign(orig.value);
		}

		DTArrayItem(ITEM_TYPE type, string value) {
			this->set_value(type, value);
		};

		DTArrayItem(int value) {
			this->set_value(value);
		};

		ITEM_TYPE get_type() { return this->type; };
		string __ATTR(__HOT) get_value() {
			if (this->type == ITEM_INT) {
				stringstream ss;
				ss << this->value;
				return ss.str();
			} else return string(this->value);
		};

		void set_value(ITEM_TYPE type, string value) {
			this->type = type;
			this->value = string(value);
		};

		void set_value(int value) {
			this->type = ITEM_INT;
			stringstream ss;
			ss << value;
			this->value = ss.str();
		};

		bool operator==(const DTArrayItem &orig) {
			return ((this->type == orig.type) && 
					    (!this->value.compare(orig.value)));
		};

		bool operator!=(const DTArrayItem &orig) {
			return (!(this->type == orig.type) || 
					    (this->value.compare(orig.value)));
		}
};

/*
 * L'implementazione dell'array.
 */
class DTArray {
	private:
		unsigned int _length;
		DTArrayItem *array;

	public:
		DTArray() {
			/* gestisci il caso in cui ci sia un unico elemento all'interno
			 * dell'array
			 */
			this->_length = 1;
			this->array = new DTArrayItem[1];
		};

		DTArray(unsigned int length) {
			this->_length = length;
			this->array = new DTArrayItem[length];
		};

		~DTArray() {
			delete[] this->array;
		};

		DTArray(const DTArray &orig) {
			_length = orig._length;
			array = new DTArrayItem[_length];
			for (unsigned int i = 0; i < _length; i++)
				array[i] = orig.array[i];
		};

		void operator= (const DTArray &orig) {
			_length = orig._length;
			array = new DTArrayItem[_length];
			for (unsigned int i = 0; i < _length; i++)
				array[i] = orig.array[i];
		};

		DTArrayItem& operator[] (unsigned int pos) { return this->array[pos]; };

		bool operator==(const DTArray &orig) {
			if (orig._length != _length)
				return false;
			for (unsigned int i = 0; i < _length; i++) {
				if (array[i] != orig.array[i])
					return false;
			}
			return true;
		};

		bool operator!=(const DTArray &orig) {
			bool equals = true;
			if (orig._length != _length)
				return true;
			for (unsigned int i = 0; i < _length; i++) {
				if (array[i] != orig.array[i])
					equals = false;
			}
			return !equals;
		}

		unsigned int length() {
			return this->_length;
		};

		DTArrayItem* at(unsigned int position) {
			if (position > this->length())
				throw ArrayIndexError();
			return &this->array[position];
		};
};

DTArray parse_array(list<Token>);

#endif /* ARRAY_H */
