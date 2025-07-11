#pragma once

enum AST_NODE_KIND {
  A_TEST,
  A_ADD,
  A_ASSIGN,
  A_ID,
  A_INTVAL,
  A_PROGRAM,
  A_PARAM_LIST,
  A_RETURN,
  A_DECL,
  A_GLUE,
  A_DEF_FUNC,
  A_BLOCK,
  A_CALL_FUNC,
  A_EXPRESSION,
  A_EMPTY,
  A_WHILE,
  A_IF,
  A_LESS,
  A_GREAT,
  A_LEQ,
  A_GEQ,
  A_NEG,
  A_EQUAL,
  A_NEQUAL,
  A_SUB,
  A_PRINT,
  A_FLOATVAL,
  A_CHARVAL,
  A_CHAR,
  A_FLOAT,
  A_STRINGVAL,
  A_STR,
  A_CALLABLE,
  A_PARAM,
  A_CAST,
  A_CASTABLE,
  A_SIZEOF,
  A_ALLOCATE,
  A_INT,
  A_MOD,
  A_DIV,
  A_MUL,
  A_ADDRESSOF,
  A_END_DEF_FUNC,
  A_POINTER,
  A_BREAK,
  A_CONTINUE,
  A_STRUCT,
  A_END_WHILE,
  A_STRING,
  A_DEREF,
  A_INDEX,
  A_DOT,
};

typedef struct AST_NODE {
  char *id;

  struct TYPE *type;
  union data {
    int intval;
    float floatval;
    char charval;
  } data;

  enum AST_NODE_KIND kind;

  struct AST_NODE *left;
  struct AST_NODE *mid;
  struct AST_NODE *right;

  int lineno;

  struct SYMBOL_TABLE *symbol_table;

} AST_NODE;

struct AST_NODE *make_node(enum AST_NODE_KIND kind, struct AST_NODE *left,
                           struct AST_NODE *mid, struct AST_NODE *right,
                           void *data, int lineno);
struct AST_NODE *make_leaf(enum AST_NODE_KIND kind, void *data, int lineno);
struct AST_NODE *make_unary(enum AST_NODE_KIND kind, struct AST_NODE *left,
                            int lineno);
struct AST_NODE *make_binary(enum AST_NODE_KIND kind, struct AST_NODE *left,
                             struct AST_NODE *right, int lineno);
struct AST_NODE *make_ternary(enum AST_NODE_KIND kind, struct AST_NODE *left,
                              struct AST_NODE *mid, struct AST_NODE *right,
                              int lineno);
