/* Output Functions and Documentation*/
#ifndef OUTPUT_H
#define OUTPUT_H

/**
*The function gets a name of a file, checks if the Code and Data Tables are filled and prints them (by order) 
*into ".ob" type file. if both tables are empty, will create an empty ".ob" type file.
*@param startOfFile - a string which contains the name of a file before the '.'
*/
void createObjectOutputFile(char * startOfFile);

/**
*The function gets a name of a file, checks if the Labels Table contains one or more entry type labels and prints them (by order) 
*into ".ent" type file. if there is no entry type labels at all, will not create the ".ent" file.
*@param startOfFile - a string which contains the name of a file before the '.'
*/
void createEntryOutputFile(char * startOfFile);

/**
*The function gets a name of a file, checks if the Externals Table contains at least one line and prints them (by order) 
*into ".ext" type file. if there is no lines at all, will not create the ".ext" file.
*@param startOfFile - a string which contains the name of a file before the '.'
*/
void createExternalOutputFile(char * startOfFile);

#endif
