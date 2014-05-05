#include <stdlib.h>

#include "DnodeList.h"

#include "Dnode.h"

extern void
DnodeList_Add(DnodeList** list, Dnode* node)
{
    DnodeList* oldList = *list;

    DnodeList* newLink; 
    newLink = malloc(sizeof(DnodeList));
    newLink->node = node;
    newLink->next = oldList;

    list = &newLink;
}

extern void
DnodeList_Free(DnodeList* list)
{
    if(list->next != NULL)
    {
        DnodeList_Free(list->next);
    }

    free(list);
}
