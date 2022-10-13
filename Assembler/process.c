#include "utils.h"
#include "validators.h"
#include "tables.h"
#include "output.h"
#include "process.h"


void startAssemblerProcess(int numberOfFiles, char* arrayOfFiles[])
{
	int i;
	char* asFile;
	FILE* f1;
	
	if(numberOfFiles == NO_FILES) /*checks if there is no files in the input*/
	{
		printf("\nYou must enter at least one file. Please try again.\n");
		exit(0);
	}
	
	printf("\nStarts Assembler Program...");
	for(i = 1; i < numberOfFiles; i++) /*for each file from the arguments of the command line*/
	{
		asFile = getFileName(arrayOfFiles[i], AS); /*get the complete ".as" file*/
		printf("\n\nOpening file \"%s\"...",asFile);
		f1 = fopen(asFile,"r"); /*open file for reading*/
		if(f1 == NULL) /*checks if cannot open the file*/
		{
			printf("\nError - file:\"%s\" not found or could not be open.",asFile);
			continue; /*loops for the next file if exist*/
		}
			
		printf("\nScanning and assembling file: \"%s\"...",asFile);
		/*sets initial values for instruction couner, data counter and line counter - for the first round*/
		IC = IC_START;
		DC = DC_START;
		lineCounter = FIRST_LINE;
		
		/*performs the first round on the ".as" file*/
		firstRound(f1,asFile);
		
		/*saves the final values of both instruction and data counters*/
		ICF = IC;
		DCF = DC;

		if(ICF+DCF > MEMORY_SIZE) /*check if memory overflow*/
		{
			addErrorToTable("Error - memory overflow.", asFile);
		}  
		
		if(!(withErrors(asFile)))/*checks if there are no errors after the first round*/
		{	
			/*sets initial values for instruction couner, data counter and line counter - for the second round*/
			IC = IC_START;
			DC = DC_START;
			lineCounter = FIRST_LINE;
	
			rewind(f1);/*gets the file pointer to the start of file*/
			updateDataTableAddresses();/*adding data words the ICF to update their address*/

			/*performs the second round on the ".as" file*/
			secondRound(f1,asFile);

			if(!(withErrors(asFile)))/*checks if there are no errors after the second round*/
			{
				/*checks if there are valid outputs and creates the ".ob", ".ent" and ".ext" files */
				createObjectOutputFile(arrayOfFiles[i]);
				createEntryOutputFile(arrayOfFiles[i]);
				createExternalOutputFile(arrayOfFiles[i]);
				printf("\nFile \"%s\" has assembled successfully. See output file/s in the folder.",asFile);
			}

			else /*if there are errors, the program won't create files but print all the errors it encounterd*/
			{
				printf("\nFile \"%s\" has some errors. Aborted.",asFile);
			}
		}
		else /*if there are errors, the program won't create files but print all the errors it encounterd*/
		{
			printf("\nFile \"%s\" has some errors. Aborted.",asFile);
		}

		fclose(f1);/*close the current file*/
		deleteAllTables();/*performs free of all allocated memory of the used tables*/
	}

	free(asFile);
	printf("\n\nProgram run has ended successfully.\n");
}


