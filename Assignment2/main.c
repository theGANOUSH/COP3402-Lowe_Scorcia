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

#define MAX_INT_SIZE 5
#define MAX_IDENT_SIZE 11

FILE *ifp, *lfp, *cifp, *tlfp;

int HALT = 0;


typedef enum {
nulsym = 1, identsym, numbersym, plussym, minussym,
multsym, slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
periodsym, becomessym, beginsym, endsym, ifsym, thensym,
whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
readsym, elsesym
} token_type;

struct lexeme
{
    int type;
    char *identifier;
    struct lexeme *next;
};

char removeComment(char a);
char removeComment(char a)
{
    if(a == '/')
    {
        return fgetc(ifp);
    }
    else
    {
        return removeComment(fgetc(ifp));
    }

}

void getCleanInput();
void getCleanInput()
{
    char a, b;
    while(a != EOF)
    {
        a = fgetc(ifp);


        if(a == '/')
        {
            b = fgetc(ifp);

            if(b == '*')
            {
                a = removeComment(b);
            }
        }

        if(a != EOF)
        {
            fprintf(cifp, "%c", a);
        }

    }
}


int main()
{
    char b = '\0';

    ifp = fopen("input.txt", "r");
    lfp = fopen("lexemetable.txt", "w+");
    cifp = fopen("cleaninput.txt", "w+");
    tlfp = fopen("tokenlist.txt", "w+");

    fprintf(lfp, "LEXEME\tTOKEN TYPE\n");

    getCleanInput();

    rewind(cifp);

    b = fgetc(cifp);

    while(b != EOF)
    {
        char nDigit;
        if(isdigit(b))
        {
            nDigit = fgetc(cifp);
            printf("%c", b);

            while(isdigit(nDigit))
            {
                printf("%c", nDigit);
                nDigit = fgetc(cifp);

            }
            printf("\n");
        }

        b = fgetc(cifp);
    }

    fclose(ifp);
    fclose(lfp);
    fclose(cifp);
    fclose(tlfp);

    return 0;
}
