#pragma once

#include "tree.h"

struct TYPE {
  enum AST_NODE_KIND type;

  // ID of the type if it is a named type
  char *id;

  // Literal ID value, e.g. variable names.
  char *var_name;

  struct TYPE *next;
  struct TYPE *pointer;
  struct TYPE **fields;
  struct TYPE **params;
  int number_of_fields;
  int number_of_params;

  struct TYPE *return_type;

  int size;
  int offset;
  int array_items;
};