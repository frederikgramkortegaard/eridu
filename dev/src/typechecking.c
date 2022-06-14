#include "typechecking.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbols.h"

struct AST_NODE * root;

int castable(struct TYPE * from, struct TYPE * to) {


    if (from == NULL || to == NULL) {
        return 0;
    }

    switch (to->type) {
        case A_INT:
            if(from->type == A_FLOAT) {
                return 1;
            };
            return 0;
        case A_CHAR:
            return 0;
        case A_FLOAT:
            if(from->type == A_INT) {
                return 1;
            };
            return 0;
        default: return 0;
    }

    // @NOTE : Structures can't be cast to be another struct, even if they're 100% equivalent

    return 0;
}


int is_ambigous_callable(struct TYPE * type) {
    if (type->return_type == NULL) {
        return 1;
    }

    return 0;
}

int are_equivalent_types(struct TYPE * a, struct TYPE * b) {
    if (a == NULL && b == NULL) {
        return 1;

        // Should never happen, but just in case
    } else if (a == NULL || b == NULL) {
        return 0;
    }


    // A_CALLABLE Equivalence, this is similar to
    // typecheck_parameter_lists - but is intented as a utility rather than
    // a part of the typechecking process, meaning this function doesn't
    // print error messages or exit the program.
    if (a->type == A_CALLABLE && b->type == A_CALLABLE) {
        // Return Types are not equivalent

        // Left Hand side doesn't care about the signature of a callable
        if (is_ambigous_callable(a)) {
            return 1;
        }

        // Return types' checking
        if (are_equivalent_types(a->return_type, b->return_type) == 0) {
            return 0;
        }

        // Parameter Count Mismatch
        if (a->number_of_params != b->number_of_params) {
            return 0;
        }

        // Parameter Types are not equivalent
        for (int i = 0; i < a->number_of_params; i++) {
            if (are_equivalent_types(a->params[i], b->params[i]) == 0) {
                return 0;
            }
        }
        return 1;
    }

    // Struct Equivalance
    if (a->type == A_STRUCT && b->type == A_STRUCT) {


        // Struct names are not the same
        if (strcmp(a->id, b->id) != 0) {
            return 0;
        }

        /* The following is not neccesary as structures can not
        have different member types if they have the same structural
        name. However, if one wanted to forego named equivalence and instead
        utilize a form of structural equivalence, it could be
        achieved by removing the comparison above this comment.*/

        // Member counts are not the same
        if (a->number_of_fields != b->number_of_fields) {
            return 0;
        }

        if (a->number_of_fields > 0) {
            for (int i = 0; i < a->number_of_fields; i++) {

                // Member names are not equivalent
                if (strcmp(a->fields[i]->var_name, b->fields[i]->var_name) != 0) {
                    return 0;
                }

                // Member Types are not equivalent
                if (a->fields[i]->type != b->fields[i]->type) {
                    return 0;
                }
            }
        }
        return 1;
    }

    // Pointer Equivalence
    if (a->type == A_POINTER && b->type == A_POINTER) {
        return are_equivalent_types(a->pointer, b->pointer);
    }

    // Primitive Types are equivalent
    if (a->type == b->type) {
        return 1;

        // Str and char * are aliases
    } else if ((a->type == A_STRING && is_char_pointer(b)) || (is_char_pointer(a) && b->type == A_STRING)) {
        return 1;
    }

    return 0;
}

// Deep pointers are e.g. int ** arr, signifying a dynamic array.
int is_deep_pointer(struct TYPE * type) {
    if (type->type == A_POINTER && type->pointer != NULL) {
        return 1;
    }

    return 0;
}

int is_char_pointer(struct TYPE * type) {
    if (type->type == A_POINTER && type->pointer != NULL && type->pointer->type == A_CHAR) {
        return 1;
    }

    return 0;
}


