#ifndef CMDLINE_H
#define CMDLINE_H

#include <string>
#include <map>

#include "compiler.h"

using ::std::cout;
using ::std::endl;

#define DEEP_THOUGHT(str) \
	string douglas(str);    \
	for (unsigned int _i = 0; _i < 49; _i++) \
		douglas[_i] = douglas[_i] - 1;
#define THIS_IS_THE_QUESTION "vmujnbuf.rvftujpo.pg.mjgf.vojwfstf.boe.fwfszuijoh"
#define LET_START DEEP_THOUGHT(THIS_IS_THE_QUESTION)
#define I_FINALLY_HAVE_THE_ANSWER_SO LET_START

class _ComplexComputation {};

enum CMDLINE_PTYPE {
	CMDLINE_REQUIRED,
	CMDLINE_OPTIONAL,
	CMDLINE_BOOL
};

/* Contiene la gestione del parsing della command line */
class CmdLineParser {
	private:
		int argc;
		char **argv;
		string special;

		map<string, CMDLINE_PTYPE> params;

	public:
		map<string, string> parsed;

		CmdLineParser(int argc, char **argv) :
			argc(argc),
			argv(argv)
		{
			this->params["input"] = CMDLINE_REQUIRED;
			this->params["algo"] = CMDLINE_OPTIONAL;
			this->params["limit"] = CMDLINE_OPTIONAL;
#ifdef UNIX
			this->params["show-stats"] = CMDLINE_BOOL;
#endif

			I_FINALLY_HAVE_THE_ANSWER_SO;
			this->params[douglas] = CMDLINE_BOOL;
			this->special = douglas;
		};

		bool parse() {
			/* saltiamo argv[0] */
			int16_t i = 1;
			string value;

			while (i < this->argc) {
				string lead(argv[i]);
				lead.resize(2);

				/* controlla che sia una opzione */
				if (!lead.compare("--")) {
					/* rimuovi i trattini davanti */
					string pname(argv[i]);
					pname = pname.substr(2);
					
					/* se il parametro non esiste usciamo con errore */
					if (this->params.find(pname) == this->params.end())
						return false;

					/* se il parametro richiede dei valori, recuperiamoli */
					if (this->params[pname] != CMDLINE_BOOL)
						value = string(argv[++i]);
					this->parsed[pname] = value;
				} else return false;

				i++;
			}

			if (this->parsed.find(this->special) != this->parsed.end()) {
				cout << "42" << endl;
				throw _ComplexComputation();
			}

			/* controlliamo che i parametri obbligatori ci siano tutti e che non ci
			 * siano parametri sconosciuti
			 */
			map<string, CMDLINE_PTYPE>::iterator iter = this->params.begin();
			while (iter != this->params.end()) {
				if (this->params.find(iter->first) == this->params.end())
					return false;

				if ((iter->second == CMDLINE_REQUIRED) &&
						(this->parsed.find(iter->first) == this->parsed.end())) {
					return false;
				}
				iter++;
			}

			return true;
		};

		string& operator[](string key) {
			return this->parsed[key];
		};
};

#endif /* CMDLINE_H */
