#ifndef WM_H
#define WM_H

#include <list>
#include <string>
#include <stdint.h>

#include "expression.h"
#include "facts.h"
#include "predicate.h"
#include "rules.h"
#include "parser/cmdline.h"
#include "parser/setblock.h"

/*
 * Puntatore a funzione per le implementazioni degli algoritmi.
 * Il primo parametro e' il ruleset da considerare, il secondo
 * il fatto iniziale ed il terzo quello finale
 * Deve tornare il numero di nodi espansi, indipendentemente dal
 * fatto che abbia trovato una soluzione o meno.
 */
typedef uint32_t(*AlgoRunner)(RuleSet *, Fact *, Fact *, Options *);

/*
 * In questo file risiedono tutte le implementazioni delle
 * strutture dati della working memory
 */

/* Eccezione lanciata dagli algoritmi, interrompe l'esecuzione ma mostra comunque
 * un elenco parziale di statistiche
 */
class AlgorithmError {};

/* Eccezione per errori all'interno della working memory
 */
class WorkingMemoryError {
  public:
    string err;
    WorkingMemoryError(string err) { this->err = err; };
};

/* La working memory. Sostanzialmente una lista di fatti su cui andranno
 * valutate ed applicate le regole
 */
class WorkingMemory {
  public:
    WorkingMemory() {
      this->options = NULL;
    };

    ~WorkingMemory() {
      if (this->options)
        delete this->options;
    };

    void add_fact(Fact);
    uint32_t fact_number() { return this->fact_list.size(); };

    void prepare(CmdLineParser *);
    void die(CmdLineParser *, int);
    int32_t run(RuleSet *);

    void setopt(Options *opt) { this->options = opt; };

  private:
    Options *options;
    list<Fact> fact_list;
    Fact fact_initial, fact_final;
};

#endif /* WM_H */
