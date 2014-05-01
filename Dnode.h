#ifndef DNODE_H
#define DNODE_H

typedef struct tree_element {
	char word[20];

	struct tree_element* children[26];

	struct tree_element** derivatives;
	struct tree_element** synonyms;
} Dnode;


extern Dnode* Dnode_new();

extern Dnode* Dnode_getOrAdd(char* word);

extern void Dnode_addDerive(Dnode* tree, char* base, char* derive);
extern void Dnode_addSynonym(Dnode* tree, char* base, char* derive);

extern void Dnode_free(Dnode* tree);

#endif