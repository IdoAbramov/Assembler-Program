/* Process Functions and Documentation */
#ifndef PROCESS_H
#define PROCESS_H

/**
*The function gets number of files and their names and performs the whole assembler process on each of them
*including the first and second rounds and the suitable outputs. 
*If errors occurred after first round and second round - prints the errors and loops into the next file
*@param numberOfFiles - contains the number of files in the input
*@param arrayOfFiles - contains the files' names input
*/
void startAssemblerProcess(int numberOfFiles, char * arrayOfFiles[]);

/**
*The function gets the current file pointer and its name. It performs the directives ".data", ".string" and ".extern" and adding labels into Labels Table
*@param f1 - the file's pointer
*@param fileName - the current file's name 
*/
void firstRound(FILE * f1, char * fileName);

/**
*The function gets the current file pointer and its name. It performs the operation codes and ".entry" directives
*@param f1 - the file's pointer
*@param fileName - the current file's name 
*/
void secondRound(FILE * f1, char * fileName);

/**
*The function gets an operation code name and returns how many operands it uses
*@param opcode - the name of the operation code
*@return the number of operands used by the operation code
*/
int opcodeNumOfOperands(const char * opcode);

/**
*The function takes care of operation code with two operands - source and destination. It gets the source and destination operands' address modes
*and performs coding of the operation code, source operand and the destination operand into code words and inserting them into Code Table
*@param opcode - operation code's name
*@param src - source operand name
*@param dest - destination operand name
*@param fileName - the current file's name 
*/
void twoOperandsOpcode(char * opcode, char * src, char * dest, char * fileName);

/**
*The function takes care of operation code with one operand - destination. It gets the destination operand's address mode
*and performs coding of the operation code and the destination operand into code words and inserting them into Code Table
*@param opcode - operation code's name
*@param dest - destination operand name
*@param fileName - the current file's name
*/
void oneOperandOpcode(char * opcode, char * dest, char * fileName);

/**
*The function takes care of operation code without any other operands. It performs coding it into code word and inserting it into Code Table
*@param opcode - operation code's name
*@param fileName - the current file's name
*/
void noOperandsOpcode(char * opcode, char * fileName);

/**
*The function gets line string and splitting it into two word for source operand name and destination operand name
*@param line - the line string input
*@param src - the string which will contain the soruce operand name
*@param dest - the string which will conatin the destination operand name
*@param fileName - the current file's name
*/
void getSrcAndDestOperands(char * line, char * src, char * dest, char * fileName);

/**
*The function gets the operation code name and source operand name. It returns the address mode of source operand
The function checks if the source operand is valid by the operation code name. If not returns NO_ADDRESS_MODE. 
*@param opcode - the operation code name
*@param src - the source operand name
*@return enumerated value of address mode
*/
addressingMode checkSrcOperandAddressMode(char * opcode, char * src);

/**
*The function gets the operation code name and destination operand name. It returns the address mode of destination operand
The function checks if the destination operand is valid by the operation code name. If not returns NO_ADDRESS_MODE. 
*@param opcode - the operation code name
*@param dest - the destination operand name
*@return enumerated value of address mode
*/
addressingMode checkDestOperandAddressMode(char * opcode, char * dest);

#endif


