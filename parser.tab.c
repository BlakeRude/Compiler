/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

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

#line 104 "parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ID = 258,
    NUMCONST = 259,
    CHARCONST = 260,
    STRINGCONST = 261,
    KEYWORD = 262,
    INVALID = 263,
    BOOLCONST = 264,
    STATIC = 265,
    INT = 266,
    BOOL = 267,
    CHAR = 268,
    IF = 269,
    NOT = 270,
    THEN = 271,
    ELSE = 272,
    WHILE = 273,
    OR = 274,
    DO = 275,
    FOR = 276,
    TO = 277,
    BY = 278,
    RETURN = 279,
    BREAK = 280,
    ADDASS = 281,
    MINASS = 282,
    DIVASS = 283,
    MULASS = 284,
    INC = 285,
    DEC = 286,
    LEQ = 287,
    GEQ = 288,
    NEQ = 289,
    EQ = 290,
    MAX = 291,
    MIN = 292,
    ttrue = 293,
    ffalse = 294,
    ANDD = 295
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 34 "parser.y"

    ExpType etype;
    DeclType dtype;
    TokenData* tokenData;
    TreeNode* tree;

#line 204 "parser.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  12
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   266

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  59
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  114
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  178

#define YYUNDEFTOK  2
#define YYMAXUTOK   295


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    48,     2,     2,
      50,    49,    44,    45,    57,    46,     2,    47,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    52,    51,
      41,    43,    42,    58,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    53,     2,    54,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    55,     2,    56,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    44,    44,    49,    63,    66,    68,    71,    96,   120,
     142,   155,   158,   164,   181,   195,   211,   224,   237,   251,
     290,   346,   349,   351,   365,   368,   391,   406,   409,   422,
     437,   439,   442,   481,   483,   485,   487,   489,   492,   513,
     526,   540,   543,   545,   548,   584,   597,   599,   612,   614,
     638,   677,   701,   728,   741,   756,   767,   781,   793,   810,
     827,   844,   861,   878,   894,   910,   913,   930,   933,   950,
     953,   969,   972,   982,   985,  1000,  1015,  1030,  1045,  1060,
    1076,  1086,  1089,  1104,  1120,  1130,  1133,  1148,  1164,  1174,
    1177,  1192,  1207,  1223,  1232,  1235,  1251,  1267,  1283,  1285,
    1288,  1301,  1327,  1329,  1331,  1334,  1356,  1359,  1361,  1374,
    1377,  1395,  1412,  1430,  1448
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ID", "NUMCONST", "CHARCONST",
  "STRINGCONST", "KEYWORD", "INVALID", "BOOLCONST", "STATIC", "INT",
  "BOOL", "CHAR", "IF", "NOT", "THEN", "ELSE", "WHILE", "OR", "DO", "FOR",
  "TO", "BY", "RETURN", "BREAK", "ADDASS", "MINASS", "DIVASS", "MULASS",
  "INC", "DEC", "LEQ", "GEQ", "NEQ", "EQ", "MAX", "MIN", "ttrue", "ffalse",
  "ANDD", "'<'", "'>'", "'='", "'*'", "'+'", "'-'", "'/'", "'%'", "')'",
  "'('", "';'", "':'", "'['", "']'", "'{'", "'}'", "','", "'?'", "$accept",
  "program", "declList", "decl", "varDecl", "scopedVarDecl", "varDeclList",
  "varDeclInit", "varDeclId", "typeSpec", "funDecl", "parms", "parmList",
  "parmTypeList", "parmIdList", "parmId", "stmt", "matched", "unmatched",
  "expStmt", "compoundStmt", "localDecls", "stmtList", "matchediterStmt",
  "unmatchediterStmt", "iterRange", "returnStmt", "breakStmt", "exp",
  "simpleExp", "andExp", "unaryRelExp", "relExp", "relop", "minmaxExp",
  "minmaxop", "sumExp", "sumop", "mulExp", "mulop", "unaryExp", "unaryop",
  "factor", "mutable", "immutable", "call", "args", "argList", "constant", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,    60,    62,    61,    42,    43,    45,    47,    37,    41,
      40,    59,    58,    91,    93,   123,   125,    44,    63
};
# endif

#define YYPACT_NINF (-134)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     116,   -45,  -134,  -134,  -134,    44,   116,  -134,  -134,    51,
    -134,    90,  -134,  -134,   -20,   -39,  -134,    15,    58,    -3,
      29,  -134,    90,    72,  -134,    88,   195,    46,    49,  -134,
     133,    90,    59,    53,    60,  -134,     2,  -134,  -134,  -134,
     195,  -134,  -134,  -134,  -134,   195,  -134,    92,    75,  -134,
    -134,    89,    25,     1,  -134,   208,  -134,  -134,  -134,  -134,
    -134,    64,    58,   195,   195,   129,   158,    82,  -134,  -134,
    -134,  -134,  -134,  -134,  -134,  -134,  -134,  -134,  -134,    83,
      92,    -7,  -134,   133,  -134,   195,   195,  -134,    86,   195,
     195,  -134,  -134,  -134,  -134,  -134,  -134,  -134,  -134,   208,
     208,  -134,  -134,   208,  -134,  -134,  -134,   208,  -134,  -134,
    -134,     9,    63,    97,  -134,    91,  -134,    84,  -134,   195,
     195,   195,   195,  -134,  -134,   195,  -134,  -134,    95,    96,
     101,  -134,    75,  -134,    52,    25,     1,  -134,   133,   133,
     195,  -134,    90,  -134,    88,    54,  -134,  -134,  -134,  -134,
    -134,  -134,   195,  -134,   132,  -134,  -134,  -134,   126,    43,
      88,   -25,  -134,  -134,  -134,   133,   133,   195,   -10,  -134,
    -134,  -134,  -134,  -134,    -6,  -134,   195,    92
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,    16,    17,    18,     0,     2,     4,     5,     0,
       6,    22,     1,     3,    14,     0,    11,    12,     0,     0,
      21,    24,    22,     0,     7,     0,     0,    28,    25,    27,
       0,     0,     0,     0,    14,    10,   100,   110,   111,   112,
       0,   113,   114,    96,    95,     0,    97,    13,    67,    69,
      71,    73,    81,    85,    89,     0,    94,    99,    98,   103,
     104,     0,     0,     0,     0,     0,     0,     0,    43,    46,
      20,    30,    31,    33,    34,    35,    41,    36,    37,     0,
      65,    99,    23,     0,    15,   107,     0,    70,     0,     0,
       0,    74,    77,    79,    78,    82,    83,    75,    76,     0,
       0,    86,    87,     0,    90,    91,    92,     0,    93,    29,
      26,     0,     0,     0,    55,     0,    57,    48,    42,     0,
       0,     0,     0,    63,    64,     0,    19,   109,     0,   106,
       0,   102,    66,    68,    72,    80,    84,    88,     0,     0,
       0,    56,     0,    45,     0,     0,    59,    60,    62,    61,
      58,   105,     0,   101,    38,    39,    49,    51,     0,     0,
       0,     0,    44,    47,   108,     0,     0,     0,     0,     9,
      32,    40,    50,    52,    53,     8,     0,    54
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -134,  -134,  -134,   150,  -134,  -134,  -133,   134,  -134,     3,
    -134,   138,  -134,   135,  -134,   103,   -79,  -131,  -123,  -134,
    -134,  -134,  -134,  -134,  -134,  -134,  -134,  -134,   -35,   -24,
      78,   -34,  -134,  -134,    69,  -134,    70,  -134,    66,  -134,
     -54,  -134,  -134,   -26,  -134,  -134,  -134,  -134,  -134
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,     6,     7,     8,   143,    15,    16,    17,    18,
      10,    19,    20,    21,    28,    29,    70,    71,    72,    73,
      74,   117,   145,    75,    76,   158,    77,    78,    79,    80,
      48,    49,    50,    99,    51,   100,    52,   103,    53,   107,
      54,    55,    56,    81,    58,    59,   128,   129,    60
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      57,   108,    47,     9,   126,    11,    87,   154,   156,     9,
      88,   161,    24,    89,    57,   155,   157,   176,    25,   119,
     120,   121,   122,   123,   124,   138,   169,   168,    89,    57,
      22,   115,    25,    23,   170,   172,   125,    57,    57,   111,
     112,   175,   171,   173,    12,   104,    30,    25,   105,   106,
     127,   130,    85,   137,    14,    86,   133,    36,    37,    38,
      39,    27,    89,    57,    57,   167,   163,    26,    63,    40,
     101,   102,    64,    57,    57,    65,    33,    57,    66,    67,
      31,    57,    89,   139,   146,   147,   148,   149,    95,    96,
     150,    34,    41,    42,   142,     2,     3,     4,    43,    61,
      44,     2,     3,     4,    45,    68,    62,    84,    83,    69,
     162,    89,    46,    23,    57,    90,   159,   164,   109,     1,
     144,    91,    92,    93,    94,    95,    96,     2,     3,     4,
      97,    98,   113,   116,   118,   131,    36,    37,    38,    39,
     140,    57,   141,   174,   151,   160,   166,    63,    40,   165,
      57,    64,   177,   152,    65,   153,    13,    66,    67,    35,
      32,    36,    37,    38,    39,   110,    82,   132,   134,   136,
     135,    41,    42,    40,     0,     0,     0,    43,     0,    44,
       0,     0,     0,    45,    68,     0,     0,     0,    69,     0,
       0,    46,     0,     0,     0,     0,    41,    42,    36,    37,
      38,    39,    43,     0,    44,     0,     0,     0,    45,   114,
      40,    36,    37,    38,    39,     0,    46,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    41,    42,     0,     0,     0,     0,    43,
       0,    44,     0,     0,     0,    45,    41,    42,     0,     0,
       0,     0,    43,    46,    44,     0,     0,     0,    45,     0,
       0,     0,     0,     0,     0,     0,    46
};

