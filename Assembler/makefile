assembler:	assembler.o process.o tables.o validators.o utils.o output.o
	gcc -g -ansi -Wall -pedantic assembler.o process.o tables.o validators.o utils.o output.o -lm -o assembler


assembler.o:	assembler.c utils.h process.h
	gcc -c -ansi -Wall -pedantic assembler.c -o assembler.o

process.o:	process.c utils.h validators.h tables.h output.h process.h
	gcc -c -ansi -Wall -pedantic process.c -o process.o	


tables.o:	tables.c utils.h tables.h validators.h
	gcc -c -ansi -Wall -pedantic tables.c -lm -o tables.o

validators.o:	validators.c utils.h validators.h
	gcc -c -ansi -Wall -pedantic validators.c -o validators.o


utils.o:	utils.c utils.h dataTable.h codeTable.h errorsTable.h externalsTable.h labelsTable.h
	gcc -c -ansi -Wall -pedantic utils.c -o utils.o

output.o:	output.c utils.h output.h
	gcc -c -ansi -Wall -pedantic output.c -o output.o
