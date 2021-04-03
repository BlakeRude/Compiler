%{
/*
 * Blake Rude
 * CS445 Compiler Design
 * This comment is in parser.y, Bison File
 * Spring 2021
 */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "scanType.h"
#include "globals.h"
#include "ourgetopt.h"
#include "util.h"
#include "symbolTable.h"
#include "semantic.h"

extern int yylex();
extern FILE *yyin;
extern int yydebug;
static TreeNode* savedTree;
static TreeNode* IOtree;
static SymbolTable savedsymbolTable;
int numErrors=0;
int numWarnings=0;
#define YYERROR_VERBOSE
void yyerror(const char *msg)
{
      printf("ERROR(PARSER): %s\n", msg);
}
%}
%union {
    ExpType etype;
    DeclType dtype;
    TokenData* tokenData;
    TreeNode* tree;
}
%token <tokenData> ID NUMCONST CHARCONST STRINGCONST KEYWORD INVALID BOOLCONST STATIC INT BOOL CHAR IF NOT THEN ELSE WHILE OR DO FOR TO BY RETURN BREAK ADDASS MINASS DIVASS MULASS INC DEC LEQ GEQ NEQ EQ MAX MIN ttrue ffalse ANDD
%token <tokenData> '<' '>' '=' '*' '+' '-' '/' '%' ')' '(' ';' ':' '[' ']' '{' '}' ',' '?'
%type <tree> program declList decl varDecl scopedVarDecl varDeclList varDeclInit varDeclId typeSpec funDecl parms parmList parmTypeList parmIdList parmId stmt matched unmatched expStmt compoundStmt matchediterStmt unmatchediterStmt returnStmt breakStmt localDecls stmtList iterRange exp simpleExp andExp unaryRelExp relExp relop minmaxExp minmaxop sumExp sumop mulExp mulop unaryExp unaryop factor mutable immutable call args argList constant
%%
program         : declList
                    {
                        savedTree = $1;
                    }
                ;
declList        : declList decl
                    {
                        //from book appendix
                        TreeNode* t = $1;
                        if (t != NULL)
                        {
                            while (t->sibling != NULL)
                            { t = t->sibling; }
                            t->sibling = $2;
                            $$ = $1;
                        }
                        else
                        { $$ = $2; }
                    }
                | decl
                    { $$ = $1; }
                ;
decl            : varDecl
                    { $$ = $1; }
                | funDecl
                    { $$ = $1; }
                ;
varDecl         : typeSpec varDeclList ';'
                    {
                        //i tried dealing with typespec multiple ways
                        //i think its best to just have it be a node with
                        //relevent information
                        
                        TreeNode* t1 = $1;      //typeSpec, has relevant typing info
                        TreeNode* t2 = $2;      //Our list of varDecl, go and set t1's type info to all siblings
                        if(t1 != NULL && t2 != NULL)
                        {
                            TreeNode* temp = t2;
                            while(temp->sibling != NULL)
                            {
                                temp->declType = t1->declType;
                                temp->nodeType = t1->nodeType;
                                temp = temp->sibling;
                                //printf("type lineno: %i\n", $1->lineno);
                                //temp->lineno = $1->lineno;
                            }
                            temp->declType = t1->declType;
                            temp->nodeType = t1->nodeType;
                        }
                        $$ = $2;
                    }
                ;
scopedVarDecl   : STATIC typeSpec varDeclList ';'
                    {
                        //Same as varDecl, but set isStatic
                        TreeNode* t1 = $2;      //typeSpec, has relevant typing info
                        TreeNode* t2 = $3;      //Our list of varDecl, go and set t1's type info to all siblings

                        if(t1 != NULL && t2 != NULL)
                        {
                            TreeNode* temp = t2;
                            while(temp->sibling != NULL)
                            {
                                temp->declType = t1->declType;
                                temp->nodeType = t1->nodeType;
                                //printf("sVD1: ",t1->attr.name);
                                //temp->attr.name = t1->attr.name;
                                temp->isStatic = true;
                                temp = temp->sibling;
                            }
                            temp->isStatic = true;
                            temp->declType = t1->declType;
                            temp->nodeType = t1->nodeType;
                        }
                        $$ = $3;
                    }
                | typeSpec varDeclList ';'
                    {
                        TreeNode* t1 = $1;      //typeSpec, has relevant typing info
                        TreeNode* t2 = $2;      //Our list of varDecl, go and set t1's type info to all siblings
                        if(t1 != NULL && t2 != NULL)
                        {
                            TreeNode* temp = t2;
                            while(temp->sibling != NULL)
                            {
                                temp->declType = t1->declType;
                                temp->nodeType = t1->nodeType;
                                temp = temp->sibling;
                                //printf("type lineno: %i\n", $1->lineno);
                                //temp->lineno = $1->lineno;
                            }
                            temp->declType = t1->declType;
                            temp->nodeType = t1->nodeType;
                            //temp = temp->sibling;
                        }
                        $$ = $2;
                    }
                ;