void firstRound(FILE* f1, char* fileName)
{
	boolean labelFlag;
	char *line, *word, *label, *lineBuffer;
	lineBuffer = (char*)malloc(LINE_MAX_LENGTH);/*allocating memory for line string buffer with size of maximum line length*/

	if(lineBuffer == NULL)/*if the allocation of memory failed*/
	{
		memoryAllocationFailure();/*termiantes the program*/
	}

	while(fgets(lineBuffer, LINE_MAX_LENGTH, f1) != NULL)/*reads from the input file line by line until the end of file*/
	{
		labelFlag = FALSE; /*used to check if there is a label in the line*/
		line = lineBuffer;/*sets the line to point on the line buffer string*/

		/*allocatin memory for strings with maximum line length: word and label*/
		word = (char *) malloc(LINE_MAX_LENGTH);
		label = (char *) malloc(LINE_MAX_LENGTH);

		if(word == NULL || label == NULL)/*if the allocation of memory failed*/ 
		{
			memoryAllocationFailure();/*termiantes the program*/
		}

		if(!(isEmptyOrCommentLine(line)))/*if the line is not empty or comment line*/
		{
			getNextWordInLine(line,word);/*gets the first word in the line*/
			if(strlen(word) > 0)/*checks if the word string contains chars*/
			{
				if(isLegalLabel(word))/*checks if the word is a legal label*/
				{
					labelFlag = TRUE;/*label found*/
					strcpy(label,word);/*copies the string from word into label*/
					label[strlen(label)-1] ='\0';/*adds null-terminator at the end of label*/
					getNextWordInLine(line,word);/*gets the second word in the line*/
				}
		
				/*word contains the first word in the line if no label found or the second word in line if a label found*/

				if(strcmp(word,".data") == 0)/*if word is ".data" directive*/
				{
					if(labelFlag && isLabelAlreadyInTable(label))/*if there is a label which is already in the Labels Table*/
					{
						addErrorToTable("Error - label is already defined in the Labels Table",fileName);
					}
					else if(labelFlag)/*if label is a new label*/
					{								
						addNewLabelToTable(label,DATA);/*adds new label to Labels Table with type "data"*/
					}
					addNewNumbersToDataTable(line,fileName);/*the rest of the line will contain an array of numbers to add into Data Table*/
				}
		
				else if(strcmp(word,".string") == 0)/*if word is ".string" directive*/
				{
					if(labelFlag && isLabelAlreadyInTable(label))/*if there is a label which is already in the Labels Table*/
					{
						addErrorToTable("Error - label is already defined in the Labels Table",fileName);
					}
					else if(labelFlag)/*if label is a new label*/
					{
						addNewLabelToTable(label,DATA);/*adds new label to Labels Table with type "data"*/
					}
					addNewStringToDataTable(line, fileName);/*the rest of the line will contain a string of chars to add into Data Table*/
				}
		
				else if(strcmp(word,".extern") == 0)/*if word is ".extern" directive*/
				{
					if(labelFlag && !isLabelAlreadyInTable(label))/*if valid label, prints warning for labal before ".extern" directive*/
					{
						printf("\nFile:%s line:%d: Warning - Label at .extern directive\n", fileName, lineCounter);
					}
					getNextWordInLine(line,label);/*gets the next word in line which is the extern label (aka symbol)*/
					addNewLabelToTable(label,EXTERN);/*adds the extern label into Labels Table*/
				}
		
				else if(strcmp(word,".entry") == 0)/*if word is ".entry" directive*/
				{
					if(labelFlag && !isLabelAlreadyInTable(label))/*if valid label, prints warning for labal before ".entry" directive*/
					{
						printf("\nFile:%s line:%d: Warning - Label at .entry directive\n", fileName, lineCounter);
					}
				}
		
				else if(isOpcode(word))/*if the word is a valid opcode*/
				{
					if(labelFlag && isLabelAlreadyInTable(label))/*if there is a label which is already in the Labels Table*/
					{
						addErrorToTable("Error - label is already defined in the Labels Table",fileName);
					} 
					else if(labelFlag)/*if label is a new label*/
					{
						addNewLabelToTable(label,CODE);/*adds new label to Labels Table with type "code"*/
					}
					IC += opcodeNumOfOperands(word) + OPCODE_COUNT; /*counts the instruction counter by the predicted number of operands and the 												opcode which will be coded as code words into Code Table in the second round*/
				}
				else /*if the word is not directive or opcode, means its wrong line, adds an error*/
				{
					addErrorToTable("Error - invalid line, no valid directive nor opcode was found.",fileName);
				}
			}
			else /*word is empty, means the line contains label only, adds an error*/
			{
				addErrorToTable("Error - invalid line, label without a command.",fileName);
			}
		}

		/*realloacting the line buffer for the next line in the file*/
		free(lineBuffer);
		lineBuffer = (char*)malloc(LINE_MAX_LENGTH);

		if(lineBuffer == NULL)/*if memory allocation failed*/
		{
			memoryAllocationFailure();/*terminates the program*/
		}

		free(word);/*frees the word allocated memory for the next line in file*/
		lineCounter++;/*increasing the line counter for the errors line*/
	}

	free(lineBuffer);/*at the end of the file, frees the line buffer allocated memory*/
}


