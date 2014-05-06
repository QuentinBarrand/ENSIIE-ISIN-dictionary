#include <stdlib.h>
#include <string.h>

#include "Dword.h"

extern void
Dword_addBase(Dword* word, Dword* base)
{
	// TODO
}

extern void
Dword_addDerivative(Dword* word, Dword* derivative)
{
	// TODO
}

extern void
Dword_addSynonym(Dword* word, Dword* synonym)
{
	// TODO
}

extern Dword*
Dword_new(char* word)
{
	Dword* newDword;
	newDword = malloc(sizeof(Dword));

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

	free(word);
}

extern void
Dword_printBases(Dword* word)
{
    DwordList_print(word->bases);
}

extern void
Dword_printDerivatives(Dword* word)
{
    DwordList_print(word->derivatives);
}

extern void
Dword_printSynonyms(Dword* word)
{
    DwordList_print(word->synonyms);
}