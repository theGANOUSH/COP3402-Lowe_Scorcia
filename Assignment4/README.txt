ASSIGNMENT 4

Austin Lowe
Kyle Scorcia

11/30/15

Implement an integrated compiler and virtual machine driver for PL/0.


Compiling
Extract all the files from the provided ZIP file.
Navigate into the newly created directory and run the following command:

make

Executing the program

To run the program simply enter the following command:

./Assignment4

INPUT FILE

tokenlist.txt:


OUTPUT FILES

symboltable.txt:

mcode.txt:


============================================================================


Implement an integrated compiler and virtual machine driver for PL/0.

Assignment
Your program reuses code from all three previous assignments to create a unified compiler and virtual machine for executing PL/0 programs.  Your program must:

    Read a PL/0 source code file from input.txt
    Execute the scanning step. On error, print the error with line number, and exit.  Otherwise, produce:
        The source code file without comments in cleaninput.txt
        The lexeme table in lexemetable.txt
        The token list in tokenlist.txt
    Execute the parsing step. On error, print the error and exit.  Otherwise, produce:
        The symbol table in symboltable.txt
        Machine code suitable for execution in the virtual machine in mcode.txt
    Execute the virtual machine. Produce (in split files, whereas previously they were one file):
        The disassembled code in acode.txt
        The execution stack trace in stacktrace.txt

Your program must accept any or all of the following command line switches.  Each switch prints its corresponding type of output to the screen, as well as writing it to its corresponding file.  You must accept the switches in any order, but the order of the switches does not change the order of output.

    -t The token list
    -s The symbol table
    -m The machine code
    -a The disassembled code
    -v The virtual machine execution stack trace

Submissions

Submit to Webcourses (inside a single zip file):

    Your complete source code.
    Instructions to compile and use your program in a readme document. Indicate the names of all team members in this file.
    The record of one successful run, including the input source file and all output files.
    The record of two unsuccessful runs, including:
        A source program causing a scanning error
        A source program causing a parsing error
        A screenshot of your compiler's output of each error

      Compile these into a Microsoft Word or PDF document and include it in the zip file.