void secondRound(FILE* f1, char* fileName)
{
	char *line, *word, *lineBuffer, *src, *dest;
	lineBuffer = (char*)malloc(LINE_MAX_LENGTH);/*allocating memory for line string buffer with size of maximum line length*/

	if(lineBuffer == NULL)/*if the allocation of memory failed*/
	{
		memoryAllocationFailure();/*termiantes the program*/
	}

	while(fgets(lineBuffer, LINE_MAX_LENGTH, f1) != NULL)/*reads from the input file line by line until the end of file*/
	{
		line = lineBuffer;/*sets the line to point on the line buffer string*/
		/*allocatin memory for strings with maximum line length: word, src and dest*/
		word = (char*) malloc(LINE_MAX_LENGTH);
		src = (char*) malloc(LINE_MAX_LENGTH);
		dest = (char*) malloc(LINE_MAX_LENGTH);

		if(word == NULL || src == NULL || dest == NULL)/*if the allocation of memory failed*/
		{
			memoryAllocationFailure();/*terminates the program*/
		}

		if(!(isEmptyOrCommentLine(line)))/*if the line is not empty or comment line*/
		{
			getNextWordInLine(line,word);/*gets the next word in the line into word string*/
			if(strlen(word) > 0)/*checks if the word string contains chars*/
			{
				if(isLegalLabel(word))/*checks if the label is a valid label*/
				{
					getNextWordInLine(line,word); /*gets the word after*/
				}
				if(isDirective(word))/*checks if the label is a valid directive. word will contain the second word in the line if the first was a label 							or the first word in the line was not a label*/
				{
					if(strcmp(word,".entry") == 0)/*if the directive is ".entry"*/
					{
						getNextWordInLine(line,word);/*gets the next word in line - should be a available label*/
						if(!isLabelAlreadyInTable(word) && strlen(word) > 0)/*if label does not exist in the Labels Table*/
						{
							addErrorToTable("Error - the entry label does not exist in the Labels Table",fileName);
						}
						else if(strlen(word) > 0)/*checks if the word string contains chars*/
						{
							updateEntryAttributeToLabel(word,fileName);/*adds the label an entry attribute(type)*/
						}
						else /*the entry label not found in the Labels Table. adds an error*/
						{
							addErrorToTable("Error - invalid line \".entry\" directive has no label.",fileName);
						}

						getNextWordInLine(line,word); /*gets the next word in line*/
						if(strlen(word) > 0) /*if there is an extra word in the line - adds an error*/
						{
							addErrorToTable("Error - invalid line \".entry\" directive has to many arguments.",fileName);
						}
					}
				}

				else if(isOpcode(word))/*checks if the label is a valid opcode. word will contain the second word in the line if the first was a label
							or the first word in the line was not a label*/
				{
					if(opcodeNumOfOperands(word) == TWO_OPERANDS) /*checks if the opcode configured as using two operands*/
					{	
						getSrcAndDestOperands(line, src, dest, fileName);/*gets the source and destination operands from line*/
						if(strlen(src) == 0) /*if missing source operand from line, adds an error*/
						{
							addErrorToTable("Error - no source operand found on this line.", fileName);
						}
						else if(strlen(dest) == 0)/*if missing destination operand from line, adds an error*/
						{
							addErrorToTable("Error - no destenation operand found on this line.", fileName);
						}
						else /*both source and destination operands found, sent for validity checking and coding it into Code Table*/
						{	
							twoOperandsOpcode(word, src, dest, fileName);
						}
					}

					else if(opcodeNumOfOperands(word) == ONE_OPERAND)/*checks if the opcode configured as using one operand*/
					{
						getNextWordInLine(line,dest);/*gets the destination operand from line*/
						if(strlen(dest) == 0)/*if missing destination operand from line, adds an error*/
						{
							addErrorToTable("Error - no destenation operand found on this line.", fileName);
						}
						else /*destination operand found, sent for validity checking and coding it into Code Table*/
						{
							oneOperandOpcode(word,dest,fileName);
						}
					}

					else /*the opcode configured as using no operands*/
					{
						noOperandsOpcode(word, fileName);
					}
				}

				else /*if the word is not valid opcode or directive, adds an error*/
				{
					addErrorToTable("Error - invalid opcode or directive.",fileName);
				}
			}

			else /*if the line contains only label, adds an error*/
			{
				addErrorToTable("Error - label without a opcode or directive.",fileName);
			}
		}

		/*realloacting the line buffer for the next line in the file*/
		free(lineBuffer);
		lineBuffer = (char*)malloc(LINE_MAX_LENGTH);

		if(lineBuffer == NULL)/*if memory allocation failed*/
		{
			memoryAllocationFailure();/*terminates the program*/
		}
		/*frees the whole allocated memory for the next line in file*/
		free(word);
		free(src);
		free(dest);
		lineCounter++;/*increasing the line counter for the errors line*/
	}
	free(lineBuffer);/*at the end of the file, frees the line buffer allocated memory*/
}


