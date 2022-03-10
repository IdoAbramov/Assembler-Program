/* Tables Functions and Documentation - seperated to sections by purpose */
#ifndef TABLES_H
#define TABLES_H

/* Code Table Functions and Documentation */

/**
*The function gets operation code number, function number and both destination and source operands address modes and coding it as opcode word into Code Table
*@param opcode - the number of operation code
*@param funct - the number of function code
*@param destAddressMode - the destination operand address mode
*@param srcAddressMode - the source operand address mode
*/
void addOpcodeWordIntoCodeTable(int opcode, int funct, int destAddressMode, int srcAddressMode);

/**
*The function gets a register name and coding it as register word into Code Table
*@param reg - the register's name
*/
void addRegisterWordIntoCodeTable(char * reg);

/**
*The function gets a number's string and coding it as number word into Code Table
*@param num - the number's string
*/
void addNumberWordIntoCodeTable(char * num);

/**
*The function gets a label's name and coding its address as code word into Code Table
*@param label - the label's name
*/
void addDirectAddressWordIntoCodeTable(char * label);

/**
*The function gets a relative label's name, coding its address as the distance from the current instruction address to the label's address
*the distance coded as code word into Code Table
*@param label - the label's name
*/
void addRelativeAddressWordIntoCodeTable(char * label);

/**
*The function performs delete of code words in the Code Table
*/
void deleteCodeTable();



/* Data Table Functions and Documentation */

/**
*The function gets a string and file's name. It checks if the string is a valid string and inserting it char by char into Data Table.
*If it has errors, using the file name for messages with focus the current file.
*@param str - the string to insert
*@param fileName - the current file's name
*/
void addNewStringToDataTable(char * str, char * fileName);

/**
*The function gets a char and coding it by its ascii value as data word and inserts it to Data Table
*@param ch - the char to add into Data Table
*@param fileName - the current file's name
*/
void addCharIntoDataTable(char ch, char * fileName);

/**
*The function gets an array of numbers and file's name. It checks if the array is valid (numbers and splitted by commas)
*and inserting it number by number into Data Table
*@param str - the array of numbers to insert
*@param fileName - the current file's name
*/
void addNewNumbersToDataTable(char * str, char * fileName);

/**
*The function gets a number and codint it by its numerical value as data word and inserts it to Data Table
*@param num - the number to add into Data Table
*@param fileName - the current file's name
*/
void addNumberIntoDataTable(int num, char * fileName);

/**
*The function updates the Data Table lines' addresses to match with the Code Table lines' addresses by the IC value
*/
void updateDataTableAddresses();

/**
*The function performs delete of data words in the Data Table
*/
void deleteDataTable();



/* Labels Table Functions and Documentation */

/**
*The function gets a label's name and its type and inserts it into the Labels table
*@param label - the label's name
*@param type - the label's type
*/
void addNewLabelToTable(char * label, labelType type);

/**
*The function gets label and file's name. It updates the entry type adding an entry type to it
*@param entLabel - the label to update its type to entry type
*@param fileName - the current file's name
*/
void updateEntryAttributeToLabel(const char * entLabel, char * fileName);

/**
*The function checks if the Labels Table contains at least one entry type label
*@return TRUE/FALSE
*/
int checkForEntry();

/**
*The function performs delete of all labels in the Labels Table
*/
void deleteLabelTable();



/* Externals Table Functions and Documentation */

/**
*The function adds an external label with its address (by the current address of instuction counter) into the External Labels Table
*@param extLabel - the external label's name
*/
void addExtLabelIntoExtTable(char * extLabel);

/**
*The function performs delete of all externals in the External Labels Table
*/
void deleteExternTable();



/* Errors Table Functions and Documentation */

/**
*The function adds new error line into Errors Table with its file name and line where the error occurred and its description
*@param errorMsg - the description of the error message
*@param fileName - the current file's name
*/
void addErrorToTable(char * errorMsg, char * fileName);

/**
*The function checks if the current file name has errors. If does, prints all the errors and return TRUE. Otherwise doesn't print at all and return FALSE
*@param fileName - the current file's name
*@return TRUE/FALSE
*/
int withErrors(char * fileName);

/**
*The function prints all errors from the Errors Table
*/
void printAllErrors();

/**
*The function performs delete of errors in the Erorrs Table
*/
void deleteErrorTable();



/* Common Functions and Documentation */

/**
*The function performs delete of all program's Tables
*/
void deleteAllTables();

#endif

