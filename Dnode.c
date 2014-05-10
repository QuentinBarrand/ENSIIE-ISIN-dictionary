#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Dnode.h"


/*******************************************************************************
 * Extern functions
 */

/** Frees the specified Dnode using a specific function for the element member.
 *
 * \param node the Dnode to be recursively freed.
 * \param freeElement a pointer to the function used to free the element member.
 */
extern void
Dnode_free(Dnode* node, void (*freeElement)(void*))
{
    int i;

    for(i = 0; i < CHILDREN_NUMBER; i++)
    {
        if(node->children[i])
        {
            Dnode_free(node->children[i], freeElement);
        }
    }

    if(node->element)
    {
        freeElement(node->element);
    }

    free(node);
}

/** Allocates a new Dnode object in memory, and set its attributes to `0`.
 *
 * \returns A new, freshly allocated Dnode object.
 */
extern Dnode*
Dnode_new()
{
    return calloc(1, sizeof(Dnode));
}
