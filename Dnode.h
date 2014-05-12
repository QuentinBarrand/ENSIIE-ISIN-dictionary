#ifndef DNODE_H
#define DNODE_H

/** The size of the `children` array containing the node's children. */
#define ALPHABET_SIZE 26

/** Project's subject allows us to set the word's maximum length. */
#define MAX_WORD_SIZE 20


/** N-ary tree representing the dictionary. */
typedef struct _Dnode {
    /** The node's element. */
    void* element;

    /** This is a N-ary tree, one children for each one of the alphabet
     * letter.
     */
    struct _Dnode* children[ALPHABET_SIZE];
} Dnode;


extern void
Dnode_free(Dnode*, void (*freeElement)(void*));

extern Dnode*
Dnode_new();


#endif
