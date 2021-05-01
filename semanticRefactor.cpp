#include "globals.h"
#include "semantic.h"
#include "symbolTable.h"
#include "util.h"
//#include "util.h"
int twotime = 2;
static int indentno = 0;
static int siblingFlag = 0;
//int siblingCount = 0;
static int firstflag = 0;
static int firstLeftglag = 0;
#define INDENT indentno += 1
#define UNINDENT indentno -= 1
int loopCount = 0;
bool returnFlag = false;
int functionScope = 0;
bool disableW = false;
int isMain = 0;
bool inCompound = false;
bool forflag = false;
TreeNode *mostRecentFunc = new treeNode;
void semanticAnalysis(TreeNode *syntaxTree, SymbolTable symbolTable)
{
    fflush(stdout);
    //DECLARATIONS
    if (syntaxTree->nodekind == DeclK)
    {
        switch (syntaxTree->subkind.decl)
        {
        case VarK:
        {
            if (syntaxTree->isStatic)
            {
                //printf("static initialization\n");
                syntaxTree->isInit = true;
            }

            void *data = symbolTable.lookup(syntaxTree->attr.name);
            if (data == NULL)
            {
                if (forflag)
                {
                    syntaxTree->isInit = true;
                }
                //if static, initialized
                if (syntaxTree->isStatic)
                {
                    syntaxTree->isInit = true;
                }
                bool b = symbolTable.insert(syntaxTree->attr.name, syntaxTree);
                if (b)
                {
                    //if global, initialized
                    void *data = symbolTable.lookupGlobal(syntaxTree->attr.name);
                    if (data != NULL)
                    {
                        //printf("global initialization\n");
                        syntaxTree->isInit = true;
                        syntaxTree->mem = glob;
                    }
                    else
                    {
                        if (syntaxTree->isStatic)
                        {
                            syntaxTree->mem = stat;
                        }
                        else
                        {
                            syntaxTree->mem = loca;
                        }
                    }
                }
                else
                {
                    void *data = symbolTable.lookup(syntaxTree->attr.name);
                    printf("ERROR(%d): Symbol '%s' is already declared at line %d.\n", syntaxTree->lineno, syntaxTree->attr.name, static_cast<TreeNode *>(data)->lineno);
                    numErrors++;
                }
            }
            else
            {
                bool ins = symbolTable.insert(syntaxTree->attr.name, syntaxTree);
                if (!ins)
                {
                    printf("ERROR(%d): Symbol '%s' is already declared at line %d.\n", syntaxTree->lineno, syntaxTree->attr.name, static_cast<TreeNode *>(data)->lineno);
                    numErrors++;
                }
            }

            if (syntaxTree->child[0] != NULL)
            {
                semanticAnalysis(syntaxTree->child[0], symbolTable);
            }

            switch (syntaxTree->mem)
            {
            case glob:
                if (syntaxTree->isArray)
                {
                    syntaxTree->location = goffset - 1;
                }
                else
                {
                    syntaxTree->location = goffset;
                }
                break;
            case stat:
                if (syntaxTree->isArray)
                {
                    syntaxTree->location = goffset - 1;
                }
                else
                {
                    syntaxTree->location = goffset;
                }
                break;
            default:
                if (syntaxTree->isArray)
                {
                    syntaxTree->location = foffset - 1;
                }
                else
                {
                    syntaxTree->location = foffset;
                }
                break;
            }

            if (syntaxTree->child[0] != NULL)
            {
                syntaxTree->isInit = true;

                // initializer not constant
                if (!syntaxTree->child[0]->isConstant)
                {
                    printf("ERROR(%d): Initializer for variable '%s' is not a constant expression.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                }

                // initializer not same type
                if (syntaxTree->nodeType != syntaxTree->child[0]->nodeType)
                {
                    printf("ERROR(%d): Initializer for variable '%s' of type ", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;

                    switch (syntaxTree->nodeType)
                    {
                    case nInt:
                        printf("int is of type ");
                        break;
                    case nBool:
                        printf("bool is of type ");
                        break;
                    case nChar:
                        printf("char is of type ");
                        break;
                    case nUndefinedType:
                        printf("undefined is of type ");
                        break;
                    default:
                        printf("bad2, dont want this\n");
                        break;
                    }
                    switch (syntaxTree->child[0]->nodeType)
                    {
                    case nInt:
                        printf("int\n");
                        break;
                    case nVoid:
                        printf("void\n");
                        break;
                    case nBool:
                        printf("bool\n");
                        break;
                    case nChar:
                        printf("char\n");
                        break;
                    case nUndefinedType:
                        printf("undefined\n");
                        break;
                    default:
                        printf("badbad\n");
                        break;
                        break;
                    }
                }

                // both be arrays or not

                if ((syntaxTree->isArray && !syntaxTree->child[0]->isArray) || (!syntaxTree->isArray && syntaxTree->child[0]->isArray))
                {
                    printf("ERROR(%d): Initializer for variable '%s' requires both operands be arrays or not but variable is ", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    if (!syntaxTree->isArray)
                    {
                        printf("not ");
                    }
                    printf("an array and rhs is ");
                    if (!syntaxTree->child[0]->isArray)
                    {
                        printf("not ");
                    }
                    printf("an array.\n");
                }
            }
            switch (syntaxTree->mem)
            {
            case glob:
                goffset = goffset - syntaxTree->size;
                break;
            case stat:
                goffset = goffset - syntaxTree->size;
                break;
            default:
                foffset = foffset - syntaxTree->size;
                break;
            }
            break;
        }
        case FuncK:
        {
            syntaxTree->mem = glob;
            foffset = -2;

            returnFlag = false;
            syntaxTree->isInit = true;
            if (strcmp(syntaxTree->attr.name, "main") == 0)
            {
                if (syntaxTree->child[0] == NULL)
                {
                    isMain = 1;
                }
            }
            // continue forward
            //bool b = symbolTable.insertGlobal(syntaxTree->attr.name, syntaxTree);
            void *data = symbolTable.lookup(syntaxTree->attr.name);
            if (data == NULL)
            {
                //functionScope = 0;
                syntaxTree->isInit = true;
                symbolTable.insertGlobal(syntaxTree->attr.name, syntaxTree);
            }
            else
            {
                //void *data = symbolTable.lookup(syntaxTree->attr.name);
                printf("ERROR(%d): Symbol '%s' is already declared at line %d.\n", syntaxTree->lineno, syntaxTree->attr.name, static_cast<TreeNode *>(data)->lineno);
                numErrors++;
            }
            symbolTable.enter(syntaxTree->attr.name);
            mostRecentFunc->attr.name = syntaxTree->attr.name;
            mostRecentFunc->lineno = syntaxTree->lineno;
            mostRecentFunc->nodeType = syntaxTree->nodeType;
            //
            //children
            if (syntaxTree->child[0] != NULL)
            {
                semanticAnalysis(syntaxTree->child[0], symbolTable);
            }
            //after parms, set func size = foffset
            syntaxTree->size = foffset;

            if (syntaxTree->child[1] != NULL)
            {
                if (syntaxTree->child[1]->nodekind == StmtK && syntaxTree->child[1]->subkind.stmt == CompoundK)
                {
                    syntaxTree->child[1]->firstCompound = true;
                }
                semanticAnalysis(syntaxTree->child[1], symbolTable);
            }

            //check for used before leaving
            is_Used(symbolTable);
            if (syntaxTree->nodeType == nInt || syntaxTree->nodeType == nChar || syntaxTree->nodeType == nBool || syntaxTree->nodeType == nUndefinedType)
            {
                if (!returnFlag)
                {
                    printf("WARNING(%d): Expecting to return type ", syntaxTree->lineno);
                    numWarnings++;
                    switch (syntaxTree->nodeType)
                    {
                    case nInt:
                        printf("int ");
                        break;
                    case nChar:
                        printf("char ");
                        break;
                    case nBool:
                        printf("bool ");
                        break;
                    case nUndefinedType:
                        printf("undefined ");
                        break;
                    default:
                        break;
                    }
                    printf("but function '%s' has no return statement.\n", syntaxTree->attr.name);
                }
            }

            mostRecentFunc->attr.name = NULL;
            mostRecentFunc->lineno = -1;
            mostRecentFunc->nodeType = nBad;
            symbolTable.leave();
            break;
        }
        case ParamK:
        {
            syntaxTree->mem = para;
            if (syntaxTree->isArray)
            {
                syntaxTree->location = foffset;
            }
            else
            {
                syntaxTree->location = foffset;
            }
            syntaxTree->isInit = true;
            bool b = symbolTable.insert(syntaxTree->attr.name, syntaxTree);
            //void *data = symbolTable.lookup(syntaxTree->attr.name);
            if (b)
            {
            }
            else
            {
                void *data = symbolTable.lookup(syntaxTree->attr.name);
                printf("ERROR(%d): Symbol '%s' is already declared at line %d.\n", syntaxTree->lineno, syntaxTree->attr.name, static_cast<TreeNode *>(data)->lineno);
                numErrors++;
            }
            /*
            if (data == NULL)
            {
                symbolTable.insert(syntaxTree->attr.name, syntaxTree);
            }
            else
            {
            }
            */
            foffset = foffset - syntaxTree->size;
            break;
        }
        default:
            printf("ERROR: Unknown DeclK!! (SemanticAnalysis)\n");
            break;
        }
    }
    // STATEMENTS
    else if (syntaxTree->nodekind == StmtK)
    {
        switch (syntaxTree->subkind.stmt)
        {
        case NullK:
        {
            printf("ERROR(PRETTY SURE THIS CAN BE DELETED, WAS IN CLASS SUPPLIED FILE THOUGH)\n");
            break;
        }
        case IfK:
        {
            //symbolTable.print(pointerPrintAddr);
            symbolTable.enter("If");
            //process stuff
            if (syntaxTree->child[0] != NULL)
            {
                semanticAnalysis(syntaxTree->child[0], symbolTable);
            }
            else
            {
                printf("ERROR(YOU SHOULD NOT SEE THIS) IfKc[0].\n");
            }
            if (syntaxTree->child[0]->isNC || syntaxTree->child[0]->isConstant)
            {
            }
            else if (!(strcmp(syntaxTree->child[0]->attr.name, "OR") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "<=") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "!=") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, ">") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "++") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "!=") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "NOT") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "==") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "=") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "<=") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, ">=") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "<") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, ">") == 0))
            {
                void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                if (data == NULL)
                {
                    syntaxTree->child[0]->isInit = true;
                    symbolTable.insert(syntaxTree->child[0]->attr.name, syntaxTree);
                }
                else
                {
                    static_cast<TreeNode *>(data)->isUsed = true; //MAYBE
                    syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                    syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                    syntaxTree->child[0]->isInit = static_cast<TreeNode *>(data)->isInit;
                    if (!syntaxTree->child[0]->isInit)
                    {
                        printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        numWarnings++;
                        static_cast<TreeNode *>(data)->isInit = true;
                    }
                }
            }
            if (syntaxTree->child[0]->isArray)
            {
                printf("ERROR(%d): Cannot use array as test condition in if statement.\n", syntaxTree->lineno);
                numErrors++;
            }
            if (syntaxTree->child[0]->nodeType != nBool)
            {
                switch (syntaxTree->child[0]->nodeType)
                {
                case nInt:
                    printf("ERROR(%d): Expecting Boolean test condition in if statement but got type int.\n", syntaxTree->lineno);
                    numErrors++;
                    break;
                case nChar:
                    printf("ERROR(%d): Expecting Boolean test condition in if statement but got type char.\n", syntaxTree->lineno);
                    numErrors++;
                    break;
                case nVoid:
                    printf("ERROR(%d): Expecting Boolean test condition in if statement but got type void.\n", syntaxTree->lineno);
                    numErrors++;
                    break;
                case nUndefinedType:
                    printf("ERROR(%d): Expecting Boolean test condition in if statement but got undefined type.\n", syntaxTree->lineno);
                    numErrors++;
                    break;
                default:
                    //printf("bad news ifk\n");
                    break;
                }
            }
            //process stuff
            if (syntaxTree->child[1] != NULL)
            {
                semanticAnalysis(syntaxTree->child[1], symbolTable);
            }
            if (syntaxTree->child[2] != NULL)
            {
                semanticAnalysis(syntaxTree->child[2], symbolTable);
            }
            is_Used(symbolTable);
            symbolTable.leave();
            break;
        }
        case WhileK:
        {
            //process stuff
            symbolTable.enter("While");
            loopCount++;
            if (syntaxTree->child[0] != NULL)
            {
                semanticAnalysis(syntaxTree->child[0], symbolTable);
            }
            else
            {
                printf("ERROR(YOU SHOULD NOT SEE THIS) WhileKc[0].\n");
            }
            if (syntaxTree->child[0]->isNC || syntaxTree->child[0]->isConstant)
            {
            }
            else if (!(strcmp(syntaxTree->child[0]->attr.name, "%") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "=") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "OR") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "==") == 0))
            {
                void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                if (data == NULL)
                {
                    syntaxTree->child[0]->isInit = true;
                    //symbolTable.insert(syntaxTree->child[0]->attr.name, syntaxTree);
                }
                else
                {
                    static_cast<TreeNode *>(data)->isUsed = true; //MAYBE
                    syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                    syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                }
            }
            if (syntaxTree->child[0]->isArray)
            {
                printf("ERROR(%d): Cannot use array as test condition in while statement.\n", syntaxTree->lineno);
                numErrors++;
            }
            if (syntaxTree->child[0]->nodeType != nBool)
            {
                switch (syntaxTree->child[0]->nodeType)
                {
                case nInt:
                    printf("ERROR(%d): Expecting Boolean test condition in while statement but got type int.\n", syntaxTree->lineno);
                    numErrors++;
                    break;
                case nChar:
                    printf("ERROR(%d): Expecting Boolean test condition in while statement but got type char.\n", syntaxTree->lineno);
                    numErrors++;
                    break;
                case nVoid:
                    printf("ERROR(%d): Expecting Boolean test condition in while statement but got type void.\n", syntaxTree->lineno);
                    numErrors++;
                    break;
                case nUndefinedType:
                    printf("ERROR(%d): Expecting Boolean test condition in while statement but got undefined type.\n", syntaxTree->lineno);
                    numErrors++;
                    break;
                default:
                    //printf("bad news ifk\n");
                    break;
                }
            }

            //process stuff
            if (syntaxTree->child[1] != NULL)
            {
                semanticAnalysis(syntaxTree->child[1], symbolTable);
            }
            else
            {
                //c1 is allowed to be NULL
            }
            is_Used(symbolTable);
            loopCount--;
            symbolTable.leave();
            break;
        }
        case ForK:
        {
            int savedfoffset = foffset;
            //Process stuff
            symbolTable.enter("For");
            loopCount++;
            forflag = true;
            //void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
            //if(data != NULL){
            //    static_cast<TreeNode *>(data)->isInit = true;
            //}
            syntaxTree->child[0]->isInit = true;
            if (syntaxTree->child[0] != NULL)
            {
                semanticAnalysis(syntaxTree->child[0], symbolTable);
            }
            else
            {
                printf("ERROR(YOU SHOULD NOT SEE THIS) ForKc0.\n");
            }
            syntaxTree->size = foffset;
            forflag = false;

            if (syntaxTree->child[1] != NULL)
            {
                semanticAnalysis(syntaxTree->child[1], symbolTable);
            }
            else
            {
            }

            if (syntaxTree->child[2] != NULL)
            {
                if (syntaxTree->child[2]->nodekind == StmtK && syntaxTree->child[2]->subkind.stmt == CompoundK)
                {
                    syntaxTree->child[2]->firstCompound = true;
                }
                semanticAnalysis(syntaxTree->child[2], symbolTable);
            }

            //check for used before leaving
            foffset = savedfoffset;
            is_Used(symbolTable);
            loopCount--;
            symbolTable.leave();
            break;
        }
        case CompoundK:
        {
            int savedfoffset = foffset;

            //enter scope
            if (!syntaxTree->firstCompound)
            {
                symbolTable.enter("Compound Statement");
            }
            inCompound = true;
            if (syntaxTree->child[0] != NULL)
            {
                semanticAnalysis(syntaxTree->child[0], symbolTable);
            }
            else
            {
                //c0 is allowed NULL
            }
            syntaxTree->size = foffset;
            //
            //children
            if (syntaxTree->child[1] != NULL)
            {
                semanticAnalysis(syntaxTree->child[1], symbolTable);
            }
            else
            {
                //c1 is allowed NULL
            }
            foffset = savedfoffset;
            //leave scope
            inCompound = false;
            if (!syntaxTree->firstCompound)
            {
                //check for used before leaving
                is_Used(symbolTable);
                symbolTable.leave();
            }
            break;
        }
        case ReturnK:
        {
            //process stuff
            returnFlag = true;
            if (syntaxTree->child[0] != NULL)
            {
                semanticAnalysis(syntaxTree->child[0], symbolTable);
            }
            if (syntaxTree->child[0] != NULL)
            {
                if (syntaxTree->child[0]->isConstant)
                {
                }
                else if (!(strcmp(syntaxTree->child[0]->attr.name, "SIZEOF") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "--") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "++") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, ">") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "%") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "NOT") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "[") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "=") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "*") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "+") == 0))
                {
                    void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                        syntaxTree->child[0]->isInit = static_cast<TreeNode *>(data)->isInit;

                        if (!syntaxTree->child[0]->isInit)
                        {
                            printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                            numWarnings++;
                            static_cast<TreeNode *>(data)->isInit = true;
                        }
                    }
                }
            }
            if (mostRecentFunc->attr.name != NULL)
            {
                //has a return value when void func
                if (mostRecentFunc->nodeType == nVoid && syntaxTree->child[0] != NULL)
                {
                    printf("ERROR(%d): Function '%s' at line %d is expecting no return value, but return has a value.\n", syntaxTree->lineno, mostRecentFunc->attr.name, mostRecentFunc->lineno);
                    numErrors++;
                }

                //wants return value, but as none
                if ((mostRecentFunc->nodeType == nInt || mostRecentFunc->nodeType == nBool || mostRecentFunc->nodeType == nChar || mostRecentFunc->nodeType == nUndefinedType) && syntaxTree->child[0] == NULL)
                {
                    printf("ERROR(%d): Function '%s' at line %d is expecting to return type ", syntaxTree->lineno, mostRecentFunc->attr.name, mostRecentFunc->lineno);
                    numErrors++;
                    switch (mostRecentFunc->nodeType)
                    {
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
                        printf("bad, dont want this");
                        break;
                    }
                    printf(" but return has no value.\n");
                }

                //wants x return value, but was given y return value
                if (syntaxTree->child[0] != NULL) //maybe revise this?
                {
                    if (mostRecentFunc->nodeType != nVoid && (mostRecentFunc->nodeType != syntaxTree->child[0]->nodeType))
                    {
                        printf("ERROR(%d): Function '%s' at line %d is expecting to return type ", syntaxTree->lineno, mostRecentFunc->attr.name, mostRecentFunc->lineno);
                        numErrors++;
                        switch (mostRecentFunc->nodeType)
                        {
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
                            printf("bad, dont want this");
                            break;
                        }
                        printf(" but returns type ");
                        switch (syntaxTree->child[0]->nodeType)
                        {
                        case nInt:
                            printf("int.\n");
                            break;
                        case nBool:
                            printf("bool.\n");
                            break;
                        case nChar:
                            printf("char.\n");
                            break;
                        case nUndefinedType:
                            printf("undefined.\n");
                            break;
                        default:
                            printf("bad, dont want this2.\n");
                            break;
                        }
                    }
                }
            }

            if (syntaxTree->child[0] != NULL)
            {
                if (syntaxTree->child[0]->isArray)
                {
                    printf("ERROR(%d): Cannot return an array.\n", syntaxTree->lineno);
                    numErrors++;
                }
            }
            if (!inCompound)
            { //printf("return leave\n"); fflush(stdout);
                //symbolTable.leave();
            }
            break;
        }
        case BreakK:
        { //no children
            if (loopCount == 0)
            {
                printf("ERROR(%d): Cannot have a break statement outside of loop.\n", syntaxTree->lineno);
                numErrors++;
            }
            else if (loopCount < 0)
            {
                printf("BreakK, shouldnt see this.\n");
            }
            //if > 0, we're good.
            break;
        }
        case RangeK:
        {
            //process stuff
            if (syntaxTree->child[0] != NULL)
            {
                semanticAnalysis(syntaxTree->child[0], symbolTable);
            }
            else
            {
                printf("ERROR(YOU SHOULD NOT SEE THIS) RangeKc0.\n");
            }

            //c0 processing
            if (syntaxTree->child[0]->isConstant)
            {
            }
            else //if (syntaxTree->child[0] != NULL)
            {
                void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                if (data == NULL)
                {
                    //syntaxTree->child[0]->isInit = true;
                    //symbolTable.insert(syntaxTree->child[0]->attr.name, syntaxTree);
                }
                else
                {
                    static_cast<TreeNode *>(data)->isUsed = true; //MAYBE
                    //syntaxTree->child[0]->nodekind = static_cast<TreeNode *>(data)->nodekind;
                    syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                    syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                    syntaxTree->child[0]->isInit = static_cast<TreeNode *>(data)->isInit;

                    if (!syntaxTree->child[0]->isInit)
                    {
                        printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        numWarnings++;
                        static_cast<TreeNode *>(data)->isInit = true;
                    }
                }
            }
            if (syntaxTree->child[0]->isArray)
            {
                printf("ERROR(%d): Cannot use array in position 1 in range of for statement.\n", syntaxTree->lineno);
                numErrors++;
            }
            if (syntaxTree->child[0]->nodekind == DeclK && syntaxTree->child[0]->subkind.decl == FuncK)
            {
                //printf("ERROR(%d): Cannot use function '%s' as a variable.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                //numErrors++;
            }
            if (syntaxTree->child[0]->nodeType != nInt)
            {
                switch (syntaxTree->child[0]->nodeType)
                {
                case nBool:
                    printf("ERROR(%d): Expecting type int in position 1 in range of for statement but got type bool.\n", syntaxTree->lineno);
                    numErrors++;
                    break;
                case nChar:
                    printf("ERROR(%d): Expecting type int in position 1 in range of for statement but got type char.\n", syntaxTree->lineno);
                    numErrors++;
                    break;
                case nVoid:
                    printf("ERROR(%d): Expecting type int in position 1 in range of for statement but got type void.\n", syntaxTree->lineno);
                    numErrors++;
                    break;
                case nUndefinedType:
                    printf("ERROR(%d): Expecting type int in position 1 in range of for statement but got type undefined.\n", syntaxTree->lineno);
                    numErrors++;
                    break;
                default:
                    //printf("bad news rankgek01\n");
                    break;
                }
            }
            //end of c0 processing

            if (syntaxTree->child[1] != NULL)
            {
                semanticAnalysis(syntaxTree->child[1], symbolTable);
            }
            else
            {
                printf("ERROR(YOU SHOULD NOT SEE THIS) RangeKc1.\n");
            }
            //c1 processing
            bool dNull = false;
            if (syntaxTree->child[1]->isNC || syntaxTree->child[1]->isConstant)
            {
            }
            else if (!(strcmp(syntaxTree->child[1]->attr.name, "SIZEOF") == 0)) //if (syntaxTree->child[1] != NULL && (syntaxTree->child[1]->nodeType == uninit || syntaxTree->child[1]->nodeType == nVoid))
            {
                void *data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                if (data == NULL)
                {
                    dNull = true;
                    //printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[1]->attr.name);
                    //numErrors++;
                }
                else
                {
                    static_cast<TreeNode *>(data)->isUsed = true; //MAYBE
                    //syntaxTree->child[1]->nodekind = static_cast<TreeNode *>(data)->nodekind;
                    syntaxTree->child[1]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                    syntaxTree->child[1]->isArray = static_cast<TreeNode *>(data)->isArray;
                    syntaxTree->child[1]->isInit = static_cast<TreeNode *>(data)->isInit;

                    if (!syntaxTree->child[1]->isInit)
                    {
                        printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", syntaxTree->lineno, syntaxTree->child[1]->attr.name);
                        numWarnings++;
                        static_cast<TreeNode *>(data)->isInit = true;
                    }
                }
            }
            if (!dNull)
            {
                if (syntaxTree->child[1]->isArray)
                {
                    printf("ERROR(%d): Cannot use array in position 2 in range of for statement.\n", syntaxTree->lineno);
                    numErrors++;
                }
                if (syntaxTree->child[1]->nodekind == DeclK && syntaxTree->child[1]->subkind.decl == FuncK)
                {
                    //printf("ERROR(%d): Cannot use function '%s' as a variable.\n", syntaxTree->lineno, syntaxTree->child[1]->attr.name);
                    //numErrors++;
                }
                if (syntaxTree->child[1]->nodeType != nInt)
                {
                    switch (syntaxTree->child[1]->nodeType)
                    {
                    case nBool:
                        printf("ERROR(%d): Expecting type int in position 2 in range of for statement but got type bool.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nChar:
                        printf("ERROR(%d): Expecting type int in position 2 in range of for statement but got type char.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nVoid:
                        printf("ERROR(%d): Expecting type int in position 2 in range of for statement but got type void.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nUndefinedType:
                        printf("ERROR(%d): Expecting type int in position 2 in range of for statement but got type undefined.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    default:
                        printf("bad news rankgek02\n");
                        break;
                    }
                }
            }
            //end of c1 processing

            if (syntaxTree->child[2] != NULL)
            {
                semanticAnalysis(syntaxTree->child[2], symbolTable);
            }
            else
            {
                //c2 is allowed NULL
            }

            //c2 processing
            bool d2Null = false;
            if (syntaxTree->child[2] != NULL)
            {
                if (syntaxTree->child[2]->isNC)
                {
                }
                else if (syntaxTree->child[2] != NULL && (syntaxTree->child[2]->nodeType == uninit || syntaxTree->child[2]->nodeType == nVoid))
                {
                    void *data = symbolTable.lookup(syntaxTree->child[2]->attr.name);
                    if (data == NULL)
                    {
                        d2Null = true;
                        //printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[2]->attr.name);
                        //numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true; //MAYBE
                        //syntaxTree->child[2]->nodekind = static_cast<TreeNode *>(data)->nodekind;
                        //syntaxTree->child[2]->subkind.decl = static_cast<TreeNode *>(data)->subkind.decl;
                        syntaxTree->child[2]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[2]->isArray = static_cast<TreeNode *>(data)->isArray;
                        syntaxTree->child[2]->isInit = static_cast<TreeNode *>(data)->isInit;

                        if (!syntaxTree->child[2]->isInit)
                        {
                            printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", syntaxTree->lineno, syntaxTree->child[2]->attr.name);
                            numWarnings++;
                            static_cast<TreeNode *>(data)->isInit = true;
                        }
                    }
                }
                if (!d2Null)
                {
                    if (syntaxTree->child[2]->isArray)
                    {
                        printf("ERROR(%d): Cannot use array in position 3 in range of for statement.\n", syntaxTree->lineno);
                        numErrors++;
                    }
                    if (syntaxTree->child[2]->nodekind == DeclK && syntaxTree->child[2]->subkind.decl == FuncK)
                    {
                        //printf("ERROR(%d): Cannot use function '%s' as a variable.\n", syntaxTree->lineno, syntaxTree->child[2]->attr.name);
                        //numErrors++;
                    }
                    if (syntaxTree->child[2]->nodeType != nInt)
                    {
                        switch (syntaxTree->child[2]->nodeType)
                        {
                        case nBool:
                            printf("ERROR(%d): Expecting type int in position 3 in range of for statement but got type bool.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case nChar:
                            printf("ERROR(%d): Expecting type int in position 3 in range of for statement but got type char.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case nVoid:
                            printf("ERROR(%d): Expecting type int in position 3 in range of for statement but got type void.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        case nUndefinedType:
                            printf("ERROR(%d): Expecting type int in position 3 in range of for statement but got type undefined.\n", syntaxTree->lineno);
                            numErrors++;
                            break;
                        default:
                            printf("bad news rankgek2\n");
                            break;
                        }
                    }
                }
            }
            //end of c2 processing
            break;
        }
        default:
            printf("ERROR: Unknown StmtK!! (SemanticAnalysis)\n");
            break;
        }
    }
    // EXPRESSIONS
    else if (syntaxTree->nodekind == ExpK)
    {
        switch (syntaxTree->subkind.exp)
        {
        case OpK:
        {
            //Unaries "ERROR(%d): Unary '%s' requires an operand of %s but was given %s.\n"
            if (strcmp(syntaxTree->attr.name, "NOT") == 0)
            {
                syntaxTree->nodeType = nBool;

                //traverse c0 to get its type
                if (syntaxTree->child[0] != NULL)
                {
                    semanticAnalysis(syntaxTree->child[0], symbolTable);
                }
                else
                {
                    printf("ERROR(YOU SHOULD NOT SEE THIS) OpKc0.\n");
                }
                if (syntaxTree->child[0]->isNC || syntaxTree->child[0]->isConstant)
                {
                }
                else if (!(strcmp(syntaxTree->child[0]->attr.name, "NOT") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "<=") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, ">=") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "OR") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "==") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "-") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "NOT") == 0))
                {
                    void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                    }
                }
                if (syntaxTree->child[0]->isArray)
                {
                    printf("ERROR(%d): The operation 'not' does not work with arrays.\n", syntaxTree->lineno);
                    numErrors++;
                }
                else
                {
                    switch (syntaxTree->child[0]->nodeType)
                    {
                    case nBool:
                        syntaxTree->nodeType = nBool;
                        break;
                    case nVoid:
                        printf("ERROR(%d): Unary 'not' requires an operand of type bool but was given type void.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nInt:
                        printf("ERROR(%d): Unary 'not' requires an operand of type bool but was given type int.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nChar:
                        printf("ERROR(%d): Unary 'not' requires an operand of type bool but was given type char.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nUndefinedType:
                        printf("ERROR(%d): Unary 'not' requires an operand of type bool but was given type undefined.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case uninit:
                        break;
                    default:
                        printf("ERROR: semantic.cpp line 836\n");
                        break;
                    }
                }
                if (syntaxTree->child[0]->isConstant)
                {
                    syntaxTree->isConstant = true; // FactCheck this
                }
            }
            else if (strcmp(syntaxTree->attr.name, "SIZEOF") == 0)
            {
                //syntaxTree->nodeType = nInt;
                //traverse c0 to get its type
                if (syntaxTree->child[0] != NULL)
                {
                    semanticAnalysis(syntaxTree->child[0], symbolTable);
                }
                else
                {
                    printf("ERROR(YOU SHOULD NOT SEE THIS) OpKc0.\n");
                }
                if (syntaxTree->child[0]->isNC || syntaxTree->child[0]->isConstant)
                {
                }
                else if (!(strcmp(syntaxTree->child[0]->attr.name, "SIZEOF") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "[") == 0)) // if (syntaxTree->child[0]->nodeType == uninit || syntaxTree->child[0]->nodeType == nVoid)
                {
                    void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                        syntaxTree->child[0]->isStatic = static_cast<TreeNode *>(data)->isStatic;
                        syntaxTree->child[0]->isInit = static_cast<TreeNode *>(data)->isInit;

                        if (!syntaxTree->child[0]->isInit)
                        {
                            printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                            numWarnings++;
                            static_cast<TreeNode *>(data)->isInit = true;
                        }
                    }
                }
                if (!syntaxTree->child[0]->isArray)
                {
                    printf("ERROR(%d): The operation 'sizeof' only works with arrays.\n", syntaxTree->lineno);
                    numErrors++;
                }
                else
                {
                    syntaxTree->nodeType = nInt;
                }
                if (syntaxTree->child[0]->isConstant)
                {
                    syntaxTree->isConstant = true; // FactCheck this
                }
            }
            else if (strcmp(syntaxTree->attr.name, "CHSIGN") == 0)
            {
                //syntaxTree->nodeType = nInt;

                //traverse c0 to get its type
                if (syntaxTree->child[0] != NULL)
                {
                    semanticAnalysis(syntaxTree->child[0], symbolTable);
                }
                else
                {
                    printf("ERROR(YOU SHOULD NOT SEE THIS) OpKc0.\n");
                }
                if (syntaxTree->child[0]->isNC || syntaxTree->child[0]->isConstant)
                {
                }
                else if (!(strcmp(syntaxTree->child[0]->attr.name, "--") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "[") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "==") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "-") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "?") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "CHSIGN") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "+") == 0))
                {
                    void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                    }
                }

                if (syntaxTree->child[0]->isArray)
                {
                    printf("ERROR(%d): The operation 'chsign' does not work with arrays.\n", syntaxTree->lineno);
                    numErrors++;
                }
                else
                {
                    switch (syntaxTree->child[0]->nodeType)
                    {
                    case nInt:
                        syntaxTree->nodeType = nInt;
                        break;
                    case nVoid:
                        printf("ERROR(%d): Unary 'chsign' requires an operand of type int but was given type void.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nBool:
                        printf("ERROR(%d): Unary 'chsign' requires an operand of type int but was given type bool.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nChar:
                        printf("ERROR(%d): Unary 'chsign' requires an operand of type int but was given type char.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nUndefinedType:
                        printf("ERROR(%d): Unary 'chsign' requires an operand of type int but was given type undefined.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case uninit:
                        break;
                    default:
                        printf("ERROR: semantic.cpp line 836\n");
                        break;
                    }
                }

                if (syntaxTree->child[0]->isConstant)
                {
                    syntaxTree->isConstant = true; // FactCheck this
                }
            }
            else if (strcmp(syntaxTree->attr.name, "?") == 0)
            {
                //syntaxTree->nodeType = nInt;
                syntaxTree->isConstant = false; // ? is never constant
                //traverse c0 to get its type
                if (syntaxTree->child[0] != NULL)
                {
                    semanticAnalysis(syntaxTree->child[0], symbolTable);
                }
                else
                {
                    printf("ERROR(YOU SHOULD NOT SEE THIS) OpKc0.\n");
                }
                if (syntaxTree->child[0]->isNC || syntaxTree->child[0]->isConstant)
                {
                }
                else if (syntaxTree->child[0]->nodeType == uninit || syntaxTree->child[0]->nodeType == nVoid)
                {
                    void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                    }
                }
                //no c1
                if (syntaxTree->child[0]->isArray)
                {
                    printf("ERROR(%d): The operation '?' does not work with arrays.\n", syntaxTree->lineno);
                    numErrors++;
                }

                switch (syntaxTree->child[0]->nodeType)
                {
                case nInt:
                    syntaxTree->nodeType = nInt;
                    break;
                case nVoid:
                    printf("ERROR(%d): Unary '?' requires an operand of type int but was given type void.\n", syntaxTree->lineno);
                    numErrors++;
                    break;
                case nBool:
                    printf("ERROR(%d): Unary '?' requires an operand of type int but was given type bool.\n", syntaxTree->lineno);
                    numErrors++;
                    break;
                case nChar:
                    printf("ERROR(%d): Unary '?' requires an operand of type int but was given type char.\n", syntaxTree->lineno);
                    numErrors++;
                    break;
                case nUndefinedType:
                    printf("ERROR(%d): Unary '?' requires an operand of type int but was given type undefined.\n", syntaxTree->lineno);
                    numErrors++;
                    break;
                case uninit:
                    break;
                default:
                    printf("ERROR: semantic.cpp line 836\n");
                    break;
                }

            } //and or separate so i can lowercase
            else if (strcmp(syntaxTree->attr.name, "AND") == 0)
            {
                syntaxTree->nodeType = nBool;
                //traverse C0 to get its type
                if (syntaxTree->child[0] != NULL)
                {
                    semanticAnalysis(syntaxTree->child[0], symbolTable);
                }
                else
                {
                    printf("ERROR(YOU SHOULD NOT SEE THIS) ExpKc0.\n");
                }
                if (syntaxTree->child[0]->isNC || syntaxTree->child[0]->isConstant)
                {
                }
                else if (!(strcmp(syntaxTree->child[0]->attr.name, "AND") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "NOT") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "+") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "*") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "SIZEOF") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "<") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, ">") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "<=") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, ">=") == 0)) //(syntaxTree->child[0]->nodeType == uninit || syntaxTree->child[0]->nodeType == nVoid) //if its gonna be in symbol table
                {
                    void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                        syntaxTree->child[0]->isInit = static_cast<TreeNode *>(data)->isInit;
                        if (!syntaxTree->child[0]->isInit)
                        {
                            printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                            numWarnings++;
                            static_cast<TreeNode *>(data)->isInit = true;
                        }
                    }
                }
                //traverse c1 to get its type
                if (syntaxTree->child[1] != NULL)
                {
                    semanticAnalysis(syntaxTree->child[1], symbolTable);
                }
                else
                {
                    printf("ERROR(YOU SHOULD NOT SEE THIS) ExpKc1.\n");
                }
                if (syntaxTree->child[1]->isNC || syntaxTree->child[1]->isConstant)
                {
                }
                else if (!(strcmp(syntaxTree->child[1]->attr.name, "AND") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "*") == 0)) //if its gonna be in symbol table
                {
                    void *data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        //printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[1]->attr.name);
                        //numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[1]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[1]->isArray = static_cast<TreeNode *>(data)->isArray;
                        syntaxTree->child[1]->isInit = static_cast<TreeNode *>(data)->isInit;
                    }
                    if (!syntaxTree->child[1]->isInit)
                    {
                        //printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", syntaxTree->lineno, syntaxTree->child[1]->attr.name);
                        //numWarnings++;
                        //syntaxTree->child[1]->isInit = true;
                    }
                }

                if (syntaxTree->child[0]->isArray || syntaxTree->child[1]->isArray)
                {
                    printf("ERROR(%d): The operation 'and' does not work with arrays.\n", syntaxTree->lineno);
                    numErrors++;
                }
                else
                {
                    switch (syntaxTree->child[0]->nodeType)
                    {
                    case nInt:
                        printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type int.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nVoid:
                        printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type void.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nBool:
                        break;
                    case nChar:
                        printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type char.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nUndefinedType:
                        printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type undefined.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case uninit:
                        break;
                    default:
                        printf("ERROR: semantic.cpp line 836\n");
                        break;
                    }
                    switch (syntaxTree->child[1]->nodeType)
                    {
                    case nInt:
                        printf("ERROR(%d): 'and' requires operands of type bool but rhs is of type int.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nVoid:
                        printf("ERROR(%d): 'and' requires operands of type bool but rhs is of type void.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nBool:
                        break;
                    case nChar:
                        printf("ERROR(%d): 'and' requires operands of type bool but rhs is of type char.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nUndefinedType:
                        printf("ERROR(%d): 'and' requires operands of type bool but rhs is of type undefined.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case uninit:
                        break;
                    default:
                        printf("ERROR: semantic.cpp line 836\n");
                        break;
                    }
                }
                if (syntaxTree->child[0]->isConstant && syntaxTree->child[1]->isConstant)
                {
                    syntaxTree->isConstant = true; // FactCheck this
                }
            }
            else if (strcmp(syntaxTree->attr.name, "OR") == 0)
            {
                syntaxTree->nodeType = nBool;
                //traverse C0 to get its type
                if (syntaxTree->child[0] != NULL)
                {
                    semanticAnalysis(syntaxTree->child[0], symbolTable);
                }
                else
                {
                    printf("ERROR(YOU SHOULD NOT SEE THIS) ExpKc0.\n");
                }
                if (syntaxTree->child[0]->isNC || syntaxTree->child[0]->isConstant)
                {
                }
                else if (!(strcmp(syntaxTree->child[0]->attr.name, "NOT") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, ">") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "==") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "-") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "?") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "OR") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "AND") == 0))
                {
                    void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                    }
                }
                //traverse c1 to get its type
                if (syntaxTree->child[1] != NULL)
                {
                    semanticAnalysis(syntaxTree->child[1], symbolTable);
                }
                else
                {
                    printf("ERROR(YOU SHOULD NOT SEE THIS) ExpKc1.\n");
                }
                if (syntaxTree->child[1]->isNC || syntaxTree->child[1]->isConstant)
                {
                }
                else if (!(strcmp(syntaxTree->child[1]->attr.name, "AND") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "OR") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "NOT") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "-") == 0)) //if its gonna be in symbol table
                {
                    void *data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[1]->attr.name);
                        numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[1]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[1]->isArray = static_cast<TreeNode *>(data)->isArray;
                    }
                }
                if (syntaxTree->child[0]->isArray || syntaxTree->child[1]->isArray)
                {
                    printf("ERROR(%d): The operation 'or' does not work with arrays.\n", syntaxTree->lineno);
                    numErrors++;
                }
                else
                {
                    switch (syntaxTree->child[0]->nodeType)
                    {
                    case nInt:
                        printf("ERROR(%d): 'or' requires operands of type bool but lhs is of type int.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nVoid:
                        printf("ERROR(%d): 'or' requires operands of type bool but lhs is of type void.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nBool:
                        break;
                    case nChar:
                        printf("ERROR(%d): 'or' requires operands of type bool but lhs is of type char.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nUndefinedType:
                        printf("ERROR(%d): 'or' requires operands of type bool but lhs is of type undefined.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case uninit:
                        break;
                    default:
                        printf("ERROR: semantic.cpp line 836\n");
                        break;
                    }
                    switch (syntaxTree->child[1]->nodeType)
                    {
                    case nInt:
                        printf("ERROR(%d): 'or' requires operands of type bool but rhs is of type int.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nVoid:
                        printf("ERROR(%d): 'or' requires operands of type bool but rhs is of type void.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nBool:
                        break;
                    case nChar:
                        printf("ERROR(%d): 'or' requires operands of type bool but rhs is of type char.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nUndefinedType:
                        printf("ERROR(%d): 'or' requires operands of type bool but rhs is of type undefined.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case uninit:
                        break;
                    default:
                        printf("ERROR: semantic.cpp line 836\n");
                        break;
                    }
                }

                if (syntaxTree->child[0]->isConstant && syntaxTree->child[1]->isConstant)
                {
                    syntaxTree->isConstant = true; // FactCheck this
                }
            }
            //same type + Array -> bools: ==, !=, <=, <, >, >=
            else if ((strcmp(syntaxTree->attr.name, "==") == 0) || (strcmp(syntaxTree->attr.name, "!=") == 0) ||
                     (strcmp(syntaxTree->attr.name, "<=") == 0) || (strcmp(syntaxTree->attr.name, ">=") == 0) ||
                     (strcmp(syntaxTree->attr.name, "<") == 0) || (strcmp(syntaxTree->attr.name, ">") == 0))
            {

                bool c0decl = true;
                bool c1decl = true;
                syntaxTree->nodeType = nBool;
                //traverse C0 to get its type
                if (syntaxTree->child[0] != NULL)
                {
                    semanticAnalysis(syntaxTree->child[0], symbolTable);
                }
                else
                {
                    printf("ERROR(YOU SHOULD NOT SEE THIS) OpKc0.\n");
                }

                if (syntaxTree->child[0]->isNC || syntaxTree->child[0]->isConstant)
                {
                }
                else if (!(strcmp(syntaxTree->child[0]->attr.name, "%") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "=") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "-") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "*") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "+") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "[") == 0)) //if () //if its gonna be in symbol table
                {
                    void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        //printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        //numErrors++;
                        c0decl = false;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[0]->isInit = static_cast<TreeNode *>(data)->isInit;
                        syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                        if (!syntaxTree->child[0]->isInit)
                        {
                            printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                            numWarnings++;
                            static_cast<TreeNode *>(data)->isInit = true;
                        }
                    }
                }

                //traverse c1 to get its type
                if (syntaxTree->child[1] != NULL)
                {
                    semanticAnalysis(syntaxTree->child[1], symbolTable);
                }
                else
                {
                    printf("ERROR(YOU SHOULD NOT SEE THIS) OpKc1.\n");
                }
                if (syntaxTree->child[1]->isNC || syntaxTree->child[1]->isConstant)
                {
                }
                else if (!(strcmp(syntaxTree->child[1]->attr.name, "+") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "*") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "[") == 0)) //(syntaxTree->child[1]->nodeType == uninit) //if its gonna be in symbol table
                {
                    void *data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        //printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[1]->attr.name);
                        //numErrors++;
                        c1decl = false;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[1]->isInit = static_cast<TreeNode *>(data)->isInit;
                        syntaxTree->child[1]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[1]->isArray = static_cast<TreeNode *>(data)->isArray;
                        if (!syntaxTree->child[1]->isInit)
                        {
                            printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", syntaxTree->lineno, syntaxTree->child[1]->attr.name);
                            numWarnings++;
                            static_cast<TreeNode *>(data)->isInit = true;
                        }
                    }
                }
                //syntaxTree->nodeType = nBool;
                bool twoArrays = false;
                //both arrays
                if (syntaxTree->child[0]->isArray && syntaxTree->child[1]->isArray)
                {
                    twoArrays = true;
                    syntaxTree->isArray;
                }
                //rhs not array
                else if (syntaxTree->child[0]->isArray && !syntaxTree->child[1]->isArray)
                {
                    printf("ERROR(%d): '%s' requires both operands be arrays or not but lhs is an array and rhs is not an array.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                }
                //lhs not array
                else if ((!syntaxTree->child[0]->isArray && syntaxTree->child[1]->isArray))
                {
                    printf("ERROR(%d): '%s' requires both operands be arrays or not but lhs is not an array and rhs is an array.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                }
                //both not arrays, or both arrays
                /*
                if ((!syntaxTree->child[0]->isArray && !syntaxTree->child[1]->isArray) || twoArrays)
                {*/
                //should have both types, now compare, output error if not equal types.

                if (syntaxTree->child[0]->nodeType == syntaxTree->child[1]->nodeType)
                {
                    syntaxTree->nodeType = nBool;
                }
                else if (c1decl && c0decl)
                {
                    printf("ERROR(%d): '%s' requires operands of the same type but lhs is type ", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    switch (syntaxTree->child[0]->nodeType)
                    {
                    case nInt:
                        printf("int");
                        break;
                    case nVoid:
                        printf("void");
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
                    case uninit:
                        break;
                    default:
                        printf("ERROR: semantic.cpp line 836\n");
                        break;
                    }
                    printf(" and rhs is type ");
                    switch (syntaxTree->child[1]->nodeType)
                    {
                    case nInt:
                        printf("int");
                        break;
                    case nVoid:
                        printf("void");
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
                        printf("ERROR: semantic.cpp line 836\n");
                        break;
                    }
                    printf(".\n");
                }
                if (syntaxTree->child[0]->isConstant && syntaxTree->child[1]->isConstant)
                {
                    syntaxTree->isConstant = true; // FactCheck this
                }
            }
            //int,int -> ints: :>:, :<:, *, +, /, -, %
            else if ((strcmp(syntaxTree->attr.name, ":>:") == 0) || (strcmp(syntaxTree->attr.name, ":<:") == 0) ||
                     (strcmp(syntaxTree->attr.name, "*") == 0) || (strcmp(syntaxTree->attr.name, "+") == 0) ||
                     (strcmp(syntaxTree->attr.name, "/") == 0) || (strcmp(syntaxTree->attr.name, "-") == 0) ||
                     (strcmp(syntaxTree->attr.name, "%") == 0))
            {
                syntaxTree->nodeType = nInt;
                //traverse C0
                if (syntaxTree->child[0] != NULL)
                {
                    semanticAnalysis(syntaxTree->child[0], symbolTable);
                }
                else
                {
                    printf("ERROR(YOU SHOULD NOT SEE THIS) OpKc0.\n");
                }

                //do stuff with c0
                if (syntaxTree->child[0]->isNC || syntaxTree->child[0]->isConstant)
                {
                }
                else if (!(strcmp(syntaxTree->child[0]->attr.name, "%") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "[") == 0)) //if its gonna be in symbol table
                {
                    void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        //printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        //numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                        syntaxTree->child[0]->isInit = static_cast<TreeNode *>(data)->isInit;
                        if (!syntaxTree->child[0]->isInit)
                        {
                            printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                            numWarnings++;
                            static_cast<TreeNode *>(data)->isInit = true;
                        }
                    }
                }

                //traverse c1
                if (syntaxTree->child[1] != NULL)
                {
                    semanticAnalysis(syntaxTree->child[1], symbolTable);
                }
                else
                {
                    printf("ERROR(YOU SHOULD NOT SEE THIS) OpKc1.\n");
                }

                //do stuff with c1
                if (syntaxTree->child[1]->isNC || syntaxTree->child[1]->isConstant)
                {
                }
                else if (!(strcmp(syntaxTree->child[1]->attr.name, "%") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "[") == 0)) //if its gonna be in symbol table
                {
                    void *data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        //printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[1]->attr.name);
                        //numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[1]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[1]->isArray = static_cast<TreeNode *>(data)->isArray;
                        syntaxTree->child[1]->isInit = static_cast<TreeNode *>(data)->isInit;
                        if (!syntaxTree->child[1]->isInit)
                        {
                            printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", syntaxTree->lineno, syntaxTree->child[1]->attr.name);
                            numWarnings++;
                            static_cast<TreeNode *>(data)->isInit = true;
                        }
                    }
                }
                if (syntaxTree->child[0]->isArray || syntaxTree->child[1]->isArray)
                {
                    printf("ERROR(%d): The operation '%s' does not work with arrays.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                }
                //was an else ehere
                //should have both types, now compare, output error if not equal types.

                switch (syntaxTree->child[0]->nodeType)
                {
                case nInt:
                    break;
                case nVoid:
                    printf("ERROR(%d): '%s' requires operands of type int but lhs is of type void.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case nBool:
                    printf("ERROR(%d): '%s' requires operands of type int but lhs is of type bool.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case nChar:
                    printf("ERROR(%d): '%s' requires operands of type int but lhs is of type char.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case nUndefinedType:
                    printf("ERROR(%d): '%s' requires operands of type int but lhs is of type undefined.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case uninit:
                    break;
                default:
                    printf("ERROR: semantic.cpp line 836\n");
                    break;
                }
                switch (syntaxTree->child[1]->nodeType)
                {
                case nInt:
                    break;
                case nVoid:
                    printf("ERROR(%d): '%s' requires operands of type int but rhs is of type void.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case nBool:
                    printf("ERROR(%d): '%s' requires operands of type int but rhs is of type bool.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case nChar:
                    printf("ERROR(%d): '%s' requires operands of type int but rhs is of type char.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case nUndefinedType:
                    printf("ERROR(%d): '%s' requires operands of type int but rhs is of type undefined.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case uninit:
                    break;
                default:
                    printf("ERROR: semantic.cpp line 836\n");
                    break;
                }
                if (syntaxTree->child[0]->isConstant && syntaxTree->child[1]->isConstant)
                {
                    syntaxTree->isConstant = true; // FactCheck this
                }
            }
            else // '[' , array,int ->type of array
            {

                //syntaxTree->nodeType = nInt;
                //traverse C0
                bool iserr = false;
                if (syntaxTree->child[0] != NULL)
                {
                    semanticAnalysis(syntaxTree->child[0], symbolTable);
                }
                else
                {
                    printf("ERROR(YOU SHOULD NOT SEE THIS) OpKc0.\n");
                }

                bool temp = false;
                void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                if (data != NULL)
                {
                    temp = static_cast<TreeNode *>(data)->isInit;
                    if (syntaxTree->child[1]->isNC)
                    {
                        static_cast<TreeNode *>(data)->isInit = true;
                    }
                }
                if (syntaxTree->child[0]->isNC || syntaxTree->child[0]->isConstant)
                {
                }
                else if ((data != NULL) && !disableW && !temp && !(strcmp(syntaxTree->child[0]->attr.name, "[") == 0)) // if c1 isnt inited, and isnt '['
                {
                    printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                    numWarnings++;
                    static_cast<TreeNode *>(data)->isInit = true;
                    syntaxTree->child[0]->isInit = true;
                }

                // Mess with c0
                syntaxTree->child[0]->isInit = true; //MAYBE SHOULDNT BE HERE
                if (syntaxTree->child[0]->isNC || syntaxTree->child[0]->isConstant)
                {
                }
                else // if (syntaxTree->child[0]->nodeType == uninit || syntaxTree->child[0]->nodeType == nVoid) //if its gonna be in symbol table
                {
                    void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        //printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        //numErrors++;
                        syntaxTree->child[0]->isArray = false;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[0]->isInit = static_cast<TreeNode *>(data)->isInit;
                        syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                        if (!syntaxTree->child[0]->isInit)
                        {
                            printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                            numWarnings++;
                            static_cast<TreeNode *>(data)->isInit = true;
                        }
                    }
                }
                syntaxTree->nodeType = syntaxTree->child[0]->nodeType;
                //traverse c1
                if (syntaxTree->child[1] != NULL)
                {
                    semanticAnalysis(syntaxTree->child[1], symbolTable);
                }
                else
                {
                    printf("ERROR(YOU SHOULD NOT SEE THIS) OpKc1.\n");
                }

                if (syntaxTree->child[1]->isNC || syntaxTree->child[1]->isConstant)
                {
                    //if(syntaxTree->child[1]->isNC){ syntaxTree->nodeType = syntaxTree->child[0]->nodeType;}
                }
                else
                {
                    temp = false;
                    data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                    if (data != NULL)
                    {
                        temp = static_cast<TreeNode *>(data)->isInit;
                    }
                }
                //mess with c1
                if (syntaxTree->child[1]->isNC || syntaxTree->child[1]->isConstant)
                {
                }
                else if (!temp && !(strcmp(syntaxTree->child[1]->attr.name, "[") == 0)) // if c1 isnt inited, and isnt '['
                {
                    //printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", syntaxTree->lineno, syntaxTree->child[1]->attr.name);
                    //numWarnings++;
                    syntaxTree->child[1]->isInit = true;
                }
                if (syntaxTree->child[1]->isNC || syntaxTree->child[1]->isConstant)
                {
                    syntaxTree->isArray = false;
                }
                else if (!(strcmp(syntaxTree->child[1]->attr.name, "[") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "SIZEOF") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "*") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "+") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "-") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "?") == 0)) //if its gonna be in symbol table
                {
                    void *data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        //printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[1]->attr.name);
                        //numErrors++;
                        syntaxTree->isArray = true;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        //static_cast<TreeNode *>(data)->isInit = true;
                        syntaxTree->child[1]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[1]->isArray = static_cast<TreeNode *>(data)->isArray;
                        syntaxTree->child[1]->isFun = static_cast<TreeNode *>(data)->isFun;
                    }
                    if (data)
                    {
                        if (static_cast<TreeNode *>(data)->isArray)
                        {
                            printf("ERROR(%d): Array index is the unindexed array '%s'.\n", syntaxTree->lineno, static_cast<TreeNode *>(data)->attr.name);
                            numErrors++;
                        }
                    }
                }
                // now work with both
                if (!syntaxTree->child[0]->isArray)
                {
                    printf("ERROR(%d): Cannot index nonarray '%s'.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                    numErrors++;
                }
                if (!syntaxTree->child[1]->isFun)
                {
                    switch (syntaxTree->child[1]->nodeType)
                    {
                    case nInt:
                        break;
                    case nVoid:
                        printf("ERROR(%d): Array '%s' should be indexed by type int but got type void.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        numErrors++;
                        break;
                    case nBool:
                        printf("ERROR(%d): Array '%s' should be indexed by type int but got type bool.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        numErrors++;
                        break;
                    case nChar:
                        printf("ERROR(%d): Array '%s' should be indexed by type int but got type char.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        numErrors++;
                        break;
                    case nUndefinedType:
                        printf("ERROR(%d): Array '%s' should be indexed by type int but got type undefined.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        numErrors++;
                        break;
                    case uninit:
                        break;
                    default:
                        printf("ERROR: semantic.cpp line 836\n");
                        break;
                    }
                }
            }
            break;
        }
        case ConstantK:
        {
            //no children
            if (syntaxTree->isStringConstant)
            {
                syntaxTree->location = goffset - 1;
                syntaxTree->mem = glob;
                goffset = goffset - syntaxTree->size;
            }
            break;
        }
        case IdK:
        {
            void *data = symbolTable.lookup(syntaxTree->attr.name);
            if (data != NULL)
            {
                static_cast<TreeNode *>(data)->isUsed = true;
                syntaxTree->nodeType = static_cast<TreeNode *>(data)->nodeType;
                syntaxTree->isArray = static_cast<TreeNode *>(data)->isArray;
                syntaxTree->isInit = static_cast<TreeNode *>(data)->isInit;
                syntaxTree->size = static_cast<TreeNode *>(data)->size;
                syntaxTree->mem = static_cast<TreeNode *>(data)->mem;
                syntaxTree->location = static_cast<TreeNode *>(data)->location;
                switch (static_cast<TreeNode *>(data)->subkind.decl)
                {
                case FuncK:
                    printf("ERROR(%d): Cannot use function '%s' as a variable.\n", syntaxTree->lineno, static_cast<TreeNode *>(data)->attr.name);
                    numErrors++;
                    static_cast<TreeNode *>(data)->isFun = true;
                    break;
                default:
                    break;
                }
            }
            else
            {
                printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->attr.name);
                numErrors++;
            }
            //No Children
            break;
        }
        case AssignK:
        {
            void *dataSave = NULL;
            void *dataSave2 = NULL;
            bool iserr = false;                          //"ERROR(%d): '%s' requires operands of the same type but lhs is %s and rhs is %s.\n"
            if (strcmp(syntaxTree->attr.name, "=") == 0) // =
            {
                //syntaxTree->nodeType = nBool;
                //traverse C0 to get its type
                bool c0Decl, c1Decl;
                c0Decl = c1Decl = true;
                if (strcmp(syntaxTree->child[0]->attr.name, "[") == 0)
                {
                    disableW = true;
                }
                if (syntaxTree->child[0] != NULL)
                {
                    semanticAnalysis(syntaxTree->child[0], symbolTable);
                }
                else
                {
                    printf("ERROR(YOU SHOULD NOT SEE THIS) AssignKc0.\n");
                }
                //traverse c1 to get its type
                if (syntaxTree->child[1] != NULL)
                {
                    semanticAnalysis(syntaxTree->child[1], symbolTable);
                }
                else
                {
                    printf("ERROR(YOU SHOULD NOT SEE THIS) AssignKc1.\n");
                }
                disableW = false;
                syntaxTree->child[0]->isInit = true;
                if (syntaxTree->child[0]->isNC || syntaxTree->child[0]->isConstant)
                {
                }
                else // if ((syntaxTree->child[0]->nodeType == uninit || syntaxTree->child[0]->nodeType == nVoid)) //if its gonna be in symbol table
                {
                    if (!(strcmp(syntaxTree->child[0]->attr.name, "[") == 0))
                    {
                        void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                        if (data == NULL)
                        { //should this error be here?

                            //printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                            c0Decl = false;
                            iserr = true;
                            //numErrors++;
                        }
                        else
                        {
                            syntaxTree->child[0]->isInit = true;
                            syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                            syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                            static_cast<TreeNode *>(data)->isUsed = true;
                            dataSave = data;
                        }
                    }
                    else
                    {
                        c0Decl = false;
                    }
                }

                if (syntaxTree->child[1]->isNC || syntaxTree->child[1]->isConstant)
                {
                }
                else if (!(strcmp(syntaxTree->child[1]->attr.name, "==") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "[") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "=") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "!=") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "%") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "+") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "AND") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "+=") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "NOT") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "CHSIGN") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "+=") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "--") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "SIZEOF") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "OR") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "*") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "+") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "-") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "?") == 0)) // if (syntaxTree->child[1]->nodeType == uninit) // || syntaxTree->child[1]->nodeType == nVoid) //if its gonna be in symbol table
                {
                    void *data;
                    data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        iserr = true;
                        if (!(strcmp(syntaxTree->child[1]->attr.name, "[") == 0))
                        {
                            //printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[1]->attr.name);
                            c1Decl = false;
                            //numErrors++;
                        }
                    }
                    else
                    {
                        //printf("%s\n", static_cast<TreeNode *>(data)->attr.name);
                        if ((strcmp(syntaxTree->child[1]->attr.name, "[") == 0))
                        {
                            if (!syntaxTree->child[1]->isArray)
                            {
                                static_cast<TreeNode *>(data)->isUsed = true;
                                dataSave2 = data;
                                syntaxTree->child[1]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                                syntaxTree->child[1]->isArray = static_cast<TreeNode *>(data)->isArray;
                                syntaxTree->child[1]->isInit = static_cast<TreeNode *>(data)->isInit;
                            }
                            else
                            {
                                static_cast<TreeNode *>(data)->isUsed = true;
                                dataSave2 = data;
                                syntaxTree->child[1]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                                syntaxTree->child[1]->isArray = static_cast<TreeNode *>(data)->isArray;
                                syntaxTree->child[1]->isInit = static_cast<TreeNode *>(data)->isInit;
                            }
                        }
                        else
                        {
                            static_cast<TreeNode *>(data)->isUsed = true;
                            dataSave2 = data;
                            syntaxTree->child[1]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                            syntaxTree->child[1]->isArray = static_cast<TreeNode *>(data)->isArray;
                            syntaxTree->child[1]->isInit = static_cast<TreeNode *>(data)->isInit;
                        }
                    }
                    if (!syntaxTree->child[1]->isInit && !(strcmp(syntaxTree->child[1]->attr.name, "[") == 0) && c1Decl) // && !iserr) // if c1 isnt inited, and isnt '['
                    {
                        printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", syntaxTree->lineno, syntaxTree->child[1]->attr.name);
                        numWarnings++;
                        syntaxTree->child[1]->isInit = true;
                    }
                    if (syntaxTree->child[0] != NULL)
                    {
                        void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                        if (data)
                        {
                            static_cast<TreeNode *>(data)->isInit = true;
                        }
                    }
                    if (dataSave2 != NULL)
                    {
                        static_cast<TreeNode *>(dataSave2)->isInit = true;
                    }
                }
                if (dataSave != NULL)
                {
                    static_cast<TreeNode *>(dataSave)->isInit = true;
                }
                bool twoArrays = false;
                //both arrays
                if (iserr)
                {
                }
                else if (c0Decl)
                {
                    if (syntaxTree->child[0]->isArray && syntaxTree->child[1]->isArray)
                    {
                        twoArrays = true;
                        syntaxTree->isArray = true;
                    }
                    //rhs not array
                    else if (syntaxTree->child[0]->isArray && !syntaxTree->child[1]->isArray)
                    {
                        printf("ERROR(%d): '=' requires both operands be arrays or not but lhs is an array and rhs is not an array.\n", syntaxTree->lineno);
                        numErrors++;
                    }
                    //lhs not array
                    else if ((!syntaxTree->child[0]->isArray && syntaxTree->child[1]->isArray))
                    {
                        printf("ERROR(%d): '=' requires both operands be arrays or not but lhs is not an array and rhs is an array.\n", syntaxTree->lineno);
                        numErrors++;
                    }
                    //both not arrays, or both arrays
                    if (1) //((!syntaxTree->child[0]->isArray && !syntaxTree->child[1]->isArray) || twoArrays)
                    {
                        //should have both types, now compare, output error if not equal types.
                        if (syntaxTree->child[0]->nodeType == syntaxTree->child[1]->nodeType)
                        {
                            syntaxTree->nodeType = syntaxTree->child[0]->nodeType;
                        }
                        else //if(syntaxTree->child[0]->nodeType != uninit && syntaxTree->child[1]->nodeType != uninit)
                        {
                            syntaxTree->nodeType = syntaxTree->child[0]->nodeType;
                            printf("ERROR(%d): '=' requires operands of the same type but lhs is type ", syntaxTree->lineno);
                            numErrors++;
                            switch (syntaxTree->child[0]->nodeType)
                            {
                            case nInt:
                                printf("int");
                                break;
                            case nVoid:
                                printf("void");
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
                            case uninit:
                                break;
                            default:
                                //printf("ERROR: semantic.cpp line 836\n");
                                break;
                            }
                            printf(" and rhs is type ");
                            switch (syntaxTree->child[1]->nodeType)
                            {
                            case nInt:
                                printf("int");
                                break;
                            case nVoid:
                                printf("void");
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
                                printf("void");
                                break;
                            }
                            printf(".\n");
                        }
                    }
                }
            }
            else if (strcmp(syntaxTree->attr.name, "++") == 0 || strcmp(syntaxTree->attr.name, "--") == 0) //separate cause no c1
            {
                //syntaxTree->nodeType = nInt; //maybe delete this

                if (syntaxTree->child[0] != NULL)
                {
                    semanticAnalysis(syntaxTree->child[0], symbolTable);
                }
                else
                {
                    printf("ERROR(YOU SHOULD NOT SEE THIS) AssignKc0.\n");
                }
                //syntaxTree->child[0]->isInit = true;

                if (syntaxTree->child[0]->isNC || syntaxTree->child[0]->isConstant)
                {
                }
                else if (!(strcmp(syntaxTree->child[0]->attr.name, "[") == 0)) // if (syntaxTree->child[0]->nodeType == uninit || syntaxTree->child[0]->nodeType == nVoid) //if its gonna be in symbol table
                {

                    void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[0]->isInit = static_cast<TreeNode *>(data)->isInit;
                        syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                        if (!syntaxTree->child[0]->isInit)
                        {
                            printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                            numWarnings++;
                            static_cast<TreeNode *>(data)->isInit = true;
                        }
                    }
                }
                //array check
                if (syntaxTree->child[0]->isArray)
                {
                    printf("ERROR(%d): The operation '%s' does not work with arrays.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                }
                switch (syntaxTree->child[0]->nodeType)
                {
                case nInt:
                    syntaxTree->nodeType = nInt;
                    break;
                case nBool:
                    printf("ERROR(%d): Unary '%s' requires an operand of type int but was given type bool.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case nChar:
                    printf("ERROR(%d): Unary '%s' requires an operand of type int but was given type char.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case nVoid:
                    printf("ERROR(%d): Unary '%s' requires an operand of type int but was given type void.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                default:
                    break;
                }
            }
            else // +=, -=, *= /=, separate from = cause these dont require arrays
            {
                //syntaxTree->nodeType = nInt; //will always be int, may as well set it

                //traverse C0 to get its type
                if (syntaxTree->child[0] != NULL)
                {
                    semanticAnalysis(syntaxTree->child[0], symbolTable);
                }
                else
                {
                    printf("ERROR(YOU SHOULD NOT SEE THIS) AssignKc0.\n");
                }
                syntaxTree->child[0]->isInit = true;
                if (syntaxTree->child[0]->isNC || syntaxTree->child[0]->isConstant)
                {
                }
                else if (!(strcmp(syntaxTree->child[0]->attr.name, "?") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "+=") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "%") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "/") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "*") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "-=") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "/=") == 0) && !(strcmp(syntaxTree->child[0]->attr.name, "[") == 0))
                {
                    void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        if (!(strcmp(syntaxTree->child[0]->attr.name, "[") == 0))
                        {
                            printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                            numErrors++;
                        }
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        static_cast<TreeNode *>(data)->isInit = true;
                        syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                    }
                }
                //c0 check;
                switch (syntaxTree->child[0]->nodeType)
                {
                case nInt:
                    syntaxTree->nodeType = nInt;
                    break;
                case nBool:
                    printf("ERROR(%d): '%s' requires operands of type int but lhs is of type bool.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case nChar:
                    printf("ERROR(%d): '%s' requires operands of type int but lhs is of type char.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case nVoid:
                    printf("ERROR(%d): '%s' requires operands of type int but lhs is of type void.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case nUndefinedType:
                    printf("ERROR(%d): '%s' requires operands of type int but lhs is of type undefined.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                default:
                    //printf("bad news assignk\n");
                    break;
                }
                //traverse c1 to get its type
                if (syntaxTree->child[1] != NULL)
                {
                    semanticAnalysis(syntaxTree->child[1], symbolTable);
                }
                else
                {
                    printf("ERROR(YOU SHOULD NOT SEE THIS) AssignKc1.\n");
                }
                if (syntaxTree->child[1]->isNC || syntaxTree->child[1]->isConstant)
                {
                }
                else if (!(strcmp(syntaxTree->child[1]->attr.name, "?") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "+=") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "+") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "/=") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "/") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "%") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "*") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "*=") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "-=") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "++") == 0)) //(syntaxTree->child[1]->nodeType == uninit) //if its gonna be in symbol table
                {
                    void *data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[1]->attr.name);
                        numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[1]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[1]->isArray = static_cast<TreeNode *>(data)->isArray;
                        syntaxTree->child[1]->isInit = static_cast<TreeNode *>(data)->isInit;
                    }
                }
                if (syntaxTree->child[1]->isNC || syntaxTree->isConstant)
                {
                }
                else if (!syntaxTree->child[1]->isInit && !(strcmp(syntaxTree->child[1]->attr.name, "[") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "[") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "/=") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "%") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "AND") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "OR") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "==") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "!=") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "<=") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "<") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, ">") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, ">=") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, ":>:") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, ":<:") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "*") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "+") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "-") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "/") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "+=") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "NOT") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "SIZEOF") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "CHSIGN") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "?") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "=") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "-=") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "*=") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "--") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "++") == 0)) // if c1 isnt inited, and isnt '['
                {
                    printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", syntaxTree->lineno, syntaxTree->child[1]->attr.name);
                    numWarnings++;
                    syntaxTree->child[1]->isInit = true;
                }

                //c1 check;
                switch (syntaxTree->child[1]->nodeType)
                {
                case nInt:
                    syntaxTree->nodeType = nInt;
                    break;
                case nBool:
                    printf("ERROR(%d): '%s' requires operands of type int but rhs is of type bool.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case nChar:
                    printf("ERROR(%d): '%s' requires operands of type int but rhs is of type char.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case nVoid:
                    printf("ERROR(%d): '%s' requires operands of type int but rhs is of type void.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case nUndefinedType:
                    printf("ERROR(%d): '%s' requires operands of type int but rhs is of type undefined.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                default:
                    //printf("bad news assignk\n");
                    break;
                }
            }
            //
            /*
            if (syntaxTree->child[0] != NULL)
            {
                semanticAnalysis(syntaxTree->child[0], symbolTable);
            }
            else
            {
                printf("ERROR(YOU SHOULD NOT SEE THIS) AssignKc0.\n");
            }
            if (syntaxTree->child[1] != NULL)
            {
                semanticAnalysis(syntaxTree->child[1], symbolTable);
            }
            else
            {
                //allowed for -- and ++
            }
            */
            break;
        }
        case InitK:
        {
            printf("I THINK THIS CAN BE DELETED? WAS IN SUPPLIED CLASS FILE\n");
            break;
        }
        case CallK:
        {
            int funNparam = 0;
            int callNparam = 0;
            int lowerLim = 0;
            bool noFcallCheck = false;
            TreeNode *funcT = new TreeNode;
            TreeNode *callT = new TreeNode;
            if (syntaxTree->child[0] != NULL)
            {
                semanticAnalysis(syntaxTree->child[0], symbolTable);
            }
            void *data = symbolTable.lookup(syntaxTree->attr.name); //symbolTable.lookupGlobal(syntaxTree->attr.name);
            if (data == NULL)
            {
                data = symbolTable.lookupGlobal(syntaxTree->attr.name);
            }
            if (data == NULL)
            {
                noFcallCheck = true;
                printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->attr.name);
                numErrors++;
            }
            else
            {
                static_cast<TreeNode *>(data)->isUsed = true;
                //now, construct two small trees, one for the fun decl, and one for our call to compare.
                funcT->attr.name = static_cast<TreeNode *>(data)->attr.name;
                funcT->lineno = static_cast<TreeNode *>(data)->lineno;
                funcT->nodeType = static_cast<TreeNode *>(data)->nodeType;
                syntaxTree->nodeType = static_cast<TreeNode *>(data)->nodeType;
                if (static_cast<TreeNode *>(data)->child[0] != NULL)
                {
                    funNparam = 1;
                    funcT->child[0] = static_cast<TreeNode *>(data)->child[0];
                }
                else
                {
                    funcT->child[0] = NULL;
                }
                callT->attr.name = syntaxTree->attr.name;
                callT->lineno = syntaxTree->lineno;
                callT->nodeType = syntaxTree->nodeType;
                if (syntaxTree->child[0] != NULL)
                {
                    callNparam = 1;
                    callT->child[0] = syntaxTree->child[0];
                }
                else
                {
                    callT->child[0] = NULL;
                }

                if (funcT->child[0] != NULL)
                {
                    TreeNode *temp = funcT->child[0];
                    while (temp->sibling != NULL)
                    {
                        funNparam++;
                        temp = temp->sibling;
                    }
                }
                if (callT->child[0] != NULL)
                {
                    TreeNode *temp = callT->child[0];
                    //callNparam++;
                    while (temp->sibling != NULL)
                    {
                        callNparam++;
                        if (!temp->isConstant && !(strcmp(temp->attr.name, "[") == 0) && !(strcmp(temp->attr.name, "%") == 0) && !(strcmp(temp->attr.name, "*") == 0) && !(strcmp(temp->attr.name, "AND") == 0) && !(strcmp(temp->attr.name, "=") == 0) && !(strcmp(temp->attr.name, "+") == 0) && !(strcmp(temp->attr.name, "-") == 0) && !(strcmp(temp->attr.name, "SIZEOF") == 0) && !(strcmp(temp->attr.name, "<") == 0) && !(strcmp(temp->attr.name, "==") == 0) && !(strcmp(temp->attr.name, "<=") == 0) && !(strcmp(temp->attr.name, "!=") == 0) && !(strcmp(temp->attr.name, "OR") == 0) && !(strcmp(temp->attr.name, "CHSIGN") == 0) && !(strcmp(temp->attr.name, "NOT") == 0) && !(strcmp(temp->attr.name, "/") == 0) && !(strcmp(temp->attr.name, "=") == 0) && !(strcmp(temp->attr.name, "+=") == 0) && !(strcmp(temp->attr.name, "-=") == 0) && !(strcmp(temp->attr.name, "*=") == 0) && !(strcmp(temp->attr.name, "/=") == 0) && !(strcmp(temp->attr.name, "++") == 0) && !(strcmp(temp->attr.name, "--") == 0) && !(strcmp(temp->attr.name, ">=") == 0) && !(strcmp(temp->attr.name, ">") == 0))
                        {
                            void *data = symbolTable.lookup(temp->attr.name);
                            if (data != NULL)
                            {
                                temp->nodeType = static_cast<TreeNode *>(data)->nodeType;
                                temp->isArray = static_cast<TreeNode *>(data)->isArray;
                                temp->isInit = static_cast<TreeNode *>(data)->isInit;
                                temp->isDeclared = true;
                            }
                            else
                            {
                                //printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, temp->attr.name);
                                //numErrors++;
                                temp->isDeclared = false;
                            }
                        }
                        temp = temp->sibling;
                    }
                    if (!temp->isConstant && !(strcmp(temp->attr.name, "[") == 0) && !(strcmp(temp->attr.name, "%") == 0) && !(strcmp(temp->attr.name, "*") == 0) && !(strcmp(temp->attr.name, "AND") == 0) && !(strcmp(temp->attr.name, "=") == 0) && !(strcmp(temp->attr.name, "+") == 0) && !(strcmp(temp->attr.name, "-") == 0) && !(strcmp(temp->attr.name, "SIZEOF") == 0) && !(strcmp(temp->attr.name, "<") == 0) && !(strcmp(temp->attr.name, "==") == 0) && !(strcmp(temp->attr.name, "<=") == 0) && !(strcmp(temp->attr.name, "!=") == 0) && !(strcmp(temp->attr.name, "OR") == 0) && !(strcmp(temp->attr.name, "CHSIGN") == 0) && !(strcmp(temp->attr.name, "NOT") == 0) && !(strcmp(temp->attr.name, "/") == 0) && !(strcmp(temp->attr.name, "=") == 0) && !(strcmp(temp->attr.name, "+=") == 0) && !(strcmp(temp->attr.name, "-=") == 0) && !(strcmp(temp->attr.name, "*=") == 0) && !(strcmp(temp->attr.name, "/=") == 0) && !(strcmp(temp->attr.name, "++") == 0) && !(strcmp(temp->attr.name, "--") == 0) && !(strcmp(temp->attr.name, ">=") == 0) && !(strcmp(temp->attr.name, ">") == 0))
                    {
                        void *data = symbolTable.lookup(temp->attr.name);
                        if (data != NULL)
                        {
                            temp->nodeType = static_cast<TreeNode *>(data)->nodeType;
                            temp->isArray = static_cast<TreeNode *>(data)->isArray;
                            temp->isInit = static_cast<TreeNode *>(data)->isInit;
                            temp->isDeclared = true;
                        }
                        else
                        {
                            //printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, temp->attr.name);
                            //temp->isDeclared = false;
                            //numErrors++;
                        }
                    }
                }
                void *tdata = NULL;
                bool testing = false;
                tdata = symbolTable.lookup(syntaxTree->attr.name);
                if (tdata == NULL) //temporary bad fix, cant think of good solution at the moment
                {
                    if (!(strcmp(syntaxTree->attr.name, "zog") == 0))
                    {
                        symbolTable.insert(syntaxTree->attr.name, syntaxTree);
                    }
                }
                else
                {
                    switch (static_cast<TreeNode *>(tdata)->subkind.decl)
                    {
                    case FuncK:
                        break;
                    case VarK:
                        printf("ERROR(%d): '%s' is a simple variable and cannot be called.\n", syntaxTree->lineno, syntaxTree->attr.name);
                        numErrors++;
                        testing = true;
                        break;
                    case ParamK:
                        printf("ERROR(%d): '%s' is a simple variable and cannot be called.\n", syntaxTree->lineno, syntaxTree->attr.name);
                        testing = true;
                        numErrors++;
                        break;
                    default:
                        //printf("not good\n");
                        break;
                    }
                }

                if (callNparam < funNparam)
                {
                    lowerLim = callNparam;
                    if (!testing)
                    {
                        printf("ERROR(%d): Too few parameters passed for function '%s' declared on line %d.\n", syntaxTree->lineno, syntaxTree->attr.name, funcT->lineno);
                        numErrors++;
                    }
                }
                else if (funNparam < callNparam)
                {
                    lowerLim = funNparam;
                    if (!testing)
                    {
                        printf("ERROR(%d): Too many parameters passed for function '%s' declared on line %d.\n", syntaxTree->lineno, syntaxTree->attr.name, funcT->lineno);
                        numErrors++;
                    }
                }
                else
                {
                    lowerLim = funNparam;
                }

                //printf("%s(%d) | f: %d | c: %d | ll: %d\n", syntaxTree->attr.name, syntaxTree->lineno, funNparam, callNparam, lowerLim);
                if (lowerLim)
                {
                    TreeNode *t1 = funcT->child[0];
                    TreeNode *t2 = callT->child[0];
                    bool wantContinue = true;
                    //maybe check initialize above
                    for (int i = 0; i < lowerLim; i++)
                    {
                        if (t2->isDeclared && !t2->isInit && !t2->isConstant && !(strcmp(t2->attr.name, "[") == 0) && !(strcmp(t2->attr.name, "%") == 0) && !(strcmp(t2->attr.name, "*") == 0) && !(strcmp(t2->attr.name, "[") == 0) && !(strcmp(t2->attr.name, "AND") == 0) && !(strcmp(t2->attr.name, "=") == 0) && !(strcmp(t2->attr.name, "[") == 0) && !(strcmp(t2->attr.name, "+") == 0) && !(strcmp(t2->attr.name, "-") == 0) && !(strcmp(t2->attr.name, "SIZEOF") == 0) && !(strcmp(t2->attr.name, "<") == 0) && !(strcmp(t2->attr.name, "==") == 0) && !(strcmp(t2->attr.name, "<=") == 0) && !(strcmp(t2->attr.name, "!=") == 0) && !(strcmp(t2->attr.name, "OR") == 0) && !(strcmp(t2->attr.name, "CHSIGN") == 0) && !(strcmp(t2->attr.name, "NOT") == 0) && !(strcmp(t2->attr.name, "/") == 0) && !(strcmp(t2->attr.name, "=") == 0) && !(strcmp(t2->attr.name, "+=") == 0) && !(strcmp(t2->attr.name, "-=") == 0) && !(strcmp(t2->attr.name, "*=") == 0) && !(strcmp(t2->attr.name, "/=") == 0) && !(strcmp(t2->attr.name, "++") == 0) && !(strcmp(t2->attr.name, "--") == 0) && !(strcmp(t2->attr.name, ">=") == 0) && !(strcmp(t2->attr.name, ">") == 0))
                        {
                            void *data = symbolTable.lookup(t2->attr.name);
                            if (data != NULL)
                            {
                                static_cast<TreeNode *>(data)->isInit = true;
                            }
                            t2->isInit = true;
                            printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", syntaxTree->lineno, t2->attr.name);
                            numWarnings++;
                        }
                        if (!(t1->isArray) && t2->isArray)
                        {
                            printf("ERROR(%d): Not expecting array in parameter %i of call to '%s' declared on line %d.\n", syntaxTree->lineno, i + 1, funcT->attr.name, funcT->lineno);
                            numErrors++;
                        }
                        if (t1->isArray && !t2->isArray)
                        {
                            printf("ERROR(%d): Expecting array in parameter %i of call to '%s' declared on line %d.\n", syntaxTree->lineno, i + 1, funcT->attr.name, funcT->lineno);
                            numErrors++;
                        }
                        wantContinue = true;
                        //CHECKER, DONT LIKE THIS, BUT WORKS
                        switch (t1->nodeType)
                        {
                        case nInt:
                            break;
                        case nBool:
                            break;
                        case nChar:
                            break;
                        case nVoid:
                            break;
                        case nUndefinedType:
                            break;
                        default:
                            wantContinue = false;
                            break;
                        }
                        switch (t2->nodeType)
                        {
                        case nInt:
                            break;
                        case nBool:
                            break;
                        case nChar:
                            break;
                        case nVoid:
                            break;
                        case nUndefinedType:
                            break;
                        default:
                            wantContinue = false;
                            break;
                        }

                        // BACK TO NORMAL
                        if ((t1->nodeType != t2->nodeType) && wantContinue)
                        {
                            numErrors++;
                            printf("ERROR(%d): Expecting type ", syntaxTree->lineno);
                            switch (t1->nodeType)
                            {
                            case nInt:
                                printf("int");
                                break;
                            case nBool:
                                printf("bool");
                                break;
                            case nChar:
                                printf("char");
                                break;
                            case nVoid:
                                printf("void");
                                break;
                            case nUndefinedType:
                                printf("undefined");
                                break;
                            default:
                                printf("bad news callk\n");
                                break;
                            }
                            printf(" in parameter %d of call to '%s' declared on line %d but got type ", i + 1, funcT->attr.name, funcT->lineno);
                            switch (t2->nodeType)
                            {
                            case nInt:
                                printf("int.\n");
                                break;
                            case nBool:
                                printf("bool.\n");
                                break;
                            case nChar:
                                printf("char.\n");
                                break;
                            case nVoid:
                                printf("void.\n");
                                break;
                            case nUndefinedType:
                                printf("undefined.\n");
                                break;
                            default:
                                printf("bad news callk2\n");
                                break;
                            }
                        }
                        t1 = t1->sibling;
                        t2 = t2->sibling;
                    }
                }
            }
            if (noFcallCheck)
            {
                callT->attr.name = syntaxTree->attr.name;
                callT->lineno = syntaxTree->lineno;
                callT->nodeType = syntaxTree->nodeType;
                if (syntaxTree->child[0] != NULL)
                {
                    callNparam = 1;
                    callT->child[0] = syntaxTree->child[0];
                }
                else
                {
                    callT->child[0] = NULL;
                }
                if (callT->child[0] != NULL)
                {
                    TreeNode *temp = callT->child[0];
                    while (temp->sibling != NULL)
                    {
                        callNparam++;
                        if (!temp->isConstant && !(strcmp(temp->attr.name, "[") == 0) && !(strcmp(temp->attr.name, "*") == 0) && !(strcmp(temp->attr.name, "%") == 0) && !(strcmp(temp->attr.name, "AND") == 0) && !(strcmp(temp->attr.name, "=") == 0) && !(strcmp(temp->attr.name, "+") == 0) && !(strcmp(temp->attr.name, "-") == 0) && !(strcmp(temp->attr.name, "SIZEOF") == 0) && !(strcmp(temp->attr.name, "<") == 0) && !(strcmp(temp->attr.name, "==") == 0) && !(strcmp(temp->attr.name, "<=") == 0) && !(strcmp(temp->attr.name, "!=") == 0) && !(strcmp(temp->attr.name, "OR") == 0) && !(strcmp(temp->attr.name, "CHSIGN") == 0) && !(strcmp(temp->attr.name, "NOT") == 0) && !(strcmp(temp->attr.name, "/") == 0) && !(strcmp(temp->attr.name, "=") == 0) && !(strcmp(temp->attr.name, "+=") == 0) && !(strcmp(temp->attr.name, "-=") == 0) && !(strcmp(temp->attr.name, "*=") == 0) && !(strcmp(temp->attr.name, "/=") == 0) && !(strcmp(temp->attr.name, "++") == 0) && !(strcmp(temp->attr.name, "--") == 0) && !(strcmp(temp->attr.name, ">=") == 0) && !(strcmp(temp->attr.name, ">") == 0))
                        {
                            void *data = symbolTable.lookup(temp->attr.name);
                            if (data != NULL)
                            {
                                temp->nodeType = static_cast<TreeNode *>(data)->nodeType;
                                temp->isArray = static_cast<TreeNode *>(data)->isArray;
                                temp->isInit = static_cast<TreeNode *>(data)->isInit;
                                temp->isDeclared = true;
                            }
                            else
                            {
                                //printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, temp->attr.name);
                                //numErrors++;
                                temp->isDeclared = false;
                            }
                        }
                        temp = temp->sibling;
                    }
                    if (!temp->isConstant && !(strcmp(temp->attr.name, "[") == 0) && !(strcmp(temp->attr.name, "*") == 0) && !(strcmp(temp->attr.name, "%") == 0) && !(strcmp(temp->attr.name, "AND") == 0) && !(strcmp(temp->attr.name, "=") == 0) && !(strcmp(temp->attr.name, "+") == 0) && !(strcmp(temp->attr.name, "-") == 0) && !(strcmp(temp->attr.name, "SIZEOF") == 0) && !(strcmp(temp->attr.name, "<") == 0) && !(strcmp(temp->attr.name, "==") == 0) && !(strcmp(temp->attr.name, "<=") == 0) && !(strcmp(temp->attr.name, "!=") == 0) && !(strcmp(temp->attr.name, "OR") == 0) && !(strcmp(temp->attr.name, "CHSIGN") == 0) && !(strcmp(temp->attr.name, "NOT") == 0) && !(strcmp(temp->attr.name, "/") == 0) && !(strcmp(temp->attr.name, "=") == 0) && !(strcmp(temp->attr.name, "+=") == 0) && !(strcmp(temp->attr.name, "-=") == 0) && !(strcmp(temp->attr.name, "*=") == 0) && !(strcmp(temp->attr.name, "/=") == 0) && !(strcmp(temp->attr.name, "++") == 0) && !(strcmp(temp->attr.name, "--") == 0) && !(strcmp(temp->attr.name, ">=") == 0) && !(strcmp(temp->attr.name, ">") == 0))
                    {
                        void *data = symbolTable.lookup(temp->attr.name);
                        if (data != NULL)
                        {
                            temp->nodeType = static_cast<TreeNode *>(data)->nodeType;
                            temp->isArray = static_cast<TreeNode *>(data)->isArray;
                            temp->isInit = static_cast<TreeNode *>(data)->isInit;
                            temp->isDeclared = true;
                        }
                        else
                        {
                            //printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, temp->attr.name);
                            //temp->isDeclared = false;
                            //numErrors++;
                        }
                    }
                }
            }
            //A3 stuff
            if (syntaxTree->child[0] != NULL)
            {
                if (syntaxTree->child[0]->isNC || syntaxTree->child[0]->isConstant)
                {
                }
                else
                {
                    if ((syntaxTree->child[0]->nodekind == ExpK) && (syntaxTree->child[0]->subkind.exp == CallK))
                    {
                    }
                    else if (!syntaxTree->isNC && !syntaxTree->isConstant)
                    {
                        //if its gonna be in symbol table

                        void *data;
                        data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                        if (data == NULL)
                        {
                            if (!((syntaxTree->child[0]->nodekind == ExpK) && (syntaxTree->child[0]->subkind.exp == OpK)) && (strcmp(syntaxTree->attr.name, "=") == 0) && (strcmp(syntaxTree->attr.name, "%") == 0) && (strcmp(syntaxTree->attr.name, "+=") == 0) && (strcmp(syntaxTree->attr.name, "-=") == 0) && (strcmp(syntaxTree->attr.name, "*=") == 0) && (strcmp(syntaxTree->attr.name, "/=") == 0) && (strcmp(syntaxTree->attr.name, "++") == 0) && (strcmp(syntaxTree->attr.name, "--") == 0))
                            {
                                //printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                                //numErrors++;
                            }
                        }
                        else
                        {
                            //printf("hello: %s\n", static_cast<TreeNode *>(data)->attr.name);
                            syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                            syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;

                            switch (static_cast<TreeNode *>(data)->subkind.decl)
                            {
                            case FuncK:
                                //printf("ERROR(%d): Cannot use function '%s' as a variable.\n", syntaxTree->lineno, static_cast<TreeNode *>(data)->attr.name);
                                //numErrors++;
                                break;
                            default:
                                break;
                            }
                        }
                    }
                }
            }
            /*
            data = NULL;
            data = symbolTable.lookup(syntaxTree->attr.name);
            if (data == NULL) //temporary bad fix, cant think of good solution at the moment
            {
                if (!(strcmp(syntaxTree->attr.name, "zog") == 0))
                {
                    symbolTable.insert(syntaxTree->attr.name, syntaxTree);
                }
            }
            else
            {
                switch (static_cast<TreeNode *>(data)->subkind.decl)
                {
                case FuncK:
                    break;
                case VarK:
                    printf("ERROR(%d): '%s' is a simple variable and cannot be called.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case ParamK:
                    printf("ERROR(%d): '%s' is a simple variable and cannot be called.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                default:
                    //printf("not good\n");
                    break;
                }
            }
            */

            break;
        }
        default:
            printf("ERROR: Unknown ExpK!! (SemanticAnalysis)\n");
        }
    }
    else
    {
        printf("UNKNOWN NODEKIND (syntaxTree->nodekind)(semanticanalysis)\n");
    }
    //fflush(stdout);
    //symbolTable.print(pointerPrintAddr); fflush(stdout);
    if (syntaxTree->sibling != NULL)
    {
        semanticAnalysis(syntaxTree->sibling, symbolTable);
    }
}

