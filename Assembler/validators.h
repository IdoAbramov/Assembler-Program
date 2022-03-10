/* Validators Functions and Doucmentation*/
#ifndef VALIDATORS_H
#define VALIDATORS_H
/**
*The function checks if a line is either empty or comment line
*@param line - contains the full command line to check
*@return TRUE/FALSE
*/
int isEmptyOrCommentLine(const char * line);

/**
*The function checks if the input string is considered as saved word.
*@param str - string to check 
*@return TRUE/FALSE
*/
int isSavedWord(const char * str);

/**
*The function checks if the input string is considered as number.
*@param strNum - string to check
*@return TRUE/FALSE
*/
int isLegalNumber(const char * strNum);

/**
*The function checks if the input string is considered as legal label (starts with alphabetic letter and ends with ':')
*@param str - string to check
*@return TRUE/FALSE
*/
int isLegalLabel(const char * str);

/**
*The function checks if the input string is considered as legal operation code.
*@param str - string to check
*@return TRUE/FALSE
*/
int isOpcode(const char * str);

/**
*The function checks if the input string is considered as legal register
*@param str - string to check
*@return TRUE/FALSE
*/
int isRegister(const char * str);

/**
*The function checks if the input string is considered as legal directive
*@param str - string to check
*@return TRUE/FALSE
*/
int isDirective(const char * str);

/**
*The function checks if the input string is a label which is already inside the Labels Table
*@param str - string to check
*@return TRUE/FALSE
*/
int isLabelAlreadyInTable(const char * str);

/**
*The function checks if the input string is a legal operand number (starts with '#') - which is a number used with operation codes
*@param str - string to check
*@return TRUE/FALSE
*/
int isOperandNumber(char * strNum);

/**
*The function checks if the input string is a legal relative address (starts with '%')
*@param label - string to check
*@return TRUE/FALSE
*/
int isRelativeAddress(char * label);

#endif
