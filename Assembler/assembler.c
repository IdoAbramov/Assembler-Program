#include "utils.h"
#include "process.h"


/*starts the assembler proccess function which contains for each file: the input check, two rounds of process and the output*/
int main(int argc, char* argv[])
{
	startAssemblerProcess(argc, argv);
	return EXIT_SUCCESS;
}

