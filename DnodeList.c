#include <stdlib.h>

#include "DnodeList.h"

#include "Dnode.h"

/** Allocates a new DnodeList object with a specified Dnode object and adds
 * it to a specified list.
 *
 * \param list a pointer to the DnodeList to which the new element should be 
 *    appended to.
 * \param node the Dnode that should be used to create the new DnodeList link.
 */
extern void
DnodeList_add(DnodeList** list, Dnode* node)
{
    DnodeList* newLink; 
    newLink = malloc(sizeof(DnodeList));

    newLink->node = node;
    newLink->next = *list;

    *list = newLink;
}

extern int
DnodeList_count(DnodeList* list)
{
    int i;
    i = 0;

    while(list != NULL && list->next != NULL)
    {
        i++;
        list = list->next;
    }

    return i;
}

/** Frees recursively a DnodeList linked list.
 *
 * \param list the DnodeList to be freed.
 */
extern void
DnodeList_free(DnodeList* list)
{
    if(list != NULL)
    {
        if(list->next != NULL)
        {
            DnodeList_free(list->next);
        }

        free(list);
    }
}
