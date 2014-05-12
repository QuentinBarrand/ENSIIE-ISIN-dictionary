#include <stdlib.h>
#include <stdio.h>

#include "DwordList.h"

#include "Dword.h"

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

/** Prints the word in the DwordList object.
 *
 * \param list the list containing the Dword->word to be printed.
 */
extern void
DwordList_print(DwordList* list)
{
        if(list->next)
        {
            DwordList_print(list->next);
        }

        printf("\t%s\n", list->word->word);
}