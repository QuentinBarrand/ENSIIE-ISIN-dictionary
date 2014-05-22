#ifndef DDICTIONARY_H
#define DDICTIONARY_H


#include <stdbool.h>

#include "Dnode.h"


// Those declarations are necessary because of circular dependencies
typedef struct _Dword Dword;
typedef struct _DwordList DwordList;

/*******************************************************************************
 * Dword type
 */

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

/*******************************************************************************
 * DwordList type
 */

/** Linked list allowing us to represent a one-to-many relationship between
 * a base Dnode and its derivatives and synonyms.
 */
typedef struct _DwordList
{
    /** The next element in the linked list. */
    struct _DwordList* next;

    /** The Dnode element pointed by this linked list element. */
    Dword* word;
} DwordList;

typedef struct _Dword Dword;

/*******************************************************************************
 * Ddictionary type
 */

/** A container for the N-ary tree and other things. */
typedef struct _Ddictionary
{
    /** The dictionary itself : a tree of Dnode. */
    Dnode* tree;

    /** A hastable of DwordList. The key of the hashtable is the multiplication
     * of two consecutive letters that are present in the DwordList's words.
     */
    DwordList* hashTable[ALPHABET_SIZE * ALPHABET_SIZE + ALPHABET_SIZE];

    /** A counter that allows counting the number of iterations that are
     * necessary to run a request (`-d` option).
     */
    int counter;
} Ddictionary;


/*******************************************************************************
 * Ddictionary extern functions
 */

extern Ddictionary*
Ddictionary_new();

extern void
Ddictionary_free(Ddictionary*);

extern Dword*
Ddictionary_getOrAddWord(Ddictionary*, char*);


/*******************************************************************************
 * DwordList extern functions
 */

extern void
DwordList_add(DwordList**, Dword*);

extern bool
DwordList_contains(DwordList*, char*);

extern void
DwordList_free(DwordList*);


/*******************************************************************************
 * Dword extern functions
 */

extern void
Dword_addBase(Dword*, Dword*);

extern void
Dword_addSynonym(Dword*, Dword*);

extern bool
Dword_contains(Dword*, char*);

extern bool
Dword_doesMatch(Dword*, char*);

extern bool
Dword_equals(Dword*, Dword*);

extern void
Dword_free(void*);

extern Dword*
Dword_new();


#endif
