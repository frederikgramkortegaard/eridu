#include "symbols.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "translater.h"
#include "types.h"

struct TYPE * struct_has_member(struct TYPE * instruct, char * member) {
    for (int i = 0; i < instruct->number_of_fields; i++) {
        if (strcmp(instruct->fields[i]->var_name, member) == 0) {
            return instruct->fields[i];
        }
    }
    return NULL;
}

// Dynamically allocate space for params and append them to the given types list of parameters
void push_param_to_type_params(struct TYPE * type, struct TYPE * param) {
    if (type == NULL || param == NULL) {
        fprintf(stderr, "Error: push_param_to_type_params() called with NULL type or param\n");
        exit(1);
    }

    type->params = realloc(type->params, sizeof(struct TYPE *) * (type->number_of_params + 1));
    type->params[type->number_of_params] = param;
    type->number_of_params++;
}

// Creates a struct TYPE * corresponding to a given AST_NODE of kind A_PARAM
struct TYPE * eat_param(struct AST_NODE * node) {
    struct TYPE * type = malloc(sizeof(struct TYPE));
    type->number_of_params = 0;

    // Parameter is not typed
    if (node->right == NULL) {
        // Parameter already has a type
        if (node->left->kind == A_ID) {
            type = lookup(node->left->id, node->symbol_table)->type;
            type->var_name = malloc(strlen(node->left->id) + 1);
            strcpy(type->var_name, node->left->id);

            // Parameter does not represent an identifier
        } else {
            type = fetch_type_from_subtree(node->left);
        }

        // Parameter is typed
    } else {
        // Parse the type definition of the parameter
        type = fetch_type_from_subtree(node->left);

        // If the parameter is an ID, save the value of said ID along side its type.
        if (node->right->kind == A_ID) {
            type->var_name = malloc(strlen(node->right->id) + 1);
            strcpy(type->var_name, node->right->id);
        }
    };

    return type;
}

void fetch_params_from_subtree(struct AST_NODE * node, struct TYPE * type) {
    if (node == NULL) {
        return;
    }

    if (node->kind == A_PARAM) {
        push_param_to_type_params(type, eat_param(node));
    } else if (node->kind == A_GLUE) {
        fetch_params_from_subtree(node->left, type);
        fetch_params_from_subtree(node->right, type);
    } else if (node->kind == A_PARAM_LIST) {
        fetch_params_from_subtree(node->left, type);
    }
}

void fetch_callable_signature_from_subtree(struct AST_NODE * node, struct TYPE * type) {
    if (node == NULL) {
        return;
    }

    if (node->kind == A_GLUE) {
        fetch_callable_signature_from_subtree(node->left, type);
        fetch_callable_signature_from_subtree(node->right, type);

    } else {
        push_param_to_type_params(type, fetch_type_from_subtree(node));
    }
}

int count_num_params(struct AST_NODE * node) {
    if (node == NULL) {
        return 0;
    }

    if (node->kind == A_PARAM) {
        return 1;
    } else if (node->kind == A_GLUE) {
        return count_num_params(node->left) + count_num_params(node->right);
    } else if (node->kind == A_PARAM_LIST) {
        return count_num_params(node->left);
    } else {
        return 0;
    }
}

/* Given an AST_NODE; we can construct a TYPE struct based on the nodes subtree,
allowing for easier access to information regarding the type info of a node. */
struct TYPE * fetch_type_from_subtree(struct AST_NODE * node) {
    if (node == NULL) {
        return;
    }

    struct TYPE * type = malloc(sizeof(struct TYPE));
    type->number_of_params = 0;
    type->type = node->kind;

    // Normalize AST_NODE_KINDs
    if (node->kind == A_INTVAL) {
        type->type = A_INT;
    }
    if (node->kind == A_FLOATVAL) {
        type->type = A_FLOAT;
    }
    if (node->kind == A_CHARVAL) {
        type->type = A_CHAR;
    }
    if (node->kind == A_STRINGVAL) {
        type->type = A_STRING;
    }