varDeclList     : varDeclList ',' varDeclInit
                    {
                        TreeNode* t = $1;
                        if(t != NULL)
                        {
                            while(t->sibling != NULL)
                            { t = t->sibling;}
                            t->sibling = $3;
                            $$ = $1;
                        }
                        else
                        { $$ = $3; }
                    }
                | varDeclInit
                    { $$ = $1; }
                ;
varDeclInit     : varDeclId
                    {
                        //printf("testvardeclinit: %s\n", $1->attr.name);
                        //printf("testvardeclinit: %i\n", $1->lineno);
                        $$ = $1;                //not sure if better to do this or $$ = t, not experienced enough with pointers
                    }
                | varDeclId ':' simpleExp
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = DeclK;
                        t->subkind.decl = VarK;
                        //printf("vDI: ",$1->attr.name);
                        t->attr.name = $1->attr.name;
                        t->lineno = $1->lineno;
                        t->isArray = $1->isArray;
                        t->child[0] = $3;
                        t->child[0]->isC0 = 1;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        t->lineno = $2->linenum;    //get linenums from tokenData blocks 
                        $$ = t;
                    }
                ;
varDeclId       : ID
                    {
                        TreeNode* t = new TreeNode;
                        //t->nodekind = DeclK;
                        //t->subkind.decl = VarK;
                        //printf("test: %s\n", $1->tokenstr);
                        t->attr.name = $1->tokenstr;
                        t->lineno = $1->linenum;
                        
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | ID '[' NUMCONST ']'
                    {
                        TreeNode* t = new TreeNode;
                        //t->nodekind = DeclK;
                        //t->subkind.decl = VarK;
                        t->isArray = true;
                        //printf("%s\n",$1->tokenstr);
                        t->attr.size = $3->nvalue;
                        t->attr.name = $1->tokenstr;
                        t->lineno = $1->linenum;
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                ;
typeSpec        : INT
                    {
                        TreeNode* t = new TreeNode;
                        t->declType = DeclInteger;      //from enum DeclType
                        t->nodeType = nInt;
                        t->lineno = $1->linenum;    //(tokenData)
                        //printf("tS: ",$1->svalue);
                        //t->attr.name = $1->svalue;
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | BOOL
                    {
                        TreeNode* t = new TreeNode;
                        t->declType = DeclBoolean;      //from enum DeclType
                        t->nodeType = nBool;
                        t->lineno = $1->linenum;    //(tokenData)
                        //printf("tS: ",$1->svalue);
                        //t->attr.name = $1->tokenstr; 
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | CHAR
                    {
                        TreeNode* t = new TreeNode;
                        t->declType = DeclChar;         //from enum DeclType
                        t->nodeType = nChar;
                        t->lineno = $1->linenum;    //(tokenData)
                        //printf("tS: ",$1->svalue);
                        //t->attr.name = $1->tokenstr;
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                ;
funDecl         : typeSpec ID '(' parms ')' stmt
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = DeclK;
                        t->subkind.decl = FuncK;
                        t->attr.name = $2->tokenstr;
                        t->lineno = $2->linenum;
                        t->declType = $1->declType;
                        t->nodeType = $1->nodeType;
                        if($4 == NULL && $6 == NULL)
                        {
                            t->child[0] = NULL;
                            t->child[1] = NULL;
                            t->child[2] = NULL;
                        }
                        else if($4 != NULL && $6 == NULL)
                        {
                            t->child[0] = $4;
                            t->child[0]->isC0 = 1;
                            t->child[1] = NULL;
                            t->child[2] = NULL;
                        }
                        else if($4 == NULL && $6 != NULL)
                        {
                            t->child[0] = NULL;
                            t->child[1] = $6;
                            t->child[1]->isC1 = 1;
                            t->child[2] = NULL;
                        }
                        else
                        {
                            t->child[0] = $4;
                            t->child[0]->isC0 = 1;
                            t->child[1] = $6;
                            t->child[1]->isC1 = 1;
                            t->child[2] = NULL;
                        }
                        $$ = t;
                    }
                | ID '(' parms ')' stmt
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = DeclK;
                        t->subkind.decl = FuncK; 
                        //printf("fD2: ",$2->svalue);
                        t->attr.name = $1->tokenstr; 
                        t->lineno = $2->linenum;
                        t->declType = DeclVoid;
                        t->nodeType = nVoid;
                        if($3 == NULL && $5 == NULL)
                        {
                            t->child[0] = NULL;
                            t->child[1] = NULL;
                            t->child[2] = NULL;
                        }
                        else if($3 != NULL && $5 == NULL)
                        {
                            t->child[0] = $3;
                            t->child[0]->isC0 = 1;
                            t->child[1] = NULL;
                            t->child[2] = NULL;
                        }
                        else if($3 == NULL && $5 != NULL)
                        {
                            t->child[0] = NULL;
                            t->child[1] = $5;
                            t->child[1]->isC1 = 1;
                            t->child[2] = NULL;
                        }
                        else
                        {
                            t->child[0] = $3;
                            t->child[0]->isC0 = 1;
                            t->child[1] = $5;
                            t->child[1]->isC1 = 1;
                            t->child[2] = NULL;
                        }
                        /*
                        if( $3 != NULL){
                            t->child[0] = $3;
                            t->child[0]->isC0 = 1;
                            t->child[1] = $5;
                            t->child[1]->isC1 = 1;
                            t->child[2] = NULL;
                        }
                        else{
                            t->child[0] = NULL;
                            t->child[1] = $5;
                            t->child[1]->isC1 = 1;
                            t->child[2] = NULL;
                        }
                        */
                        $$ = t;
                    }
                ;
parms           : parmList
                    { $$ = $1; }
                | /* Epsilon */
                    { $$ = NULL;}
                ;
parmList        : parmList ';' parmTypeList
                    {
                        TreeNode* t = $1;
                        if(t != NULL)
                        {
                            while(t->sibling != NULL)
                            { t = t->sibling; }
                            t->lineno = $2->linenum; //steal linenum from ';'
                            t->sibling = $3;
                            $$ = $1;
                        }
                        else
                        { $$ = $3; }
                    }
                | parmTypeList
                    { $$ = $1; }
                ;
parmTypeList    : typeSpec parmIdList
                    {
                        TreeNode* t = $2;
                        if(t != NULL)
                        {   
                            t->nodekind = DeclK;
                            t->declType = $1->declType;
                            t->nodeType = $1->nodeType;
                            t->subkind.decl = ParamK;
                            while(t->sibling != NULL)
                            {
                                t = t->sibling;
                                t->nodekind = DeclK;
                                t->declType = $1->declType;
                                t->nodeType = $1->nodeType;
                                t->subkind.decl = ParamK;
                            }
                            $$ = $2;
                        }
                        else
                        { $$ = $2; }
                    }
                ;
parmIdList      : parmIdList ',' parmId
                    {
                        //Copied and modified from declList above
                        TreeNode* t = $1;
                        if(t != NULL)
                        {
                            while(t->sibling != NULL)
                            { t = t->sibling; }
                            t->lineno = $2->linenum; //steal linenum from ','
                            t->sibling = $3;
                            $$ = $1;
                        }
                        else
                        { $$ = $3; }
                    }
                | parmId
                    { $$ = $1; }
                ;
parmId          : ID
                    {
                        TreeNode* t = new TreeNode;
                        //printf("pId: ",$1->svalue);
                        t->attr.name = $1->tokenstr;
                        t->lineno = $1->linenum;
                        t->subkind.exp = IdK;
                        t->nodekind = ExpK;
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | ID '['']'
                    {
                        TreeNode* t = new TreeNode;
                        //printf("pId2: ",$1->svalue);
                        t->attr.name = $1->tokenstr;
                        t->lineno = $1->linenum;
                        t->subkind.exp = IdK;
                        t->nodekind = ExpK;
                        t->isArray = true;
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                ;
stmt            : matched
                    { $$ = $1; }
                | unmatched
                    { $$ = $1; }
                ;
matched         : IF simpleExp THEN matched ELSE matched
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = StmtK;            //nodekind
                        t->subkind.stmt = IfK;               //StmtKind
                        t->lineno = $1->linenum;        //TokenData: linenum
                        if($4 == NULL && $6 == NULL)
                        {
                            t->child[0] = $2;
                            t->child[0]->isC0 = 1;
                            t->child[1] = NULL;
                            t->child[2] = NULL;
                        }
                        else if($4 != NULL && $6 == NULL)
                        {
                            t->child[0] = $2;
                            t->child[0]->isC0 = 1;
                            t->child[1] = $4;
                            t->child[1]->isC1 = 1;
                            t->child[2] = NULL;
                        }
                        else if($4 == NULL && $6 != NULL)
                        {
                            t->child[0] = $2;
                            t->child[0]->isC0 = 1;
                            t->child[1] = NULL;
                            t->child[2] = $6;
                            t->child[2]->isC2 = 1;
                        }
                        else{
                            t->child[0] = $2;
                            t->child[0]->isC0 = 1;
                            t->child[1] = $4;
                            t->child[1]->isC1 = 1;
                            t->child[2] = $6;
                         t->child[2]->isC2 = 1;
                        }
                        $$ = t;
                    }
                | expStmt
                    { $$ = $1; }
                | compoundStmt
                    { $$ = $1; }
                | matchediterStmt
                    { $$ = $1; }
                | returnStmt
                    { $$ = $1; }
                | breakStmt
                    { $$ = $1; }
                ;
unmatched       : IF simpleExp THEN matched
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = StmtK;            //nodekind
                        t->subkind.stmt = IfK;               //StmtKind
                        t->lineno = $1->linenum;        //TokenData: linenum
                        if($4 != NULL){
                            t->child[0] = $2;
                            t->child[0]->isC0 = 1;
                            t->child[1] = $4;
                            t->child[1]->isC1 = 1;
                            t->child[2] = NULL;
                        }
                        else{
                            t->child[0] = $2;
                            t->child[0]->isC0 = 1;
                            t->child[1] = NULL;
                            t->child[2] = NULL;
                        }
                        $$ = t;
                    }
                | IF simpleExp THEN unmatched
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = StmtK;            //nodekind
                        t->subkind.stmt = IfK;               //StmtKind
                        t->lineno = $1->linenum;        //TokenData: linenum
                        t->child[0] = $2;
                        t->child[0]->isC0 = 1;
                        t->child[1] = $4;
                        t->child[1]->isC1 = 1;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | IF simpleExp THEN matched ELSE unmatched
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = StmtK;            //nodekind
                        t->subkind.stmt = IfK;               //StmtKind
                        t->lineno = $1->linenum;        //TokenData: linenum
                        t->child[0] = $2;
                        t->child[0]->isC0 = 1;
                        t->child[1] = $4;
                        t->child[1]->isC1 = 1;
                        t->child[2] = $6;
                        t->child[2]->isC2 = 1;
                        $$ = t;
                    }
                | unmatchediterStmt
                    { $$ = $1; }
                ;
expStmt         : exp ';'
                    { $$ = $1; }
                | ';'
                    { $$ = NULL; }
                ;
compoundStmt    : '{' localDecls stmtList '}'
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = StmtK;            //nodekind
                        t->subkind.stmt = CompoundK;         //StmtKind
                        t->lineno = $1->linenum;        //TokenData: linenum
                        if($2 == NULL && $3 == NULL)
                        {
                            t->child[0] = NULL;
                            t->child[1] = NULL;
                            t->child[2] = NULL;
                        }
                        else if($2 != NULL && $3 == NULL)
                        {
                            t->child[0] = $2;
                            t->child[0]->isC0 = 1;
                            t->child[1] = NULL;
                            t->child[2] = NULL;
                        }
                        else if ($2 == NULL && $3 != NULL)
                        {
                            t->child[0] = NULL;
                            t->child[1] = $3;
                            t->child[1]->isC1 = 1;
                            t->child[2] = NULL;
                        }
                        else{
                            t->child[0] = $2;
                            t->child[0]->isC0 = 1;
                            t->child[1] = $3;
                            t->child[1]->isC1 = 1;
                            t->child[2] = NULL;
                        }
                        $$ = t;
                    }
                ;
localDecls      : localDecls scopedVarDecl
                    {
                        TreeNode* t = $1;
                        if(t != NULL)
                        {
                            while(t->sibling != NULL)
                            { t = t->sibling; }
                            t->sibling = $2;
                            $$ = $1;
                        }
                        else{ $$ = $2; }
                    }
                | /* Epsilon */
                    { $$ = NULL; }
                ;
stmtList        : stmtList stmt
                    {
                        TreeNode* t = $1;
                        if(t != NULL)
                        {
                            while(t->sibling != NULL)
                            { t = t->sibling; }
                            t->sibling = $2;
                            $$ = $1;
                        }
                        else{ $$ = $2; }
                    }
                | /* Epsilon */
                    { $$ = NULL; }
                ;
matchediterStmt : WHILE simpleExp DO matched
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = StmtK;            //nodekind
                        t->subkind.stmt = WhileK;         //StmtKind
                        t->lineno = $1->linenum;        //TokenData: linenum
                        if($4 != NULL)
                        {
                            t->child[0] = $2;
                            t->child[0]->isC0 = 1;
                            t->child[1] = $4;
                            t->child[1]->isC1 = 1;
                            t->child[2] = NULL;
                        }
                        else
                        {
                            t->child[0] = $2;
                            t->child[0]->isC0 = 1;
                            t->child[1] = NULL;
                            t->child[2] = NULL;   
                        }
                        $$ = t;

                    }
                | FOR ID '=' iterRange DO matched
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = StmtK;            //nodekind
                        t->subkind.stmt = ForK;              //StmtKind
                        t->lineno = $1->linenum;        //TokenData: linenum
                        //printf("iS: ",$1->svalue);
                        t->attr.name = $1->tokenstr;
                        TreeNode* t2 = new TreeNode;
                        t2->nodekind = DeclK;
                        t2->isInit = true;
                        t2->subkind.decl = VarK;
                        t2->declType = DeclInteger;
                        t2->nodeType = nInt;
                        t2->lineno = $2->linenum;
                        t2->attr.name = $2->tokenstr;
                        t2->isC0 = 1;
                        t->lineno = $1->linenum;
                        if($6 != NULL)
                        {
                            t->child[0] = t2;
                            t->child[0]->isC0 = 1;
                            t->child[1] = $4;
                            t->child[1]->isC1 = 1;
                            t->child[2] = $6;
                            t->child[2]->isC2 = 1;
                        }
                        else
                        {
                            t->child[0] = t2;
                            t->child[0]->isC0 = 1;
                            t->child[1] = $4;
                            t->child[1]->isC1 = 1;
                            t->child[2] = NULL;
                        }
                        
                        $$ = t;
                    }
                ;
