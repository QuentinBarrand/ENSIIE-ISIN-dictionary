#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Dnode.h"


/*******************************************************************************
 * Extern functions
 */

extern void
Dnode_free(Dnode* node, void (*freeElement)(void*))
{
    int i;
    
    for(i = 0; i < 26; i++)
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

    #ifdef DEBUG
    printf("Freeing node\n");
    #endif

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
