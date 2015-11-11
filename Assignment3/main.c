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

FILE *ifp;
FILE *ofp;
FILE *stfp;

struct token
{
    int OP;
    int L;
    int M;
    int line;
    char opString[4];
    struct token *left;
    struct token *right;
};

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



    fclose(ifp);
    fclose(ofp);
	return 0;
}