    // Convert an A_DEF_FUNC into a type object for A_CALLABLES
    else if (node->kind == A_DEF_FUNC) {
        type->id = malloc(strlen(node->left->right->id) + 1);
        strcpy(type->id, node->left->right->id);

        // Propagate symbol table down to the type-definitions
        fill_symbol_tables(node->left, node->symbol_table);
        fill_symbol_tables(node->mid, node->symbol_table);

        type->return_type = fetch_type_from_subtree(node->left->left);
        type->type = A_CALLABLE;
        fetch_params_from_subtree(node->mid, type);

        // Parse LHS and RHS to find the signature of the function
    } else if (node->kind == A_CALLABLE) {
        type->return_type = fetch_type_from_subtree(node->left);
        fetch_callable_signature_from_subtree(node->right, type);

        // Parse LHS to find the type of the pointer
    } else if (node->kind == A_POINTER) {
        type->pointer = fetch_type_from_subtree(node->left);


    } else if (node->kind == A_DECL) {
        type = fetch_type_from_subtree(node->left);

        // Structure Definition
    } else if (node->kind == A_STRUCT && node->right != NULL) {
        type->type = A_STRUCT;
        type->id = malloc(strlen(node->left->id) + 1);
        strcpy(type->id, node->left->id);

        // @NOTE : Needs to be updated if we ever specify type sizes
        type->size = count_num_params(node->right);
        type->number_of_fields = 0;

        // Use of Structure as a type
    } else if (node->kind == A_STRUCT) {

        struct SYMBOL * callback = lookup(node->left->id, node->symbol_table);
        if (callback == NULL) {
            #if DEBUG
                printf("symbol table is null '%d'\n", node->symbol_table == NULL);
            #endif
            fprintf(stderr, "Error: struct '%s' is not defined in any accessible scope at line '%d'\n", node->left->id,
                    node->lineno);
            exit(-1);
        }
        type = callback->type;
    }

    node->type = type;
    return type;
}


/* Symbol table constructor */
SYMBOL_TABLE * make_table(SYMBOL_TABLE * parent) {
    static int table_count = 0;

    SYMBOL_TABLE * new_table = malloc(sizeof(SYMBOL_TABLE));
    new_table->id = table_count;
    new_table->symbols = malloc(sizeof(SYMBOL) * SYMBOL_TABLE_SIZE);
    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
        new_table->symbols[i] = NULL;
    }

    new_table->symbol_table_size = SYMBOL_TABLE_SIZE;
    new_table->number_of_variables = 0;
    new_table->number_of_parameters = 0;
    new_table->parent = parent;

    #ifdef DEBUG
        printf("symbols.c :: make_table :: created table with id '%d'\n", table_count);
    #endif

    table_count++;

    return new_table;
}

// Constructor for symbol creating
SYMBOL * make_symbol(char * id, struct TYPE * type, enum SYMBOL_CATEGORY category) {
    SYMBOL * new_symbol = malloc(sizeof(SYMBOL));

    new_symbol->id = malloc(strlen(id) + 1);
    strcpy(new_symbol->id, id);

    new_symbol->type = type;
    new_symbol->category = category;

    #if DEBUG
        printf("symbols.c :: make_symbol :: Creating symbol with ID '%s'\n", id);
    #endif
    return new_symbol;
}

int get_field_offset_in_struct(char * struct_id, char * id, struct SYMBOL_TABLE * table) {
    struct SYMBOL * symbol_category = lookup(struct_id, table);
    for (int i = 0; i < symbol_category->type->pointer->number_of_fields; i++) {
        if (strcmp(symbol_category->type->pointer->fields[i]->var_name, id) == 0) {
            return i;
        }
    }
    return -1;
}