int opcodeNumOfOperands(const char* opcode)
{
	/*depands on the opcode name, returns how many operands are should be used */
	if(strcmp(opcode,"mov") == 0 ||
	   strcmp(opcode,"cmp") == 0 ||
	   strcmp(opcode,"add") == 0 ||
	   strcmp(opcode,"sub") == 0 ||
	   strcmp(opcode,"lea") == 0) {
		
		return TWO_OPERANDS;		
	}
	
	else if(strcmp(opcode,"clr") == 0 || strcmp(opcode,"not") == 0 ||
		strcmp(opcode,"inc") == 0 || strcmp(opcode,"dec") == 0 ||
		strcmp(opcode,"jmp") == 0 || strcmp(opcode,"bne") == 0 || 
		strcmp(opcode,"jsr") == 0 || strcmp(opcode,"red") == 0 || 
		strcmp(opcode,"prn") == 0) {
		
		return ONE_OPERAND;		
	}

	return NO_OPERANDS; /* (strcmp(opcode,"rts") == 0 || strcmp(opcode,"stop") == 0) */
}


void twoOperandsOpcode(char* opcode, char* src, char* dest, char* fileName)
{
	int i=0;
	addressingMode srcAddressMode = checkSrcOperandAddressMode(opcode, src);/*gets the address mode of source operand by its opcode. if not valid, will returns 											NO_ADDRESS_MODE value*/
	addressingMode destAddressMode = checkDestOperandAddressMode(opcode, dest);/*gets the address mode of destinaton operand by its opcode. if not valid, will 											returns NO_ADDRESS_MODE value*/

	while((strcmp(opcodeAddressModes[i].opcodeName, opcode) != 0) && (i < NUM_OF_OPCODES))/*gets the opcode index in the addressing modes array*/
	{
		i++;
	}

	if((srcAddressMode != NO_ADDRESS_MODE) &&
	   (destAddressMode != NO_ADDRESS_MODE))/*if both source and destination operands has valid address modes*/
	{
		/*adds the opcode to the Code Table with its opcode, funct and the correct source and destination address modes*/
		addOpcodeWordIntoCodeTable(opcodeAddressModes[i].opcode, opcodeAddressModes[i].funct, destAddressMode, srcAddressMode);
	
		if(srcAddressMode == IMMEDIATE) /*checks if the source operand is a addressed as a number*/
		{
			addNumberWordIntoCodeTable(src);/*adds it as code word to the Code Table*/
		}

		else if(srcAddressMode == DIRECT)/*checks if the source operand is a addressed as a direct address - label's address*/
		{
			addDirectAddressWordIntoCodeTable(src);/*adds it as code word to the Code Table*/
		}

		else if(srcAddressMode == RELATIVE)/*checks if the source operand is a addressed as a relative address - label's relative address*/
		{
			addRelativeAddressWordIntoCodeTable(src);/*adds it as code word to the Code Table*/
		}	

		else if(srcAddressMode == REGISTER_DIRECT)/*checks if the source operand is a addressed as a direct register - register's value*/
		{
			addRegisterWordIntoCodeTable(src);/*adds it as code word to the Code Table*/
		}


		if(destAddressMode == IMMEDIATE)/*checks if the destination operand is a addressed as a number*/
		{
			addNumberWordIntoCodeTable(dest);/*adds it as code word to the Code Table*/
		}

		else if(destAddressMode == DIRECT)/*checks if the destination operand is a addressed as a direct address - label's address*/
		{
			addDirectAddressWordIntoCodeTable(dest);/*adds it as code word to the Code Table*/
		}

		else if(destAddressMode == RELATIVE)/*checks if the destination operand is a addressed as a relative address - label's relative address*/
		{
			addRelativeAddressWordIntoCodeTable(dest);/*adds it as code word to the Code Table*/
		}	

		else if(destAddressMode == REGISTER_DIRECT)/*checks if the destination operand is a addressed as a direct register - register's value*/
		{
			addRegisterWordIntoCodeTable(dest);/*adds it as code word to the Code Table*/
		}
	}

	else /*if one of the source or destination operands has incorrect address mode which doesn't match with the opcode requirements - adds an error*/
	{
		addErrorToTable("Error - invalid source and destination operands for performed opcode.",fileName);
	}
}


