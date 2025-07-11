#pragma once

#include "tree.h"
#include "types.h"

#define SYMBOL_TABLE_SIZE 512
#define LEAF_NODE_ARRAY_SIZE 512

enum SYMBOL_CATEGORY { S_VAR, S_FUNC, S_PARAM };

typedef struct SYMBOL {
  struct SYMBOL *next;
  char *id;
  enum SYMBOL_CATEGORY category;
  struct TYPE *type;
  char *function_label;
} SYMBOL;

typedef struct SYMBOL_TABLE {
  int id;
  struct SYMBOL_TABLE *parent;
  struct SYMBOL **symbols;
  int symbol_table_size;

  int number_of_variables;
  int number_of_parameters;

} SYMBOL_TABLE;

struct TYPE *struct_has_member(struct TYPE *instruct, char *member);
void push_param_to_type_params(struct TYPE *type, struct TYPE *param);
struct TYPE *eat_param(struct AST_NODE *node);
void fetch_params_from_subtree(struct AST_NODE *node, struct TYPE *type);
void fetch_callable_signature_from_subtree(struct AST_NODE *node,
                                           struct TYPE *type);
int count_num_params(struct AST_NODE *node);
struct TYPE *fetch_type_from_subtree(struct AST_NODE *node);
SYMBOL_TABLE *make_table(SYMBOL_TABLE *parent);
SYMBOL *make_symbol(char *id, struct TYPE *type, enum SYMBOL_CATEGORY category);
int get_field_offset_in_struct(char *struct_id, char *id,
                               struct SYMBOL_TABLE *table);
int get_symbol_offset_in_stackframe(char *id, struct SYMBOL_TABLE *table);
struct SYMBOL *lookup_in_scope(char *id, SYMBOL_TABLE *table);
struct SYMBOL_TABLE *get_symbol_table_from_id(char *id, SYMBOL_TABLE *table);
struct SYMBOL *lookup(char *id, struct SYMBOL_TABLE *table);
int insert(char *id, struct TYPE *type, enum SYMBOL_CATEGORY category,
           SYMBOL_TABLE *table);
void push_member_to_field(struct TYPE *member, struct TYPE *to);
void fill_symbol_tables(struct AST_NODE *node, struct SYMBOL_TABLE *table);
void fill_struct_type(struct AST_NODE *node, struct TYPE *type);
int contains_return(struct AST_NODE *node);
int expand_ast_with_symbol_tables(struct AST_NODE *tree,
                                  struct SYMBOL_TABLE *parent);
int contains_return(struct AST_NODE *node);