unmatchediterStmt : WHILE simpleExp DO unmatched
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = StmtK;            //nodekind
                        t->subkind.stmt = WhileK;         //StmtKind
                        t->lineno = $1->linenum;        //TokenData: linenum
                        if($4 != NULL)
                        {
                            t->child[0] = $2;
                            t->child[0]->isC0 = 1;
                            t->child[1] = $4;
                            t->child[1]->isC1 = 1;
                            t->child[2] = NULL;
                        }
                        else
                        {
                            t->child[0] = $2;
                            t->child[0]->isC0 = 1;
                            t->child[1] = NULL;
                            t->child[2] = NULL;   
                        }
                        $$ = t;

                    }
                | FOR ID '=' iterRange DO unmatched
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = StmtK;            //nodekind
                        t->subkind.stmt = ForK;              //StmtKind
                        t->lineno = $1->linenum;        //TokenData: linenum
                        //printf("iS: ",$1->svalue);
                        t->attr.name = $1->tokenstr;
                        TreeNode* t2 = new TreeNode;
                        t2->nodekind = DeclK;
                        t2->subkind.decl = VarK;
                        t2->declType = DeclInteger;
                        t2->nodeType = nInt;
                        t2->isInit = true;
                        t2->lineno = $2->linenum;
                        t2->attr.name = $2->tokenstr;
                        t2->isC0 = 1;
                        t->lineno = $1->linenum;
                        t->child[0] = t2;
                        t->child[0]->isC0 = 1;
                        t->child[1] = $4;
                        t->child[1]->isC1 = 1;
                        t->child[2] = $6;
                        t->child[2]->isC2 = 1;
                        $$ = t;
                    }
                    ;
