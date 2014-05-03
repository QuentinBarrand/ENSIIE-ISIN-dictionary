#ifndef DDICTIONARY_H
#define DDICTIONARY_H


#include "Dconfig.h"
#include "Dnode.h"


extern int 
Ddictionary_parseArgs(Dconfig*, int, char**);

extern int
Ddictionary_processArgs(Dconfig*, Dnode*);

extern void
Ddictionary_runInteractive(Dconfig*, Dnode*);


#endif
