#include "utils.h"
#include "tables.h"
#include "validators.h"

/***** Code Table Functions *****/

void addOpcodeWordIntoCodeTable(int opcode, int funct, int destAddressMode, int srcAddressMode)
{
	codeWordNode *temp, *currentCodeWordPtr;
	temp = (codeWordNode*)malloc(sizeof(codeWordNode));/*creates new code word*/
	
	if(temp == NULL) { /*if the memory allocation for the new code word fails*/
		memoryAllocationFailure();
	}

	/*if the allocation of memory for new code word is successful, adding the values into it's fields*/
	temp->address = IC; /*address is instruction counter current value*/
	temp->areValue = 'A'; /*adding ARE value of 'A'*/
	temp->value.opcode.destAddressMode = destAddressMode; /*sets the opcode destination address mode*/
	temp->value.opcode.srcAddressMode = srcAddressMode; /*sets the opcode source operand address mode*/
	temp->value.opcode.opcode = opcode; /*sets the opcode number*/
	temp->value.opcode.funct = funct; /*sets the opcode function number*/
	temp->next = NULL;

	if(codeWordPtrHead == NULL) { /*if the Code Table is empty, adds it as first line*/
		codeWordPtrHead = temp;
	}
	
	else { /*if the Code Table is not empty adds it as last line in the Code Table*/
		currentCodeWordPtr = codeWordPtrHead;
		while(currentCodeWordPtr->next != NULL) {
			currentCodeWordPtr = currentCodeWordPtr->next; 
		}
		currentCodeWordPtr->next = temp;
	}
	IC++; /*increasing the instruction counter at the end of insertion into the Code Table*/
}


void addRegisterWordIntoCodeTable(char* reg)
{
	int i = 0;
	codeWordNode *temp, *currentCodeWordPtr;
	temp = (codeWordNode*)malloc(sizeof(codeWordNode)); /*creates new code word*/

	if(temp == NULL) { /*if the memory allocation for the new code word fails*/
		memoryAllocationFailure();
	}

	/*sets i as the register number from (range is 0 to 7)*/
	while(i < NUM_OF_REGISTERS &&
	      strcmp(registers[i],reg) != 0) {
		i++;
	}

	temp->address = IC; /*address is instruction counter current value*/
	temp->value.reg.value = pow(2,i); /*the register value is 2 power i which makes it one bit of 1 and the rest of the bits are 0 
					- depends on the register number*/
	temp->areValue = 'A'; /*adding ARE value of 'A'*/
	temp->next = NULL;

	if(codeWordPtrHead == NULL) { /*if the Code Table is empty, adds it as first line*/
		codeWordPtrHead = temp;
	}
	
	else { /*if the Code Table is not empty adds it as last line in the Code Table*/
		currentCodeWordPtr = codeWordPtrHead;
		while(currentCodeWordPtr->next != NULL) {
			currentCodeWordPtr = currentCodeWordPtr->next; 
		}
		currentCodeWordPtr->next = temp;
	}
	IC++; /*increasing the instruction counter at the end of insertion into the Code Table*/
}


void addNumberWordIntoCodeTable(char* num)
{
	int number;
	codeWordNode *temp, *currentCodeWordPtr;

	number = atoi(num+1); /*converts the num string into integer number (points to the second char, first char is '#')*/
	temp = (codeWordNode*)malloc(sizeof(codeWordNode)); /*creates new code word*/
	
	if(temp == NULL) {/*if the memory allocation for the new code word fails*/
		memoryAllocationFailure();
	}

	temp->address = IC; /*address is instruction counter current value*/
	temp->value.number.value = number; /*insert the number as value*/
	temp->areValue = 'A'; /*adding ARE value of 'A'*/
	temp->next = NULL;

	if(codeWordPtrHead == NULL) { /*if the Code Table is empty, adds it as first line*/
		codeWordPtrHead = temp;
	}
	
	else { /*if the Code Table is not empty adds it as last line in the Code Table*/
		currentCodeWordPtr = codeWordPtrHead;
		while(currentCodeWordPtr->next != NULL) {
			currentCodeWordPtr = currentCodeWordPtr->next; 
		}
		currentCodeWordPtr->next = temp;
	}
	IC++; /*increasing the instruction counter at the end of insertion into the Code Table*/
}