iterRange       : simpleExp TO simpleExp
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = StmtK;            //nodekind
                        t->subkind.stmt = RangeK;         //StmtKind
                        t->lineno = $2->linenum;        //TokenData: linenum
                        t->child[0] = $1;
                        t->child[0]->isC0 = 1;
                        t->child[1] = $3;
                        t->child[1]->isC1 = 1;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | simpleExp TO simpleExp BY simpleExp
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = StmtK;            //nodekind
                        t->subkind.stmt = RangeK;         //StmtKind
                        t->lineno = $2->linenum;        //TokenData: linenum
                        t->child[0] = $1;
                        t->child[0]->isC0 = 1;
                        t->child[1] = $3;
                        t->child[1]->isC1 = 1;
                        t->child[2] = $5;
                        t->child[2]->isC2 = 1;
                        $$ = t;
                    }
                ;
returnStmt      : RETURN ';'
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = StmtK;            //nodekind
                        t->subkind.stmt = ReturnK;           //StmtKind
                        t->lineno = $1->linenum;        //TokenData: linenum
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | RETURN exp ';'
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = StmtK;            //nodekind
                        t->subkind.stmt = ReturnK;           //StmtKind
                        t->attr.name = $2->attr.name;
                        t->lineno = $1->linenum;        //TokenData: linenum
                        t->child[0] = $2;
                        t->child[0]->isC0 = 1;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                ;
