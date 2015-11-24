#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SYMBOL_TABLE_SIZE 100
#define MAX_IDENTIFIER_LENGTH 11
#define MAX_NUMBER_LENGTH 5
#define CODE_SIZE 2000

// a struct to store the symbol information
typedef struct
{
    int kind;           // const = 1, var = 2, procedure = 3
    char name[12];      // name of identifier
    int val;            // value for constants / numbers
    int level;          // L level
    int addr;           // M address
} symbol;


// create a enum for the token types
typedef enum
{
    nulsym = 1, identSym, numberSym, plusSym, minusSym,
    multSym,  slashSym, oddSym, equalSym, neqSym, lesSym, leqSym,
    gtrSym, geqSym, lparentSym, rparentSym, commaSym, semicolonSym,
    periodSym, becomesSym, beginSym, endSym, ifSym, thenSym,
    whileSym, doSym, callSym, constSym, varSym, procSym, writeSym,
    readSym , elseSym
} token_type;


typedef enum
{
    LIT = 1, RTN, LOD, STO, CAL, INC, JMP, JPC, SIO1, SIO2, SIO3,
    NEG, ADD, SUB, MUL, DIV, ODD, MOD, EQL, NEQ, LSS, LEQ, GTR, GEQ
} op_code;


// kind of identifier
typedef enum
{
    constant = 1,
    variable,
    procedure
} symbol_kind;


// a node that represents a single token from the code
typedef struct node
{
    int token;
    struct node *next;
} node;

// represents a line of code
typedef struct
{
    int op;
    int r;
    int l;
    int m;
} instruction;


// Global variables
int currentToken;
int curReg;
symbol symbolList[50];
symbol symbolTable[100];
int stIndex;
int codeLine;
instruction code[CODE_SIZE];
int printSuccess;
int level;
int numVars = 0;
int stSize;
int pointer;


// function declarations
int program( node *currentNode);
void block( node *currentNode );
int const_declaration( node *currentNode );
int  var_declaration( node *currentNode );
int procedure_declaration( node *currentNode );
void statement( node *currentNode );
void condition( node *currentNode );
int  rel_op( );
void expression( node *currentNode );
void term( node *currentNode );
void factor( node *currentNode );
void getNextToken( node *currentNode );
void error( int errorVal );
void addNumberSymbol(FILE *inputFile, int token, node *head, node *tail);
void addtoSymbolTable( int symbolKind, char name[12], int Value);
int findToken( int token);
int findSymbol(char name[12]);

void insertNode( node **head, int token );
node *getTokenList();

int getSymbolList( symbol *symbolList );
void emit( int op, int r, int l, int m );
void printCode();
void printNodes( node **head );

// begin function main
int main()
{
    //FILE *outFP = fopen("mcode.txt", "w");
    int i;
    curReg = -1;
    codeLine = 0;
    printSuccess = 0;
    level = 0;
    stIndex = 0;
    pointer = 1;

    // gets the lexeme list into the global variable
    node *currentNode, *head;
    head = getTokenList();
    currentNode = head;

    // put the symbol table into global variable symbol table

    // call program
    printSuccess = program(currentNode);


    if(printSuccess)
        printCode();

}// end function main


// checks to make sure this block of code fits the definition for a program
// as defined by the EBNF grammar
int program( node *currentNode )
{
    // get the first token
    getNextToken( currentNode );
    level = -1;

    block( currentNode );

    // if the program does not end with a period, error
    if (currentToken != periodSym )
    {
        error(6);
    }
    else
    {
        emit( SIO3, 0, 0, 3 );  // SIO R 0 3 - halt program
        return 1;
    }
}// end function program


