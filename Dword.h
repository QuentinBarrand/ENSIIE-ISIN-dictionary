#ifndef WORD_H
#define WORD_H


#include <stdbool.h>


#include "DwordList.h"
#include "Dutils.h"

typedef struct _DwordList DwordList;

/** Structure containing representing a word. */
typedef struct _Dword
{
    /** The actual dictionary word member that's contained into the Dnode. */
    char word[MAX_WORD_SIZE];

    /** Boolean. Value is `0` if the node does not contain a word, else `1`. */
    bool isWord;

    /** DwordList of the word's bases. */
    DwordList* bases;

    /** DwordList of the word's derivatives. */
    DwordList* derivatives;

    /** DwordList of the word's synonyms. */
    DwordList* synonyms;
} Dword;

extern void
Dword_addBase(Dword*, Dword*);

extern void
Dword_addDerivative(Dword*, Dword*);

extern void
Dword_addSynonym(Dword*, Dword*);

extern void
Dword_free(void*);

extern bool
Dword_doesMatch(Dword*, char*);

extern Dword*
Dword_new();

extern void
Dword_printBases(Dword*);

extern void
Dword_printDerivatives(Dword*);

extern void
Dword_printSynonyms(Dword*);

#endif
