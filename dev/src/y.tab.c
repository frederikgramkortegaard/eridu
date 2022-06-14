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

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


#include "tree.h"
#include "types.h"
#include "symbols.h"
#include "typechecking.h"
#include "optimizer.h"
#include "codegen.h"
#include "emit.h"

#include "logging.h"


extern char *yytext;
extern int yylineno;

void yyerror() {
    printf("syntax error before %s on line: '%ld'\n", yytext, yylineno);
}

float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}


#line 101 "y.tab.c"

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
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
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
    INTVAL = 259,
    CHARVAL = 260,
    FLOATVAL = 261,
    STRINGVAL = 262,
    INT = 263,
    STR = 264,
    CHAR = 265,
    FLOAT = 266,
    CALLABLE = 267,
    DEFINE = 268,
    RETURN = 269,
    WHILE = 270,
    BREAK = 271,
    CONTINUE = 272,
    IF = 273,
    LEQ = 274,
    GEQ = 275,
    EQUAL = 276,
    NEQUAL = 277,
    STAR = 278,
    STRUCT = 279,
    CAST = 280,
    PRINT = 281,
    SIZEOF = 282,
    CASTABLE = 283,
    ALLOCATE = 284,
    ADDRESSOF = 285,
    DEREFERENCE = 286
  };
#endif
/* Tokens.  */
#define ID 258
#define INTVAL 259
#define CHARVAL 260
#define FLOATVAL 261
#define STRINGVAL 262
#define INT 263
#define STR 264
#define CHAR 265
#define FLOAT 266
#define CALLABLE 267
#define DEFINE 268
#define RETURN 269
#define WHILE 270
#define BREAK 271
#define CONTINUE 272
#define IF 273
#define LEQ 274
#define GEQ 275
#define EQUAL 276
#define NEQUAL 277
#define STAR 278
#define STRUCT 279
#define CAST 280
#define PRINT 281
#define SIZEOF 282
#define CASTABLE 283
#define ALLOCATE 284
#define ADDRESSOF 285
#define DEREFERENCE 286

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 32 "parser.y"

    int intval;
    char charval;
    float floatval;
    char * stringval;
    struct AST_NODE * node;

#line 223 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */



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
#define YYFINAL  85
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   616

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  82
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  187

#define YYUNDEFTOK  2
#define YYMAXUTOK   286


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
       2,     2,     2,    32,     2,     2,     2,    38,     2,     2,
      47,    48,     2,    35,    39,    36,    42,    37,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    46,
      34,    45,    33,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    40,     2,    41,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    43,     2,    44,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,   110,   110,   111,   112,   113,   116,   117,   118,   120,
     123,   124,   125,   126,   129,   130,   131,   135,   136,   137,
     140,   143,   144,   145,   148,   151,   152,   153,   154,   155,
     156,   157,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   178,   181,
     182,   184,   185,   186,   189,   190,   191,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   220,   221,
     224,   225,   228
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ID", "INTVAL", "CHARVAL", "FLOATVAL",
  "STRINGVAL", "INT", "STR", "CHAR", "FLOAT", "CALLABLE", "DEFINE",
  "RETURN", "WHILE", "BREAK", "CONTINUE", "IF", "LEQ", "GEQ", "EQUAL",
  "NEQUAL", "STAR", "STRUCT", "CAST", "PRINT", "SIZEOF", "CASTABLE",
  "ALLOCATE", "ADDRESSOF", "DEREFERENCE", "'!'", "'>'", "'<'", "'+'",
  "'-'", "'/'", "'%'", "','", "'['", "']'", "'.'", "'{'", "'}'", "'='",
  "';'", "'('", "')'", "$accept", "simpletype", "typedcallable", "pointer",
  "type", "typelist", "parameters", "parameterlist", "typedparameters",
  "typedparameterlist", "literal", "expression", "block", "assignable",
  "assignments", "statement", "functioncall", "derefexpr", "statementlist",
  "program", YY_NULLPTR
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
     285,   286,    33,    62,    60,    43,    45,    47,    37,    44,
      91,    93,    46,   123,   125,    61,    59,    40,    41
};
# endif