static const yytype_int16 yycheck[] =
{
      26,    55,    26,     0,    83,    50,    40,   138,   139,     6,
      45,   144,    51,    19,    40,   138,   139,    23,    57,    26,
      27,    28,    29,    30,    31,    16,    51,   160,    19,    55,
      50,    66,    57,    53,   165,   166,    43,    63,    64,    63,
      64,    51,   165,   166,     0,    44,    49,    57,    47,    48,
      85,    86,    50,   107,     3,    53,    90,     3,     4,     5,
       6,     3,    19,    89,    90,    22,   145,    52,    14,    15,
      45,    46,    18,    99,   100,    21,     4,   103,    24,    25,
      51,   107,    19,    20,   119,   120,   121,   122,    36,    37,
     125,     3,    38,    39,    10,    11,    12,    13,    44,    53,
      46,    11,    12,    13,    50,    51,    57,    54,    49,    55,
      56,    19,    58,    53,   140,    40,   140,   152,    54,     3,
     117,    32,    33,    34,    35,    36,    37,    11,    12,    13,
      41,    42,     3,    51,    51,    49,     3,     4,     5,     6,
      43,   167,    51,   167,    49,   142,    20,    14,    15,    17,
     176,    18,   176,    57,    21,    54,     6,    24,    25,    25,
      22,     3,     4,     5,     6,    62,    31,    89,    99,   103,
     100,    38,    39,    15,    -1,    -1,    -1,    44,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    -1,    55,    -1,
      -1,    58,    -1,    -1,    -1,    -1,    38,    39,     3,     4,
       5,     6,    44,    -1,    46,    -1,    -1,    -1,    50,    51,
      15,     3,     4,     5,     6,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,
      -1,    46,    -1,    -1,    -1,    50,    38,    39,    -1,    -1,
      -1,    -1,    44,    58,    46,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    58
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    11,    12,    13,    60,    61,    62,    63,    68,
      69,    50,     0,    62,     3,    65,    66,    67,    68,    70,
      71,    72,    50,    53,    51,    57,    52,     3,    73,    74,
      49,    51,    70,     4,     3,    66,     3,     4,     5,     6,
      15,    38,    39,    44,    46,    50,    58,    88,    89,    90,
      91,    93,    95,    97,    99,   100,   101,   102,   103,   104,
     107,    53,    57,    14,    18,    21,    24,    25,    51,    55,
      75,    76,    77,    78,    79,    82,    83,    85,    86,    87,
      88,   102,    72,    49,    54,    50,    53,    90,    87,    19,
      40,    32,    33,    34,    35,    36,    37,    41,    42,    92,
      94,    45,    46,    96,    44,    47,    48,    98,    99,    54,
      74,    88,    88,     3,    51,    87,    51,    80,    51,    26,
      27,    28,    29,    30,    31,    43,    75,    87,   105,   106,
      87,    49,    89,    90,    93,    95,    97,    99,    16,    20,
      43,    51,    10,    64,    68,    81,    87,    87,    87,    87,
      87,    49,    57,    54,    76,    77,    76,    77,    84,    88,
      68,    65,    56,    75,    87,    17,    20,    22,    65,    51,
      76,    77,    76,    77,    88,    51,    23,    88
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    59,    60,    61,    61,    62,    62,    63,    64,    64,
      65,    65,    66,    66,    67,    67,    68,    68,    68,    69,
      69,    70,    70,    71,    71,    72,    73,    73,    74,    74,
      75,    75,    76,    76,    76,    76,    76,    76,    77,    77,
      77,    77,    78,    78,    79,    80,    80,    81,    81,    82,
      82,    83,    83,    84,    84,    85,    85,    86,    87,    87,
      87,    87,    87,    87,    87,    87,    88,    88,    89,    89,
      90,    90,    91,    91,    92,    92,    92,    92,    92,    92,
      93,    93,    94,    94,    95,    95,    96,    96,    97,    97,
      98,    98,    98,    99,    99,   100,   100,   100,   101,   101,
     102,   102,   103,   103,   103,   104,   105,   105,   106,   106,
     107,   107,   107,   107,   107
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     3,     4,     3,
       3,     1,     1,     3,     1,     4,     1,     1,     1,     6,
       5,     1,     0,     3,     1,     2,     3,     1,     1,     3,
       1,     1,     6,     1,     1,     1,     1,     1,     4,     4,
       6,     1,     2,     1,     4,     2,     0,     2,     0,     4,
       6,     4,     6,     3,     5,     2,     3,     2,     3,     3,
       3,     3,     3,     2,     2,     1,     3,     1,     3,     1,
       2,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     1,     1,     3,     1,     1,     1,     3,     1,
       1,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     4,     3,     1,     1,     4,     1,     0,     3,     1,
       1,     1,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 45 "parser.y"
                    {
                        savedTree = (yyvsp[0].tree);
                    }
#line 1534 "parser.tab.c"
    break;

  case 3:
#line 50 "parser.y"
                    {
                        //from book appendix
                        TreeNode* t = (yyvsp[-1].tree);
                        if (t != NULL)
                        {
                            while (t->sibling != NULL)
                            { t = t->sibling; }
                            t->sibling = (yyvsp[0].tree);
                            (yyval.tree) = (yyvsp[-1].tree);
                        }
                        else
                        { (yyval.tree) = (yyvsp[0].tree); }
                    }
#line 1552 "parser.tab.c"
    break;

  case 4:
#line 64 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 1558 "parser.tab.c"
    break;

  case 5:
#line 67 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 1564 "parser.tab.c"
    break;

  case 6:
#line 69 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 1570 "parser.tab.c"
    break;

  case 7:
#line 72 "parser.y"
                    {
                        //i tried dealing with typespec multiple ways
                        //i think its best to just have it be a node with
                        //relevent information
                        
                        TreeNode* t1 = (yyvsp[-2].tree);      //typeSpec, has relevant typing info
                        TreeNode* t2 = (yyvsp[-1].tree);      //Our list of varDecl, go and set t1's type info to all siblings
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
                        (yyval.tree) = (yyvsp[-1].tree);
                    }
#line 1598 "parser.tab.c"
    break;

  case 8:
#line 97 "parser.y"
                    {
                        //Same as varDecl, but set isStatic
                        TreeNode* t1 = (yyvsp[-2].tree);      //typeSpec, has relevant typing info
                        TreeNode* t2 = (yyvsp[-1].tree);      //Our list of varDecl, go and set t1's type info to all siblings

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
                        (yyval.tree) = (yyvsp[-1].tree);
                    }
#line 1626 "parser.tab.c"
    break;

  case 9:
#line 121 "parser.y"
                    {
                        TreeNode* t1 = (yyvsp[-2].tree);      //typeSpec, has relevant typing info
                        TreeNode* t2 = (yyvsp[-1].tree);      //Our list of varDecl, go and set t1's type info to all siblings
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
                        (yyval.tree) = (yyvsp[-1].tree);
                    }
#line 1651 "parser.tab.c"
    break;

  case 10:
#line 143 "parser.y"
                    {
                        TreeNode* t = (yyvsp[-2].tree);
                        if(t != NULL)
                        {
                            while(t->sibling != NULL)
                            { t = t->sibling;}
                            t->sibling = (yyvsp[0].tree);
                            (yyval.tree) = (yyvsp[-2].tree);
                        }
                        else
                        { (yyval.tree) = (yyvsp[0].tree); }
                    }
#line 1668 "parser.tab.c"
    break;

  case 11:
#line 156 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 1674 "parser.tab.c"
    break;

  case 12:
#line 159 "parser.y"
                    {
                        //printf("testvardeclinit: %s\n", $1->attr.name);
                        //printf("testvardeclinit: %i\n", $1->lineno);
                        (yyval.tree) = (yyvsp[0].tree);                //not sure if better to do this or $$ = t, not experienced enough with pointers
                    }
#line 1684 "parser.tab.c"
    break;

  case 13:
#line 165 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = DeclK;
                        t->subkind.decl = VarK;
                        //printf("vDI: ",$1->attr.name);
                        t->attr.name = (yyvsp[-2].tree)->attr.name;
                        t->lineno = (yyvsp[-2].tree)->lineno;
                        t->isArray = (yyvsp[-2].tree)->isArray;
                        t->child[0] = (yyvsp[0].tree);
                        t->child[0]->isC0 = 1;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        t->lineno = (yyvsp[-1].tokenData)->linenum;    //get linenums from tokenData blocks 
                        (yyval.tree) = t;
                    }