int get_symbol_offset_in_stackframe(char * id, struct SYMBOL_TABLE * table) {
    int offset = 0;
    int negate = 0;
    struct SYMBOL * symbol_category = lookup(id, table);

    if (symbol_category->category == S_PARAM) {
        for (int i = 0; i < table->number_of_parameters + table->number_of_variables; i++) {
            if (strcmp(table->symbols[i]->id, id) == 0) {
                break;
            }
            if (table->symbols[i]->category == S_FUNC) {
                negate++;
                //continue;
            }
            if (table->symbols[i]->category == S_PARAM) {
                // continue;
            }
            if (table->symbols[i]->type->type == A_STRUCT) {
                continue;
            }
            offset++;
        }

#if DEBUG
        printf(
            "symbols.c :: get_symbol_offset_in_stackframe :: getting offset '%d' "
            "for symbol '%s' by traversing the given symbol table and keeping "
            "count of the position in which the symbol is found\n",
            offset, id);
#endif

        return offset - negate;
    }


    for (int i = 0; i < table->number_of_parameters + table->number_of_variables; i++) {
        if (strcmp(table->symbols[i]->id, id) == 0) {
            break;
        }
        if (table->symbols[i]->category == S_FUNC) {
            negate++;
            //continue;
        }
        if (table->symbols[i]->category == S_PARAM) {
            continue;
        }
        if (table->symbols[i]->type->type == A_STRUCT) {
            continue;
        }
        offset++;
    }

#if DEBUG
    printf(
        "symbols.c :: get_symbol_offset_in_stackframe :: getting offset '%d' "
        "for symbol '%s' by traversing the given symbol table and keeping "
        "count of the position in which the symbol is found\n",
        offset, id);
#endif


    return offset - negate;
}

// Search for a specific ID in the given scope.
// Does not traverse parent scopes!
struct SYMBOL * lookup_in_scope(char * id, SYMBOL_TABLE * table) {
#if DEBUG
    printf("symbols.c :: lookup_in_scope :: Trying to lookup ID '%s' in current scope\n", id);
#endif


    // Search for the symbol in the current scope using linear search
    for (int i = 0; i < table->number_of_parameters + table->number_of_variables; i++) {
        if (strcmp(id, table->symbols[i]->id) == 0) {
            return table->symbols[i];
        }
    }


    return NULL;
}

struct SYMBOL_TABLE * get_symbol_table_from_id(char * id, SYMBOL_TABLE * table) {
    // While we still have scopes to check
    while (table != NULL) {
        if (lookup_in_scope(id, table) != NULL) {
            return table;
        }

        // Attempt to visit parents scope.
        table = table->parent;
    }

    return NULL;
}

// Search for a specific ID in the given scope AND it's parents.
// Traverses the linked-list of scopes bottom-up starting at the given table.
struct SYMBOL * lookup(char * id, struct SYMBOL_TABLE * table) {
#if DEBUG
    printf("symbols.c :: lookup :: Trying to lookup ID '%s' in given scope and all parent scopes\n", id);
#endif

    struct SYMBOL * result = NULL;


    // While we still have scopes to check
    while (table != NULL) {
        // If we've found the ID in the current scope, return it.
        result = lookup_in_scope(id, table);
        if (result != NULL) {
            break;
        }

        // Attempt to visit parents scope.
        table = table->parent;
    }

    return result;
}

