#include <stdlib.h>
#include <sys/time.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <list>
#include <string>

#include "compiler.h"
#include "debug.h"
#include "expression.h"
#include "wm.h" 

#include "algorithms/astar.h"
#include "algorithms/bfs.h"
#include "algorithms/dfs.h"
#include "algorithms/hill.h"

#ifdef UNIX
#include <sys/resource.h>
#endif

using ::std::cout;
using ::std::endl;
using ::std::setw;

/*************
 * Classe WM *
 *************/
void
WorkingMemory::add_fact(Fact fact) {
#ifdef DEBUG
  string r("Adding fact ");
  r.append(fact.get_name());
  r.append(" to working memory");
  debug(r);
#endif
  this->fact_list.push_back(fact);
};

/*
 * Effettua controlli di validita` sulla working memory e
 * imposta alcune variabili di istanza necessarie per
 * l'esecuzione del programma
 * XXX: scritta cosi', questa funzione e` alquanto obbrobriosa.
 * Andrebbe piu` o meno tutta riscritta in maniera piu` intelligente.
 */
void
WorkingMemory::prepare(CmdLineParser *cmdline) {
  debug("Preparing working memory");
  
  list<Fact>::iterator fact_iter = this->fact_list.begin();
  bool found = false;

  __INIT_LHS_PREDICATE_RESOLVER();
  __INIT_RHS_PREDICATE_RESOLVER();

  if (!this->options->has("start") || !this->options->has("stop"))
    throw WorkingMemoryError("missing start/stop option");

  string start = this->options->get("start")->get_value();
  string stop = this->options->get("stop")->get_value();

  if (start.length() == 0)
    throw WorkingMemoryError("undefined initial state");

  if (stop.length() == 0)
    throw WorkingMemoryError("undefined final state");

  if (!this->options->has("algo"))
    throw WorkingMemoryError("undefined algorithm");

  while (fact_iter != this->fact_list.end()) {
    found = (!fact_iter->get_name().compare(start));
    if (found) {
      this->fact_initial = *fact_iter;
      break;
    }
    fact_iter++;
  }

  if (!found) {
    string err = "can't find initial fact named ";
    err.append(start);
    throw WorkingMemoryError(err);
  }

  found = false;
  fact_iter = this->fact_list.begin();
  while (fact_iter != this->fact_list.end()) {
    found = (!fact_iter->get_name().compare(stop));
    if (found) {
      this->fact_final = *fact_iter;
      break;
    }
    fact_iter++;
  }

  if (!found) {
    string err = "can't find final fact named ";
    err.append(stop);
    throw WorkingMemoryError(err);
  }

  /* sovrascrivi le opzioni del sorgente con quelle passate in input
   * XXX: questa parte andrebbe riscritta meglio
   */
  if (cmdline->parsed.find("limit") != cmdline->parsed.end()) {
    stringstream ss;
    int32_t tmpnum;

    ss << (*cmdline)["limit"];
    if (!(ss >> std::dec >> tmpnum)) throw WorkingMemoryError("'limit' option is not numerical");
    this->options->set("limit", DTArrayItem(tmpnum));
  }

  if (cmdline->parsed.find("algo") != cmdline->parsed.end()) {
    this->options->set("algo", DTArrayItem(ITEM_STRING, (*cmdline)["algo"]));
  }
};

/* Operazioni di riepilogo della working memory */
void
WorkingMemory::die(CmdLineParser *cmdline, int32_t expanded_nodes) {
  /* statistiche di processo */
  if (cmdline->parsed.find("show-stats") != cmdline->parsed.end()) {
    cout << std::left;
    cout << "Usage statistics:" << endl;
    cout << setw(32) << " Expanded nodes:";

    if (expanded_nodes < 0)
      cout << "n/a";
    else cout << expanded_nodes;
    cout << endl;
#ifdef UNIX
    struct rusage res_usage;
  
    if (getrusage(RUSAGE_SELF, &res_usage)) {
      cout << "[ No memory usage stats available ]" << endl;
      return;
    }
  
    /* vedi http://rabbit.eng.miami.edu/info/functions/time.html#getrusage */
    /* I campi ru_maxrss, ru_ixrss, ru_idrss, ru_isrss non vegono forniti da
     * Linux (sono messi a 0) pertanto non visualizziamoli (sono definiti,
     * invece, dai vari *BDS)
     */
    cout << setw(32) << " User time used:" << res_usage.ru_utime.tv_sec << "," << res_usage.ru_utime.tv_usec << "s" << endl;
    cout << setw(32) << " System time used:" << res_usage.ru_stime.tv_sec << "," << res_usage.ru_stime.tv_usec << "s" << endl;
    cout << setw(32) << " Page reclaims:" << res_usage.ru_minflt << endl;
    cout << setw(32) << " Page faults:" << res_usage.ru_majflt << endl;
    cout << setw(32) << " No. of swaps:" << res_usage.ru_nswap << endl;
    cout << setw(32) << " Voluntary context switches:" << res_usage.ru_nvcsw << endl;
    cout << setw(32) << " Involuntary context switches:" << res_usage.ru_nivcsw << endl;

    /* recuperiamo la memoria occupata dal processo
     * Purtroppo l'unico modo per recuperare questa informazione e` parsare
     * /proc/<PID>/status (viene creato un symlink su da /proc/<PID> a /proc/self
     * quando avviamo un programma dal procfs che ci semplifica un po' le cose)
     */
    std::ifstream self;
    self.open("/proc/self/status");
    if (self.is_open()) {
      string line;
      while (!self.eof()) {
        getline(self, line);
        if (!line.compare(0, 6, "VmSize")) {
          cout << setw(32) << " Total memory used:";
          /* siamo semplicistici, assumiamo semplicemente che gli unici numeri
           * all'interno della stringa siano quelli del consumo di kb di memoria
           */

          for (uint16_t i = 0; i < line.length(); i++)
            if (isdigit(line[i])) cout << line[i];
          cout << "kB" << endl;
          break;
        }
      }
      self.close();
    } else {
      cout << "Can't provide memory usage statistics, /proc/<PID>/status doesn't exist" << endl;
    }
#endif /* UNIX */
  }
};

/* Gestiamo il Ctrl+C su piattaforme linux */
#ifdef UNIX
#include <signal.h>
static void sigint_handler(int signal) {
  cout << "-- PROCESS INTERRUPTED --" << endl;
  throw AlgorithmError();
};
#endif /* UNIX */

/*
 * Da il comando all'algoritmo specificato
 */
int32_t
WorkingMemory::run(RuleSet *ruleset) {
  /* gestisci il ^C su linux */
#ifdef UNIX
  signal(SIGINT, &sigint_handler);
#endif /* UNIX */
  map<string, AlgoRunner> algorithms;

  algorithms["bfs"] = algo_bfs;
  algorithms["dfs"] = algo_dfs;
  algorithms["astar"] = algo_astar;
  algorithms["hill"] = algo_hill;

  string algo_name = this->options->get("algo")->get_value();

  if (algorithms.find(algo_name) == algorithms.end())
    throw WorkingMemoryError("invalid algorithm, choices are: 'bfs', 'dfs', 'astar' and 'hill'");

  cout << "Processing (algorithm: " << algo_name << ")..." << endl;

  try {
    return algorithms[algo_name](ruleset, &this->fact_initial, &this->fact_final, this->options);
  } catch (AlgorithmError) {
    return -1;
  }
};