void oneOperandOpcode(char* opcode, char* dest, char* fileName)
{
	int i=0;
	addressingMode destAddressMode = checkDestOperandAddressMode(opcode, dest) ;/*gets the address mode of destinaton operand by its opcode. if not valid, will 											returns NO_ADDRESS_MODE value*/
	while((strcmp(opcodeAddressModes[i].opcodeName, opcode) != 0) && (i < NUM_OF_OPCODES))/*gets the opcode index in the addressing modes array*/
	{
		i++;
	}

	if(destAddressMode != NO_ADDRESS_MODE)/*if destination operand has valid address mode*/
	{
		/*adds the opcode to the Code Table with its opcode, funct and the correct destination address mode. there is no source address mode so its value is 0*/
		addOpcodeWordIntoCodeTable(opcodeAddressModes[i].opcode, opcodeAddressModes[i].funct, destAddressMode, NULL_ADDRESS_MODE);

		if(destAddressMode == IMMEDIATE)/*checks if the destination operand is a addressed as a number*/
		{
			addNumberWordIntoCodeTable(dest);/*adds it as code word to the Code Table*/
		}

		else if(destAddressMode == DIRECT)/*checks if the destination operand is a addressed as a direct address - label's address*/
		{
			addDirectAddressWordIntoCodeTable(dest);/*adds it as code word to the Code Table*/
		}

		else if(destAddressMode == RELATIVE)/*checks if the destination operand is a addressed as a relative address - label's relative address*/
		{
			addRelativeAddressWordIntoCodeTable(dest);/*adds it as code word to the Code Table*/
		}	

		else if(destAddressMode == REGISTER_DIRECT)/*checks if the destination operand is a addressed as a direct register - register's value*/
		{
			addRegisterWordIntoCodeTable(dest);/*adds it as code word to the Code Table*/
		}
	}

	else /*if the destination operand has incorrect address mode which doesn't match with the opcode requirements - adds an error*/
	{
		addErrorToTable("Error - invalid destination operand for performed opcode.",fileName);
	}
}


void noOperandsOpcode(char* opcode, char* fileName)
{
	int i=0;

	while((strcmp(opcodeAddressModes[i].opcodeName,opcode) != 0) && (i < NUM_OF_OPCODES))/*gets the opcode index in the addressing modes array*/
	{
		i++;
	}

	/*adds the opcode to the Code Table with its opcode and funct. there is neither source nor destination operands which makes their address modes value of 0*/
	addOpcodeWordIntoCodeTable(opcodeAddressModes[i].opcode, opcodeAddressModes[i].funct, NULL_ADDRESS_MODE, NULL_ADDRESS_MODE);
}


void getSrcAndDestOperands(char* line, char* src, char* dest, char* fileName)
{
	int i = 0, j = 0;
	boolean commaFlag = FALSE;
	src[0] = '\0';	
	dest[0] = '\0';

	while(line[i] != '\0' && line[i] != ',' && isspace(line[i]))/*skipping the whitespaces*/
	{
		i++;
	}

	if(line[i] != '\0')/*if not the end of string*/
	{
		if (line[i] == ',')/*checks if the char is a comma*/
		{
			addErrorToTable("Error - invalid comma before source operand.", fileName);
		}
		else/*if not a comma*/
		{
			while(line[i] != '\0' && !(isspace(line[i])) && line[i] != ',')/*get the source operand*/
			{
				src[j] = line[i];
				i++;
				j++;
			}
			src[j] = '\0';/*close the src operand*/

			if(line[i] != '\0')/*if there are more chars*/
			{
				if(line[i] == ',')/*check if char is comma*/
				{
					commaFlag = TRUE;
					i++;					
				}
				else/*if not comma, finds it if exists*/
				{
					while(line[i] != ',' && line[i] != '\0' && isspace(line[i]))/*stops where comma or end of string skipping whitespaces*/
					{
						i++;
					}

					if(line[i] == '\0')/*if only whitespaces until the end of string*/
					{
						addErrorToTable("Error - missing destination operand.",fileName);
					}

					else if(line[i] == ',')/*check if char is comma*/
					{
						commaFlag = TRUE;
						i++;						
					}
					else/*found char which is not comma, whitespace or end of string*/
					{
						addErrorToTable("Error - missing comma.",fileName);
					}	
				}
		
				if(commaFlag)/*if there is comma, start to look for the destination operand*/
				{
					while(line[i] != '\0' && line[i] != ',' && isspace(line[i]))/*skipping the whitespaces if exist*/
					{
						i++;
					}

					if(line[i] != '\0')/*if no end of string, means there is destintation operand*/
					{	
						if (line[i] == ',')/*if comma, too many commas - error*/
						{
							addErrorToTable("Error - too many commas between source and destination operands.", fileName);
						}
						else/*if not comma, whitespace or end of string*/
						{
							j=0;
							while(line[i] != '\0' && !(isspace(line[i])) && line[i] != ',')/*get the destination operand*/
							{
								dest[j] = line[i];
								i++;
								j++;
							}
							
							dest[j] = '\0';/*close the string*/
					
				
							while(line[i] !='\0' && isspace(line[i]))/*skips the next whitespaces*/
							{
								i++;
							}
							
							if(line[i] == ',')/*if the line conatains an extra comma, adds an error*/
							{
								addErrorToTable("Error - invalid comma after destination operand.",fileName);
							}

							if(line[i] != '\0' && line[i] != '\n' && !(isspace(line[i])))
							{
								addErrorToTable("Error - too many arguments in the opcode sentence.",fileName);
							}
						}
					}
					else /*if end of line, there is no destination operand, adds an error*/
					{
						addErrorToTable("Error - no destination operand found.",fileName);
					}
				}
				else /*no comma after the source operand*/
				{
					addErrorToTable("Error - missing comma between source and destination operands.",fileName);
				}
			}
			else/*no other chars after source operand means destination operand is missing*/
			{
				addErrorToTable("Error - no destination operand found.",fileName);
			}
		}
	}
	else/*the line has no chars except of whitespaces means no source or destination operands found*/
	{
		addErrorToTable("Error - neither source not destination operands found.",fileName);
	}
}	


