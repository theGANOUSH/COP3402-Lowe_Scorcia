/*
Author: Austin Lowe & Kyle Scorcia
Date: 11/16/2015
Assignment 3: PL/0 Parser
COP3402 Systems Software
Matthew B. Gerber (Adjunct)
University of Central Florida
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SYMBOL_TABLE_SIZE 100

FILE *ifp;
FILE *ofp;
FILE *stfp;

int currentToken = 0;
int totalSymbols = 0;
int lLevel = 0;

typedef enum
{
    nulsym = 1, identsym, numbersym, plussym, minussym, multsym, slashsym, oddsym,
    eqsym, neqsym, lessym, leqsym, gtrsym, geqsym, lparentsym, rparentsym, commasym,
    semicolonsym, periodsym, becomessym, beginsym, endsym, ifsym, thensym,  whilesym,
    dosym, callsym, constsym, varsym, procsym, writesym, readsym, elsesym
} token_type;

typedef struct symbol
 {
    int kind;
    char name[12];
    int val;
    int level;
    int addr;
} symbol;

symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];

void getToken();
void program();
void block();
void getIdentifier(symbol node);


int main()
{

    ifp = fopen("tokenlist.txt", "r+");
    ofp = fopen("mcode.txt", "w+");
    stfp = fopen("symboletable.txt", "w+");

    if(ifp == NULL)
    {
        printf("INPUT FILE IS NOT VALID\nEXITING");

        return -1;
    }




    fclose(ifp);
    fclose(ofp);
    fclose(stfp);
	return 0;
}

void getToken()
{
    fscanf(ifp, "%d", &currentToken);

}

void getIdentifier(symbol node)
{
    fscanf(ifp, "%s", node.name);
}
void statement()
{
    getToken();
    if(currentToken != becomessym)
    {
        printf("ERROR");
        //Halt
    }

}
void block()
{
    if(currentToken == constsym)
    {
        getToken();

        do
        {
            symbol temp;
            getToken();

            if(currentToken != identsym)
            {
                printf("ERROR");

            }
            temp.kind = 1;
            getIdentifier(temp);
            temp.level = lLevel;
            temp.addr = -1;

            getToken();

            if(currentToken != eqsym)
            {
                printf("ERROR");

            }
            getToken();

            if(currentToken != numbersym)
            {
                printf("ERROR");

            }

            getToken();
            temp.val = currentToken;
            symbol_table[totalSymbols++] = temp;

            getToken();

        }while(currentToken == commasym);

    }

    if(currentToken == varsym)
    {
        do
        {
            getToken();

            if(currentToken !=identsym)
            {
                printf("ERROR");
                //HALT

            }


        }while(currentToken == commasym);
    }

    if(currentToken != semicolonsym)
    {
        printf("ERROR");

    }

    getToken();

    while(currentToken == procsym)
    {
        getToken();

        if(currentToken != identsym)


    }

}

void program()
{
    getToken();

    block();

    if(currentToken != periodsym)
    {
        printf("ERROR");
        //halt
    }


}





