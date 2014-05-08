#ifndef DDICTIONARY_H
#define DDICTIONARY_H


#include "Dconfig.h"
#include "Dnode.h"

#define DICTIONARY_SIZE 26

extern void 
Ddictionary_free(Dnode*);

extern int 
Ddictionary_parseArgs(Dconfig*, int, char**);

extern int
Ddictionary_processArgs(Dconfig*, Dnode*);

extern void
Ddictionary_runInteractive(Dconfig*, Dnode*);


#endif
