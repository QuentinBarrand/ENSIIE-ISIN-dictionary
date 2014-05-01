#ifndef DNODE_H
#define DNODE_H

typedef struct _Dnode {
    char word[20];

    struct _Dnode* children[26];

    struct _Dnode** derivatives;
    struct _Dnode** synonyms;
} Dnode;


extern Dnode* 
Dnode_new();

extern Dnode* 
Dnode_getOrAdd(char* word);

extern void 
Dnode_addDerive(Dnode* tree, char* base, char* derive);

extern void 
Dnode_addSynonym(Dnode* tree, char* base, char* derive);

extern void 
Dnode_free(Dnode* tree);

#endif