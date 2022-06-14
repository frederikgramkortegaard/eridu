#pragma once

#include "symbols.h"
#include "translater.h"
#include "tree.h"
#include "types.h"

#define MAX_BLOCK_NESTING 1024

static inline const enum AST_NODE_KIND to_int[] = {A_FLOAT};
static inline const enum AST_NODE_KIND to_float[] = {A_INT};
static inline const enum AST_NODE_KIND to_char[] = {};
static inline const enum AST_NODE_KIND to_string[] = {A_CHAR, A_FLOAT, A_INT};

int castable(struct TYPE * from, struct TYPE * to);
int is_ambigous_callable(struct TYPE * type);
int are_equivalent_types(struct TYPE * a, struct TYPE * b);
int is_deep_pointer(struct TYPE * type);
int is_arithmetic_type(struct TYPE * a);
int is_compareable(struct TYPE * a);
void typecheck_parameter_lists(struct AST_NODE * caller, struct TYPE * caller_type, struct TYPE * callee);
struct TYPE * fetch_evaluated_parameter_types_from_subtree(struct AST_NODE * node, struct TYPE * caller_type);
struct TYPE * get_evaluation_type_of_subtree(struct AST_NODE * node);
void typecheck(struct AST_NODE * tree);
