#include "utils.h"
#include "validators.h"


int isEmptyOrCommentLine(const char* line)
{
	int i=0;

	while(line[i] != '\n' && line[i] != '\0') { /* loop till it's next line or end of string */
		if(line[i] == ';') {
			return TRUE;
		}
		
		if(line[i] != '\t' &&
		   line[i] != ' ') { /* if there are chars which are not tabs or spaces - means its not an empty line */
			return FALSE;
		}
		
		i++;
	}
 	return TRUE; /* returns true if there are no ';' at all and rest of the line full of whitespaces */
}


int isOpcode(const char* str)
{
	int i;
	for(i=0; i<NUM_OF_OPCODES; i++) { 
		if(strcmp(str,opcodes[i]) == 0) { /*checks if the string is a legal opcode*/
			return TRUE;		
		}
	}
	return FALSE;
}

int isRegister(const char* str)
{
	int i;
	for(i=0; i<NUM_OF_REGISTERS; i++) {
		if(strcmp(str,registers[i]) == 0) { /*checks if the string is a legal register*/
			return TRUE;		
		}
	}
	return FALSE;
}

int isDirective(const char* str)
{
	int i;
	for(i=0; i<NUM_OF_DIRECTIVES; i++) {
		if(strcmp(str,directives[i]) == 0) { /*checks if the string is a legal directive*/
			return TRUE;		
		}
	}
	return FALSE;
}


int isSavedWord(const char* str) 
{
	/*checks if the string is a saved word - opcode, register or directive*/
	return (isOpcode(str) ||
		isRegister(str) ||
		isDirective(str));
}


int isLegalNumber(const char* strNum)
{
	int i = 0, num = 0;
	
	if(strNum[0] != '-' &&
	   strNum[0] != '+') { /* case of number without a +/- sign */
		for(i=0; i < strlen(strNum); i++) {
			if(!(isdigit(strNum[i]))) { /*if a char is not a digit - it is not a valid number */
				return FALSE;
			}
		}

		if(strlen(strNum) < 1) { 
			return FALSE;
		}
	}

	else { /* case of number with a +/- sign */
		for(i=1; i < strlen(strNum); i++) { 
			if(!(isdigit(strNum[i]))) { /*if a char is not a digit - it is not a valid number */
				return FALSE;
			}
		}

		if(strlen(strNum) <= 1) { /*in case it is only a sign without other digits*/
			return FALSE;
		}
	}
	
	num = atoi(strNum); /*converts the number from string into integer*/

	if(num > MAX_NUMBER_SIZE ||
	   num < MIN_NUMBER_SIZE) { /*checks if the number is in the range of 12 bits signed numebr */
		return FALSE;	
	}
	return TRUE;
}

int isLegalLabel(const char* str)/*checks if the field is a legal label*/
{
	int i;
	boolean isLegal = TRUE;
	
	/*checks if the label has its all requirements - starts with alphabetic, isn't longer then the max length, isn't a saved word and has ':' at the end*/
	if(!(isalpha(str[0])) ||
	   (strlen(str) > LABEL_MAX_LENGTH) ||
	   isSavedWord(str) ||
	   str[strlen(str)-1] != ':' ) {
		isLegal = FALSE;
	}

	for(i = 1; i < strlen(str)-1; i++) { /*checks if the rest of the chars in the label are alphanumerics*/
		if(!(isalpha(str[i]) ||
		     isdigit(str[i]))) {
			isLegal = FALSE;
		}
	}
	return isLegal;
}



int isLabelAlreadyInTable(const char* str)
{
	labelNode* currentLabelPtr = labelPtrHead;

	while(currentLabelPtr != NULL) { 
		if(strcmp(currentLabelPtr->name, str) == 0) { /*checks if the label is already in the labels table*/
			return TRUE;
		}
		currentLabelPtr = currentLabelPtr->next;
	}	
	return FALSE;
}


int isOperandNumber(char* strNum)/*if starts with # and the rest is legal number*/
{
	char* temp;

	if(strNum[0] == '#') {	
		temp = strNum + 1;
		if(isLegalNumber(temp)) {
			return TRUE;
		}
	}
	return FALSE;
}


int isRelativeAddress(char* label)/*if starts with % and the rest is an exist label*/
{
	char* temp;

	if(label[0] == '%') { 
		temp = label + 1;
		if(isLabelAlreadyInTable(temp)) { 
			return TRUE;
		}
	}
	return FALSE;
}