void addDirectAddressWordIntoCodeTable(char* label)
{
	codeWordNode *temp, *currentCodeWordPtr;
	labelNode* currentLabelPtr = labelPtrHead;
	temp = (codeWordNode *)malloc(sizeof(codeWordNode));/*creates new code word*/

	if(temp == NULL) { /*if the memory allocation for the new code word fails*/
		memoryAllocationFailure();
	}

	while(currentLabelPtr != NULL &&
	      strcmp(currentLabelPtr->name,label) != 0) { /*checks if the label exists in the Labels Table*/
		currentLabelPtr = currentLabelPtr->next;
	}	

	temp->address = IC; /*address is instruction counter current value*/
	temp->value.labelValue.value = currentLabelPtr->address; /*the value of the code word is the address of the label*/
	temp->next = NULL;

	if(currentLabelPtr->type == EXTERN) { /*if the label type is external*/
		temp->areValue = 'E'; /*adding ARE value of 'E'*/
		addExtLabelIntoExtTable(label);/*adds the label into the External Labels Table*/
	}
	else { /*if the label type is not external*/
		temp->areValue = 'R';/*adding ARE value of 'R'*/
	}

	if(codeWordPtrHead == NULL) { /*if the Code Table is empty, adds it as first line*/
		codeWordPtrHead = temp;
	}
	
	else { /*if the Code Table is not empty adds it as last line in the Code Table*/
		currentCodeWordPtr = codeWordPtrHead;
		while(currentCodeWordPtr->next != NULL) {
			currentCodeWordPtr = currentCodeWordPtr->next; 
		}
		currentCodeWordPtr->next = temp;
	}
	IC++; /*increasing the instruction counter at the end of insertion into the Code Table*/
}


void addRelativeAddressWordIntoCodeTable(char* label)/*adding an address word of relative label (label's address - IC)*/
{
	codeWordNode *temp, *currentCodeWordPtr;
	labelNode* currentLabelPtr = labelPtrHead;
	temp = (codeWordNode*)malloc(sizeof(codeWordNode));/*creates new code word*/

	if(temp == NULL) { /*if the memory allocation for the new code word fails*/
		memoryAllocationFailure();
	}

	while(currentLabelPtr != NULL &&
	      strcmp(currentLabelPtr->name,(label+1)) != 0) { /*checks if the label exists in the Labels Table (points to the second char beacuse the first one is '%')*/
		currentLabelPtr = currentLabelPtr->next;
	}	

	temp->address = IC;/*address is instruction counter current value*/
	temp->value.labelValue.value = currentLabelPtr->address - IC; /*gets the relative address of the label by the label address minus the instruction counter*/
	temp->next = NULL;

	if(currentLabelPtr->type == EXTERN) { /*if the label type is external*/
		temp->areValue = 'E'; /*adding ARE value of 'E'*/
		addExtLabelIntoExtTable(label+1);/*adds the label into the External Labels Table (without first char which is '%')*/
	}
	else {
		temp->areValue = 'A';/*adding ARE value of 'A'*/
	}

	if(codeWordPtrHead == NULL) { /*if the Code Table is empty, adds it as first line*/
		codeWordPtrHead = temp;
	}
	
	else { /*if the Code Table is not empty adds it as last line in the Code Table*/
		currentCodeWordPtr = codeWordPtrHead;
		while(currentCodeWordPtr->next != NULL) {
			currentCodeWordPtr = currentCodeWordPtr->next; 
		}
		currentCodeWordPtr->next = temp;
	}
	IC++;/*increasing the instruction counter at the end of insertion into the Code Table*/
}


