/* Pretty print utilities for the various phases and structs of the compiler,
 * this code is not meant to be pretty, rather it has a purely functional
 * purpose. */

#include "logging.h"
#include "translater.h"
#include <stdio.h>
#include <stdlib.h>

// Eliminates infinite recursion
#define MAX_RECURSION_LEVEL 3

void print_ast_node(struct AST_NODE *node) {
  if (node == NULL)
    return;

  printf("node->kind: %s, ", ast_node_kinds_as_strings[node->kind]);
  printf("node->type:\n");
  print_type(node->type, 0);
  printf("node->line: %d, ", node->lineno);
  switch (node->kind) {
  case A_INTVAL:
    printf("node->data: %d", node->data.intval);
    break;
  case A_CHARVAL:
    printf("node->data: '%c'", node->data.charval);
    break;
  case A_FLOATVAL:
    printf("node->data: %f", node->data.floatval);
    break;
  case A_STRINGVAL:
  case A_ID:
    printf("node->data: %s", node->id);
    break;
  }

  printf("\n");
}

void print_ast(struct AST_NODE *node, int indent) {
  if (node == NULL)
    return;

  for (int i = 0; i < indent; i++)
    printf(" | ");

  printf("%s\n", ast_node_kinds_as_strings[node->kind]);

  print_ast(node->left, indent + 1);
  print_ast(node->mid, indent + 1);
  print_ast(node->right, indent + 1);
}

void print_type(struct TYPE *type, int indent) {
  if (indent > 3) {
    return;
  }

  if (type == NULL) {
    return;
  }

  for (int i = 0; i < indent; i++) {
    printf("\t");
  }

  printf("type->type: %s\n", ast_node_kinds_as_strings[type->type]);

  if (type->id != NULL) {
    for (int i = 0; i < indent; i++) {
      printf("\t");
    }

    printf("type->id: %s\n", type->id);
  }

  if (type->var_name != NULL) {
    for (int i = 0; i < indent; i++) {
      printf("\t");
    }

    printf("type->var_name %s\n", type->var_name);
  }

  if (type->return_type != NULL) {
    for (int i = 0; i < indent; i++) {
      printf("\t");
    }

    printf("type->return_type:\n");
    print_type(type->return_type, indent + 1);
  }

  if (type->next != NULL) {
    for (int i = 0; i < indent; i++) {
      printf("\t");
    }

    printf("type->next:\n");
    print_type(type->next, indent + 1);
  }

  if (type->number_of_params > 0) {
    for (int i = 0; i < indent; i++) {
      printf("\t");
    }
    printf("type->number_of_params: %d\n", type->number_of_params);

    for (int i = 0; i < indent; i++) {
      printf("\t");
    }
    printf("type->params:\n");

    for (int i = 0; i < type->number_of_params; i++) {
      print_type(type->params[i], indent + 1);
    }
  }

  if (type->number_of_fields > 0) {
    for (int i = 0; i < indent; i++) {
      printf("\t");
    }
    printf("type->number_of_fields: %d\n", type->number_of_fields);

    for (int i = 0; i < indent; i++) {
      printf("\t");
    }
    printf("type->fields:\n");

    for (int i = 0; i < type->number_of_fields; i++) {
      print_type(type->fields[i], indent + 1);
    }
  }

  if (type->pointer != NULL) {
    for (int i = 0; i < indent; i++) {
      printf("\t");
    }

    printf("type->pointer:\n");
    print_type(type->pointer, indent + 1);
  }
}

void print_symbol(struct SYMBOL *symbol, int indent) {
  if (symbol == NULL) {
    return;
  }

  if (symbol->id != NULL) {
    for (int i = 0; i < indent; i++) {
      printf("\t");
    }
    printf("symbol->id: %s\n", symbol->id);
  }

  if (symbol->category != NULL) {
    for (int i = 0; i < indent; i++) {
      printf("\t");
    }
    printf("symbol->category: %s\n",
           symbol_catagories_as_strings[symbol->category]);
  }

  if (symbol->type != NULL) {
    for (int i = 0; i < indent; i++) {
      printf("\t");
    }
    printf("symbol->type:\n");
    print_type(symbol->type, indent + 1);
  }

  printf("\n");
}

void print_symbol_table(struct SYMBOL_TABLE *table, int indent) {
  if (table == NULL) {
    return;
  }

  for (int i = 0; i < indent; i++) {
    printf("\t");
  }
  printf("table->id: %d\n", table->id);

  for (int i = 0; i < indent; i++) {
    printf("\t");
  }
  printf("table->number_of_variables: %d\n", table->number_of_variables);

  for (int i = 0; i < indent; i++) {
    printf("\t");
  }
  printf("table->number_of_parameters: %d\n\n", table->number_of_parameters);

  for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
    if (table->symbols[i] == NULL) {
      continue;
    }
    print_symbol(table->symbols[i], indent + 1);
  }

  printf("\n");
}

void print_all_symbol_tables(struct AST_NODE *tree) {
  if (tree == NULL) {
    return;
  }

  if (tree->kind == A_BLOCK || tree->kind == A_PROGRAM) {
    print_symbol_table(tree->symbol_table, 0);
  }

  print_all_symbol_tables(tree->left);
  print_all_symbol_tables(tree->mid);
  print_all_symbol_tables(tree->right);
}

int count_nodes(struct AST_NODE *tree) {
  if (tree == NULL) {
    return 0;
  }

  int nodes = 1 + count_nodes(tree->left) + count_nodes(tree->mid) +
              count_nodes(tree->right);
  return nodes;
}