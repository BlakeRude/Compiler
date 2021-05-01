%{
// Section for any C/C++

/*
 * Blake Rude
 * CS445 Compiler Design
 * This comment is in parser.l, Flex file
 * Spring 2021
 */
 #include <stdlib.h>
 #include "scanType.h"
 #include "parser.tab.c"

 //static int line=1;
 int line=1;
 char *lastToken;
 static int setValue(int linenum, int tokenClass, char *svalue)
{/*
    if(svalue)
    {
        printf("s: %s\n", svalue);fflush(stdout);
    }*/
    // create and handle default data storage
    yylval.tokenData = new TokenData;
    yylval.tokenData->tokenclass = tokenClass;
    yylval.tokenData->linenum = linenum;
    yylval.tokenData->tokenstr = strdup(svalue);
    lastToken = svalue;
    switch (tokenClass) {
    case NUMCONST:
        yylval.tokenData->nvalue = atof(svalue); 
        break;
    case CHARCONST:
        if(svalue[1] == '\\'){
            if(svalue[2] == '0')
            { yylval.tokenData->cvalue = '\0'; }
            else if(svalue[2] == 'n')
            { yylval.tokenData->cvalue = '\n'; }
            else{ yylval.tokenData->cvalue = svalue[2]; }
        }
        else{
            yylval.tokenData->cvalue = svalue[1]; 
        }
        break;
    case BOOLCONST:
        if(svalue[0] == 't'){
            yylval.tokenData->nvalue = 1; 
        }
        else{
            yylval.tokenData->nvalue = 0; 
        }         
         break;
    case BOOL:
        yylval.tokenData->svalue = svalue;         
        break;
    case IF:
        yylval.tokenData->svalue = svalue;         
        break;
    case BY:
        yylval.tokenData->svalue = svalue;         
        break;
    case CHAR:
        yylval.tokenData->svalue = svalue;         
        break;
    case INT:
        yylval.tokenData->svalue = svalue;         
        break;
    case STATIC:
        yylval.tokenData->svalue = svalue;         
        break;
    case ANDD:
        yylval.tokenData->svalue = svalue;         
        break;
    case OR:
        yylval.tokenData->svalue = svalue;         
        break;
    case NOT:
        yylval.tokenData->svalue = svalue;         
        break;
    case FOR:
        yylval.tokenData->svalue = svalue;         
        break;
    case RETURN:
        yylval.tokenData->svalue = svalue;         
        break;
    case DO:
        yylval.tokenData->svalue = svalue;         
        break;
    case WHILE:
        yylval.tokenData->svalue = svalue;         
        break;
    case ELSE:
        yylval.tokenData->svalue = svalue;         
        break;
    case BREAK:
        yylval.tokenData->svalue = svalue;         
        break;
    case THEN:
        yylval.tokenData->svalue = svalue;         
        break;
    case ttrue:
        yylval.tokenData->svalue = svalue;         
        break;
    case ffalse:
        yylval.tokenData->svalue = svalue;         
        break;
    case TO:
        yylval.tokenData->svalue = svalue;         
        break;
    case ADDASS:
        yylval.tokenData->svalue = svalue;         
        break;
    case DEC:
        yylval.tokenData->svalue = svalue;         
        break;
    case EQ:
        yylval.tokenData->svalue = svalue;         
        break;
    case LEQ:
        yylval.tokenData->svalue = svalue;         
        break;
    case GEQ:
        yylval.tokenData->svalue = svalue;         
        break;
    case MAX:
        yylval.tokenData->svalue = svalue;         
        break;
    case MIN:
        yylval.tokenData->svalue = svalue;         
        break;
    case NEQ:
        yylval.tokenData->svalue = svalue;         
        break;
    case INC:
        yylval.tokenData->svalue = svalue;         
        break;
    case MINASS:
        yylval.tokenData->svalue = svalue;         
        break;
    case MULASS:
        yylval.tokenData->svalue = svalue;         
        break;
    case DIVASS:
        yylval.tokenData->svalue = svalue;         
        break;
    case '<':
        yylval.tokenData->svalue = svalue;         
        break;
    case '>':
        yylval.tokenData->svalue = svalue;         
        break;
    case '=':
        yylval.tokenData->svalue = svalue;         
        break;
    case '*':
        yylval.tokenData->svalue = svalue;         
        break;
    case '+':
        yylval.tokenData->svalue = svalue;         
        break;
    case '-':
        yylval.tokenData->svalue = svalue;         
        break;
    case '/':
        yylval.tokenData->svalue = svalue;         
        break;
    case '%':
        yylval.tokenData->svalue = svalue;         
        break;
    case ')':
        yylval.tokenData->svalue = svalue;         
        break;
    case '(':
        yylval.tokenData->svalue = svalue;         
        break;
    case ';':
        yylval.tokenData->svalue = svalue;      
        break;
    case ':':
        yylval.tokenData->svalue = svalue;         
        break;
    case '[':
        yylval.tokenData->svalue = svalue;         
        break;
    case ']':
        yylval.tokenData->svalue = svalue;         
        break;
    case '{':
        yylval.tokenData->svalue = svalue;         
        break;
    case '}':
        yylval.tokenData->svalue = svalue;         
        break;
    case ',':
        yylval.tokenData->svalue = svalue;         
        break;
    case '?':
        yylval.tokenData->svalue = svalue;         
        break;
    case ID:
         yylval.tokenData->svalue = svalue;         
         break;
    case STRINGCONST:
        yylval.tokenData->svalue = (char*)svalue;
        break;
    default:
        printf("ERROR(parser.l): switch statement\n");
        break;
    }
    yylval.tokenData->svalue = (char*)svalue;

    // return token class
    return tokenClass;
}
char ADDASS_[] = "ADDASS";
char BOOL_[] = "BOOL";
char CHAR_[] = "CHAR";
char INT_[] = "INT";
char STATIC_[] = "STATIC";
char AND_[] = "AND";
char OR_[] = "OR";
char NOT_[] = "NOT";
char FOR_[] = "FOR";
char RETURN_[] = "RETURN";
char DEC_[] = "DEC";
char DO_[] = "DO";
char EQ_[] = "EQ";
char LEQ_[] = "LEQ";
char GEQ_[] = "GEQ";
char WHILE_[] = "WHILE";
char MAX_[] = "MAX";
char MIN_[] = "MIN";
char ELSE_[] = "ELSE";
char BREAK_[] = "BREAK";
char THEN_[] = "THEN";
char NEQ_[] = "NEQ";
char BY_[] = "BY";
char INC_[] = "INC";
char IF_[] = "IF";
char TO_[] = "TO";
char MULASS_[] = "MULASS";
char DIVASS_[] = "DIVASS";
char MINASS_[] = "MINASS";

%}

