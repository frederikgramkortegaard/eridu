%{
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

%}

%union {
    int intval;
    char charval;
    float floatval;
    char * stringval;
    struct AST_NODE * node;
}

%token <stringval> ID

%token <intval> INTVAL
%token <charval> CHARVAL
%token <floatval> FLOATVAL
%token <stringval> STRINGVAL


%token INT
%token STR
%token CHAR

// Named P TYPE as to not collide with TYPE in the compiler
%token FLOAT
%token CALLABLE

%token DEFINE
%token RETURN
%token WHILE
%token BREAK
%token CONTINUE
%token IF
%token LEQ
%token GEQ
%token EQUAL
%token NEQUAL
%token STAR

%token STRUCT
%token CAST
%token PRINT
%token SIZEOF
%token CASTABLE
%token ALLOCATE
%token ADDRESSOF
%token DEREFERENCE

%right '!' LEQ GEQ '>' '<' NEQUAL EQUAL
%left '+' '-'
%left STAR '/' '%'

// Types
%type <node> typelist
%type <node> typedparameters
%type <node> typedparameterlist
%type <node> typedcallable
%type <node> type
%type <node> simpletype

%type <node> statementlist
%type <node> statement
%type <node> derefexpr
%type <node> program
%type <node> pointer
%type <node> parameters
%type <node> parameterlist
%type <node> literal
%type <node> functioncall
%type <node> expression
%type <node> block
%type <node> assignments
%type <node> assignable

%start program

%%
/* Start of Grammar rules and AST generation

Types */

simpletype : INT { $$ = make_leaf(A_INT,   NULL, yylineno); }
     | STR       { $$ = make_leaf(A_STRING,   NULL, yylineno); }
     | CHAR      { $$ = make_leaf(A_CHAR,  NULL, yylineno); }
     | FLOAT     { $$ = make_leaf(A_FLOAT, NULL, yylineno); }


typedcallable : '<'type'>'CALLABLE'<'typelist'>' { $$ = make_binary(A_CALLABLE, $2, $6, yylineno); }
              | '<'type'>'CALLABLE { $$ = make_binary(A_CALLABLE, $2, NULL, yylineno); }
              | CALLABLE { $$ = make_leaf(A_CALLABLE, NULL, yylineno); }

pointer : type STAR { $$ = make_unary(A_POINTER, $1, yylineno); }
    

type : simpletype    { $$ = $1; }
     | typedcallable { $$ = $1; }
     | pointer       { $$ = $1; }
     | STRUCT ID { $$ = make_unary(A_STRUCT, make_leaf(A_ID, $2, yylineno), yylineno); }


typelist : type               { $$ = $1; }
         | type ',' typelist  { $$ = make_binary(A_GLUE, $1, $3, yylineno); }
         | %empty             { $$ = make_leaf(A_EMPTY, NULL, yylineno);    }

/* Parameters */ 

parameters : expression                           { $$ = make_unary(A_PARAM, $1, yylineno); }
           | expression ',' parameters            { $$ = make_binary(A_GLUE, make_unary(A_PARAM, $1, yylineno), $3, yylineno);}
           | %empty                               { $$ = make_leaf(A_EMPTY, NULL, yylineno);}


parameterlist : parameters                        {$$ = make_unary(A_PARAM_LIST, $1, yylineno);}


typedparameters : type expression                 { $$ = make_binary(A_PARAM, $1, $2, yylineno);}
           | type expression ',' typedparameters  { $$ = make_binary(A_GLUE, make_binary(A_PARAM, $1, $2, yylineno), $4, yylineno);}
           | %empty                               { $$ = make_leaf(A_EMPTY, NULL, yylineno);}


typedparameterlist : typedparameters         {$$ = make_unary(A_PARAM_LIST, $1, yylineno);}


literal : INTVAL         { $$ = make_leaf(A_INTVAL,   &$1, yylineno); }
        | FLOATVAL       { $$ = make_leaf(A_FLOATVAL, &$1, yylineno); }
        | CHARVAL        { $$ = make_leaf(A_CHARVAL,  &$1, yylineno); }
        | ID             { $$ = make_leaf(A_ID,        $1, yylineno); }
        | STRINGVAL      { $$ = make_leaf(A_STRINGVAL, $1, yylineno); }
        | ID '[' expression ']' { $$ = make_binary(A_INDEX, make_leaf(A_ID, $1, yylineno), $3, yylineno); }
        | ID '.' ID      { $$ = make_binary(A_DOT, make_leaf(A_ID, $1, yylineno), make_leaf(A_ID, $3, yylineno), yylineno); }


