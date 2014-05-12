#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "Dword.h"

/** Adds a word as another word's base and vice versa.
 *
 * \param word the word to add a base to.
 * \param base the word that should be added as a base.
 */
extern void
Dword_addBase(Dword* word, Dword* base)
{
    DwordList_add(&word->bases, base);
}

/** Adds a word as another word's derivative and vice versa.
 *
 * \param word the word to add a derivative to.
 * \param derivative the word that should be added as a derivative.
 */
extern void
Dword_addDerivative(Dword* word, Dword* derivative)
{
    DwordList_add(&word->derivatives, derivative);
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
    DwordList_add(&word->synonyms, synonym);
    DwordList_add(&synonym->synonyms, word);
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

/** Compares a word to a regular expression.
 *
 * \param word the Dword to be checked.
 * \param regex a string containing a regular expression to try to match to the
 *    the Dword.
 *
 * \returns `1` if the regex matches the specified Dword, else `0`.
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

/** Prints the list of the argument's bases on the standard output.
 *
 * \param word the word of which bases should be printed.
 */
extern void
Dword_printBases(Dword* word)
{
    if(word->bases)
    {
        DwordList_print(word->bases);
    }
}

/** Prints the list of the argument's derivatives on the standard output.
 *
 * \param word the word of which derivatives should be printed.
 */
extern void
Dword_printDerivatives(Dword* word)
{
    if(word->derivatives)
    {
        DwordList_print(word->derivatives);
    }
}

/** Prints the list of the argument's synonyms on the standard output.
 *
 * \param word the word of which synonyms should be printed.
 */
extern void
Dword_printSynonyms(Dword* word)
{
    if(word->synonyms)
    {
        DwordList_print(word->synonyms);
    }
}