// Types in which builtin arithmetic operations such as '+' and '-' can be
// performed
int is_arithmetic_type(struct TYPE * a) {
    switch (a->type) {
        case A_INT:
        case A_FLOAT:

        // @NOTE : Pointer arithmatic does NOT increment in the way you'd think. adding 1 to a pointer increases it by a literal
        // one, and not by an address
        case A_POINTER:
            return 1;

        default:
            return 0;
    }
}

// Types where the builtin '<', '>', '<=', '>=' etc. can be performed
int is_compareable(struct TYPE * a) {
    // Double pointers are essentially arrays, which are not comparable in and of themselves,
    // except for '==' and '!=' which are handled by the caller of this function.
    if (is_deep_pointer(a)) {
        return 0;
    }

    switch (a->type) {
        case A_INT:
        case A_FLOAT:
        case A_CHAR:
        case A_STRING:
        case A_POINTER:
            return 1;

        default:
            return 0;
    }
}


// Given a A_CALL_FUNC AST_NODE, we can find the function definition that it is
// calling and ensure that the order, types, and number of arguments are correct
void typecheck_parameter_lists(struct AST_NODE * caller, struct TYPE * caller_type, struct TYPE * callee) {
    if (caller_type == NULL && callee == NULL) {
        return;

        // For untyped callables we infer the return type from the LHS and let the programmer worry about the safety
    } else if (is_ambigous_callable(callee)) {
        return;
    }

    // Check that the number of parameters are the same
    if (caller_type->number_of_params != callee->number_of_params) {
        fprintf(stderr,
                "types.c :: typecheck_parameter_lists :: Function call '%s' "
                "expects '%d' arguments but was given '%d' at  at line '%d'\n",
                caller_type->id, callee->number_of_params, caller_type->number_of_params, caller->lineno);
        exit(-1);
    }

    struct TYPE * a;
    struct TYPE * b;

    for (int i = 0; i < caller_type->number_of_params; i++) {
        a = caller_type->params[i];
        b = callee->params[i];

        // Parsing Error wrt. parameter types in function calls
        if (a == NULL) {
            fprintf(stderr,
                    "types.c :: typecheck_parameter_lists :: Compiler-error, function "
                    "call '%s' is supposed to have '%d' parameters but its TYPE struct "
                    "has NULL values before that at line '%d' - this is a bug\n",
                    caller_type->id, caller_type->number_of_params, i, caller->lineno);
            exit(-1);
        }

        // Error messages for callables
        if (are_equivalent_types(a, b) == 0) {
            if (a->type == A_CALLABLE) {
                fprintf(stderr,
                        "types.c :: typecheck_parameter_lists :: Function call '%s' was given a CALLABLE argument with "
                        "signature:\n",
                        caller_type->id);

                print_type(a, 1);
                fprintf(stderr, "where the signature of the function being called is:\n");
                print_type(b, 1);
                fprintf(stderr, "at line '%d'\n", caller->lineno);

                // Error messages for non-complex types

            } else if (a->pointer != NULL && a->pointer->type == A_STRUCT) {
                fprintf(stderr,
                        "types.c :: typecheck_parameter_lists :: Function call '%s' expected a struct of type %s with ID %s, "
                        "but was given type:\n",
                        caller_type->id, ast_node_kinds_as_strings[a->pointer->type], a->pointer->id);

                if (b->pointer != NULL && b->pointer->type == A_STRUCT) {
                    print_type(b->pointer, 1);
                } else {
                    print_type(b, 1);
                }

                fprintf("at line '%d'\n", caller->lineno);

            } else {
                fprintf(stderr,
                        "types.c :: typecheck_parameter_lists :: Function call '%s' was "
                        "given argument of type '%s' where type '%s' was expected at "
                        "line '%d'\n",
                        caller_type->id, ast_node_kinds_as_strings[a->type], ast_node_kinds_as_strings[b->type],
                        caller->lineno);
            }
            exit(-1);
        }
    }
}