// Attempts to create and insert a symbol using the given data, into the given
// symbol table. if an identical symbol already exists, return -1.
int insert(char * id, struct TYPE * type, enum SYMBOL_CATEGORY category, SYMBOL_TABLE * table) {
    // Avoid duplicate insertions
    if (lookup_in_scope(id, table) != NULL) {
        fprintf(stderr, "symbols.c :: insert :: ID '%s' already exists in scope\n", id);
        exit(-1);
    }

    SYMBOL * new_symbol = make_symbol(id, type, category);

#ifdef DEBUG
    printf("symbols.c :: insert :: Inserting ID '%s' to symbol table\n", id);
#endif


    // Dynamic array of symbols
    if (table->symbol_table_size == table->number_of_variables + table->number_of_parameters) {
        table->symbol_table_size *= 2;
        table->symbols = realloc(table->symbols, sizeof(SYMBOL) * table->symbol_table_size);
    }

    // Linear search for valid position in symbol table
    int inserted = 0;
    int offset = 0;
    for (int i = 0; i < table->symbol_table_size; i++) {
        if (table->symbols[i] == NULL) {
            table->symbols[i] = new_symbol;
            inserted = 1;
            offset = i;
            // Update number of variables and parameters
            switch (category) {
                case S_VAR:
                case S_FUNC:
                    table->number_of_variables++;
                    break;
                case S_PARAM:
                    table->number_of_parameters++;
                    break;
            }
            break;
        }
    }

    if (inserted == 0) {
        fprintf(stderr,
                "symbols.c :: insert :: No available slots in symbol table, the maximum possible symbol count is '%d'\n",
                SYMBOL_TABLE_SIZE);
        exit(-1);
    }
    #ifdef DEBUG
        printf("symbols.c :: insert :: FOUND %d OFFSET FOR SYMBOL %s\n", offset, id);
    #endif
    return 0;
}


void push_member_to_field(struct TYPE * member, struct TYPE * to) {
    if (member == NULL || to == NULL) {
#if DEBUG
        printf("Error: push_member_to_field() called with NULL member or to-type\n");
#endif
        fprintf(stderr, "Could not add struct member to struct, this is a compiler error.\n");
        exit(1);
    }

    to->fields = realloc(to->fields, sizeof(struct TYPE *) * (to->number_of_fields + 1));
    to->fields[to->number_of_fields] = member;
    to->number_of_fields++;
}

// Propagate a symbol-table through a subtree, giving all nodes in the subtree the given table as its node->symbol_table
void fill_symbol_tables(struct AST_NODE * node, struct SYMBOL_TABLE * table) {
    if (node == NULL) {
        return;
    }

    if (node->left != NULL) {
        node->left->symbol_table = table;
        fill_symbol_tables(node->left, table);
    }

    if (node->right != NULL) {
        node->right->symbol_table = table;
        fill_symbol_tables(node->right, table);
    }
}

void fill_struct_type(struct AST_NODE * node, struct TYPE * type) {
    if (node == NULL) {
        return;
    }

    if (node->kind == A_PARAM) {
        if (node->left->kind == A_STRUCT) {
            fprintf(stderr, "Cannot declare a struct member with type of a struct without it being a pointer at line '%d'\n",
                    node->lineno);
            exit(-1);
        }

        // @NOTE : This isn't super efficient, since we're traversing the sub-tree once more.
        // we should really propagate this information during an earlier pass
        fill_symbol_tables(node, node->symbol_table);

        // Ensure that the struct doesn't already have a member with the same name
        struct TYPE * res = struct_has_member(type, node->right->id);

        if (res != NULL) {
            fprintf(stderr, "Error: struct '%s' already has a member '%s' at line '%d'\n", type->id, node->right->id,
                    node->lineno);
            exit(-1);
        }

        struct TYPE * newtype = fetch_type_from_subtree(node->left);


        newtype->var_name = malloc(sizeof(char) * (strlen(node->right->id) + 1));
        strcpy(newtype->var_name, node->right->id);
        push_member_to_field(newtype, type);
    } else {
        // Traverse until we find another parameter or run out of AST,
        // we also have to propagate the symbol table to all children
        // in the tree because struct members whose type refer to the
        // struct we're currently filling out need to be able to lookup
        // the type of the structure itself.
        if (node->left != NULL) {
            node->left->symbol_table = node->symbol_table;
            fill_struct_type(node->left, type);
        }

        if (node->right != NULL) {
            node->right->symbol_table = node->symbol_table;
            fill_struct_type(node->right, type);
        }
    }
}

