/*
Author: Austin Lowe & Kyle Scorcia
Date: 10/12/2015
Assignment 2: PL/0 Scanner
COP3402 Systems Software
Matthew B. Gerber (Adjunct)
University of Central Florida
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_SYMBOL_TABLE_SIZE 100
#define MAX_IDENTIFIER_LENGTH 11
#define MAX_NUMBER_LENGTH 5

//A struct to store the symbol information
typedef struct{
    int kind;           // const = 1, var = 2, procedure = 3
    char name[12];      // reg
    int val;            // number (ASCII value)
    int level;          // L level
    int addr;           // M level
} symbol;


//A node that represents a single word or symbol in the program code
typedef struct node{
    //A variable, number or symbol in the code
    char *word;
    //A pointer to the next word in the code
    struct node *next;
} node;

//Create a enum for the token types
typedef enum{
    nulsym = 1, identsym, numbersym, plussym, minussym,
    multsym,  slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
    gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
    periodsym, becomessym, beginsym, endsym, ifsym, thensym,
    whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
    readsym , elsesym
} token_type;


//Function declarations
node *BeginsWithLetter(char firstLetter, FILE *inputFile, node* tail, FILE *outputFile);
node *BeginsWithNumber(char firstDigit, FILE *inputFile, node* tail, FILE *outputFile, int lineNum);
node *BeginsWithSymbol(char firstSymbol, FILE *inputFile, node* tail, FILE *outputFile, int lineNum);
void findLexeme(FILE *outFP, char *text, FILE *tokenList, symbol *table, int *numSymbs, int lineNum);
node *createNode();


//Begin main
int main(void){

    int c, buffer;
    char letter;
    int i = 0;
    int numSymbols = 0;
    int lineNum = 1;

    //Begin a linked list to store each word or symbol in a node
    node *head, *tail;
    head = tail = createNode();

    //Declare and initialize the symbol table
    symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];
    for (i = 0; i < MAX_SYMBOL_TABLE_SIZE; i++){
        symbol_table[i].kind = 0;
        strcpy(symbol_table[i].name,  "");
        symbol_table[i].val = 0;
        symbol_table[i].level = 0;
        symbol_table[i].addr = 0;
    }

    //File variables
    FILE *ifp, *lfp, *cifp, *tlfp;

    ifp = fopen("input.txt", "rb");
    lfp = fopen("lexemetable.txt", "w+");
    cifp = fopen("cleaninput.txt", "w+");
    tlfp = fopen("tokenlist.txt", "w+");

    // exit program if file not found
    if (ifp == NULL){
        printf("\"input.txt\" file not found\n");
        exit(1);
    }
    fprintf(lfp, "lexeme\t\ttoken type\n");

    // get the first character from the input file
    c = fgetc(ifp);

	// Read the first character of each new word.  Creates a node for each
    // symbol, word, or number in the code (excluding comments).  Prints out the
    // code to output, removing comments
	while(c  != EOF){
        // if the first character is a letter, handle as a letter
        if (isalpha(c)){
            letter = c;
            tail = BeginsWithLetter(c, ifp, tail, cifp);
            c = fgetc(ifp);
        }

        // handle differently if the first character is a number
        else if (isdigit(c)){
            tail = BeginsWithNumber(c, ifp, tail, cifp, lineNum);
            c = fgetc(ifp);
        }

        // and if the first character is a symbol
        else if (ispunct(c)){
            letter = c;
            tail = BeginsWithSymbol(c, ifp, tail, cifp, lineNum);
            c = fgetc(ifp);
        }

        // if it is none of these, print it out to maintain white space
        else{
            fprintf(cifp,"%c",c);
            c = fgetc(ifp);
        }

        // Print the lexime list to the lexime output file
        // Go through each "word" in the linked list code
        for(; head->next != NULL; head = head->next){
            fprintf(lfp, "%s\t\t", head->word);
            findLexeme(lfp, head->word, tlfp, symbol_table, &numSymbols, lineNum);
        }

        if(c == '\n'){
            lineNum += 1;
        }
    }


    //Close all
    fclose(ifp);
    fclose(lfp);
    fclose(cifp);
    fclose(tlfp);

}// end function main

// creates a node if the first character is a letter, so it is either a variable
// or a reserved word.
node *BeginsWithLetter(char firstLetter, FILE *inputFile, node *tail, FILE *outputFile){
    int wordLen = 11;
    int nextLetter;
    int letterPos = 1;

    // create space for a word of size 11
    char *word = calloc(wordLen + 1,sizeof(char));

    // put the first letter to begin building the word
    strcpy(word, "");
    word[0] = firstLetter;

    // get the next character
    nextLetter = fgetc(inputFile);

    // as long as the next character is a letter or number, keep adding it to
    // the word
    while(isalpha(nextLetter) || isdigit(nextLetter)){
        // reallocate space if the word is too big
        if (letterPos >= wordLen){
            wordLen *= 2;
            word = realloc(word, wordLen + 1);
        }
        word[letterPos] = nextLetter;
        letterPos++;
        nextLetter = fgetc(inputFile);
    }// ends when the next character is not a letter or number

    // copy the word to the linkedlist node
    tail->word = malloc(strlen(word) + 1);
    strcpy(tail->word,  word);
    tail->next = createNode();

    // free up the space occupied by the word
    free(word);

    // go back in the file so the character can be read again in the main program
    if (nextLetter != EOF)
        fseek(inputFile, -1, SEEK_CUR);

    // print the word to the file
    fprintf( outputFile, "%s", tail->word );

    // return pointer to the newly created node
    return tail->next;

}// end function BeginsWithLetter


// creates a node if the first character is a number
node *BeginsWithNumber(char firstDigit, FILE *inputFile, node *tail, FILE *outputFile, int lineNum){
    int numDigits = 5;
    int digitPos = 1;
    int nextDigit;


    // allocate space for the number and begin copying the first digit
    char *word = calloc(numDigits + 1, sizeof(char));
    strcpy( word, "" );
    word[0] = firstDigit;

    // get the next character from the file
    nextDigit = fgetc(inputFile);

    // as long as the next character is a number, keep adding it to the string
    while( isdigit(nextDigit) )
    {
        // reallocate space if necessary
        if ( digitPos >= numDigits )
        {
            numDigits *= 2;
            word = realloc(word, numDigits + 1);
        }
        // add new digits to the string
        word[digitPos] = nextDigit;
        digitPos++;
        nextDigit = fgetc(inputFile);

    }// end if nextDigit is not a digit

    // if the next character is a letter, print error message and exit program
    if (isalpha(nextDigit)){
        printf("Error on line %d", lineNum);
        exit(1);
    }

    // copy string to the linked list
    tail->word = malloc(strlen(word) + 1);
    strcpy(tail->word,  word);
    tail->next = createNode();

    // go back a character so the main file can read the non-digit character
    if ( nextDigit != EOF )
        fseek(inputFile, -1, SEEK_CUR);

    // free memory for the word
    free(word);

    // print number to output
    fprintf(outputFile, "%s", tail->word );

    return tail->next;

}// end function BeginsWithNumber


//creates a node if the first character is a symbol
node *BeginsWithSymbol( char firstSymbol, FILE *inputFile, node* tail, FILE *outputFile, int lineNum){
    int maxNumSymbols = 2;

    // allocate space and begin building string for the symbol
    char *symbol = calloc(maxNumSymbols + 1, sizeof(char) );

    strcpy( symbol, "" );
    symbol[0] = firstSymbol;

    // check for the first character, and determine string accordingly
    switch (firstSymbol){
        case '/':{
            char nextChar;

            nextChar = fgetc(inputFile);

            // if the string is /*, begin comments.  Remove all characters until
            // the close comments */ string is found. Do not output any
            // characters between comments.
            if (nextChar == '*'){
                nextChar = fgetc(inputFile);
                // repeat until the / charcter is found, but only checks right
                // after the * is found
                do{
                    // repeat until the * is found
                    while (nextChar != '*'){
                        // exit program with error if no closing comments found
                        if (nextChar == EOF){
                             printf("Error on line %d", lineNum);
                             exit(1);
                         }
                        nextChar = fgetc(inputFile);
                    }
                    nextChar = fgetc(inputFile);
                } while(nextChar != '/');

                // dont add new node to the linked list
                return tail;
            }

            // if the 2nd character is not comments go back so main program
            // can read it
            else{
                if(nextChar != EOF)
                    fseek(inputFile, -1, SEEK_CUR);
            }
            break;
        }// end case for /

        case '<' :{
            char nextChar;

            int digitPos = 1;

            nextChar = fgetc(inputFile);

            // if the next character is = or >, add to string
            if ( nextChar == '=' || nextChar == '>' ){
                symbol[digitPos] = nextChar;
            }
            else{
                if ( nextChar != EOF )
                    fseek(inputFile, -1, SEEK_CUR);
            }
            break;
        }// end case for <

        case '>' :{
            char nextChar;

            int digitPos = 1;

            nextChar = fgetc(inputFile);

            // add = to string to make >=
            if ( nextChar == '='){
                symbol[digitPos] = nextChar;
            }
            else{
                if ( nextChar != EOF )
                    fseek(inputFile, -1, SEEK_CUR);
            }
            break;
        }// end case for >

        case ':' :{
            char nextChar;
            int digitPos = 1;
            nextChar = fgetc(inputFile);
            // if :=
            if ( nextChar == '='){
                symbol[digitPos] = nextChar;
            }
            // if not :=, then is invalid symbol
            else{
                printf("Error on line %d", lineNum);
                exit(1);
            }
            break;
        }// end case for :

        // if any other approved symbol, string is ok, and nothing else needs
        // to be done
        case '+' :
        case '-' :
        case '*' :
        case '(' :
        case ')' :
        case '=' :
        case ',' :
        case '.' :
        case ';' :
            break;
        // if any other symbol, it is invalid.  Exit program.
        default :
            printf("Error on line %d", lineNum);
            exit(1);

    }// end switch case statement

    // add the string for the symbol to the linked list
    tail->word = malloc(strlen(symbol) + 1);
    strcpy( tail->word,  symbol);
    tail->next = createNode();

    // free space used for the symbol
    free(symbol);

    // print symbol to output file
    fprintf(outputFile, "%s", tail->word);

    return tail->next;

}// end function BeginsWithSymbol