// checks to make sure this block of code fits the definition for a block
// as defined by EBNF
void block( node *currentNode )
{
    int space, numProcs = 0, numConsts = 0;
    int jmpAddress, procAddress;
    int i;

    level++;
    space = 4;

    jmpAddress = codeLine;
    emit( JMP, 0, 0, 0 );

    // calls the appropriate function based on current token
    //if (currentToken == constSym)
        //numConsts = const_declaration( currentNode );


    //if (currentToken == varSym)
        //numVars = var_declaration( currentNode );


    space += numVars;
    printf("Space = %d\n", space);

    if ( currentToken == procSym )
        numProcs = procedure_declaration( currentNode );

    code[jmpAddress].m = codeLine;

    emit( INC, 0, 0 , space );

    statement(currentNode);


    // uncomment next section for debugging purposes
    /*
    for ( i = 0; i <= stIndex; i++ )
        printf("%s\t%d\t%d\t%d\t%d\n", symbolTable[i].name,
                       symbolTable[i].kind, symbolTable[i].val, symbolTable[i].level, symbolTable[i].addr);
    */

    stIndex = stIndex - (numVars + numProcs + numConsts);
    emit( RTN, 0, 0, 0 );

    level--;

}// end function block


// checks to make sure this block of code fits the definition for the constant
// declaration as defined by EBNF
int const_declaration( node *currentNode )
{
    int symListIndex, constIndex, constValue;
    int constCount = 0;

    // repeat getting new constants as long as there are more
    // (indicated by a comma)
    do{

        getNextToken( currentNode );

        if ( currentToken != identSym ){
            error(4);
        }


        symListIndex = stIndex+1;
        addtoSymbolTable(constant, symListIndex, symbolTable);
        constCount++;
        getNextToken( currentNode );

        if ( currentToken != equalSym )
        {
            if ( currentToken == becomesSym )
                error(1);
            else
                error(3);
        }

        getNextToken(currentNode);

        if(currentToken != numberSym){
            error(2);
        }

        //add next node (a number) to the value of the current constant in the symbolTable

        getNextToken(currentNode);
    } while ( currentToken == commaSym );

    // if there are no more const, then we should have a semicolon
    if ( currentToken != semicolonSym )
        error(5);

    getNextToken( currentNode );

    return constCount;

}// end function const_declaration



// checks to make sure this block of code fits the definition for the variable
// declaration as defined by EBNF, and returns # of variables declared
int var_declaration( node *currentNode )
{
    int symListIndex;
    int varCount = 0;

    // repeat getting new constants as long as there are more
    // (indicated by a comma)
    do{

        getNextToken( currentNode );

        if ( currentToken != identSym )
            error(4);

        getNextToken(currentNode);

        // get the symbol table index for the variable and initialize the
        // appropriate symbol table values
        symListIndex = stIndex + 1;
        addtoSymbolTable(variable, symListIndex, symbolTable);
        symbolTable[stIndex].addr = 4 + varCount; // add 4 to account for other items in activation record  (space from base pointer)

        // keep track of the number of variables declared
        varCount++;
    } while ( currentToken == commaSym );

    // if there are no more const, then we should have a semicolon
    if ( currentToken != semicolonSym )
        error(5);

    getNextToken( currentNode );
    //emit( INC, 0 , 0, varCount );     // INC

    return varCount;
    printf("Leaving variables\n");
}// end function var_declaration



// checks to make sure this block of code fits the definition for the procedure
// declaration as defined by the EBNF
int procedure_declaration( node *currentNode )
{

    int symListIndex, procCount = 0;

    do{
        procCount++;
        getNextToken( currentNode );

        if ( currentToken != identSym )
            error(4);

        // get the symbol table index for the variable and initialize the
        // appropriate symbol table values
        symListIndex = stIndex + 1;
        addtoSymbolTable( procedure, symListIndex, symbolTable );
        symbolTable[stIndex].level = level;
        symbolTable[stIndex].addr = codeLine;

        getNextToken( currentNode );

        // if there are no more const, then we should have a semicolon
        if ( currentToken != semicolonSym )
            error(5);

        getNextToken( currentNode );
        block( currentNode );
        // if there are no more const, then we should have a semicolon
        if ( currentToken != semicolonSym )
            error(5);
        getNextToken( currentNode );

    } while ( currentToken == procSym );

    return procCount;

}// end function procedure_declaration