breakStmt       : BREAK ';'
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = StmtK;            //nodekind
                        t->subkind.stmt = BreakK;         //StmtKind
                        t->lineno = $1->linenum;        //TokenData: linenum
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                ;
exp             : mutable '=' exp
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;             //nodekind
                        t->subkind.exp = AssignK;           //ExpKind   MAYBE SHOULD BE OpK??
                        t->expType = Integer;
                        t->nodeType = nInt;
                        t->lineno = $2->linenum;        //TokenData: linenum
                        t->attr.op = '=';
                        t->attr.name = "=";                       
                        t->child[0] = $1;
                        t->child[0]->isC0 = 1;
                        t->child[1] = $3;
                        t->child[1]->isC1 = 1;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | mutable ADDASS exp
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;            //nodekind
                        t->subkind.exp = AssignK;              //ExpKind
                        t->lineno = $2->linenum;       //TokenData: linenum
                        t->expType = Integer;
                        t->nodeType = nInt;
                        t->attr.op = ADDASS;           //t->attr.op = $2; ????     
                        t->attr.name = "+=";            
                        t->child[0] = $1;
                        t->child[0]->isC0 = 1;
                        t->child[1] = $3;
                        t->child[1]->isC1 = 1;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | mutable MINASS exp
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;            //nodekind
                        t->subkind.exp = AssignK;              //ExpKind
                        t->lineno = $2->linenum;        //TokenData: linenum
                        t->expType = Integer;
                        t->nodeType = nInt;
                        t->attr.op = MINASS;
                        t->attr.name = "-=";                     
                        t->child[0] = $1;
                        t->child[0]->isC0 = 1;
                        t->child[1] = $3;
                        t->child[1]->isC1 = 1;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | mutable MULASS exp
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;            //nodekind
                        t->subkind.exp = AssignK;         //ExpKind
                        t->lineno = $2->linenum;        //TokenData: linenum
                        t->expType = Integer;
                        t->nodeType = nInt;
                        t->attr.op = MULASS;
                        t->attr.name = "*=";                       
                        t->child[0] = $1;
                        t->child[0]->isC0 = 1;
                        t->child[1] = $3;
                        t->child[1]->isC1 = 1;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | mutable DIVASS exp
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;            //nodekind
                        t->subkind.exp = AssignK;              //ExpKind
                        t->lineno = $2->linenum;       //TokenData: linenum
                        t->expType = Integer;
                        t->nodeType = nInt;
                        t->attr.op = DIVASS;
                        t->attr.name = "/=";                      
                        t->child[0] = $1;
                        t->child[0]->isC0 = 1;
                        t->child[1] = $3;
                        t->child[1]->isC1 = 1;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | mutable INC
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;            //nodekind
                        t->subkind.exp = AssignK;         //ExpKind
                        t->lineno = $2->linenum;        //TokenData: linenum
                        t->expType = Integer;
                        t->nodeType = nInt;
                        t->attr.op = INC;
                        t->attr.name = "++";                      
                        t->child[0] = $1;
                        t->child[0]->isC0 = 1;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | mutable DEC
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;            //nodekind
                        t->subkind.exp = AssignK;         //ExpKind
                        t->lineno = $2->linenum;        //TokenData: linenum
                        t->expType = Integer;
                        t->nodeType = nInt;
                        t->attr.op = DEC;  
                        t->attr.name = "--";                       
                        t->child[0] = $1;
                        t->child[0]->isC0 = 1;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | simpleExp
                    { $$ = $1; }
                ;
