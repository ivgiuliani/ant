#ifndef GLOBAL_H
#define GLOBAL_H

#include "rules.h"
#include "wm.h"

/*
 * Mantiene in memoria lo stato globale del sistema, es:
 * - puntatore alla working memory
 * - nomi dei fatti che rappresentano stato iniziale e finale
 * - strategia di ricerca
 * - ...
 */
class Global {
  private:
    WorkingMemory *_wm;

  public:
    RuleSet *ruleset;

    Global() {
      this->_wm = new WorkingMemory();
      this->ruleset = new RuleSet();
    };
    ~Global() {
      delete this->_wm;
    };
    WorkingMemory *wm() { return this->_wm; };
};

#endif /* GLOBAL_H */