void deleteCodeTable()
{
	codeWordNode* currentCodeWordPtr = codeWordPtrHead;
	codeWordNode* nextCodeWordPtr;
	
	while(currentCodeWordPtr != NULL) { /*frees the allocated memory line by line from Code Table*/
		nextCodeWordPtr = currentCodeWordPtr->next; /*saves the pointer to next line*/
		free(currentCodeWordPtr); /*frees the memory of the current line*/
		currentCodeWordPtr = nextCodeWordPtr; /*points to the next line saved before freeing the memory*/
	}
	codeWordPtrHead = NULL; /*the head of the table is initialized*/
}


/***** Data Table Functions *****/

void addNewStringToDataTable(char* str,char* fileName)
{
	int i=0, j=0, k=0;

	while(str[i] != '\0' && isspace(str[i])) { /*skipping on the whitespaces until the end of the line*/
		i++;
	}

	if(i >= strlen(str)) { /*if there were only whitespaces, means no string at all*/
		addErrorToTable("Error - no string found.",fileName);
	}
	else if(str[i] != '\"') { /*if the first letter which is not whitespace is also not a start of a string*/
		addErrorToTable("Error - no string found.",fileName);
	}
	else { /*if a start of a string*/
		i++;/* for the the index of the first char in the string */
		j = i;/*copies the index of the first char*/
		
		while(str[i] != '\0' && str[i] != '\"') { /*gets into the index of the end of string*/
			i++;
		}

		if(i >= strlen(str)) { /* if no end of string found*/
			addErrorToTable("Error - no string found.",fileName);
		}
		else {
			k = i + 1; /*gets the end of string*/
			while(str[k] != '\0' && isspace(str[k])) {
				k++;	
			}
			
			if(k < strlen(str)) { /*checks if there are any other letters after the end of string*/
				addErrorToTable("Error - other arguments after string.",fileName);
			}
			
			else { /* in case there is a string start and ending and not any other letters (except of whitespaces) in the line*/
				while(j < i) { /* adds each char of the string, one by one as data words, into the Data Table*/
					addCharIntoDataTable(str[j],fileName);
					DC++; /*increasing the data counter*/
					j++;/*increasing the index to the next char in the string*/
				}
				addCharIntoDataTable('\0',fileName);/*at the end of string, adds into Data Table the null-terminator char as data word*/
				DC++;/*increasing the data counter*/
			}
		}
	}
}


void addCharIntoDataTable(char ch, char* fileName)
{
	dataWordNode *temp, *currentDataWordPtr;
	temp = (dataWordNode*)malloc(sizeof(dataWordNode));/*creates new data word*/

	if(temp == NULL) { /*if allocation of memory for new data word fails*/
		memoryAllocationFailure();
	}

	/*if the allocation of memory for new data word is successful, adding the values into it's fields*/
	temp->dataAddress = DC;/*the address is the data counter*/
	temp->areValue = 'A'; /*adding ARE value of 'A'*/
	temp->value.ascii.value = ch;
	temp->next = NULL; 

	if(dataWordPtrHead == NULL) { /*if it is the first data word in the Data Table, put it has first line*/
		dataWordPtrHead = temp;
	}
	
	else { /*if it is not the first word, puts it as last data word in the Data Table*/
		currentDataWordPtr = dataWordPtrHead;
		while(currentDataWordPtr->next != NULL) { 
			currentDataWordPtr = currentDataWordPtr->next; 
		}
		currentDataWordPtr->next = temp;
	}
}

