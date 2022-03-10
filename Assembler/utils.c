#include "utils.h"

/* declaring global variables */
int IC, DC, ICF, DCF,lineCounter;

/* declaring global constant arrays with values. contains the valid opcodes, registers and directives*/
const char * opcodes[] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop"};
const char * registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
const char * directives[] = {".data", ".string", ".extern", ".entry"};

/* declaring global constant array of all opcodes allowed modes structure which defines the 
opcode, funct, available source and destination addressing mode for each operation code, by flags: 1 - available(ON), 0 - not available(OFF)*/
const opcodeAllowedModes opcodeAddressModes[] = {
{"mov", 0, 0, {1,1,0,1}, {0,1,0,1}},
{"cmp", 1, 0, {1,1,0,1}, {1,1,0,1}},
{"add", 2, 10, {1,1,0,1}, {0,1,0,1}},
{"sub", 2, 11, {1,1,0,1}, {0,1,0,1}},
{"lea", 4, 0, {0,1,0,0}, {0,1,0,1}},
{"clr", 5, 10, {0,0,0,0}, {0,1,0,1}},
{"not", 5, 11, {0,0,0,0}, {0,1,0,1}},
{"inc", 5, 12, {0,0,0,0}, {0,1,0,1}},
{"dec", 5, 13, {0,0,0,0}, {0,1,0,1}},
{"jmp", 9, 10, {0,0,0,0}, {0,1,1,0}},
{"bne", 9, 11, {0,0,0,0}, {0,1,1,0}},
{"jsr", 9, 12, {0,0,0,0}, {0,1,1,0}},
{"red", 12, 0, {0,0,0,0}, {0,1,0,1}},
{"prn", 13, 0, {0,0,0,0}, {1,1,0,1}},
{"rts", 14, 0, {0,0,0,0}, {0,0,0,0}},
{"stop", 15, 0, {0,0,0,0}, {0,0,0,0}}
};


/* declaring global linked lists initialization. used for the code, data, errors, labels and external tables */
codeWordNode * codeWordPtrHead = NULL;
dataWordNode * dataWordPtrHead = NULL;
errorNode * errorPtrHead = NULL;
labelNode * labelPtrHead = NULL;
externalNode * extrenalPtrHead = NULL;


/***** Utils Functions *****/

void memoryAllocationFailure()
{
	printf("\nError - memory allocation failed.\nExiting program.\n");
	exit(0);/*terminates the program after memory allocation has failed*/
}

void getNextWordInLine(char * line, char * word)
{
	int i=0, j=0;

	while(line[i] != '\0' && isspace(line[i])) /*skips the whitespaces*/
	{
		i++;
	}

	while(line[i] != '\0' && !isspace(line[i])) /*copies char by char from the line to the word string, until it encounters another whitespace or end of string*/
	{
		word[j] = line[i];
		i++;
		j++;
	}
	word[j] = '\0';/*puts a null-terminator at the end of word string*/
		
	j=0;

	while(line[i] != '\0')/*reducing the line string from the taken word*/
	{
		line[j] = line[i];
		j++;
		i++;
	}
	line[j] = '\0';/*puts a null-terminator at the end of line string*/
}

char * getFileName(char * startOfFile, int fileType)
{
	char * fullFileName = (char *)malloc(MAX_FILE_NAME_LEGNTH);/*allocates full file name string with maximum capacity of max line name*/
	
	if(fullFileName == NULL)/*if allocation of full file name string has failed*/
	{
		memoryAllocationFailure();
	}
	
	strcpy(fullFileName, startOfFile);

	/*checks the file type and concatenates the file name with its correct ending*/
	if(fileType == AS)
	{
		strcat(fullFileName,".as");
	}
	else if(fileType == OB)
	{
		strcat(fullFileName,".ob");	
	}
	else if(fileType == ENT)
	{
		strcat(fullFileName,".ent");
	}
	else if(fileType == EXT)
	{
		strcat(fullFileName,".ext");
	}

	return fullFileName;/*returns the full file name with it's ending*/
}




