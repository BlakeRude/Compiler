#ifndef _SEMANTIC_H_
#define _SEMANTIC_H_
#include "symbolTable.h"
void semanticAnalysis(TreeNode*, SymbolTable);
void semanticNoOut(TreeNode*, SymbolTable);
void actionUsed(std::string, void*);
void is_Used(SymbolTable);
#endif