addressingMode checkSrcOperandAddressMode(char* opcode, char* src)
{	
	int i=0;
	while((strcmp(opcodeAddressModes[i].opcodeName, opcode) != 0) && (i < NUM_OF_OPCODES)) /*gets the index of the opcode by its name 
												from the opcode address modes array */
	{
		i++;
	}

	if(isOperandNumber(src) && (opcodeAddressModes[i].srcMode.immediate))/*checks if the source operand is a valid number and if the
										source operand allows "immediate" address mode */
	{
		return IMMEDIATE;/*if so, returns the addressing mode - immediate addressing*/
	}

	if(isLabelAlreadyInTable(src) && (opcodeAddressModes[i].srcMode.direct)) /*checks if the source operand is a valid label and if
											source operand allows "direct" address mode*/
	{
		return DIRECT;/*if so, returns the addressing mode - direct addressing*/
	}

	if(isRelativeAddress(src) && (opcodeAddressModes[i].srcMode.relative)) /*checks if the source operand is a valid relative label and if
										source operand allows "relative" address mode*/
	{
		return RELATIVE;/*if so, returns the addressing mode - relative addressing*/
	}

	if(isRegister(src) && (opcodeAddressModes[i].srcMode.direct_register))/*checks if the source operand is a valid register and if
										source operand allows "register direct" address mode*/
	{
		return REGISTER_DIRECT;/*if so, returns the addressing mode - register direct addressing*/
	}

	return NO_ADDRESS_MODE;	/*if non of the above, returns that there is now available address mode for the opcode and source operand*/
}


addressingMode checkDestOperandAddressMode(char* opcode, char* dest)
{	
	int i=0;
	while((strcmp(opcodeAddressModes[i].opcodeName, opcode) != 0) && (i < NUM_OF_OPCODES))/*gets the index of the opcode by its name 
												from the opcode address modes array */
	{
		i++;
	}

	if(isOperandNumber(dest) && (opcodeAddressModes[i].destMode.immediate))/*checks if the destination operand is a valid number and if the
										destination operand allows "immediate" address mode */
	{
		return IMMEDIATE;/*if so, returns the addressing mode - immediate addressing*/
	}

	if(isLabelAlreadyInTable(dest) && (opcodeAddressModes[i].destMode.direct))/*checks if the destination operand is a valid label and if
											destination operand allows "direct" address mode*/
	{
		return DIRECT;/*if so, returns the addressing mode - direct addressing*/
	}

	if(isRelativeAddress(dest) && (opcodeAddressModes[i].destMode.relative))/*checks if the destination operand is a valid relative label and if
										destination operand allows "relative" address mode*/
	{
		return RELATIVE;/*if so, returns the addressing mode - relative addressing*/
	}

	if(isRegister(dest) && (opcodeAddressModes[i].destMode.direct_register))/*checks if the destination operand is a valid register and if
										destination operand allows "register direct" address mode*/
	{
		return REGISTER_DIRECT;/*if so, returns the addressing mode - register direct addressing*/
	}

	return NO_ADDRESS_MODE;	/*if non of the above, returns that there is now available address mode for the opcode and destination operand*/
}

