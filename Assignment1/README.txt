ASSIGNMENT 1
Austin Lowe
09/20/2015

Implement a virtual machine interpreter for PM/0 as described in Lecture 4.

Compiling
Extract all the files from the provided ZIP file.
Navigate into the newly created directory and run the following command:

gcc -o Assignment1 main.c

Executing the program

To run the program simply enter the following command:

./Assignment1

Input file (mcode.txt):

7 0 10
7 0 2
6 0 6
1 0 13
4 0 4
1 0 1
4 1 4
1 0 7
4 0 5
2 0 0
6 0 6
1 0 3
4 0 4
1 0 0
4 0 5
5 0 2
11 0 3

Output file (stacktrace.txt):

LINE	OP	L	M
0	JMP	0	10
1	JMP	0	2
2	INC	0	6
3	LIT	0	13
4	STO	0	4
5	LIT	0	1
6	STO	1	4
7	LIT	0	7
8	STO	0	5
9	OPR	0	0
10	INC	0	6
11	LIT	0	3
12	STO	0	4
13	LIT	0	0
14	STO	0	5
15	CAL	0	2
16	SIO	0	3

				PC	BP	SP	STACK
Initial Values			0	1	0
0	JMP	0	10	10	1	0
10	INC	0	6	11	1	6	0 0 0 0 0 0
11	LIT	0	3	12	1	7	0 0 0 0 0 0 3
12	STO	0	4	13	1	6	0 0 0 0 3 0
13	LIT	0	0	14	1	7	0 0 0 0 3 0 0
14	STO	0	5	15	1	6	0 0 0 0 3 0
15	CAL	0	2	2	7	6	0 0 0 0 3 0
2	INC	0	6	3	7	12	0 0 0 0 3 0 | 0 1 1 16 0 0
3	LIT	0	13	4	7	13	0 0 0 0 3 0 | 0 1 1 16 0 0 13
4	STO	0	4	5	7	12	0 0 0 0 3 0 | 0 1 1 16 13 0
5	LIT	0	1	6	7	13	0 0 0 0 3 0 | 0 1 1 16 13 0 1
6	STO	1	4	7	7	12	0 0 0 0 1 0 | 0 1 1 16 13 0
7	LIT	0	7	8	7	13	0 0 0 0 1 0 | 0 1 1 16 13 0 7
8	STO	0	5	9	7	12	0 0 0 0 1 0 | 0 1 1 16 13 7
9	OPR	0	0	16	1	6	0 0 0 0 1 0
16	SIO	0	3	0	0	0


============================================================================
Assignment Guidelines

The program must be written in C (not C++) and must run on Eustis.

Submit a single ZIP file containing:

Your source code
A readme document indicating how to compile and run
The output of a test program; you can use the example from below
Values and Bounds

Initially SP = 0; BP = 1; PC = 0; IR = 0.

MAX_STACK_HEIGHT = 2000; you can statically allocate a stack store of this size.
MAX_CODE_LENGTH = 500; you can statically allocate a code store of this size.
MAX_LEXI_LEVELS = 3.

Initialize all cells of the stack store to 0.

Input and Output

The input and output files will be named mcode.txt and stacktrace.txt.  Examples are in the Files section.

Every line of input will consist of the three numbers OP, L and M.
The output consists first of the interpreted assembly language with line numbers, then of the execution trace of the program in the virtual machine, showing the registers and stack after each instruction.
Follow the example format as closely as you can.
Don’t show stack[0].
You need to separate each stack frame with a vertical line. The VM does not really explicitly know about stack frames, so use special case code on the CAL and OPR 0, 0 instructions to set this up.
Helpful Hints

Use a three-member structure for instructions.
Rather than actually popping twice and pushing once, you can cheat a bit on arithmetic functions and use pseudocode like the following for OPR 0, 2 (ADD):
    { sp <-- sp – 1; stack[sp] <-- stack[sp] + stack[sp + 1]; }
You can find base(L) with a simple iterative function like this one (the base parameter will be the current value of BP):

    int base(l, base) {
        int b1 = base; // find base L levels down
        while (l > 0) {
            b1 = stack[b1 + 1];
            l--;
        }
        return b1;
    }
