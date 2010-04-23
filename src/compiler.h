#ifndef COMPILER_H
#define COMPILER_H

/*
 * Questo file contiene dichiarazioni compiler-dependent per ottimizzare
 * l'esecuzione del codice o per gestire parti di codice che si devono
 * comportare diversamente da linux a windows o altro
 */

/* Determiniamo l'ambiente, se UNIX-like oppure windows */

#if defined(__unix__) && defined(WIN32)
# error("Both UNIX and Win32 environment defined")
#endif
#if defined(__unix__)
# define UNIX
#elif defined(WIN32)
#else
# error("Unknown environment, can't compile")
#endif

/* Assumiamo che la funzione non lanci nessuna eccezione durante la sua
 * esecuzione. Questo permette al compilatore di non inserire il meccanismo
 * di stack unwind per le eccezioni quando specifichiamo questo parametro
 */
#if defined(__GNUC__)
# define __NOTHROW nothrow
# else
# define __NOTHROW
#endif

/* Forza l'inline della funzione */
#if defined(__GNUC__) && (__GNUC__ > 4)
# define __INLINE always_inline
#else
# define __INLINE
#endif

/* Ottimizza questa funzione piu` delle altre (utile *solo* se la funzione in
 * questione e` chiamata, letteralmente, milioni di volte
 * Disponibile a partire da GCC 4.3.
 */
#if defined(__GNUC__) && (__GNUC__ >= 4) && (__GNUC_MINOR >= 3)
# define __HOT hot
#else
# define __HOT
#endif


/* Generica macro per supportare piu` di un attributo alla volta */
#if defined(__GNUC__)
# define __ATTR(...) __attribute__((__VA_ARGS__))
#else
# define __ATTR(...)
#endif

#endif /* COMPILER_H */