#line 1704 "parser.tab.c"
    break;

  case 14:
#line 182 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        //t->nodekind = DeclK;
                        //t->subkind.decl = VarK;
                        //printf("test: %s\n", $1->tokenstr);
                        t->attr.name = (yyvsp[0].tokenData)->tokenstr;
                        t->lineno = (yyvsp[0].tokenData)->linenum;
                        
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 1722 "parser.tab.c"
    break;

  case 15:
#line 196 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        //t->nodekind = DeclK;
                        //t->subkind.decl = VarK;
                        t->isArray = true;
                        //printf("%s\n",$1->tokenstr);
                        t->attr.size = (yyvsp[-1].tokenData)->nvalue;
                        t->attr.name = (yyvsp[-3].tokenData)->tokenstr;
                        t->lineno = (yyvsp[-3].tokenData)->linenum;
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 1741 "parser.tab.c"
    break;

  case 16:
#line 212 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->declType = DeclInteger;      //from enum DeclType
                        t->nodeType = nInt;
                        t->lineno = (yyvsp[0].tokenData)->linenum;    //(tokenData)
                        //printf("tS: ",$1->svalue);
                        //t->attr.name = $1->svalue;
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 1758 "parser.tab.c"
    break;

  case 17:
#line 225 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->declType = DeclBoolean;      //from enum DeclType
                        t->nodeType = nBool;
                        t->lineno = (yyvsp[0].tokenData)->linenum;    //(tokenData)
                        //printf("tS: ",$1->svalue);
                        //t->attr.name = $1->tokenstr; 
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 1775 "parser.tab.c"
    break;

  case 18:
#line 238 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->declType = DeclChar;         //from enum DeclType
                        t->nodeType = nChar;
                        t->lineno = (yyvsp[0].tokenData)->linenum;    //(tokenData)
                        //printf("tS: ",$1->svalue);
                        //t->attr.name = $1->tokenstr;
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 1792 "parser.tab.c"
    break;

  case 19:
#line 252 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = DeclK;
                        t->subkind.decl = FuncK;
                        t->attr.name = (yyvsp[-4].tokenData)->tokenstr;
                        t->lineno = (yyvsp[-4].tokenData)->linenum;
                        t->declType = (yyvsp[-5].tree)->declType;
                        t->nodeType = (yyvsp[-5].tree)->nodeType;
                        if((yyvsp[-2].tree) == NULL && (yyvsp[0].tree) == NULL)
                        {
                            t->child[0] = NULL;
                            t->child[1] = NULL;
                            t->child[2] = NULL;
                        }
                        else if((yyvsp[-2].tree) != NULL && (yyvsp[0].tree) == NULL)
                        {
                            t->child[0] = (yyvsp[-2].tree);
                            t->child[0]->isC0 = 1;
                            t->child[1] = NULL;
                            t->child[2] = NULL;
                        }
                        else if((yyvsp[-2].tree) == NULL && (yyvsp[0].tree) != NULL)
                        {
                            t->child[0] = NULL;
                            t->child[1] = (yyvsp[0].tree);
                            t->child[1]->isC1 = 1;
                            t->child[2] = NULL;
                        }
                        else
                        {
                            t->child[0] = (yyvsp[-2].tree);
                            t->child[0]->isC0 = 1;
                            t->child[1] = (yyvsp[0].tree);
                            t->child[1]->isC1 = 1;
                            t->child[2] = NULL;
                        }
                        (yyval.tree) = t;
                    }
