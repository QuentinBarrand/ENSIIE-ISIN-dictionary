#ifndef DDICTIONARY_H
#define DDICTIONARY_H


#include "Dconfig.h"
#include "Dnode.h"
#include "Dword.h"

/** A container for the N-ary tree and othr things. */
typedef struct _Ddictionary
{
    /** The dictionary itself : a tree of Dnode. */
    Dnode* tree;

    /** A list of all the proper words contained into the tree. */
    DwordList* words; 
} Ddictionary;


extern Ddictionary*
Ddictionary_new();

extern void
Ddictionary_free(Ddictionary*);

extern int
Ddictionary_processArgs(Dconfig*, Ddictionary*);

extern void
Ddictionary_runInteractive(Dconfig*, Ddictionary*);


#endif