// checks to make sure this block of code fits the definition for the statement
// as defined by EBNF
void statement(node *currentNode)
{
    int i, ctemp, cx1, cx2;
    int index;
    printf("in statement, token = %d\n", currentToken);
    // ident ":=" expression
    if (currentToken == identSym )
    {
        getNextToken(currentNode);

        printf("index = %d\n", currentToken );
        i = currentToken;

        if ( i == -1 )
            error(7);

        if (symbolTable[i].kind != variable )
        {
            error(8);
        }

        getNextToken( currentNode );

        if ( currentToken != becomesSym )
            error(9);

        getNextToken( currentNode );

        expression( currentNode );

        emit( STO, curReg, level - symbolTable[i].level, symbolTable[i].addr );    // STO = 4
        curReg--;

    } // end ident if

    // "call" ident
    else if ( currentToken == callSym )
    {
        getNextToken( currentNode );

        if ( currentToken != identSym )
            error(23);

        getNextToken( currentNode );

        i = findToken( currentToken );

        printf("At location %d\n", i);

        if ( i == 0 )
            error(7);

        if ( symbolTable[i].kind != procedure )
            error(24);

        // printf( "CAL on index %d for %s is at location %d\n", i, symbolTable[i].name, symbolTable[i].addr);
        emit ( CAL, 0, level - symbolTable[i].level, symbolTable[i].addr ); // CAL = 5

        getNextToken( currentNode );

    }


    // "begin" statement { ";" statement } "end"
    else if ( currentToken == beginSym )
    {
        getNextToken( currentNode );

        statement( currentNode );

        getNextToken(currentNode);

        while ( currentToken == semicolonSym )
        {
            getNextToken( currentNode );
            statement( currentNode );
        }

        if ( currentToken != endSym )
            error(11);

        getNextToken( currentNode );
    }// end "begin" if


    // "if" condition "then" statement ["else" statement]
    else if ( currentToken == ifSym )
    {
        getNextToken( currentNode );

        condition( currentNode );

        if ( currentToken != thenSym )
            error(10);

        getNextToken( currentNode );

        ctemp = codeLine;
        emit( JPC, curReg, 0, 0 );    // JPC
        curReg--;

        statement( currentNode );

        if ( currentToken == elseSym )
        {
            getNextToken( currentNode );

            cx2 = codeLine;
            emit (JMP, 0, 0, 0 );

            code[ctemp].m = codeLine;

            statement( currentNode );
            code[cx2].m = codeLine;
        }
        else
        {
            code[ctemp].m = codeLine;
        }

    }// end "if" if

    // "while" condition "do" statement
    else if ( currentToken == whileSym )
    {
        cx1 = codeLine;

        getNextToken( currentNode );

        condition( currentNode );

        cx2 = codeLine;

        emit( JPC, curReg, 0, 0 );    // JPC

        if ( currentToken != doSym )
            error(12);          // then expected

        getNextToken( currentNode );

        statement( currentNode );

        emit( JMP, 0, 0, cx1 );    // JMP

        code[cx2].m = codeLine;

    }// end "while" if

    // "read" ident
    else if ( currentToken == readSym )
    {
        getNextToken( currentNode );

        if ( currentToken != identSym )
        {
            error(18);
        }

        getNextToken(currentNode);

        i = currentToken;
        index = findToken(i);
        printf("At location %d\n", index);

        if ( symbolTable[index].kind != variable )
        {
            error(11);
        }

        // read in user input and store it in variable
        curReg++;
        emit( SIO2, curReg, 0, 2 );   // SIO R 0 2 - read
        emit( STO, curReg, level - symbolTable[index].level, symbolTable[index].addr );    // STO
        curReg--;

        getNextToken( currentNode );

    }// end "read" if

    // "write"  ident
    else if ( currentToken == writeSym )
    {
        getNextToken( currentNode );

        if ( currentToken != identSym )
        {
            error(18);
        }

        getNextToken(currentNode);
        i = currentToken;
        index = findToken(i);
        printf("At location %d\n", index);

        if ( symbolTable[index].kind != variable )
        {
            error(11);
        }

        // write variable to screen
        curReg++;
        emit( LOD, curReg, level - symbolTable[index].level, symbolTable[index].addr );    // LOD
        emit( SIO1, curReg, 0, 1 );    // SIO R 0 1 - print
        curReg--;

        getNextToken( currentNode );

    }// end "write" if

    // empty string do nothing


}// end function statement