#line 1835 "parser.tab.c"
    break;

  case 20:
#line 291 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = DeclK;
                        t->subkind.decl = FuncK; 
                        //printf("fD2: ",$2->svalue);
                        t->attr.name = (yyvsp[-4].tokenData)->tokenstr; 
                        t->lineno = (yyvsp[-3].tokenData)->linenum;
                        t->declType = DeclVoid;
                        t->nodeType = nVoid;
                        if((yyvsp[-2].tree) == NULL && (yyvsp[0].tree) == NULL)
                        {
                            t->child[0] = NULL;
                            t->child[1] = NULL;
                            t->child[2] = NULL;
                        }
                        else if((yyvsp[-2].tree) != NULL && (yyvsp[0].tree) == NULL)
                        {
                            t->child[0] = (yyvsp[-2].tree);
                            t->child[0]->isC0 = 1;
                            t->child[1] = NULL;
                            t->child[2] = NULL;
                        }
                        else if((yyvsp[-2].tree) == NULL && (yyvsp[0].tree) != NULL)
                        {
                            t->child[0] = NULL;
                            t->child[1] = (yyvsp[0].tree);
                            t->child[1]->isC1 = 1;
                            t->child[2] = NULL;
                        }
                        else
                        {
                            t->child[0] = (yyvsp[-2].tree);
                            t->child[0]->isC0 = 1;
                            t->child[1] = (yyvsp[0].tree);
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
                        (yyval.tree) = t;
                    }
#line 1894 "parser.tab.c"
    break;

  case 21:
#line 347 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 1900 "parser.tab.c"
    break;

  case 22:
#line 349 "parser.y"
                    { (yyval.tree) = NULL;}
#line 1906 "parser.tab.c"
    break;

  case 23:
#line 352 "parser.y"
                    {
                        TreeNode* t = (yyvsp[-2].tree);
                        if(t != NULL)
                        {
                            while(t->sibling != NULL)
                            { t = t->sibling; }
                            t->lineno = (yyvsp[-1].tokenData)->linenum; //steal linenum from ';'
                            t->sibling = (yyvsp[0].tree);
                            (yyval.tree) = (yyvsp[-2].tree);
                        }
                        else
                        { (yyval.tree) = (yyvsp[0].tree); }
                    }
#line 1924 "parser.tab.c"
    break;

  case 24:
#line 366 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 1930 "parser.tab.c"
    break;

  case 25:
#line 369 "parser.y"
                    {
                        TreeNode* t = (yyvsp[0].tree);
                        if(t != NULL)
                        {   
                            t->nodekind = DeclK;
                            t->declType = (yyvsp[-1].tree)->declType;
                            t->nodeType = (yyvsp[-1].tree)->nodeType;
                            t->subkind.decl = ParamK;
                            while(t->sibling != NULL)
                            {
                                t = t->sibling;
                                t->nodekind = DeclK;
                                t->declType = (yyvsp[-1].tree)->declType;
                                t->nodeType = (yyvsp[-1].tree)->nodeType;
                                t->subkind.decl = ParamK;
                            }
                            (yyval.tree) = (yyvsp[0].tree);
                        }
                        else
                        { (yyval.tree) = (yyvsp[0].tree); }
                    }
#line 1956 "parser.tab.c"
    break;

  case 26:
#line 392 "parser.y"
                    {
                        //Copied and modified from declList above
                        TreeNode* t = (yyvsp[-2].tree);
                        if(t != NULL)
                        {
                            while(t->sibling != NULL)
                            { t = t->sibling; }
                            t->lineno = (yyvsp[-1].tokenData)->linenum; //steal linenum from ','
                            t->sibling = (yyvsp[0].tree);
                            (yyval.tree) = (yyvsp[-2].tree);
                        }
                        else
                        { (yyval.tree) = (yyvsp[0].tree); }
                    }
#line 1975 "parser.tab.c"
    break;

  case 27:
#line 407 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 1981 "parser.tab.c"
    break;

  case 28:
#line 410 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        //printf("pId: ",$1->svalue);
                        t->attr.name = (yyvsp[0].tokenData)->tokenstr;
                        t->lineno = (yyvsp[0].tokenData)->linenum;
                        t->subkind.exp = IdK;
                        t->nodekind = ExpK;
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 1998 "parser.tab.c"
    break;

  case 29:
#line 423 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        //printf("pId2: ",$1->svalue);
                        t->attr.name = (yyvsp[-2].tokenData)->tokenstr;
                        t->lineno = (yyvsp[-2].tokenData)->linenum;
                        t->subkind.exp = IdK;
                        t->nodekind = ExpK;
                        t->isArray = true;
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2016 "parser.tab.c"
    break;

  case 30:
#line 438 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 2022 "parser.tab.c"
    break;

  case 31:
#line 440 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 2028 "parser.tab.c"
    break;

  case 32:
#line 443 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = StmtK;            //nodekind
                        t->subkind.stmt = IfK;               //StmtKind
                        t->lineno = (yyvsp[-5].tokenData)->linenum;        //TokenData: linenum
                        if((yyvsp[-2].tree) == NULL && (yyvsp[0].tree) == NULL)
                        {
                            t->child[0] = (yyvsp[-4].tree);
                            t->child[0]->isC0 = 1;
                            t->child[1] = NULL;
                            t->child[2] = NULL;
                        }
                        else if((yyvsp[-2].tree) != NULL && (yyvsp[0].tree) == NULL)
                        {
                            t->child[0] = (yyvsp[-4].tree);
                            t->child[0]->isC0 = 1;
                            t->child[1] = (yyvsp[-2].tree);
                            t->child[1]->isC1 = 1;
                            t->child[2] = NULL;
                        }
                        else if((yyvsp[-2].tree) == NULL && (yyvsp[0].tree) != NULL)
                        {
                            t->child[0] = (yyvsp[-4].tree);
                            t->child[0]->isC0 = 1;
                            t->child[1] = NULL;
                            t->child[2] = (yyvsp[0].tree);
                            t->child[2]->isC2 = 1;
                        }
                        else{
                            t->child[0] = (yyvsp[-4].tree);
                            t->child[0]->isC0 = 1;
                            t->child[1] = (yyvsp[-2].tree);
                            t->child[1]->isC1 = 1;
                            t->child[2] = (yyvsp[0].tree);
                         t->child[2]->isC2 = 1;
                        }
                        (yyval.tree) = t;
                    }
#line 2071 "parser.tab.c"
    break;

  case 33:
#line 482 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 2077 "parser.tab.c"
    break;

  case 34:
#line 484 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 2083 "parser.tab.c"
    break;

  case 35:
#line 486 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 2089 "parser.tab.c"
    break;

  case 36:
#line 488 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 2095 "parser.tab.c"
    break;

  case 37:
#line 490 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 2101 "parser.tab.c"
    break;

  case 38:
#line 493 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = StmtK;            //nodekind
                        t->subkind.stmt = IfK;               //StmtKind
                        t->lineno = (yyvsp[-3].tokenData)->linenum;        //TokenData: linenum
                        if((yyvsp[0].tree) != NULL){
                            t->child[0] = (yyvsp[-2].tree);
                            t->child[0]->isC0 = 1;
                            t->child[1] = (yyvsp[0].tree);
                            t->child[1]->isC1 = 1;
                            t->child[2] = NULL;
                        }
                        else{
                            t->child[0] = (yyvsp[-2].tree);
                            t->child[0]->isC0 = 1;
                            t->child[1] = NULL;
                            t->child[2] = NULL;
                        }
                        (yyval.tree) = t;
                    }
