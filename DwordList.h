#ifndef DNODELIST_H
#define DNODELIST_H


#include "Dword.h"


typedef struct _Dword Dword;


/** Linked list allowing us to represent a one-to-many relationship between
 * a base Dnode and its derivatives and synonyms.
 */
typedef struct _DwordList
{
    /** The next element in the linked list. */
    struct _DwordList* next;

    /** The Dnode element pointed by this linked list element. */
    Dword* word;
} DwordList;

extern void
DwordList_add(DwordList**, Dword*);

extern void
DwordList_free(DwordList*);

extern void
DwordList_print(DwordList*);

#endif