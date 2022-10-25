#include "utils.h"
#include "output.h"

void createObjectOutputFile(char * startOfFile)
{
	char* obFile;
	FILE* f1;
	codeWordNode* currentCodeWordPtr;
	dataWordNode* currentDataWordPtr;

	obFile = getFileName(startOfFile, OB);/*gets the file name with ".ob" ending*/

	f1 = fopen(obFile, "w");/*creates a new file and opens for writing*/

	if(f1 == NULL){/*if file could not be open, prints an error*/
		printf("\nError - file:\"%s\" not found or could not open.\n",obFile);
	} 
	else { /*if file opened successfully*/
		fprintf(f1, " %d %d \n", ICF-IC_START, DCF);/*prints into the file the number of code words and the number of data words*/

		if(codeWordPtrHead != NULL) { /*checks if the Code Table is not empty*/
			currentCodeWordPtr = codeWordPtrHead;
			while(currentCodeWordPtr != NULL) { /*prints into the file each code word address, value and ARE value*/
				fprintf(f1, "%.4d %.3X %c\n", 
					currentCodeWordPtr->address, 
					currentCodeWordPtr->value.labelValue.value,
					currentCodeWordPtr->areValue);
				currentCodeWordPtr = currentCodeWordPtr->next;
			}
		}
	
		if(dataWordPtrHead != NULL) {/*checks if the Data Table is not empty*/
			currentDataWordPtr = dataWordPtrHead;
			while(currentDataWordPtr != NULL) { /*prints into the file each data word address, value and ARE value*/
				fprintf(f1,
					"%.4d %.3X %c\n",
					currentDataWordPtr->dataAddress,
					currentDataWordPtr->value.number.value,
					currentDataWordPtr->areValue); 
				currentDataWordPtr = currentDataWordPtr->next;
			}
		}
		fclose(f1);/*close the file*/
		free(obFile);/*frees the allocated memory for the ".ob" file name*/
	}
}

void createEntryOutputFile(char* startOfFile)
{
	char* entFile;
	FILE* f1;
	labelNode* currentLabelPtr;
	boolean existEntry = FALSE;/*flag used to check if there is any entry type label in the Label Table*/

	currentLabelPtr = labelPtrHead;
	while(currentLabelPtr != NULL) { /*checks if there is any entry type label*/
		if(currentLabelPtr->type == CODE_ENTRY ||
		   currentLabelPtr->type == DATA_ENTRY) {
			existEntry = TRUE;/*labal with entry type found*/
		}
		currentLabelPtr = currentLabelPtr->next;
	}
	
	entFile = getFileName(startOfFile,ENT);/*gets the file name with ".ent" ending*/
	
	if(existEntry){/*if at least one label with entry type found*/
		f1 = fopen(entFile,"w");/*creates a new file and opens for writing*/
		if(f1 == NULL)/*if file could not be open, prints an error*/
		{
			printf("\nError - file:\"%s\" not found or could not open.\n", entFile);
		}
		else {/*if file opened successfully*/
		{
			currentLabelPtr = labelPtrHead;
			while(currentLabelPtr != NULL) { /*for each entry type label, prints it into the ".ent" file with its name and address*/	
				if(currentLabelPtr->type == CODE_ENTRY ||
				   currentLabelPtr->type == DATA_ENTRY) {
					fprintf(f1,
						"%s %.4d\n", 
						currentLabelPtr->name, 
						currentLabelPtr->address);
				}
				currentLabelPtr = currentLabelPtr->next;
			}
			
		fclose(f1);/*close the file*/
		}
	}
	free(entFile);/*frees the allocated memory for the ".ent" file name*/
}

void createExternalOutputFile(char* startOfFile)
{
	char* extFile;
	FILE* f1;
	externalNode* currentExternalPtr;

	extFile = getFileName(startOfFile, EXT);/*gets the file name with ".ext" ending*/
	if(extrenalPtrHead != NULL)/*checks if the External Labels Table is not empty */
	{
		f1 = fopen(extFile,"w");/*creates a new file and opens for writing*/
		if(f1 == NULL) {/*if file could not be open, prints an error*/
			printf("\nError - file:\"%s\" not found or could not open.\n",extFile);
		}
		else { /*if file opened successfully*/
			currentExternalPtr = extrenalPtrHead;
			
			 /*for each external label from the External Labels Table, prints it into the ".ext" file with its name and address where its used*/
			while(currentExternalPtr != NULL) {
				fprintf(f1,
					"%s %.4d\n",
					currentExternalPtr->name,
					currentExternalPtr->address); 
				currentExternalPtr = currentExternalPtr->next; 
			}	
			fclose(f1);/*close the file*/
		}
	}
	free(extFile);/*frees the allocated memory for the ".ext" file name*/
}

