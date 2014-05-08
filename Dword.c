#include <stdlib.h>
#include <string.h>

#include "Dword.h"

extern void
Dword_addBase(Dword* word, Dword* base)
{
    DwordList_add(&word->bases, base);
    DwordList_add(&base->bases, word);
}

extern void
Dword_addDerivative(Dword* word, Dword* derivative)
{
    DwordList_add(&word->derivatives, derivative);
    DwordList_add(&derivative->derivatives, word);
}

extern void
Dword_addSynonym(Dword* word, Dword* synonym)
{
    #ifdef DEBUG
    printf("Adding synonym %s to word %s\n", word->word, synonym->word);
    #endif

    DwordList_add(&word->synonyms, synonym);
    DwordList_add(&synonym->synonyms, word);
}

extern Dword*
Dword_new(char* word)
{
    Dword* newDword;
    newDword = calloc(1, sizeof(Dword));

    strcpy(newDword->word, word);

    return newDword;
}

extern void
Dword_free(void* void_word)
{
    Dword* word;
    word = (Dword*)void_word; 

    DwordList_free(word->bases);
    DwordList_free(word->derivatives);
    DwordList_free(word->synonyms); 

    #ifdef DEBUG
    printf("Freeing word %s\n", word->word);
    #endif

    free(word);
}

extern void
Dword_printBases(Dword* word)
{
    if(word->bases)
    {
        DwordList_print(word->bases);        
    }
}

extern void
Dword_printDerivatives(Dword* word)
{
    if(word->derivatives)
    {
        DwordList_print(word->derivatives);        
    }
}

extern void
Dword_printSynonyms(Dword* word)
{
    if(word->synonyms)
    {
        DwordList_print(word->synonyms);        
    }
}