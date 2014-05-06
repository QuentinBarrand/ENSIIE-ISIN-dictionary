#ifndef DNODE_H
#define DNODE_H

/** Project's subject allows us to set the word's maximum length. */
#define MAX_WORD_SIZE 20


/** 26-ary tree representing the dictionary. */
typedef struct _Dnode {
    /** The node's element. */
    void* element;

    /** This is a 26-ary tree, one children for each one of the alphabet 
     * letter.
     */
    struct _Dnode* children[26];
} Dnode;


extern void 
Dnode_free(Dnode*, void (*freeElement)(void*));

extern Dnode* 
Dnode_new();


#endif
