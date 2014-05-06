#ifndef DNODELIST_H
#define DNODELIST_H


#include <stdio.h>

#include "Dnode.h"

typedef struct _Dnode Dnode;


/** Linked list allowing us to represent a one-to-many relationship between
 * a base Dnode and its derivatives and synonyms.
 */
typedef struct _DnodeList
{
    /** The next element in the linked list. */
    struct _DnodeList* next;

    /** The Dnode element pointed by this linked list element. */
    struct _Dnode* node;
} DnodeList;

extern void
DnodeList_add(DnodeList**, Dnode*);

extern void
DnodeList_free(DnodeList*);

extern void
DnodeList_print(DnodeList* list);

#endif