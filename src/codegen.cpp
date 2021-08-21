// Author: Blake Rude
//
//
#include "globals.h"
#include "codegen.h"
#include "symbolTable.h"
#include "emitcode.h"
#include "util.h"
#define INDENT indentno += 1
#define UNINDENT indentno -= 1
int brk = 0; //needs to be used to carry though multiple nodes, ForK, WhileK, BreakK
bool Disable = false;
void codeGeneration(TreeNode *syntaxTree, SymbolTable symbolTable)
{
    if (syntaxTree->nodekind == DeclK)
    {
        switch (syntaxTree->subkind.decl)
        {
        case VarK:
        {
            if (syntaxTree->mem == stat)
            {
            }
            else
            {

                if (syntaxTree->isArray)
                {
                    goffset -= syntaxTree->size;
                    emitRM((char *)"LDC", 3, syntaxTree->size - 1, 6, (char *)"load size of array", syntaxTree->attr.name);
                    if (syntaxTree->mem == glob || syntaxTree->mem == stat) //if global
                    {
                        emitRM((char *)"ST", 3, syntaxTree->location + 1, 0, (char *)"save size of array", syntaxTree->attr.name);
                    }
                    else //if local
                    {
                        emitRM((char *)"ST", 3, syntaxTree->location + 1, 1, (char *)"save size of array", syntaxTree->attr.name);
                    }
                    if (syntaxTree->child[0] != NULL)
                    {
                        codeGeneration(syntaxTree->child[0], symbolTable);
                        emitRM((char *)"LDA", 3, syntaxTree->child[0]->location, 0, (char *)"Load address of char array");
                        emitRM((char *)"LDA", 4, syntaxTree->location, 1, (char *)"address of lhs");
                        emitRM((char *)"LD", 5, 1, 3, (char *)"size of rhs");
                        emitRM((char *)"LD", 6, 1, 4, (char *)"size of lhs");
                        emitRO((char *)"SWP", 5, 6, 6, (char *)"pick smallest size");
                        emitRO((char *)"MOV", 4, 3, 5, (char *)"array op =");
                    }
                }
                else
                {
                    if (syntaxTree->child[0] != NULL)
                    {
                        codeGeneration(syntaxTree->child[0], symbolTable);
                        if (syntaxTree->mem == glob || syntaxTree->mem == stat) // if global
                        {
                            emitRM((char *)"ST", 3, syntaxTree->location, 0, (char *)"Store variable", syntaxTree->attr.name);
                        }
                        else //if local
                        {
                            emitRM((char *)"ST", 3, syntaxTree->location, 1, (char *)"Store variable", syntaxTree->attr.name);
                        }
                    }
                }
            }
            break;
        }
        case FuncK:
        {
            toffset = syntaxTree->size;
            emitComment((char *)"");
            emitComment((char *)"** ** ** ** ** ** ** ** ** ** ** **");
            emitComment((char *)"FUNCTION", syntaxTree->attr.name);
            emitComment((char *)"TOFF set:", toffset);
            syntaxTree->emitLoc = emitSkip(0);
            if (strcmp(syntaxTree->attr.name, "main") == 0)
            {
                mainLocation = syntaxTree->emitLoc;
            }
            emitRM((char *)"ST", 3, -1, 1, (char *)"Store return address");
            //params
            if (syntaxTree->child[0] != NULL)
            {
                codeGeneration(syntaxTree->child[0], symbolTable);
            }
            //body
            if (syntaxTree->child[1] != NULL)
            {
                codeGeneration(syntaxTree->child[1], symbolTable);
            }
            //end of function
            emitComment((char *)"Add standard closing in case there is no return statement");
            emitRM((char *)"LDC", 2, 0, 6, (char *)"Set return value to 0");
            emitRM((char *)"LD", 3, -1, 1, (char *)"Load return address");
            emitRM((char *)"LD", 1, 0, 1, (char *)"Adjust fp");
            emitRM((char *)"JMP", 7, 0, 3, (char *)"Return");
            emitComment((char *)"END FUNCTION", syntaxTree->attr.name);
            break;
        }
        case ParamK:
        {
            break;
        }
        default:
            break;
        }
    }
    // STATEMENTS
    else if (syntaxTree->nodekind == StmtK)
    {
        switch (syntaxTree->subkind.stmt)
        {
        case IfK:
        {
            int savedtoffset = toffset;
            int BP1; //if backpatch
            int BP2; //else backpatch
            //toffset = syntaxTree->size;
            emitComment((char *)"IF");
            if (syntaxTree->child[0] != NULL)
            {
                codeGeneration(syntaxTree->child[0], symbolTable);
                BP1 = emitSkip(1);
            }
            emitComment((char *)"THEN");
            if (syntaxTree->child[1] != NULL)
            {
                codeGeneration(syntaxTree->child[1], symbolTable);
            }
            if (syntaxTree->child[2] != NULL)
            {
                BP2 = emitSkip(1); //needs to be checked here and not below or else instructions are off
            }
            backPatchAJumpToHere((char *)"JZR", 3, BP1, (char *)"Jump around the THEN if false [backpatch]");
            if (syntaxTree->child[2] != NULL)
            {
                emitComment((char *)"ELSE");
                codeGeneration(syntaxTree->child[2], symbolTable);
                backPatchAJumpToHere(BP2, (char *)"Jump around the ELSE [backpatch]");
            }
            emitComment((char *)"END IF");
            toffset = savedtoffset;
            break;
        }
        case WhileK:
        {
            int savedtoffset = toffset;
            int savedBreakPoint = brk;
            int Abs = emitSkip(0);
            int BP1;
            //toffset = syntaxTree->size;
            emitComment((char *)"WHILE");
            if (syntaxTree->child[0] != NULL)
            {
                codeGeneration(syntaxTree->child[0], symbolTable);
            }
            emitRM((char *)"JNZ", 3, 1, 7, (char *)"Jump to while part");
            BP1 = emitSkip(1);
            brk = BP1;
            emitComment((char *)"DO");
            if (syntaxTree->child[1] != NULL)
            {
                codeGeneration(syntaxTree->child[1], symbolTable);
            }
            emitGotoAbs(Abs, (char *)"go to beginning of loop");
            backPatchAJumpToHere(brk, (char *)"Jump past loop [backpatch]");
            brk = savedBreakPoint;
            emitComment((char *)"END WHILE");
            toffset = savedtoffset;
            break;
        }
        case ForK:
        {
            int top = emitSkip(0);
            int back;           
            int loopn;      // want to keep these to know loop index
            int stepn;      // and step value
            emitComment((char *)"FOR");
            toffset = syntaxTree->size;
            int savedtoffset = toffset;
            if (syntaxTree->child[0] != NULL)
            {
                codeGeneration(syntaxTree->child[0], symbolTable);
            }
            top = emitSkip(0);

            if (syntaxTree->child[1] != NULL)
            {
                codeGeneration(syntaxTree->child[1], symbolTable);
            }
            top = emitSkip(0); //correct location
            emitRM((char *)"LD", 4, loopn=toffset+3, 1, (char *)"loop index");
            emitRM((char *)"LD", 5, toffset+2, 1, (char *)"stop value");
            emitRM((char *)"LD", 3, stepn=toffset+1, 1, (char *)"step value");
            emitRO((char *)"SLT", 3, 4, 5, (char *)"Op <");
            emitRM((char *)"JNZ", 3, 1, 7, (char *)"Jump to loop body");
            back = emitSkip(1);
            
            if (syntaxTree->child[2] != NULL)
            {
                codeGeneration(syntaxTree->child[2], symbolTable);
            }
            emitComment((char *)"Bottom of loop increment and jump");
            emitRM((char *)"LD", 3, loopn, 1, (char *)"Load index");
            emitRM((char *)"LD", 5, stepn, 1, (char *)"Load step");
            emitRO((char *)"ADD", 3, 3, 5, (char *)"increment");
            emitRM((char *)"ST", 3, loopn, 1, (char *)"store back to index");
            emitGotoAbs(top, (char *)"go to beginning of loop");
            backPatchAJumpToHere(back, (char *)"Jump past loop [backpatch]");
            emitComment((char *)"END LOOP");
            break;
        }
        case CompoundK:
        {
            toffset = syntaxTree->size;
            emitComment((char *)"COMPOUND");
            emitComment((char *)"TOFF set:", toffset);
            //decls
            if (syntaxTree->child[0] != NULL)
            {
                codeGeneration(syntaxTree->child[0], symbolTable);
            }
            emitComment((char *)"Compound Body");
            //body
            if (syntaxTree->child[1] != NULL)
            {
                codeGeneration(syntaxTree->child[1], symbolTable);
            }
            //end of compound
            emitComment((char *)"TOFF set:", toffset);
            emitComment((char *)"END COMPOUND");
            break;
        }
        case ReturnK:
        {
            emitComment((char *)"RETURN");
            if (syntaxTree->child[0] != NULL)
            {
                codeGeneration(syntaxTree->child[0], symbolTable);
                emitRM((char *)"LDA", 2, 0, 3, (char *)"Copy result to return register");
            }
            emitRM((char *)"LD", 3, -1, 1, (char *)"Load return address");
            emitRM((char *)"LD", 1, 0, 1, (char *)"Adjust fp");
            emitRM((char *)"JMP", 7, 0, 3, (char *)"Return");
            break;
        }
        case BreakK:
        {
            emitComment((char *)"BREAK");
            emitGotoAbs(brk, (char *)"break");
            break;
        }
        case RangeK:
        {
            if (syntaxTree->child[0] != NULL)
            {
                codeGeneration(syntaxTree->child[0], symbolTable);
            }
            emitRM((char *)"ST", 3, toffset + 3, 1, (char *)"save starting value in index variable");
            if (syntaxTree->child[1] != NULL)
            {
                codeGeneration(syntaxTree->child[1], symbolTable);
            }
            //toffset--;
            emitRM((char *)"ST", 3, toffset + 2, 1, (char *)"save stop value");
            if (syntaxTree->child[2] != NULL)
            {
                codeGeneration(syntaxTree->child[2], symbolTable);
            }
            else
            {
                emitRM((char *)"LDC", 3, 1, 6, (char *)"default increment by 1");
            }
            //toffset--;
            emitRM((char *)"ST", 3, toffset + 1, 1, (char *)"save step value");
            //toffset += 2;
            break;
        }
        default:
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
            if (strcmp(syntaxTree->attr.name, "NOT") == 0)
            {
                if (syntaxTree->child[0] != NULL)
                {
                    codeGeneration(syntaxTree->child[0], symbolTable);
                }
                emitRM((char *)"LDC", 4, 1, 6, (char *)"Load 1");
                emitRO((char *)"XOR", 3, 3, 4, (char *)"Op XOR to get logical not");
            }
            else if (strcmp(syntaxTree->attr.name, "CHSIGN") == 0)
            {
                if (syntaxTree->child[0] != NULL)
                {
                    codeGeneration(syntaxTree->child[0], symbolTable);
                }
                emitRO((char *)"NEG", 3, 3, 3, (char *)"Op CHSIGN");
            }
            else if (strcmp(syntaxTree->attr.name, "SIZEOF") == 0)
            {
                if (syntaxTree->child[0] != NULL)
                {
                    codeGeneration(syntaxTree->child[0], symbolTable);
                }
                emitRM((char *)"LD", 3, 1, 3, (char *)"Load array size");
            }
            else if (strcmp(syntaxTree->attr.name, "?") == 0)
            {
                if (syntaxTree->child[0] != NULL)
                {
                    codeGeneration(syntaxTree->child[0], symbolTable);
                }
                emitRO((char *)"RND", 3, 3, 3, (char *)"op ?");
            }
            else if (strcmp(syntaxTree->attr.name, "AND") == 0)
            {
                if (syntaxTree->child[0] != NULL)
                {
                    codeGeneration(syntaxTree->child[0], symbolTable);
                }
                emitRM((char *)"ST", 3, toffset, 1, (char *)"Push left side");
                toffset--;
                emitComment((char *)"TOFF dec:", toffset);
                if (syntaxTree->child[1] != NULL)
                {
                    codeGeneration(syntaxTree->child[1], symbolTable);
                }
                toffset++;
                emitComment((char *)"TOFF inc:", toffset);
                emitRM((char *)"LD", 4, toffset, 1, (char *)"Pop left into ac1");
                emitRO((char *)"AND", 3, 4, 3, (char *)"op AND");
            }
            else if (strcmp(syntaxTree->attr.name, "OR") == 0)
            {
                if (syntaxTree->child[0] != NULL)
                {
                    codeGeneration(syntaxTree->child[0], symbolTable);
                }
                emitRM((char *)"ST", 3, toffset, 1, (char *)"Push left side");
                toffset--;
                emitComment((char *)"TOFF dec:", toffset);
                if (syntaxTree->child[1] != NULL)
                {
                    codeGeneration(syntaxTree->child[1], symbolTable);
                }
                toffset++;
                emitComment((char *)"TOFF inc:", toffset);
                emitRM((char *)"LD", 4, toffset, 1, (char *)"Pop left into ac1");
                emitRO((char *)"OR", 3, 4, 3, (char *)"op OR");
            }
            else if ((strcmp(syntaxTree->attr.name, "==") == 0) || (strcmp(syntaxTree->attr.name, "!=") == 0) ||
                     (strcmp(syntaxTree->attr.name, "<=") == 0) || (strcmp(syntaxTree->attr.name, ">=") == 0) ||
                     (strcmp(syntaxTree->attr.name, "<") == 0) || (strcmp(syntaxTree->attr.name, ">") == 0))
            {
                if (syntaxTree->child[0] != NULL)
                {
                    codeGeneration(syntaxTree->child[0], symbolTable);
                }
                emitRM((char *)"ST", 3, toffset, 1, (char *)"Push left side");
                toffset--;
                emitComment((char *)"TOFF dec:", toffset);
                if (syntaxTree->child[1] != NULL)
                {
                    codeGeneration(syntaxTree->child[1], symbolTable);
                }
                toffset++;
                emitComment((char *)"TOFF inc:", toffset);
                emitRM((char *)"LD", 4, toffset, 1, (char *)"Pop left into ac1");
                if (syntaxTree->child[0]->isArray)
                {
                    emitRM((char *)"LD", 5, 1, 3, (char *)"AC2 <- |RHS|");
                    emitRM((char *)"LD", 6, 1, 4, (char *)"AC3 <- |LHS|");
                    emitRM((char *)"LDA", 2, 0, 5, (char *)"R2 <- |RHS|");
                    emitRO((char *)"SWP", 5, 6, 6, (char *)"pick smallest size");
                    emitRM((char *)"LD", 6, 1, 4, (char *)"AC3 <- |LHS|");
                    emitRO((char *)"CO", 4, 3, 5, (char *)"setup array compare  LHS vs RHS");
                    emitRO((char *)"TNE", 5, 4, 3, (char *)"if not equal then test (AC1, AC)");
                    emitRO((char *)"JNZ", 5, 2, 7, (char *)"jump not equal");
                    emitRM((char *)"LDA", 3, 0, 2, (char *)"AC1 <- |RHS|");
                    emitRM((char *)"LDA", 4, 0, 6, (char *)"AC <- |LHS|");
                }
                if (strcmp(syntaxTree->attr.name, "==") == 0)
                {
                    emitRO((char *)"TEQ", 3, 4, 3, (char *)"Op ==");
                }
                else if (strcmp(syntaxTree->attr.name, "!=") == 0)
                {
                    emitRO((char *)"TNE", 3, 4, 3, (char *)"Op !=");
                }
                else if (strcmp(syntaxTree->attr.name, "<=") == 0)
                {
                    emitRO((char *)"TLE", 3, 4, 3, (char *)"Op <=");
                }
                else if (strcmp(syntaxTree->attr.name, ">=") == 0)
                {
                    emitRO((char *)"TGE", 3, 4, 3, (char *)"Op >=");
                }
                else if (strcmp(syntaxTree->attr.name, "<") == 0)
                {
                    emitRO((char *)"TLT", 3, 4, 3, (char *)"Op <");
                }
                else if (strcmp(syntaxTree->attr.name, ">") == 0)
                {
                    emitRO((char *)"TGT", 3, 4, 3, (char *)"Op >");
                }
            }
            //int,int -> ints: :>:, :<:, *, +, /, -, %
            else if ((strcmp(syntaxTree->attr.name, ":>:") == 0) || (strcmp(syntaxTree->attr.name, ":<:") == 0) ||
                     (strcmp(syntaxTree->attr.name, "*") == 0) || (strcmp(syntaxTree->attr.name, "+") == 0) ||
                     (strcmp(syntaxTree->attr.name, "/") == 0) || (strcmp(syntaxTree->attr.name, "-") == 0) ||
                     (strcmp(syntaxTree->attr.name, "%") == 0))
            {
                if (syntaxTree->child[0] != NULL)
                {
                    codeGeneration(syntaxTree->child[0], symbolTable);
                }
                emitRM((char *)"ST", 3, toffset, 1, (char *)"Push left side");
                toffset--;
                emitComment((char *)"TOFF dec:", toffset);
                if (syntaxTree->child[1] != NULL)
                {
                    codeGeneration(syntaxTree->child[1], symbolTable);
                }
                toffset++;
                emitComment((char *)"TOFF inc:", toffset);
                emitRM((char *)"LD", 4, toffset, 1, (char *)"Pop left into ac1");

                if (strcmp(syntaxTree->attr.name, ":>:") == 0)
                {
                    // There were no tests that used these
                }
                else if (strcmp(syntaxTree->attr.name, ":<:") == 0)
                {
                    // There were no tests that used these
                }
                else if (strcmp(syntaxTree->attr.name, "*") == 0)
                {
                    emitRO((char *)"MUL", 3, 4, 3, (char *)"Op *");
                }
                else if (strcmp(syntaxTree->attr.name, "+") == 0)
                {
                    emitRO((char *)"ADD", 3, 4, 3, (char *)"Op +");
                }
                else if (strcmp(syntaxTree->attr.name, "/") == 0)
                {
                    emitRO((char *)"DIV", 3, 4, 3, (char *)"Op /");
                }
                else if (strcmp(syntaxTree->attr.name, "-") == 0)
                {
                    emitRO((char *)"SUB", 3, 4, 3, (char *)"Op -");
                }
                else if (strcmp(syntaxTree->attr.name, "%") == 0)
                {
                    emitRO((char *)"MOD", 3, 4, 3, (char *)"Op %");
                }
            }
            else // '['
            {
                if (syntaxTree->child[0]->mem == para)
                {
                    emitRM((char *)"LD", 3, syntaxTree->child[0]->location, 1, (char *)"Load address of base of array", syntaxTree->child[0]->attr.name);
                }
                else
                {
                    if (syntaxTree->child[0]->mem == glob || syntaxTree->child[0]->mem == stat)
                    {
                        emitRM((char *)"LDA", 3, syntaxTree->child[0]->location, 0, (char *)"Load address of base of array", syntaxTree->child[0]->attr.name);
                    }
                    else
                    {
                        emitRM((char *)"LDA", 3, syntaxTree->child[0]->location, 1, (char *)"Load address of base of array", syntaxTree->child[0]->attr.name);
                    }
                }
                emitRM((char *)"ST", 3, toffset, 1, (char *)"Push left side");
                toffset--;
                emitComment((char *)"TOFF dec:", toffset);
                if (syntaxTree->child[1] != NULL)
                {
                    codeGeneration(syntaxTree->child[1], symbolTable);
                }
                toffset++;
                emitComment((char *)"TOFF inc:", toffset);
                emitRM((char *)"LD", 4, toffset, 1, (char *)"Pop left into ac1");
                emitRO((char *)"SUB", 3, 4, 3, (char *)"compute location from index");
                emitRM((char *)"LD", 3, 0, 3, (char *)"Load array element");
            }
            break;
        }
        case ConstantK:
        {
            switch (syntaxTree->nodeType)
            {
            case nBool:
            {
                emitRM((char *)"LDC", 3, syntaxTree->attr.value, 6, (char *)"Load Boolean constant");
                break;
            }
            case nInt:
            {
                emitRM((char *)"LDC", 3, syntaxTree->attr.value, 6, (char *)"Load integer constant");
                break;
            }
            case nChar:
            {
                if (syntaxTree->isStringConstant)
                {
                    //need to get rid of "s and \0s
                    char *newname = (char *)malloc(strlen(syntaxTree->attr.name));
                    char *name = syntaxTree->attr.name;
                    int i2 = 0;
                    for (int i = 0; i < strlen(syntaxTree->attr.name); i++)
                    {
                        if (name[i] != '\"')
                        {
                            if (name[i] != '\\' && name[i] != '0')
                            {
                                newname[i2] = syntaxTree->attr.name[i];
                                i2++;
                            }
                        }
                    }
                    newname[i2] = '\0';
                    emitStrLit(syntaxTree->location, newname);
                    goffset--;
                    emitRM((char *)"LDA", 3, syntaxTree->location, 0, (char *)"Load address of char array");
                }
                else
                {
                    emitRM((char *)"LDC", 3, syntaxTree->attr.cvalue, 6, (char *)"Load char constant");
                }
                break;
            }
            }
            break;
        }
        case IdK:
        {
            if (syntaxTree->mem == para)
            {
                if (syntaxTree->mem == glob || syntaxTree->mem == stat) //if global
                {
                    if (syntaxTree->isArray)
                    {
                        emitRM((char *)"LD", 3, syntaxTree->location, 0, (char *)"Load address of base of array", syntaxTree->attr.name);
                    }
                    else
                    {
                        emitRM((char *)"LD", 3, syntaxTree->location, 0, (char *)"Load variable", syntaxTree->attr.name);
                    }
                }
                else // if not global
                {
                    if (syntaxTree->isArray)
                    {
                        emitRM((char *)"LD", 3, syntaxTree->location, 1, (char *)"Load address of base of array", syntaxTree->attr.name);
                    }
                    else
                    {
                        emitRM((char *)"LD", 3, syntaxTree->location, 1, (char *)"Load variable", syntaxTree->attr.name);
                    }
                }
            }
            else
            {
                if (syntaxTree->mem == glob || syntaxTree->mem == stat) //if global
                {
                    if (syntaxTree->isArray)
                    {
                        emitRM((char *)"LDA", 3, syntaxTree->location, 0, (char *)"Load address of base of array", syntaxTree->attr.name);
                    }
                    else
                    {
                        emitRM((char *)"LD", 3, syntaxTree->location, 0, (char *)"Load variable", syntaxTree->attr.name);
                    }
                }
                else // if not global
                {
                    if (syntaxTree->isArray)
                    {
                        emitRM((char *)"LDA", 3, syntaxTree->location, 1, (char *)"Load address of base of array", syntaxTree->attr.name);
                    }
                    else
                    {
                        emitRM((char *)"LD", 3, syntaxTree->location, 1, (char *)"Load variable", syntaxTree->attr.name);
                    }
                }
            }

            break;
        }
        case AssignK:
        {
            emitComment((char *)"EXPRESSION");
            if (strcmp(syntaxTree->attr.name, "=") == 0) // =
            {
                if (strcmp(syntaxTree->child[0]->attr.name, "[") == 0) // if lhs indexed array
                {
                    if (syntaxTree->child[1] != NULL)
                    {
                        codeGeneration(syntaxTree->child[0]->child[1], symbolTable);
                    }
                    emitRM((char *)"ST", 3, toffset, 1, (char *)"Push index");
                    toffset--;
                    emitComment((char *)"TOFF dec:", toffset);
                    if (syntaxTree->child[1] != NULL)
                    {
                        codeGeneration(syntaxTree->child[1], symbolTable);
                    }
                    toffset++;
                    emitComment((char *)"TOFF inc:", toffset);
                    emitRM((char *)"LD", 4, toffset, 1, (char *)"Pop index");
                    if (syntaxTree->child[0]->child[0]->mem == para)
                    {
                        emitRM((char *)"LD", 5, syntaxTree->child[0]->child[0]->location, 1, (char *)"Load address of base of array", syntaxTree->child[0]->child[0]->attr.name);
                    }
                    else
                    {
                        if (syntaxTree->child[0]->child[0]->mem == glob || syntaxTree->child[0]->child[0]->mem == stat) // if global
                        {
                            emitRM((char *)"LDA", 5, syntaxTree->child[0]->child[0]->location, 0, (char *)"Load address of base of array", syntaxTree->child[0]->child[0]->attr.name);
                        }
                        else //if local
                        {
                            emitRM((char *)"LDA", 5, syntaxTree->child[0]->child[0]->location, 1, (char *)"Load address of base of array", syntaxTree->child[0]->child[0]->attr.name);
                        }
                    }
                    emitRO((char *)"SUB", 5, 5, 4, (char *)"Compute offset of value");
                    emitRM((char *)"ST", 3, 0, 5, (char *)"Store variable", syntaxTree->child[0]->child[0]->attr.name);
                }
                else
                {
                    if (syntaxTree->child[1] != NULL)
                    {
                        codeGeneration(syntaxTree->child[1], symbolTable);
                    }
                    if (syntaxTree->child[0]->isArray)
                    {
                        if (syntaxTree->child[0]->mem == glob || syntaxTree->child[0]->mem == stat) // if global
                        {
                            emitRM((char *)"LDA", 4, syntaxTree->child[0]->location, 0, (char *)"address of lhs");
                        }
                        else //if local
                        {
                            emitRM((char *)"LDA", 4, syntaxTree->child[0]->location, 1, (char *)"address of lhs");
                        }
                        emitRM((char *)"LD", 5, 1, 3, (char *)"size of rhs");
                        emitRM((char *)"LD", 6, 1, 4, (char *)"size of lhs");
                        emitRO((char *)"SWP", 5, 6, 6, (char *)"pick smallest size");
                        emitRO((char *)"MOV", 4, 3, 5, (char *)"array op =");
                    }
                    else
                    {
                        if (syntaxTree->child[0]->mem == glob || syntaxTree->child[0]->mem == stat) // if  global
                        {
                            emitRM((char *)"ST", 3, syntaxTree->child[0]->location, 0, (char *)"Store variable", syntaxTree->child[0]->attr.name);
                        }
                        else //if not global
                        {
                            emitRM((char *)"ST", 3, syntaxTree->child[0]->location, 1, (char *)"Store variable", syntaxTree->child[0]->attr.name);
                        }
                    }
                }
            }
            else if (strcmp(syntaxTree->attr.name, "++") == 0 || strcmp(syntaxTree->attr.name, "--") == 0) //separate cause no c1
            {
                if (strcmp(syntaxTree->child[0]->attr.name, "[") == 0)
                {
                    codeGeneration(syntaxTree->child[0]->child[1], symbolTable);
                    if (syntaxTree->child[0]->child[0]->mem == para)
                    {
                        emitRM((char *)"LD", 5, syntaxTree->child[0]->child[0]->location, 1, (char *)"Load address of base of array", syntaxTree->child[0]->child[0]->attr.name);
                    }
                    else
                    {
                        if (syntaxTree->child[0]->child[0]->mem == glob || syntaxTree->child[0]->child[0]->mem == stat) // if global
                        {
                            emitRM((char *)"LDA", 5, syntaxTree->child[0]->child[0]->location, 0, (char *)"Load address of base of array", syntaxTree->child[0]->child[0]->attr.name);
                        }
                        else //if local
                        {
                            emitRM((char *)"LDA", 5, syntaxTree->child[0]->child[0]->location, 1, (char *)"Load address of base of array", syntaxTree->child[0]->child[0]->attr.name);
                        }
                    }
                    emitRO((char *)"SUB", 5, 5, 3, (char *)"Compute offset of value");
                    emitRM((char *)"LD", 3, 0, 5, (char *)"load lhs variable", syntaxTree->child[0]->child[0]->attr.name);
                    if (strcmp(syntaxTree->attr.name, "++") == 0)
                    {
                        emitRM((char *)"LDA", 3, 1, 3, (char *)"increment value of", syntaxTree->child[0]->child[0]->attr.name);
                    }
                    else if (strcmp(syntaxTree->attr.name, "--") == 0)
                    {
                        emitRM((char *)"LDA", 3, -1, 3, (char *)"decrement value of", syntaxTree->child[0]->child[0]->attr.name);
                    }
                    emitRM((char *)"ST", 3, 0, 5, (char *)"Store variable", syntaxTree->child[0]->child[0]->attr.name);
                }
                else
                {
                    if (syntaxTree->child[0]->mem == glob || syntaxTree->child[0]->mem == stat)
                    {
                        emitRM((char *)"LD", 3, syntaxTree->child[0]->location, 0, (char *)"load lhs variable", syntaxTree->child[0]->attr.name);
                    }
                    else
                    {
                        emitRM((char *)"LD", 3, syntaxTree->child[0]->location, 1, (char *)"load lhs variable", syntaxTree->child[0]->attr.name);
                    }
                    if (strcmp(syntaxTree->attr.name, "++") == 0)
                    {
                        emitRM((char *)"LDA", 3, 1, 3, (char *)"increment value of", syntaxTree->child[0]->attr.name);
                    }
                    else if (strcmp(syntaxTree->attr.name, "--") == 0)
                    {
                        emitRM((char *)"LDA", 3, -1, 3, (char *)"decrement value of", syntaxTree->child[0]->attr.name);
                    }
                    if (syntaxTree->child[0]->mem == glob || syntaxTree->child[0]->mem == stat) //if global
                    {
                        emitRM((char *)"ST", 3, syntaxTree->child[0]->location, 0, (char *)"Store variable", syntaxTree->child[0]->attr.name);
                    }
                    else
                    {
                        emitRM((char *)"ST", 3, syntaxTree->child[0]->location, 1, (char *)"Store variable", syntaxTree->child[0]->attr.name);
                    }
                }
            }
            else // +=, -=, *= /=, separate from = cause these dont require arrays
            {
                if (strcmp(syntaxTree->child[0]->attr.name, "[") == 0) // if lhs is array
                {
                    if (syntaxTree->child[1] != NULL)
                    {
                        codeGeneration(syntaxTree->child[0]->child[1], symbolTable);
                    }
                    emitRM((char *)"ST", 3, toffset, 1, (char *)"Push index");
                    toffset--;
                    emitComment((char *)"TOFF dec:", toffset);
                    if (syntaxTree->child[1] != NULL)
                    {
                        codeGeneration(syntaxTree->child[1], symbolTable);
                    }
                    toffset++;
                    emitComment((char *)"TOFF inc:", toffset);
                    emitRM((char *)"LD", 4, toffset, 1, (char *)"Pop index");
                    if (syntaxTree->child[0]->child[0]->mem == para)
                    {
                        emitRM((char *)"LD", 5, syntaxTree->child[0]->child[0]->location, 1, (char *)"Load address of base of array", syntaxTree->child[0]->child[0]->attr.name);
                    }
                    else
                    {
                        if (syntaxTree->child[0]->child[0]->mem == glob || syntaxTree->child[0]->child[0]->mem == stat) // if global
                        {
                            emitRM((char *)"LDA", 5, syntaxTree->child[0]->child[0]->location, 0, (char *)"Load address of base of array", syntaxTree->child[0]->child[0]->attr.name);
                        }
                        else //if local
                        {
                            emitRM((char *)"LDA", 5, syntaxTree->child[0]->child[0]->location, 1, (char *)"Load address of base of array", syntaxTree->child[0]->child[0]->attr.name);
                        }
                    }
                    emitRO((char *)"SUB", 5, 5, 4, (char *)"Compute offset of value");
                    emitRM((char *)"LD", 4, 0, 5, (char *)"load lhs variable", syntaxTree->child[0]->child[0]->attr.name);
                    if (strcmp(syntaxTree->attr.name, "+=") == 0)
                    {
                        emitRO((char *)"ADD", 3, 4, 3, (char *)"op +=");
                    }
                    else if (strcmp(syntaxTree->attr.name, "-=") == 0)
                    {
                        emitRO((char *)"SUB", 3, 4, 3, (char *)"op -=");
                    }
                    else if (strcmp(syntaxTree->attr.name, "*=") == 0)
                    {
                        emitRO((char *)"MUL", 3, 4, 3, (char *)"op *=");
                    }
                    else if (strcmp(syntaxTree->attr.name, "/=") == 0)
                    {
                        emitRO((char *)"DIV", 3, 4, 3, (char *)"op /=");
                    }
                    emitRM((char *)"ST", 3, 0, 5, (char *)"Store variable", syntaxTree->child[0]->child[0]->attr.name);
                }
                else
                {
                    if (syntaxTree->child[1] != NULL)
                    {
                        codeGeneration(syntaxTree->child[1], symbolTable);
                    }

                    emitRM((char *)"LD", 4, syntaxTree->child[0]->location, 0, (char *)"load lhs variable", syntaxTree->child[0]->attr.name);
                    if (strcmp(syntaxTree->attr.name, "+=") == 0)
                    {
                        emitRO((char *)"ADD", 3, 4, 3, (char *)"op +=");
                    }
                    else if (strcmp(syntaxTree->attr.name, "-=") == 0)
                    {
                        emitRO((char *)"SUB", 3, 4, 3, (char *)"op -=");
                    }
                    else if (strcmp(syntaxTree->attr.name, "*=") == 0)
                    {
                        emitRO((char *)"MUL", 3, 4, 3, (char *)"op *=");
                    }
                    else if (strcmp(syntaxTree->attr.name, "/=") == 0)
                    {
                        emitRO((char *)"DIV", 3, 4, 3, (char *)"op /=");
                    }
                    if (syntaxTree->child[0]->mem == glob || syntaxTree->child[0]->mem == stat) //if global
                    {
                        emitRM((char *)"ST", 3, syntaxTree->child[0]->location, 0, (char *)"Store variable", syntaxTree->child[0]->attr.name);
                    }
                    else
                    {
                        emitRM((char *)"ST", 3, syntaxTree->child[0]->location, 1, (char *)"Store variable", syntaxTree->child[0]->attr.name);
                    }
                }
            }
            break;
        }
        case InitK:
        {
            break;
        }
        case CallK:
        {
            emitComment((char *)"EXPRESSION");
            int savedtoffset = toffset;
            emitComment((char *)"CALL", syntaxTree->attr.name);
            emitRM((char *)"ST", 1, toffset, 1, (char *)"Store fp in ghost frame for", syntaxTree->attr.name);
            // Decrement 2 separate times, matches provided .tm
            toffset--;
            emitComment((char *)"TOFF dec:", toffset);
            toffset--;
            emitComment((char *)"TOFF dec:", toffset);
            //Need to call codegeneration for each param HERE
            if (syntaxTree->child[0] != NULL)
            {
                TreeNode *temp = syntaxTree->child[0];
                int i = 1;
                for (temp; temp != NULL; temp = temp->sibling)
                {
                    emitComment((char *)"Param", i);
                    i++;
                    Disable = true;
                    codeGeneration(temp, symbolTable);
                    Disable = false;
                    emitRM((char *)"ST", 3, toffset, 1, (char *)"Push parameter");

                    toffset--;
                    emitComment((char *)"TOFF dec:", toffset);
                }
            }
            emitComment((char *)"Param end", syntaxTree->attr.name);
            toffset = savedtoffset;
            void *data = symbolTable.lookupGlobal(syntaxTree->attr.name);
            emitRM((char *)"LDA", 1, toffset, 1, (char *)"Ghost frame becomes new active frame");
            emitRM((char *)"LDA", 3, 1, 7, (char *)"Return address in ac");
            emitRM((char *)"JMP", 7, static_cast<TreeNode *>(data)->emitLoc - emitSkip(0) - 1, 7, (char *)"CALL", syntaxTree->attr.name);
            emitRM((char *)"LDA", 3, 0, 2, (char *)"Save the result in ac");

            emitComment((char *)"Call end", syntaxTree->attr.name);
            break;
        }
        default:
            break;
        }
    }
    if (syntaxTree->sibling != NULL)
    {
        if ((syntaxTree->sibling->mem != glob) && (syntaxTree->sibling->mem != stat) && !Disable)
        {
            codeGeneration(syntaxTree->sibling, symbolTable);
        }
    }
}

