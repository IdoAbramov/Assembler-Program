#ifndef DATA_TABLE_H
#define DATA_TABLE_H

/*definitions of constants*/
#define DATA_WORD_SIZE 12

/***** Data Table Definitions *****/

/*structure of number data word value contains a bit-field value in size of a data word */
typedef struct {
	unsigned int value : DATA_WORD_SIZE;
} dataNumberWord;

/*structure of char data word value contains a bit-field value in size of a data word */
typedef struct {
	unsigned int value : DATA_WORD_SIZE;
} dataAsciiWord;

/*structure of data word contains its address, the value(depands on the data word type) and the ARE value. 
also has pointer for the next data word in the Data Table*/
typedef struct dataWordNode{
	int dataAddress;
	union {
		dataNumberWord number;
		dataAsciiWord ascii;
	} value;
	char areValue;
	struct dataWordNode* next;
} dataWordNode;

#endif
