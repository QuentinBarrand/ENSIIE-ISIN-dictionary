#ifndef DDICTIONARY_H
#define DDICTIONARY_H


#include "Dconfig.h"
#include "Dnode.h"


extern Dnode*
Ddictionary_create(Dconfig*);

extern void 
Ddictionary_parseArgs(Dconfig*, int, char**);

extern void
Ddictionary_processArgs(Dconfig*);

extern void
Ddictionary_runInteractive(Dconfig*, Dnode*);

#endif