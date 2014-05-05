#ifndef DNODE_H
#define DNODE_H

#include "DnodeList.h"


/** Project's subject allows us to set the word's maximum length. */
#define MAX_WORD_SIZE 20


/** 26-ary tree representing the dictionary. */
typedef struct _Dnode {
    /** The actual dictionary word member that's contained into the Dnode. */
    char word[MAX_WORD_SIZE];

    /** Boolean. Value is `0` if the node does not contain a word, else `1`. */
    char isWord;

    /** This is a 26-ary tree, one children for each one of the alphabet 
     * letter.
     */
    struct _Dnode* children[26];

    struct _DnodeList* bases;
    struct _DnodeList* derivatives;
    struct _DnodeList* synonyms;
} Dnode;


extern void 
Dnode_addBaseToDerivative(Dnode*, char*, char*);

extern void 
Dnode_addDerivativeToBase(Dnode*, char*, char*);

extern void 
Dnode_addSynonym(Dnode*, char*, char*);

extern void 
Dnode_free(Dnode*);

extern Dnode* 
Dnode_getOrAddWord(Dnode*, char*);

extern Dnode* 
Dnode_new();

extern void 
Dnode_print(Dnode*);


#endif
