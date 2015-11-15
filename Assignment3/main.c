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
int stackIndex = 0;
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
void constantSym();
void variableSym();
void procedure();
void getIdentifier(symbol node);
symbol fetchSymbol(char* name, int level);


int main()
{

    //Set up variables
    struct token *root = NULL;



    ifp = fopen("tokenlist.txt", "r+");
    stfp = fopen("symboletable.txt", "w+");
    int c, i = 0, symbol, blanks = 0, lineNum = 1;
    char letter;


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

    if(ifp == NULL)
    {
        printf("INPUT FILE IS NOT VALID\nEXITING");

        return -1;
    }



    fclose(ifp);
    fclose(ofp);
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
    symbol temp;
    if(currentToken == callsym)
    {
        getToken();

        if(currentToken != identsym)
        {
            printf("ERROR");
        }

        //temp =

        getToken();
    }

    if(currentToken == beginsym)
    {
        getToken();

        statement();

        while(currentToken == semicolonsym)
        {

            getToken();
            statement();

        }

        if(currentToken != endsym)
        {
            printf("ERROR");

        }

        getToken();

    }

    if(currentToken == ifsym)
    {
        getToken();

        condition();

        if(currentToken != thensym)
        {
            printf("ERROR");

        }
        getToken();

        statement();

        if(currentToken == elsesym)
        {
            getToken();
            statement();

        }


    }

}

void block()
{
    if(currentToken == constsym)
    {
        constantSym();
        getToken();
    }

    if(currentToken == varsym)
    {
       variableSym();
       getToken();
    }

    while(currentToken == procsym)
    {
        procedure();
        getToken();
    }

    statement();

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

void constantSym()
{
    symbol temp;
    getToken();

    do
    {

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

    if(currentToken != semicolonsym)
    {
        printf("ERROR");

    }


}

void variableSym()
{
    symbol temp;
    do
    {
        getToken();

        if(currentToken !=identsym)
        {
            printf("ERROR");
            //HALT

        }

        getIdentifier(temp);

        temp.kind = 2;
        temp.level = lexilevel;
        temp.addr = 4 + totalVars++;

        symbol_table[totalSymbols++] = temp;

        getToken();

    }while(currentToken == commasym);

    if(currentToken != semicolonsym)
    {
        printf("ERROR");

    }
}

void procedure()
{
    int totalProcess = 0;

    do
    {
        totalProcess++;

        getToken();

        if(currentToken != identsym)
        {
            printf("ERROR");

        }

        getToken();

        symbol_table[currentToken].level = lLevel;


        getToken();

        if(currentToken != semicolonsym)
        {
            printf("ERROR");

        }
        getToken();

        block():

        if(currentToken != semicolonsym)
        {
            printf("ERROR");

        }

        getToken();

    }while(currentToken == procsym)


}

symbol fetchSymbol(char* name, int level)
{
    symbol *temp = (symbol*)malloc(sizeof symbol);
    int i;
    int lLevelMax = 0;

    for(i =0; i < totalSymbols; i++)
    {
        if(strcmp(name, symbol_table[i].name) == 0)
        {
            if(symbol_table[i].level <= level)
            {
                if(symbol_table.level >= lLevelMax)
                {
                    temp = &symbol_table[i];

                }

            }
        }

    }

    return *temp;
}