// Not the best implementation but I was having trouble doing the way
// which was described in class
//
// Still works though
// Just runs through the whole tree but only prints if is a static VarK
void GenerateStatics(TreeNode *syntaxTree, SymbolTable symbolTable)
{
    while (syntaxTree != NULL)
    {
        if (syntaxTree->nodekind == DeclK)
        {
            if (syntaxTree->subkind.decl == VarK)
            {
                if (syntaxTree->mem == stat)
                {
                    if (syntaxTree->isArray)
                    {
                        goffset -= syntaxTree->size;
                        emitRM((char *)"LDC", 3, syntaxTree->size - 1, 6, (char *)"load size of array", syntaxTree->attr.name);
                        if (syntaxTree->mem == glob || syntaxTree->mem == stat)
                        {
                            emitRM((char *)"ST", 3, syntaxTree->location + 1, 0, (char *)"save size of array", syntaxTree->attr.name);
                        }
                        else
                        {
                            emitRM((char *)"ST", 3, syntaxTree->location + 1, 1, (char *)"save size of array", syntaxTree->attr.name);
                        }
                        if (syntaxTree->child[0] != NULL)
                        {
                            codeGeneration(syntaxTree->child[0], symbolTable);
                            //emitRM((char *)"LDA", 3, syntaxTree->child[0]->location, 0, (char *)"Load address of char array");
                            emitRM((char *)"LDA", 4, syntaxTree->location, 1, (char *)"address of lhs");
                            emitRM((char *)"LD", 5, 1, 3, (char *)"size of rhs");
                            emitRM((char *)"LD", 6, 1, 4, (char *)"size of lhs");
                            emitRO((char *)"SWP", 5, 6, 6, (char *)"pick smallest size");
                            emitRO((char *)"MOV", 4, 3, 5, (char *)"array op =");
                        }
                    }
                    else
                    {
                        if (syntaxTree->child[0] != NULL)
                        {
                            codeGeneration(syntaxTree->child[0], symbolTable);
                            if (syntaxTree->mem == glob || syntaxTree->mem == stat) // if global
                            {
                                emitRM((char *)"ST", 3, syntaxTree->location, 0, (char *)"Store variable", syntaxTree->attr.name);
                            }
                            else //if local
                            {
                                emitRM((char *)"ST", 3, syntaxTree->location, 1, (char *)"Store variable", syntaxTree->attr.name);
                            }
                        }
                    }
                }
            }
        }
        for (int i = 0; i < 3; i++)
        {
            GenerateStatics(syntaxTree->child[i], symbolTable);
        }
        syntaxTree = syntaxTree->sibling;
    }
}