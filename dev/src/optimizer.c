#include "optimizer.h"

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "symbols.h"
#include "typechecking.h"
#include "types.h"
#include <float.h>

/* Utilities */

int is_literal(struct AST_NODE *node) {

  switch (node->kind) {
  case A_INTVAL:
  case A_FLOATVAL:
  case A_CHARVAL:
  case A_POINTER:
    return 1;

  default:
    return 0;
  }
}

int are_literals(struct AST_NODE *a, struct AST_NODE *b) {
  return is_literal(a) && is_literal(b);
}

int is_unary_operator(struct AST_NODE *node) {
  switch (node->kind) {
  case A_NEG:
    return 1;

  default:
    return 0;
  }
}

int is_binary_operator(struct AST_NODE *node) {
  switch (node->kind) {
  case A_ADD:
  case A_SUB:
  case A_MUL:
  case A_DIV:
  case A_MOD:
    return 1;

  default:
    return 0;
  }
}

/* Contains:

    Constant Folding     ( Convert simple expressions such as 2 + 3 to 5 )
    Constant Propagation ( Convert variables that aren't ever reassigned into
   literals ) Intrinsic Functions  ( builtins )

*/

void fold_integers(struct AST_NODE *parent) {

  int a = parent->left->data.intval;
  int x = parent->right->data.intval;

  switch (parent->kind) {
  case A_ADD:

    if ((x < 0) && (a > INT_MAX + x) || (x > 0) && (a < INT_MIN + x)) {
      printf("Warning! Possible integer (over)- underflow in addition at line "
             "'%d'\n",
             parent->lineno);
      break;
    }

    parent->kind = A_INTVAL;
    parent->data.intval =
        parent->left->data.intval + parent->right->data.intval;
    break;

  case A_SUB:

    if ((x < 0) && (a > INT_MAX + x) || (x > 0) && (a < INT_MIN + x)) {
      printf("Warning! Possible integer (over)- underflow in subtraction at "
             "line '%d'\n",
             parent->lineno);
      break;
    }

    parent->kind = A_INTVAL;
    parent->data.intval =
        parent->left->data.intval - parent->right->data.intval;
    break;

  case A_MUL:

    if ((a == -1) && (x == INT_MIN) || (x == -1) && (a == INT_MIN) ||
        (a > INT_MAX / x) || (a < INT_MIN / x)) {
      printf("Warning! Possible integer (over)- underflow in multiplication at "
             "line '%d'\n",
             parent->lineno);
      break;
    }

    parent->kind = A_INTVAL;
    parent->data.intval =
        parent->left->data.intval * parent->right->data.intval;
    break;

  case A_DIV:
    parent->kind = A_INTVAL;
    parent->data.intval =
        parent->left->data.intval / parent->right->data.intval;
    break;

  case A_MOD:
    parent->kind = A_INTVAL;
    parent->data.intval =
        parent->left->data.intval % parent->right->data.intval;
    break;

  default:
    return 0;
  }

  return 1;
}

void fold_floats(struct AST_NODE *parent) {

  switch (parent->kind) {
  case A_ADD:
    parent->kind = A_FLOATVAL;
    parent->data.floatval =
        parent->left->data.floatval + parent->right->data.floatval;
    break;

  case A_SUB:
    parent->kind = A_FLOATVAL;
    parent->data.floatval =
        parent->left->data.floatval - parent->right->data.floatval;
    break;

  case A_MUL:
    parent->kind = A_FLOATVAL;
    parent->data.floatval =
        parent->left->data.floatval * parent->right->data.floatval;
    break;

  case A_DIV:
    parent->kind = A_FLOATVAL;
    parent->data.floatval =
        parent->left->data.floatval / parent->right->data.floatval;
    break;

  default:
    return 0;
  }

  return 1;
}

void optimize_inner(struct AST_NODE *tree, struct AST_NODE *parent) {

  if (tree == NULL) {
    return;
  }

  optimize_inner(tree->left, tree);
  optimize_inner(tree->mid, tree);
  optimize_inner(tree->right, tree);

  int did_fold = 0;
  int c;
  int d;
  struct TYPE *a;
  struct TYPE *b;
  int len = 4;
  // Constant Folding (Convert simple expressions such as 2 + 3 to 5)
  if (is_binary_operator(tree) && are_literals(tree->left, tree->right)) {

    switch (tree->left->kind) {
    case A_INTVAL:
      fold_integers(tree);
      did_fold = 1;
      break;
    case A_FLOATVAL:
      fold_floats(tree);
      did_fold = 1;
      break;

    // @NOTE : Strings have no builtin arithmetic, so we can't fold them.
    default:
      break;
    }
  }

  if (tree->kind == A_EQUAL || tree->kind == A_NEQUAL) {

    a = get_evaluation_type_of_subtree(tree->left);
    b = get_evaluation_type_of_subtree(tree->right);

    // @NOTE : This is a somewhat hacky way of converting an equality statement
    // of a complex type into a simple integer comparison. This is required
    // because of a small regression bug found late enough that
    // we felt that this solution was a better use of our time.
    if (a->pointer != NULL && a->pointer->type == A_STRUCT &&
        b->pointer != NULL & b->pointer->type == A_STRUCT) {

      c = are_equivalent_types(a, b);
      if (c == 1 && tree->kind == A_EQUAL) {
        d = 1;
      } else if (c == 0 && tree->kind == A_EQUAL) {
        d = 1;
      } else if (c == 0 && tree->kind == A_NEQUAL) {
        d = 0;
      } else {
        d = 1;
      }

      struct AST_NODE *l = make_leaf(A_INTVAL, (void *)&c, tree->lineno);
      struct AST_NODE *r = make_leaf(A_INTVAL, (void *)&d, tree->lineno);
      a = malloc(sizeof(struct TYPE));
      a->type = A_INT;
      l->type = a;
      r->type = a;

      free(tree->left);
      free(tree->right);

      tree->left = l;
      tree->right = r;
    }
  }

  // Intrinsics (builtins)
  switch (tree->kind) {
  // We can reduce castable calls at compile time since we have static
  // typechecking
  case A_CASTABLE:
    tree->kind = A_INTVAL;

    tree->data.intval = castable(get_evaluation_type_of_subtree(tree->right),
                                 get_evaluation_type_of_subtree(tree->left));
    did_fold = 1;
    break;

  // Since we've already calculated the size of any type previously, we can just
  // insert it as an intval
  case A_SIZEOF:

    b = get_evaluation_type_of_subtree(tree->left);
    did_fold = 1;

    tree->kind = A_INTVAL;
    if (b->type == A_STRUCT) {
      tree->data.intval = b->size;
    } else {
      tree->data.intval = 1;
    }
    break;

  case A_CAST:
    a = get_evaluation_type_of_subtree(tree->left);
    b = get_evaluation_type_of_subtree(tree->right);

    // We can cast literals at compile-time;
    if (is_literal(tree->right) == 1) {
      switch (a->type) {
      case A_INT:
        tree->kind = A_INTVAL;
        tree->data.intval = (int)tree->right->data.floatval;
        did_fold = 1;
        break;
      case A_FLOAT:
        tree->kind = A_FLOATVAL;
        tree->data.floatval = (float)tree->right->data.intval;
        did_fold = 1;
        break;
      default:
        break;
      }
    }

  default:
    break;
  }

  if (did_fold == 1) {
    free(tree->left);
    tree->left = NULL;
    if (tree->right != NULL) {
      free(tree->right);
      tree->right = NULL;
    }
  }
}

void optimize(struct AST_NODE *tree) { optimize_inner(tree->left, tree); }
