#ifndef DNODE_H
#define DNODE_H

/** Project's subject allows us to set the word's maximum length. */
#define MAX_WORD_SIZE 20


struct _DnodeList;
struct _Dnode;

/** Linked list allowing us to represent a one-to-many relationship between
 * a base Dnode and its derivatives and synonyms.
 */
typedef struct _DnodeList
{
    /** The next element in the linked list. */
    struct _DnodeList* next;

    /** The Dnode element pointed by this linked list element. */
    struct _Dnode* node;
} DnodeList;

/** 26-ary tree representing the dictionary. */
typedef struct _Dnode {
    /** The actual dictionary word member that's contained into the Dnode. */
    char word[MAX_WORD_SIZE];

    /** This is a 26-ary tree, one children for each one of the alphabet 
     * letter.
     */
    struct _Dnode* children[26];

    struct _DnodeList* derivatives;
    struct _DnodeList* synonyms;
} Dnode;


extern Dnode* 
Dnode_new();

extern Dnode* 
Dnode_getOrAdd(char*);

extern void 
Dnode_addDerive(Dnode*, char*, char*);

extern void 
Dnode_addSynonym(Dnode*, char*, char*);

extern void 
Dnode_free(Dnode*);


#endif
