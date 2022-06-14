/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

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

#line 127 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
