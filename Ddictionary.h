#ifndef DDICTIONARY_H
#define DDICTIONARY_H


#include "Dnode.h"
#include "Dword.h"

/** A container for the N-ary tree and other things. */
typedef struct _Ddictionary
{
    /** The dictionary itself : a tree of Dnode. */
    Dnode* tree;

    #warning doc à refaire
    /** A list of all the proper words contained into the tree. */
    DwordList* hashTable[ALPHABET_SIZE * ALPHABET_SIZE + ALPHABET_SIZE];

    int counter;
} Ddictionary;


extern Ddictionary*
Ddictionary_new();

extern void
Ddictionary_free(Ddictionary*);

extern Dword*
Ddictionary_getOrAddWord(Ddictionary*, char*);

#endif