// Given an A_PARAM_LIST, we traverse the parameters and at each parameter (leaf
// node) we get the type that the parameter evaluates to (e.g. an expression
// could evaluate to an int), and push it to the given struct TYPE *s params
// array
struct TYPE * fetch_evaluated_parameter_types_from_subtree(struct AST_NODE * node, struct TYPE * caller_type) {
    if (node == NULL) {
        return NULL;
    }

    // Evaluate the return type of the left subtree, push it, and continue
    // recursing on the RHS
    if (node->kind == A_GLUE) {
        struct TYPE * type = get_evaluation_type_of_subtree(node->left->left);
        push_param_to_type_params(caller_type, type);
        fetch_evaluated_parameter_types_from_subtree(node->right, caller_type);
    }

    // Recursion Entrpoint
    if (node->kind == A_PARAM_LIST) {
        fetch_evaluated_parameter_types_from_subtree(node->left, caller_type);
    }

    // Leaf node, push its type to the caller_types's params
    if (node->kind == A_PARAM) {
        struct TYPE * type = get_evaluation_type_of_subtree(node->left);
        push_param_to_type_params(caller_type, type);
    }
}

/* Checks the compatabilty of operators by recursively
fetching the returned type from its respective sub-trees */
struct TYPE * get_evaluation_type_of_subtree(struct AST_NODE * node) {
    static AST_NODE * function_stack[MAX_BLOCK_NESTING];
    static AST_NODE * loop_stack[MAX_BLOCK_NESTING];
    static int function_stack_cursor = 0;
    static int loop_stack_cursor = 0;

    if (node == NULL) {
        return;
    }

    struct TYPE * a = NULL;
    struct TYPE * b = NULL;
    struct SYMBOL * callback = NULL;
    struct AST_NODE * c = NULL;