void addNewNumbersToDataTable(char* str,char* fileName)
{
	char* arguments = (char*)malloc(LINE_MAX_LENGTH);/*allocating memory for new string which will contain the numbers seperated by commas*/
	int i = 0, j=0, number;
   	char* token;

	if(arguments == NULL) { /*if the allocation of arguments fails*/
		memoryAllocationFailure();
	}

	/*cleans the line from whitespaces*/
	while(i < strlen(str)) {
		if(!(isspace(str[i]))) { 
			arguments[j] = str[i];/*copies the letter if it is not a whitespace*/
			j++;	
		}
		i++;
	}	
	
	arguments[j] = '\0';/*adds null-terminator for the end of arguments string*/

	/*seperates each number by comma token*/
	token = strtok(arguments, ","); 
	while (token != NULL) { 
		if(isLegalNumber(token)) { /*check if the token is a valid number*/
			number = atoi(token);/*if is valid number, converts it to integer*/
			addNumberIntoDataTable(number, fileName); /*adds the number as data word into Data Table*/
			DC++;/*increasing the data counter*/
		}
		else { /*if not a valid number*/
			addErrorToTable("Error - at least one number is invalid (number must start sign or digit, size of signed word with digits only).", fileName);
		}
       	 	token = strtok(NULL, ","); /*repeat the token action*/
    	}
	free(arguments);/*frees the allocated memory of arguments*/
}


void addNumberIntoDataTable(int num, char* fileName)
{
	dataWordNode *temp, *currentDataWordPtr;
	temp = (dataWordNode*)malloc(sizeof(dataWordNode));/*creates new data word*/

	if(temp == NULL) { /*if allocation of memory for new data word fails*/
		memoryAllocationFailure();
	}

	/*if the allocation of memory for new data word is successful, adding the values into it's fields*/
	temp->dataAddress = DC; /*the address is the data counter*/
	temp->areValue = 'A'; /*adding ARE value of 'A'*/
	temp->value.number.value = num;
	temp->next = NULL; 

	if(dataWordPtrHead == NULL) { /*if it is the first data word in the Data Table, put it has first line*/
		dataWordPtrHead = temp;
	}
	
	else { /*if it is not the first word, puts it as last data word in the Data Table*/
		currentDataWordPtr = dataWordPtrHead;
		while(currentDataWordPtr->next != NULL) { 
			currentDataWordPtr = currentDataWordPtr->next; 
		}
		currentDataWordPtr->next = temp;
	}
}

void updateDataTableAddresses()
{
	dataWordNode* currentDataWordPtr = dataWordPtrHead;/*sets the pointer on the head of the Data Table */
	labelNode* currentLabelPtr = labelPtrHead;/*sets the pointer on the head of the Labels Table*/

	while(currentDataWordPtr != NULL) { /*for each data word in the Data Table, update its address by adding the ICF - insturcion counter final*/
		currentDataWordPtr->dataAddress += ICF; 
		currentDataWordPtr = currentDataWordPtr->next; 
	}

	while(currentLabelPtr != NULL) { /*for each label in the Labels Table, if it's type is data - update it's address by adding the ICF*/
		if(currentLabelPtr->type == DATA ||
		   currentLabelPtr->type == DATA_ENTRY){
			currentLabelPtr->address += ICF;
		}
		currentLabelPtr = currentLabelPtr->next;
	}
}


void deleteDataTable()
{
	dataWordNode* currentDataWordPtr = dataWordPtrHead;
	dataWordNode* nextDataWordPtr;

	while(currentDataWordPtr != NULL) { /*frees the allocated memory line by line from Data Table*/
		nextDataWordPtr = currentDataWordPtr->next;/*saves the pointer to next line*/
		free(currentDataWordPtr);/*frees the memory of the current line*/
		currentDataWordPtr = nextDataWordPtr;/*points to the next line saved before freeing the memory*/
	}
	dataWordPtrHead = NULL;/*the head of the table is initialized*/
}



/***** Labels Table Functions *****/

