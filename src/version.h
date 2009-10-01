#ifndef VERSION_H
#define VERSION_H

/*
 * Per cambiare numero di versione NON cambiare queste due righe,
 * piuttosto cambia il parametro di _VERSION nella definizione
 * di ANT_VERSION.
 */

#include <cmath>
#define _VERSION(x) log10(x)

/*
 * Cambia questo parametro per impostare il numero di versione
 * Es: per impostare la versione numero 5 scrivi
 * #define ANT_VERSION _VERSION(5)
 */
#define ANT_VERSION _VERSION(6)

#endif /* VERSION_H */
