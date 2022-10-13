#ifndef LABELS_TABLE_H
#define LABELS_TABLE_H

/*define new variable type of label type*/
typedef enum {NO_TYPE = 0,
	      DATA = 1,
	      CODE = 2,
	      EXTERN = 3,
	      CODE_ENTRY = 4,
	      DATA_ENTRY =5} labelType;

/*structure of label node contains its name, address and its label type.
also has pointer for the next label node in the Labels Table*/
typedef struct labelNode{
	char* name;
	int address;
	labelType type; 
	struct labelNode* next;
}labelNode;

#endif