// checks to make sure this block of code fits the definition for the condtion
// as defined by the EBNF
void condition( node *currentNode )
{
    int relOpCode;

    // "odd" expression
    if ( currentToken == oddSym )
    {
        getNextToken( currentNode );

        expression( currentNode );

        //emit( ODD, curReg, 0, 0 );   // ODD

    }

    // expression rel_op expression
    else
    {
        expression( currentNode );

        relOpCode = rel_op( );
        if ( !relOpCode )
        {
            error(13);
        }

        getNextToken( currentNode );

        expression( currentNode );

        //emit( relOpCode, curReg-1, curReg-1, curReg );  // EQL thru GEQ
        //curReg--;
    }
}// end function condition


// returns token value for relational op, returns 0 if it is not one
int rel_op (  )
{
    switch ( currentToken )
    {
        case equalSym:
            return EQL;  // 19
            break;
        case neqSym:
            return NEQ;  // 20
            break;
        case lesSym:
            return LSS;  // 21
            break;
        case leqSym:
            return LEQ;  // 22
            break;
        case gtrSym:
            return GTR;  // 23
            break;
        case geqSym:
            return GEQ;  // 24
            break;
        default:
            return 0;   // error
    }// end switch

}// end function rel_op


// checks to make sure this block of code fits the definition for expression
// as defined by the EBNF
void expression( node *currentNode )
{
    //int addop;

    // if there is a plus or minus symbol in front
    if ( currentToken == plusSym || currentToken == minusSym )
    {
        //addop = currentToken;

        getNextToken( currentNode );
        term ( currentNode );

        //emit( NEG, curReg, curReg, 0 );  // 12
    }
    else
    {
        term ( currentNode );
    }
    // keep looping as long as there is another plus or minus symbol
    while ( currentToken == plusSym || currentToken == minusSym )
    {
       int addop = currentToken;

        getNextToken( currentNode );
        term( currentNode );


        if ( addop == plusSym )
        {
            emit ( ADD, curReg-1, curReg-1, curReg );    // ADD = 13
            curReg--;
        }
        if ( addop == minusSym )
        {
            emit ( SUB, curReg-1, curReg-1, curReg );    // SUB = 14
            curReg--;
        }

        getNextToken(currentNode);

    }

}// end function expression


// checks to make sure this block of code fits the definition for the term
// as defined by the EBNF
void term( node *currentNode )
{
    int mulOp;

    factor( currentNode );

    // keep looping if there are more divide or multiplication symbols
    while ( currentToken == slashSym || currentToken == multSym )
    {
        //mulOp = currentToken;

        getNextToken( currentNode );
        factor( currentNode );

        /*
        if ( mulOp == multSym )     // Multiplication
        {
            emit( MUL, curReg-1, curReg-1, curReg ); // MUL = 15
            curReg--;
        }
        if ( mulOp == slashSym )    // Division
        {
            emit( DIV, curReg-1, curReg-1, curReg ); // DIV = 16
            curReg--;
        }
        */
    }
}// end function term


// checks to make sure this block of code fits the definition for factor
// as defined by the EBNF
void factor( node *currentNode )
{
    int index, i;
    int value;

    // identifier
    if ( currentToken == identSym )
    {
        getNextToken( currentNode );
        i = currentToken;
        //index = findToken(i);

        curReg++;

        if ( symbolTable[i].kind == variable )
        {
            emit( LOD, curReg, level - symbolTable[i].level, symbolTable[i].addr );    // LOD = 3
        }
        else if ( symbolTable[i].kind == constant )
        {
            emit( LIT, curReg, 0, symbolTable[i].val );     // LIT = 1
        }
        else
        {
            error(14);
        }

        getNextToken( currentNode );

    }
    // number
    else if ( currentToken == numberSym )
    {
        getNextToken( currentNode );
        i = currentToken;
        //index = findToken(i);

        value = atoi( symbolList[i].name );
        curReg++;
        emit( LIT, curReg, 0, value );                   // LIT = 1

        //getNextToken( currentNode );

    }

    // "(" expression ")"
    else if ( currentToken == lparentSym )
    {
        getNextToken( currentNode );
        expression( currentNode );

        if ( currentToken != rparentSym )
            error(15);

        getNextToken( currentNode );
    }
    else
        error(16);

}// end function factor



