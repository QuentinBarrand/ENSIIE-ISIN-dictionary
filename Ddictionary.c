#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "Ddictionary.h"


/*******************************************************************************
 * Extern functions
 */

/** Frees recursively a Dnode dictionary and all of its dynamically allocated
 * attributes.
 *
 * \param dict The Dnode dictionary to be freed.
 */
extern void
Ddictionary_free(Ddictionary* dict)
{
    int i;

    Dnode_free(dict->tree, Dword_free);

    // Free all the hashtable's lists.
    for(i = 0; i < ALPHABET_SIZE * ALPHABET_SIZE; i++)
    {
        DwordList_free(dict->hashTable[i]);
    }

    free(dict);
}

/** Gets the Dword object for a given word in the given dictionary, or creates
 * it if it does not exist yet.
 *
 * \param dict the dictionary to look into.
 * \param word the word to look for in the give dictionary.
 * \param addToList a boolean variable. If `true`, the word is added to the
 *    Ddictionary.words list.
 *
 * \returns A pointer to the Dword object that contains the queried word.
 */
extern Dword*
Ddictionary_getOrAddWord(Ddictionary* dict, char* word)
{
    char tempWord[MAX_WORD_SIZE];
    int c, c1, c2, nbChars;
    int i;

    Dnode* tree;
    tree = dict->tree;

    nbChars = strlen(word);

    for(i = 0; i < nbChars; i++)
    {
        c = word[i];

        strncpy(tempWord, word, i + 1);
        tempWord[i + 1] = '\0';

        if(! tree->children[c - 'a'])
        {
            tree->children[c - 'a'] = Dnode_new();
            tree->children[c - 'a']->element = Dword_new(tempWord);
        }

        tree = tree->children[c - 'a'];

        dict->counter++;
    }

    i = 0;

    while(word[i] != '\0' && word[i + 1] != '\0')
    {
        c1 = word[i] - 'a';
        c2 = word[i + 1] - 'a';

        if(! DwordList_exists(dict->hashTable[(c1 * ALPHABET_SIZE) + c2], 
            tree->element))
        {
            DwordList_add(&dict->hashTable[(c1 * ALPHABET_SIZE) + c2], 
                ((Dword*)tree->element));
        }

        i++;
    }

    ((Dword*)tree->element)->isWord = true;

    return ((Dword*)tree->element);
}

/** Allocates a new Ddictionary object in memory, and set it Ddicitonary.tree to
 * a new Dnode using Dnode_new().
 *
 * \returns A new, freshly allocated Ddictionary object.
 */
extern Ddictionary*
Ddictionary_new()
{
    Ddictionary* newDic;
    newDic = calloc(1, sizeof(Ddictionary));

    newDic->tree = Dnode_new();

    return newDic;
}