/* FLEX OPTIONS HERE using %option */
%option noyywrap 

/* REGEX Rules Section after %% */
%%
\/\/.*\n           {line++;} // hopefully should get rid of comments
bool                 { return setValue(line, BOOL, BOOL_);}
if                   { return setValue(line, IF, IF_);}
by                   { return setValue(line, BY, BY_);}
char                 { return setValue(line, CHAR, CHAR_);}
int                  { return setValue(line, INT, yytext);}
static               { return setValue(line, STATIC, STATIC_);}
and                  { return setValue(line, ANDD, AND_);}
or                   { return setValue(line, OR, OR_);}
not                  { return setValue(line, NOT, NOT_);}
for                  { return setValue(line, FOR, FOR_);}
return               { return setValue(line, RETURN, RETURN_);}
do                   { return setValue(line, DO, DO_);}
while                { return setValue(line, WHILE, WHILE_);}
else                 { return setValue(line, ELSE, ELSE_);}
break                { return setValue(line, BREAK, BREAK_);}
then                 { return setValue(line, THEN, THEN_);}
true                 { return setValue(line, ttrue, yytext);}
false                { return setValue(line, ffalse, yytext);}
to                   { return setValue(line, TO, TO_);}
"+="                 { return setValue(line, ADDASS, ADDASS_);}
"--"                 { return setValue(line, DEC, DEC_);}
"=="                 { return setValue(line, EQ, EQ_);}
"<="                 { return setValue(line, LEQ, LEQ_);}
">="                 { return setValue(line, GEQ, GEQ_);}
":>:"                { return setValue(line, MAX, MAX_);}
":<:"                { return setValue(line, MIN, MIN_);}
"!="                 { return setValue(line, NEQ, NEQ_);}
"++"                 { return setValue(line, INC, INC_);}
"-="                 { return setValue(line, MINASS, MINASS_);}
"*="                 { return setValue(line, MULASS, MULASS_);}
"/="                 { return setValue(line, DIVASS, DIVASS_);}
"<"                  { return setValue(line, '<', yytext);}
">"                  { return setValue(line, '>', yytext);}
"="                  { return setValue(line, '=', yytext);}
"*"                  { return setValue(line, '*', yytext);}
"+"                  { return setValue(line, '+', yytext);}
"-"                  { return setValue(line, '-', yytext);}
"/"                  { return setValue(line, '/', yytext);}
"%"                  { return setValue(line, '%', yytext);}
")"                  { return setValue(line, ')', yytext);}
"("                  { return setValue(line, '(', yytext);}
";"                  { return setValue(line, ';', yytext);}
":"                  { return setValue(line, ':', yytext);}
"["                  { return setValue(line, '[', yytext);}
"]"                  { return setValue(line, ']', yytext);}
"{"                  { return setValue(line, '{', yytext);}
"}"                  { return setValue(line, '}', yytext);}
","                  { return setValue(line, ',', yytext);}
"?"                  { return setValue(line, '?', yytext);}


[a-zA-Z][a-zA-Z0-9]*  { return setValue(line, ID, yytext); }
[0-9][0-9]*           { return setValue(line, NUMCONST, yytext); }
['](\\.|[^'\\\n])*['] { if(yyleng-2 > 1){ if((strcmp(yytext, "'\\n'") == 0) ||(strcmp(yytext, "'\\0'") == 0) || (strcmp(yytext, "'\\''") == 0) ||(strcmp(yytext, "'\\t'") == 0)){} else{printf("WARNING(%d): character is %d characters long and not a single character: '%s'.  The first char will be used.\n", line, yyleng-2, yytext); numWarnings++;} return setValue(line, CHARCONST, yytext); } else if(yyleng-2 == 0){ printf("ERROR(%d): Empty character ''.  Characters ignored.\n",line); numErrors++; } else{ return setValue(line, CHARCONST, yytext); } }
["](\\.|[^"\\\n])*["] { return setValue(line, STRINGCONST, yytext); } //finding out about \\. was a godsend...
[ \t]                 ; // get rid of white space (from calc.l)
\n                    { line++; }
.                     { printf("ERROR(%d): Invalid or misplaced input character: '%s'. Character Ignored.\n", line, yytext); numErrors++; }
%%
/*
// C/C++ Code here, main can be the bison part

int main(int argc, char** argv)
{
    yylex();
    return 0;
}
*/