// gets the next token in the lexeme list from lexeme list linked list
void getNextToken( node *currentNode )
{
    currentToken = currentNode->token;

    if ( currentNode->next != NULL )
    {
        *currentNode = *currentNode->next;
    }

    printf("%d\n", currentToken);

    // used for debugging:  comment out when done

}// end function getNextToken



// exits program and prints appropriate error message
void error( int errorVal )
{
    printf("Error ");
    switch ( errorVal )
    {
        case 1:
            printf("1. Use = instead of :=.");
            break;
        case 2:
            printf("2. = must be followed by a number.");
            break;
        case 3:
            printf("3. Identifier must be followed by =.");
            break;
        case 4:
            printf("4. const, var, procedure must be followed by identifier.");
            break;
        case 5:
            printf("5. Semicolon or comma missing.");
            break;
        case 6:
            printf("6. Period expected.");
            break;
        case 7:
            printf("7. Undeclared identifier.");
            break;
        case 8:
            printf("8. Assignment to constant or procedure is not allowed.");
            break;
        case 9:
            printf("9. Assignment operator expected.");
            break;
        case 10:
            printf("10. then expected.");
            break;
        case 11:
            printf("11. Semicolon or end expected.");
            break;
        case 12:
            printf("12. do expected.");
            break;
        case 13:
            printf("13. Relational operator expected.");
            break;
        case 14:        // Not used this time
            printf("14. Expression must not contain a procedure identifier.");
            break;
        case 15:
            printf("15. Right parenthesis missing.");
            break;
        case 16:
            printf("16. An expression cannot begin with this symbol.");
            break;
        case 17:
            printf("17. This number is too large.");
            break;
        case 18:
            printf("18. Read or write must be followed by an identifier.");
            break;

            // renumber these ?!
        case 23:
            printf("23. Call must be followed by an identifier.");
            break;
        case 24:
            printf("24. Call of a constant or variable is meaningless." );
            break;
    }

    printf( "\n" );
    exit(1);
}// end function error


// reads the lexeme list from file into a linked list and returns the head
node *getTokenList()
{
    int buffer, i, j, symSize = 1, found = 0, currentLevel = 0, finished = 0;
    char word[12];
    int tableIndex;
    FILE *tlfp = fopen("tokenlist.txt", "r");
    FILE *stfp = fopen("symboltable.txt", "w");
    node *head, *tail;
    head = tail = NULL;
    int type = 0;

    // exit program if file not found
    if (tlfp == NULL) {
        printf("File for token list not found\n");
        exit(1);
    }

    while(fscanf(tlfp, "%d", &buffer) != EOF)
    {

        if(buffer == constSym || buffer == varSym || buffer == procSym)
        {
             addNumberSymbol(tlfp, buffer, head, tail);
        }

        else if(buffer == identSym)
        {
            insertNode(&head, buffer);
            fscanf(tlfp, "%s", word);
            tableIndex = findSymbol(word);
            if(tableIndex == -1)
            {
                error(7);
            }

            insertNode(&head, tableIndex);

        }
        else
        {
            insertNode(&head, buffer);
        }
    }



    fclose(tlfp);
    fclose(stfp);
    return head;
}// end function getTokenList

