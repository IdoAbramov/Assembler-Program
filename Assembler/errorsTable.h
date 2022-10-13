#ifndef ERRORS_TABLE_H
#define ERRORS_TABLE_H

/*structure of error node contains its description, the line and the file which the error had occurred.
also has pointer for the next error node in the Errors Table*/
typedef struct errorNode{
	char* description;
	int line;
	char* file;
	struct errorNode* next;
}errorNode;

#endif
