#include <iostream>
#include <iomanip>

#include "./compiler.h"
#include "./global.h"
#include "./functions.h"
#include "./version.h"
#include "./parser/cmdline.h"
#include "./parser/parser.h"

using ::std::cout;
using ::std::cerr;

void print_header();
void print_usage(const char *);

int
main(int argc, char **argv) {
  cout << std::setprecision(5) << "ANT v" << ANT_VERSION << endl;

  CmdLineParser cmdline(argc, argv);

  if (!cmdline.parse()) {
    print_usage(argv[0]);
    return -1;
  }

  print_header();

  /* La classe Global inizializza la working memory. La stessa contiene metodi
   * per distruggere la working memory e per recuperarne un'istanza.
   */
  Global global;
  Parser parser(&global);
  if (!(parser.start(cmdline["input"].c_str())))
    return -1;

  cout << "Facts in memory: " << global.wm()->fact_number() << endl;
  cout << "Rules in memory: " << global.ruleset->rules_number() << endl;

  try {
    global.wm()->prepare(&cmdline);
    unsigned int expanded_nodes = global.wm()->run(global.ruleset);
    global.wm()->die(&cmdline, expanded_nodes);
  } catch(WorkingMemoryError wme) {
    cerr << "Working memory error: " << wme.err << endl;
  } catch(PredicateError perr) {
    cerr << "Predicate error: " << perr.err << endl;
  }

  return 0;
};

void
print_header() {
#ifdef UNIX
  cout << " ____________________________________________ " << endl;
  cout << "|                                            |" << endl;
  cout << "|  °\\ /°       ***     **    ** ********* ®  |" << endl;
  cout << "|  ('.')      *****    ****  ** *********    |" << endl;
  cout << "|  <( (>     **   **   ** ** **    ***       |" << endl;
  cout << "|   / \\     *********  **  ****    ***       |" << endl;
  cout << "|          ***     *** **   ***    ***       |" << endl;
  cout << "|            Another  Non-trivial  Tool      |" << endl;
  cout << "|____________________________________________|" << endl << endl;
#endif
};

void
print_usage(const char *prog_name) {
  cerr << "Usage: " << prog_name << endl;
  cerr << "  --input <inputfile>  Specify the path to the input file" << endl;
  cerr << " [--algo <algorithm>]  Which algorithm to use (choices are bfs, dfs, astar, hill)" << endl;
  cerr << " [--limit <limit>   ]  Set a limit for the algorithm, must be a numerical value" << endl;
#ifdef UNIX
  cerr << " [--show-stats      ]  Show usage statistics" << endl;
#endif
};
