#include <stdlib.h>
#include <stdio.h>

#include "Ddictionary.h"


/*******************************************************************************
 * Extern functions
 */

/** Allocates a new DwordList object with a specified Dwordobject and adds
 * it to a specified list.
 *
 * \param list a pointer to the DwordList to which the new element should be 
 *    appended to.
 * \param word the Dword that should be used to create the new DwordList link.
 */
extern void
DwordList_add(DwordList** list, Dword* word)
{
    DwordList* newLink; 
    newLink = calloc(1, sizeof(DwordList));

    newLink->word = word;
    newLink->next = *list;

    *list = newLink;
}

extern bool
DwordList_exists(DwordList* list, char* word)
{
    while(list)
    {
        if(Dword_contains(list->word, word))
        {
            return true;
        }

        list = list->next;
    }

    return false;
}


/** Frees recursively a DwordList linked list.
 *
 * \param list the DwordList to be freed.
 */
extern void
DwordList_free(DwordList* list)
{
    if(list)
    {
        if(list->next)
        {
            DwordList_free(list->next);
        }

        free(list);
    }
}
