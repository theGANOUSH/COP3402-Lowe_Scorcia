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
<<<<<<< HEAD
    //Set up variables
    struct token *root = NULL;
=======

>>>>>>> origin/master
    ifp = fopen("tokenlist.txt", "r+");
    stfp = fopen("symboletable.txt", "w+");
    int c, i = 0, symbol, blanks = 0, lineNum = 1;
    char letter;


<<<<<<< HEAD
    //Read char by char, and get symbol table
    fscanf(ifp, "%d", &c);
    while(!= feof(ifp)){
        if(c == 2 || c == 3){
            symbol = fgetc(ifp);
            while(blanks < 2){
                if(symbol==' '){
                    blanks += 1;
                    symbol = fgetc(ifp);
                }
                else(){
                    fprintf(stpf, "%c", symbol);
                    symbol = fgetc(ifp);
                }
            }
        }
        fprintf(stpf, " ");
        blanks = 0;
        fscanf(ifp, "%d", &c);
    }

    //Close all
    fclose(ifp);
    fclose(stfp);


    //reopen ifp and deal with machine code
    FILE *newIFP = fopen("tokenlist.txt", "rb");
    ofp = fopen("mcode.txt", "w+");
=======
    if(ifp == NULL)
    {
        printf("INPUT FILE IS NOT VALID\nEXITING");

        return -1;
    }

>>>>>>> origin/master



    fclose(ifp);
    fclose(ofp);
	return 0;
}
<<<<<<< HEAD
=======

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

        while(currentToken == commasym)
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

        }

    }

    if(currentToken == varsym)
    {
        while(currentToken == commasym)
        {
            getToken();

            if(currentToken !=identsym)
            {
                printf("ERROR");
                //HALT

            }


        }
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





>>>>>>> origin/master
