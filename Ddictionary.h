#ifndef DDICTIONARY_H
#define DDICTIONARY_H


#include "Dconfig.h"
#include "Dnode.h"


extern Dconfig* 
Ddictionary_parseArgs(int, char**);

extern Dnode*
Ddictionary_processArgs(Dconfig*);

extern void
Ddictionary_runInteractive(Dconfig*, Dnode*);

#endif