    switch (node->kind) {
        // Binary operators recursively call this method on their left and right hand
        // sides
        case A_ADD:
        case A_SUB:
        case A_DIV:
        case A_MUL:
        case A_MOD:


            a = get_evaluation_type_of_subtree(node->left);
            b = get_evaluation_type_of_subtree(node->right);

            if (node->kind == A_MOD && a->type == A_FLOAT) {
                fprintf(stderr,
                        "types.c :: get_evaluation_type_of_subtree :: Modulo operator "
                        "cannot be used with floating point numbers at line '%d'\n",
                        node->lineno);
                exit(-1);
            }


            // 'b' will be null if the right hand side is an untyped function call
            // and in that case, we just accept it and infer' the return type of
            // the callable to be that of the left-hand side, and let the programmer worry about the rest
            if (b == NULL) {
                b = a;

                printf("WARNING! Infering return type '%s' from untyped callable at line '%d'\n",
                       ast_node_kinds_as_strings[a->type], node->lineno);
            }

            // Cannot perform Arithmetic on the given types.
            if (are_equivalent_types(a, b) == 0 || is_arithmetic_type(a) == 0) {
                fprintf(stderr,
                        "types.c :: get_evaluation_type_of_subtree :: Incompatible types "
                        "('%s', '%s') given to operator '%s' at line '%d'\n",
                        ast_node_kinds_as_strings[a->type], ast_node_kinds_as_strings[b->type],
                        ast_node_kinds_as_strings[node->kind], node->lineno);
                exit(-1);
            }

            return a;

        case A_GEQ:
        case A_LEQ:
        case A_NEG:
        case A_EQUAL:
        case A_NEQUAL:
        case A_LESS:
        case A_GREAT:
            a = get_evaluation_type_of_subtree(node->left);
            b = get_evaluation_type_of_subtree(node->right);


            if(a->pointer != NULL && a->pointer->type == A_STRUCT && b->pointer != NULL & b->pointer->type == A_STRUCT) {
                return a;
            }

            if (are_equivalent_types(a, b) == 0 ||
                (is_compareable(a) == 0 && node->kind != A_EQUAL && node->kind != A_NEQUAL)) {


                fprintf(stderr,
                        "types.c :: get_evaluation_type_of_subtree :: Incompatible types "
                        "('%s', '%s') given to operator '%s' at line '%d'\n",
                        ast_node_kinds_as_strings[a->type], ast_node_kinds_as_strings[b->type],
                        ast_node_kinds_as_strings[node->kind], node->lineno);
                exit(-1);
            }

            // Comparisons always return either 0 or 1 as an integer
            return a;

        case A_ASSIGN:

            a = get_evaluation_type_of_subtree(node->left);
            b = get_evaluation_type_of_subtree(node->right);

            if (node->right->kind == A_DEREF) {
                if (a->type == A_CALLABLE && is_ambigous_callable(b)) {
                    return a;
                }
            }


            // 'b' will be null if the right hand side is an untyped function call
            // and in that case, we just accept it and infer' the return type of
            // the callable to be that of the left-hand side, and let the programmer worry about the rest
            if (b == NULL) {
                b = a;
                printf("WARNING! Infering return type '%s' from untyped callable at line '%d'\n",
                       ast_node_kinds_as_strings[a->type], node->lineno);
            }


            if (are_equivalent_types(a, b) == 0) {
                fprintf(stderr,
                        "types.c :: get_evaluation_type_of_subtree :: Tried to assign value "
                        "of type:\n\n");

                print_type(b, 0);
                fprintf(stderr, "\n\nto variable of type:\n\n");
                print_type(a, 0);
                fprintf(stderr, "type at line '%d'\n", node->lineno);

                if (node->right->kind == A_ALLOCATE) {
                    fprintf(stderr,
                            "Remember, allocate returns a pointer to the type given as the first parameter at line '%d'\n",
                            node->lineno);
                }

                fprintf(stderr,
                        "Remember, if you're working with pointer pointers the nesting will not be show in the output of the "
                        "expected types\n");
                exit(-1);
            }

            return a;

        // Find an id in the nearest scope and return the type it was declared as.
        case A_ID:

            callback = lookup(node->id, node->symbol_table);
            if (callback == NULL) {
                fprintf(stderr,
                        "types.c :: get_evaluation_type_of_subtree :: ID '%s' could not "
                        "be found in any accesable scope at line '%d'\n",
                        node->id, node->lineno);
                exit(-1);
            }

            return callback->type;

        // Find the function being called in the nearest scope and get its expected
        // return type. Here we also typecheck all given parameters
        // @WARN : Calling a callable that is not actually aliasing a properly defined function is Undefined Behavior
        case A_CALL_FUNC:

            callback = lookup(node->left->id, node->symbol_table);

            if (callback == NULL) {
                fprintf(stderr,
                        "types.c :: get_evaluation_type_of_subtree :: ID '%s' could not "
                        "be found in any accessable scope at line '%d'\n",
                        node->left->id, node->lineno);
                exit(-1);
            }

            if (callback->category != S_FUNC && callback->type->type != A_CALLABLE) {
                fprintf(stderr,
                        "types.c :: get_evaluation_type_of_subtree :: ID '%s' is not a "
                        "function at line '%d'\n",
                        node->left->id, node->lineno);
                exit(-1);
            }


            // Fetch the parameters given to the callable
            struct TYPE * caller_type = malloc(sizeof(struct TYPE));
            caller_type->number_of_params = 0;
            fetch_evaluated_parameter_types_from_subtree(node->right, caller_type);

            // Update the return type to contain information about the caller as well
            caller_type->type = node->kind;
            caller_type->id = malloc(strlen(node->left->id) + 1);
            strcpy(caller_type->id, node->left->id);

            // Check if the given parameters match the expected parameters as defined in the respective A_DEF_FUNC node
            typecheck_parameter_lists(node, caller_type, callback->type);

            return callback->type->return_type;

        // Get the left-most parameters of the 'cast' function, representing the
        // literal type it's converting a given expression into.
        case A_CAST:

            if (castable(get_evaluation_type_of_subtree(node->right), get_evaluation_type_of_subtree(node->left)) != 1) {
                fprintf(stderr,
                        "types.c :: get_evaluation_type_of_subtree :: Illegal Cast - tried "
                        "to cast from type '%s' to type '%s' at line '%d'\n",
                        ast_node_kinds_as_strings[get_evaluation_type_of_subtree(node->right)->type],
                        ast_node_kinds_as_strings[get_evaluation_type_of_subtree(node->left)->type], node->lineno);
                exit(-1);
            }
            return get_evaluation_type_of_subtree(node->left);

        // Theese builtins always returns a 0 or 1, and thus is always an int
        case A_ALLOCATE:

            a = fetch_type_from_subtree(node->left);
            b = get_evaluation_type_of_subtree(node->right);

            if (b->type != A_INT) {
                fprintf(stderr,
                        "types.c :: get_evaluation_type_of_subtree :: the builtin 'allocate' takes in and integer-expression, "
                        "but was given a '%s' at line '%d'\n",
                        ast_node_kinds_as_strings[b->type], node->lineno);
                exit(-1);
            }

            b = malloc(sizeof(struct TYPE));
            b->type = A_POINTER;
            b->pointer = a;

            return b;

        case A_SIZEOF:
        case A_CASTABLE:
            a = malloc(sizeof(struct TYPE));
            a->type = A_INT;
            return a;

        // Manage function stack so we always have a reference to the function an
        // A_RETURN node would return from
        case A_DEF_FUNC:
            function_stack[function_stack_cursor] = node;
            function_stack_cursor++;
            break;
        case A_END_DEF_FUNC:
            function_stack_cursor--;
            break;

        case A_WHILE:
            loop_stack[loop_stack_cursor] = node;
            loop_stack_cursor++;
            break;

        case A_END_WHILE:
            loop_stack_cursor--;
            break;

        // Verify that a return statement complies with the function it is returning
        // froms defined return type
        case A_RETURN:

            if (function_stack_cursor == 0) {
                fprintf(stderr,
                        "types.c :: get_evaluation_type_of_subtree :: A return statement "
                        "was found outside of a function at line '%d'\n",
                        node->lineno);
                exit(-1);
            }

            if (are_equivalent_types(get_evaluation_type_of_subtree(node->left),
                                     fetch_type_from_subtree(function_stack[function_stack_cursor - 1])->return_type) != 1) {
                fprintf(stderr,
                        "types.c :: get_evaluation_of_subtree :: Invalid return type from "
                        "function '%s', expected type '%s' but got '%s' at line '%d'\n",
                        function_stack[function_stack_cursor - 1]->left->right->id,
                        ast_node_kinds_as_strings[fetch_type_from_subtree(function_stack[function_stack_cursor - 1])
                                                      ->return_type->type],
                        ast_node_kinds_as_strings[get_evaluation_type_of_subtree(node->left)->type], node->lineno);

                exit(-1);
            }

            return get_evaluation_type_of_subtree(node->left);

        // Verify that break/continue statements are directly within a loop block
        case A_CONTINUE:
        case A_BREAK:
            if (loop_stack_cursor == 0 || node->symbol_table != loop_stack[loop_stack_cursor - 1]->symbol_table) {
                fprintf(stderr,
                        "types.c :: get_evaluation_type_of_subtree :: A %s statement "
                        "was found outside of a loop at line '%d'\n",
                        node->kind == A_BREAK ? "break" : "continue", node->lineno);
                exit(-1);
            }
            break;

        case A_DEREF:
            if (get_evaluation_type_of_subtree(node->left)->pointer == NULL) {
                fprintf(stderr,
                        "types.c :: get_evaluation_type_of_subtree :: Dereferencing "
                        "non-pointer type at line '%d'\n",
                        node->lineno);
                exit(-1);
            }


            // Return the value the pointer is pointing to
            return get_evaluation_type_of_subtree(node->left)->pointer;

        case A_ADDRESSOF:

            a = get_evaluation_type_of_subtree(node->left);
            if (a->type == A_POINTER) {
                fprintf(stderr,
                        "types.c :: get_evaluation_type_of_subtree :: Cannot take the "
                        "address of a pointer at line '%d'\n",
                        node->lineno);
                exit(-1);
            }

            a = malloc(sizeof(struct TYPE));
            a->type = A_POINTER;
            a->pointer = get_evaluation_type_of_subtree(node->left);
            return a;


        case A_INDEX:

            b = get_evaluation_type_of_subtree(node->right);

            // Only integer values can be used to index into an array
            if (b->type != A_INT) {
                fprintf(stderr,
                        "types.c :: get_evaluation_type_of_subtree :: "
                        "Only integer values can be used to index into an array at line '%d'\n",
                        node->lineno);
                exit(-1);
            }

            // Only arrays can be indexed, so we have to check whether or not the ID we're trying to index into is at least a
            // double pointer
            // @NOTE : We only have dynamic arrays, so [] is never used for anything but indexing.

            callback = lookup(node->left->id, node->symbol_table);

            if (callback == NULL) {
                fprintf(stderr,
                        "types.c :: get_evaluation_type_of_subtree :: ID '%s' could not "
                        "be found in any accessable scope at line '%d'\n",
                        node->left->id, node->lineno);
                exit(-1);
            }


            // String indexing returns a char
            if (callback->type->type == A_STRING) {
                a = malloc(sizeof(struct TYPE));
                a->type = A_CHAR;
                return a;
            }


            // If we're trying to index into something that isn't a pointer or a string its an error
            if (is_deep_pointer(callback->type) == 0) {
                fprintf(stderr,
                        "types.c :: get_evaluation_type_of_subtree :: Cannot index into a non-nested or non-string variable at "
                        "line '%d'\n",
                        node->lineno);
                exit(-1);
            }

            return callback->type->pointer;

        case A_DOT:
#if DEBUG
            printf("types.c :: get_evaluation_type_of_subtree :: A DOT node was encountered at line '%d'\n", node->lineno);
#endif

            a = get_evaluation_type_of_subtree(node->left);
            if (a->type != A_POINTER || a->pointer->type != A_STRUCT) {
                fprintf(stderr,
                        "types.c :: get_evaluation_type_of_subtree :: Cannot access a member of a non-struct at line '%d'\n",
                        node->lineno);
                exit(-1);
            }

            a = a->pointer;
            b = struct_has_member(a, node->right->id);

            if (b == NULL) {
                fprintf(stderr,
                        "types.c :: get_evaluation_type_of_subtree :: Struct '%s' does not have member '%s' at line '%d'\n",
                        a->id, node->right->id, node->lineno);
                exit(-1);
            }

            return b;

        // We're looking at a struct decl, so we return it's type
        case A_STRUCT:
            callback = lookup(node->left->id, node->symbol_table);
            if (callback == NULL) {
                fprintf(
                    stderr,
                    "types.c :: get_evaluation_type_of_subtree :: Struct declaration '%s' could not find itself at line '%d'\n",
                    node->left->id, node->lineno);
                exit(-1);
            }

            return callback->type;

        case A_PRINT:
            a = get_evaluation_type_of_subtree(node->left);
            if (a == NULL) {
                fprintf(stderr, "Cannot print return value of untyped callable, you need to unpack it first at line '%d'\n",
                        node->lineno);
                exit(-1);
            };
            return a;

        // Get the type of a literal
        case A_INTVAL:
        case A_INT:
        case A_FLOATVAL:
        case A_FLOAT:
        case A_CHARVAL:
        case A_CHAR:
        case A_STRINGVAL:
        case A_STRING:
            return fetch_type_from_subtree(node);
    }

    get_evaluation_type_of_subtree(node->left);
    get_evaluation_type_of_subtree(node->mid);
    get_evaluation_type_of_subtree(node->right);
}

/* Entrypoint for the typechecking pipeline */
void typecheck(struct AST_NODE * tree) {
    // Save reference to root of AST for finding function definitions
    root = tree;

    get_evaluation_type_of_subtree(tree);
}