#define YYPACT_NINF (-126)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-82)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     266,    59,  -126,  -126,  -126,  -126,  -126,  -126,  -126,  -126,
    -126,    39,   189,   -34,   -13,   -10,    -9,    41,    -2,     6,
      19,    21,    27,    29,    30,   189,    39,   307,  -126,  -126,
    -126,     9,  -126,   461,    11,   -15,    43,   224,  -126,    -3,
    -126,    90,   189,    88,   178,    99,    20,    58,    91,   489,
    -126,   189,  -126,  -126,   189,    70,    39,   189,     7,    39,
      39,   189,   189,   578,    42,    72,   -39,  -126,   189,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,  -126,
    -126,    76,   189,  -126,  -126,  -126,   495,    73,  -126,    69,
     550,  -126,    77,   189,   120,  -126,    74,   365,    82,   -18,
     371,    78,   -20,   -14,     1,   395,   401,   115,  -126,   189,
    -126,   578,   578,   578,   578,  -126,   578,   578,    32,    32,
    -126,  -126,   189,   578,    83,   189,   178,  -126,   178,    39,
     518,    86,    94,    96,    39,   189,  -126,  -126,  -126,   189,
     189,  -126,    93,   107,   578,   578,   527,    95,  -126,   349,
    -126,    98,  -126,   307,   307,   100,   425,   431,   455,   178,
      39,  -126,  -126,   557,   104,   108,   113,   102,  -126,  -126,
    -126,   103,    23,   109,    39,   112,   116,   117,  -126,  -126,
      39,  -126,  -126,  -126,  -126,  -126,  -126
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
      47,    28,    25,    27,    26,    29,     2,     3,     4,     5,
       8,     0,    47,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    47,     0,    47,    10,    11,
      12,    33,    32,     0,     0,     0,     0,    47,    34,    77,
      82,     0,    47,     0,    47,     0,     0,    28,    33,     0,
      77,    47,    63,    64,    47,    13,     0,    47,     0,     0,
       0,    47,    47,    46,     0,     0,     0,     9,    47,    47,
      47,    47,    47,    47,    47,    47,    47,    47,    47,    62,
      66,     0,    47,    61,    80,     1,     0,    31,    20,     0,
      17,    13,     0,    47,     0,    59,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    47,
      57,    37,    38,    39,    36,    35,    40,    41,    42,    45,
      44,    43,    47,    56,    30,    47,    47,    68,    47,    23,
       0,    31,     0,     0,    23,    47,    71,    70,    69,    47,
      47,    75,    78,     7,    54,    55,     0,     0,    18,    47,
      24,     0,    30,    47,    47,     0,     0,     0,     0,    47,
      16,    53,    76,    21,     0,     0,     0,     0,    73,    72,
      74,     0,    14,     0,    23,     0,     0,     0,    67,    79,
      16,     6,    22,    65,    58,    60,    15
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -126,  -126,  -126,  -126,     0,   -19,    36,  -125,    -8,    31,
    -126,    10,     3,  -126,  -126,  -126,  -126,     2,   -23,  -126
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    28,    29,    30,    48,   173,    88,    89,   150,   151,
      32,    33,    34,    35,    36,    37,    38,    50,    40,    41
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      31,   147,    39,    67,    65,    67,   109,   110,    81,    67,
     101,    46,    66,    51,    84,     6,     7,     8,     9,    10,
     -50,   135,    49,    92,    67,   139,    64,    31,   138,    39,
      82,    45,    67,    52,   171,    63,    53,    31,    54,    39,
     140,    26,   -50,    67,    55,    56,    67,     6,     7,     8,
       9,    10,    86,    57,    90,    72,    99,    80,   102,   103,
     104,    96,   180,    45,    97,    67,    58,   100,    59,    77,
      78,   105,   106,    26,    60,   107,    61,    62,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,    83,
      85,    87,   123,    68,    69,    70,    71,    72,    93,    42,
      94,    43,    91,   130,   -49,    44,    44,    73,    74,    75,
      76,    77,    78,   125,    67,    98,   108,   127,   -52,   144,
     126,   122,   132,   131,   129,   134,   137,   143,   -51,   149,
     165,   166,   145,   126,   149,   146,    90,   153,    90,   154,
     159,   160,   181,   162,   167,   156,   164,    27,   178,   157,
     158,   179,   176,    31,    31,    39,    39,   177,   183,   163,
     172,   186,   184,   185,   148,   155,   182,   175,     0,    90,
       0,     0,     0,     0,   149,     0,     0,     0,     0,     0,
     172,    47,     2,     3,     4,     5,     6,     7,     8,     9,
      10,     0,    47,     2,     3,     4,     5,     6,     7,     8,
       9,    10,    45,    18,    19,    20,    21,    22,    23,    24,
      25,     0,    26,    45,    18,    19,    20,    21,    22,    23,
      24,    25,     0,    26,   -81,     0,   -19,     1,     2,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,     0,     0,     0,     0,     0,    17,    18,
      19,    20,    21,    22,    23,    24,    25,     0,    26,     0,
       0,     0,     0,     0,     0,     0,   -81,    27,   -81,     1,
       2,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,     0,     0,     0,     0,     0,
      17,    18,    19,    20,    21,    22,    23,    24,    25,     0,
      26,     0,     0,     0,     0,     0,     0,     0,     0,    27,
       1,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,     0,     0,     0,     0,
       0,    17,    18,    19,    20,    21,    22,    23,    24,    25,
       0,    26,     0,     0,     0,     0,     0,     0,     0,     0,
      27,   -81,    47,     2,     3,     4,     5,     6,     7,     8,
       9,    10,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    67,    45,    18,    19,    20,    21,    22,    23,
      24,    25,     0,    26,    68,    69,    70,    71,    72,     0,
      68,    69,    70,    71,    72,     0,     0,     0,    73,    74,
      75,    76,    77,    78,    73,    74,    75,    76,    77,    78,
       0,     0,     0,   133,    68,    69,    70,    71,    72,   136,
      68,    69,    70,    71,    72,     0,     0,     0,    73,    74,
      75,    76,    77,    78,    73,    74,    75,    76,    77,    78,
       0,     0,     0,   141,    68,    69,    70,    71,    72,   142,
      68,    69,    70,    71,    72,     0,     0,     0,    73,    74,
      75,    76,    77,    78,    73,    74,    75,    76,    77,    78,
       0,     0,     0,   168,    68,    69,    70,    71,    72,   169,
      68,    69,    70,    71,    72,     0,     0,     0,    73,    74,
      75,    76,    77,    78,    73,    74,    75,    76,    77,    78,
       0,     0,     0,   170,     0,     0,     0,    79,    68,    69,
      70,    71,    72,     0,    68,    69,    70,    71,    72,     0,
       0,     0,    73,    74,    75,    76,    77,    78,    73,    74,
      75,    76,    77,    78,     0,    95,   124,    68,    69,    70,
      71,    72,     0,     0,     0,     0,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,     0,     0,   152,
      73,    74,    75,    76,    77,    78,     0,     0,   161,    68,
      69,    70,    71,    72,     0,     0,    68,    69,    70,    71,
      72,     0,     0,    73,    74,    75,    76,    77,    78,   128,
      73,    74,    75,    76,    77,    78,   174,    68,    69,    70,
      71,    72,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,    78
};

