#include <stdlib.h>
#include <string.h>

#include "Dnode.h"


/*******************************************************************************
 * Extern functions
 */

extern void 
Dnode_addBaseToDerivative(Dnode* tree, char* derivative, char* base)
{
    // TODO
}

extern void 
Dnode_addDerivativeToBase(Dnode* tree, char* base, char* derivative)
{
    // TODO
}

extern void 
Dnode_addSynonym(Dnode* tree, char* base, char* derive)
{
    // TODO
}

extern void 
Dnode_free(Dnode* tree)
{
    
    // TODO

}

extern Dnode*
Dnode_getOrAddWord(Dnode* tree, char* word)
{
    char c;
    uint i;

    Dnode* currentNode;
    currentNode = tree;

    for(i = 0; i < strlen(word); i++)
    {
        c = word[i];
        
        if(currentNode->children[c - 'a'] == NULL)
        {
            currentNode->children[c - 'a'] = malloc(sizeof(Dnode));
        }

        currentNode = currentNode->children[c - 'a'];
        strncpy(currentNode->word, word, i);
    }

    strncpy(currentNode->word, word, i);

    return currentNode;
}

extern Dnode* 
Dnode_new()
{
    return calloc(1, sizeof(Dnode));
}

extern void
Dnode_print(Dnode* tree)
{
    int i;

    printf("%s\n", tree->word);

    for(i = 0; i < 26; i++)
    {
        if(tree->children[i] != NULL)
        {
            printf("\t");
            Dnode_print(tree->children[i]);
        }
    }
}