void addNewLabelToTable(char* label, labelType type)
{
	labelNode *temp, *currentLabelPtr;
	temp = (labelNode*)malloc(sizeof(labelNode));/*creates a new label line for the labels table*/
	
	if(temp == NULL) { /*if the creation of a new line is not successful*/
		memoryAllocationFailure();
	}

	/*if the memory allocation is successful, adding the values into the label line fields*/
	temp->name= label;
	temp->type = type;

	/*depends on the label type, enters its address*/
	if(type == CODE) {
		temp->address = IC;
	}
	else if(type == DATA) {
		temp->address = DC;
	}
	else if(type == EXTERN) { 
		temp->address = EXTERNAL_ADDRESS; /*constant address of external labels*/
	}

	temp->next = NULL; 

	/*checks if the tables is empty. if it is, adding the first line*/
	if(labelPtrHead == NULL) {
		labelPtrHead = temp;
	}
	
	else { /*if the table is not empty, adding the line to the bottom of the table*/
		currentLabelPtr = labelPtrHead;
		while(currentLabelPtr->next != NULL) {
			currentLabelPtr = currentLabelPtr->next; 
		}
		currentLabelPtr->next = temp;
	}
}

void updateEntryAttributeToLabel(const char* entLabel, char* fileName)
{
	labelNode* currentLabelPtr = labelPtrHead;

	if(!(isLabelAlreadyInTable(entLabel))) { /*label which is defined entry must be in the labels table*/
		addErrorToTable("Error - .entry label not found in the label's table.", fileName);
	}
	else { /*if the label is found in the labels table*/
		while(currentLabelPtr != NULL) { /*checks the type of the entry label in the table*/
			
			if(strcmp(currentLabelPtr->name,entLabel) == 0) {
				if(currentLabelPtr->type == EXTERN) { /*label with extern type could not be a entry label too*/
					addErrorToTable("Error - the label is already in table with extern attribute.", fileName);
				}
				else if(currentLabelPtr->type == CODE) { /*label with code type changes to code and entry types*/
					currentLabelPtr->type = CODE_ENTRY; /*represented as new type CODE_ENTRY which contains both*/
				}
				else if(currentLabelPtr->type == DATA) { /*label with data type changes to data and entry types*/
					currentLabelPtr->type = DATA_ENTRY; /*represented as new type DATA_ENTRY which contains both*/
				}
				else { /*already in table with an entry type*/
					addErrorToTable("Error - .entry label has been defined already in the Labels Table.", fileName);
				}
			}
			currentLabelPtr = currentLabelPtr->next; 
		}
	}
}


int checkForEntry()
{
	labelNode* currentLabelPtr;

	if(labelPtrHead == NULL) { /*if labels table has no labels at all*/
		return FALSE;
	}
	
	else { /*if there are labels inside the labels table*/
		currentLabelPtr = labelPtrHead;
		while(currentLabelPtr != NULL) { /*checks if there is at least one entry type label inside labels table*/
			if(currentLabelPtr->type == DATA_ENTRY ||
			   currentLabelPtr->type == CODE_ENTRY) {
				return TRUE;
			}
			currentLabelPtr = currentLabelPtr->next;
		}
	}
	return FALSE; /*if there are no entry labels at all in the labels table*/
}

void deleteLabelTable()
{
	labelNode* currentLabelPtr = labelPtrHead;
	labelNode* nextLabelPtr;

	while(currentLabelPtr != NULL) { /*frees the allocated memory line by line from labels table*/
		nextLabelPtr = currentLabelPtr->next; /*saves the pointer to next line*/
		free(currentLabelPtr); /*frees the memory of the current line*/
		currentLabelPtr = nextLabelPtr; /*points to the next line saved before freeing the memory*/
	}
	labelPtrHead = NULL; /*the head of the table is initialized*/
}




/***** Externals Table Functions *****/

