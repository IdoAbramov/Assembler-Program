#ifndef EXTERNALS_TABLE_H
#define EXTERNALS_TABLE_H

/*structure of external node contains its name and address.
also has pointer for the next external node in the External Labels Table*/
typedef struct externalNode{
	char *name;
	int address;
	struct externalNode * next;
}externalNode;

#endif
