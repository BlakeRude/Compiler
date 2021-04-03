#include "globals.h"
#include "semantic.h"
#include "symbolTable.h"
#include "util.h"
//#include "util.h"
static int indentno = 0;
static int siblingFlag = 0;
//int siblingCount = 0;
static int firstflag = 0;
static int firstLeftglag = 0;
#define INDENT indentno += 1
#define UNINDENT indentno -= 1
int functionScope = 0;
int isMain = 0;
static void printSpaces(void)
{
    int i;
    for (i = 1; i < indentno; i++)
    {
        printf(".   ");
    }
}

void semanticAnalysis(TreeNode *syntaxTree, SymbolTable symbolTable)
{
    fflush(stdout);
    printf("1\n");
    fflush(stdout);
    symbolTable.print(pointerPrintAddr);
    bool isCompound = false;
    //if(!firstflag)
    //printf("SEMANTIC ANALYSIS\n");
    //Taken and modified from Textbook p.510
    int siblingCount = 0;
    INDENT;
    while (syntaxTree != NULL)
    {
        if (syntaxTree->isC0)
        { /*printf("Child: 0  ");*/
        }
        else if (syntaxTree->isC1)
        { /*printf("Child: 1  ");*/
        }
        else if (syntaxTree->isC2)
        { /*printf("Child: 2  ");*/
        }
        else if (!syntaxTree->isC0 && !syntaxTree->isC1 && !syntaxTree->isC2 && siblingCount > 0)
        {
            /*printf("Sibling: %i  ",siblingCount);*/
        }
        //DECLARATIONS
        if (syntaxTree->nodekind == DeclK)
        {
            switch (syntaxTree->subkind.decl)
            {
            case VarK:
            {
                //symbolTable.print(pointerPrintAddr);
                void *data = symbolTable.lookupGlobal(syntaxTree->attr.name);
                if (data == NULL)
                {
                    bool b = symbolTable.insert(syntaxTree->attr.name, syntaxTree);
                }
                else
                {
                    printf("ERROR(%d): Symbol '%s' is already declared at line %d.\n", syntaxTree->lineno, syntaxTree->attr.name, static_cast<TreeNode *>(data)->lineno);
                    numErrors++;
                }
                break;
            }
            case FuncK:
            {
                if (strcmp(syntaxTree->attr.name, "main") == 0)
                {
                    isMain = 1;
                }
                void *data = symbolTable.lookup(syntaxTree->attr.name);
                if (data == NULL)
                {
                    functionScope = 0;
                    symbolTable.insert(syntaxTree->attr.name, syntaxTree);
                    symbolTable.enter("Function Call");
                }
                else
                {
                    printf("ERROR(%d): Symbol '%s' is already declared at line %d.\n", syntaxTree->lineno, syntaxTree->attr.name, static_cast<TreeNode *>(data)->lineno);
                    numErrors++;
                }
                break;
            }
            case ParamK:
            {
                void *data = symbolTable.lookup(syntaxTree->attr.name);
                if (data == NULL)
                {
                    symbolTable.insert(syntaxTree->attr.name, syntaxTree);
                }
                else
                {
                    printf("ERROR(%d): Symbol '%s' is already declared at line %d.\n", syntaxTree->lineno, syntaxTree->attr.name, static_cast<TreeNode *>(data)->lineno);
                    numErrors++;
                }
                break;
            }
            default:
                printf("ERROR: Unknown DeclK!! (printTree Function)\n");
                break;
            }
        }
        // STATEMENTS
        else if (syntaxTree->nodekind == StmtK)
        {
            switch (syntaxTree->subkind.stmt)
            {
                case NullK:
                    break;
                case IfK:
                    break;
                case WhileK:
                    break;
                case ForK:
                    break;
                case CompoundK:
                    if (functionScope)
                    {
                        isCompound = true;
                        symbolTable.enter("Compound Statement");
                    }
                    else
                    {
                        functionScope = 1;
                    }
                    break;
                case ReturnK:
                    break;
                case BreakK:
                    break;
                case RangeK:
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
                if (strcmp(syntaxTree->attr.name, "AND") == 0)
                {
                    // bool,bool=bool
                    if (syntaxTree->isArray)
                    {
                        printf("ERROR(%d): The operation 'and' does not work with arrays.\n", syntaxTree->lineno);
                        numErrors++;
                    }
                    else
                    {
                        //LHS check
                        void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                        if (!data)
                        {
                            printf("FIX THIS\n");
                            numErrors++;
                        }
                        else
                        {
                            syntaxTree->child[0]->declType = static_cast<TreeNode *>(data)->declType;
                        }
                        switch (syntaxTree->child[0]->declType)
                        {
                        case DeclBoolean:
                            syntaxTree->expType = Boolean;
                            syntaxTree->declType = DeclBoolean;
                            break;
                        case DeclVoid:
                            printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type void.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case DeclInteger:
                            printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type int.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case DeclChar:
                            printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type char.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case DeclCharInt:
                            printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type charint.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case DeclEqual:
                            printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type equal.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case DeclUndefinedType:
                            printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type UndefinedType.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        default:
                            printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                            break;
                        }
                        //RHS check
                        switch (syntaxTree->child[1]->nodekind)
                        {
                        case DeclK:
                        {
                            void *data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                            if (!data)
                            {
                                printf("FIX THIS\n");
                                numErrors++;
                            }
                            else
                            {
                                syntaxTree->child[1]->declType = static_cast<TreeNode *>(data)->declType;
                            }
                            switch (syntaxTree->child[1]->declType)
                            {
                            case DeclBoolean:
                                syntaxTree->expType = Boolean;
                                syntaxTree->declType = DeclBoolean;
                                break;
                            case DeclVoid:
                                printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type void.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case DeclInteger:
                                printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type int.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case DeclChar:
                                printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type char.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case DeclCharInt:
                                printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type charint.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case DeclEqual:
                                printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type equal.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case DeclUndefinedType:
                                printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type UndefinedType.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            default:
                                printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                                break;
                            }
                            break;
                        }
                        case ExpK:
                            switch (syntaxTree->child[1]->expType)
                            {
                            case Boolean:
                                syntaxTree->expType = Boolean;
                                syntaxTree->declType = DeclBoolean;
                                break;
                            case Void:
                                printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type void.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case Integer:
                                printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type int.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case Char:
                                printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type char.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case CharInt:
                                printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type charint.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case Equal:
                                printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type equal.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case UndefinedType:
                                printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type UndefinedType.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            default:
                                printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                                break;
                            }
                            break;
                        default:
                            printf("SHOULD NOT BE HERE (AND)\n");
                            break;
                        }
                    }
                }
                else if (strcmp(syntaxTree->attr.name, "OR") == 0)
                {
                    //bool,bool=bool
                    //LHS check
                    if (syntaxTree->isArray)
                    {
                        printf("ERROR(%d): The operation 'or' does not work with arrays.\n", syntaxTree->lineno);
                        numErrors++;
                    }
                    else
                    {
                        //LHS check
                        void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                        if (!data)
                        {
                            printf("FIX THIS\n");
                            numErrors++;
                        }
                        else
                        {
                            syntaxTree->child[0]->declType = static_cast<TreeNode *>(data)->declType;
                        }
                        switch (syntaxTree->child[0]->declType)
                        {
                        case DeclBoolean:
                            syntaxTree->expType = Boolean;
                            syntaxTree->declType = DeclBoolean;
                            break;
                        case DeclVoid:
                            printf("ERROR(%d): 'or' requires operands of type bool but lhs is of type void.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case DeclInteger:
                            printf("ERROR(%d): 'or' requires operands of type bool but lhs is of type int.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case DeclChar:
                            printf("ERROR(%d): 'or' requires operands of type bool but lhs is of type char.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case DeclCharInt:
                            printf("ERROR(%d): 'or' requires operands of type bool but lhs is of type charint.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case DeclEqual:
                            printf("ERROR(%d): 'or' requires operands of type bool but lhs is of type equal.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case DeclUndefinedType:
                            printf("ERROR(%d): 'or' requires operands of type bool but lhs is of type UndefinedType.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        default:
                            printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                            break;
                        }
                        //RHS check
                        switch (syntaxTree->child[1]->nodekind)
                        {
                        case DeclK:
                        {
                            void *data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                            if (!data)
                            {
                                printf("FIX THIS\n");
                                numErrors++;
                            }
                            else
                            {
                                syntaxTree->child[1]->declType = static_cast<TreeNode *>(data)->declType;
                            }
                            switch (syntaxTree->child[1]->declType)
                            {
                            case DeclBoolean:
                                syntaxTree->expType = Boolean;
                                syntaxTree->declType = DeclBoolean;
                                break;
                            case DeclVoid:
                                printf("ERROR(%d): 'or' requires operands of type bool but lhs is of type void.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case DeclInteger:
                                printf("ERROR(%d): 'or' requires operands of type bool but lhs is of type int.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case DeclChar:
                                printf("ERROR(%d): 'or' requires operands of type bool but lhs is of type char.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case DeclCharInt:
                                printf("ERROR(%d): 'or' requires operands of type bool but lhs is of type charint.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case DeclEqual:
                                printf("ERROR(%d): 'or' requires operands of type bool but lhs is of type equal.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case DeclUndefinedType:
                                printf("ERROR(%d): 'or' requires operands of type bool but lhs is of type UndefinedType.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            default:
                                printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                                break;
                            }
                            break;
                        }
                        case ExpK:
                            switch (syntaxTree->child[1]->expType)
                            {
                            case Boolean:
                                syntaxTree->expType = Boolean;
                                syntaxTree->declType = DeclBoolean;
                                break;
                            case Void:
                                printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type void.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case Integer:
                                printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type int.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case Char:
                                printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type char.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case CharInt:
                                printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type charint.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case Equal:
                                printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type equal.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case UndefinedType:
                                printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type UndefinedType.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            default:
                                printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                                break;
                            }
                            break;
                        default:
                            printf("SHOULD NOT BE HERE (AND)\n");
                            break;
                        }
                    }
                }
                ///////////////////
                //   "="
                //
                else if (strcmp(syntaxTree->attr.name, "==") == 0)
                {
                    //equal types + arrays = bool
                    bool twoArrays = false;
                    if (syntaxTree->child[0]->isArray && syntaxTree->child[1]->isArray)
                    {
                        twoArrays = true;
                    }
                    else if (syntaxTree->child[0]->isArray && !syntaxTree->child[1]->isArray)
                    {
                        printf("ERROR(%d): '==' requires both operands be arrays or not but lhs is an array and rhs is not an array.\n", syntaxTree->lineno);
                        numErrors++;
                    }
                    else if ((!syntaxTree->child[0]->isArray && syntaxTree->child[1]->isArray))
                    {
                        printf("ERROR(%d): '==' requires both operands be arrays or not but lhs is not an array and rhs is an array.\n", syntaxTree->lineno);
                        numErrors++;
                    }
                    else if ((!syntaxTree->child[0]->isArray && !syntaxTree->child[1]->isArray) || twoArrays)
                    {   
                        if (0)//isEqual(syntaxTree->child[0], syntaxTree->child[1])) //->declType == ->expType)
                        {
                            syntaxTree->declType = DeclBoolean;
                            syntaxTree->expType = Boolean;
                        }
                        else
                        {
                            //PRINT LHS
                            printf("ERROR(%d): '==' requires operands of the same type but lhs is type ", syntaxTree->lineno);
                            numErrors++;
                            if (syntaxTree->child[0]->attr.value > -10000)
                            {
                                switch (syntaxTree->child[0]->expType)
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
                                    printf("charint");
                                    break;
                                case Equal:
                                    printf("equal");
                                    break;
                                case UndefinedType:
                                    printf("undefined");
                                    break;
                                default:
                                    printf("ERROR: semantic.cpp line 357\n");
                                    break;
                                }
                            }
                            else
                            {
                                void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                                if (!data)
                                {
                                    printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->attr.name);
                                    numErrors++;
                                }
                                else
                                {
                                    syntaxTree->child[0]->declType = static_cast<TreeNode *>(data)->declType;
                                    switch (syntaxTree->child[0]->declType)
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
                                        printf("charint");
                                        break;
                                    case DeclEqual:
                                        printf("equal");
                                        break;
                                    case DeclUndefinedType:
                                        printf("undefined");
                                        break;
                                    default:
                                        printf("ERROR: semantic.cpp line 322\n");
                                        break;
                                    }
                                }
                            }
                            printf(" and rhs is type ");
                            //PRINT RHS
                            if (syntaxTree->child[1]->attr.value > -10000)
                            {
                                switch (syntaxTree->child[1]->expType)
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
                                    printf("charint");
                                    break;
                                case Equal:
                                    printf("equal");
                                    break;
                                case UndefinedType:
                                    printf("undefined");
                                    break;
                                default:
                                    printf("ERROR: semantic.cpp line 357\n");
                                    break;
                                }
                            }
                            else
                            {
                                void *data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                                if (!data)
                                {
                                    printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->attr.name);
                                    numErrors++;
                                }
                                else
                                {
                                    syntaxTree->child[1]->declType = static_cast<TreeNode *>(data)->declType;
                                    switch (syntaxTree->child[1]->declType)
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
                                        printf("charint");
                                        break;
                                    case DeclEqual:
                                        printf("equal");
                                        break;
                                    case DeclUndefinedType:
                                        printf("undefined");
                                        break;
                                    default:
                                        printf("ERROR: semantic.cpp line 322\n");
                                        break;
                                    }
                                }
                            }
                            printf(".\n");
                        }
                    }
                }
                ///////////////////
                //   "!="
                //
                else if (strcmp(syntaxTree->attr.name, "!=") == 0)
                {
                    //equal types + arrays = bool
                    bool twoArrays = false;
                    if (syntaxTree->child[0]->isArray && syntaxTree->child[1]->isArray)
                    {
                        twoArrays = true;
                    }
                    else if (syntaxTree->child[0]->isArray && !syntaxTree->child[1]->isArray)
                    {
                        printf("ERROR(%d): '!=' requires both operands be arrays or not but lhs is an array and rhs is not an array.\n", syntaxTree->lineno);
                        numErrors++;
                    }
                    else if ((!syntaxTree->child[0]->isArray && syntaxTree->child[1]->isArray))
                    {
                        printf("ERROR(%d): '!=' requires both operands be arrays or not but lhs is not an array and rhs is an array.\n", syntaxTree->lineno);
                        numErrors++;
                    }
                    else if ((!syntaxTree->child[0]->isArray && !syntaxTree->child[1]->isArray) || twoArrays)
                    {
                        if (syntaxTree->child[0]->expType == syntaxTree->child[1]->expType)
                        {
                            syntaxTree->expType = Boolean;
                        }
                        else
                        {
                            printf("ERROR(%d): '!=' requires operands of the same type but lhs is ", syntaxTree->lineno);
                            numErrors++;
                            switch (syntaxTree->child[0]->expType)
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
                                printf("charint");
                                break;
                            case Equal:
                                printf("equal");
                                break;
                            case UndefinedType:
                                printf("undefined");
                                break;
                            default:
                                printf("ERROR: semantic.cpp line 410\n");
                                break;
                            }
                            printf(" and rhs is ");
                            switch (syntaxTree->child[1]->expType)
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
                                printf("charint");
                                break;
                            case Equal:
                                printf("equal");
                                break;
                            case UndefinedType:
                                printf("undefined");
                                break;
                            default:
                                printf("ERROR: semantic.cpp line 438\n");
                                break;
                            }
                            printf(".\n");
                        }
                    }
                }
                else if (strcmp(syntaxTree->attr.name, "<=") == 0)
                {
                    //equal types + arrays = bool
                    bool twoArrays = false;
                    if (syntaxTree->child[0]->isArray && syntaxTree->child[1]->isArray)
                    {
                        twoArrays = true;
                    }
                    else if (syntaxTree->child[0]->isArray && !syntaxTree->child[1]->isArray)
                    {
                        printf("ERROR(%d): '<=' requires both operands be arrays or not but lhs is an array and rhs is not an array.\n", syntaxTree->lineno);
                        numErrors++;
                    }
                    else if ((!syntaxTree->child[0]->isArray && syntaxTree->child[1]->isArray))
                    {
                        printf("ERROR(%d): '<=' requires both operands be arrays or not but lhs is not an array and rhs is an array.\n", syntaxTree->lineno);
                        numErrors++;
                    }
                    else if ((!syntaxTree->child[0]->isArray && !syntaxTree->child[1]->isArray) || twoArrays)
                    {
                        void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                        if (!data)
                        {
                            printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                        }
                        else
                        {
                            syntaxTree->child[0]->declType = static_cast<TreeNode *>(data)->declType;
                        }
                        if (syntaxTree->child[0]->declType == syntaxTree->child[1]->expType)
                        {
                            syntaxTree->declType = DeclBoolean;
                        }
                        else
                        {
                            printf("ERROR(%d): '<=' requires operands of the same type but lhs is type ", syntaxTree->lineno);
                            numErrors++;
                            switch (syntaxTree->child[0]->declType)
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
                                printf("charint");
                                break;
                            case DeclEqual:
                                printf("equal");
                                break;
                            case DeclUndefinedType:
                                printf("undefined");
                                break;
                            default:
                                printf("ERROR: semantic.cpp line 500\n");
                                break;
                            }
                            printf(" and rhs is type ");
                            switch (syntaxTree->child[1]->expType)
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
                                printf("charint");
                                break;
                            case Equal:
                                printf("equal");
                                break;
                            case UndefinedType:
                                printf("undefined");
                                break;
                            default:
                                printf("ERROR: semantic.cpp line 528\n");
                                break;
                            }
                            printf(".\n");
                        }
                    }
                }
                else if (strcmp(syntaxTree->attr.name, "<") == 0)
                {
                    //equal types + arrays = bool
                    bool twoArrays = false;
                    if (syntaxTree->child[0]->isArray && syntaxTree->child[1]->isArray)
                    {
                        twoArrays = true;
                    }
                    else if (syntaxTree->child[0]->isArray && !syntaxTree->child[1]->isArray)
                    {
                        printf("ERROR(%d): '<' requires both operands be arrays or not but lhs is an array and rhs is not an array.\n", syntaxTree->lineno);
                        numErrors++;
                    }
                    else if ((!syntaxTree->child[0]->isArray && syntaxTree->child[1]->isArray))
                    {
                        printf("ERROR(%d): '<' requires both operands be arrays or not but lhs is not an array and rhs is an array.\n", syntaxTree->lineno);
                        numErrors++;
                    }
                    else if ((!syntaxTree->child[0]->isArray && !syntaxTree->child[1]->isArray) || twoArrays)
                    {
                        void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                        if (!data)
                        {
                            printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                        }
                        else
                        {
                            syntaxTree->child[0]->declType = static_cast<TreeNode *>(data)->declType;
                        }
                        if (syntaxTree->child[0]->declType == syntaxTree->child[1]->declType)
                        {
                            syntaxTree->declType = DeclBoolean;
                        }
                        else
                        {
                            printf("ERROR(%d): '<' requires operands of the same type but lhs is type ", syntaxTree->lineno);
                            numErrors++;
                            switch (syntaxTree->child[0]->declType)
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
                                printf("charint");
                                break;
                            case DeclEqual:
                                printf("equal");
                                break;
                            case DeclUndefinedType:
                                printf("undefined");
                                break;
                            default:
                                printf("ERROR: semantic.cpp line 584\n");
                                break;
                            }
                            printf(" and rhs is type ");
                            switch (syntaxTree->child[1]->expType)
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
                                printf("charint");
                                break;
                            case Equal:
                                printf("equal");
                                break;
                            case UndefinedType:
                                printf("undefined");
                                break;
                            default:
                                printf("ERROR: semantic.cpp line 612\n");
                                break;
                            }
                            printf(".\n");
                        }
                    }
                }
                else if (strcmp(syntaxTree->attr.name, ">=") == 0)
                {
                    //equal types + arrays = bool
                    bool twoArrays = false;
                    if (syntaxTree->child[0]->isArray && syntaxTree->child[1]->isArray)
                    {
                        twoArrays = true;
                    }
                    else if (syntaxTree->child[0]->isArray && !syntaxTree->child[1]->isArray)
                    {
                        printf("ERROR(%d): '>=' requires both operands be arrays or not but lhs is an array and rhs is not an array.\n", syntaxTree->lineno);
                        numErrors++;
                    }
                    else if ((!syntaxTree->child[0]->isArray && syntaxTree->child[1]->isArray))
                    {
                        printf("ERROR(%d): '>=' requires both operands be arrays or not but lhs is not an array and rhs is an array.\n", syntaxTree->lineno);
                        numErrors++;
                    }
                    else if ((!syntaxTree->child[0]->isArray && !syntaxTree->child[1]->isArray) || twoArrays)
                    {
                        void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                        if (!data)
                        {
                            printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                        }
                        else
                        {
                            syntaxTree->child[0]->declType = static_cast<TreeNode *>(data)->declType;
                        }
                        if (syntaxTree->child[0]->declType == syntaxTree->child[1]->expType)
                        {
                            syntaxTree->declType = DeclBoolean;
                        }
                        else
                        {
                            printf("ERROR(%d): '>=' requires operands of the same type but lhs is type ", syntaxTree->lineno);
                            numErrors++;
                            switch (syntaxTree->child[0]->declType)
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
                                printf("charint");
                                break;
                            case DeclEqual:
                                printf("equal");
                                break;
                            case DeclUndefinedType:
                                printf("undefined");
                                break;
                            default:
                                printf("ERROR: semantic.cpp line 668\n");
                                break;
                            }
                            printf(" and rhs is type ");
                            switch (syntaxTree->child[1]->expType)
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
                                printf("charint");
                                break;
                            case Equal:
                                printf("equal");
                                break;
                            case UndefinedType:
                                printf("undefined");
                                break;
                            default:
                                printf("ERROR: semantic.cpp line 696\n");
                                break;
                            }
                            printf(".\n");
                        }
                    }
                }
                else if (strcmp(syntaxTree->attr.name, ">") == 0)
                {
                    //equal types + arrays = bool
                    bool twoArrays = false;
                    if (syntaxTree->child[0]->isArray && syntaxTree->child[1]->isArray)
                    {
                        twoArrays = true;
                    }
                    else if (syntaxTree->child[0]->isArray && !syntaxTree->child[1]->isArray)
                    {
                        printf("ERROR(%d): '>' requires both operands be arrays or not but lhs is an array and rhs is not an array.\n", syntaxTree->lineno);
                        numErrors++;
                    }
                    else if ((!syntaxTree->child[0]->isArray && syntaxTree->child[1]->isArray))
                    {
                        printf("ERROR(%d): '>' requires both operands be arrays or not but lhs is not an array and rhs is an array.\n", syntaxTree->lineno);
                        numErrors++;
                    }
                    else if ((!syntaxTree->child[0]->isArray && !syntaxTree->child[1]->isArray) || twoArrays)
                    {
                        void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                        if (!data)
                        {
                            printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                        }
                        else
                        {
                            syntaxTree->child[0]->declType = static_cast<TreeNode *>(data)->declType;
                        }
                        if (syntaxTree->child[0]->declType == syntaxTree->child[1]->declType)
                        {
                            syntaxTree->declType = DeclBoolean;
                        }
                        else
                        {
                            printf("ERROR(%d): '>' requires operands of the same type but lhs is type ", syntaxTree->lineno);
                            numErrors++;
                            switch (syntaxTree->child[0]->declType)
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
                                printf("charint");
                                break;
                            case DeclEqual:
                                printf("equal");
                                break;
                            case DeclUndefinedType:
                                printf("undefined");
                                break;
                            default:
                                printf("ERROR: semantic.cpp line 752\n");
                                break;
                            }
                            printf(" and rhs is type ");
                            switch (syntaxTree->child[1]->expType)
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
                                printf("charint");
                                break;
                            case Equal:
                                printf("equal");
                                break;
                            case UndefinedType:
                                printf("undefined");
                                break;
                            default:
                                printf("ERROR: semantic.cpp line 780\n");
                                break;
                            }
                            printf(".\n");
                        }
                    }
                }
                else if (strcmp(syntaxTree->attr.name, ":>:") == 0)
                {
                    //int,int=int
                    //LHS check
                    switch (syntaxTree->child[0]->expType)
                    {
                    case Boolean:
                        printf("ERROR(%d): '%s' requires operands of type int but lhs is of type bool.\n", syntaxTree->lineno, syntaxTree->attr.name);
                        numErrors++;
                        break;
                    case Void:
                        printf("ERROR(%d): '%s' requires operands of type int but lhs is of type void.\n", syntaxTree->lineno, syntaxTree->attr.name);
                        numErrors++;
                        break;
                    case Integer:
                        syntaxTree->expType = Integer;
                        break;
                    case Char:
                        printf("ERROR(%d): '%s' requires operands of type int but lhs is of type char.\n", syntaxTree->lineno, syntaxTree->attr.name);
                        numErrors++;
                        break;
                    case CharInt:
                        printf("ERROR(%d): '%s' requires operands of type int but lhs is of type charint.\n", syntaxTree->lineno, syntaxTree->attr.name);
                        numErrors++;
                        break;
                    case Equal:
                        printf("ERROR(%d): '%s' requires operands of type int but lhs is of type equal.\n", syntaxTree->lineno, syntaxTree->attr.name);
                        numErrors++;
                        break;
                    case UndefinedType:
                        printf("ERROR(%d): '%s' requires operands of type int but lhs is of type UndefinedType.\n", syntaxTree->lineno, syntaxTree->attr.name);
                        numErrors++;
                        break;
                    default:
                        printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                        break;
                    }
                    //RHS check
                    switch (syntaxTree->child[1]->expType)
                    {
                    case Boolean:
                        printf("ERROR(%d): '%s' requires operands of type int but rhs is of type bool.\n", syntaxTree->lineno, syntaxTree->attr.name);
                        numErrors++;
                        break;
                    case Void:
                        printf("ERROR(%d): '%s' requires operands of type int but rhs is of type void.\n", syntaxTree->lineno, syntaxTree->attr.name);
                        numErrors++;
                        break;
                    case Integer:
                        syntaxTree->expType = Integer;
                        break;
                    case Char:
                        printf("ERROR(%d): '%s' requires operands of type int but rhs is of type char.\n", syntaxTree->lineno, syntaxTree->attr.name);
                        numErrors++;
                        break;
                    case CharInt:
                        printf("ERROR(%d): '%s' requires operands of type int but rhs is of type charint.\n", syntaxTree->lineno, syntaxTree->attr.name);
                        numErrors++;
                        break;
                    case Equal:
                        printf("ERROR(%d): '%s' requires operands of type int but rhs is of type equal.\n", syntaxTree->lineno, syntaxTree->attr.name);
                        numErrors++;
                        break;
                    case UndefinedType:
                        printf("ERROR(%d): '%s' requires operands of type int but rhs is of type UndefinedType.\n", syntaxTree->lineno, syntaxTree->attr.name);
                        numErrors++;
                        break;
                    default:
                        printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                        break;
                    }
                }
                else if (strcmp(syntaxTree->attr.name, ":<:") == 0)
                {
                    //int,int=int
                    //LHS check
                    switch (syntaxTree->child[0]->expType)
                    {
                    case Boolean:
                        printf("ERROR(%d): '%s' requires operands of type int but lhs is of type bool.\n", syntaxTree->lineno, syntaxTree->attr.name);
                        numErrors++;
                        break;
                    case Void:
                        printf("ERROR(%d): '%s' requires operands of type int but lhs is of type void.\n", syntaxTree->lineno, syntaxTree->attr.name);
                        numErrors++;
                        break;
                    case Integer:
                        syntaxTree->expType = Integer;
                        break;
                    case Char:
                        printf("ERROR(%d): '%s' requires operands of type int but lhs is of type char.\n", syntaxTree->lineno, syntaxTree->attr.name);
                        numErrors++;
                        break;
                    case CharInt:
                        printf("ERROR(%d): '%s' requires operands of type int but lhs is of type charint.\n", syntaxTree->lineno, syntaxTree->attr.name);
                        numErrors++;
                        break;
                    case Equal:
                        printf("ERROR(%d): '%s' requires operands of type int but lhs is of type equal.\n", syntaxTree->lineno, syntaxTree->attr.name);
                        numErrors++;
                        break;
                    case UndefinedType:
                        printf("ERROR(%d): '%s' requires operands of type int but lhs is of type UndefinedType.\n", syntaxTree->lineno, syntaxTree->attr.name);
                        numErrors++;
                        break;
                    default:
                        printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                        break;
                    }
                    //RHS check
                    switch (syntaxTree->child[1]->expType)
                    {
                    case Boolean:
                        printf("ERROR(%d): '%s' requires operands of type int but rhs is of type bool.\n", syntaxTree->lineno, syntaxTree->attr.name);
                        numErrors++;
                        break;
                    case Void:
                        printf("ERROR(%d): '%s' requires operands of type int but rhs is of type void.\n", syntaxTree->lineno, syntaxTree->attr.name);
                        numErrors++;
                        break;
                    case Integer:
                        syntaxTree->expType = Integer;
                        break;
                    case Char:
                        printf("ERROR(%d): '%s' requires operands of type int but rhs is of type char.\n", syntaxTree->lineno, syntaxTree->attr.name);
                        numErrors++;
                        break;
                    case CharInt:
                        printf("ERROR(%d): '%s' requires operands of type int but rhs is of type charint.\n", syntaxTree->lineno, syntaxTree->attr.name);
                        numErrors++;
                        break;
                    case Equal:
                        printf("ERROR(%d): '%s' requires operands of type int but rhs is of type equal.\n", syntaxTree->lineno, syntaxTree->attr.name);
                        numErrors++;
                        break;
                    case UndefinedType:
                        printf("ERROR(%d): '%s' requires operands of type int but rhs is of type UndefinedType.\n", syntaxTree->lineno, syntaxTree->attr.name);
                        numErrors++;
                        break;
                    default:
                        printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                        break;
                    }
                }
                else if (strcmp(syntaxTree->attr.name, "*") == 0)
                {
                    //int,int=int
                    //LHS check
                    void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                    if (!data)
                    {
                        printf("fix if needed\n");
                        numErrors++;
                    }
                    else
                    {
                        syntaxTree->child[0]->declType = static_cast<TreeNode *>(data)->declType;
                        switch (static_cast<TreeNode *>(data)->declType)
                        {
                        case DeclVoid:
                            syntaxTree->child[0]->expType = Void;
                            break;
                        case DeclInteger:
                            syntaxTree->child[0]->expType = Integer;
                            break;
                        case DeclChar:
                            syntaxTree->child[0]->expType = Char;
                            break;
                        case DeclBoolean:
                            syntaxTree->child[0]->expType = Boolean;
                            break;
                        default:
                            printf("ADD MORE\n");
                            break;
                        }
                    }
                    switch (syntaxTree->child[0]->expType)
                    {
                    case Boolean:
                        printf("ERROR(%d): '*' requires operands of type int but rhs is of type bool.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case Void:
                        printf("ERROR(%d): '*' requires operands of type int but rhs is of type void.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case Integer:
                        syntaxTree->expType = Integer;
                        break;
                    case Char:
                        printf("ERROR(%d): '*' requires operands of type int but rhs is of type char.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case CharInt:
                        printf("ERROR(%d): '*' requires operands of type int but rhs is of type charint.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case Equal:
                        printf("ERROR(%d): '*' requires operands of type int but rhs is of type equal.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case UndefinedType:
                        printf("ERROR(%d): '*' requires operands of type int but rhs is of type UndefinedType.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    default:
                        printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                        break;
                    }
                    //RHS check
                    if (syntaxTree->child[1]->attr.value != NULL) // constant check
                    {
                        switch (syntaxTree->child[1]->expType)
                        {
                        case Boolean:
                            printf("ERROR(%d): '*' requires operands of type int but rhs is of type bool.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Void:
                            printf("ERROR(%d): '*' requires operands of type int but rhs is of type void.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Integer:
                            syntaxTree->expType = Integer;
                            break;
                        case Char:
                            printf("ERROR(%d): '*' requires operands of type int but rhs is of type char.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case CharInt:
                            printf("ERROR(%d): '*' requires operands of type int but rhs is of type charint.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Equal:
                            printf("ERROR(%d): '*' requires operands of type int but rhs is of type equal.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case UndefinedType:
                            printf("ERROR(%d): '*' requires operands of type int but rhs is of type UndefinedType.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        default:
                            printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                            break;
                        }
                    }
                    else // is a decl var
                    {
                        data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                        if (!data)
                        {
                            printf("fix if needed\n");
                            numErrors++;
                        }
                        else
                        {
                            syntaxTree->child[1]->declType = static_cast<TreeNode *>(data)->declType;
                            switch (static_cast<TreeNode *>(data)->declType)
                            {
                            case DeclVoid:
                                syntaxTree->child[1]->expType = Void;
                                break;
                            case DeclInteger:
                                syntaxTree->child[1]->expType = Integer;
                                break;
                            case DeclChar:
                                syntaxTree->child[1]->expType = Char;
                                break;
                            case DeclBoolean:
                                syntaxTree->child[1]->expType = Boolean;
                                break;
                            default:
                                printf("ADD MORE\n");
                                break;
                            }
                        }
                    }
                }
                else if (strcmp(syntaxTree->attr.name, "+") == 0)
                {
                    //int,int=int
                    //LHS check
                    void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                    if (!data)
                    {
                        switch (syntaxTree->child[0]->expType)
                        {
                        case Boolean:
                            printf("ERROR(%d): '%s' requires operands of type int but lhs is of type bool.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case Void:
                            printf("ERROR(%d): '%s' requires operands of type int but lhs is of type void.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case Integer:
                            syntaxTree->expType = Integer;
                            break;
                        case Char:
                            printf("ERROR(%d): '%s' requires operands of type int but lhs is of type char.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case CharInt:
                            printf("ERROR(%d): '%s' requires operands of type int but lhs is of type charint.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case Equal:
                            printf("ERROR(%d): '%s' requires operands of type int but lhs is of type equal.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case UndefinedType:
                            printf("ERROR(%d): '%s' requires operands of type int but lhs is of type UndefinedType.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        default:
                            printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                            break;
                        }
                    }
                    else
                    {
                        syntaxTree->child[0]->declType = static_cast<TreeNode *>(data)->declType;
                        switch (syntaxTree->child[0]->declType)
                        {
                        case DeclBoolean:
                            printf("ERROR(%d): '+' requires operands of type int but lhs is of type bool.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case DeclVoid:
                            printf("ERROR(%d): '+' requires operands of type int but lhs is of type void.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case DeclInteger:
                            syntaxTree->expType = Integer;
                            break;
                        case DeclChar:
                            printf("ERROR(%d): '+' requires operands of type int but lhs is of type char.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case DeclCharInt:
                            printf("ERROR(%d): '+' requires operands of type int but lhs is of type charint.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case DeclEqual:
                            printf("ERROR(%d): '+' requires operands of type int but lhs is of type equal.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case DeclUndefinedType:
                            printf("ERROR(%d): '+' requires operands of type int but lhs is of type UndefinedType.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        default:
                            printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                            break;
                        }
                    }
                    //RHS check
                    data = NULL;
                    if (syntaxTree->child[1]->nodekind == DeclK)
                    {
                        data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                    }
                    if (!data)
                    {
                        switch (syntaxTree->child[1]->expType)
                        {
                        case Boolean:
                            printf("ERROR(%d): '%s' requires operands of type int but lhs is of type bool.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case Void:
                            printf("ERROR(%d): '%s' requires operands of type int but lhs is of type void.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case Integer:
                            syntaxTree->expType = Integer;
                            break;
                        case Char:
                            printf("ERROR(%d): '%s' requires operands of type int but lhs is of type char.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case CharInt:
                            printf("ERROR(%d): '%s' requires operands of type int but lhs is of type charint.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case Equal:
                            printf("ERROR(%d): '%s' requires operands of type int but lhs is of type equal.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case UndefinedType:
                            printf("ERROR(%d): '%s' requires operands of type int but lhs is of type UndefinedType.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        default:
                            printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                            break;
                        }
                    }
                    else
                    {
                        syntaxTree->child[1]->declType = static_cast<TreeNode *>(data)->declType;
                        switch (syntaxTree->child[1]->declType)
                        {
                        case DeclBoolean:
                            printf("ERROR(%d): '+' requires operands of type int but lhs is of type bool.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case DeclVoid:
                            printf("ERROR(%d): '+' requires operands of type int but lhs is of type void.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case DeclInteger:
                            syntaxTree->expType = Integer;
                            break;
                        case DeclChar:
                            printf("ERROR(%d): '+' requires operands of type int but lhs is of type char.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case DeclCharInt:
                            printf("ERROR(%d): '+' requires operands of type int but lhs is of type charint.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case DeclEqual:
                            printf("ERROR(%d): '+' requires operands of type int but lhs is of type equal.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case DeclUndefinedType:
                            printf("ERROR(%d): '+' requires operands of type int but lhs is of type UndefinedType.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        default:
                            printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                            break;
                        }
                    }
                }
                else if (strcmp(syntaxTree->attr.name, "-") == 0)
                {
                    //int,int=int
                    //LHS check
                    switch (syntaxTree->child[0]->expType)
                    {
                    case Boolean:
                        printf("ERROR(%d): '-' requires operands of type int but lhs is of type bool.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case Void:
                        printf("ERROR(%d): '-' requires operands of type int but lhs is of type void.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case Integer:
                        syntaxTree->expType = Integer;
                        break;
                    case Char:
                        printf("ERROR(%d): '-' requires operands of type int but lhs is of type char.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case CharInt:
                        printf("ERROR(%d): '-' requires operands of type int but lhs is of type charint.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case Equal:
                        printf("ERROR(%d): '-' requires operands of type int but lhs is of type equal.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case UndefinedType:
                        printf("ERROR(%d): '-' requires operands of type int but lhs is of type UndefinedType.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    default:
                        printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                        break;
                    }
                    //RHS check
                    void *data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                    if (!data)
                    {
                        printf("fix if needed\n");
                        numErrors++;
                    }
                    else
                    {
                        syntaxTree->child[1]->declType = static_cast<TreeNode *>(data)->declType;
                        switch (static_cast<TreeNode *>(data)->declType)
                        {
                        case DeclVoid:
                            syntaxTree->child[1]->expType = Void;
                            break;
                        case DeclInteger:
                            syntaxTree->child[1]->expType = Integer;
                            break;
                        case DeclChar:
                            syntaxTree->child[1]->expType = Char;
                            break;
                        case DeclBoolean:
                            syntaxTree->child[1]->expType = Boolean;
                            break;
                        default:
                            printf("ADD MORE\n");
                            break;
                        }
                    }
                    switch (syntaxTree->child[1]->expType)
                    {
                    case Boolean:
                        printf("ERROR(%d): '-' requires operands of type int but rhs is of type bool.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case Void:
                        printf("ERROR(%d): '-' requires operands of type int but rhs is of type void.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case Integer:
                        syntaxTree->expType = Integer;
                        break;
                    case Char:
                        printf("ERROR(%d): '-' requires operands of type int but rhs is of type char.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case CharInt:
                        printf("ERROR(%d): '-' requires operands of type int but rhs is of type charint.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case Equal:
                        printf("ERROR(%d): '-' requires operands of type int but rhs is of type equal.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case UndefinedType:
                        printf("ERROR(%d): '-' requires operands of type int but rhs is of type UndefinedType.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    default:
                        printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                        break;
                    }
                }
                ////////////////////
                //         "/"
                //
                else if (strcmp(syntaxTree->attr.name, "/") == 0)
                {
                    //int,int=int
                    //LHS check
                    if (syntaxTree->child[0]->attr.value > -10000) //constant check
                    {
                        switch (syntaxTree->child[0]->expType)
                        {
                        case Boolean:
                            printf("ERROR(%d): '/' requires operands of type int but lhs is of type bool.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Void:
                            printf("ERROR(%d): '/' requires operands of type int but lhs is of type void.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Integer:
                            syntaxTree->expType = Integer;
                            break;
                        case Char:
                            printf("ERROR(%d): '/' requires operands of type int but lhs is of type char.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case CharInt:
                            printf("ERROR(%d): '/' requires operands of type int but lhs is of type charint.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Equal:
                            printf("ERROR(%d): '/' requires operands of type int but lhs is of type equal.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case UndefinedType:
                            printf("ERROR(%d): '/' requires operands of type int but lhs is of type UndefinedType.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        default:
                            printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                            break;
                        }
                    }
                    else //decl var
                    {
                        void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                        if (!data)
                        {
                            printf("fix if needed\n");
                            numErrors++;
                        }
                        else
                        {
                            syntaxTree->child[0]->declType = static_cast<TreeNode *>(data)->declType;
                            switch (static_cast<TreeNode *>(data)->declType)
                            {
                            case DeclVoid:
                                printf("ERROR(%d): '/' requires operands of type int but lhs is of type void.\n", syntaxTree->lineno);
                                break;
                            case DeclInteger:
                                syntaxTree->child[0]->expType = Integer;
                                break;
                            case DeclChar:
                                printf("ERROR(%d): '/' requires operands of type int but lhs is of type char.\n", syntaxTree->lineno);
                                break;
                            case DeclBoolean:
                                printf("ERROR(%d): '/' requires operands of type int but lhs is of type bool.\n", syntaxTree->lineno);
                                break;
                            default:
                                printf("ADD MORE\n");
                                break;
                            }
                        }
                    }
                    //RHS check
                    if (syntaxTree->child[1]->attr.value > -10000) //constant check
                    {
                        switch (syntaxTree->child[1]->expType)
                        {
                        case Boolean:
                            printf("ERROR(%d): '/' requires operands of type int but rhs is of type bool.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Void:
                            printf("ERROR(%d): '/' requires operands of type int but rhs is of type void.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Integer:
                            syntaxTree->expType = Integer;
                            break;
                        case Char:
                            printf("ERROR(%d): '/' requires operands of type int but rhs is of type char.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case CharInt:
                            printf("ERROR(%d): '/' requires operands of type int but rhs is of type charint.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Equal:
                            printf("ERROR(%d): '/' requires operands of type int but rhs is of type equal.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case UndefinedType:
                            printf("ERROR(%d): '/' requires operands of type int but rhs is of type UndefinedType.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        default:
                            printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                            break;
                        }
                    }
                    else //decl var
                    {
                        void *data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                        if (!data)
                        {
                            printf("fix if needed\n");
                            numErrors++;
                        }
                        else
                        {
                            syntaxTree->child[1]->declType = static_cast<TreeNode *>(data)->declType;
                            switch (static_cast<TreeNode *>(data)->declType)
                            {
                            case DeclVoid:
                                printf("ERROR(%d): '/' requires operands of type int but rhs is of type void.\n", syntaxTree->lineno);
                                break;
                            case DeclInteger:
                                syntaxTree->child[1]->expType = Integer;
                                break;
                            case DeclChar:
                                printf("ERROR(%d): '/' requires operands of type int but rhs is of type char.\n", syntaxTree->lineno);
                                break;
                            case DeclBoolean:
                                printf("ERROR(%d): '/' requires operands of type int but rhs is of type bool.\n", syntaxTree->lineno);
                                break;
                            default:
                                printf("ADD MORE\n");
                                break;
                            }
                        }
                    }
                }
                /////////////////
                //      "%"
                //
                else if (strcmp(syntaxTree->attr.name, "%") == 0)
                {
                    //int,int=int
                    if (syntaxTree->child[0]->attr.value > -10000) //constant check
                    {
                        switch (syntaxTree->child[0]->expType)
                        {
                        case Boolean:
                            printf("ERROR(%d): '%' requires operands of type int but lhs is of type bool.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Void:
                            printf("ERROR(%d): '%' requires operands of type int but lhs is of type void.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Integer:
                            syntaxTree->expType = Integer;
                            break;
                        case Char:
                            printf("ERROR(%d): '%' requires operands of type int but lhs is of type char.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case CharInt:
                            printf("ERROR(%d): '%' requires operands of type int but lhs is of type charint.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Equal:
                            printf("ERROR(%d): '%' requires operands of type int but lhs is of type equal.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case UndefinedType:
                            printf("ERROR(%d): '%' requires operands of type int but lhs is of type UndefinedType.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        default:
                            printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                            break;
                        }
                    }
                    else //decl var
                    {
                        void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                        if (!data)
                        {
                            printf("fix if needed\n");
                            numErrors++;
                        }
                        else
                        {
                            syntaxTree->child[0]->declType = static_cast<TreeNode *>(data)->declType;
                            switch (static_cast<TreeNode *>(data)->declType)
                            {
                            case DeclVoid:
                                printf("ERROR(%d): '%' requires operands of type int but lhs is of type void.\n", syntaxTree->lineno);
                                break;
                            case DeclInteger:
                                syntaxTree->child[0]->expType = Integer;
                                break;
                            case DeclChar:
                                printf("ERROR(%d): '%' requires operands of type int but lhs is of type char.\n", syntaxTree->lineno);
                                break;
                            case DeclBoolean:
                                printf("ERROR(%d): '%' requires operands of type int but lhs is of type bool.\n", syntaxTree->lineno);
                                break;
                            default:
                                printf("ADD MORE\n");
                                break;
                            }
                        }
                    }
                    //RHS check
                    if (syntaxTree->child[1]->attr.value > -10000) //constant check
                    {
                        switch (syntaxTree->child[1]->expType)
                        {
                        case Boolean:
                            printf("ERROR(%d): '%' requires operands of type int but rhs is of type bool.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Void:
                            printf("ERROR(%d): '%' requires operands of type int but rhs is of type void.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Integer:
                            syntaxTree->expType = Integer;
                            break;
                        case Char:
                            printf("ERROR(%d): '%' requires operands of type int but rhs is of type char.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case CharInt:
                            printf("ERROR(%d): '%' requires operands of type int but rhs is of type charint.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Equal:
                            printf("ERROR(%d): '%' requires operands of type int but rhs is of type equal.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case UndefinedType:
                            printf("ERROR(%d): '%' requires operands of type int but rhs is of type UndefinedType.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        default:
                            printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                            break;
                        }
                    }
                    else //decl var
                    {
                        void *data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                        if (!data)
                        {
                            printf("fix if needed\n");
                            numErrors++;
                        }
                        else
                        {
                            syntaxTree->child[1]->declType = static_cast<TreeNode *>(data)->declType;
                            switch (static_cast<TreeNode *>(data)->declType)
                            {
                            case DeclVoid:
                                printf("ERROR(%d): '%' requires operands of type int but rhs is of type void.\n", syntaxTree->lineno);
                                break;
                            case DeclInteger:
                                syntaxTree->child[1]->expType = Integer;
                                break;
                            case DeclChar:
                                printf("ERROR(%d): '%' requires operands of type int but rhs is of type char.\n", syntaxTree->lineno);
                                break;
                            case DeclBoolean:
                                printf("ERROR(%d): '%' requires operands of type int but rhs is of type bool.\n", syntaxTree->lineno);
                                break;
                            default:
                                printf("ADD MORE\n");
                                break;
                            }
                        }
                    }
                }
                else if (strcmp(syntaxTree->attr.name, "[") == 0)
                {
                    //array,int=type of lhs
                    if (!syntaxTree->child[0]->isArray)
                    {
                        numErrors++;
                        printf("ERROR(%d): Cannot index nonarray '%s'.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    }
                    else
                    {
                        switch (syntaxTree->child[1]->expType)
                        {
                        case Boolean:
                            printf("ERROR(%d): Array '%s' should be indexed by type int but got bool.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case Void:
                            printf("ERROR(%d): Array '%s' should be indexed by type int but got void.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case Integer:
                            syntaxTree->expType = Integer;
                            syntaxTree->isArray = true;
                            break;
                        case Char:
                            printf("ERROR(%d): Array '%s' should be indexed by type int but got char.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case CharInt:
                            printf("ERROR(%d): Array '%s' should be indexed by type int but got charint.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case Equal:
                            printf("ERROR(%d): Array '%s' should be indexed by type int but got equal.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case UndefinedType:
                            printf("ERROR(%d): Array '%s' should be indexed by type int but got undefinedtype.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        default:
                            printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                            break;
                        }
                    }
                }
                else if (strcmp(syntaxTree->attr.name, "NOT") == 0)
                {
                    //bool=bool
                    if (syntaxTree->isArray)
                    {
                        printf("ERROR(%d): The operation 'not' does not work with arrays.\n", syntaxTree->lineno);
                        numErrors++;
                    }
                    else
                    {
                        void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                        if (!data)
                        {
                            printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                        }
                        else
                        {
                            syntaxTree->child[0]->declType = static_cast<TreeNode *>(data)->declType;
                        }
                        switch (syntaxTree->child[0]->declType)
                        {
                        case DeclBoolean:
                            syntaxTree->declType = DeclBoolean;
                            break;
                        case DeclVoid:
                            printf("ERROR(%d): Unary 'not' requires an operand of type bool but was given type void.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case DeclInteger:
                            printf("ERROR(%d): Unary 'not' requires an operand of type bool but was given type int.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case DeclChar:
                            printf("ERROR(%d): Unary 'not' requires an operand of type bool but was given type char.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case DeclCharInt:
                            printf("ERROR(%d): Unary 'not' requires an operand of type bool but was given type charint.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case DeclEqual:
                            printf("ERROR(%d): Unary 'not' requires an operand of type bool but was given type equal.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case DeclUndefinedType:
                            printf("ERROR(%d): Unary 'not' requires an operand of type bool but was given type undefinedtype.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        default:
                            printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                            break;
                        }
                        break;
                    }
                }
                else if (strcmp(syntaxTree->attr.name, "SIZEOF") == 0)
                {
                    //array=int
                    printf("%s\n", syntaxTree->child[0]->attr.name);
                    if (!syntaxTree->isArray)
                    {
                        printf("ERROR(%d): The operation 'SIZEOF' only works with arrays.\n", syntaxTree->lineno);
                    }
                    else
                    {
                        syntaxTree->expType = Integer;
                    }
                }
                else if (strcmp(syntaxTree->attr.name, "CHSIGN") == 0)
                {
                    //int=int
                    if (syntaxTree->isArray)
                    {
                        printf("ERROR(%d): The operation 'chsign' does not work with arrays.\n", syntaxTree->lineno);
                        numErrors++;
                    }
                    else
                    {
                        if (syntaxTree->child[0]->attr.value > -10000) //constant check
                        {
                            switch (syntaxTree->child[0]->expType)
                            {
                            case Boolean:
                                printf("ERROR(%d): Unary 'chsign' requires an operand of type int but was given type bool.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case Void:
                                printf("ERROR(%d): Unary 'chsign' requires an operand of type int but was given type void.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case Integer:
                                syntaxTree->expType = Integer;
                                break;
                            case Char:
                                printf("ERROR(%d): Unary 'chsign' requires an operand of type int but was given type char.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case CharInt:
                                printf("ERROR(%d): Unary 'chsign' requires an operand of type int but was given type charint.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case Equal:
                                printf("ERROR(%d): Unary 'chsign' requires an operand of type int but was given type Equal.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case UndefinedType:
                                printf("ERROR(%d): Unary 'chsign' requires an operand of type int but was given type undefined type.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            default:
                                printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                                break;
                            }
                        }
                        else //decl var
                        {
                            void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                            if (!data)
                            {
                                printf("fix if needed\n");
                                numErrors++;
                            }
                            else
                            {
                                syntaxTree->child[0]->declType = static_cast<TreeNode *>(data)->declType;
                                switch (static_cast<TreeNode *>(data)->declType)
                                {
                                case DeclVoid:
                                    printf("ERROR(%d): Unary 'chsign' requires an operand of type int but was given type void.\n", syntaxTree->lineno);
                                    break;
                                case DeclInteger:
                                    syntaxTree->child[0]->expType = Integer;
                                    break;
                                case DeclChar:
                                    printf("ERROR(%d): Unary 'chsign' requires an operand of type int but was given type char.\n", syntaxTree->lineno);
                                    break;
                                case DeclBoolean:
                                    printf("ERROR(%d): Unary 'chsign' requires an operand of type int but was given type bool.\n", syntaxTree->lineno);
                                    break;
                                default:
                                    printf("ADD MORE\n");
                                    break;
                                }
                            }
                        }
                    }
                }
                else if (strcmp(syntaxTree->attr.name, "?") == 0)
                {
                    //int=int
                    if (syntaxTree->isArray)
                    {
                        printf("ERROR(%d): The operation '?' does not work with arrays.\n", syntaxTree->lineno);
                        numErrors++;
                    }
                    else
                    {
                        if (syntaxTree->child[0]->attr.value > -10000) //constant check
                        {
                            switch (syntaxTree->child[0]->expType)
                            {
                            case Boolean:
                                printf("ERROR(%d): Unary '?' requires an operand of type int but was given type bool.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case Void:
                                printf("ERROR(%d): Unary '?' requires an operand of type int but was given type void.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case Integer:
                                syntaxTree->expType = Integer;
                                break;
                            case Char:
                                printf("ERROR(%d): Unary '?' requires an operand of type int but was given type char.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case CharInt:
                                printf("ERROR(%d): Unary '?' requires an operand of type int but was given type charint.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case Equal:
                                printf("ERROR(%d): Unary '?' requires an operand of type int but was given type Equal.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            case UndefinedType:
                                printf("ERROR(%d): Unary '?' requires an operand of type int but was given type undefined type.\n", syntaxTree->lineno);
                                numErrors++;
                                break;
                            default:
                                printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                                break;
                            }
                        }
                        else //decl var
                        {
                            void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                            if (!data)
                            {
                                printf("fix if needed\n");
                                numErrors++;
                            }
                            else
                            {
                                syntaxTree->child[0]->declType = static_cast<TreeNode *>(data)->declType;
                                switch (static_cast<TreeNode *>(data)->declType)
                                {
                                case DeclVoid:
                                    printf("ERROR(%d): Unary '?' requires an operand of type int but was given type void.\n", syntaxTree->lineno);
                                    break;
                                case DeclInteger:
                                    syntaxTree->child[0]->expType = Integer;
                                    break;
                                case DeclChar:
                                    printf("ERROR(%d): Unary '?' requires an operand of type int but was given type char.\n", syntaxTree->lineno);
                                    break;
                                case DeclBoolean:
                                    printf("ERROR(%d): Unary '?' requires an operand of type int but was given type bool.\n", syntaxTree->lineno);
                                    break;
                                default:
                                    printf("ADD MORE\n");
                                    break;
                                }
                            }
                        }
                    }
                }
                else
                {
                    printf("ERROR(MY IMPLEMENTATION): unable to determine Operator: case OpK:\n");
                }
                break;
            case ConstantK:
                /*
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
                    */
                break;
            case IdK:
            {
                void *data = (TreeNode *)symbolTable.lookup(syntaxTree->attr.name);

                if (data == NULL)
                {
                    printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numWarnings++;
                    syntaxTree->declType = DeclUndefinedType;
                }
                else
                {
                    syntaxTree->isUsed = true;
                    //printf("ERROR(%d): Symbol '%s' is already declared at line %d.\n", syntaxTree->lineno, syntaxTree->attr.name, data->lineno);
                    //numErrors++;
                }
                break;
            }
            case AssignK:
            {
                if (strcmp(syntaxTree->attr.name, "++") == 0)
                {
                    //int=int
                    if (syntaxTree->isArray)
                    {
                        printf("ERROR(%d): The operation '--' does not work with arrays.\n", syntaxTree->lineno);
                        numErrors++;
                    }
                    else
                    {
                        switch (syntaxTree->child[0]->expType)
                        {
                        case Boolean:
                            printf("ERROR(%d): Unary '++' requires an operand of type int but was given bool.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case Void:
                            printf("ERROR(%d): Unary '++' requires an operand of type int but was given void.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case Integer:
                            syntaxTree->expType = Integer;
                            break;
                        case Char:
                            printf("ERROR(%d): Unary '++' requires an operand of type int but was given char.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case CharInt:
                            printf("ERROR(%d): Unary '++' requires an operand of type int but was given charint.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case Equal:
                            printf("ERROR(%d): Unary '++' requires an operand of type int but was given equal.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case UndefinedType:
                            printf("ERROR(%d): Unary '++' requires an operand of type int but was given undefinedtype.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        default:
                            printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                            break;
                        }
                    }
                }
                else if (strcmp(syntaxTree->attr.name, "--") == 0)
                {
                    //int=int
                    if (syntaxTree->isArray)
                    {
                        printf("ERROR(%d): The operation '--' does not work with arrays.\n", syntaxTree->lineno);
                        numErrors++;
                    }
                    else
                    {
                        switch (syntaxTree->child[0]->expType)
                        {
                        case Boolean:
                            printf("ERROR(%d): Unary '--' requires an operand of type int but was given bool.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case Void:
                            printf("ERROR(%d): Unary '--' requires an operand of type int but was given void.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case Integer:
                            syntaxTree->expType = Integer;
                            break;
                        case Char:
                            printf("ERROR(%d): Unary '--' requires an operand of type int but was given char.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case CharInt:
                            printf("ERROR(%d): Unary '--' requires an operand of type int but was given charint.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case Equal:
                            printf("ERROR(%d): Unary '--' requires an operand of type int but was given equal.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        case UndefinedType:
                            printf("ERROR(%d): Unary '--' requires an operand of type int but was given undefinedtype.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                            break;
                        default:
                            printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                            break;
                        }
                    }
                }
                else if (strcmp(syntaxTree->attr.name, "/=") == 0)
                {
                    //int,int=int
                    //LHS check
                    if (syntaxTree->child[0]->attr.value > -10000) //constant check
                    {
                        switch (syntaxTree->child[0]->expType)
                        {
                        case Boolean:
                            printf("ERROR(%d): '/=' requires operands of type int but lhs is of type bool.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Void:
                            printf("ERROR(%d): '/=' requires operands of type int but lhs is of type void.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Integer:
                            syntaxTree->expType = Integer;
                            break;
                        case Char:
                            printf("ERROR(%d): '/=' requires operands of type int but lhs is of type char.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case CharInt:
                            printf("ERROR(%d): '/=' requires operands of type int but lhs is of type charint.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Equal:
                            printf("ERROR(%d): '/=' requires operands of type int but lhs is of type equal.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case UndefinedType:
                            printf("ERROR(%d): '/=' requires operands of type int but lhs is of type UndefinedType.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        default:
                            printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                            break;
                        }
                    }
                    else //decl var
                    {
                        void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                        if (!data)
                        {
                            printf("fix if needed\n");
                            numErrors++;
                        }
                        else
                        {
                            syntaxTree->child[0]->declType = static_cast<TreeNode *>(data)->declType;
                            switch (static_cast<TreeNode *>(data)->declType)
                            {
                            case DeclVoid:
                                printf("ERROR(%d): '/=' requires operands of type int but lhs is of type void.\n", syntaxTree->lineno);
                                break;
                            case DeclInteger:
                                syntaxTree->child[0]->expType = Integer;
                                break;
                            case DeclChar:
                                printf("ERROR(%d): '/=' requires operands of type int but lhs is of type char.\n", syntaxTree->lineno);
                                break;
                            case DeclBoolean:
                                printf("ERROR(%d): '/=' requires operands of type int but lhs is of type bool.\n", syntaxTree->lineno);
                                break;
                            default:
                                printf("ADD MORE\n");
                                break;
                            }
                        }
                    }
                    //RHS check
                    if (syntaxTree->child[1]->attr.value > -10000) //constant check
                    {
                        switch (syntaxTree->child[1]->expType)
                        {
                        case Boolean:
                            printf("ERROR(%d): '/=' requires operands of type int but rhs is of type bool.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Void:
                            printf("ERROR(%d): '/=' requires operands of type int but rhs is of type void.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Integer:
                            syntaxTree->expType = Integer;
                            break;
                        case Char:
                            printf("ERROR(%d): '/=' requires operands of type int but rhs is of type char.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case CharInt:
                            printf("ERROR(%d): '/=' requires operands of type int but rhs is of type charint.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Equal:
                            printf("ERROR(%d): '/=' requires operands of type int but rhs is of type equal.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case UndefinedType:
                            printf("ERROR(%d): '/=' requires operands of type int but rhs is of type UndefinedType.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        default:
                            printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                            break;
                        }
                    }
                    else //decl var
                    {
                        void *data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                        if (!data)
                        {
                            printf("fix if needed\n");
                            numErrors++;
                        }
                        else
                        {
                            syntaxTree->child[1]->declType = static_cast<TreeNode *>(data)->declType;
                            switch (static_cast<TreeNode *>(data)->declType)
                            {
                            case DeclVoid:
                                printf("ERROR(%d): '/=' requires operands of type int but rhs is of type void.\n", syntaxTree->lineno);
                                break;
                            case DeclInteger:
                                syntaxTree->child[1]->expType = Integer;
                                break;
                            case DeclChar:
                                printf("ERROR(%d): '/=' requires operands of type int but rhs is of type char.\n", syntaxTree->lineno);
                                break;
                            case DeclBoolean:
                                printf("ERROR(%d): '/=' requires operands of type int but rhs is of type bool.\n", syntaxTree->lineno);
                                break;
                            default:
                                printf("ADD MORE\n");
                                break;
                            }
                        }
                    }
                }
                else if (strcmp(syntaxTree->attr.name, "*=") == 0)
                {
                    //int,int=int
                    //LHS check
                    if (syntaxTree->child[0]->attr.value > -10000) //constant check
                    {
                        switch (syntaxTree->child[0]->expType)
                        {
                        case Boolean:
                            printf("ERROR(%d): '*=' requires operands of type int but lhs is of type bool.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Void:
                            printf("ERROR(%d): '*=' requires operands of type int but lhs is of type void.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Integer:
                            syntaxTree->expType = Integer;
                            break;
                        case Char:
                            printf("ERROR(%d): '*=' requires operands of type int but lhs is of type char.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case CharInt:
                            printf("ERROR(%d): '*=' requires operands of type int but lhs is of type charint.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Equal:
                            printf("ERROR(%d): '*=' requires operands of type int but lhs is of type equal.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case UndefinedType:
                            printf("ERROR(%d): '*=' requires operands of type int but lhs is of type UndefinedType.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        default:
                            printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                            break;
                        }
                    }
                    else //decl var
                    {
                        void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                        if (!data)
                        {
                            printf("fix if needed\n");
                            numErrors++;
                        }
                        else
                        {
                            syntaxTree->child[0]->declType = static_cast<TreeNode *>(data)->declType;
                            switch (static_cast<TreeNode *>(data)->declType)
                            {
                            case DeclVoid:
                                printf("ERROR(%d): '*=' requires operands of type int but lhs is of type void.\n", syntaxTree->lineno);
                                break;
                            case DeclInteger:
                                syntaxTree->child[0]->expType = Integer;
                                break;
                            case DeclChar:
                                printf("ERROR(%d): '*=' requires operands of type int but lhs is of type char.\n", syntaxTree->lineno);
                                break;
                            case DeclBoolean:
                                printf("ERROR(%d): '*=' requires operands of type int but lhs is of type bool.\n", syntaxTree->lineno);
                                break;
                            default:
                                printf("ADD MORE\n");
                                break;
                            }
                        }
                    }
                    //RHS check
                    if (syntaxTree->child[1]->attr.value > -10000) //constant check
                    {
                        switch (syntaxTree->child[1]->expType)
                        {
                        case Boolean:
                            printf("ERROR(%d): '*=' requires operands of type int but rhs is of type bool.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Void:
                            printf("ERROR(%d): '*=' requires operands of type int but rhs is of type void.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Integer:
                            syntaxTree->expType = Integer;
                            break;
                        case Char:
                            printf("ERROR(%d): '*=' requires operands of type int but rhs is of type char.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case CharInt:
                            printf("ERROR(%d): '*=' requires operands of type int but rhs is of type charint.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Equal:
                            printf("ERROR(%d): '*=' requires operands of type int but rhs is of type equal.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case UndefinedType:
                            printf("ERROR(%d): '*=' requires operands of type int but rhs is of type UndefinedType.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        default:
                            printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                            break;
                        }
                    }
                    else //decl var
                    {
                        void *data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                        if (!data)
                        {
                            printf("fix if needed\n");
                            numErrors++;
                        }
                        else
                        {
                            syntaxTree->child[1]->declType = static_cast<TreeNode *>(data)->declType;
                            switch (static_cast<TreeNode *>(data)->declType)
                            {
                            case DeclVoid:
                                printf("ERROR(%d): '*=' requires operands of type int but rhs is of type void.\n", syntaxTree->lineno);
                                break;
                            case DeclInteger:
                                syntaxTree->child[1]->expType = Integer;
                                break;
                            case DeclChar:
                                printf("ERROR(%d): '*=' requires operands of type int but rhs is of type char.\n", syntaxTree->lineno);
                                break;
                            case DeclBoolean:
                                printf("ERROR(%d): '*=' requires operands of type int but rhs is of type bool.\n", syntaxTree->lineno);
                                break;
                            default:
                                printf("ADD MORE\n");
                                break;
                            }
                        }
                    }
                }
                else if (strcmp(syntaxTree->attr.name, "-=") == 0)
                {
                    //int,int=int
                    //LHS check
                    if (syntaxTree->child[0]->attr.value > -10000) //constant check
                    {
                        switch (syntaxTree->child[0]->expType)
                        {
                        case Boolean:
                            printf("ERROR(%d): '-=' requires operands of type int but lhs is of type bool.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Void:
                            printf("ERROR(%d): '-=' requires operands of type int but lhs is of type void.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Integer:
                            syntaxTree->expType = Integer;
                            break;
                        case Char:
                            printf("ERROR(%d): '-=' requires operands of type int but lhs is of type char.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case CharInt:
                            printf("ERROR(%d): '-=' requires operands of type int but lhs is of type charint.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Equal:
                            printf("ERROR(%d): '-=' requires operands of type int but lhs is of type equal.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case UndefinedType:
                            printf("ERROR(%d): '-=' requires operands of type int but lhs is of type UndefinedType.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        default:
                            printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                            break;
                        }
                    }
                    else //decl var
                    {
                        void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                        if (!data)
                        {
                            printf("fix if needed\n");
                            numErrors++;
                        }
                        else
                        {
                            syntaxTree->child[0]->declType = static_cast<TreeNode *>(data)->declType;
                            switch (static_cast<TreeNode *>(data)->declType)
                            {
                            case DeclVoid:
                                printf("ERROR(%d): '-=' requires operands of type int but lhs is of type void.\n", syntaxTree->lineno);
                                break;
                            case DeclInteger:
                                syntaxTree->child[0]->expType = Integer;
                                break;
                            case DeclChar:
                                printf("ERROR(%d): '-=' requires operands of type int but lhs is of type char.\n", syntaxTree->lineno);
                                break;
                            case DeclBoolean:
                                printf("ERROR(%d): '-=' requires operands of type int but lhs is of type bool.\n", syntaxTree->lineno);
                                break;
                            default:
                                printf("ADD MORE\n");
                                break;
                            }
                        }
                    }
                    //RHS check
                    if (syntaxTree->child[1]->attr.value > -10000) //constant check
                    {
                        switch (syntaxTree->child[1]->expType)
                        {
                        case Boolean:
                            printf("ERROR(%d): '-=' requires operands of type int but rhs is of type bool.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Void:
                            printf("ERROR(%d): '-=' requires operands of type int but rhs is of type void.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Integer:
                            syntaxTree->expType = Integer;
                            break;
                        case Char:
                            printf("ERROR(%d): '-=' requires operands of type int but rhs is of type char.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case CharInt:
                            printf("ERROR(%d): '-=' requires operands of type int but rhs is of type charint.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Equal:
                            printf("ERROR(%d): '-=' requires operands of type int but rhs is of type equal.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case UndefinedType:
                            printf("ERROR(%d): '-=' requires operands of type int but rhs is of type UndefinedType.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        default:
                            printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                            break;
                        }
                    }
                    else //decl var
                    {
                        void *data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                        if (!data)
                        {
                            printf("fix if needed\n");
                            numErrors++;
                        }
                        else
                        {
                            syntaxTree->child[1]->declType = static_cast<TreeNode *>(data)->declType;
                            switch (static_cast<TreeNode *>(data)->declType)
                            {
                            case DeclVoid:
                                printf("ERROR(%d): '-=' requires operands of type int but rhs is of type void.\n", syntaxTree->lineno);
                                break;
                            case DeclInteger:
                                syntaxTree->child[1]->expType = Integer;
                                break;
                            case DeclChar:
                                printf("ERROR(%d): '-=' requires operands of type int but rhs is of type char.\n", syntaxTree->lineno);
                                break;
                            case DeclBoolean:
                                printf("ERROR(%d): '-=' requires operands of type int but rhs is of type bool.\n", syntaxTree->lineno);
                                break;
                            default:
                                printf("ADD MORE\n");
                                break;
                            }
                        }
                    }
                }
                else if (strcmp(syntaxTree->attr.name, "+=") == 0)
                {
                    //int,int=int
                    //LHS check
                    if (syntaxTree->child[0]->attr.value > -10000) //constant check
                    {
                        switch (syntaxTree->child[0]->expType)
                        {
                        case Boolean:
                            printf("ERROR(%d): '+=' requires operands of type int but lhs is of type bool.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Void:
                            printf("ERROR(%d): '+=' requires operands of type int but lhs is of type void.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Integer:
                            syntaxTree->expType = Integer;
                            break;
                        case Char:
                            printf("ERROR(%d): '+=' requires operands of type int but lhs is of type char.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case CharInt:
                            printf("ERROR(%d): '+=' requires operands of type int but lhs is of type charint.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Equal:
                            printf("ERROR(%d): '+=' requires operands of type int but lhs is of type equal.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case UndefinedType:
                            printf("ERROR(%d): '+=' requires operands of type int but lhs is of type UndefinedType.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        default:
                            printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                            break;
                        }
                    }
                    else //decl var
                    {
                        void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                        if (!data)
                        {
                            printf("fix if needed\n");
                            numErrors++;
                        }
                        else
                        {
                            syntaxTree->child[0]->declType = static_cast<TreeNode *>(data)->declType;
                            switch (static_cast<TreeNode *>(data)->declType)
                            {
                            case DeclVoid:
                                printf("ERROR(%d): '+=' requires operands of type int but lhs is of type void.\n", syntaxTree->lineno);
                                break;
                            case DeclInteger:
                                syntaxTree->child[0]->expType = Integer;
                                break;
                            case DeclChar:
                                printf("ERROR(%d): '+=' requires operands of type int but lhs is of type char.\n", syntaxTree->lineno);
                                break;
                            case DeclBoolean:
                                printf("ERROR(%d): '+=' requires operands of type int but lhs is of type bool.\n", syntaxTree->lineno);
                                break;
                            default:
                                printf("ADD MORE\n");
                                break;
                            }
                        }
                    }
                    //RHS check
                    if (syntaxTree->child[1]->attr.value > -10000) //constant check
                    {
                        switch (syntaxTree->child[1]->expType)
                        {
                        case Boolean:
                            printf("ERROR(%d): '+=' requires operands of type int but rhs is of type bool.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Void:
                            printf("ERROR(%d): '+=' requires operands of type int but rhs is of type void.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Integer:
                            syntaxTree->expType = Integer;
                            break;
                        case Char:
                            printf("ERROR(%d): '+=' requires operands of type int but rhs is of type char.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case CharInt:
                            printf("ERROR(%d): '+=' requires operands of type int but rhs is of type charint.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case Equal:
                            printf("ERROR(%d): '+=' requires operands of type int but rhs is of type equal.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case UndefinedType:
                            printf("ERROR(%d): '+=' requires operands of type int but rhs is of type UndefinedType.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        default:
                            printf("ERROR(MY_IMPLEMENTATION): case OpK: AND switch\n");
                            break;
                        }
                    }
                    else //decl var
                    {
                        void *data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                        if (!data)
                        {
                            printf("fix if needed\n");
                            numErrors++;
                        }
                        else
                        {
                            syntaxTree->child[1]->declType = static_cast<TreeNode *>(data)->declType;
                            switch (static_cast<TreeNode *>(data)->declType)
                            {
                            case DeclVoid:
                                printf("ERROR(%d): '+=' requires operands of type int but rhs is of type void.\n", syntaxTree->lineno);
                                break;
                            case DeclInteger:
                                syntaxTree->child[1]->expType = Integer;
                                break;
                            case DeclChar:
                                printf("ERROR(%d): '+=' requires operands of type int but rhs is of type char.\n", syntaxTree->lineno);
                                break;
                            case DeclBoolean:
                                printf("ERROR(%d): '+=' requires operands of type int but rhs is of type bool.\n", syntaxTree->lineno);
                                break;
                            default:
                                printf("ADD MORE\n");
                                break;
                            }
                        }
                    }
                }
                else if (strcmp(syntaxTree->attr.name, "=") == 0)
                {
                    //printf("2");
                    //fflush(stdout);
                    //equal types + arrays = bool
                    bool twoArrays = false;
                    if (syntaxTree->child[0]->isArray && syntaxTree->child[1]->isArray)
                    {
                        twoArrays = true;
                    }
                    else if (syntaxTree->child[0]->isArray && !syntaxTree->child[1]->isArray)
                    {
                        printf("ERROR(%d): '=' requires both operands be arrays or not but lhs is an array and rhs is not an array.\n", syntaxTree->lineno);
                        numErrors++;
                    }
                    else if ((!syntaxTree->child[0]->isArray && syntaxTree->child[1]->isArray))
                    {
                        printf("ERROR(%d): '=' requires both operands be arrays or not but lhs is not an array and rhs is an array.\n", syntaxTree->lineno);
                        numErrors++;
                    }
                    else if ((!syntaxTree->child[0]->isArray && !syntaxTree->child[1]->isArray) || twoArrays)
                    {
                        void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                        if (!data)
                        {
                            printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->attr.name);
                            numErrors++;
                        }
                        else
                        {
                            syntaxTree->child[0]->declType = static_cast<TreeNode *>(data)->declType;
                        }
                        if (syntaxTree->child[0]->declType == syntaxTree->child[1]->declType)
                        {
                            syntaxTree->declType = syntaxTree->child[0]->declType;
                        }
                        else
                        {
                            printf("ERROR(%d): '=' requires operands of the same type but lhs is type ", syntaxTree->lineno);
                            numErrors++;
                            switch (syntaxTree->child[0]->declType)
                            {
                            case DeclInteger:
                                printf("int");
                                break;
                            case DeclVoid:
                                printf("void");
                                break;
                            case DeclBoolean:
                                printf("bool");
                                break;
                            case DeclChar:
                                printf("char");
                                break;
                            case DeclCharInt:
                                printf("charint");
                                break;
                            case DeclEqual:
                                printf("equal");
                                break;
                            case DeclUndefinedType:
                                printf("undefined");
                                break;
                            default:
                                printf("ERROR: semantic.cpp line 836\n");
                                break;
                            }
                            printf(" and rhs is type ");
                            switch (syntaxTree->child[1]->expType)
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
                                printf("charint");
                                break;
                            case Equal:
                                printf("equal");
                                break;
                            case UndefinedType:
                                printf("undefined");
                                break;
                            default:
                                printf("ERROR: semantic.cpp line 864\n");
                                break;
                            }
                            printf(".\n");
                        }
                    }
                }
                break;
            }
            case InitK:
                break;
            case CallK:
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
            semanticAnalysis(syntaxTree->child[i], symbolTable);
            //siblingCount = 0;
        }
        if(isCompound) { symbolTable.leave();}
        /*
        else if(functionScope)
        {
            //printf("shoiuld leave\n");
            symbolTable.leave();
        }
        */
        syntaxTree = syntaxTree->sibling;
        siblingCount++;
    }
    firstflag = 1;
    UNINDENT;
}