// takes a string node and converts it to the appropriate token value
// creates a symbol in the symbol table if needed
void findLexeme(FILE *outFP, char *text, FILE *tokenList, symbol *table, int *numSymbs, int lineNum){
    int index;

    // if the first character is a letter
    if (isalpha(text[0]))
    {
        // check to make sure identifier name is not too long.  Error and exit
        // if it is too long
        if (strlen(text) > MAX_IDENTIFIER_LENGTH){
            printf("Error on line %d", lineNum);
            exit(1);
        }

        // print the appropriate token value to the output file and the lexeme
        // list file for all reserved words
        if (strcmp(text, "odd") == 0){
            fprintf(outFP, "%d\n", 8);
            fprintf(tokenList, "%d ", 8);
        }
        else if (strcmp( text, "begin") == 0){
            fprintf(outFP, "%d\n", 21);
            fprintf(tokenList, "%d ", 21);
        }
        else if(strcmp(text, "end") == 0){
            fprintf(outFP, "%d\n", 22);
            fprintf(tokenList, "%d ", 22);
        }
        else if(strcmp(text, "if") == 0){
            fprintf(outFP, "%d\n", 23);
            fprintf(tokenList, "%d ", 23);
        }
        else if(strcmp(text, "then") == 0){
            fprintf(outFP, "%d\n", 24);
            fprintf(tokenList, "%d ", 24);
        }
        else if(strcmp(text, "while") == 0){
            fprintf(outFP, "%d\n", 25);
            fprintf(tokenList, "%d ", 25);
        }
        else if(strcmp( text, "do") == 0){
            fprintf(outFP, "%d\n", 26);
            fprintf(tokenList, "%d ", 26);
        }
        else if(strcmp( text, "call") == 0){
            fprintf(outFP, "%d\n", 27);
            fprintf(tokenList, "%d ", 27);
        }
        else if(strcmp( text, "const") == 0){
            fprintf(outFP, "%d\n", 28);
            fprintf(tokenList, "%d ", 28);
        }
        else if(strcmp( text, "var") == 0){
            fprintf(outFP, "%d\n", 29);
            fprintf(tokenList, "%d ", 29);
        }
        else if(strcmp( text, "procedure") == 0){
            fprintf(outFP, "%d\n", 30);
            fprintf(tokenList, "%d ", 30);
        }
        else if(strcmp( text, "write") == 0){
            fprintf(outFP, "%d\n", 31);
            fprintf(tokenList, "%d ", 31);
        }
        else if(strcmp( text, "read") == 0){
            fprintf (outFP, "%d\n", 32);
            fprintf(tokenList, "%d ", 32);
        }
        else if(strcmp( text, "else") == 0){
            fprintf(outFP, "%d\n", 33);
            fprintf(tokenList, "%d ", 33);
        }

        else{
            // if it is not a reserved word, it is an identifier
            // print the appropriate token "2" and add it to the symbol table
            fprintf (outFP, "2\n");
            fprintf(tokenList, "%d ", 2);
            fprintf(tokenList, "%s ", text);
        }
    }// end if first character is letter

    // if the first character is a number
    else if (isdigit(text[0])){
        // error and exit if the number has too many digits
        if (strlen(text) > MAX_NUMBER_LENGTH){
            printf("Error on line %d", lineNum);
            exit(1);
        }
        // print the appropriate token "3" and add it to the symbol table
        fprintf(outFP, "3\n");
        fprintf (tokenList, "3 %s ", text);
    }// end if first character is number

    // if the first character is punctuation.  Already tested for invalid
    // characters, so only need to find which one.
    else if (ispunct(text[0])){
        // look at first character
        switch(text[0]){
            case '+' :
                fprintf(outFP, "%d\n", 4);
                fprintf(tokenList, "%d ", 4);
                break;
            case '-' :
                fprintf(outFP, "%d\n", 5);
                fprintf(tokenList, "%d ", 5);
                break;
            case '*' :
                fprintf(outFP, "%d\n", 6);
                fprintf(tokenList, "%d ", 6);
                break;
            case '/' :
                fprintf(outFP, "%d\n", 7);
                fprintf(tokenList, "%d ", 7);
                break;
            case '(' :
                fprintf(outFP, "%d\n", 15);
                fprintf(tokenList, "%d ", 15);
                break;
            case ')' :
                fprintf(outFP, "%d\n", 16);
                fprintf(tokenList, "%d ", 16);
                break;
            case '=' :
                fprintf(outFP, "%d\n", 9);
                fprintf(tokenList, "%d ", 9);
                break;
            case ',' :
                fprintf(outFP, "%d\n", 17);
                fprintf(tokenList, "%d ", 17);
                break;
            case '.' :
                fprintf(outFP, "%d\n", 19);
                fprintf(tokenList, "%d ", 19);
                break;

            // <, <>, <=, <>, and <=
            case '<' :
                if (strlen(text) > 1){
                    if (strcmp(text, "<>") == 0){
                        fprintf(outFP, "%d\n", 10);
                        fprintf(tokenList, "%d ", 10);
                    }
                    else if (strcmp(text, "<=") == 0){
                        fprintf(outFP, "%d\n", 12);
                        fprintf(tokenList, "%d ", 12);
                    }
                }
                else{
                    fprintf(outFP, "%d\n", 11);
                    fprintf(tokenList, "%d ", 11);
                }
                break;
            case '>' :      // > and >=
                if (strlen(text) > 1){
                    if (strcmp(text, ">=") == 0){
                        fprintf(outFP, "%d\n", 14);
                        fprintf(tokenList, "%d ", 14);
                    }
                }
                else{
                    fprintf(outFP, "%d\n", 13);
                    fprintf(tokenList, "%d ", 13);
                }
                break;
            case ';' :
                fprintf(outFP, "%d\n", 18);
                fprintf(tokenList, "%d ", 18);
                break;
            case ':' :
                fprintf(outFP, "%d\n", 20);
                fprintf(tokenList, "%d ", 20);
                break;

        }// end switch statement

    }// end if first character is symbol
}// end function FindLexeme

//Creates a new node.  Will store strings for each piece of code representing
//identifiers, numbers, and symbols, but for now just creating the node.
node *createNode(){
    node *ptr = malloc(sizeof(node));
    ptr->next = NULL;
    return ptr;
}// end function createNode
