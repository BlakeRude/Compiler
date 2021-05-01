#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

extern int lineno;
extern int isMain;
extern int numErrors;
extern int numWarnings;
extern int foffset;
extern int goffset;
const int MAXCHILDREN = 3;
// the exact type of the token or node involved.  These are divided into
// various "kinds" in the enums that follow

// Kinds of Operators
// these are the token numbers for the operators same as in flex
typedef int OpKind;  

// Kinds of Statements
//typedef enum {DeclK, StmtK, ExpK} NodeKind;
enum NodeKind {DeclK, StmtK, ExpK};

// Subkinds of Declarations
enum DeclKind {VarK, FuncK, ParamK};

// Subkinds of Statements
enum  StmtKind {NullK, IfK, WhileK, ForK, CompoundK, ReturnK, BreakK, RangeK};

// Subkinds of Expressions
enum ExpKind {OpK, ConstantK, IdK, AssignK, InitK, CallK};

enum NodeType{nVoid, nInt, nBool, nChar, nUndefinedType, uninit, nBad};
// ExpType is used for type checking (Void means no type or value, UndefinedType means undefined)
enum ExpType {Void, Integer, Boolean, Char, CharInt, Equal, UndefinedType};
// Feel Like id want an ExpType for Decls like varDecl, instead of using ExpType on a non ExpKind
enum DeclType {DeclVoid, DeclInteger, DeclBoolean, DeclChar, DeclCharInt, DeclEqual, DeclUndefinedType};
// What kind of scoping is used?  (decided during typing)
enum VarKind {None, Local, Global, Parameter, LocalStatic};
enum MemoryKind {loca, stat, para, glob, non};
typedef struct treeNode
{
    // connectivity in the tree
    struct treeNode *child[MAXCHILDREN];   // children of the node
    struct treeNode *sibling=NULL;              // siblings for the node

    // what kind of node
    int lineno;                            // linenum relevant to this node
    NodeKind nodekind;                     // type of this node
    union                                  // subtype of type
    {
	    DeclKind decl;                     // used when DeclK
	    StmtKind stmt;                     // used when StmtK
	    ExpKind exp;                       // used when ExpK
    } subkind;
    // extra properties about the node depending on type of the node
    union                                  // relevant data to type -> attr
    {
        OpKind op;                         // type of token (same as in bison)
	    int value;                         // used when an integer constant or boolean
        unsigned char cvalue;              // used when a character
    	char *string;                      // used when a string constant
    	char *name;                       // used when IdK
        //int size = 1; //array
    } attr;                                 
    ExpType expType;		               // used when ExpK for type checking
    DeclType declType;                     // explained above when declaring enum DeclType
    NodeType nodeType = uninit;
    bool isArray;                          // is this an array
    bool isStatic;                         // is staticly allocated?
    bool isChild;
    bool isUsed=false;
    bool funFirst;
    bool isC0=0;
    bool isC1=0;
    bool isC2=0;
    bool firstCompound = false;
    bool isInit=false;
    bool isNC = false;
    bool isConstant = false;
    bool isDeclared = true;
    bool prevDecl = false;
    bool isFun = false;
    bool isStringConstant = false;
    MemoryKind mem = non;
    int size = 1;
    int location = 0; //is 0 right? n>=0

    // even more semantic stuff will go here in later assignments.
} TreeNode;
TreeNode *addSibling(TreeNode *t, TreeNode *s);
void setType(TreeNode *t, ExpType type, bool isStatic);
#endif