simpleExp       : simpleExp OR andExp
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;            //nodekind
                        t->subkind.exp = OpK;         //ExpKind
                        t->lineno = $2->linenum;        //TokenData: linenum
                        t->expType =  Boolean;
                        t->nodeType = nBool;
                        t->attr.op = OR;
                        t->attr.name = "OR";                  
                        t->child[0] = $1;
                        t->child[0]->isC0 = 1;
                        t->child[1] = $3;
                        t->child[1]->isC1 = 1;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | andExp
                    { $$ = $1; }
                ;
andExp          : andExp ANDD unaryRelExp
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;            //nodekind
                        t->subkind.exp = OpK;         //ExpKind
                        t->lineno = $2->linenum;        //TokenData: linenum
                        t->expType =  Boolean;
                        t->nodeType = nBool;
                        t->attr.op = ANDD;
                        t->attr.name = "AND";                     
                        t->child[0] = $1;
                        t->child[0]->isC0 = 1;
                        t->child[1] = $3;
                        t->child[1]->isC1 = 1;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | unaryRelExp
                    { $$ = $1; }
                ;
unaryRelExp     : NOT unaryRelExp
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;            //nodekind
                        t->subkind.exp = OpK;              //ExpKind
                        t->lineno = $1->linenum;       //TokenData: linenum
                        t->expType =  Boolean;
                        t->nodeType = nBool;
                        t->attr.op = NOT;
                        t->attr.name = "NOT";                      
                        t->child[0] = $2;
                        t->child[0]->isC0 = 1;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | relExp
                    { $$ = $1; }
                ;
