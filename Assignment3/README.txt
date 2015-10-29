ASSIGNMENT 3

Austin Lowe
Kyle Scorcia

11/16/15

Implement the parser (syntax analyzer) and PM/0 code generator for PL/0.

Compiling
Extract all the files from the provided ZIP file.
Navigate into the newly created directory and run the following command:

gcc -o Assignment3 main.c

Executing the program

To run the program simply enter the following command:

./Assignment3

INPUT FILE

tokenlist.txt:


OUTPUT FILES

symboltable.txt:

mcode.txt:


============================================================================


Implement the parser (syntax analyzer) and PM/0 code generator for PL/0.

Assignment

Your program must read the output of the PL/0 scanner you developed in Assignment 2, and produce:

    The symbol table
    Machine code suitable for execution in the virtual machine you developed in Assignment 1

You must detect all errors specified in the PL/0 pseudocode and print appropriate messages for them.  If there is an error, print it and its line number, then halt.  If there is no error, print nothing and output the files below.

Restrictions, Notes and Hints

    Lean heavily on the PL/0 reference document version 2.
    The example errors in the document are neither complete nor binding - you may create your own.
    Remember that a parser is just a big automaton. (The pseudocode will make that clearer.)
    You are about to be quite happy that PM/0 is a stack architecture.
    Test the program on Eustis.
    COMPLETE THE PARSER LOGIC BEFORE BEGINNING WORK ON CODE GENERATION. 

Before You Ask

    We are not making you write an assembler, because you will already be able to view the disassembled code with your virtual machine, so it would just be extra work.
    We are not splitting up the parser and code generator, because a C implementation of a good way to output and input the parse tree as a text file would be more trouble than it would be worth.

Filenames

    The input token list will be read from tokenlist.txt
    The output symbol table will be written to symboltable.txt
    The output machine code will be written to mcode.txt

Formatting Notes

Follow the format from the PL/0 reference document version 2, and the example files. 

Submissions

Submit to Webcourses (inside a single zip file):

    Source code of your parser and code generator.
    Instructions to compile and use your program in a readme document. Indicate the names of all team members in this file.
    The record of one successful run, including:

    The input source file
    All of the scanner output files
    All of the parser outfiles
    The virtual machine output file

4.   The record of ten unsuccessful runs, including:

    Ten source programs causing ten different parser-level errors
    A screenshot of your parser's output of each error
    (Compile these into a Microsoft Word or PDF document and include it in the zip file.)
