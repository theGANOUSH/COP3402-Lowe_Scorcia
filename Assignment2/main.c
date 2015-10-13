/*
Author: Austin Lowe & Kyle Scorcia
Date: 10/12/2015
Assignment 2: PL/0 Scanner
COP3402 Systems Software
Matthew B. Gerber (Adjunct)
University of Central Florida
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

FILE *ifp, *lfp, *cifp, tlfp*;


typedef enum {
nulsym = 1, identsym, numbersym, plussym, minussym,
multsym, slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
periodsym, becomessym, beginsym, endsym, ifsym, thensym,
whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
readsym, elsesym
} token_type;

int main()
{
    ifp = fopen("input.txt", "r");
    lfp = fopen("lexemetable.txt", "w");
    cifp = fopen("cleaninput.txt", "w");
    tlfp = fopen("tokenlist.txt", "w");

    return 0;
}