#line 2126 "parser.tab.c"
    break;

  case 39:
#line 514 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = StmtK;            //nodekind
                        t->subkind.stmt = IfK;               //StmtKind
                        t->lineno = (yyvsp[-3].tokenData)->linenum;        //TokenData: linenum
                        t->child[0] = (yyvsp[-2].tree);
                        t->child[0]->isC0 = 1;
                        t->child[1] = (yyvsp[0].tree);
                        t->child[1]->isC1 = 1;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2143 "parser.tab.c"
    break;

  case 40:
#line 527 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = StmtK;            //nodekind
                        t->subkind.stmt = IfK;               //StmtKind
                        t->lineno = (yyvsp[-5].tokenData)->linenum;        //TokenData: linenum
                        t->child[0] = (yyvsp[-4].tree);
                        t->child[0]->isC0 = 1;
                        t->child[1] = (yyvsp[-2].tree);
                        t->child[1]->isC1 = 1;
                        t->child[2] = (yyvsp[0].tree);
                        t->child[2]->isC2 = 1;
                        (yyval.tree) = t;
                    }
#line 2161 "parser.tab.c"
    break;

  case 41:
#line 541 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 2167 "parser.tab.c"
    break;

  case 42:
#line 544 "parser.y"
                    { (yyval.tree) = (yyvsp[-1].tree); }
#line 2173 "parser.tab.c"
    break;

  case 43:
#line 546 "parser.y"
                    { (yyval.tree) = NULL; }
#line 2179 "parser.tab.c"
    break;

  case 44:
#line 549 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = StmtK;            //nodekind
                        t->subkind.stmt = CompoundK;         //StmtKind
                        t->lineno = (yyvsp[-3].tokenData)->linenum;        //TokenData: linenum
                        if((yyvsp[-2].tree) == NULL && (yyvsp[-1].tree) == NULL)
                        {
                            t->child[0] = NULL;
                            t->child[1] = NULL;
                            t->child[2] = NULL;
                        }
                        else if((yyvsp[-2].tree) != NULL && (yyvsp[-1].tree) == NULL)
                        {
                            t->child[0] = (yyvsp[-2].tree);
                            t->child[0]->isC0 = 1;
                            t->child[1] = NULL;
                            t->child[2] = NULL;
                        }
                        else if ((yyvsp[-2].tree) == NULL && (yyvsp[-1].tree) != NULL)
                        {
                            t->child[0] = NULL;
                            t->child[1] = (yyvsp[-1].tree);
                            t->child[1]->isC1 = 1;
                            t->child[2] = NULL;
                        }
                        else{
                            t->child[0] = (yyvsp[-2].tree);
                            t->child[0]->isC0 = 1;
                            t->child[1] = (yyvsp[-1].tree);
                            t->child[1]->isC1 = 1;
                            t->child[2] = NULL;
                        }
                        (yyval.tree) = t;
                    }
#line 2218 "parser.tab.c"
    break;

  case 45:
#line 585 "parser.y"
                    {
                        TreeNode* t = (yyvsp[-1].tree);
                        if(t != NULL)
                        {
                            while(t->sibling != NULL)
                            { t = t->sibling; }
                            t->sibling = (yyvsp[0].tree);
                            (yyval.tree) = (yyvsp[-1].tree);
                        }
                        else{ (yyval.tree) = (yyvsp[0].tree); }
                    }
#line 2234 "parser.tab.c"
    break;

  case 46:
#line 597 "parser.y"
                    { (yyval.tree) = NULL; }
#line 2240 "parser.tab.c"
    break;

  case 47:
#line 600 "parser.y"
                    {
                        TreeNode* t = (yyvsp[-1].tree);
                        if(t != NULL)
                        {
                            while(t->sibling != NULL)
                            { t = t->sibling; }
                            t->sibling = (yyvsp[0].tree);
                            (yyval.tree) = (yyvsp[-1].tree);
                        }
                        else{ (yyval.tree) = (yyvsp[0].tree); }
                    }
#line 2256 "parser.tab.c"
    break;

  case 48:
#line 612 "parser.y"
                    { (yyval.tree) = NULL; }
#line 2262 "parser.tab.c"
    break;

  case 49:
#line 615 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = StmtK;            //nodekind
                        t->subkind.stmt = WhileK;         //StmtKind
                        t->lineno = (yyvsp[-3].tokenData)->linenum;        //TokenData: linenum
                        if((yyvsp[0].tree) != NULL)
                        {
                            t->child[0] = (yyvsp[-2].tree);
                            t->child[0]->isC0 = 1;
                            t->child[1] = (yyvsp[0].tree);
                            t->child[1]->isC1 = 1;
                            t->child[2] = NULL;
                        }
                        else
                        {
                            t->child[0] = (yyvsp[-2].tree);
                            t->child[0]->isC0 = 1;
                            t->child[1] = NULL;
                            t->child[2] = NULL;   
                        }
                        (yyval.tree) = t;

                    }
#line 2290 "parser.tab.c"
    break;

  case 50:
#line 639 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = StmtK;            //nodekind
                        t->subkind.stmt = ForK;              //StmtKind
                        t->lineno = (yyvsp[-5].tokenData)->linenum;        //TokenData: linenum
                        //printf("iS: ",$1->svalue);
                        t->attr.name = (yyvsp[-5].tokenData)->tokenstr;
                        TreeNode* t2 = new TreeNode;
                        t2->nodekind = DeclK;
                        t2->isInit = true;
                        t2->subkind.decl = VarK;
                        t2->declType = DeclInteger;
                        t2->nodeType = nInt;
                        t2->lineno = (yyvsp[-4].tokenData)->linenum;
                        t2->attr.name = (yyvsp[-4].tokenData)->tokenstr;
                        t2->isC0 = 1;
                        t->lineno = (yyvsp[-5].tokenData)->linenum;
                        if((yyvsp[0].tree) != NULL)
                        {
                            t->child[0] = t2;
                            t->child[0]->isC0 = 1;
                            t->child[1] = (yyvsp[-2].tree);
                            t->child[1]->isC1 = 1;
                            t->child[2] = (yyvsp[0].tree);
                            t->child[2]->isC2 = 1;
                        }
                        else
                        {
                            t->child[0] = t2;
                            t->child[0]->isC0 = 1;
                            t->child[1] = (yyvsp[-2].tree);
                            t->child[1]->isC1 = 1;
                            t->child[2] = NULL;
                        }
                        
                        (yyval.tree) = t;
                    }
#line 2332 "parser.tab.c"
    break;

  case 51:
#line 678 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = StmtK;            //nodekind
                        t->subkind.stmt = WhileK;         //StmtKind
                        t->lineno = (yyvsp[-3].tokenData)->linenum;        //TokenData: linenum
                        if((yyvsp[0].tree) != NULL)
                        {
                            t->child[0] = (yyvsp[-2].tree);
                            t->child[0]->isC0 = 1;
                            t->child[1] = (yyvsp[0].tree);
                            t->child[1]->isC1 = 1;
                            t->child[2] = NULL;
                        }
                        else
                        {
                            t->child[0] = (yyvsp[-2].tree);
                            t->child[0]->isC0 = 1;
                            t->child[1] = NULL;
                            t->child[2] = NULL;   
                        }
                        (yyval.tree) = t;

                    }
