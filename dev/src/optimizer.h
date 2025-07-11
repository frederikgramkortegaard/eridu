#pragma once

#include "tree.h"

int is_literal(struct AST_NODE *node);
int are_literals(struct AST_NODE *a, struct AST_NODE *b);
int is_intrinsic(struct AST_NODE *node);
int is_binary_operator(struct AST_NODE *node);
void handle_intrinsics(struct AST_NODE *node);
void optimize_inner(struct AST_NODE *tree, struct AST_NODE *parent);
void optimize(struct AST_NODE *tree);