#ifndef HEAP_H
#define HEAP_H

#include <cmath>

#include "debug.h"

#include <iostream>

/*
 * Implementazione di un heap [Skiena08]
 * Non si danno garanzie di validita` sul contenuto della memoria 
 * se non esplicitamente inizializzata.
 * In altre parole: la memoria non viene pulita su inizializzazione,
 * pertanto si faccia attenzione a non andare su campi non inizializzati
 * oppure di marcarli come "non-usati".
 */
template <class T>
class Heap {
	private:
		T *array;
		uint32_t length;
		uint32_t n_ariety;

	public:
		Heap(unsigned int size, unsigned int n_ariety) {
			debug(size);

			this->array = new T[size];
			this->n_ariety = n_ariety;
		};

		~Heap() {
			delete[] this->array;
		};

		T* at(unsigned int position) {
			return &this->array[position];
		};

		T &operator[] (unsigned int p) { return this->array[p]; };

		/**********************
		 * metodi di supporto *
		 **********************/

		/* Genitore dell'elemento specificato */
		unsigned int parent(unsigned int pos) {
			return floor((1/(float)this->n_ariety) * (pos - 1));
		};

		/* L'i-esimo figlio del genitore alla posizione 'pos' */
		unsigned int ith_child(unsigned int pos, unsigned int i) {
			return (this->n_ariety * pos) + i;
		};
};

#endif /* HEAD_H */