int contains_return(struct AST_NODE * node) {
    if (node == NULL) {
        return;
    }

    if (node->kind == A_RETURN) {
        return 1;
    }

    if (node->left != NULL) {
        if (contains_return(node->left) == 1) {
            return 1;
        }
    }

    if (node->right != NULL) {
        if (contains_return(node->right) == 1) {
            return 1;
        }
    }

    if (node->mid != NULL) {
        if (contains_return(node->mid) == 1) {
            return 1;
        }
    }

    return 0;
}

/* Entrypoint for parsing the AST and generating symbol tables */
int expand_ast_with_symbol_tables(struct AST_NODE * tree, struct SYMBOL_TABLE * parent) {
    if (tree == NULL) {
        return 0;
    }

    // When we encounter a new scope we create a new
    // symbol table, unless the scope already has a table
    // as a result of some lookahead parsing for functions
    if (tree->kind == A_BLOCK || tree->kind == A_PROGRAM) {
        if (tree->symbol_table == NULL) {
            tree->symbol_table = make_table(parent);
        }

        parent = tree->symbol_table;

        // If the current node is a declaration node,
        // we know that there cannot be any other node structure than a single
        // left-child
    } else if (tree->kind == A_DECL) {
        // Ensure proper use of A_STRUCT as a type specifier
        if (tree->left->kind == A_STRUCT) {
            fprintf(stderr, "Cannot declare a variable as a struct, only as a pointer to a struct, at line '%d'\n",
                    tree->lineno);
            exit(-1);
        }

        // If the current node is a struct-pointer declaration, we need to
        // send it the proper symbol table to lookup in.
        struct AST_NODE * container = tree->left;
        while (container != NULL) {
            if (container->kind == A_STRUCT) {
                container->symbol_table = parent;
                break;
            }
            container = container->left;
        }

        // Array declaration
        if (tree->mid != NULL) {
            insert(tree->mid->id, fetch_type_from_subtree(tree), S_VAR, parent);
        } else {
            insert(tree->right->id, fetch_type_from_subtree(tree), S_VAR, parent);
        }

        // When creating symbol tables for functions we have
        // to create the inner-scope before parsing parameters
        // as they're inserted into the functions local scope
    } else if (tree->kind == A_DEF_FUNC) {
        if (contains_return(tree) == 0) {
            fprintf(stderr, "Function '%s' does not contain a return statement, which is required at line '%d'\n",
                    tree->left->right->id, tree->lineno);
            exit(-1);
        }

        // Fetch Callable Signature and create callable type
        tree->symbol_table = parent;
        struct TYPE * type = fetch_type_from_subtree(tree);

        // Insert function name in outer scope
        insert(tree->left->right->id, type, S_FUNC, parent);
        tree->left->right->symbol_table = parent;
        tree->right->symbol_table = make_table(parent);

#if DEBUG
        printf("symbols.c :: extend_ast_with_symbol_tables :: Beginning to parse a function definition\n");
#endif

        parent = tree->right->symbol_table;

        // Insert parameters of function into symbol table
        for (int i = 0; i < type->number_of_params; i++) {
            insert(type->params[i]->var_name, type->params[i], S_PARAM, parent);
        }

        // Declaration of a structure type
    } else if (tree->kind == A_STRUCT && tree->right != NULL) {
        struct TYPE * type = fetch_type_from_subtree(tree);
        insert(tree->left->id, type, S_VAR, parent);


        tree->symbol_table = parent;
        type->number_of_fields = 0;

        // Add members to the structures TYPE *
        fill_struct_type(tree, type);
        return;

    } else {
        tree->symbol_table = parent;
    }

    // Recursively extend all children of the current node with symbol tables
    if (tree->kind != A_DEF_FUNC) {
        expand_ast_with_symbol_tables(tree->left, parent);
    }

    expand_ast_with_symbol_tables(tree->mid, parent);
    expand_ast_with_symbol_tables(tree->right, parent);

    return 0;
}
