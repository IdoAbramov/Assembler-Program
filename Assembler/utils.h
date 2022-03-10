#ifndef UTILS_H
#define UTILS_H
/* includes the standart libraries which are used in the program */
#include <stdio.h> 
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

/* includes the tables structures */
#include "dataTable.h"
#include "codeTable.h"
#include "labelsTable.h"
#include "externalsTable.h"
#include "errorsTable.h"

/* constants values */
#define FAILED 0
#define SUCCESS 1
#define SPACE_FOR_END_OF_STRING 1 /*the extra space for null-terminator char*/
#define EXTERNAL_ADDRESS 0 /*the address of an external label in the code image*/
#define NO_FILES 1 /*used for checking the number of input files. if the number is 1, means no input files*/
#define TWO_OPERANDS 2
#define ONE_OPERAND 1
#define NO_OPERANDS 0
#define OPCODE_COUNT 1 /*used for calculation of how many code words added into the code table except of source and destination operands*/
#define NUM_OF_OPCODES 16 /*number of valid opcodes in the program*/
#define NUM_OF_REGISTERS 8 /*number of valid registers in the program*/
#define NUM_OF_ADDRESS_MODES 4 /*number of valid addressing modes in the program*/
#define NUM_OF_DIRECTIVES 4 /*number of valid directives in the program*/
#define NULL_ADDRESS_MODE 0 /*used for opcodes code words with no source or/and no destination address modes */

/* constant limitations */
#define MEMORY_SIZE 4096 /*maximum memory size*/
#define LINE_MAX_LENGTH 81 /*maximum line length*/
#define LABEL_MAX_LENGTH 32 /*the label max length including the ':' ending*/
#define MAX_FILE_NAME_LEGNTH 100 /*the input file maximum name length without ".as" ending*/
#define IC_START 100 /*starting address for the code image*/
#define DC_START 0 /*starting address for the data image*/
#define FIRST_LINE 1 
#define MAX_NUMBER_SIZE 2048 /*the maximum number which can be represented as 12 bits signed number (2^11)*/
#define MIN_NUMBER_SIZE -2048 /*the minumum number which can be represented as 12 bits signed number (2^11)*/

/* enumerations */
typedef enum {FALSE = 0,TRUE = 1} boolean; /*used for boolean variable definition*/
typedef enum {AS = 1,OB = 2,ENT = 3,EXT = 4} fileType;/*used for files types*/
typedef enum {NO_ADDRESS_MODE = -1, IMMEDIATE = 0, DIRECT = 1, RELATIVE = 2, REGISTER_DIRECT = 3} addressingMode;/*used for addressing mode types*/


/* extern global variables to use in the program */
extern int IC;
extern int DC;
extern int ICF;
extern int DCF;
extern int lineCounter;

/* extern global arrays to use in the program */
extern const char * opcodes[];
extern const char * registers[];
extern const char * directives[];
extern const opcodeAllowedModes opcodeAddressModes[NUM_OF_OPCODES];

/* extern global pointers to structures to use in the program */
extern codeWordNode * codeWordPtrHead;
extern dataWordNode * dataWordPtrHead;
extern errorNode * errorPtrHead;
extern labelNode * labelPtrHead;
extern externalNode * extrenalPtrHead;


/* Utils Functions and Documentation */

/*
*The function terminates the program when a memory allocation failure occurred
*/
void memoryAllocationFailure();

/**
*The function cuts the next word from line string into word string, without considering any whitespaces.
*@param line - the full line string ends with '\0'
*@param word - the full word string which will contain the next word from the line
*/
void getNextWordInLine(char * line, char * word);

/**
*The function gets a start of file name and its type and returns the full file name with its matching end of file name
*@param startOfFile - a string which contains the name of a file before the '.'
*@param fileType - an enumerated value which represents the file requested end of file
*@return a string of the full file name 
*/
char * getFileName(char * startOfFile, int fileType);


#endif