static const yytype_int16 yycheck[] =
{
       0,   126,     0,    23,    27,    23,    45,    46,    23,    23,
       3,    11,     3,    47,    37,     8,     9,    10,    11,    12,
      23,    39,    12,     3,    23,    39,    26,    27,    48,    27,
      45,    24,    23,    46,   159,    25,    46,    37,    47,    37,
      39,    34,    45,    23,     3,    47,    23,     8,     9,    10,
      11,    12,    42,    47,    44,    23,    56,    46,    58,    59,
      60,    51,    39,    24,    54,    23,    47,    57,    47,    37,
      38,    61,    62,    34,    47,    33,    47,    47,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    46,
       0,     3,    82,    19,    20,    21,    22,    23,    40,    40,
      42,    42,     3,    93,    45,    47,    47,    33,    34,    35,
      36,    37,    38,    40,    23,    45,    44,    48,    45,   109,
      47,    45,    48,     3,    47,    43,    48,    12,    45,   129,
     153,   154,   122,    47,   134,   125,   126,    43,   128,    43,
      47,    34,    33,    48,    44,   135,    48,    43,    46,   139,
     140,    48,    44,   153,   154,   153,   154,    44,    46,   149,
     160,   180,    46,    46,   128,   134,   174,   164,    -1,   159,
      -1,    -1,    -1,    -1,   174,    -1,    -1,    -1,    -1,    -1,
     180,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    -1,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    34,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    34,     0,    -1,    48,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     0,    43,    44,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      43,    44,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    34,    19,    20,    21,    22,    23,    -1,
      19,    20,    21,    22,    23,    -1,    -1,    -1,    33,    34,
      35,    36,    37,    38,    33,    34,    35,    36,    37,    38,
      -1,    -1,    -1,    48,    19,    20,    21,    22,    23,    48,
      19,    20,    21,    22,    23,    -1,    -1,    -1,    33,    34,
      35,    36,    37,    38,    33,    34,    35,    36,    37,    38,
      -1,    -1,    -1,    48,    19,    20,    21,    22,    23,    48,
      19,    20,    21,    22,    23,    -1,    -1,    -1,    33,    34,
      35,    36,    37,    38,    33,    34,    35,    36,    37,    38,
      -1,    -1,    -1,    48,    19,    20,    21,    22,    23,    48,
      19,    20,    21,    22,    23,    -1,    -1,    -1,    33,    34,
      35,    36,    37,    38,    33,    34,    35,    36,    37,    38,
      -1,    -1,    -1,    48,    -1,    -1,    -1,    46,    19,    20,
      21,    22,    23,    -1,    19,    20,    21,    22,    23,    -1,
      -1,    -1,    33,    34,    35,    36,    37,    38,    33,    34,
      35,    36,    37,    38,    -1,    46,    41,    19,    20,    21,
      22,    23,    -1,    -1,    -1,    -1,    19,    20,    21,    22,
      23,    33,    34,    35,    36,    37,    38,    -1,    -1,    41,
      33,    34,    35,    36,    37,    38,    -1,    -1,    41,    19,
      20,    21,    22,    23,    -1,    -1,    19,    20,    21,    22,
      23,    -1,    -1,    33,    34,    35,    36,    37,    38,    39,
      33,    34,    35,    36,    37,    38,    39,    19,    20,    21,
      22,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    34,    35,    36,    37,    38
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    34,    43,    50,    51,
      52,    53,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    40,    42,    47,    24,    53,     3,    53,    60,
      66,    47,    46,    46,    47,     3,    47,    47,    47,    47,
      47,    47,    47,    60,    53,    67,     3,    23,    19,    20,
      21,    22,    23,    33,    34,    35,    36,    37,    38,    46,
      46,    23,    45,    46,    67,     0,    60,     3,    55,    56,
      60,     3,     3,    40,    42,    46,    60,    60,    45,    53,
      60,     3,    53,    53,    53,    60,    60,    33,    44,    45,
      46,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    45,    60,    41,    40,    47,    48,    39,    47,
      60,     3,    48,    48,    43,    39,    48,    48,    48,    39,
      39,    48,    48,    12,    60,    60,    60,    56,    55,    53,
      57,    58,    41,    43,    43,    58,    60,    60,    60,    47,
      34,    41,    48,    60,    48,    67,    67,    44,    48,    48,
      48,    56,    53,    54,    39,    61,    44,    44,    46,    48,
      39,    33,    57,    46,    46,    46,    54
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    49,    50,    50,    50,    50,    51,    51,    51,    52,
      53,    53,    53,    53,    54,    54,    54,    55,    55,    55,
      56,    57,    57,    57,    58,    59,    59,    59,    59,    59,
      59,    59,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    61,    62,
      62,    62,    62,    62,    63,    63,    63,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    64,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    66,    66,
      67,    67,    68
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     7,     4,     1,     2,
       1,     1,     1,     2,     1,     3,     0,     1,     3,     0,
       1,     2,     4,     0,     1,     1,     1,     1,     1,     1,
       4,     3,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     0,     3,     1,
       1,     4,     3,     6,     4,     4,     3,     3,     8,     3,
       8,     2,     2,     2,     2,     8,     2,     7,     4,     4,
       4,     4,     6,     6,     6,     4,     6,     1,     4,     7,
       2,     0,     1
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
#line 110 "parser.y"
                 { (yyval.node) = make_leaf(A_INT,   NULL, yylineno); }
#line 1603 "y.tab.c"
    break;

  case 3:
#line 111 "parser.y"
                 { (yyval.node) = make_leaf(A_STRING,   NULL, yylineno); }
#line 1609 "y.tab.c"
    break;

  case 4:
#line 112 "parser.y"
                 { (yyval.node) = make_leaf(A_CHAR,  NULL, yylineno); }
#line 1615 "y.tab.c"
    break;

  case 5:
#line 113 "parser.y"
                 { (yyval.node) = make_leaf(A_FLOAT, NULL, yylineno); }
#line 1621 "y.tab.c"
    break;

  case 6:
#line 116 "parser.y"
                                                 { (yyval.node) = make_binary(A_CALLABLE, (yyvsp[-5].node), (yyvsp[-1].node), yylineno); }
#line 1627 "y.tab.c"
    break;

  case 7:
#line 117 "parser.y"
                                   { (yyval.node) = make_binary(A_CALLABLE, (yyvsp[-2].node), NULL, yylineno); }
#line 1633 "y.tab.c"
    break;

  case 8:
#line 118 "parser.y"
                         { (yyval.node) = make_leaf(A_CALLABLE, NULL, yylineno); }
#line 1639 "y.tab.c"
    break;

  case 9:
#line 120 "parser.y"
                    { (yyval.node) = make_unary(A_POINTER, (yyvsp[-1].node), yylineno); }
#line 1645 "y.tab.c"
    break;

  case 10:
#line 123 "parser.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 1651 "y.tab.c"
    break;

  case 11:
#line 124 "parser.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 1657 "y.tab.c"
    break;

  case 12:
#line 125 "parser.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 1663 "y.tab.c"
    break;

  case 13:
#line 126 "parser.y"
                 { (yyval.node) = make_unary(A_STRUCT, make_leaf(A_ID, (yyvsp[0].stringval), yylineno), yylineno); }
#line 1669 "y.tab.c"
    break;

  case 14:
#line 129 "parser.y"
                              { (yyval.node) = (yyvsp[0].node); }
#line 1675 "y.tab.c"
    break;

  case 15:
#line 130 "parser.y"
                              { (yyval.node) = make_binary(A_GLUE, (yyvsp[-2].node), (yyvsp[0].node), yylineno); }
#line 1681 "y.tab.c"
    break;

  case 16:
#line 131 "parser.y"
                              { (yyval.node) = make_leaf(A_EMPTY, NULL, yylineno);    }
#line 1687 "y.tab.c"
    break;

  case 17:
#line 135 "parser.y"
                                                  { (yyval.node) = make_unary(A_PARAM, (yyvsp[0].node), yylineno); }
#line 1693 "y.tab.c"
    break;

  case 18:
#line 136 "parser.y"
                                                  { (yyval.node) = make_binary(A_GLUE, make_unary(A_PARAM, (yyvsp[-2].node), yylineno), (yyvsp[0].node), yylineno);}
#line 1699 "y.tab.c"
    break;

  case 19:
#line 137 "parser.y"
                                                  { (yyval.node) = make_leaf(A_EMPTY, NULL, yylineno);}
#line 1705 "y.tab.c"
    break;

  case 20:
#line 140 "parser.y"
                                                  {(yyval.node) = make_unary(A_PARAM_LIST, (yyvsp[0].node), yylineno);}
#line 1711 "y.tab.c"
    break;

  case 21:
#line 143 "parser.y"
                                                  { (yyval.node) = make_binary(A_PARAM, (yyvsp[-1].node), (yyvsp[0].node), yylineno);}
#line 1717 "y.tab.c"
    break;

  case 22:
#line 144 "parser.y"
                                                  { (yyval.node) = make_binary(A_GLUE, make_binary(A_PARAM, (yyvsp[-3].node), (yyvsp[-2].node), yylineno), (yyvsp[0].node), yylineno);}
#line 1723 "y.tab.c"
    break;

  case 23:
#line 145 "parser.y"
                                                  { (yyval.node) = make_leaf(A_EMPTY, NULL, yylineno);}
#line 1729 "y.tab.c"
    break;

  case 24:
#line 148 "parser.y"
                                             {(yyval.node) = make_unary(A_PARAM_LIST, (yyvsp[0].node), yylineno);}
#line 1735 "y.tab.c"
    break;

  case 25:
#line 151 "parser.y"
                         { (yyval.node) = make_leaf(A_INTVAL,   &(yyvsp[0].intval), yylineno); }
#line 1741 "y.tab.c"
    break;

  case 26:
#line 152 "parser.y"
                         { (yyval.node) = make_leaf(A_FLOATVAL, &(yyvsp[0].floatval), yylineno); }
#line 1747 "y.tab.c"
    break;

  case 27:
#line 153 "parser.y"
                         { (yyval.node) = make_leaf(A_CHARVAL,  &(yyvsp[0].charval), yylineno); }
#line 1753 "y.tab.c"
    break;

  case 28:
#line 154 "parser.y"
                         { (yyval.node) = make_leaf(A_ID,        (yyvsp[0].stringval), yylineno); }
#line 1759 "y.tab.c"
    break;

  case 29:
#line 155 "parser.y"
                         { (yyval.node) = make_leaf(A_STRINGVAL, (yyvsp[0].stringval), yylineno); }
#line 1765 "y.tab.c"
    break;

  case 30:
#line 156 "parser.y"
                                { (yyval.node) = make_binary(A_INDEX, make_leaf(A_ID, (yyvsp[-3].stringval), yylineno), (yyvsp[-1].node), yylineno); }
#line 1771 "y.tab.c"
    break;

  case 31:
#line 157 "parser.y"
                         { (yyval.node) = make_binary(A_DOT, make_leaf(A_ID, (yyvsp[-2].stringval), yylineno), make_leaf(A_ID, (yyvsp[0].stringval), yylineno), yylineno); }
#line 1777 "y.tab.c"
    break;

  case 32:
#line 160 "parser.y"
                                           { (yyval.node) = (yyvsp[0].node); }
#line 1783 "y.tab.c"
    break;

  case 33:
#line 161 "parser.y"
                                           { (yyval.node) = (yyvsp[0].node); }
#line 1789 "y.tab.c"
    break;

  case 34:
#line 162 "parser.y"
                                           { (yyval.node) = (yyvsp[0].node); }
#line 1795 "y.tab.c"
    break;

  case 35:
#line 163 "parser.y"
                                           { (yyval.node) = make_binary(A_MUL,    (yyvsp[-2].node), (yyvsp[0].node), yylineno); }
#line 1801 "y.tab.c"
    break;

  case 36:
#line 164 "parser.y"
                                           { (yyval.node) = make_binary(A_NEQUAL, (yyvsp[-2].node), (yyvsp[0].node), yylineno); }
#line 1807 "y.tab.c"
    break;

  case 37:
#line 165 "parser.y"
                                           { (yyval.node) = make_binary(A_LEQ,    (yyvsp[-2].node), (yyvsp[0].node), yylineno); }
#line 1813 "y.tab.c"
    break;

  case 38:
#line 166 "parser.y"
                                           { (yyval.node) = make_binary(A_GEQ,    (yyvsp[-2].node), (yyvsp[0].node), yylineno); }
#line 1819 "y.tab.c"
    break;

  case 39:
#line 167 "parser.y"
                                           { (yyval.node) = make_binary(A_EQUAL,  (yyvsp[-2].node), (yyvsp[0].node), yylineno); }
#line 1825 "y.tab.c"
    break;

  case 40:
#line 168 "parser.y"
                                           { (yyval.node) = make_binary(A_GREAT,  (yyvsp[-2].node), (yyvsp[0].node), yylineno); }
#line 1831 "y.tab.c"
    break;

  case 41:
#line 169 "parser.y"
                                           { (yyval.node) = make_binary(A_LESS,   (yyvsp[-2].node), (yyvsp[0].node), yylineno); }
#line 1837 "y.tab.c"
    break;

  case 42:
#line 170 "parser.y"
                                           { (yyval.node) = make_binary(A_ADD,    (yyvsp[-2].node), (yyvsp[0].node), yylineno); }
#line 1843 "y.tab.c"
    break;

  case 43:
#line 171 "parser.y"
                                           { (yyval.node) = make_binary(A_MOD,    (yyvsp[-2].node), (yyvsp[0].node), yylineno); }
#line 1849 "y.tab.c"
    break;

  case 44:
#line 172 "parser.y"
                                           { (yyval.node) = make_binary(A_DIV,    (yyvsp[-2].node), (yyvsp[0].node), yylineno); }
#line 1855 "y.tab.c"
    break;

  case 45:
#line 173 "parser.y"
                                           { (yyval.node) = make_binary(A_SUB,    (yyvsp[-2].node), (yyvsp[0].node), yylineno); }
#line 1861 "y.tab.c"
    break;

  case 46:
#line 174 "parser.y"
                                           { (yyval.node) = make_unary(A_NEG,     (yyvsp[0].node), yylineno);     }
#line 1867 "y.tab.c"
    break;

  case 47:
#line 175 "parser.y"
                                           { (yyval.node) = make_leaf(A_EMPTY, NULL, yylineno);      }
#line 1873 "y.tab.c"
    break;

  case 48:
#line 178 "parser.y"
                                                { (yyval.node) = make_unary(A_BLOCK, (yyvsp[-1].node), yylineno); }
#line 1879 "y.tab.c"
    break;

  case 49:
#line 181 "parser.y"
                                                { (yyval.node) = make_leaf(A_ID, (yyvsp[0].stringval), yylineno); }
#line 1885 "y.tab.c"
    break;

  case 50:
#line 182 "parser.y"
                                                { (yyval.node) = (yyvsp[0].node);}
#line 1891 "y.tab.c"
    break;

  case 51:
#line 184 "parser.y"
                                                { (yyval.node) = make_binary(A_INDEX, make_leaf(A_ID, (yyvsp[-3].stringval), yylineno), (yyvsp[-1].node), yylineno); }
#line 1897 "y.tab.c"
    break;

  case 52:
#line 185 "parser.y"
                                                { (yyval.node) = make_binary(A_DOT, make_leaf(A_ID, (yyvsp[-2].stringval), yylineno), make_leaf(A_ID, (yyvsp[0].stringval), yylineno), yylineno); }
#line 1903 "y.tab.c"
    break;

  case 53:
#line 186 "parser.y"
                                                { (yyval.node) = make_binary(A_INDEX, make_binary(A_DOT, make_leaf(A_ID, (yyvsp[-5].stringval), yylineno), make_leaf(A_ID, (yyvsp[-3].stringval), yylineno), yylineno), (yyvsp[-1].node), yylineno); }
#line 1909 "y.tab.c"
    break;

  case 54:
#line 189 "parser.y"
                                                { (yyval.node) = make_binary(A_GLUE, make_binary(A_DECL, (yyvsp[-3].node), make_leaf(A_ID, (yyvsp[-2].stringval), yylineno), yylineno), make_binary(A_ASSIGN, make_leaf(A_ID, (yyvsp[-2].stringval), yylineno), (yyvsp[0].node), yylineno), yylineno); }
#line 1915 "y.tab.c"
    break;

  case 55:
#line 190 "parser.y"
                                                { (yyval.node) = make_binary(A_ASSIGN, (yyvsp[-3].node), make_binary(A_MUL, (yyvsp[-3].node), (yyvsp[0].node), yylineno), yylineno); }
#line 1921 "y.tab.c"
    break;

  case 56:
#line 191 "parser.y"
                                                { (yyval.node) = make_binary(A_ASSIGN, (yyvsp[-2].node), (yyvsp[0].node), yylineno);                                   }
#line 1927 "y.tab.c"
    break;

  case 57:
#line 195 "parser.y"
                                                              { (yyval.node) = make_binary(A_DECL, (yyvsp[-2].node), make_leaf(A_ID, (yyvsp[-1].stringval), yylineno), yylineno);}
#line 1933 "y.tab.c"
    break;

  case 58:
#line 196 "parser.y"
                                                              { (yyval.node) =  make_binary(A_GLUE, make_binary(A_WHILE, (yyvsp[-5].node), (yyvsp[-2].node), yylineno), make_unary(A_END_WHILE, NULL, yylineno), yylineno); }
#line 1939 "y.tab.c"
    break;

  case 59:
#line 197 "parser.y"
                                                              { (yyval.node) = make_unary(A_RETURN, (yyvsp[-1].node), yylineno);}
#line 1945 "y.tab.c"
    break;

  case 60:
#line 198 "parser.y"
                                                              { (yyval.node) = make_binary(A_IF, (yyvsp[-5].node), (yyvsp[-2].node), yylineno); }
#line 1951 "y.tab.c"
    break;

  case 61:
#line 199 "parser.y"
                                                              { (yyval.node) = (yyvsp[-1].node); }
#line 1957 "y.tab.c"
    break;

  case 62:
#line 200 "parser.y"
                                                              { (yyval.node) = (yyvsp[-1].node); }
#line 1963 "y.tab.c"
    break;

  case 63:
#line 201 "parser.y"
                                                              { (yyval.node) = make_leaf(A_BREAK, NULL, yylineno); }
#line 1969 "y.tab.c"
    break;

  case 64:
#line 202 "parser.y"
                                                              { (yyval.node) = make_leaf(A_CONTINUE, NULL, yylineno); }
#line 1975 "y.tab.c"
    break;

  case 65:
#line 203 "parser.y"
                                                              { (yyval.node) = make_binary(A_GLUE, make_ternary(A_DEF_FUNC, make_binary(A_DECL, (yyvsp[-6].node), make_leaf(A_ID, (yyvsp[-5].stringval), yylineno), yylineno), (yyvsp[-3].node), (yyvsp[-1].node), yylineno), make_unary(A_END_DEF_FUNC, NULL, yylineno), yylineno); }
#line 1981 "y.tab.c"
    break;

  case 66:
#line 204 "parser.y"
                                                              { (yyval.node) = (yyvsp[-1].node); }
#line 1987 "y.tab.c"
    break;

  case 67:
#line 205 "parser.y"
                                                              { (yyval.node) = make_binary(A_STRUCT, make_leaf(A_ID, (yyvsp[-5].stringval), yylineno), (yyvsp[-2].node), yylineno); }
#line 1993 "y.tab.c"
    break;

  case 68:
#line 208 "parser.y"
                                                  { (yyval.node) = make_binary(A_CALL_FUNC, make_leaf(A_ID, (yyvsp[-3].stringval), yylineno), (yyvsp[-1].node), yylineno); }
#line 1999 "y.tab.c"
    break;

  case 69:
#line 209 "parser.y"
                                                    { (yyval.node) = make_unary(A_SIZEOF, (yyvsp[-1].node), yylineno);}
#line 2005 "y.tab.c"
    break;

  case 70:
#line 210 "parser.y"
                                                  { (yyval.node) = make_unary(A_SIZEOF, make_leaf(A_ID, (yyvsp[-1].stringval), yylineno), yylineno);}
#line 2011 "y.tab.c"
    break;

  case 71:
#line 211 "parser.y"
                                                  { (yyval.node) = make_unary(A_PRINT, (yyvsp[-1].node), yylineno);}
#line 2017 "y.tab.c"
    break;

  case 72:
#line 212 "parser.y"
                                                  { (yyval.node) = make_binary(A_CASTABLE, (yyvsp[-3].node), (yyvsp[-1].node), yylineno); }
#line 2023 "y.tab.c"
    break;

  case 73:
#line 213 "parser.y"
                                                  { (yyval.node) = make_binary(A_CAST, (yyvsp[-3].node), (yyvsp[-1].node), yylineno);}
#line 2029 "y.tab.c"
    break;

  case 74:
#line 214 "parser.y"
                                                  { (yyval.node) = make_binary(A_ALLOCATE, (yyvsp[-3].node), (yyvsp[-1].node), yylineno);}
#line 2035 "y.tab.c"
    break;

  case 75:
#line 215 "parser.y"
                                                  { (yyval.node) = make_unary(A_ADDRESSOF, (yyvsp[-1].node), yylineno);}
#line 2041 "y.tab.c"
    break;

  case 76:
#line 216 "parser.y"
                                                  { (yyval.node) = make_binary(A_CALL_FUNC, make_binary(A_DOT, make_leaf(A_ID, (yyvsp[-5].stringval), yylineno), make_leaf(A_ID, (yyvsp[-3].stringval), yylineno), yylineno), (yyvsp[-1].node), yylineno); }
#line 2047 "y.tab.c"
    break;

  case 77:
#line 217 "parser.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 2053 "y.tab.c"
    break;

  case 78:
#line 220 "parser.y"
                                           { (yyval.node) = make_unary(A_DEREF, (yyvsp[-1].node), yylineno); }
#line 2059 "y.tab.c"
    break;

  case 79:
#line 221 "parser.y"
                                                                 { (yyval.node) = make_binary(A_CALL_FUNC, make_unary(A_DEREF, (yyvsp[-4].node), yylineno),(yyvsp[-1].node), yylineno); }
#line 2065 "y.tab.c"
    break;

  case 80:
#line 224 "parser.y"
                                        { (yyval.node) = make_binary(A_GLUE, (yyvsp[-1].node), (yyvsp[0].node), yylineno); }
#line 2071 "y.tab.c"
    break;

  case 81:
#line 225 "parser.y"
                       { (yyval.node) = make_leaf(A_EMPTY, NULL, yylineno); }
#line 2077 "y.tab.c"
    break;

  case 82:
#line 228 "parser.y"
                        { 
    

    #if DEBUG

        // Setup Timers for Debugging
        struct timeval start, end;
        float total = 0;
        float delta = 0;

        printf("/* Initializing Parsing */\n");
    
        // Start Timer
        gettimeofday(&start, NULL);
    #endif

    (yyval.node) = make_unary(A_PROGRAM, (yyval.node), yylineno);

    #if DEBUG

        // Stop Timer
        gettimeofday(&end, NULL);
        delta = timedifference_msec(start, end); total += delta;

        printf("/* Parsing Took %f milliseconds */\n", delta);

        printf("/* Parsing Complete, Printing AST */\n");
        print_ast((yyval.node), 0);
        printf("/* Generating Symbols */\n");
        int number_of_nodes = count_nodes((yyval.node));

        // Start Timer
        gettimeofday(&start, NULL);
    #endif

    expand_ast_with_symbol_tables((yyval.node), NULL);

    #if DEBUG

        // Stop Timer
        gettimeofday(&end, NULL);
        delta = timedifference_msec(start, end); total += delta;
        printf("/* Symbols Took %f milliseconds */\n", delta);

        printf("/* Printing Symbol Tables */\n");
        print_all_symbol_tables((yyval.node));
        printf("/* Typechecking AST and expanding Symbols */\n");

        // Start Timer
        gettimeofday(&start, NULL);
    #endif
    
    typecheck((yyval.node));

    #if DEBUG

        // Stop Timer
        gettimeofday(&end, NULL);
        delta = timedifference_msec(start, end); total += delta;
        printf("/* Typechecking Took %f milliseconds */\n", delta);

        printf("/* Typechecking Completed, Printing Updated Symbol Tables */\n");
        print_all_symbol_tables((yyval.node));

        printf("/* Beginning Optimization Phase */\n");


        // Start Timer
        gettimeofday(&start, NULL);

    #endif

    optimize((yyval.node));

    #if DEBUG

        // Stop Timer
        gettimeofday(&end, NULL);
        delta = timedifference_msec(start, end); total += delta;
        printf("/* Optimization Took %f milliseconds */\n", delta);

        // Calculate ast reduction from optimization
        int new_number_of_nodes = count_nodes((yyval.node));
        print_ast((yyval.node), 0);
        float diff = 100 * (number_of_nodes - new_number_of_nodes) / ((number_of_nodes+new_number_of_nodes) / 2);
        printf("Optimizer reduced AST node count by '%.3f' percent\n", diff);

    #endif


    /* Construct the Code Generation status */ 
    struct CODE_GENERATION_STATUS * status = malloc(sizeof(struct CODE_GENERATION_STATUS));
    
    /* First symbol table is the global scope */
    status->current_symbol_table = (yyval.node)->symbol_table;    

    status->function_stack_length = 0;
    status->function_stack_max_length = 16;
    status->function_stack = malloc(sizeof(struct SYMBOL_TABLE) * 16);

    status->max_num_labels = 1024;
    status->num_labels = 0;
    status->labels = malloc(sizeof(char* ) * status->max_num_labels);


    status->instructions_length = 0;
    status->max_instructions_length = 1024;
    status->instruction_array = malloc(sizeof(void *) * status->max_instructions_length);

    status->max_float_data_length = 512;
    status->float_array_length = 0;
    status->float_data_array = malloc(sizeof(void *) * status->max_float_data_length);

    status->max_string_data_length = 512;
    status->string_array_length = 0;
    status->string_data_array = malloc(sizeof(void *) * status->max_string_data_length);

    generate_code(status, (yyval.node));

    emit_asm(status, status->float_data_array);

    #if DEBUG
        printf("/* ASM Generation Completed, Cleaning Up */\n");
        printf("/* Total Parsing and Code Generation Took %f milliseconds */\n", total);
        printf("Note, all calculated times also includes #if DEBUG code\n");
    #endif
}
#line 2209 "y.tab.c"
    break;


#line 2213 "y.tab.c"

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
#line 356 "parser.y"