void actionUsed(std::string, void *t)
{
    if (static_cast<TreeNode *>(t)->isUsed == false && static_cast<TreeNode *>(t)->lineno != -1 && !(strcmp(static_cast<TreeNode *>(t)->attr.name, "main") == 0))
    {
        if (!(static_cast<TreeNode *>(t)->subkind.exp == CallK))
        {
            switch (static_cast<TreeNode *>(t)->subkind.decl)
            {
            case VarK:
                printf("WARNING(%d): The variable '%s' seems not to be used.\n", static_cast<TreeNode *>(t)->lineno, static_cast<TreeNode *>(t)->attr.name);
                numWarnings++;
                break;
            case FuncK:
                printf("WARNING(%d): The function '%s' seems not to be used.\n", static_cast<TreeNode *>(t)->lineno, static_cast<TreeNode *>(t)->attr.name);
                numWarnings++;
                break;
            case ParamK:
                printf("WARNING(%d): The parameter '%s' seems not to be used.\n", static_cast<TreeNode *>(t)->lineno, static_cast<TreeNode *>(t)->attr.name);
                numWarnings++;
                break;
            default:
                printf("SHOULDNT SEE THIS actionUsed - %s\n", static_cast<TreeNode *>(t)->attr.name);
                break;
            }
        }
    }
}

