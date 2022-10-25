# OpenU-Assembler
OpenU - Assembler project

C under Linux.

The program takes an assembly code file, parsing it and translates to binary code (hexadecimal form).


The program output is (maximum):

*  .obj - contains the machine code and data.

*  .ent - contains the enteries labels (which its code is in the input file).

*  .ext - contains the external labels (which its code is in external files).


The assembler has 8 registers (r0 - r7). each register in size of a word (= 12 bits).

Each machine command line is 1 to 3 words. 

The first word structure will be:

*  0 - 1 : Destination Address (dest).

*  2 - 3 : Source Address (src).

*  4 - 7 : Function type (funct).
  
*  8 - 11 : Operation code (opcode).


Its source & destination Addresses values set according the opcode arguements type of addressing 

(immediate (=00), direct (=01), relative (=10), register direct (=11))

Each opcode name (such as mov, cmp, add and so on) has its funct + opcode, 

