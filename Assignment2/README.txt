ASSIGNMENT 2

Austin Lowe
Kyle Scorcia

10/12/15

Implement the lexical analyzer - the scanner - for PL/0.

Compiling
Extract all the files from the provided ZIP file.
Navigate into the newly created directory and run the following command:

gcc -o Assignment2 main.c

Executing the program

To run the program simply enter the following command:

./Assignment2

INPUT FILE

(input.txt):

OUTPUT FILES

cleaninput.txt:

lexemetable.txt:

tokenlist.txt:

============================================================================

Assignment 2

Your program must read a source code file written in PL/0 and produce: 

The source code file without comments
The lexeme table
The token list
You must detect the following errors:

Variables that don't start with letters
Numbers and identifiers that are too long
Invalid symbols
Do not get ahead of yourself and try to detect other errors.  The input file is not necessarily grammatically correct and you are not (yet) parsing it for grammar!

If there is an error, print the error and its associated line number to standard output, then halt.  If there is no error, print nothing to standard output and output the files below.

Restrictions, Notes and Hints

Identifiers may not be longer than 11 characters
Numbers may not be longer than 5 digits
Do not tokenize comments or whitespace
Remember that a scanner is just a set of automata!
Test this program on Eustis!
Filenames

The input filename will be named input.txt
The source code without comments will be written to cleaninput.txt
The lexeme table will be written to lexemetable.txt
The token list will be written to tokenlist.txt
Formatting Notes

Follow the format from the PL/0 reference document version 1, and the example files.  (All are available in the "Files" section.)

Submissions

Submit to Webcourses (inside a single zip file):

Source code of your lexical analyzer.
Instructions to compile and use your program in a readme document. Indicate the names of all team members in this file.
The record of one successful run, including the input source file and all four output files.