relExp          : minmaxExp relop minmaxExp
                    {
                        TreeNode* t = $2;    //make nodes in relop
                        t->child[0] = $1;
                        t->child[0]->isC0 = 1;
                        t->child[1] = $3;
                        t->child[1]->isC1 = 1;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | minmaxExp
                    { $$ = $1; }
                ;
relop           : LEQ
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = $1->linenum;       //TokenData: linenum
                        t->expType =  Boolean;
                        t->nodeType = nBool;
                        t->attr.op = LEQ;
                        t->attr.name = "<=";                     
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | '<'
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = $1->linenum;       //TokenData: linenum
                        t->expType =  Boolean;
                        t->nodeType = nBool;
                        t->attr.op = '<';
                        t->attr.name = "<";                    
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | '>'
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = $1->linenum;       //TokenData: linenum
                        t->expType =  Boolean;
                        t->nodeType = nBool;
                        t->attr.op = '>';
                        t->attr.name = ">";                    
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | GEQ
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = $1->linenum;       //TokenData: linenum
                        t->expType =  Boolean;
                        t->nodeType = nBool;
                        t->attr.op = GEQ;
                        t->attr.name = ">=";                     
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | EQ
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = $1->linenum;       //TokenData: linenum
                        t->expType =  Boolean;
                        t->nodeType = nBool;
                        t->attr.op = EQ;
                        t->attr.name = "==";                     
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | NEQ
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = $1->linenum;       //TokenData: linenum
                        t->expType =  Boolean;
                        t->nodeType = nBool;
                        t->attr.op = NEQ;
                        t->attr.name = "!=";                       
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                ;
minmaxExp       : minmaxExp minmaxop sumExp
                    {
                        TreeNode* t = $2;
                        t->child[0] = $1;
                        t->child[0]->isC0 = 1;
                        t->child[1] = $3;
                        t->child[1]->isC1 = 1;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | sumExp
                    { $$ = $1; }
                ;
minmaxop        : MAX
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = $1->linenum;       //TokenData: linenum
                        t->expType =  Integer;
                        t->nodeType = nInt;
                        t->attr.op = MAX;
                        t->attr.name = ":>:";                          
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | MIN
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = $1->linenum;       //TokenData: linenum
                        t->expType =  Integer;
                        t->nodeType = nInt;
                        t->attr.op = MIN;
                        t->attr.name = ":<:";                       
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                ;
sumExp          : sumExp sumop mulExp
                    {
                        TreeNode* t = $2;
                        t->child[0] = $1;
                        t->child[0]->isC0 = 1;
                        t->child[1] = $3;
                        t->child[1]->isC1 = 1;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | mulExp
                    { $$ = $1; }
                ;
sumop           : '+'
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = $1->linenum;       //TokenData: linenum
                        t->expType =  Integer;
                        t->nodeType = nInt;
                        t->attr.op = '+';
                        t->attr.name = "+";                       
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | '-'
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = $1->linenum;       //TokenData: linenum
                        t->expType =  Integer;
                        t->nodeType = nInt;
                        t->attr.op = '-';
                        t->attr.name = "-";                    
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                ;
mulExp          : mulExp mulop unaryExp
                    {
                        TreeNode* t = $2;
                        t->child[0] = $1;
                        t->child[0]->isC0 = 1;
                        t->child[1] = $3;
                        t->child[1]->isC1 = 1;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | unaryExp
                    { $$ = $1; }
                ;
mulop           : '*'
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = $1->linenum;       //TokenData: linenum
                        t->expType =  Integer;
                        t->nodeType = nInt;
                        t->attr.op = '*';
                        t->attr.name = "*";                          
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | '/'
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = $1->linenum;       //TokenData: linenum
                        t->expType =  Integer;
                        t->nodeType = nInt;
                        t->attr.op = '/';
                        t->attr.name = "/";                          
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | '%'
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = $1->linenum;       //TokenData: linenum
                        t->expType =  Integer;
                        t->nodeType = nInt;
                        t->attr.op = '%';
                        t->attr.name = "%";                          
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                ;
unaryExp        : unaryop unaryExp
                    {
                        TreeNode* t = $1;
                        t->child[0] = $2;
                        t->child[0]->isC0 = 1;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | factor
                    { $$ = $1; }
                ;
unaryop         : '-'
                    {
                        //NEGATE
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = $1->linenum;       //TokenData: linenum
                        t->expType =  Integer;
                        t->nodeType = nInt;
                        t->attr.op = '-';
                        t->attr.name = "CHSIGN";                          
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | '*'
                    {
                        //SIZEOF
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = $1->linenum;       //TokenData: linenum
                        t->expType =  Integer;
                        t->nodeType = nInt;
                        t->attr.op = '*';
                        t->attr.name = "SIZEOF";                          
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | '?'
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = $1->linenum;       //TokenData: linenum
                        t->expType =  Integer;
                        t->nodeType = nInt;
                        t->attr.op = '?';
                        t->attr.name = "?";                          
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                ;
factor          : immutable
                    { $$ = $1; }
                | mutable
                    { $$ = $1; }
                ;
mutable         : ID
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = IdK;               //ExpKind
                        //printf("m: ",$1->svalue);
                        t->attr.name = $1->tokenstr;
                        t->lineno = $1->linenum;
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | ID '[' exp ']'
                    {
                        //this one is odd
                        //need to manually add ID child and [ operator
                        //because $1 is a TokenData
                        //if theres a better way to do this i cant think of it
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->attr.name = "[";
                        //t->isArray= true; // IS THIS OKAY?????????
                        TreeNode* t2 = new TreeNode;
                        t2->nodekind = ExpK;
                        t2->subkind.exp = IdK;
                        t2->lineno = $1->linenum;
                        t2->attr.name = $1->tokenstr;
                        t2->isArray = true;
                        t2->isC0 = 1;
                        t->lineno = $1->linenum;
                        t->child[0] = t2;
                        t->child[1] = $3;
                        t->child[1]->isC1 = 1;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                ;
immutable       : '(' exp ')'
                    { $$ = $2; }
                | call
                    { $$ = $1; }
                | constant
                    { $$ = $1; }
                ;
call            : ID '(' args ')'
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = CallK;               //ExpKind
                        //printf("c: ",$1->svalue);
                        t->attr.name = $1->tokenstr;
                        t->lineno = $1->linenum;
                        if($3 != NULL){
                            t->child[0] = $3;
                            t->child[0]->isC0 = 1;
                            t->child[1] = NULL;
                            t->child[2] = NULL;
                        }
                        else{
                            t->child[0] = NULL;
                            t->child[1] = NULL;
                            t->child[2] = NULL;
                        }
                        $$ = t;
                    }
                ;
args            : argList
                    { $$ = $1; }
                | /* Epsilon */
                    { $$ = NULL; }
                ;
argList         : argList ',' exp
                    {
                        TreeNode* t = $1;
                        if(t != NULL)
                        {   
                            while(t->sibling != NULL)
                            { t = t->sibling; }
                            t->sibling = $3;
                            t->lineno = $2->linenum;
                            $$ = $1;
                        }
                        else{ $$ = $3; }
                    }
                | exp
                    { $$ = $1; }
                ;
constant        : NUMCONST
                    {
                        TreeNode* t = new TreeNode;
                        t->expType = Integer;
                        t->nodeType = nInt;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = ConstantK;               //ExpKind
                        t->attr.name = $1->tokenstr;
                        //printf("nc: %s\n", $1->tokenstr);
                        t->attr.value = $1->nvalue;
                        t->lineno = $1->linenum;
                        t->isNC = true;
                        t->isConstant = true;
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | CHARCONST
                    {
                        TreeNode* t = new TreeNode;
                        t->expType = Char;
                        t->nodeType = nChar;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = ConstantK;               //ExpKind
                        //printf("Cc: %s\n",$1->tokenstr);
                        t->isConstant = true;
                        t->attr.name = $1->tokenstr;
                        t->attr.cvalue = $1->cvalue;
                        t->lineno = $1->linenum;
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | STRINGCONST
                    {
                        TreeNode* t = new TreeNode;
                        t->expType = Char;
                        t->nodeType = nChar;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = ConstantK;               //ExpKind
                        //printf("Sc: ",$1->svalue);
                        t->isArray = 1;
                        t->isConstant = true;
                        t->attr.name = $1->tokenstr;
                        t->attr.string = $1->tokenstr;       // MAYBE TOKENSTR???
                        t->lineno = $1->linenum;
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | ttrue
                    {
                        TreeNode* t = new TreeNode;
                        t->expType = Boolean;
                        t->nodeType = nBool;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = ConstantK;               //ExpKind
                        //printf("Tc: ",$1->tokenstr);
                        //t->attr.name = $1->tokenstr;
                        t->isConstant = true;
                        t->attr.name = "true";
                        t->attr.value = 1;
                        t->lineno = $1->linenum;
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                | ffalse
                    {
                        TreeNode* t = new TreeNode;
                        t->expType = Boolean;
                        t->nodeType = nBool;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = ConstantK;               //ExpKind
                        //printf("Fc: ",$1->svalue);
                        //t->attr.name = $1->tokenstr;
                        t->isConstant = true;
                        t->attr.name = "false";
                        t->attr.value = 0;
                        t->lineno = $1->linenum;
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        $$ = t;
                    }
                ;
%%
int main(int argc, char** argv) 
{
    numErrors = 0;
    numWarnings = 0;
    int numOption = 0;
    /* ourgetopt things */
    int c;
    extern char* optarg;
    extern int optfind;
    int printTreeFlag, printExtendedTreeFlag;
    int errflag;
    char* ofile;
    printTreeFlag =  printExtendedTreeFlag = 0;
    ofile = NULL;
    while(1)
    {
        while((c = ourGetopt(argc, argv, (char*)"dDpPh")) != EOF)
        {
            switch(c)
            {
                case 'd':
                    yydebug = 1;
                    numOption++;
                    break;
                case 'D':
                    //turn on symbol table debugging
                    savedsymbolTable.debug(true);
                    numOption++;
                    break;
                case 'p':
                    printTreeFlag = 1;
                    numOption++;
                    break;
                case 'P':
                    printExtendedTreeFlag = 1;
                    numOption++;
                    break;
                case 'h':
                    printHelp();
                    numOption++;
                    break;
                default:
                    break;
            }
        }
        break;
    }
    if (argc == numOption+2)
        yyin = fopen(argv[argc-1], "r");
    else
        yyin = stdin;
    yyparse();
    if(numErrors == 0 ){
        if(printTreeFlag){
            printTree(savedTree); // set by -p 
        }
    }

    IOtree = buildIOTree();                         //Build Tree Containing I/O Functions
    semanticNoOut(IOtree, savedsymbolTable);        //Load them into our Symbol Table, Global Scope
    semanticAnalysis(savedTree, savedsymbolTable);  //Proceed with Abstract Syntax Tree analysis
    is_Used(savedsymbolTable);
    if(!isMain)
    {
        printf("ERROR(LINKER): A function named 'main' with no parameters must be defined.\n");
        numErrors++;
    }
    if(numErrors == 0){
        if(printExtendedTreeFlag){
            printExtendedTree(savedTree); // set by -P
        }
    }
    printf("Number of warnings: %d\n", numWarnings);
    printf("Number of errors: %d\n", numErrors);
    return 0;
}