expression : literal                       { $$ = $1; }
           | type                          { $$ = $1; }
           | functioncall                  { $$ = $1; }
           | expression STAR expression    { $$ = make_binary(A_MUL,    $1, $3, yylineno); }
           | expression NEQUAL expression  { $$ = make_binary(A_NEQUAL, $1, $3, yylineno); }
           | expression LEQ expression     { $$ = make_binary(A_LEQ,    $1, $3, yylineno); }
           | expression GEQ expression     { $$ = make_binary(A_GEQ,    $1, $3, yylineno); }
           | expression EQUAL expression   { $$ = make_binary(A_EQUAL,  $1, $3, yylineno); }
           | expression '>' expression     { $$ = make_binary(A_GREAT,  $1, $3, yylineno); }
           | expression '<' expression     { $$ = make_binary(A_LESS,   $1, $3, yylineno); }
           | expression '+' expression     { $$ = make_binary(A_ADD,    $1, $3, yylineno); }
           | expression '%' expression     { $$ = make_binary(A_MOD,    $1, $3, yylineno); }
           | expression '/' expression     { $$ = make_binary(A_DIV,    $1, $3, yylineno); }
           | expression '-' expression     { $$ = make_binary(A_SUB,    $1, $3, yylineno); }
           | '!' expression                { $$ = make_unary(A_NEG,     $2, yylineno);     } 
           | %empty                        { $$ = make_leaf(A_EMPTY, NULL, yylineno);      }


block : '{' statementlist '}'                   { $$ = make_unary(A_BLOCK, $2, yylineno); }


assignable : ID                                 { $$ = make_leaf(A_ID, $1, yylineno); }
           | derefexpr                          { $$ = $1;}

           | ID '[' expression ']'              { $$ = make_binary(A_INDEX, make_leaf(A_ID, $1, yylineno), $3, yylineno); }
           | ID '.' ID                          { $$ = make_binary(A_DOT, make_leaf(A_ID, $1, yylineno), make_leaf(A_ID, $3, yylineno), yylineno); }
           | ID '.' ID '[' expression ']'       { $$ = make_binary(A_INDEX, make_binary(A_DOT, make_leaf(A_ID, $1, yylineno), make_leaf(A_ID, $3, yylineno), yylineno), $5, yylineno); }


assignments : type ID '=' expression            { $$ = make_binary(A_GLUE, make_binary(A_DECL, $1, make_leaf(A_ID, $2, yylineno), yylineno), make_binary(A_ASSIGN, make_leaf(A_ID, $2, yylineno), $4, yylineno), yylineno); }
            | assignable STAR '=' expression    { $$ = make_binary(A_ASSIGN, $1, make_binary(A_MUL, $1, $4, yylineno), yylineno); }
            | assignable '=' expression         { $$ = make_binary(A_ASSIGN, $1, $3, yylineno);                                   }
            
            

statement : type ID ';'                                       { $$ = make_binary(A_DECL, $1, make_leaf(A_ID, $2, yylineno), yylineno);} 
        | WHILE '(' expression ')' '{' statementlist '}' ';'  { $$ =  make_binary(A_GLUE, make_binary(A_WHILE, $3, $6, yylineno), make_unary(A_END_WHILE, NULL, yylineno), yylineno); }
        | RETURN expression ';'                               { $$ = make_unary(A_RETURN, $2, yylineno);}
        | IF '(' expression ')' '{' statementlist '}' ';'     { $$ = make_binary(A_IF, $3, $6, yylineno); }
        | assignments  ';'                                    { $$ = $1; }
        | expression ';'                                      { $$ = $1; }
        | BREAK ';'                                           { $$ = make_leaf(A_BREAK, NULL, yylineno); }
        | CONTINUE ';'                                        { $$ = make_leaf(A_CONTINUE, NULL, yylineno); }
        | DEFINE type ID '(' typedparameterlist ')' block ';' { $$ = make_binary(A_GLUE, make_ternary(A_DEF_FUNC, make_binary(A_DECL, $2, make_leaf(A_ID, $3, yylineno), yylineno), $5, $7, yylineno), make_unary(A_END_DEF_FUNC, NULL, yylineno), yylineno); }
        | block ';'                                           { $$ = $1; }
        | STRUCT ID '=' '{' typedparameterlist '}' ';'        { $$ = make_binary(A_STRUCT, make_leaf(A_ID, $2, yylineno), $5, yylineno); }


