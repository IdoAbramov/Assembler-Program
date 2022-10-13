#ifndef CODE_TABLE_H
#define CODE_TABLE_H

/*definitions of constants*/
#define CODE_WORD_SIZE 12
#define OPCODE_BITS_SIZE 4
#define FUNCT_BITS_SIZE 4
#define SRC_BITS_SIZE 2
#define DEST_BITS_SIZE 2
#define FLAG_BITS_SIZE 1


/***** Code Table Definitions *****/

/*structure of number code word value contains a bit-field value in size of a code word */
typedef struct codeNumber{
	unsigned int value : CODE_WORD_SIZE;
}codeNumber;

/*structure of label code word value contains a bit-field value in size of a code word */
typedef struct codeLabel{
	unsigned int value : CODE_WORD_SIZE;
}codeLabel;

/*structure of register code word value contains a bit-field value in size of a code word */
typedef struct codeRegister{
	unsigned int value : CODE_WORD_SIZE;
}codeRegister;

/*structure of opcode code word value contains bit-fields of: 
destination address mode, source address mode, funct number and opcode number*/
typedef struct codeOpcode{
	unsigned int destAddressMode : DEST_BITS_SIZE;
	unsigned int srcAddressMode : SRC_BITS_SIZE;
	unsigned int funct : FUNCT_BITS_SIZE;
	unsigned int opcode : OPCODE_BITS_SIZE;
}codeOpcode;

/*structure of code word contains its address, the value(depands on the code word type) and the ARE value. 
also has pointer for the next code word in the Code Table*/
typedef struct codeWordNode{
	int address;
	union{
	codeNumber number;
	codeLabel labelValue;
	codeRegister reg;
	codeOpcode opcode;
	}value;
	char areValue;
	struct codeWordNode* next;
}codeWordNode;


/***** Table of data for code words *****/

/*structure of source operand allowed addressing modes represented by flags (0 - false, 1 - true) */
typedef struct srcOperandAllowedAddresMode
{
	unsigned int immediate : FLAG_BITS_SIZE;
	unsigned int direct : FLAG_BITS_SIZE;
	unsigned int relative : FLAG_BITS_SIZE;
	unsigned int direct_register : FLAG_BITS_SIZE;
}srcAllowedMode;

/*structure of destination operand allowed addressing modes represented by flags (0 - false, 1 - true) */
typedef struct destOperandAllowedAddresMode
{
	unsigned int immediate : FLAG_BITS_SIZE;
	unsigned int direct : FLAG_BITS_SIZE;
	unsigned int relative : FLAG_BITS_SIZE;
	unsigned int direct_register : FLAG_BITS_SIZE;
}destAllowedMode;

/*structure of opcode contains its name, opcode value, funct value and 
its allowed modes of source and destination operands */
typedef struct opcodeAllowedSrcAndDestAddressModes
{
	char* opcodeName;
	unsigned int opcode : OPCODE_BITS_SIZE;
	unsigned int funct: FUNCT_BITS_SIZE;
	srcAllowedMode srcMode;
	destAllowedMode destMode;

}opcodeAllowedModes;

#endif 
