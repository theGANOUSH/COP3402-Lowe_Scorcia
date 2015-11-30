#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printfile(FILE *input);
void error(int code);

int main(int argc, char* argv[])
{
    FILE *argument;
    int i = 0;
    int status = 0;

    status = system("./Assignment2");

    if(status == 0)
    {
        printf("Assignment2 -- DONE\n");
//        status = system("./Assignment3");

        if(status == 0)
        {
            //printf("Assignment3 -- DONE/n");
            status = system("./Assignment1");
        }
    }

    for(i = 1; i < argc; i++)
    {
        //printf("%s\n", argv[i]);

        if(strcmp(argv[i], "-v") == 0)
        {
            argument = fopen("stacktrace.txt", "r");
            printfile(argument);
        }
        if(strcmp(argv[i], "-t") == 0)
        {
            argument = fopen("tokenlist.txt", "r");
            printfile(argument);
        }
        if(strcmp(argv[i], "-s") == 0)
        {
            argument = fopen("symboltable.txt", "r");
            printfile(argument);
        }
        if(strcmp(argv[i], "-m") == 0)
        {
            argument = fopen("mcode.txt", "r");
            printfile(argument);
        }
        if(strcmp(argv[i], "-a") == 0)
        {
            argument = fopen("acode.txt", "r");
            printfile(argument);
        }

    }


    return 0;
}

void printfile(FILE *input)
{
    char buffer;

    if(input != NULL)
    {
        while(fscanf(input, "%c", &buffer) != EOF)
        {
            printf("%c", buffer);
        }

        printf("\n");

        fclose(input);
    }

    else
    {
        return;
    }

}

void error(int code)
{
    switch(code)
    {
        case 404:
            printf("ERROR: FILE NOT FOUND");
            break;
        case 1:
            printf("ERROR: PROGRAM HALTED");
            break;
    }
}
