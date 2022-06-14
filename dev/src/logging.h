#pragma once

#include "symbols.h"
#include "tree.h"

void print_ast_node(struct AST_NODE * node);
void print_ast(struct AST_NODE * node, int indent);
void print_type(struct TYPE * type, int indent);
void print_symbol(struct SYMBOL * symbol, int indent);
void print_symbol_table(struct SYMBOL_TABLE * table, int indent);
void print_all_symbol_tables(struct AST_NODE * tree);
int count_nodes(struct AST_NODE * tree);