void addExtLabelIntoExtTable(char* extLabel)
{
	externalNode *temp, *currentExternalPtr;
	char* extLabelTemp = (char*)malloc(LABEL_MAX_LENGTH);
	temp = (externalNode*)malloc(sizeof(externalNode));/*creates a new external label line for the externals table*/

	if(temp == NULL ||
	   extLabelTemp == NULL) { /*if the creation of new line or new string for label - not successful */
		memoryAllocationFailure();
	}
	
	/*if the memory allocation is successful, adding the values into the external label line fields*/
	strcpy(extLabelTemp, extLabel); /*copies the name of the label into new string*/
	temp->next = NULL; 	
	temp->name = extLabelTemp;
	temp->address = IC; /*the external address where it used in the code image*/

	if(extrenalPtrHead == NULL) { /*checks if the tables is empty. if it is, adding the first line*/
		extrenalPtrHead = temp;
	}
	
	else { /*if the table is not empty, adding the line to the bottom of the table*/
		currentExternalPtr = extrenalPtrHead;
		while(currentExternalPtr->next != NULL) {
			currentExternalPtr = currentExternalPtr->next; 
		}
		currentExternalPtr->next = temp;
	}
}

void deleteExternTable()
{
	externalNode* currentExternPtr = extrenalPtrHead;
	externalNode* nextExternPtr;

	while(currentExternPtr != NULL) { /*frees the allocated memory line by line from external labels table*/
		nextExternPtr = currentExternPtr->next; /*saves the pointer to next line*/
		free(currentExternPtr->name); /*frees the memory of the string name of the external label*/
		free(currentExternPtr); /*frees the memory of the current line*/
		currentExternPtr = nextExternPtr; /*points to the next line saved before freeing the memory*/
	}
	extrenalPtrHead = NULL; /*the head of the table is initialized*/
}

/***** Errors Table Functions *****/
void addErrorToTable(char* errorMsg, char* fileName)
{
	errorNode *temp, *currentErrorPtr;
	temp = (errorNode*)malloc(sizeof(labelNode)); /*creates a new error line for the errors table*/

	if(temp == NULL) { /*if the creation of a new line is not successful*/
		memoryAllocationFailure();
	}

	/*if the memory allocation is successful, adding the values into the error line fields*/
	temp->next = NULL; 
	temp->description = errorMsg;
	temp->line = lineCounter;
	temp->file = fileName;

	/*checks if the tables is empty. if it is, adding the first line*/
	if(errorPtrHead == NULL) {
		errorPtrHead = temp;
	}
	
	else { /*if the table is not empty, adding the line to the bottom of the table*/
		currentErrorPtr = errorPtrHead;
		while(currentErrorPtr->next != NULL) {
			currentErrorPtr = currentErrorPtr->next; 
		}
		currentErrorPtr->next = temp;
	}
}

void printAllErrors()
{
	errorNode* currentErrorPtr = errorPtrHead; /*gets the head of the table*/
	
	while(currentErrorPtr != NULL) { /*prints all the errors in the errors table including file and line of error and description*/
		printf("\n- File:\"%s\" line:%d: %s", 
		       currentErrorPtr->file,
		       currentErrorPtr->line, 
		       currentErrorPtr->description);
		currentErrorPtr = currentErrorPtr->next;
	}
}


int withErrors(char* fileName)
{
	if(errorPtrHead == NULL) { /*if the errors table is empty*/
		return FALSE;/*returns that there are no errors*/
	}	
	/*otherwise, prints all errors from the errors table*/
	printf("\nFile \"%s\" has encountered the following error/s:",fileName);
	printAllErrors();
	return TRUE;/*returns that there are errors*/
}


void deleteErrorTable()
{
	errorNode* currentErrorPtr = errorPtrHead;
	errorNode* nextErrorPtr;

	while(currentErrorPtr != NULL) { /*frees the allocated memory line by line from errors table*/
		nextErrorPtr = currentErrorPtr->next; /*saves the pointer to next line*/
		free(currentErrorPtr); /*frees the memory of the current line*/
		currentErrorPtr = nextErrorPtr; /*points to the next line saved before freeing the memory*/
	}
	errorPtrHead = NULL; /*the head of the table is initialized*/
}


/***** Common Table Functions *****/

void deleteAllTables() /*frees the allocated memory of all linked-list/tables in the program*/
{
	deleteLabelTable();
	deleteCodeTable();
	deleteDataTable();
	deleteExternTable();
	deleteErrorTable();
}
/***** END OF FILE *****/