void addNumberSymbol(FILE *inputFile, int token, node *head, node *tail)
{
    int kind = token;
    int buffer;
    char word[12];
    int val = 0;
    int flag = 0;

    while(flag == 0)
    {
        fscanf(inputFile, "%d", &buffer);

        if(buffer == identSym)
        {
            fscanf(inputFile, "%s", word);

            fscanf(inputFile, "%d", &buffer);

            switch(buffer)
            {
                case commaSym:
                    insertSymbol(kind, word, val);
                    stIndex++;
                    val = 0;
                    break;
                case semicolonSym:
                    insertSymbol(kind, word, val);
                    flag = 1;
                    break;
                case equalSym:
                    fscanf(inputFile, "%d", &buffer);
                    if(buffer == numberSym)
                    {
                        fscanf(inputFile, "%d", &val);
                        insertSymbol(kind, word, val);
                        stIndex++;
                    }
                    break;
            }
            if(buffer == semicolonSym)
            {
                flag = 1;
            }
        }
    }

}

void insertSymbol(int kind, char name[12], int Value)
{

    if(kind == constSym)
    {
        return addtoSymbolTable(constant, name, Value);
    }
    if(kind == varSym)
    {
        numVars++;
        return addtoSymbolTable(variable, name, Value);
    }
    if(kind == procSym)
    {
        return addtoSymbolTable(procedure, name, Value);

    }

}
// insert a new node into the linked list
void insertNode( node **head, int token )
{
    struct node *end, *temp;

    temp = (struct node *)malloc(sizeof(struct node));
    temp->token = token;
    temp->next = NULL;

    if ( *head == NULL )
    {
        *head = temp;
    }
    else
    {
        end = *head;

        while(end->next != NULL)
        {
            end = end->next;
        }

        end->next = temp;
    }

    printf("%d\n", token);

}// end function insertNode


// puts the symbol table into the symbol table array, and returns the length
// of the symbol table
int getSymbolList( symbol *symbolList )
{
    char buffer[MAX_IDENTIFIER_LENGTH + 1];
    int i = 1;

    FILE *sTfp;

    // create a pointer for the input file
    sTfp = fopen("symboltable.txt", "rb");

    // exit program if file not found
    if (sTfp == NULL) {
        printf("File with symbol table info not found\n");
        exit(1);
    }

    // keep scanning until reaching the end of the file
    while ( fscanf(sTfp, "%s", buffer) != EOF)
    {
        strcpy(symbolList[i].name, buffer);
        i++;
    }

    fclose( sTfp );
    return i-1;

}// end function getSymbolTable



// print the appropriate code to the code file
void emit( int op, int r, int l, int m )
{
    code[codeLine].op = op;
    code[codeLine].r = r;
    code[codeLine].l = l;
    code[codeLine].m = m;

   // printf( "In Emit: op = %d r = %d l = %d m = %d\n", op, r, l, m );

    codeLine++;

}// end function emit


// prints the generated code to the code.txt output file
void printCode()
{
    int i;

    FILE *ofp;
    ofp = fopen("code.txt", "w");

    for ( i = 0; i < codeLine; i++ )
    {
        fprintf( ofp, "%d %d %d %d\n", code[i].op, code[i].r, code[i].l,
                code[i].m );
    }
    fclose(ofp);
}// end function printcode


// adds the symbol from the symbol table list (from Scanner) to the symbol table
void addtoSymbolTable( int symbolKind, char name[12], int Value)
{
        strcpy( symbolTable[stIndex].name, name);
        symbolTable[stIndex].val = Value;
        symbolTable[stIndex].level = level;
        symbolTable[stIndex].kind = symbolKind;

        return stIndex;
}// end function addtoSymbolTable

int findSymbol(char name[12])
{
    int i;
    for(i = stIndex; i >= 0; i--)
    {
        if(strcmp(symbolTable[i].name, name) == 0)
        {
            if(symbolTable[i].level == level)
            {
                return i;
            }

        }

    }

    return -1;

}

// finds the location of the token in the symbol table
int findToken(int token)
{
    int location;
    for ( location = stIndex; location > 0; location-- ){
        if ( strcmp( symbolTable[location].name, symbolList[token].name ) == 0 ){
            if(symbolTable[location].level == level)
                return location;
        }
    }
    return location;
}// end function findToken
