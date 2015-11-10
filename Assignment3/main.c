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
    int line;
    char tokenString[11];
    struct token *next;
};

int main()
{
    struct token *root = NULL;
    
    int temp;

    ifp = fopen("tokenlist.txt", "r+");
    ofp = fopen("mcode.txt", "w+");
    stfp = fopen("symboletable.txt", "w+");
    
    
    while(fscanf(ifp, "%d", &temp) != EOF)
    {
    	switch(temp)
    	{
    		case identsym:
    		break;
    		
    		case numbsym:
    		break;
    		
    		default:
    		break;
    		
    	}
    	
    	
    }



    fclose(ifp);
    fclose(ofp);
    fclose(stfp);
	return 0;
}

