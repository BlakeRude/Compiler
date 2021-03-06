// Author: Blake Rude
//
//
#include "globals.h"
#include "util.h"
#include <string>
static int indentno = 0;
int siblingFlag = 0;
//int siblingCount = 0;
int firstflag = 0;
#define INDENT indentno += 1
#define UNINDENT indentno -= 1

//from book
static void printSpaces(void)
{
    int i;
    for (i = 1; i < indentno; i++)
    {
        printf(".   ");
    }
}

void printTree(TreeNode *syntaxTree)
{
    int siblingCount = 0;
    INDENT;
    while (syntaxTree != NULL)
    {
        printSpaces();
        if (syntaxTree->isC0)
        {
            printf("Child: 0  ");
        }
        else if (syntaxTree->isC1)
        {
            printf("Child: 1  ");
        }
        else if (syntaxTree->isC2)
        {
            printf("Child: 2  ");
        }
        else if (!syntaxTree->isC0 && !syntaxTree->isC1 && !syntaxTree->isC2 && siblingCount > 0)
        {
            printf("Sibling: %i  ", siblingCount);
        }
        if (syntaxTree->nodekind == DeclK)
        {
            switch (syntaxTree->subkind.decl)
            {
            case VarK:
                if (syntaxTree->isArray)
                {
                    printf("Var: %s is array of type ", syntaxTree->attr.name);
                    switch (syntaxTree->declType)
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
                    printf(" [line: %i]\n", syntaxTree->lineno);
                }
                else
                {
                    printf("Var: %s of type ", syntaxTree->attr.name);
                    switch (syntaxTree->declType)
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
                    printf(" [line: %i]\n", syntaxTree->lineno);
                }
                break;
            case FuncK:
                printf("Func: %s returns type ", syntaxTree->attr.name);
                switch (syntaxTree->declType)
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
                printf(" [line: %i]\n", syntaxTree->lineno);
                break;
            case ParamK:
                if (syntaxTree->isArray)
                {
                    printf("Parm: %s is array of type ", syntaxTree->attr.name);
                    switch (syntaxTree->declType)
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
                    printf(" [line: %i]\n", syntaxTree->lineno);
                }
                else
                {
                    printf("Parm: %s of type ", syntaxTree->attr.name);
                    switch (syntaxTree->declType)
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
                    printf(" [line: %i]\n", syntaxTree->lineno);
                }
                break;
            default:
                printf("ERROR: Unknown DeclK!! (printTree Function)\n");
            }
        }
        // STATEMENTS
        else if (syntaxTree->nodekind == StmtK)
        {
            switch (syntaxTree->subkind.stmt)
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
                printf("Compound [line: %i]\n", syntaxTree->lineno);
                break;
            case ReturnK:
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
        else if (syntaxTree->nodekind == ExpK)
        {
            switch (syntaxTree->subkind.exp)
            {
            case OpK:
                printf("Op: %s [line: %i]\n", syntaxTree->attr.name, syntaxTree->lineno);
                break;
            case ConstantK:
                if (syntaxTree->isArray)
                {
                    printf("Const is array of type ");
                }
                else
                {
                    printf("Const of type ");
                }

                switch (syntaxTree->expType)
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
                switch (syntaxTree->expType)
                {
                case Void:
                    printf(": %i [line: %i]\n", syntaxTree->attr.value, syntaxTree->lineno);
                    break;
                case Integer:
                    printf(": %i [line: %i]\n", syntaxTree->attr.value, syntaxTree->lineno);
                    break;
                case Boolean:
                    printf(": ");
                    if (syntaxTree->attr.value == 1)
                    {
                        printf("true [line: %i]\n", syntaxTree->lineno);
                    }
                    else
                    {
                        printf("false [line: %i]\n", syntaxTree->lineno);
                    }
                    break;
                case Char:
                    if (syntaxTree->attr.string[0] != '"')
                    {
                        printf(": '%c' [line: %i]\n", syntaxTree->attr.cvalue, syntaxTree->lineno);
                    }
                    else
                    {
                        printf(": %s [line: %i]\n", syntaxTree->attr.string, syntaxTree->lineno);
                    }
                    break;
                case CharInt:
                    printf(": %i [line: %i]\n", syntaxTree->attr.value, syntaxTree->lineno);
                    break;
                case Equal:
                    printf(": %i [line: %i]\n", syntaxTree->attr.value, syntaxTree->lineno);
                    break;
                case UndefinedType:
                    printf(": %i [line: %i]\n", syntaxTree->attr.value, syntaxTree->lineno);
                    break;
                default:
                    printf(": %i [line: %i]\n", syntaxTree->attr.value, syntaxTree->lineno);
                    break;
                }
                break;
            case IdK:
                printf("Id: %s [line: %i]\n", syntaxTree->attr.name, syntaxTree->lineno);
                break;
            case AssignK:
                printf("Assign: %s [line: %i]\n", syntaxTree->attr.name, syntaxTree->lineno);
                break;
            case InitK:
                printf("Init: %s [line: %i]\n", syntaxTree->attr.name, syntaxTree->lineno);
                break;
            case CallK:
                printf("Call: %s [line: %i]\n", syntaxTree->attr.name, syntaxTree->lineno);
                break;
            default:
                printf("ERROR: Unknown ExpK!! (printTree Function)\n");
            }
        }
        else
        {
            printf("UNKNOWN NODEKIND (syntaxTree->nodekind)\n");
        }
        firstflag = 1;
        for (int i = 0; i < MAXCHILDREN; i++)
        {
            printTree(syntaxTree->child[i]);
            //siblingCount = 0;
        }
        syntaxTree = syntaxTree->sibling;
        siblingCount++;
    }
    UNINDENT;
}

void printHelp()
{
    printf("Usage: c- [options] [sourceFile]\n");
    printf("options:\n");
    printf("-d      - turn on parser debugging\n");
    printf("-D      - turn on symbol table debugging\n");
    printf("-h      - this usage message\n");
    printf("-d      - print the abstract syntax tree\n");
    printf("-d      - print the abstract syntax tree plus type information\n");
    return;
}

void printExtendedTree(TreeNode *syntaxTree)
{
    //bool isCompound = false;
    //Taken and modified from Textbook p.510
    int siblingCount = 0;
    INDENT;
    while (syntaxTree != NULL)
    {
        printSpaces();
        if (syntaxTree->isC0)
        {
            printf("Child: 0  ");
        }
        else if (syntaxTree->isC1)
        {
            printf("Child: 1  ");
        }
        else if (syntaxTree->isC2)
        {
            printf("Child: 2  ");
        }
        else if (!syntaxTree->isC0 && !syntaxTree->isC1 && !syntaxTree->isC2 && siblingCount > 0)
        {
            printf("Sibling: %i  ", siblingCount);
        }
        if (syntaxTree->nodekind == DeclK)
        {
            switch (syntaxTree->subkind.decl)
            {
            case VarK:
                if (syntaxTree->isArray)
                {
                    printf("Var: %s is array of type ", syntaxTree->attr.name);
                    switch (syntaxTree->declType)
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
                    printf(" [line: %i]\n", syntaxTree->lineno);
                }
                else
                {
                    printf("Var: %s of type ", syntaxTree->attr.name);
                    switch (syntaxTree->declType)
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
                    printf(" [line: %i]\n", syntaxTree->lineno);
                }
                break;
            case FuncK:
                printf("Func: %s returns type ", syntaxTree->attr.name);
                switch (syntaxTree->declType)
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
                printf(" [line: %i]\n", syntaxTree->lineno);
                break;
            case ParamK:
                if (syntaxTree->isArray)
                {
                    printf("Parm: %s is array of type ", syntaxTree->attr.name);
                    switch (syntaxTree->declType)
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
                    printf(" [line: %i]\n", syntaxTree->lineno);
                }
                else
                {
                    printf("Parm: %s of type ", syntaxTree->attr.name);
                    switch (syntaxTree->declType)
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
                    printf(" [line: %i]\n", syntaxTree->lineno);
                }
                break;
            default:
                printf("ERROR: Unknown DeclK!! (printExtendedTree Function)\n");
            }
        }
        // STATEMENTS
        else if (syntaxTree->nodekind == StmtK)
        {
            switch (syntaxTree->subkind.stmt)
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
                //isCompound = true;
                printf("Compound [line: %i]\n", syntaxTree->lineno);
                break;
            case ReturnK:
                printf("Return [line: %u]\n", syntaxTree->lineno);
                break;
            case BreakK:
                printf("Break [line: %u]\n", syntaxTree->lineno);
                break;
            case RangeK:
                printf("Range [line: %u]\n", syntaxTree->lineno);
                break;
            default:
                printf("ERROR: Unknown StmtK!! (printExtendedTree Function)\n");
                break;
            }
        }
        // EXPRESSIONS
        else if (syntaxTree->nodekind == ExpK)
        {
            switch (syntaxTree->subkind.exp)
            {
            case OpK:
                if ((strcmp(syntaxTree->attr.name, "SIZEOF") == 0))
                {
                    printf("Op: sizeof of type ");
                    switch (syntaxTree->nodeType)
                    {
                    case nVoid:
                        printf("void [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nInt:
                        printf("int [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nBool:
                        printf("bool [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nChar:
                        printf("char [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nUndefinedType:
                        printf("undefined [line: %i]\n", syntaxTree->lineno);
                        break;
                    default:
                        printf("ERROR: No syntaxTree->expType\n");
                        break;
                    }
                }
                else if ((strcmp(syntaxTree->attr.name, "AND") == 0))
                {
                    printf("Op: and of type ");
                    switch (syntaxTree->nodeType)
                    {
                    case nVoid:
                        printf("void [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nInt:
                        printf("int [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nBool:
                        printf("bool [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nChar:
                        printf("char [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nUndefinedType:
                        printf("undefined [line: %i]\n", syntaxTree->lineno);
                        break;
                    default:
                        printf("ERROR: No syntaxTree->expType\n");
                        break;
                    }
                }
                else if ((strcmp(syntaxTree->attr.name, "OR") == 0))
                {
                    printf("Op: or of type ");
                    switch (syntaxTree->nodeType)
                    {
                    case nVoid:
                        printf("void [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nInt:
                        printf("int [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nBool:
                        printf("bool [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nChar:
                        printf("char [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nUndefinedType:
                        printf("undefined [line: %i]\n", syntaxTree->lineno);
                        break;
                    default:
                        printf("ERROR: No syntaxTree->expType\n");
                        break;
                    }
                }
                else if ((strcmp(syntaxTree->attr.name, "NOT") == 0))
                {
                    printf("Op: not of type ");
                    switch (syntaxTree->nodeType)
                    {
                    case nVoid:
                        printf("void [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nInt:
                        printf("int [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nBool:
                        printf("bool [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nChar:
                        printf("char [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nUndefinedType:
                        printf("undefined [line: %i]\n", syntaxTree->lineno);
                        break;
                    default:
                        printf("ERROR: No syntaxTree->expType\n");
                        break;
                    }
                }
                else if ((strcmp(syntaxTree->attr.name, "CHSIGN") == 0))
                {
                    printf("Op: chsign of type ");
                    switch (syntaxTree->nodeType)
                    {
                    case nVoid:
                        printf("void [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nInt:
                        printf("int [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nBool:
                        printf("bool [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nChar:
                        printf("char [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nUndefinedType:
                        printf("undefined [line: %i]\n", syntaxTree->lineno);
                        break;
                    default:
                        printf("ERROR: No syntaxTree->expType\n");
                        break;
                    }
                }
                else
                {
                    printf("Op: %s of type ", syntaxTree->attr.name);
                    switch (syntaxTree->nodeType)
                    {
                    case nVoid:
                        printf("void [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nInt:
                        printf("int [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nBool:
                        printf("bool [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nChar:
                        printf("char [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nUndefinedType:
                        printf("undefined [line: %i]\n", syntaxTree->lineno);
                        break;
                    default:
                        printf("char [line: %i]\n", syntaxTree->lineno);
                        syntaxTree->nodeType = nChar;
                        break;
                    }
                }

                break;
            case ConstantK:
                if (syntaxTree->isArray)
                {
                    printf("Const is array ");
                }
                else
                {
                    printf("Const ");
                }
                switch (syntaxTree->nodeType)
                {
                case nVoid:
                    printf("%d of type ", syntaxTree->attr.value);
                    break;
                case nInt:
                    printf("%d of type ", syntaxTree->attr.value);
                    break;
                case nBool:
                    if (syntaxTree->attr.value == 1)
                    {
                        printf("true of type ");
                    }
                    else
                    {
                        printf("false of type ");
                    }
                    break;
                case nChar:
                    if (syntaxTree->attr.string[0] != '"')
                    {
                        printf("'%c' of type ", syntaxTree->attr.cvalue);
                    }
                    else
                    {
                        printf("%s of type ", syntaxTree->attr.string);
                    }
                    break;
                case nUndefinedType:
                    printf("%i of type ", syntaxTree->attr.value);
                    break;
                default:
                    printf("%i of type ", syntaxTree->attr.value);
                    break;
                }
                //printf(" ");
                switch (syntaxTree->nodeType)
                {
                case nVoid:
                    printf("void [line: %i]\n", syntaxTree->lineno);
                    break;
                case nInt:
                    printf("int [line: %i]\n", syntaxTree->lineno);
                    break;
                case nBool:
                    printf("bool [line: %i]\n", syntaxTree->lineno);
                    break;
                case nChar:
                    printf("char [line: %i]\n", syntaxTree->lineno);
                    break;
                case nUndefinedType:
                    printf("undefined [line: %i]\n", syntaxTree->lineno);
                    break;
                default:
                    printf("ERROR: No syntaxTree->expType\n");
                    break;
                }
                break;
            case IdK:
                printf("Id: %s of type ", syntaxTree->attr.name);
                switch (syntaxTree->nodeType)
                {
                case nVoid:
                    printf("char [line: %i]\n", syntaxTree->lineno);
                    syntaxTree->nodeType = nChar;
                    break;
                case nInt:
                    printf("int [line: %i]\n", syntaxTree->lineno);
                    break;
                case nBool:
                    printf("bool [line: %i]\n", syntaxTree->lineno);
                    break;
                case nChar:
                    printf("char [line: %i]\n", syntaxTree->lineno);
                    break;
                case nUndefinedType:
                    printf("undefined [line: %i]\n", syntaxTree->lineno);
                    break;
                default:
                    printf("char [line: %i]\n", syntaxTree->lineno);
                    syntaxTree->nodeType = nChar;
                    break;
                }
                break;
            case AssignK:
                printf("Assign: %s of type ", syntaxTree->attr.name);
                switch (syntaxTree->nodeType)
                {
                case nVoid:
                    printf("void [line: %i]\n", syntaxTree->lineno);
                    break;
                case nInt:
                    printf("int [line: %i]\n", syntaxTree->lineno);
                    break;
                case nBool:
                    printf("bool [line: %i]\n", syntaxTree->lineno);
                    break;
                case nChar:
                    printf("char [line: %i]\n", syntaxTree->lineno);
                    break;
                case nUndefinedType:
                    printf("undefined [line: %i]\n", syntaxTree->lineno);
                    break;
                default:
                    printf("ERROR: No syntaxTree->expType\n");
                    break;
                }
                break;
            case InitK:
                printf("Init: %s [line: %i]\n", syntaxTree->attr.name, syntaxTree->lineno);
                break;
            case CallK:
                printf("Call: %s of type ", syntaxTree->attr.name);
                switch (syntaxTree->nodeType)
                {
                case nVoid:
                    printf("void [line: %i]\n", syntaxTree->lineno);
                    break;
                case nInt:
                    printf("int [line: %i]\n", syntaxTree->lineno);
                    break;
                case nBool:
                    printf("bool [line: %i]\n", syntaxTree->lineno);
                    break;
                case nChar:
                    printf("char [line: %i]\n", syntaxTree->lineno);
                    break;
                case nUndefinedType:
                    printf("undefined [line: %i]\n", syntaxTree->lineno);
                    break;
                default:
                    printf("void [line: %i]\n", syntaxTree->lineno);
                    syntaxTree->nodeType = nVoid;
                    break;
                }
                break;
            default:
                printf("ERROR: Unknown ExpK!! (printExtendedTree Function)\n");
            }
        }
        else
        {
            printf("UNKNOWN NODEKIND (syntaxTree->nodekind)\n");
        }
        firstflag = 1;
        for (int i = 0; i < MAXCHILDREN; i++)
        {
            //if(isCompound) { printf("compound child: %i\n",i);}
            printExtendedTree(syntaxTree->child[i]);
            //siblingCount = 0;
        }
        //if(isCompound){ printf("leave compound scope\n"); isCompound = false;}
        syntaxTree = syntaxTree->sibling;
        siblingCount++;
    }
    UNINDENT;
}
///////////////////////////////////
//   mem tree
//
//
//
void printMemTree(TreeNode *syntaxTree)
{
    int siblingCount = 0;
    INDENT;
    while (syntaxTree != NULL)
    {
        printSpaces();
        if (syntaxTree->isC0)
        {
            printf("Child: 0  ");
        }
        else if (syntaxTree->isC1)
        {
            printf("Child: 1  ");
        }
        else if (syntaxTree->isC2)
        {
            printf("Child: 2  ");
        }
        else if (!syntaxTree->isC0 && !syntaxTree->isC1 && !syntaxTree->isC2 && siblingCount > 0)
        {
            printf("Sibling: %i  ", siblingCount);
        }
        if (syntaxTree->nodekind == DeclK)
        {
            switch (syntaxTree->subkind.decl)
            {
            case VarK:
                if (syntaxTree->isArray)
                {
                    printf("Var: %s of ", syntaxTree->attr.name);
                    if(syntaxTree->mem == stat)
                    {
                        printf("static ");
                    }
                    printf("array of type ");
                    switch (syntaxTree->declType)
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
                    switch (syntaxTree->mem)
                    {
                    case loca:
                        printf(" [mem: Local loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                        break;
                    case stat:
                        printf(" [mem: LocalStatic loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                        break;
                    case para:
                        printf(" [mem: Parameter loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                        break;
                    case glob:
                        printf(" [mem: Global loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                        break;
                    case non:
                        printf(" [mem: Local loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                        syntaxTree->mem = loca;
                        break;
                    default:
                        printf(" no syntaxTree->mem varK ");
                        break;
                    }
                    printf(" [line: %i]\n", syntaxTree->lineno);
                }
                else
                {
                    printf("Var: %s of ", syntaxTree->attr.name);
                    switch (syntaxTree->declType)
                    {
                    case DeclVoid:
                        if(syntaxTree->mem == stat)
                        {
                            printf("static type void");
                        }
                        else
                        {
                            printf("type void");
                        }
                        break;
                    case DeclInteger:
                        if(syntaxTree->mem == stat)
                        {
                            printf("static type int");
                        }
                        else
                        {
                            printf("type int");
                        }
                        break;
                    case DeclBoolean:
                        if(syntaxTree->mem == stat)
                        {
                            printf("static type bool");
                        }
                        else
                        {
                            printf("type bool");
                        }
                        break;
                    case DeclChar:
                        if(syntaxTree->mem == stat)
                        {
                            printf("static type char");
                        }
                        else
                        {
                            printf("type char");
                        }
                        break;
                    case DeclUndefinedType:
                        printf("UndefinedType");
                        break;
                    default:
                        printf("error");
                        break;
                    }
                    switch (syntaxTree->mem)
                    {
                    case loca:
                        printf(" [mem: Local loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                        break;
                    case stat:
                        printf(" [mem: LocalStatic loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                        break;
                    case para:
                        printf(" [mem: Parameter loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                        break;
                    case glob:
                        printf(" [mem: Global loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                        break;
                    case non:
                        printf(" [mem: None loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                        break;
                    default:
                        printf(" no syntaxTree->mem varK ");
                        break;
                    }
                    printf(" [line: %i]\n", syntaxTree->lineno);
                }
                break;
            case FuncK:
                printf("Func: %s returns type ", syntaxTree->attr.name);
                switch (syntaxTree->declType)
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
                switch (syntaxTree->mem)
                {
                case loca:
                    printf(" [mem: Local loc: %d size: %i]", syntaxTree->location, syntaxTree->size);
                    break;
                case stat:
                    printf(" [mem: LocalStatic loc: %d size: %i]", syntaxTree->location, syntaxTree->size);
                    break;
                case para:
                    printf(" [mem: Parameter loc: %d size: %i]", syntaxTree->location, syntaxTree->size);
                    break;
                case glob:
                    printf(" [mem: Global loc: %d size: %i]", syntaxTree->location, syntaxTree->size);
                    break;
                case non:
                    printf(" [mem: None loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                    break;
                default:
                    printf(" no syntaxTree->mem varK ");
                    break;
                }
                printf(" [line: %i]\n", syntaxTree->lineno);
                break;
            case ParamK:
                if (syntaxTree->isArray)
                {
                    printf("Parm: %s of array of type ", syntaxTree->attr.name);
                    switch (syntaxTree->declType)
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
                    switch (syntaxTree->mem)
                    {
                    case loca:
                        printf(" [mem: Local loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                        break;
                    case stat:
                        printf(" [mem: LocalStatic loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                        break;
                    case para:
                        printf(" [mem: Parameter loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                        break;
                    case glob:
                        printf(" [mem: Global loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                        break;
                    case non:
                        printf(" [mem: None loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                        break;
                    default:
                        printf(" no syntaxTree->mem varK ");
                        break;
                    }
                    printf(" [line: %i]\n", syntaxTree->lineno);
                }
                else
                {
                    printf("Parm: %s of type ", syntaxTree->attr.name);
                    switch (syntaxTree->declType)
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
                    switch (syntaxTree->mem)
                    {
                    case loca:
                        printf(" [mem: Local loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                        break;
                    case stat:
                        printf(" [mem: LocalStatic loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                        break;
                    case para:
                        printf(" [mem: Parameter loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                        break;
                    case glob:
                        printf(" [mem: Global loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                        break;
                    case non:
                        printf(" [mem: None loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                        break;
                    default:
                        printf(" no syntaxTree->mem varK ");
                        break;
                    }
                    printf(" [line: %i]\n", syntaxTree->lineno);
                }
                break;
            default:
                printf("ERROR: Unknown DeclK!! (printExtendedTree Function)\n");
            }
        }
        // STATEMENTS
        else if (syntaxTree->nodekind == StmtK)
        {
            switch (syntaxTree->subkind.stmt)
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
                printf("For");
                switch (syntaxTree->mem)
                {
                case loca:
                    printf(" [mem: Local loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                    break;
                case stat:
                    printf(" [mem: LocalStatic loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                    break;
                case para:
                    printf(" [mem: Parameter loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                    break;
                case glob:
                    printf(" [mem: Global loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                    break;
                case non:
                    printf(" [mem: None loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                    break;
                default:
                    printf(" no syntaxTree->mem varK ");
                    break;
                }
                printf(" [line: %u]\n", syntaxTree->lineno);
                break;
            case CompoundK:
                //isCompound = true;
                printf("Compound");
                switch (syntaxTree->mem)
                {
                case loca:
                    printf(" [mem: Local loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                    break;
                case stat:
                    printf(" [mem: LocalStatic loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                    break;
                case para:
                    printf(" [mem: Parameter loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                    break;
                case glob:
                    printf(" [mem: Global loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                    break;
                case non:
                    printf(" [mem: None loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                    break;
                default:
                    printf(" no syntaxTree->mem varK ");
                    break;
                }
                printf(" [line: %i]\n", syntaxTree->lineno);
                break;
            case ReturnK:
                printf("Return [line: %u]\n", syntaxTree->lineno);
                break;
            case BreakK:
                printf("Break [line: %u]\n", syntaxTree->lineno);
                break;
            case RangeK:
                printf("Range [line: %u]\n", syntaxTree->lineno);
                break;
            default:
                printf("ERROR: Unknown StmtK!! (printExtendedTree Function)\n");
                break;
            }
        }
        // EXPRESSIONS
        else if (syntaxTree->nodekind == ExpK)
        {
            switch (syntaxTree->subkind.exp)
            {
            case OpK:
                if ((strcmp(syntaxTree->attr.name, "SIZEOF") == 0))
                {
                    printf("Op: sizeof of type ");
                    switch (syntaxTree->nodeType)
                    {
                    case nVoid:
                        printf("void [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nInt:
                        printf("int [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nBool:
                        printf("bool [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nChar:
                        printf("char [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nUndefinedType:
                        printf("undefined [line: %i]\n", syntaxTree->lineno);
                        break;
                    default:
                        printf("ERROR: No syntaxTree->expType\n");
                        break;
                    }
                }
                else if ((strcmp(syntaxTree->attr.name, "AND") == 0))
                {
                    printf("Op: and of type ");
                    switch (syntaxTree->nodeType)
                    {
                    case nVoid:
                        printf("void [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nInt:
                        printf("int [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nBool:
                        printf("bool [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nChar:
                        printf("char [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nUndefinedType:
                        printf("undefined [line: %i]\n", syntaxTree->lineno);
                        break;
                    default:
                        printf("ERROR: No syntaxTree->expType\n");
                        break;
                    }
                }
                else if ((strcmp(syntaxTree->attr.name, "OR") == 0))
                {
                    printf("Op: or of type ");
                    switch (syntaxTree->nodeType)
                    {
                    case nVoid:
                        printf("void [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nInt:
                        printf("int [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nBool:
                        printf("bool [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nChar:
                        printf("char [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nUndefinedType:
                        printf("undefined [line: %i]\n", syntaxTree->lineno);
                        break;
                    default:
                        printf("ERROR: No syntaxTree->expType\n");
                        break;
                    }
                }
                else if ((strcmp(syntaxTree->attr.name, "NOT") == 0))
                {
                    printf("Op: not of type ");
                    switch (syntaxTree->nodeType)
                    {
                    case nVoid:
                        printf("void [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nInt:
                        printf("int [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nBool:
                        printf("bool [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nChar:
                        printf("char [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nUndefinedType:
                        printf("undefined [line: %i]\n", syntaxTree->lineno);
                        break;
                    default:
                        printf("ERROR: No syntaxTree->expType\n");
                        break;
                    }
                }
                else if ((strcmp(syntaxTree->attr.name, "CHSIGN") == 0))
                {
                    printf("Op: chsign of type ");
                    switch (syntaxTree->nodeType)
                    {
                    case nVoid:
                        printf("void [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nInt:
                        printf("int [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nBool:
                        printf("bool [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nChar:
                        printf("char [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nUndefinedType:
                        printf("undefined [line: %i]\n", syntaxTree->lineno);
                        break;
                    default:
                        printf("ERROR: No syntaxTree->expType\n");
                        break;
                    }
                }
                else
                {
                    printf("Op: %s of type ", syntaxTree->attr.name);
                    switch (syntaxTree->nodeType)
                    {
                    case nVoid:
                        printf("void [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nInt:
                        printf("int [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nBool:
                        printf("bool [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nChar:
                        printf("char [line: %i]\n", syntaxTree->lineno);
                        break;
                    case nUndefinedType:
                        printf("undefined [line: %i]\n", syntaxTree->lineno);
                        break;
                    default:
                        printf("char [line: %i]\n", syntaxTree->lineno);
                        syntaxTree->nodeType = nChar;
                        break;
                    }
                }

                break;
            case ConstantK:
                printf("Const ");
                switch (syntaxTree->nodeType)
                {
                case nVoid:
                    printf("%d ", syntaxTree->attr.value);
                    break;
                case nInt:
                    printf("%d ", syntaxTree->attr.value);
                    break;
                case nBool:
                    if (syntaxTree->attr.value == 1)
                    {
                        printf("true ");
                    }
                    else
                    {
                        printf("false ");
                    }
                    break;
                case nChar:
                    if (syntaxTree->attr.string[0] != '"')
                    {
                        printf("'%c' ", syntaxTree->attr.cvalue);
                    }
                    else
                    {
                        printf("%s ", syntaxTree->attr.string);
                    }
                    break;
                case nUndefinedType:
                    printf("%i ", syntaxTree->attr.value);
                    break;
                default:
                    printf("%i ", syntaxTree->attr.value);
                    break;
                }
                if (syntaxTree->isArray)
                {
                    printf("of array of type ");
                }
                else
                {
                    printf("of type ");
                }
                switch (syntaxTree->nodeType)
                {
                case nVoid:
                    printf("void");
                    break;
                case nInt:
                    printf("int");
                    break;
                case nBool:
                    printf("bool");
                    break;
                case nChar:
                    printf("char");
                    break;
                case nUndefinedType:
                    printf("undefined");
                    break;
                default:
                    printf("ERROR: No syntaxTree->expType\n");
                    break;
                }
                if (syntaxTree->isStringConstant)
                {
                    switch (syntaxTree->mem)
                    {
                    case loca:
                        printf(" [mem: Local loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                        break;
                    case stat:
                        printf(" [mem: LocalStatic loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                        break;
                    case para:
                        printf(" [mem: Parameter loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                        break;
                    case glob:
                        printf(" [mem: Global loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                        break;
                    case non:
                        printf(" [mem: None loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                        break;
                    default:
                        printf(" no syntaxTree->mem varK ");
                        break;
                    }
                }
                printf(" [line: %i]\n", syntaxTree->lineno);
                break;
            case IdK:
            {
                bool ifHappened = false;
                printf("Id: %s ", syntaxTree->attr.name);
                if (syntaxTree->isArray)
                {
                    if(syntaxTree->mem == stat)
                    {
                        printf("of static array ");
                        ifHappened = true;
                    }
                    else
                    {
                        printf("of array ");
                    }
                }
                printf("of ");
                switch (syntaxTree->nodeType)
                {
                case nVoid:
                    if(syntaxTree->mem == stat && !ifHappened)
                    {
                        printf("static type void");
                    }
                    else
                    {
                        printf("type void");
                    }
                    syntaxTree->nodeType = nChar;
                    break;
                case nInt:
                    if(syntaxTree->mem == stat && !ifHappened)
                    {
                        printf("static type int");
                    }
                    else
                    {
                        printf("type int");
                    }
                    break;
                case nBool:
                    if(syntaxTree->mem == stat && !ifHappened)
                    {
                        printf("static type bool");
                    }
                    else
                    {
                        printf("type bool");
                    }
                    break;
                case nChar:
                    if(syntaxTree->mem == stat && !ifHappened)
                    {
                        printf("static type char");
                    }
                    else
                    {
                        printf("type char");
                    }
                    break;
                case nUndefinedType:
                    printf("undefined");
                    break;
                default:
                    printf("error");
                    break;
                }
                switch (syntaxTree->mem)
                {
                case loca:
                    printf(" [mem: Local loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                    break;
                case stat:
                    printf(" [mem: LocalStatic loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                    break;
                case para:
                    printf(" [mem: Parameter loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                    break;
                case glob:
                    printf(" [mem: Global loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                    break;
                case non:
                    printf(" [mem: Local loc: %d size: %d]", syntaxTree->location, syntaxTree->size);
                    syntaxTree->mem = loca;
                    break;
                default:
                    printf(" no syntaxTree->mem varK ");
                    break;
                }
                printf(" [line: %i]\n", syntaxTree->lineno);
                break;
            }
            case AssignK:
                printf("Assign: %s ", syntaxTree->attr.name);
                if (syntaxTree->isArray)
                {
                    printf("of array ");
                }
                printf("of type ");
                switch (syntaxTree->nodeType)
                {
                case nVoid:
                    printf("void [line: %i]\n", syntaxTree->lineno);
                    break;
                case nInt:
                    printf("int [line: %i]\n", syntaxTree->lineno);
                    break;
                case nBool:
                    printf("bool [line: %i]\n", syntaxTree->lineno);
                    break;
                case nChar:
                    printf("char [line: %i]\n", syntaxTree->lineno);
                    break;
                case nUndefinedType:
                    printf("undefined [line: %i]\n", syntaxTree->lineno);
                    break;
                default:
                    printf("ERROR: No syntaxTree->expType\n");
                    break;
                }
                break;
            case InitK:
                printf("Init: %s [line: %i]\n", syntaxTree->attr.name, syntaxTree->lineno);
                break;
            case CallK:
                printf("Call: %s of type ", syntaxTree->attr.name);
                switch (syntaxTree->nodeType)
                {
                case nVoid:
                    printf("void [line: %i]\n", syntaxTree->lineno);
                    break;
                case nInt:
                    printf("int [line: %i]\n", syntaxTree->lineno);
                    break;
                case nBool:
                    printf("bool [line: %i]\n", syntaxTree->lineno);
                    break;
                case nChar:
                    printf("char [line: %i]\n", syntaxTree->lineno);
                    break;
                case nUndefinedType:
                    printf("undefined [line: %i]\n", syntaxTree->lineno);
                    break;
                default:
                    printf("void [line: %i]\n", syntaxTree->lineno);
                    syntaxTree->nodeType = nVoid;
                    break;
                }
                break;
            default:
                printf("ERROR: Unknown ExpK!! (printMemTree Function)\n");
            }
        }
        else
        {
            printf("UNKNOWN NODEKIND (syntaxTree->nodekind)\n");
        }
        firstflag = 1;
        for (int i = 0; i < MAXCHILDREN; i++)
        {
            //if(isCompound) { printf("compound child: %i\n",i);}
            printMemTree(syntaxTree->child[i]);
            //siblingCount = 0;
        }
        //if(isCompound){ printf("leave compound scope\n"); isCompound = false;}
        syntaxTree = syntaxTree->sibling;
        siblingCount++;
    }
    UNINDENT;
}

TreeNode *buildIOTree()
{
    TreeNode *in = new TreeNode;
    TreeNode *inb = new TreeNode;
    TreeNode *inc = new TreeNode;
    TreeNode *out = new TreeNode;
    TreeNode *outb = new TreeNode;
    TreeNode *outc = new TreeNode;
    TreeNode *outnl = new TreeNode;
    TreeNode *d1 = new TreeNode;
    TreeNode *d2 = new TreeNode;
    TreeNode *d3 = new TreeNode;

    const char *temp1 = "*dummy*";
    d1->attr.name = const_cast<char *>(temp1);
    d1->nodeType = nInt;
    d1->declType = DeclInteger;
    d1->nodekind = DeclK;
    d1->subkind.decl = ParamK;
    d1->child[0] = NULL;
    d1->child[1] = NULL;
    d1->child[2] = NULL;
    d1->lineno = -1;

    d2->attr.name = const_cast<char *>(temp1);
    d2->nodeType = nBool;
    d2->nodekind = DeclK;
    d2->declType = DeclBoolean;
    d2->subkind.decl = ParamK;
    d2->child[0] = NULL;
    d2->child[1] = NULL;
    d2->child[2] = NULL;
    d2->lineno = -1;

    d3->attr.name = const_cast<char *>(temp1);
    d3->nodeType = nChar;
    d3->nodekind = DeclK;
    d3->declType = DeclChar;
    d3->subkind.decl = ParamK;
    d3->child[0] = NULL;
    d3->child[1] = NULL;
    d3->child[2] = NULL;
    d3->lineno = -1;

    const char *temp2 = "input";
    in->attr.name = const_cast<char *>(temp2);
    in->nodeType = nInt;
    in->nodekind = DeclK;
    in->declType = DeclInteger;
    in->subkind.decl = FuncK;
    in->emitLoc = 1;
    in->child[0] = NULL;
    in->child[1] = NULL;
    in->child[2] = NULL;
    in->lineno = -1;

    const char *temp3 = "inputb";
    inb->attr.name = const_cast<char *>(temp3);
    inb->nodeType = nBool;
    inb->nodekind = DeclK;
    inb->declType = DeclBoolean;
    inb->subkind.decl = FuncK;
    inb->emitLoc = 12;
    inb->child[0] = NULL;
    inb->child[1] = NULL;
    inb->child[2] = NULL;
    inb->lineno = -1;

    const char *temp4 = "inputc";
    inc->attr.name = const_cast<char *>(temp4);
    inc->nodeType = nChar;
    inc->nodekind = DeclK;
    inc->declType = DeclChar;
    inc->subkind.decl = FuncK;
    inc->emitLoc = 23;
    inc->child[0] = NULL;
    inc->child[1] = NULL;
    inc->child[2] = NULL;
    inc->lineno = -1;

    const char *temp5 = "output";
    out->attr.name = const_cast<char *>(temp5);
    out->nodeType = nVoid;
    out->nodekind = DeclK;
    out->declType = DeclVoid;
    out->subkind.decl = FuncK;
    out->emitLoc = 6;
    out->child[0] = d1;
    out->child[0]->isC0 = true;
    out->child[1] = NULL;
    out->child[2] = NULL;
    out->lineno = -1;

    const char *temp6 = "outputb";
    outb->attr.name = const_cast<char *>(temp6);
    outb->nodeType = nVoid;
    outb->nodekind = DeclK;
    outb->declType = DeclVoid;
    outb->subkind.decl = FuncK;
    outb->emitLoc = 17;
    outb->child[0] = d2;
    outb->child[0]->isC0 = true;
    outb->child[1] = NULL;
    outb->child[2] = NULL;
    outb->lineno = -1;

    const char *temp7 = "outputc";
    outc->attr.name = const_cast<char *>(temp7);
    outc->nodeType = nVoid;
    outc->nodekind = DeclK;
    outc->declType = DeclVoid;
    outc->subkind.decl = FuncK;
    outc->emitLoc = 28;
    outc->child[0] = d3;
    outc->child[0]->isC0 = true;
    outc->child[1] = NULL;
    outc->child[2] = NULL;
    outc->lineno = -1;

    const char *temp8 = "outnl";
    outnl->attr.name = const_cast<char *>(temp8);
    outnl->nodeType = nVoid;
    outnl->nodekind = DeclK;
    outnl->declType = DeclVoid;
    outnl->subkind.decl = FuncK;
    outnl->emitLoc = 34;
    outnl->child[0] = NULL;
    outnl->child[1] = NULL;
    outnl->child[2] = NULL;
    outnl->lineno = -1;

    in->sibling = inb;
    inb->sibling = inc;
    inc->sibling = out;
    out->sibling = outb;
    outb->sibling = outc;
    outc->sibling = outnl;
    outnl->sibling = NULL;
    return in;
}