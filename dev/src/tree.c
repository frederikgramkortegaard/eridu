/* Structs and utility functions for creating and manipulating ASTs */

#include "tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "translater.h"

// Internal function for creating AST NODES
// more comprehensive than the shorthands.
struct AST_NODE *make_node(enum AST_NODE_KIND kind, struct AST_NODE *left,
                           struct AST_NODE *mid, struct AST_NODE *right,
                           void *data, int lineno) {
#if DEBUG
  printf("tree.c :: make_node :: Making node of AST_NODE_KIND '%s'\n",
         ast_node_kinds_as_strings[kind]);
#endif

  struct AST_NODE *new_node = malloc(sizeof(struct AST_NODE));

  new_node->lineno = lineno;

  new_node->kind = kind;
  new_node->type = NULL;

  new_node->left = left;
  new_node->mid = mid;
  new_node->right = right;

  switch (kind) {
  case A_INTVAL:
    new_node->data.intval = *(int *)data;
    break;
  case A_STRINGVAL:
    new_node->id = data;
    break;
  case A_FLOATVAL:
    new_node->data.floatval = *(float *)data;
    break;
  case A_CHARVAL:
    new_node->data.charval = *(char *)data;
    break;
  case A_ID:
    new_node->id = data;
  }

  return new_node;
}

/* Specialized make_node functions */
struct AST_NODE *make_leaf(enum AST_NODE_KIND kind, void *data, int lineno) {
  return make_node(kind, NULL, NULL, NULL, data, lineno);
}

struct AST_NODE *make_unary(enum AST_NODE_KIND kind, struct AST_NODE *left,
                            int lineno) {
  return make_node(kind, left, NULL, NULL, NULL, lineno);
}

struct AST_NODE *make_binary(enum AST_NODE_KIND kind, struct AST_NODE *left,
                             struct AST_NODE *right, int lineno) {
  return make_node(kind, left, NULL, right, NULL, lineno);
}

struct AST_NODE *make_ternary(enum AST_NODE_KIND kind, struct AST_NODE *left,
                              struct AST_NODE *mid, struct AST_NODE *right,
                              int lineno) {
  return make_node(kind, left, mid, right, NULL, lineno);
}
