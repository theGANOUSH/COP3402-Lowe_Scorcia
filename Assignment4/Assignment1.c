
/*
Author: Austin Lowe
Date: 9/20/2015
Assignment 1: PM/0 Virtual Machine
COP3402 Systems Software
Matthew B. Gerber (Adjunct)
University of Central Florida
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3

FILE *ifp;
FILE *ofp;

int sPoint = 0, bPoint = 1, pCount = 0, HALT = 0;
int STACK[MAX_STACK_HEIGHT] = {0};
int LEXI[MAX_LEXI_LEVELS];
int LEXILvl;

struct instruction
{
    int OP;
    int L;
    int M;
    int line;
    char opString[4];
    struct instruction *previous;
    struct instruction *next;
};

void ALUOp(struct instruction *node);
void ALUOp(struct instruction *node)
{
    switch(node->M)
    {
        case 0:
            //Return
            LEXILvl = LEXILvl - 1;
            sPoint = bPoint - 1;
            pCount = STACK[sPoint + 4];
            bPoint = STACK[sPoint + 3];
            break;

        case 1:
            //Pop the stack and push the negation of the result.
            STACK[sPoint] = -STACK[sPoint];
            break;
        case 2:
            //Pop the stack twice, add the values, and push the result.
            sPoint = sPoint - 1;
            STACK[sPoint] = STACK[sPoint] + STACK[sPoint + 1];
            break;
        case 3:
            //Pop the stack twice, subtract the top value from the second value, and push the result.
            sPoint = sPoint - 1;
            STACK[sPoint] = STACK[sPoint] - STACK[sPoint + 1];
            break;
        case 4:
            //Pop the stack twice, multiply the values, and push the result.
            sPoint = sPoint - 1;
            STACK[sPoint] = STACK[sPoint] * STACK[sPoint + 1];
            break;
        case 5:
            //Pop the stack twice, divide the second value by the top value, and push the quotient.
            sPoint = sPoint - 1;
            STACK[sPoint] = STACK[sPoint] / STACK[sPoint + 1];
            break;
        case 6:
            //Pop the stack, push 1 if the value is odd, and push 0 otherwise.
            STACK[sPoint] = STACK[sPoint] % 2;
            break;
        case 7:
            //Pop the stack twice, divide the second value by the top value, and push the remainder.
            sPoint = sPoint - 1;
            STACK[sPoint] = STACK[sPoint] % STACK[sPoint + 1];
            break;
        case 8:
            //Pop the stack twice and compare the top value t with the second value s. Push 1 if s = t and 0 otherwise.
            sPoint = sPoint - 1;
            STACK[sPoint] = STACK[sPoint] == STACK[sPoint + 1];
            break;
        case 9:
            //Pop the stack twice and compare the top value t with the second value s. Push 1 if s not equal t and 0 otherwise.
            sPoint = sPoint - 1;
            STACK[sPoint] = STACK[sPoint] != STACK[sPoint + 1];
            break;
        case 10:
            //Pop the stack twice and compare the top value t with the second value s. Push 1 if s < t and 0 otherwise.
            sPoint = sPoint - 1;
            STACK[sPoint] = STACK[sPoint] < STACK[sPoint + 1];
            break;
        case 11:
            //Pop the stack twice and compare the top value t with the second value s. Push 1 if s ≤ t and 0 otherwise.
            sPoint = sPoint - 1;
            STACK[sPoint] = STACK[sPoint] <= STACK[sPoint + 1];
            break;
        case 12:
            //Pop the stack twice and compare the top value t with the second value s. Push 1 if s > t and 0 otherwise.
            sPoint = sPoint - 1;
            STACK[sPoint] = STACK[sPoint] > STACK[sPoint + 1];
            break;
        case 13:
            //Pop the stack twice and compare the top value t with the second value s. Push 1 if s ≥ t and 0 otherwise.
            sPoint = sPoint - 1;
            STACK[sPoint] = STACK[sPoint] >= STACK[sPoint + 1];
            break;
        default:
            printf("Unrecognized Code");
            break;
    }

}

void opCodeTranslate(struct instruction *node);
void opCodeTranslate(struct instruction *node)
{
    switch(node->OP)
    {
        case 1:
            //Push the literal value M onto the stack
            strcpy(node->opString, "LIT");
            //fprintf(ofp, "LIT");
            break;
        case 2:
            //Return from a procedure call OR Perform an ALU operation, specified by M
            strcpy(node->opString, "OPR");
            //fprintf(ofp, "OPR");
            ALUOp(node);
            break;
        case 3:
            //Read the value at offset M from L levels down (if L=0, our own frame) and push it onto the stack
            strcpy(node->opString, "LOD");
            //fprintf(ofp, "LOD");
            break;
        case 4:
            //Pop the stack and write the value into offset M from L levels down - if L=0, our own frame.
            strcpy(node->opString, "STO");
            //fprintf(ofp, "STO");
            break;
        case 5:
            //Call the procedure at M.
            strcpy(node->opString, "CAL");
            //fprintf(ofp, "CAL");
            break;
        case 6:
            //Allocate enough space for M local variables. We will always allocate at least four.
            strcpy(node->opString, "INC");
            //fprintf(ofp, "INC");
            break;
        case 7:
            //Branch to M.
            strcpy(node->opString, "JMP");
            //fprintf(ofp, "JMP");
            break;
        case 8:
            //Pop the stack and branch to M if the result is 0.
            strcpy(node->opString, "SIO");
            //fprintf(ofp, "SIO");
            break;
        case 9:
            //Pop the stack and write the result to the screen.
            strcpy(node->opString, "SIO");
            //fprintf(ofp, "SIO");
            break;
        case 10:
            //Read an input from the user and store it at the top of the stack.
            strcpy(node->opString, "SIO");
            //fprintf(ofp, "SIO");
            break;
        case 11:
            //Stop the machine.
            strcpy(node->opString, "SIO");
            //fprintf(ofp, "SIO");
            break;
        default:
            printf("Unrecognized Code");
            break;
    }

}

void printStack();
void printStack()
{
    if(HALT == 0)
    {

        int j = 0;
        int frame = 0;

        for(j = 1; j <= sPoint; j++)
        {
            if(LEXI[frame] > 0 && j > LEXI[frame])
            {

                fprintf(ofp, "| ");
                frame++;
            }
            fprintf(ofp, "%d ", STACK[j]);

        }
    }
}

void readFile(struct instruction **node, int OPcode, int line);
void readFile(struct instruction **node, int OPcode, int line)
{
    int lLevel = 0, mData = 0;

    struct instruction *end, *temp;

    fscanf(ifp, "%d", &lLevel);
    fscanf(ifp, "%d", &mData);

    temp = (struct instruction *)malloc(sizeof(struct instruction));
    temp->OP = OPcode;
    temp->L = lLevel;
    temp->M = mData;
    temp->line = line;
    temp->previous = NULL;
    temp->next = NULL;

    opCodeTranslate(temp);

    if(*node == NULL)
    {
        *node = temp;

    }
    else
    {
        end = *node;

        while(end->next != NULL)
        {
            end = end->next;
        }

        end->next = temp;
        temp->previous = end;

    }

}

void printList(struct instruction *node);
void printList(struct instruction *node)
{
    fprintf(ofp, "LINE\tOP\tL\tM\n");

    while(node != NULL)
    {

        fprintf(ofp, "%d\t%s", node->line, node->opString);
        fprintf(ofp, "\t%d\t%d\n", node->L, node->M);
        node = node->next;
    }
    fprintf(ofp, "\n\t\t\t\tPC\tBP\tSP\tSTACK\n");
    fprintf(ofp, "Initial Values\t\t\t%d\t%d\t%d\n", pCount, bPoint, sPoint);
}

void printListReverse(struct instruction *node);
void printListReverse(struct instruction *node)
{
    while(node->next != NULL)
    {
        node = node->next;
    }
    while(node->previous != NULL)
    {
        printf("%d\n", node->line);
        node = node->previous;
    }
}

void printInstructions(struct instruction *node);
void printInstructions(struct instruction *node)
{
    while(node->line != pCount)
    {
        node = node->next;
    }
    fprintf(ofp, "%d\t%s", node->line, node->opString);
    fprintf(ofp, "\t%d\t%d\t", node->L, node->M);
}

void printRegisters();
void printRegisters()
{
    fprintf(ofp, "%d\t%d\t%d\t", pCount, bPoint, sPoint);
    printStack();
    fprintf(ofp, "\n");

}

int base(int level)
{
    int base1 = bPoint;
    while(level > 0)
    {
        base1 = STACK[base1 + 1];
        level--;

    }

    return base1;
}

void execute(struct instruction *node);
void execute(struct instruction *node)
{

    while(node->line != pCount)
    {
        node = node->next;
    }

    printInstructions(node);
    pCount++;

    switch(node->OP)
    {
        case 1:
            //LIT 0 M
            sPoint = sPoint + 1;
            STACK[sPoint] = node->M;
            break;

        case 2:
            //OPR 0 M
            ALUOp(node);
            break;

        case 3:
            //LOD L M
            sPoint = sPoint + 1;
            STACK[sPoint] = STACK[base(node->L) + node->M];
            break;

        case 4:
            //STO L M
            STACK[base(node->L) + node->M] = STACK[sPoint];
            sPoint = sPoint - 1;
            break;

        case 5:
            //CAL L M
            STACK[sPoint+1] = 0;
            STACK[sPoint+2] = base(node->L);
            STACK[sPoint+3] = bPoint;
            STACK[sPoint+4] = pCount;
            bPoint = sPoint +1;
            pCount = node->M;
            LEXI[LEXILvl] = sPoint;
            LEXILvl = LEXILvl + 1;
            break;

        case 6:
            //INC 0 M
            sPoint = sPoint + node->M;
            break;

        case 7:
            //JMP 0 M
            pCount = node->M;
            break;

        case 8:
            //JPC
            if(STACK[sPoint] == 0)
            {
                pCount = node->M;
            }
            sPoint = sPoint - 1;
            break;

        case 9:
            //SIO 0 1
            printf("\n%d\n", STACK[sPoint]);
            sPoint = sPoint - 1;
           break;
        case 10:
            //SIO 0 2
            sPoint = sPoint + 1;
            printf("Enter integer to push on the stack: ");
            scanf("%d", &STACK[sPoint]);
            printf("\n");
            break;

        case 11:
            //SIO 0 3
            HALT = 1;
            pCount = 0;
            bPoint = 0;
            sPoint = 0;
            break;
    }

    printRegisters();

}

int main()
{
    struct instruction *root = NULL;
    int code = 0;
    int linenum = 0;

    ifp = fopen("mcode.txt", "r");
    ofp = fopen("stacktrace.txt", "w");

    while(fscanf(ifp, "%d", &code) != EOF)
    {
        readFile(&root, code, linenum);
        linenum++;
    }

    printList(root);

   while(HALT == 0)
   {
       execute(root);
   }


    fclose(ifp);
    fclose(ofp);

    return 0;
}
