#ifndef SETBLOCK_H
#define SETBLOCK_H

#include <map>

class Options {
	public:
		map<string, DTArrayItem> list;

		DTArrayItem *get(string key) {
			return &this->list[key];
		};

		void set(string key, DTArrayItem value) {
			this->list[key] = DTArrayItem(value);
		};

		bool has(string key) {
			return (!(this->list.find(key) == this->list.end()));
		};
};

#endif /* SETBLOCK_H */

