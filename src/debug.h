#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

//#define DEBUG

#ifdef DEBUG
  using ::std::cerr;
  using ::std::endl;
#	define debug(str) cerr << \
	"[" << __func__ << "] " << str << endl
#	else
#	define debug(str)
#	endif
#endif /* DEBUG_H */