void is_Used(SymbolTable sym)
{
    sym.applyToAll(actionUsed);
}

//I Really Really, ... , Really dont like this, will fix in the near future
//
//
//
//
//
//
//
//
//
void semanticNoOut(TreeNode *syntaxTree, SymbolTable symbolTable)
{
    //fflush(stdout);
    /*
    if(syntaxTree->isNC)
    {
        printf("%d\n",syntaxTree->attr.value);fflush(stdout);
    }
    else if(syntaxTree->attr.name)
    {
        printf("%s\n",syntaxTree->attr.name);fflush(stdout);
    }
    */
    //symbolTable.print(pointerPrintAddr);
    /////////////////////////////////////////////////
    //I wrote about 3k lines previously, refactoring
    //to use what you showed in the other dayclass instead of book
    //it increases my work greatly to change it last minute
    //but i was having too many scoping difficulties with
    //previous implementation
    //
    //refactoring to do:
    //HECKENDORN IN CLASS:
    //....
    //case for:                 (same idea for everything else as well)
    //      symtab->enter();
    //      process stuff...
    //      traverse(current->child[0]);
    //      process stuff...
    //      traverse(current->child[1]);
    //      process stuff...
    //      traverse(current->child[2]);
    //      process stuff...
    //      symtam->apply...(checkused)
    //      symtab->leave()
    //      break;

    //DECLARATIONS

    if (syntaxTree->nodekind == DeclK)
    {
        switch (syntaxTree->subkind.decl)
        {
        case VarK:
        {
            if (syntaxTree->isStatic)
            {
                //printf("static initialization\n");
                syntaxTree->isInit = true;
            }
            //printf("hello: %s\n",(symbolTable.stack.back()->scopeName()).c_str());
            void *data = symbolTable.lookup(syntaxTree->attr.name);
            if (data == NULL)
            {
                //shold this be here?
                if (forflag)
                {
                    syntaxTree->isInit = true;
                }
                //if static, initialized
                if (syntaxTree->isStatic)
                {
                    syntaxTree->isInit = true;
                }
                bool b = symbolTable.insert(syntaxTree->attr.name, syntaxTree);
                if (b)
                {
                    //if global, initialized
                    void *data = symbolTable.lookupGlobal(syntaxTree->attr.name);
                    if (data != NULL)
                    {
                        //printf("global initialization\n");
                        syntaxTree->isInit = true;
                    }
                }
                else
                {
                    void *data = symbolTable.lookup(syntaxTree->attr.name);
                    printf("ERROR(%d): Symbol '%s' is already declared at line %d.\n", syntaxTree->lineno, syntaxTree->attr.name, static_cast<TreeNode *>(data)->lineno);
                    numErrors++;
                }
            }
            else
            {
                printf("ERROR(%d): Symbol '%s' is already declared at line %d.\n", syntaxTree->lineno, syntaxTree->attr.name, static_cast<TreeNode *>(data)->lineno);
                numErrors++;
            }
            if (syntaxTree->child[0] != NULL)
            {
                //Process Stuff
                semanticNoOut(syntaxTree->child[0], symbolTable);
            }
            if (syntaxTree->child[0] != NULL)
            {
                syntaxTree->isInit = true;
            }
            //symbolTable.print(pointerPrintAddr);
            break;
        }
        case FuncK:
        {
            syntaxTree->isInit = true;
            if (strcmp(syntaxTree->attr.name, "main") == 0)
            {
                isMain = 1;
            }
            // continue forward
            //bool b = symbolTable.insertGlobal(syntaxTree->attr.name, syntaxTree);
            void *data = symbolTable.lookup(syntaxTree->attr.name);
            if (data == NULL)
            {
                //functionScope = 0;
                syntaxTree->isInit = true;
                symbolTable.insertGlobal(syntaxTree->attr.name, syntaxTree);
            }
            else
            {
                //void *data = symbolTable.lookup(syntaxTree->attr.name);
                printf("ERROR(%d): Symbol '%s' is already declared at line %d.\n", syntaxTree->lineno, syntaxTree->attr.name, static_cast<TreeNode *>(data)->lineno);
                numErrors++;
            }
            symbolTable.enter(syntaxTree->attr.name);
            //
            //children
            if (syntaxTree->child[0] != NULL)
            {
                semanticNoOut(syntaxTree->child[0], symbolTable);
            }
            if (syntaxTree->child[1] != NULL)
            {
                if (syntaxTree->child[1]->nodekind == StmtK && syntaxTree->child[1]->subkind.stmt == CompoundK)
                {
                    syntaxTree->child[1]->firstCompound = true;
                }
                semanticNoOut(syntaxTree->child[1], symbolTable);
            }
            else
            {
                //printf("ERROR(YOU SHOULD NOT SEE THIS).\n");
            }

            //check for used before leaving
            is_Used(symbolTable);
            symbolTable.leave();
            break;
        }
        case ParamK:
        {
            syntaxTree->isInit = true;
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
            //parms are siblings not children
            break;
        }
        default:
            printf("ERROR: Unknown DeclK!! (SemanticAnalysis)\n");
            break;
        }
    }
    // STATEMENTS
    else if (syntaxTree->nodekind == StmtK)
    {
        switch (syntaxTree->subkind.stmt)
        {
        case NullK:
        {
            printf("ERROR(PRETTY SURE THIS CAN BE DELETED, WAS IN CLASS SUPPLIED FILE THOUGH)\n");
            break;
        }
        case IfK:
        {
            //symbolTable.print(pointerPrintAddr);
            //symbolTable.enter("If");
            //process stuff
            if (syntaxTree->child[0] != NULL)
            {
                semanticNoOut(syntaxTree->child[0], symbolTable);
            }
            else
            {
                //printf("ERROR(YOU SHOULD NOT SEE THIS) IfKc[0].\n");
            }
            //symbolTable.print(pointerPrintAddr);
            //process stuff
            if (syntaxTree->child[1] != NULL)
            {
                semanticNoOut(syntaxTree->child[1], symbolTable);
            }
            else
            {
                //c1 is allowed to be null
            }
            //symbolTable.print(pointerPrintAddr);
            //process stuff
            if (syntaxTree->child[2] != NULL)
            {
                semanticNoOut(syntaxTree->child[2], symbolTable);
            }
            else
            {
                //c2 is allowed to be NULL
            }
            //symbolTable.print(pointerPrintAddr);
            //symbolTable.leave();
            //symbolTable.print(pointerPrintAddr);
            break;
        }
        case WhileK:
        {
            //process stuff
            if (syntaxTree->child[0] != NULL)
            {
                semanticNoOut(syntaxTree->child[0], symbolTable);
            }
            else
            {
                //printf("ERROR(YOU SHOULD NOT SEE THIS) WhileKc[0].\n");
            }

            //process stuff
            if (syntaxTree->child[1] != NULL)
            {
                semanticNoOut(syntaxTree->child[1], symbolTable);
            }
            else
            {
                //c1 is allowed to be NULL
            }
            break;
        }
        case ForK:
        {
            //Process stuff
            symbolTable.enter("For");
            forflag = true;
            //void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
            //if(data != NULL){
            //static_cast<TreeNode *>(data)->isInit = true;
            //}
            syntaxTree->child[0]->isInit = true;
            if (syntaxTree->child[0] != NULL)
            {
                semanticNoOut(syntaxTree->child[0], symbolTable);
            }
            else
            {
                //printf("ERROR(YOU SHOULD NOT SEE THIS) ForKc0.\n");
            }
            forflag = false;

            /* No ForK child[1] for assignment 3
            if (syntaxTree->child[1] != NULL)
            {
                semanticAnalysis(syntaxTree->child[1], symbolTable);
            }
            else
            {
            }
            */
            //process stuff

            if (syntaxTree->child[2] != NULL)
            {
                semanticNoOut(syntaxTree->child[2], symbolTable);
            }

            //check for used before leaving
            is_Used(symbolTable);
            symbolTable.leave();
            break;
        }
        case CompoundK:
        {
            //enter scope
            if (!syntaxTree->firstCompound)
            {
                symbolTable.enter("Compound Statement");
            }
            inCompound = true;
            if (syntaxTree->child[0] != NULL)
            {
                semanticNoOut(syntaxTree->child[0], symbolTable);
            }
            else
            {
                //c0 is allowed NULL
            }
            //
            //children
            if (syntaxTree->child[1] != NULL)
            {
                semanticNoOut(syntaxTree->child[1], symbolTable);
            }
            else
            {
                //c1 is allowed NULL
            }
            //leave scope
            inCompound = false;
            if (!syntaxTree->firstCompound)
            {
                //check for used before leaving
                is_Used(symbolTable);
                symbolTable.leave();
            }
            break;
        }
        case ReturnK:
        {
            //process stuff
            if (syntaxTree->child[0] != NULL)
            {
                semanticNoOut(syntaxTree->child[0], symbolTable);
            }
            if (syntaxTree->child[0] != NULL && (syntaxTree->child[0]->nodeType == uninit || syntaxTree->child[0]->nodeType == nVoid))
            {
                void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                if (data == NULL)
                { //should this error be here?
                    printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                    numErrors++;
                }
                else
                {
                    static_cast<TreeNode *>(data)->isUsed = true; //MAYBE
                    syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                    syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                }
            }
            if (syntaxTree->child[0] != NULL)
            {
                if (syntaxTree->child[0]->isArray)
                {
                    printf("ERROR(%d): Cannot return an array.\n", syntaxTree->lineno);
                    numErrors++;
                }
            }
            if (!inCompound)
            { //printf("return leave\n"); fflush(stdout);
                //symbolTable.leave();
            }
            break;
        }
        case BreakK:
        { // no children
            break;
        }
        case RangeK:
        {
            // pretty sure we don't use for this assignment (A3)
            /*
            //process stuff
            if (syntaxTree->child[0] != NULL)
            {
                semanticAnalysis(syntaxTree->child[0], symbolTable);
            }
            else
            {
                printf("ERROR(YOU SHOULD NOT SEE THIS) RangeKc0.\n");
            }
            //process stuff
            if (syntaxTree->child[1] != NULL)
            {
                semanticAnalysis(syntaxTree->child[1], symbolTable);
            }
            else
            {
                printf("ERROR(YOU SHOULD NOT SEE THIS) RangeKc1.\n");
            }
            //process stuff
            if (syntaxTree->child[2] != NULL)
            {
                semanticAnalysis(syntaxTree->child[2], symbolTable);
            }
            else
            {
                //c2 is allowed NULL
            }
            */
            break;
        }
        default:
            printf("ERROR: Unknown StmtK!! (SemanticAnalysis)\n");
            break;
        }
    }
    // EXPRESSIONS
    else if (syntaxTree->nodekind == ExpK)
    {
        switch (syntaxTree->subkind.exp)
        {
        case OpK:
        {
            //Unaries "ERROR(%d): Unary '%s' requires an operand of %s but was given %s.\n"
            if (strcmp(syntaxTree->attr.name, "NOT") == 0)
            {
                syntaxTree->nodeType = nBool;

                //traverse c0 to get its type
                if (syntaxTree->child[0] != NULL)
                {
                    semanticNoOut(syntaxTree->child[0], symbolTable);
                }
                else
                {
                    //printf("ERROR(YOU SHOULD NOT SEE THIS) OpKc0.\n");
                }
                if (syntaxTree->child[0]->nodeType == uninit || syntaxTree->child[0]->nodeType == nVoid)
                {
                    void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                    }
                }
                if (syntaxTree->child[0]->isArray)
                {
                    printf("ERROR(%d): The operation 'not' does not work with arrays.\n", syntaxTree->lineno);
                    numErrors++;
                }
                else
                {
                    switch (syntaxTree->child[0]->nodeType)
                    {
                    case nBool:
                        syntaxTree->nodeType = nBool;
                        break;
                    case nVoid:
                        printf("ERROR(%d): Unary 'not' requires an operand of type bool but was given type void.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nInt:
                        printf("ERROR(%d): Unary 'not' requires an operand of type bool but was given type int.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nChar:
                        printf("ERROR(%d): Unary 'not' requires an operand of type bool but was given type char.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nUndefinedType:
                        printf("ERROR(%d): Unary 'not' requires an operand of type bool but was given type undefined.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case uninit:
                        break;
                    default:
                        printf("ERROR: semantic.cpp line 836\n");
                        break;
                    }
                }
            }
            else if (strcmp(syntaxTree->attr.name, "SIZEOF") == 0)
            {
                //syntaxTree->nodeType = nInt;

                //traverse c0 to get its type
                if (syntaxTree->child[0] != NULL)
                {
                    semanticNoOut(syntaxTree->child[0], symbolTable);
                }
                else
                {
                    //printf("ERROR(YOU SHOULD NOT SEE THIS) OpKc0.\n");
                }
                if (syntaxTree->child[0]->nodeType == uninit || syntaxTree->child[0]->nodeType == nVoid)
                {
                    void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                        syntaxTree->child[0]->isStatic = static_cast<TreeNode *>(data)->isStatic;
                        syntaxTree->child[0]->isInit = static_cast<TreeNode *>(data)->isInit;
                    }
                }
                //no c1
                if (!syntaxTree->child[0]->isArray)
                {
                    printf("ERROR(%d): The operation 'sizeof' only works with arrays.\n", syntaxTree->lineno);
                    numErrors++;
                }
                else
                {
                    syntaxTree->nodeType = nInt;
                }
                if (!syntaxTree->child[0]->isInit)
                {
                    void *data = symbolTable.lookupGlobal(syntaxTree->attr.name);
                    if (data == NULL)
                    {
                        //printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        //numWarnings++;
                        //syntaxTree->child[0]->isInit = true;
                    }
                    else
                    {
                    }
                }
            }
            else if (strcmp(syntaxTree->attr.name, "CHSIGN") == 0)
            {
                //syntaxTree->nodeType = nInt;

                //traverse c0 to get its type
                if (syntaxTree->child[0] != NULL)
                {
                    semanticNoOut(syntaxTree->child[0], symbolTable);
                }
                else
                {
                    //printf("ERROR(YOU SHOULD NOT SEE THIS) OpKc0.\n");
                }
                if (syntaxTree->child[0]->nodeType == uninit || syntaxTree->child[0]->nodeType == nVoid)
                {
                    void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                    }
                }
                //no c1
                if (syntaxTree->child[0]->isArray)
                {
                    printf("ERROR(%d): The operation 'chsign' does not work with arrays.\n", syntaxTree->lineno);
                    numErrors++;
                }
                else
                {
                    switch (syntaxTree->child[0]->nodeType)
                    {
                    case nInt:
                        syntaxTree->nodeType = nInt;
                        break;
                    case nVoid:
                        printf("ERROR(%d): Unary 'chsign' requires an operand of type int but was given type void.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nBool:
                        printf("ERROR(%d): Unary 'chsign' requires an operand of type int but was given type bool.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nChar:
                        printf("ERROR(%d): Unary 'chsign' requires an operand of type int but was given type char.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nUndefinedType:
                        printf("ERROR(%d): Unary 'chsign' requires an operand of type int but was given type undefined.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case uninit:
                        break;
                    default:
                        printf("ERROR: semantic.cpp line 836\n");
                        break;
                    }
                }
            }
            else if (strcmp(syntaxTree->attr.name, "?") == 0)
            {
                //syntaxTree->nodeType = nInt;

                //traverse c0 to get its type
                if (syntaxTree->child[0] != NULL)
                {
                    semanticNoOut(syntaxTree->child[0], symbolTable);
                }
                else
                {
                    //printf("ERROR(YOU SHOULD NOT SEE THIS) OpKc0.\n");
                }
                if (syntaxTree->child[0]->nodeType == uninit || syntaxTree->child[0]->nodeType == nVoid)
                {
                    void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                    }
                }
                //no c1
                if (syntaxTree->child[0]->isArray)
                {
                    printf("ERROR(%d): The operation '?' does not work with arrays.\n", syntaxTree->lineno);
                    numErrors++;
                }

                switch (syntaxTree->child[0]->nodeType)
                {
                case nInt:
                    syntaxTree->nodeType = nInt;
                    break;
                case nVoid:
                    printf("ERROR(%d): Unary '?' requires an operand of type int but was given type void.\n", syntaxTree->lineno);
                    numErrors++;
                    break;
                case nBool:
                    printf("ERROR(%d): Unary '?' requires an operand of type int but was given type bool.\n", syntaxTree->lineno);
                    numErrors++;
                    break;
                case nChar:
                    printf("ERROR(%d): Unary '?' requires an operand of type int but was given type char.\n", syntaxTree->lineno);
                    numErrors++;
                    break;
                case nUndefinedType:
                    printf("ERROR(%d): Unary '?' requires an operand of type int but was given type undefined.\n", syntaxTree->lineno);
                    numErrors++;
                    break;
                case uninit:
                    break;
                default:
                    printf("ERROR: semantic.cpp line 836\n");
                    break;
                }

            } //and or separate so i can lowercase
            else if (strcmp(syntaxTree->attr.name, "AND") == 0)
            {
                syntaxTree->nodeType = nBool;
                //traverse C0 to get its type
                if (syntaxTree->child[0] != NULL)
                {
                    semanticNoOut(syntaxTree->child[0], symbolTable);
                }
                else
                {
                    //printf("ERROR(YOU SHOULD NOT SEE THIS) ExpKc0.\n");
                }
                if (syntaxTree->child[0]->nodeType == uninit || syntaxTree->child[0]->nodeType == nVoid) //if its gonna be in symbol table
                {
                    void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                    }
                }
                //traverse c1 to get its type
                if (syntaxTree->child[1] != NULL)
                {
                    semanticNoOut(syntaxTree->child[1], symbolTable);
                }
                else
                {
                    //printf("ERROR(YOU SHOULD NOT SEE THIS) ExpKc1.\n");
                }
                if (syntaxTree->child[1]->nodeType == uninit || syntaxTree->child[1]->nodeType == nVoid) //if its gonna be in symbol table
                {
                    void *data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[1]->attr.name);
                        numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[1]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[1]->isArray = static_cast<TreeNode *>(data)->isArray;
                    }
                }
                if (syntaxTree->child[0]->isArray || syntaxTree->child[1]->isArray)
                {
                    printf("ERROR(%d): The operation 'and' does not work with arrays.\n", syntaxTree->lineno);
                    numErrors++;
                }
                else
                {
                    switch (syntaxTree->child[0]->nodeType)
                    {
                    case nInt:
                        printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type int.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nVoid:
                        printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type void.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nBool:
                        break;
                    case nChar:
                        printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type char.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nUndefinedType:
                        printf("ERROR(%d): 'and' requires operands of type bool but lhs is of type undefined.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case uninit:
                        break;
                    default:
                        printf("ERROR: semantic.cpp line 836\n");
                        break;
                    }
                    switch (syntaxTree->child[1]->nodeType)
                    {
                    case nInt:
                        printf("ERROR(%d): 'and' requires operands of type bool but rhs is of type int.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nVoid:
                        printf("ERROR(%d): 'and' requires operands of type bool but rhs is of type void.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nBool:
                        break;
                    case nChar:
                        printf("ERROR(%d): 'and' requires operands of type bool but rhs is of type char.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nUndefinedType:
                        printf("ERROR(%d): 'and' requires operands of type bool but rhs is of type undefined.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case uninit:
                        break;
                    default:
                        printf("ERROR: semantic.cpp line 836\n");
                        break;
                    }
                }
            }
            else if (strcmp(syntaxTree->attr.name, "OR") == 0)
            {
                syntaxTree->nodeType = nBool;
                //traverse C0 to get its type
                if (syntaxTree->child[0] != NULL)
                {
                    semanticNoOut(syntaxTree->child[0], symbolTable);
                }
                else
                {
                    //printf("ERROR(YOU SHOULD NOT SEE THIS) ExpKc0.\n");
                }
                if (syntaxTree->child[0]->nodeType == uninit || syntaxTree->child[0]->nodeType == nVoid) //if its gonna be in symbol table
                {
                    void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                    }
                }
                //traverse c1 to get its type
                if (syntaxTree->child[1] != NULL)
                {
                    semanticNoOut(syntaxTree->child[1], symbolTable);
                }
                else
                {
                    //printf("ERROR(YOU SHOULD NOT SEE THIS) ExpKc1.\n");
                }
                if (syntaxTree->child[1]->nodeType == uninit || syntaxTree->child[1]->nodeType == nVoid) //if its gonna be in symbol table
                {
                    void *data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[1]->attr.name);
                        numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[1]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[1]->isArray = static_cast<TreeNode *>(data)->isArray;
                    }
                }
                if (syntaxTree->child[0]->isArray || syntaxTree->child[1]->isArray)
                {
                    printf("ERROR(%d): The operation 'or' does not work with arrays.\n", syntaxTree->lineno);
                    numErrors++;
                }
                else
                {
                    switch (syntaxTree->child[0]->nodeType)
                    {
                    case nInt:
                        printf("ERROR(%d): 'or' requires operands of type bool but lhs is of type int.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nVoid:
                        printf("ERROR(%d): 'or' requires operands of type bool but lhs is of type void.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nBool:
                        break;
                    case nChar:
                        printf("ERROR(%d): 'or' requires operands of type bool but lhs is of type char.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nUndefinedType:
                        printf("ERROR(%d): 'or' requires operands of type bool but lhs is of type undefined.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case uninit:
                        break;
                    default:
                        printf("ERROR: semantic.cpp line 836\n");
                        break;
                    }
                    switch (syntaxTree->child[1]->nodeType)
                    {
                    case nInt:
                        printf("ERROR(%d): 'or' requires operands of type bool but rhs is of type int.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nVoid:
                        printf("ERROR(%d): 'or' requires operands of type bool but rhs is of type void.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nBool:
                        break;
                    case nChar:
                        printf("ERROR(%d): 'or' requires operands of type bool but rhs is of type char.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case nUndefinedType:
                        printf("ERROR(%d): 'or' requires operands of type bool but rhs is of type undefined.\n", syntaxTree->lineno);
                        numErrors++;
                        break;
                    case uninit:
                        break;
                    default:
                        printf("ERROR: semantic.cpp line 836\n");
                        break;
                    }
                }
            }
            //same type + Array -> bools: ==, !=, <=, <, >, >=
            else if ((strcmp(syntaxTree->attr.name, "==") == 0) || (strcmp(syntaxTree->attr.name, "!=") == 0) ||
                     (strcmp(syntaxTree->attr.name, "<=") == 0) || (strcmp(syntaxTree->attr.name, ">=") == 0) ||
                     (strcmp(syntaxTree->attr.name, "<") == 0) || (strcmp(syntaxTree->attr.name, ">") == 0))
            {
                syntaxTree->nodeType = nBool;
                //traverse C0 to get its type
                if (syntaxTree->child[0] != NULL)
                {
                    semanticNoOut(syntaxTree->child[0], symbolTable);
                }
                else
                {
                    //printf("ERROR(YOU SHOULD NOT SEE THIS) OpKc0.\n");
                }
                if (syntaxTree->child[0]->nodeType == uninit || syntaxTree->child[0]->nodeType == nVoid) //if its gonna be in symbol table
                {
                    void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[0]->isInit = true;
                        syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                    }
                }
                //traverse c1 to get its type
                if (syntaxTree->child[1] != NULL)
                {
                    semanticNoOut(syntaxTree->child[1], symbolTable);
                }
                else
                {
                    //printf("ERROR(YOU SHOULD NOT SEE THIS) OpKc1.\n");
                }
                if (syntaxTree->child[1]->nodeType == uninit) //if its gonna be in symbol table
                {
                    void *data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[1]->attr.name);
                        numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[1]->isInit = true;
                        syntaxTree->child[1]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[1]->isArray = static_cast<TreeNode *>(data)->isArray;
                    }
                }
                //syntaxTree->nodeType = nBool;
                bool twoArrays = false;
                //both arrays
                if (syntaxTree->child[0]->isArray && syntaxTree->child[1]->isArray)
                {
                    twoArrays = true;
                    syntaxTree->isArray;
                }
                //rhs not array
                else if (syntaxTree->child[0]->isArray && !syntaxTree->child[1]->isArray)
                {
                    printf("ERROR(%d): '%s' requires both operands be arrays or not but lhs is an array and rhs is not an array.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                }
                //lhs not array
                else if ((!syntaxTree->child[0]->isArray && syntaxTree->child[1]->isArray))
                {
                    printf("ERROR(%d): '%s' requires both operands be arrays or not but lhs is not an array and rhs is an array.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                }
                //both not arrays, or both arrays
                /*
                if ((!syntaxTree->child[0]->isArray && !syntaxTree->child[1]->isArray) || twoArrays)
                {*/
                //should have both types, now compare, output error if not equal types.
                if (syntaxTree->child[0]->nodeType == syntaxTree->child[1]->nodeType)
                {
                    syntaxTree->nodeType = nBool;
                }
                else
                {
                    printf("ERROR(%d): '%s' requires operands of the same type but lhs is type ", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    switch (syntaxTree->child[0]->nodeType)
                    {
                    case nInt:
                        printf("int");
                        break;
                    case nVoid:
                        printf("void");
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
                    case uninit:
                        printf("temp error8\n");
                        break;
                    default:
                        printf("ERROR: semantic.cpp line 836\n");
                        break;
                    }
                    printf(" and rhs is type ");
                    switch (syntaxTree->child[1]->nodeType)
                    {
                    case nInt:
                        printf("int");
                        break;
                    case nVoid:
                        printf("void");
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
                        printf("ERROR: semantic.cpp line 836\n");
                        break;
                    }
                    printf(".\n");
                }
                //}
            }
            //int,int -> ints: :>:, :<:, *, +, /, -, %
            else if ((strcmp(syntaxTree->attr.name, ":>:") == 0) || (strcmp(syntaxTree->attr.name, ":<:") == 0) ||
                     (strcmp(syntaxTree->attr.name, "*") == 0) || (strcmp(syntaxTree->attr.name, "+") == 0) ||
                     (strcmp(syntaxTree->attr.name, "/") == 0) || (strcmp(syntaxTree->attr.name, "-") == 0) ||
                     (strcmp(syntaxTree->attr.name, "%") == 0))
            {
                syntaxTree->nodeType = nInt;
                //traverse C0
                //printf("hi\n");fflush(stdout);
                if (syntaxTree->child[0] != NULL)
                {
                    semanticNoOut(syntaxTree->child[0], symbolTable);
                }
                else
                {
                    //printf("ERROR(YOU SHOULD NOT SEE THIS) OpKc0.\n");
                }

                //do stuff with c0
                if (syntaxTree->child[0]->isNC)
                {
                }
                else if (!(strcmp(syntaxTree->child[0]->attr.name, "[") == 0) && (syntaxTree->child[0]->nodeType == uninit || syntaxTree->child[0]->nodeType == nVoid)) //if its gonna be in symbol table
                {
                    void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                    }
                }

                //traverse c1
                if (syntaxTree->child[1] != NULL)
                {
                    semanticNoOut(syntaxTree->child[1], symbolTable);
                }
                else
                {
                    //printf("ERROR(YOU SHOULD NOT SEE THIS) OpKc1.\n");
                }

                //do stuff with c1
                if (syntaxTree->child[1]->isNC)
                {
                }
                else if (!(strcmp(syntaxTree->child[1]->attr.name, "[") == 0) && (syntaxTree->child[1]->nodeType == uninit || syntaxTree->child[1]->nodeType == nVoid)) //if its gonna be in symbol table
                {
                    void *data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[1]->attr.name);
                        numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[1]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[1]->isArray = static_cast<TreeNode *>(data)->isArray;
                    }
                }
                if (syntaxTree->child[0]->isArray || syntaxTree->child[1]->isArray)
                {
                    printf("ERROR(%d): The operation '%s' does not work with arrays.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                }
                //was an else ehere
                //should have both types, now compare, output error if not equal types.

                switch (syntaxTree->child[0]->nodeType)
                {
                case nInt:
                    break;
                case nVoid:
                    printf("ERROR(%d): '%s' requires operands of type int but lhs is of type void.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case nBool:
                    printf("ERROR(%d): '%s' requires operands of type int but lhs is of type bool.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case nChar:
                    printf("ERROR(%d): '%s' requires operands of type int but lhs is of type char.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case nUndefinedType:
                    printf("ERROR(%d): '%s' requires operands of type int but lhs is of type undefined.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case uninit:
                    printf("temp error9\n");
                    break;
                default:
                    printf("ERROR: semantic.cpp line 836\n");
                    break;
                }
                switch (syntaxTree->child[1]->nodeType)
                {
                case nInt:
                    break;
                case nVoid:
                    printf("ERROR(%d): '%s' requires operands of type int but rhs is of type void.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case nBool:
                    printf("ERROR(%d): '%s' requires operands of type int but rhs is of type bool.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case nChar:
                    printf("ERROR(%d): '%s' requires operands of type int but rhs is of type char.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case nUndefinedType:
                    printf("ERROR(%d): '%s' requires operands of type int but rhs is of type undefined.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case uninit:
                    printf("temp error10\n");
                    break;
                default:
                    printf("ERROR: semantic.cpp line 836\n");
                    break;
                }
            }
            else // '[' , array,int ->type of array
            {
                //syntaxTree->nodeType = nInt;
                //traverse C0

                bool iserr = false;
                if (syntaxTree->child[0] != NULL)
                {
                    semanticNoOut(syntaxTree->child[0], symbolTable);
                }
                else
                {
                    //printf("ERROR(YOU SHOULD NOT SEE THIS) OpKc0.\n");
                }
                bool temp = false;
                void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                if (data != NULL)
                {
                    temp = static_cast<TreeNode *>(data)->isInit;
                }
                if (syntaxTree->child[0]->isNC)
                {
                }
                else if (!disableW && !temp && !(strcmp(syntaxTree->child[0]->attr.name, "[") == 0)) // if c1 isnt inited, and isnt '['
                {
                    printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                    numWarnings++;
                    syntaxTree->child[0]->isInit = true;
                }
                // Mess with c0
                syntaxTree->child[0]->isInit = true;                                                     //MAYBE SHOULDNT BE HERE
                if (syntaxTree->child[0]->nodeType == uninit || syntaxTree->child[0]->nodeType == nVoid) //if its gonna be in symbol table
                {
                    void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        static_cast<TreeNode *>(data)->isInit = true;
                        syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                    }
                    switch (static_cast<TreeNode *>(data)->subkind.decl)
                    {
                    case FuncK:
                        printf("ERROR(%d): Cannot use function '%s' as a variable.\n", syntaxTree->lineno, static_cast<TreeNode *>(data)->attr.name);
                        numErrors++;
                        break;
                    default:
                        break;
                    }
                }
                //syntaxTree->nodeType = syntaxTree->child[0]->nodeType;
                //traverse c1
                if (syntaxTree->child[1] != NULL)
                {
                    semanticNoOut(syntaxTree->child[1], symbolTable);
                }
                else
                {
                    //printf("ERROR(YOU SHOULD NOT SEE THIS) OpKc1.\n");
                }
                if (syntaxTree->child[1]->isNC)
                {
                    syntaxTree->isInit = true;
                }
                else
                {
                    temp = false;
                    data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                    if (data != NULL)
                    {
                        temp = static_cast<TreeNode *>(data)->isInit;
                    }
                }

                //mess with c1
                if (syntaxTree->child[1]->isNC)
                {
                }
                else if (!temp && !(strcmp(syntaxTree->child[1]->attr.name, "[") == 0)) // if c1 isnt inited, and isnt '['
                {
                    printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", syntaxTree->lineno, syntaxTree->child[1]->attr.name);
                    numWarnings++;
                    syntaxTree->child[1]->isInit = true;
                }

                if (syntaxTree->child[1]->isNC)
                {
                }
                else if (!(strcmp(syntaxTree->child[1]->attr.name, "[") == 0) && (syntaxTree->child[1]->nodeType == uninit || syntaxTree->child[1]->nodeType == nVoid)) //if its gonna be in symbol table
                {
                    void *data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[1]->attr.name);
                        numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        static_cast<TreeNode *>(data)->isInit = true;
                        syntaxTree->child[1]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[1]->isArray = static_cast<TreeNode *>(data)->isArray;
                    }
                    if (static_cast<TreeNode *>(data)->isArray)
                    {
                        printf("ERROR(%d): Array index is the unindexed array '%s'.\n", syntaxTree->lineno, static_cast<TreeNode *>(data)->attr.name);
                        numErrors++;
                    }
                }

                // now work with both
                if (!syntaxTree->child[0]->isArray)
                {
                    printf("ERROR(%d): Cannot index nonarray '%s'.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                    numErrors++;
                }
                //syntaxTree->nodeType = syntaxTree->child[0]->nodeType;
                switch (syntaxTree->child[1]->nodeType)
                {
                case nInt:
                    break;
                case nVoid:
                    printf("ERROR(%d): Array '%s' should be indexed by type int but got type void.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                    numErrors++;
                    break;
                case nBool:
                    printf("ERROR(%d): Array '%s' should be indexed by type int but got type bool.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                    numErrors++;
                    break;
                case nChar:
                    printf("ERROR(%d): Array '%s' should be indexed by type int but got type char.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                    numErrors++;
                    break;
                case nUndefinedType:
                    printf("ERROR(%d): Array '%s' should be indexed by type int but got type undefined.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                    numErrors++;
                    break;
                case uninit:
                    printf("temp error11\n");
                    break;
                default:
                    printf("ERROR: semantic.cpp line 836\n");
                    break;
                }
            }

            break;
        }
        case ConstantK:
        {
            //no children
            break;
        }
        case IdK:
        {

            void *data = symbolTable.lookup(syntaxTree->attr.name);
            if (data != NULL)
            {
                static_cast<TreeNode *>(data)->isUsed = true;
                /*
                if(!static_cast<TreeNode *>(data)->isInit)
                {
                    printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", syntaxTree->lineno, static_cast<TreeNode *>(data)->attr.name);
                    numWarnings++;
                    static_cast<TreeNode *>(data)->isInit = true;
                }
                */
            }
            //No Children
            break;
        }
        case AssignK:
        {

            bool iserr = false;                          //"ERROR(%d): '%s' requires operands of the same type but lhs is %s and rhs is %s.\n"
            if (strcmp(syntaxTree->attr.name, "=") == 0) // =
            {
                //syntaxTree->nodeType = nBool;
                //traverse C0 to get its type
                if (strcmp(syntaxTree->child[0]->attr.name, "[") == 0)
                {
                    disableW = true;
                }
                if (syntaxTree->child[0] != NULL)
                {
                    semanticNoOut(syntaxTree->child[0], symbolTable);
                }
                else
                {
                    //printf("ERROR(YOU SHOULD NOT SEE THIS) AssignKc0.\n");
                }
                disableW = false;
                syntaxTree->child[0]->isInit = true;
                if (syntaxTree->child[0]->nodeType == uninit || syntaxTree->child[0]->nodeType == nVoid) //if its gonna be in symbol table
                {
                    void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        iserr = true;
                        numErrors++;
                    }
                    else
                    {
                        syntaxTree->child[0]->isInit = true;
                        syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                        static_cast<TreeNode *>(data)->isUsed = true;
                    }
                }
                //syntaxTree->child[0]->isInit = true;

                //traverse c1 to get its type
                if (syntaxTree->child[1] != NULL)
                {
                    semanticNoOut(syntaxTree->child[1], symbolTable);
                }
                else
                {
                    //printf("ERROR(YOU SHOULD NOT SEE THIS) AssignKc1.\n");
                }
                if (syntaxTree->child[1]->nodeType == uninit) // || syntaxTree->child[1]->nodeType == nVoid) //if its gonna be in symbol table
                {
                    void *data;
                    if (!(strcmp(syntaxTree->child[1]->attr.name, "[") == 0))
                    {
                        data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                    }
                    else
                    {
                        data = symbolTable.lookup(syntaxTree->child[1]->child[0]->attr.name);
                    }
                    if (data == NULL)
                    { //should this error be here?
                        iserr = true;
                        if (!(strcmp(syntaxTree->child[1]->attr.name, "[") == 0))
                        {
                            printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[1]->attr.name);
                            numErrors++;
                        }
                    }
                    else
                    {
                        //printf("%s\n", static_cast<TreeNode *>(data)->attr.name);
                        if ((strcmp(syntaxTree->child[1]->attr.name, "[") == 0))
                        {
                            if (!syntaxTree->child[1]->isArray)
                            {
                                static_cast<TreeNode *>(data)->isUsed = true;
                                syntaxTree->child[1]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                                syntaxTree->child[1]->isArray = static_cast<TreeNode *>(data)->isArray;
                            }
                            else
                            {
                                static_cast<TreeNode *>(data)->isUsed = true;
                                syntaxTree->child[1]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                                syntaxTree->child[1]->isArray = static_cast<TreeNode *>(data)->isArray;
                            }
                        }
                        else
                        {
                            static_cast<TreeNode *>(data)->isUsed = true;
                            syntaxTree->child[1]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                            syntaxTree->child[1]->isArray = static_cast<TreeNode *>(data)->isArray;
                        }
                    }
                    if (!syntaxTree->child[1]->isInit && !(strcmp(syntaxTree->child[1]->attr.name, "[") == 0) && !iserr) // if c1 isnt inited, and isnt '['
                    {
                        printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", syntaxTree->lineno, syntaxTree->child[1]->attr.name);
                        numWarnings++;
                        syntaxTree->child[1]->isInit = true;
                    }
                    /* what is this for?
                    else if (!syntaxTree->child[0]->isArray && (strcmp(syntaxTree->child[1]->attr.name, "[") == 0))
                    {
                        printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        numWarnings++;
                        syntaxTree->child[0]->isInit = true;
                    }
                    */
                }
                /*
                else
                {
                    syntaxTree->child[0]->isInit = true;
                }
                */

                //syntaxTree->nodeType = nBool;
                bool twoArrays = false;
                //both arrays
                if (iserr)
                {
                }
                else
                {
                    if (syntaxTree->child[0]->isArray && syntaxTree->child[1]->isArray)
                    {
                        twoArrays = true;
                        syntaxTree->isArray;
                    }
                    //rhs not array
                    else if (syntaxTree->child[0]->isArray && !syntaxTree->child[1]->isArray)
                    {
                        printf("ERROR(%d): '=' requires both operands be arrays or not but lhs is an array and rhs is not an array.\n", syntaxTree->lineno);
                        numErrors++;
                    }
                    //lhs not array
                    else if ((!syntaxTree->child[0]->isArray && syntaxTree->child[1]->isArray))
                    {
                        printf("ERROR(%d): '=' requires both operands be arrays or not but lhs is not an array and rhs is an array.\n", syntaxTree->lineno);
                        numErrors++;
                    }
                    //both not arrays, or both arrays
                    if ((!syntaxTree->child[0]->isArray && !syntaxTree->child[1]->isArray) || twoArrays)
                    {
                        //should have both types, now compare, output error if not equal types.
                        if (syntaxTree->child[0]->nodeType == syntaxTree->child[1]->nodeType)
                        {
                            syntaxTree->nodeType = syntaxTree->child[0]->nodeType;
                        }
                        else
                        {
                            printf("ERROR(%d): '=' requires operands of the same type but lhs is type ", syntaxTree->lineno);
                            numErrors++;
                            switch (syntaxTree->child[0]->nodeType)
                            {
                            case nInt:
                                printf("int");
                                break;
                            case nVoid:
                                printf("void");
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
                            case uninit:
                                printf("temp error12\n");
                                break;
                            default:
                                printf("ERROR: semantic.cpp line 836\n");
                                break;
                            }
                            printf(" and rhs is type ");
                            switch (syntaxTree->child[1]->nodeType)
                            {
                            case nInt:
                                printf("int");
                                break;
                            case nVoid:
                                printf("void");
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
                                printf("ERROR: semantic.cpp line 836\n");
                                break;
                            }
                            printf(".\n");
                        }
                    }
                }
            }
            else if (strcmp(syntaxTree->attr.name, "++") == 0 || strcmp(syntaxTree->attr.name, "--") == 0) //separate cause no c1
            {
                //syntaxTree->nodeType = nInt; //maybe delete this

                if (syntaxTree->child[0] != NULL)
                {
                    semanticNoOut(syntaxTree->child[0], symbolTable);
                }
                else
                {
                    //printf("ERROR(YOU SHOULD NOT SEE THIS) AssignKc0.\n");
                }
                //syntaxTree->child[0]->isInit = true;

                if (syntaxTree->child[0]->nodeType == uninit || syntaxTree->child[0]->nodeType == nVoid) //if its gonna be in symbol table
                {
                    void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[0]->isInit = static_cast<TreeNode *>(data)->isInit; // = true;
                        syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                    }
                }
                //array check
                if (syntaxTree->child[0]->isArray)
                {
                    printf("ERROR(%d): The operation '%s' does not work with arrays.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                }
                switch (syntaxTree->child[0]->nodeType)
                {
                case nInt:
                    syntaxTree->nodeType = nInt;
                    break;
                case nBool:
                    printf("ERROR(%d): Unary '%s' requires an operand of type int but was given type bool.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case nChar:
                    printf("ERROR(%d): Unary '%s' requires an operand of type int but was given type char.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case nVoid:
                    printf("ERROR(%d): Unary '%s' requires an operand of type int but was given type void.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                default:
                    printf("shouldnt be here ++ -- check\n");
                    break;
                }
            }
            else // +=, -=, *= /=, separate from = cause these dont require arrays
            {
                //syntaxTree->nodeType = nInt; //will always be int, may as well set it

                //traverse C0 to get its type
                if (syntaxTree->child[0] != NULL)
                {
                    semanticNoOut(syntaxTree->child[0], symbolTable);
                }
                else
                {
                    //printf("ERROR(YOU SHOULD NOT SEE THIS) AssignKc0.\n");
                }
                syntaxTree->child[0]->isInit = true;
                if (syntaxTree->child[0]->nodeType == uninit || syntaxTree->child[0]->nodeType == nVoid) //if its gonna be in symbol table
                {
                    void *data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                        numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;
                    }
                }
                //c0 check;
                switch (syntaxTree->child[0]->nodeType)
                {
                case nInt:
                    syntaxTree->nodeType = nInt;
                    break;
                case nBool:
                    printf("ERROR(%d): '%s' requires operands of type int but lhs is of type bool.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case nChar:
                    printf("ERROR(%d): '%s' requires operands of type int but lhs is of type char.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case nVoid:
                    printf("ERROR(%d): '%s' requires operands of type int but lhs is of type void.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case nUndefinedType:
                    printf("ERROR(%d): '%s' requires operands of type int but lhs is of type undefined.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                default:
                    printf("bad news assignk\n");
                    break;
                }
                //traverse c1 to get its type
                if (syntaxTree->child[1] != NULL)
                {
                    semanticNoOut(syntaxTree->child[1], symbolTable);
                }
                else
                {
                    //printf("ERROR(YOU SHOULD NOT SEE THIS) AssignKc1.\n");
                }
                if (syntaxTree->child[1]->isNC)
                {
                }
                else if (syntaxTree->child[1]->nodeType == uninit) //if its gonna be in symbol table
                {
                    void *data = symbolTable.lookup(syntaxTree->child[1]->attr.name);
                    if (data == NULL)
                    { //should this error be here?
                        printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[1]->attr.name);
                        numErrors++;
                    }
                    else
                    {
                        static_cast<TreeNode *>(data)->isUsed = true;
                        syntaxTree->child[1]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                        syntaxTree->child[1]->isArray = static_cast<TreeNode *>(data)->isArray;
                    }
                }
                if (syntaxTree->child[1]->isNC)
                {
                }
                else if (!syntaxTree->child[1]->isInit && !(strcmp(syntaxTree->child[1]->attr.name, "[") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "[") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "/=") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "%") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "AND") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "OR") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "==") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "!=") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "<=") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "<") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, ">") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, ">=") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, ":>:") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, ":<:") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "*") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "+") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "-") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "/") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "+=") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "NOT") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "SIZEOF") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "CHSIGN") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "?") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "=") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "-=") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "*=") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "--") == 0) && !(strcmp(syntaxTree->child[1]->attr.name, "++") == 0)) // if c1 isnt inited, and isnt '['
                {
                    printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", syntaxTree->lineno, syntaxTree->child[1]->attr.name);
                    numWarnings++;
                    syntaxTree->child[1]->isInit = true;
                }
                //c1 check;
                switch (syntaxTree->child[1]->nodeType)
                {
                case nInt:
                    syntaxTree->nodeType = nInt;
                    break;
                case nBool:
                    printf("ERROR(%d): '%s' requires operands of type int but lhs is of type bool.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case nChar:
                    printf("ERROR(%d): '%s' requires operands of type int but lhs is of type char.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case nVoid:
                    printf("ERROR(%d): '%s' requires operands of type int but lhs is of type void.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case nUndefinedType:
                    printf("ERROR(%d): '%s' requires operands of type int but lhs is of type undefined.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                default:
                    printf("bad news assignk\n");
                    break;
                }
            }
            //
            /*
            if (syntaxTree->child[0] != NULL)
            {
                semanticAnalysis(syntaxTree->child[0], symbolTable);
            }
            else
            {
                printf("ERROR(YOU SHOULD NOT SEE THIS) AssignKc0.\n");
            }
            if (syntaxTree->child[1] != NULL)
            {
                semanticAnalysis(syntaxTree->child[1], symbolTable);
            }
            else
            {
                //allowed for -- and ++
            }
            */
            break;
        }
        case InitK:
        {
            printf("I THINK THIS CAN BE DELETED? WAS IN SUPPLIED CLASS FILE\n");
            break;
        }
        case CallK:
        {
            if (syntaxTree->child[0] != NULL)
            {
                semanticNoOut(syntaxTree->child[0], symbolTable);
            }
            void *data = symbolTable.lookupGlobal(syntaxTree->attr.name);
            if (data == NULL)
            {
                printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->attr.name);
                numErrors++;
            }
            if (syntaxTree->child[0] != NULL)
            {
                if ((syntaxTree->child[0]->nodekind == ExpK) && (syntaxTree->child[0]->subkind.exp == CallK))
                {
                }
                else
                {
                    if (syntaxTree->child[0] != NULL && !syntaxTree->child[0]->isNC) //if its gonna be in symbol table
                    {
                        void *data;
                        if (strcmp(syntaxTree->attr.name, "++") == 0)
                        {
                            data = symbolTable.lookup(syntaxTree->child[0]->child[0]->attr.name);
                        }
                        else if (syntaxTree->child[0]->nodeType == nBool && (syntaxTree->child[0]->attr.value == 1 || syntaxTree->child[0]->attr.value == 0))
                        {
                            //printf("yes\n");fflush(stdout);
                        }
                        else if (syntaxTree->child[0]->nodeType == nChar) //&& (syntaxTree->child[0]->attr.value == 1 || syntaxTree->child[0]->attr.value == 0))
                        {
                            //printf("yes\n");fflush(stdout);
                        }
                        else
                        {
                            data = symbolTable.lookup(syntaxTree->child[0]->attr.name);
                        }
                        if (data == NULL)
                        { //should this error be here?
                            if (!((syntaxTree->child[0]->nodekind == ExpK) && (syntaxTree->child[0]->subkind.exp == OpK)))
                            {
                                printf("ERROR(%d): Symbol '%s' is not declared.\n", syntaxTree->lineno, syntaxTree->child[0]->attr.name);
                                numErrors++;
                            }
                        }
                        else
                        {
                            //printf("hello: %s\n", static_cast<TreeNode *>(data)->attr.name);
                            syntaxTree->child[0]->nodeType = static_cast<TreeNode *>(data)->nodeType;
                            syntaxTree->child[0]->isArray = static_cast<TreeNode *>(data)->isArray;

                            switch (static_cast<TreeNode *>(data)->subkind.decl)
                            {
                            case FuncK:
                                printf("ERROR(%d): Cannot use function '%s' as a variable.\n", syntaxTree->lineno, static_cast<TreeNode *>(data)->attr.name);
                                numErrors++;
                                break;
                            default:
                                break;
                            }
                        }
                    }
                }
            }
            data = NULL;
            data = symbolTable.lookup(syntaxTree->attr.name);
            if (data == NULL)
            {
                symbolTable.insert(syntaxTree->attr.name, syntaxTree);
            }
            else
            {
                switch (static_cast<TreeNode *>(data)->subkind.decl)
                {
                case FuncK:
                    break;
                case VarK:
                    printf("ERROR(%d): '%s' is a simple variable and cannot be called.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                case ParamK:
                    printf("ERROR(%d): '%s' is a simple variable and cannot be called.\n", syntaxTree->lineno, syntaxTree->attr.name);
                    numErrors++;
                    break;
                default:
                    //printf("not good\n");
                    break;
                }
            }

            break;
        }
        default:
            printf("ERROR: Unknown ExpK!! (SemanticAnalysis)\n");
        }
    }
    else
    {
        printf("UNKNOWN NODEKIND (syntaxTree->nodekind)(semanticanalysis)\n");
    }
    //fflush(stdout);
    //symbolTable.print(pointerPrintAddr); fflush(stdout);
    if (syntaxTree->sibling != NULL)
    {
        semanticNoOut(syntaxTree->sibling, symbolTable);
    }
}