#line 2360 "parser.tab.c"
    break;

  case 52:
#line 702 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = StmtK;            //nodekind
                        t->subkind.stmt = ForK;              //StmtKind
                        t->lineno = (yyvsp[-5].tokenData)->linenum;        //TokenData: linenum
                        //printf("iS: ",$1->svalue);
                        t->attr.name = (yyvsp[-5].tokenData)->tokenstr;
                        TreeNode* t2 = new TreeNode;
                        t2->nodekind = DeclK;
                        t2->subkind.decl = VarK;
                        t2->declType = DeclInteger;
                        t2->nodeType = nInt;
                        t2->isInit = true;
                        t2->lineno = (yyvsp[-4].tokenData)->linenum;
                        t2->attr.name = (yyvsp[-4].tokenData)->tokenstr;
                        t2->isC0 = 1;
                        t->lineno = (yyvsp[-5].tokenData)->linenum;
                        t->child[0] = t2;
                        t->child[0]->isC0 = 1;
                        t->child[1] = (yyvsp[-2].tree);
                        t->child[1]->isC1 = 1;
                        t->child[2] = (yyvsp[0].tree);
                        t->child[2]->isC2 = 1;
                        (yyval.tree) = t;
                    }
#line 2390 "parser.tab.c"
    break;

  case 53:
#line 729 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = StmtK;            //nodekind
                        t->subkind.stmt = RangeK;         //StmtKind
                        t->lineno = (yyvsp[-1].tokenData)->linenum;        //TokenData: linenum
                        t->child[0] = (yyvsp[-2].tree);
                        t->child[0]->isC0 = 1;
                        t->child[1] = (yyvsp[0].tree);
                        t->child[1]->isC1 = 1;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2407 "parser.tab.c"
    break;

  case 54:
#line 742 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = StmtK;            //nodekind
                        t->subkind.stmt = RangeK;         //StmtKind
                        t->lineno = (yyvsp[-3].tokenData)->linenum;        //TokenData: linenum
                        t->child[0] = (yyvsp[-4].tree);
                        t->child[0]->isC0 = 1;
                        t->child[1] = (yyvsp[-2].tree);
                        t->child[1]->isC1 = 1;
                        t->child[2] = (yyvsp[0].tree);
                        t->child[2]->isC2 = 1;
                        (yyval.tree) = t;
                    }
#line 2425 "parser.tab.c"
    break;

  case 55:
#line 757 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = StmtK;            //nodekind
                        t->subkind.stmt = ReturnK;           //StmtKind
                        t->lineno = (yyvsp[-1].tokenData)->linenum;        //TokenData: linenum
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2440 "parser.tab.c"
    break;

  case 56:
#line 768 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = StmtK;            //nodekind
                        t->subkind.stmt = ReturnK;           //StmtKind
                        t->attr.name = (yyvsp[-1].tree)->attr.name;
                        t->lineno = (yyvsp[-2].tokenData)->linenum;        //TokenData: linenum
                        t->child[0] = (yyvsp[-1].tree);
                        t->child[0]->isC0 = 1;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2457 "parser.tab.c"
    break;

  case 57:
#line 782 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = StmtK;            //nodekind
                        t->subkind.stmt = BreakK;         //StmtKind
                        t->lineno = (yyvsp[-1].tokenData)->linenum;        //TokenData: linenum
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2472 "parser.tab.c"
    break;

  case 58:
#line 794 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;             //nodekind
                        t->subkind.exp = AssignK;           //ExpKind   MAYBE SHOULD BE OpK??
                        t->expType = Integer;
                        t->nodeType = nInt;
                        t->lineno = (yyvsp[-1].tokenData)->linenum;        //TokenData: linenum
                        t->attr.op = '=';
                        t->attr.name = "=";                       
                        t->child[0] = (yyvsp[-2].tree);
                        t->child[0]->isC0 = 1;
                        t->child[1] = (yyvsp[0].tree);
                        t->child[1]->isC1 = 1;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2493 "parser.tab.c"
    break;

  case 59:
#line 811 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;            //nodekind
                        t->subkind.exp = AssignK;              //ExpKind
                        t->lineno = (yyvsp[-1].tokenData)->linenum;       //TokenData: linenum
                        t->expType = Integer;
                        t->nodeType = nInt;
                        t->attr.op = ADDASS;           //t->attr.op = $2; ????     
                        t->attr.name = "+=";            
                        t->child[0] = (yyvsp[-2].tree);
                        t->child[0]->isC0 = 1;
                        t->child[1] = (yyvsp[0].tree);
                        t->child[1]->isC1 = 1;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2514 "parser.tab.c"
    break;

  case 60:
#line 828 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;            //nodekind
                        t->subkind.exp = AssignK;              //ExpKind
                        t->lineno = (yyvsp[-1].tokenData)->linenum;        //TokenData: linenum
                        t->expType = Integer;
                        t->nodeType = nInt;
                        t->attr.op = MINASS;
                        t->attr.name = "-=";                     
                        t->child[0] = (yyvsp[-2].tree);
                        t->child[0]->isC0 = 1;
                        t->child[1] = (yyvsp[0].tree);
                        t->child[1]->isC1 = 1;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2535 "parser.tab.c"
    break;

  case 61:
#line 845 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;            //nodekind
                        t->subkind.exp = AssignK;         //ExpKind
                        t->lineno = (yyvsp[-1].tokenData)->linenum;        //TokenData: linenum
                        t->expType = Integer;
                        t->nodeType = nInt;
                        t->attr.op = MULASS;
                        t->attr.name = "*=";                       
                        t->child[0] = (yyvsp[-2].tree);
                        t->child[0]->isC0 = 1;
                        t->child[1] = (yyvsp[0].tree);
                        t->child[1]->isC1 = 1;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2556 "parser.tab.c"
    break;

  case 62:
#line 862 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;            //nodekind
                        t->subkind.exp = AssignK;              //ExpKind
                        t->lineno = (yyvsp[-1].tokenData)->linenum;       //TokenData: linenum
                        t->expType = Integer;
                        t->nodeType = nInt;
                        t->attr.op = DIVASS;
                        t->attr.name = "/=";                      
                        t->child[0] = (yyvsp[-2].tree);
                        t->child[0]->isC0 = 1;
                        t->child[1] = (yyvsp[0].tree);
                        t->child[1]->isC1 = 1;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2577 "parser.tab.c"
    break;

  case 63:
#line 879 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;            //nodekind
                        t->subkind.exp = AssignK;         //ExpKind
                        t->lineno = (yyvsp[0].tokenData)->linenum;        //TokenData: linenum
                        t->expType = Integer;
                        t->nodeType = nInt;
                        t->attr.op = INC;
                        t->attr.name = "++";                      
                        t->child[0] = (yyvsp[-1].tree);
                        t->child[0]->isC0 = 1;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2597 "parser.tab.c"
    break;

  case 64:
#line 895 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;            //nodekind
                        t->subkind.exp = AssignK;         //ExpKind
                        t->lineno = (yyvsp[0].tokenData)->linenum;        //TokenData: linenum
                        t->expType = Integer;
                        t->nodeType = nInt;
                        t->attr.op = DEC;  
                        t->attr.name = "--";                       
                        t->child[0] = (yyvsp[-1].tree);
                        t->child[0]->isC0 = 1;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2617 "parser.tab.c"
    break;

  case 65:
#line 911 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 2623 "parser.tab.c"
    break;

  case 66:
#line 914 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;            //nodekind
                        t->subkind.exp = OpK;         //ExpKind
                        t->lineno = (yyvsp[-1].tokenData)->linenum;        //TokenData: linenum
                        t->expType =  Boolean;
                        t->nodeType = nBool;
                        t->attr.op = OR;
                        t->attr.name = "OR";                  
                        t->child[0] = (yyvsp[-2].tree);
                        t->child[0]->isC0 = 1;
                        t->child[1] = (yyvsp[0].tree);
                        t->child[1]->isC1 = 1;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2644 "parser.tab.c"
    break;

  case 67:
#line 931 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 2650 "parser.tab.c"
    break;

  case 68:
#line 934 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;            //nodekind
                        t->subkind.exp = OpK;         //ExpKind
                        t->lineno = (yyvsp[-1].tokenData)->linenum;        //TokenData: linenum
                        t->expType =  Boolean;
                        t->nodeType = nBool;
                        t->attr.op = ANDD;
                        t->attr.name = "AND";                     
                        t->child[0] = (yyvsp[-2].tree);
                        t->child[0]->isC0 = 1;
                        t->child[1] = (yyvsp[0].tree);
                        t->child[1]->isC1 = 1;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2671 "parser.tab.c"
    break;

  case 69:
#line 951 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 2677 "parser.tab.c"
    break;

  case 70:
#line 954 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;            //nodekind
                        t->subkind.exp = OpK;              //ExpKind
                        t->lineno = (yyvsp[-1].tokenData)->linenum;       //TokenData: linenum
                        t->expType =  Boolean;
                        t->nodeType = nBool;
                        t->attr.op = NOT;
                        t->attr.name = "NOT";                      
                        t->child[0] = (yyvsp[0].tree);
                        t->child[0]->isC0 = 1;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2697 "parser.tab.c"
    break;

  case 71:
#line 970 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 2703 "parser.tab.c"
    break;

  case 72:
#line 973 "parser.y"
                    {
                        TreeNode* t = (yyvsp[-1].tree);    //make nodes in relop
                        t->child[0] = (yyvsp[-2].tree);
                        t->child[0]->isC0 = 1;
                        t->child[1] = (yyvsp[0].tree);
                        t->child[1]->isC1 = 1;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2717 "parser.tab.c"
    break;

  case 73:
#line 983 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 2723 "parser.tab.c"
    break;

  case 74:
#line 986 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = (yyvsp[0].tokenData)->linenum;       //TokenData: linenum
                        t->expType =  Boolean;
                        t->nodeType = nBool;
                        t->attr.op = LEQ;
                        t->attr.name = "<=";                     
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2742 "parser.tab.c"
    break;

  case 75:
#line 1001 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = (yyvsp[0].tokenData)->linenum;       //TokenData: linenum
                        t->expType =  Boolean;
                        t->nodeType = nBool;
                        t->attr.op = '<';
                        t->attr.name = "<";                    
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2761 "parser.tab.c"
    break;

  case 76:
#line 1016 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = (yyvsp[0].tokenData)->linenum;       //TokenData: linenum
                        t->expType =  Boolean;
                        t->nodeType = nBool;
                        t->attr.op = '>';
                        t->attr.name = ">";                    
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2780 "parser.tab.c"
    break;

  case 77:
#line 1031 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = (yyvsp[0].tokenData)->linenum;       //TokenData: linenum
                        t->expType =  Boolean;
                        t->nodeType = nBool;
                        t->attr.op = GEQ;
                        t->attr.name = ">=";                     
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2799 "parser.tab.c"
    break;

  case 78:
#line 1046 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = (yyvsp[0].tokenData)->linenum;       //TokenData: linenum
                        t->expType =  Boolean;
                        t->nodeType = nBool;
                        t->attr.op = EQ;
                        t->attr.name = "==";                     
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2818 "parser.tab.c"
    break;

  case 79:
#line 1061 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = (yyvsp[0].tokenData)->linenum;       //TokenData: linenum
                        t->expType =  Boolean;
                        t->nodeType = nBool;
                        t->attr.op = NEQ;
                        t->attr.name = "!=";                       
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2837 "parser.tab.c"
    break;

  case 80:
#line 1077 "parser.y"
                    {
                        TreeNode* t = (yyvsp[-1].tree);
                        t->child[0] = (yyvsp[-2].tree);
                        t->child[0]->isC0 = 1;
                        t->child[1] = (yyvsp[0].tree);
                        t->child[1]->isC1 = 1;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2851 "parser.tab.c"
    break;

  case 81:
#line 1087 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 2857 "parser.tab.c"
    break;

  case 82:
#line 1090 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = (yyvsp[0].tokenData)->linenum;       //TokenData: linenum
                        t->expType =  Integer;
                        t->nodeType = nInt;
                        t->attr.op = MAX;
                        t->attr.name = ":>:";                          
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2876 "parser.tab.c"
    break;

  case 83:
#line 1105 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = (yyvsp[0].tokenData)->linenum;       //TokenData: linenum
                        t->expType =  Integer;
                        t->nodeType = nInt;
                        t->attr.op = MIN;
                        t->attr.name = ":<:";                       
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2895 "parser.tab.c"
    break;

  case 84:
#line 1121 "parser.y"
                    {
                        TreeNode* t = (yyvsp[-1].tree);
                        t->child[0] = (yyvsp[-2].tree);
                        t->child[0]->isC0 = 1;
                        t->child[1] = (yyvsp[0].tree);
                        t->child[1]->isC1 = 1;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2909 "parser.tab.c"
    break;

  case 85:
#line 1131 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 2915 "parser.tab.c"
    break;

  case 86:
#line 1134 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = (yyvsp[0].tokenData)->linenum;       //TokenData: linenum
                        t->expType =  Integer;
                        t->nodeType = nInt;
                        t->attr.op = '+';
                        t->attr.name = "+";                       
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2934 "parser.tab.c"
    break;

  case 87:
#line 1149 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = (yyvsp[0].tokenData)->linenum;       //TokenData: linenum
                        t->expType =  Integer;
                        t->nodeType = nInt;
                        t->attr.op = '-';
                        t->attr.name = "-";                    
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2953 "parser.tab.c"
    break;

  case 88:
#line 1165 "parser.y"
                    {
                        TreeNode* t = (yyvsp[-1].tree);
                        t->child[0] = (yyvsp[-2].tree);
                        t->child[0]->isC0 = 1;
                        t->child[1] = (yyvsp[0].tree);
                        t->child[1]->isC1 = 1;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2967 "parser.tab.c"
    break;

  case 89:
#line 1175 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 2973 "parser.tab.c"
    break;

  case 90:
#line 1178 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = (yyvsp[0].tokenData)->linenum;       //TokenData: linenum
                        t->expType =  Integer;
                        t->nodeType = nInt;
                        t->attr.op = '*';
                        t->attr.name = "*";                          
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 2992 "parser.tab.c"
    break;

  case 91:
#line 1193 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = (yyvsp[0].tokenData)->linenum;       //TokenData: linenum
                        t->expType =  Integer;
                        t->nodeType = nInt;
                        t->attr.op = '/';
                        t->attr.name = "/";                          
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 3011 "parser.tab.c"
    break;

  case 92:
#line 1208 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = (yyvsp[0].tokenData)->linenum;       //TokenData: linenum
                        t->expType =  Integer;
                        t->nodeType = nInt;
                        t->attr.op = '%';
                        t->attr.name = "%";                          
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 3030 "parser.tab.c"
    break;

  case 93:
#line 1224 "parser.y"
                    {
                        TreeNode* t = (yyvsp[-1].tree);
                        t->child[0] = (yyvsp[0].tree);
                        t->child[0]->isC0 = 1;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 3043 "parser.tab.c"
    break;

  case 94:
#line 1233 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 3049 "parser.tab.c"
    break;

  case 95:
#line 1236 "parser.y"
                    {
                        //NEGATE
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = (yyvsp[0].tokenData)->linenum;       //TokenData: linenum
                        t->expType =  Integer;
                        t->nodeType = nInt;
                        t->attr.op = '-';
                        t->attr.name = "CHSIGN";                          
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 3069 "parser.tab.c"
    break;

  case 96:
#line 1252 "parser.y"
                    {
                        //SIZEOF
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = (yyvsp[0].tokenData)->linenum;       //TokenData: linenum
                        t->expType =  Integer;
                        t->nodeType = nInt;
                        t->attr.op = '*';
                        t->attr.name = "SIZEOF";                          
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 3089 "parser.tab.c"
    break;

  case 97:
#line 1268 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = OpK;               //ExpKind
                        t->lineno = (yyvsp[0].tokenData)->linenum;       //TokenData: linenum
                        t->expType =  Integer;
                        t->nodeType = nInt;
                        t->attr.op = '?';
                        t->attr.name = "?";                          
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 3108 "parser.tab.c"
    break;

  case 98:
#line 1284 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 3114 "parser.tab.c"
    break;

  case 99:
#line 1286 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 3120 "parser.tab.c"
    break;

  case 100:
#line 1289 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = IdK;               //ExpKind
                        //printf("m: ",$1->svalue);
                        t->attr.name = (yyvsp[0].tokenData)->tokenstr;
                        t->lineno = (yyvsp[0].tokenData)->linenum;
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 3137 "parser.tab.c"
    break;

  case 101:
#line 1302 "parser.y"
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
                        t2->lineno = (yyvsp[-3].tokenData)->linenum;
                        t2->attr.name = (yyvsp[-3].tokenData)->tokenstr;
                        t2->isArray = true;
                        t2->isC0 = 1;
                        t->lineno = (yyvsp[-3].tokenData)->linenum;
                        t->child[0] = t2;
                        t->child[1] = (yyvsp[-1].tree);
                        t->child[1]->isC1 = 1;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 3166 "parser.tab.c"
    break;

  case 102:
#line 1328 "parser.y"
                    { (yyval.tree) = (yyvsp[-1].tree); }
#line 3172 "parser.tab.c"
    break;

  case 103:
#line 1330 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 3178 "parser.tab.c"
    break;

  case 104:
#line 1332 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 3184 "parser.tab.c"
    break;

  case 105:
#line 1335 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = CallK;               //ExpKind
                        //printf("c: ",$1->svalue);
                        t->attr.name = (yyvsp[-3].tokenData)->tokenstr;
                        t->lineno = (yyvsp[-3].tokenData)->linenum;
                        if((yyvsp[-1].tree) != NULL){
                            t->child[0] = (yyvsp[-1].tree);
                            t->child[0]->isC0 = 1;
                            t->child[1] = NULL;
                            t->child[2] = NULL;
                        }
                        else{
                            t->child[0] = NULL;
                            t->child[1] = NULL;
                            t->child[2] = NULL;
                        }
                        (yyval.tree) = t;
                    }
#line 3209 "parser.tab.c"
    break;

  case 106:
#line 1357 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 3215 "parser.tab.c"
    break;

  case 107:
#line 1359 "parser.y"
                    { (yyval.tree) = NULL; }
#line 3221 "parser.tab.c"
    break;

  case 108:
#line 1362 "parser.y"
                    {
                        TreeNode* t = (yyvsp[-2].tree);
                        if(t != NULL)
                        {   
                            while(t->sibling != NULL)
                            { t = t->sibling; }
                            t->sibling = (yyvsp[0].tree);
                            t->lineno = (yyvsp[-1].tokenData)->linenum;
                            (yyval.tree) = (yyvsp[-2].tree);
                        }
                        else{ (yyval.tree) = (yyvsp[0].tree); }
                    }
#line 3238 "parser.tab.c"
    break;

  case 109:
#line 1375 "parser.y"
                    { (yyval.tree) = (yyvsp[0].tree); }
#line 3244 "parser.tab.c"
    break;

  case 110:
#line 1378 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->expType = Integer;
                        t->nodeType = nInt;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = ConstantK;               //ExpKind
                        t->attr.name = (yyvsp[0].tokenData)->tokenstr;
                        //printf("nc: %s\n", $1->tokenstr);
                        t->attr.value = (yyvsp[0].tokenData)->nvalue;
                        t->lineno = (yyvsp[0].tokenData)->linenum;
                        t->isNC = true;
                        t->isConstant = true;
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 3266 "parser.tab.c"
    break;

  case 111:
#line 1396 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->expType = Char;
                        t->nodeType = nChar;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = ConstantK;               //ExpKind
                        //printf("Cc: %s\n",$1->tokenstr);
                        t->isConstant = true;
                        t->attr.name = (yyvsp[0].tokenData)->tokenstr;
                        t->attr.cvalue = (yyvsp[0].tokenData)->cvalue;
                        t->lineno = (yyvsp[0].tokenData)->linenum;
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 3287 "parser.tab.c"
    break;

  case 112:
#line 1413 "parser.y"
                    {
                        TreeNode* t = new TreeNode;
                        t->expType = Char;
                        t->nodeType = nChar;
                        t->nodekind = ExpK;              //nodekind
                        t->subkind.exp = ConstantK;               //ExpKind
                        //printf("Sc: ",$1->svalue);
                        t->isArray = 1;
                        t->isConstant = true;
                        t->attr.name = (yyvsp[0].tokenData)->tokenstr;
                        t->attr.string = (yyvsp[0].tokenData)->tokenstr;       // MAYBE TOKENSTR???
                        t->lineno = (yyvsp[0].tokenData)->linenum;
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 3309 "parser.tab.c"
    break;

  case 113:
#line 1431 "parser.y"
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
                        t->lineno = (yyvsp[0].tokenData)->linenum;
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 3331 "parser.tab.c"
    break;

  case 114:
#line 1449 "parser.y"
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
                        t->lineno = (yyvsp[0].tokenData)->linenum;
                        t->child[0] = NULL;
                        t->child[1] = NULL;
                        t->child[2] = NULL;
                        (yyval.tree) = t;
                    }
#line 3353 "parser.tab.c"
    break;


#line 3357 "parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1467 "parser.y"

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
