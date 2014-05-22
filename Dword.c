#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "Ddictionary.h"

/*******************************************************************************
 * Extern functions
 */

/** Adds a word as another word's base and vice versa.
 *
 * \param word the word to add a base to.
 * \param base the word that should be added as a base.
 */
extern void
Dword_addBase(Dword* word, Dword* base)
{
    DwordList_add(&word->bases, base);
    DwordList_add(&base->derivatives, word);
}

/** Adds a synonym relation between two words. They will be added to each
 * other's synonyms list.
 *
 * \param word the first word of the relation.
 * \param synonym the second word of the relation.
 */
extern void
Dword_addSynonym(Dword* word, Dword* synonym)
{
    DwordList* list;

    // Add synonym to the current word's synonyms list
    DwordList_add(&word->synonyms, synonym);    
    DwordList_add(&synonym->synonyms, word);

    /* For each synonym of the current word, 
    * add the new synonym to them and add them to the new synonym
    */
    list = word->synonyms;

    do
    {
        if(! Dword_equals(list->word, synonym))
        {
            DwordList_add(&list->word->synonyms, synonym);            
        }

        if(! DwordList_contains(synonym->synonyms, list->word->word) &&
           ! Dword_equals(synonym, list->word))
        {
            DwordList_add(&synonym->synonyms, list->word);
        }

        list = list->next;
    } while(list);
}

/** Checks if the specified Dword object contains a string.
 *
 * \param word the word to check into.
 * \param string the string to compare to Dword.word.
 *
 * \returns `true` if the Dword.word contains the string, else `false`.
 */
extern bool
Dword_contains(Dword* word, char* string)
{
    if(strcmp(word->word, string) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/** Compares a word to a regular expression.
 *
 * \param word the Dword to be checked.
 * \param regex a string containing a regular expression to try to match to the
 *    the Dword.
 *
 * \returns `true` if the regex matches the specified Dword, else `false`.
 */
extern bool
Dword_doesMatch(Dword* word, char* regex)
{
    bool status;
    status = true;

    int i, j;
    i = j = 0;

    while(word->word[i] != '\0' && regex[j] != '\0')
    {
        // If the two current chars are identical, look at the next ones
        if(word->word[i] == regex[j])
        {
            // If this is the last character in regex but not in word
            if(regex[j + 1] == '\0' && word->word[i + 1] != '\0')
            {
                status = false;
            }
            else
            {
                status = true;
            }

            i++;
            j++;
        }
        // If we're having the wildcard char *
        else if(regex[j] == '*')
        {
            // If * is the last character in regex
            if(regex[j + 1] == '\0')
            {
                return true;
            }

            status = false;

            // If the regex's next char matches the word's current
            if(regex[j + 1] == word->word[i])
            {
                j++;
            }
            else
            {
                i++;
            }
        }
        // If we're having two charaters that don't match
        else
        {
            return false;
        }
    }
    
    return status;
}

/** Compares two words.
 *
 * \param word1 the first word to be compared.
 * \param word2 the second word to be compared.
 *
 * \returns `true` if the two words are identical, else `false`.
 */
extern bool
Dword_equals(Dword* word1, Dword* word2)
{
    if(Dword_contains(word1, word2->word))
    {
        return true;
    }

    return false;
}


/** Frees a word and its bases, derivatives and synonyms lists recursively.
 *
 * \param void_word the Dword to be freed. Because this function is called  by
 *    Dnode_free() using its pointer, its argument must be of generic type
 *    void*.
 */
extern void
Dword_free(void* void_word)
{
    Dword* word;
    word = (Dword*)void_word;

    DwordList_free(word->bases);
    DwordList_free(word->derivatives);
    DwordList_free(word->synonyms);

    free(word);
}

/** Allocates a new Dword object and puts the argument in its word field.
 *
 * \param word a character string to initialize the word field with.
 *
 * \returns A new, freshly allocated Dword object.
 */
extern Dword*
Dword_new(char* word)
{
    Dword* newDword;
    newDword = calloc(1, sizeof(Dword));

    strcpy(newDword->word, word);

    return newDword;
}
