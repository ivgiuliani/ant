#ifndef HASHMAP_H
#define HASHMAP_H

#include <string>

#define HASHMAP_DEFAULT_SIZE 100

namespace ds {
	class HashFull {};

	template <typename T>
	class HashNode {
		public:
			HashNode() : deleted(false), free(true) {};
			HashNode(string key) : deleted(false), free(true), key(key) {};

			bool deleted;
			bool free;
			string key;
			T value;
	};

	template <typename T>
	class HashMap {
		protected:
			HashNode<T> array[HASHMAP_DEFAULT_SIZE];

			/* Torna il valore della chiave nell'hash */
			HashNode<T>* get(string key) {
				iterator pos = this->find(key);
				if (pos == this->end()) return NULL;

				return pos._NODE();
			};

		public:
			class iterator {
				friend class HashMap;
				private:
					HashMap<T> *hm;
					int curr;
				protected:
					HashNode<T> *_NODE() {
						return &(this->hm->array[this->curr]);
					};
				public:
					iterator() : hm(NULL), curr(0) { };

					iterator(HashMap<T> *hm, unsigned int position) :
						hm(hm), curr(position) {
						/* posizionati sul primo elemento utile */
						while (this->hm->array[this->curr].free && this->curr < HASHMAP_DEFAULT_SIZE)
							this->curr++;
					};

					HashNode<T>* operator->() {
						return &(this->hm->array[this->curr]);
					};

					/* incrementa l'iteratore (cerca l'elemento successivo) */
					void operator++(int) {
						while (this->hm->array[this->curr].free && this->curr < HASHMAP_DEFAULT_SIZE)
							this->curr++;
					};

					/* decrementa l'iteratore (cerca l'elemento precedente) */
					void operator--(int) {
						while (this->hm->array[this->curr].free && this->curr >= 0)
							this->cur--;
					};

					bool operator==(const iterator orig) {
						return orig.curr == this->curr;
					};

					bool operator!=(const iterator orig) {
						return orig.curr != this->curr;
					};
			};

			HashMap() {};

			/* metodi per l'iterazione */
			iterator begin() {
				return iterator(this, 0);
			};
			iterator end() {
				return iterator(this, HASHMAP_DEFAULT_SIZE);
			};

			unsigned int position(string key) {
				unsigned int i;
				unsigned int val = 0;

				for (i = 0; i < key.length(); i++)
					val += key[i];

				val %= (HASHMAP_DEFAULT_SIZE / 2);
				return val;
			};

			iterator find(string key) {
				unsigned int pos = this->position(key);

				/* trova la prima occorrenza della chiave (potrebbe non essere quella
				 * indicata da 'pos' in caso di collisioni, allora andiamo avanti a
				 * cercarla)
				 */
				while ((pos < HASHMAP_DEFAULT_SIZE) &&
						   (this->array[pos].key.compare(key)) &&
							 (!this->array[pos].deleted)) {
					pos++;
				}

				if (pos >= HASHMAP_DEFAULT_SIZE)
					return iterator(this, HASHMAP_DEFAULT_SIZE);
				else return iterator(this, pos);
			};

			/* Valorizza una nuova entry nell'hash */
			HashNode<T>* insert(string key) {
				unsigned int pos = this->position(key);

				/* trova uno slot libero */
				while ((pos < HASHMAP_DEFAULT_SIZE) &&
						   (!this->array[pos].free) &&
						   (!this->array[pos].deleted)) {
					pos++;
				}

				if (pos >= HASHMAP_DEFAULT_SIZE)
					return NULL;

				this->array[pos] = HashNode<T>(key);
				this->array[pos].free = false;

				return &this->array[pos];
			};

			void erase(iterator iter) {
				iter._NODE()->deleted = true;
			};

			T& operator[](const string key) {
				iterator val = this->find(key);
				if (val == this->end()) {
					this->insert(key);
					val = this->find(key);
				}

				if (val == this->end())
					throw HashFull();

				return val->value;
			};

			T& operator[](const char *key) {
				iterator val = this->find(key);
				if (val == this->end()) {
					this->insert(key);
					val = this->find(key);
				}

				if (val == this->end())
					throw HashFull();

				return val->value;
			};
	};
};

#endif /* HASHMAP_H */
