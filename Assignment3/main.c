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
    struct token *root = NULL;

    ifp = fopen("tokenlist.txt", "r+");
    ofp = fopen("mcode.txt", "w+");
    stfp = fopen("symboletable.txt", "w+");




    fclose(ifp);
    fclose(ofp);
    fclose(stfp);
	return 0;
}