functioncall : ID '(' parameterlist ')'           { $$ = make_binary(A_CALL_FUNC, make_leaf(A_ID, $1, yylineno), $3, yylineno); }
           | SIZEOF '(' type ')'                    { $$ = make_unary(A_SIZEOF, $3, yylineno);}
           | SIZEOF '(' ID ')'                    { $$ = make_unary(A_SIZEOF, make_leaf(A_ID, $3, yylineno), yylineno);}
           | PRINT '(' expression ')'             { $$ = make_unary(A_PRINT, $3, yylineno);}
           | CASTABLE '(' type ',' expression ')' { $$ = make_binary(A_CASTABLE, $3, $5, yylineno); }
           | CAST '(' type ',' expression ')'     { $$ = make_binary(A_CAST, $3, $5, yylineno);}
           | ALLOCATE '(' type ',' expression ')' { $$ = make_binary(A_ALLOCATE, $3, $5, yylineno);}
           | ADDRESSOF '(' expression ')'         { $$ = make_unary(A_ADDRESSOF, $3, yylineno);}
           | ID '.' ID '(' parameterlist ')'      { $$ = make_binary(A_CALL_FUNC, make_binary(A_DOT, make_leaf(A_ID, $1, yylineno), make_leaf(A_ID, $3, yylineno), yylineno), $5, yylineno); }
           | derefexpr { $$ = $1; }


derefexpr : DEREFERENCE '(' expression ')' { $$ = make_unary(A_DEREF, $3, yylineno); }
          | DEREFERENCE '(' expression ')' '(' parameterlist ')' { $$ = make_binary(A_CALL_FUNC, make_unary(A_DEREF, $3, yylineno),$6, yylineno); }


statementlist : statement statementlist { $$ = make_binary(A_GLUE, $1, $2, yylineno); }
              | %empty { $$ = make_leaf(A_EMPTY, NULL, yylineno); }    


program : statementlist { 
    

    #if DEBUG

        // Setup Timers for Debugging
        struct timeval start, end;
        float total = 0;
        float delta = 0;

        printf("/* Initializing Parsing */\n");
    
        // Start Timer
        gettimeofday(&start, NULL);
    #endif

    $$ = make_unary(A_PROGRAM, $$, yylineno);

    #if DEBUG

        // Stop Timer
        gettimeofday(&end, NULL);
        delta = timedifference_msec(start, end); total += delta;

        printf("/* Parsing Took %f milliseconds */\n", delta);

        printf("/* Parsing Complete, Printing AST */\n");
        print_ast($$, 0);
        printf("/* Generating Symbols */\n");
        int number_of_nodes = count_nodes($$);

        // Start Timer
        gettimeofday(&start, NULL);
    #endif

    expand_ast_with_symbol_tables($$, NULL);

    #if DEBUG

        // Stop Timer
        gettimeofday(&end, NULL);
        delta = timedifference_msec(start, end); total += delta;
        printf("/* Symbols Took %f milliseconds */\n", delta);

        printf("/* Printing Symbol Tables */\n");
        print_all_symbol_tables($$);
        printf("/* Typechecking AST and expanding Symbols */\n");

        // Start Timer
        gettimeofday(&start, NULL);
    #endif
    
    typecheck($$);

    #if DEBUG

        // Stop Timer
        gettimeofday(&end, NULL);
        delta = timedifference_msec(start, end); total += delta;
        printf("/* Typechecking Took %f milliseconds */\n", delta);

        printf("/* Typechecking Completed, Printing Updated Symbol Tables */\n");
        print_all_symbol_tables($$);

        printf("/* Beginning Optimization Phase */\n");


        // Start Timer
        gettimeofday(&start, NULL);

    #endif

    optimize($$);

    #if DEBUG

        // Stop Timer
        gettimeofday(&end, NULL);
        delta = timedifference_msec(start, end); total += delta;
        printf("/* Optimization Took %f milliseconds */\n", delta);

        // Calculate ast reduction from optimization
        int new_number_of_nodes = count_nodes($$);
        print_ast($$, 0);
        float diff = 100 * (number_of_nodes - new_number_of_nodes) / ((number_of_nodes+new_number_of_nodes) / 2);
        printf("Optimizer reduced AST node count by '%.3f' percent\n", diff);

    #endif


    /* Construct the Code Generation status */ 
    struct CODE_GENERATION_STATUS * status = malloc(sizeof(struct CODE_GENERATION_STATUS));
    
    /* First symbol table is the global scope */
    status->current_symbol_table = $$->symbol_table;    

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

    generate_code(status, $$);

    emit_asm(status, status->float_data_array);

    #if DEBUG
        printf("/* ASM Generation Completed, Cleaning Up */\n");
        printf("/* Total Parsing and Code Generation Took %f milliseconds */\n", total);
        printf("Note, all calculated times also includes #if DEBUG code\n");
    #endif
}

%%
