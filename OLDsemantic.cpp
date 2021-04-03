//#include "globals.h"
#include "semantic.h"

static int indentno = 0;
static int siblingFlag = 0;
//int siblingCount = 0;
static int firstflag = 0;
static int firstLeftglag = 0; 
#define INDENT indentno+=1
#define UNINDENT indentno-=1

static void printSpaces(void)
{
    int i;
    for(i=1; i<indentno; i++)
    {
        printf(".   ");
    }
}

void semanticAnalysis(TreeNode* syntaxTree, SymbolTable symbolTable)
{
    //Taken and modified from Textbook p.510
    int siblingCount = 0;
    INDENT;
    if(!firstflag)
    {
        symbolTable.Enter("Global Scope");
    }
    while(syntaxTree != NULL)
    {
        printSpaces();
        if(syntaxTree->isC0)
        { /*printf("Child: 0  ");*/ }
        else if(syntaxTree->isC1)
        { /*printf("Child: 1  ");*/ }
        else if(syntaxTree->isC2)
        { /*printf("Child: 2  ");*/ }
        else if(!syntaxTree->isC0 && !syntaxTree->isC1 && !syntaxTree->isC2 && siblingCount > 0)
        {
            /*printf("Sibling: %i  ",siblingCount);*/
        }
        //DECLARATIONS
        if(syntaxTree->nodekind==DeclK)
        {
            switch(syntaxTree->subkind.decl)
            {
                case VarK:
                    symbolTable.insert(syntaxTree->attr.name, *syntaxTree);
                    if (syntaxTree->isArray)
                    {
                        //printf("Var: %s is array of type ",syntaxTree->attr.name);
                        switch(syntaxTree->declType)
                        {
                            case DeclVoid:
                                symbolTable.insert(syntaxTree->attr.name, *syntaxTree);
                                break;
                            case DeclInteger:
                                //printf("int");
                                break;
                            case DeclBoolean:
                                //printf("bool");
                                break;
                            case DeclChar:
                                //printf("char");
                                break;
                            case DeclCharInt:
                                //printf("chint");
                                break;
                            case DeclEqual:
                                //printf("eq");
                                break;
                            case DeclUndefinedType:
                                //printf("UndefinedType");
                                break;
                            default:
                                printf("ERROR: No syntaxTree->declType\n");
                                break;
                        }
                        //printf(" [line: %i]\n",syntaxTree->lineno); 
                    }
                    else
                    {
                        printf("Var: %s of type ", syntaxTree->attr.name);
                        switch(syntaxTree->declType)
                        {
                            case DeclVoid:
                                printf("void");
                                break;
                            case DeclInteger:
                                printf("int");
                                break;
                            case DeclBoolean:
                                printf("bool");
                                break;
                            case DeclChar:
                                printf("char");
                                break;
                            case DeclCharInt:
                                printf("chint");
                                break;
                            case DeclEqual:
                                printf("eq");
                                break;
                            case DeclUndefinedType:
                                printf("UndefinedType");
                                break;
                            default:
                                printf("ERROR: No syntaxTree->declType\n");
                                break;
                        }
                        printf(" [line: %i]\n",syntaxTree->lineno);
                    }
                    break;
                case FuncK:
                    printf("Func: %s returns type ",syntaxTree->attr.name);
                        switch(syntaxTree->declType)
                        {
                            case DeclVoid:
                                printf("void");
                                break;
                            case DeclInteger:
                                printf("int");
                                break;
                            case DeclBoolean:
                                printf("bool");
                                break;
                            case DeclChar:
                                printf("char");
                                break;
                            case DeclCharInt:
                                printf("chint");
                                break;
                            case DeclEqual:
                                printf("eq");
                                break;
                            case DeclUndefinedType:
                                printf("UndefinedType");
                                break;
                            default:
                                printf("ERROR: No syntaxTree->declType\n");
                                break;
                        }
                        printf(" [line: %i]\n",syntaxTree->lineno);
                    break;
                case ParamK:
                    if (syntaxTree->isArray)
                    {
                        printf("Parm: %s is array of type ",syntaxTree->attr.name);
                        switch(syntaxTree->declType)
                        {
                            case DeclVoid:
                                printf("void");
                                break;
                            case DeclInteger:
                                printf("int");
                                break;
                            case DeclBoolean:
                                printf("bool");
                                break;
                            case DeclChar:
                                printf("char");
                                break;
                            case DeclCharInt:
                                printf("chint");
                                break;
                            case DeclEqual:
                                printf("eq");
                                break;
                            case DeclUndefinedType:
                                printf("UndefinedType");
                                break;
                            default:
                                printf("ERROR: No syntaxTree->declType\n");
                                break;
                        }
                        printf(" [line: %i]\n",syntaxTree->lineno); 
                    }
                    else
                    {
                        printf("Parm: %s of type ", syntaxTree->attr.name);
                        switch(syntaxTree->declType)
                        {
                            case DeclVoid:
                                printf("void");
                                break;
                            case DeclInteger:
                                printf("int");
                                break;
                            case DeclBoolean:
                                printf("bool");
                                break;
                            case DeclChar:
                                printf("char");
                                break;
                            case DeclCharInt:
                                printf("chint");
                                break;
                            case DeclEqual:
                                printf("eq");
                                break;
                            case DeclUndefinedType:
                                printf("UndefinedType");
                                break;
                            default:
                                printf("ERROR: No syntaxTree->declType\n");
                                break;
                        }
                        printf(" [line: %i]\n",syntaxTree->lineno);
                    }
                    break;
                default:
                    printf("ERROR: Unknown DeclK!! (printTree Function)\n");
            }
        }
        // STATEMENTS
        else if(syntaxTree->nodekind==StmtK)
        {
            switch(syntaxTree->subkind.stmt)
            {
                case NullK:
                    printf("Null [line: %u]\n", syntaxTree->lineno);
                    break;
                case IfK:
                    printf("If [line: %i]\n", syntaxTree->lineno);
                    break;
                case WhileK:
                    printf("While [line: %i]\n", syntaxTree->lineno);
                    break;
                case ForK:
                    printf("For [line: %u]\n", syntaxTree->lineno);
                    break;
                case CompoundK:
                    symbolTable.Enter("Compound Statement");
                    break;
                case ReturnK:
                    //if(syntaxTree->isArray)
                    printf("Return [line: %u]\n", syntaxTree->lineno);
                    break;
                case BreakK:
                    printf("Break [line: %u]\n", syntaxTree->lineno);
                    break;
                case RangeK:
                    printf("Range [line: %u]\n", syntaxTree->lineno);
                    break;
                default:
                    printf("ERROR: Unknown StmtK!! (printTree Function)\n");
                    break;
            }
        }
        // EXPRESSIONS
        else if(syntaxTree->nodekind==ExpK)
        {
            switch(syntaxTree->subkind.exp)
            {
                case OpK:
                    printf("Op: %s [line: %i]\n",syntaxTree->attr.name,syntaxTree->lineno);
                    break;
                case ConstantK:
                    if(syntaxTree->isArray){
                        printf("Const is array of type ");
                    }
                    else{
                        printf("Const of type ");
                    }
                    
                    switch(syntaxTree->expType)
                    {
                        case Void:
                            printf("void");
                            break;
                        case Integer:
                            printf("int");
                            break;
                        case Boolean:
                            printf("bool");
                            break;
                        case Char:
                            printf("char");
                            break;
                        case CharInt:
                            printf("chint");
                            break;
                        case Equal:
                            printf("eq");
                            break;
                        case UndefinedType:
                            printf("UndefinedType");
                            break;
                        default:
                            printf("ERROR: No syntaxTree->expType\n");
                            break;
                    }
                    switch(syntaxTree->expType)
                    {
                        case Void:
                            printf(": %i [line: %i]\n", syntaxTree->attr.value,syntaxTree->lineno);
                            break;
                        case Integer:
                            printf(": %i [line: %i]\n", syntaxTree->attr.value,syntaxTree->lineno);
                            break;
                        case Boolean:
                            printf(": ");
                            if(syntaxTree->attr.value == 1)
                            {
                                printf("true [line: %i]\n",syntaxTree->lineno);
                            }  
                            else
                            {
                                printf("false [line: %i]\n",syntaxTree->lineno);
                            }
                            break;
                        case Char:
                            if(syntaxTree->attr.string[0] != '"'){
                                printf(": '%c' [line: %i]\n", syntaxTree->attr.cvalue,syntaxTree->lineno);
                            }
                            else{
                                printf(": %s [line: %i]\n", syntaxTree->attr.string,syntaxTree->lineno);
                            }
                            break;
                        case CharInt:
                            printf(": %i [line: %i]\n", syntaxTree->attr.value,syntaxTree->lineno);
                            break;
                        case Equal:
                            printf(": %i [line: %i]\n", syntaxTree->attr.value,syntaxTree->lineno);
                            break;
                        case UndefinedType:
                            printf(": %i [line: %i]\n", syntaxTree->attr.value,syntaxTree->lineno);
                            break;
                        default:
                            printf(": %i [line: %i]\n", syntaxTree->attr.value,syntaxTree->lineno);
                            break;
                    }
                    break;
                case IdK:
                    printf("Id: %s [line: %i]\n",syntaxTree->attr.name, syntaxTree->lineno);
                    break;
                case AssignK:
                    printf("Assign: %s [line: %i]\n", syntaxTree->attr.name, syntaxTree->lineno);
                    break;
                case InitK:
                    printf("Init: %s [line: %i]\n",syntaxTree->attr.name,syntaxTree->lineno);
                    break;
                case CallK:
                    printf("Call: %s [line: %i]\n",syntaxTree->attr.name,syntaxTree->lineno);
                    break;
                default:
                    printf("ERROR: Unknown ExpK!! (printTree Function)\n");
            }
        }
        else { printf("UNKNOWN NODEKIND (syntaxTree->nodekind)\n"); }
        firstflag=1;
        for(int i = 0; i < MAXCHILDREN; i++)
        {
            printTree(syntaxTree->child[i]);
            //siblingCount = 0;
        }
        syntaxTree = syntaxTree->sibling;
        siblingCount++;
    }
    firstflag=1;
    UNINDENT;
}