/* Generates an intermediate language to use later for converting our language
 * to ASM */

#include "codegen.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Takes in an AST_NODE and converts it to an intermediate_code operator in
 * codegen.c */
enum Operator ast_node_to_operator(struct AST_NODE *node) {
  if (node == NULL || node->kind == NULL) {
    return O_EMPTY;
  }

  // We need slightly different instructions for floating point numbers
  if (get_evaluation_type_of_subtree(node->left)->type == A_FLOAT) {
    switch (node->kind) {
    case A_DIV:
      return O_FDIV;
    case A_ADD:
      return O_FADD;
    case A_SUB:
      return O_FSUB;
    case A_MUL:
      return O_FMUL;
    case A_EQUAL:
      return O_JE;
    case A_GREAT:
      return O_JG;
    case A_LESS:
      return O_JL;
    case A_GEQ:
      return O_JGE;
    case A_LEQ:
      return O_JLE;
    case A_NEQUAL:
      return O_JNE;
    default:
      return O_EMPTY;
    }
  }

  switch (node->kind) {
  case A_MOD:
    return O_MOD;
  case A_ADD:
    return O_ADD;
  case A_SUB:
    return O_SUB;
  case A_MUL:
    return O_MUL;
  case A_EQUAL:
    return O_JE;
  case A_GREAT:
    return O_JG;
  case A_LESS:
    return O_JL;
  case A_GEQ:
    return O_JGE;
  case A_LEQ:
    return O_JLE;
  case A_NEQUAL:
    return O_JNE;
  case A_PRINT:
    return O_CALL_PRINTF;
  default:
    return O_EMPTY;
  }
}

/* Build a Target */
struct TARGET *make_target(enum Argument_Target reg, int register_number,
                           char *label) {
  struct TARGET *new_target = malloc(sizeof(struct TARGET));

  new_target->arg_target = reg;
  new_target->register_number = register_number;
  new_target->label = label;

  return new_target;
}

/* Build a Mode */
struct MODE *make_mode(enum Addresing_Mode mode, int offset) {
  struct MODE *new_mode = malloc(sizeof(struct MODE));

  new_mode->mode = mode;
  new_mode->offset = offset;
}

/* Build an Argument */
struct ARGUMENT *make_argument(struct TARGET *target, struct MODE *mode,
                               int literal) {
  struct ARGUMENT *new_argument = malloc(sizeof(struct ARGUMENT));

  new_argument->target = target;
  new_argument->mode = mode;
  new_argument->literal = literal;

  return new_argument;
}

/* Build an INSTRUCTION */
struct INSTRUCTION *make_instruction(enum Operator opcode,
                                     struct ARGUMENT *arg1,
                                     struct ARGUMENT *arg2,
                                     struct ARGUMENT *arg3, char *comment) {
  struct INSTRUCTION *new_instruction = malloc(sizeof(struct INSTRUCTION));
  new_instruction->operator= opcode;
  new_instruction->arg1 = arg1;
  new_instruction->arg2 = arg2;
  new_instruction->arg3 = arg3;

  new_instruction->comment = malloc(strlen(comment) + 1);
  strcpy(new_instruction->comment, comment);

  return new_instruction;
}

/* Takes an instruction and pushes it to the code generation statuses
   array of instructions to be used later */
void push_to_instruction_array(struct CODE_GENERATION_STATUS *status,
                               struct INSTRUCTION *instruction) {
#if DEBUG
  printf("codegen.c :: push_to_instruction_array :: Attempting to push "
         "instruction w. comment: '%s'\n",
         instruction->comment);
#endif

  if (status->instructions_length == status->max_instructions_length) {
#if DEBUG
    printf("codegen.c :: push_to_instruction_array :: Resizing "
           "status->instructions_length to twice the size\n");
#endif
    realloc(status->instruction_array,
            sizeof(struct INSTRUCTION) * status->max_instructions_length * 2);
    status->max_instructions_length *= 2;
  }

  status->instruction_array[status->instructions_length] = instruction;
  status->instructions_length++;
}

/* Generate a unique integer for use in generating string labels */
int generate_label(struct CODE_GENERATION_STATUS *status) {
  static int num_labels = 0;

#if DEBUG
  printf("codegen.c :: generate_label :: Generating a label\n");
#endif

  if (status->num_labels == status->max_num_labels) {
    realloc(status->labels, sizeof(char) * status->max_num_labels * 2);
    status->max_num_labels *= 2;
  }

  status->labels[status->num_labels] = malloc(sizeof(int));
  status->labels[status->num_labels] = num_labels;

  num_labels++;
  status->num_labels++;

  return status->labels[status->num_labels - 1];
}

/* Converts a label to a string with the possibility of adding a suffix,
  this is used to create say 'cmp_true_1' if you wanted to have multiple
  variants of the same label */
char *label_to_string(int label, char *suffix) {
  int suffix_size;
  if (suffix != NULL) {
    suffix_size = strlen(suffix) + 1;
  }

  char *strlabel = malloc(sizeof(int) + suffix_size);

  if (suffix != NULL) {
    sprintf(strlabel, "%s_%d", suffix, label);
  } else {
    sprintf(strlabel, "%d", label);
  }

  return strlabel;
}

/* Converts floating point to integers and the reverse using a simple conversion
 * instruction from the SSE register instruction set */
void cast_operator(struct CODE_GENERATION_STATUS *status,
                   struct AST_NODE *cast) {

  if (get_evaluation_type_of_subtree(cast->left)->type == A_FLOAT) {

    char *tmp;
    asprintf(&tmp, "cvtsi2ssq %s, %s ", "%%rax", "%%xmm0");

    push_to_instruction_array(
        status, make_instruction(O_POP,
                                 make_argument(make_target(RRT, 1, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, NULL, "pop integer value to rax"));

    push_to_instruction_array(
        status, make_instruction(O_EMPTY,
                                 make_argument(make_target(-1, NULL, tmp),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, NULL,
                                 "Convert integer to floating-point number "));

    push_to_instruction_array(
        status, make_instruction(O_MOVE,
                                 make_argument(make_target(REG, 100, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 make_argument(make_target(RRT, NULL, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, " move result in xmm0 to RRT "));

    push_to_instruction_array(
        status, make_instruction(O_PUSH,
                                 make_argument(make_target(RRT, NULL, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, NULL, "push the resulting value"

                                 ));
  } else if (get_evaluation_type_of_subtree(cast->left)->type == A_INT) {
    char *tmp;
    asprintf(&tmp, "cvttss2siq %s, %s ", "%%xmm0", "%%rax");

    push_to_instruction_array(
        status, make_instruction(O_POP,
                                 make_argument(make_target(RRT, 1, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, NULL, "pop integer value to rax"));

    push_to_instruction_array(
        status, make_instruction(O_MOVE,
                                 make_argument(make_target(RRT, NULL, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 make_argument(make_target(REG, 100, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, " move value in RRT to SSE 1  "));

    push_to_instruction_array(
        status, make_instruction(O_EMPTY,
                                 make_argument(make_target(-1, NULL, tmp),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, NULL,
                                 "Convert floating-point number to integer "));

    push_to_instruction_array(
        status, make_instruction(O_PUSH,
                                 make_argument(make_target(RRT, NULL, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, NULL, "push the resulting value"

                                 ));
  }
}

/* Defines a FLOAT_DATA */
struct FLOAT_DATA *make_float_data(float floatval,
                                   struct CODE_GENERATION_STATUS *status) {
  struct FLOAT_DATA *new_float = malloc(sizeof(struct FLOAT_DATA));
  new_float->label = label_to_string(generate_label(status), "FP");
  new_float->floatval = floatval;
  return new_float;
}

/* Defines a STRING_DATA */
struct STRING_DATA *make_string_data(char *stringval,
                                     struct CODE_GENERATION_STATUS *status) {
  struct STRING_DATA *new_string = malloc(sizeof(struct STRING_DATA));
  new_string->label = label_to_string(generate_label(status), "ST");
  new_string->stringval = stringval;
  return new_string;
}

/* Safely pushes a scope (symbol table, function stack...) to the code
 * generation status tracker */
void push_to_function_stack(struct CODE_GENERATION_STATUS *status,
                            struct SYMBOL_TABLE *table) {
#if DEBUG
  printf("codegen.c :: push_to_function_stack :: Attempting to push symbol "
         "table with id '%d' to function stack\n",
         table->id);
#endif

  if (status->function_stack_length == status->function_stack_max_length) {
#if DEBUG
    printf("codegen.c :: push_to_function_stack :: Resizing "
           "status->function_stack to twice the size\n");
#endif
    realloc(status->function_stack, sizeof(struct SYMBOL_TABLE) *
                                        status->function_stack_max_length * 2);
    status->function_stack_max_length *= 2;
  }

  status->function_stack[status->function_stack_length] = table;
  status->function_stack_length++;
}

/* Removes the increments the function stack length, essentially marking the top
element as free to be overwritten*/
struct SYMBOL_TABLE *
pop_from_function_stack(struct CODE_GENERATION_STATUS *status) {
  status->function_stack_length--;
  return status->function_stack[status->function_stack_length + 1];
}

/* When a float literal is encountered by codegen, we push it to the stack */
void float_literal(struct CODE_GENERATION_STATUS *status,
                   struct AST_NODE *literal) {
  // Add the float literal to the data section in the format FP_num
  int i = 0;
  while (status->float_data_array[i] != NULL) {
    i++;
  }
  struct FLOAT_DATA *new_float =
      make_float_data(literal->data.floatval, status);
  status->float_data_array[i] = new_float;
  status->float_array_length++;

  // Push the float literal to the top of the FP stack
  push_to_instruction_array(
      status,
      make_instruction(O_PUSH,
                       make_argument(make_target(IMF, NULL, new_float->label),
                                     make_mode(DIRECT, 0), NULL),

                       NULL, NULL, "Pushing floating point literal to stack"));
}

/* When a string literal is encountered by codegen, we wush it to the stack*/
void string_literal(struct CODE_GENERATION_STATUS *status,
                    struct AST_NODE *literal) {
  // Add the string literal to the data section in the format ST_num
  int i = 0;
  while (status->string_data_array[i] != NULL) {
    i++;
  }
  struct STRING_DATA *new_string = make_string_data(literal->id, status);
  status->string_data_array[i] = new_string;
  status->string_array_length++;

  // Push the string literal to the top of the FP stack
  push_to_instruction_array(
      status,
      make_instruction(O_PUSH,
                       make_argument(make_target(IMS, NULL, new_string->label),
                                     make_mode(DIRECT, 0), NULL),

                       NULL, NULL, "Pushing string literal to stack"));
}

/* When a char literal is encountered by codegen, we push it to the stack */
void char_literal(struct CODE_GENERATION_STATUS *status,
                  struct AST_NODE *literal) {
  push_to_instruction_array(
      status, make_instruction(O_PUSH,
                               make_argument(make_target(IMI, NULL, NULL),
                                             make_mode(DIRECT, 0),
                                             literal->data.charval),

                               NULL, NULL, "Pushing char literal to stack"));
}

/* When an integer literal is encountered by codegen, we push it to the stack */
void integer_literal(struct CODE_GENERATION_STATUS *status,
                     struct AST_NODE *literal) {
  push_to_instruction_array(
      status, make_instruction(O_PUSH,
                               make_argument(make_target(IMI, NULL, NULL),
                                             make_mode(DIRECT, 0),
                                             literal->data.intval),

                               NULL, NULL, "Pushing integer literal to stack"));
}

/* Assumes an interger will be at the top of the stack and attempts to print it.
 * Name is subject to change */
void print_operator(struct CODE_GENERATION_STATUS *status,
                    struct AST_NODE *expression) {

  push_to_instruction_array(
      status, make_instruction(O_MOVE,
                               make_argument(make_target(RBP, NULL, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               make_argument(make_target(REG, 13, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, "Save RSL as print might change this"));
  push_to_instruction_array(
      status, make_instruction(O_MOVE,
                               make_argument(make_target(RSP, NULL, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               make_argument(make_target(REG, 14, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, "Save RSP as print might change this"));
  if (get_evaluation_type_of_subtree(expression->left)->type == A_CHAR) {
    push_to_instruction_array(
        status, make_instruction(O_MOVE,
                                 make_argument(make_target(RSL, NULL, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 make_argument(make_target(REG, 12, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, "Save RSL as print might change this"));

    push_to_instruction_array(
        status,
        make_instruction(
            O_EMPTY,
            make_argument(make_target(-1, NULL, "leaq cform(%rip) , %rdi "),
                          make_mode(DIRECT, 0), NULL),
            NULL, NULL, "Prepare format in RDI"));

    push_to_instruction_array(status, make_instruction(O_CALL_PRINTF,

                                                       NULL, NULL, NULL,
                                                       "Printing given interger"

                                                       ));
    push_to_instruction_array(
        status, make_instruction(O_MOVE,
                                 make_argument(make_target(REG, 12, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 make_argument(make_target(RSL, NULL, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, "Restore RSL after printing"));
  } else if (get_evaluation_type_of_subtree(expression->left)->type ==
             A_FLOAT) {
    push_to_instruction_array(
        status, make_instruction(O_MOVE,
                                 make_argument(make_target(RSL, NULL, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 make_argument(make_target(REG, 12, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, "Save RSL as print might change this"));

    push_to_instruction_array(
        status, make_instruction(O_POP,
                                 make_argument(make_target(RRT, 1, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, NULL, "pop floating point value to rax"

                                 ));

    push_to_instruction_array(
        status,
        make_instruction(O_MOVE,
                         make_argument(make_target(RRT, NULL, NULL),
                                       make_mode(DIRECT, 0), NULL),
                         make_argument(make_target(REG, 100, NULL),
                                       make_mode(DIRECT, 0), NULL),
                         NULL, "Move floating point value to SSE reg 1"));

    push_to_instruction_array(
        status,
        make_instruction(
            O_EMPTY,
            make_argument(make_target(-1, NULL, "cvtps2pd %%xmm0, %%xmm0 "),
                          make_mode(DIRECT, 0), NULL),
            NULL, NULL,
            "Convert single precision float to double precision before "
            "printing "));

    push_to_instruction_array(
        status,
        make_instruction(O_MOVE,
                         make_argument(make_target(IMI, NULL, NULL),
                                       make_mode(DIRECT, 0), 1),
                         make_argument(make_target(RRT, NULL, NULL),
                                       make_mode(DIRECT, 0), NULL),
                         NULL, "Tell printf that we use 1 SSE register "));

    push_to_instruction_array(
        status,
        make_instruction(
            O_EMPTY,
            make_argument(make_target(-1, NULL, "leaq fform(%rip) , %rdi "),
                          make_mode(DIRECT, 0), NULL),
            NULL, NULL, "Prepare format in RDI"));

    push_to_instruction_array(status, make_instruction(O_CALL_PRINTF,

                                                       NULL, NULL, NULL,
                                                       "Printing given interger"

                                                       ));
    push_to_instruction_array(
        status, make_instruction(O_MOVE,
                                 make_argument(make_target(REG, 12, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 make_argument(make_target(RSL, NULL, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, "Restore RSL after printing"));
  } else if (get_evaluation_type_of_subtree(expression->left)->type ==
             A_STRING) {
    push_to_instruction_array(
        status, make_instruction(O_MOVE,
                                 make_argument(make_target(RSL, NULL, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 make_argument(make_target(REG, 12, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, "Save RSL as print might change this"));

    push_to_instruction_array(
        status,
        make_instruction(
            O_EMPTY,
            make_argument(make_target(-1, NULL, "leaq sform(%rip) , %rdi "),
                          make_mode(DIRECT, 0), NULL),
            NULL, NULL, "Prepare format in RDI"));

    push_to_instruction_array(status, make_instruction(O_CALL_PRINTF,

                                                       NULL, NULL, NULL,
                                                       "Printing given interger"

                                                       ));
    push_to_instruction_array(
        status, make_instruction(O_MOVE,
                                 make_argument(make_target(REG, 12, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 make_argument(make_target(RSL, NULL, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, "Restore RSL after printing"));
  } else if (get_evaluation_type_of_subtree(expression->left)->type == A_INT ||
             expression->left->kind == A_ADDRESSOF ||
             get_evaluation_type_of_subtree(expression->left)->type ==
                 A_POINTER) {
    push_to_instruction_array(
        status, make_instruction(O_MOVE,
                                 make_argument(make_target(RSL, NULL, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 make_argument(make_target(REG, 12, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, "Save RSL as print might change this"));

    push_to_instruction_array(
        status,
        make_instruction(
            O_EMPTY,
            make_argument(make_target(-1, NULL, "leaq iform(%rip) , %rdi "),
                          make_mode(DIRECT, 0), NULL),
            NULL, NULL, "Prepare format in RDI"));

    push_to_instruction_array(status, make_instruction(O_CALL_PRINTF,

                                                       NULL, NULL, NULL,
                                                       "Printing given interger"

                                                       ));
    push_to_instruction_array(
        status, make_instruction(O_MOVE,
                                 make_argument(make_target(REG, 12, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 make_argument(make_target(RSL, NULL, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, "Restore RSL after printing"));
  }
  push_to_instruction_array(
      status, make_instruction(O_MOVE,
                               make_argument(make_target(REG, 13, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               make_argument(make_target(RBP, NULL, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, "Restore RSL after printing"));
  push_to_instruction_array(
      status, make_instruction(O_MOVE,
                               make_argument(make_target(REG, 14, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               make_argument(make_target(RSP, NULL, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, "Restore RSP after printing"));
}

/* Defines an arithmetic operation */
void arithmetic_operator(struct CODE_GENERATION_STATUS *status,
                         struct AST_NODE *operation) {
  /*                         pop reg1
                             pop reg2
                             op reg1, reg2
                             push reg2

  */
  push_to_instruction_array(
      status, make_instruction(O_POP,
                               make_argument(make_target(REG, 1, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, NULL, "pop 2nd expression to register"

                               ));
  push_to_instruction_array(
      status, make_instruction(O_POP,
                               make_argument(make_target(REG, 2, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, NULL, "pop 1st expression to register"

                               ));

  push_to_instruction_array(
      status,
      make_instruction(
          ast_node_to_operator(operation),
          make_argument(make_target(REG, 1, NULL), make_mode(DIRECT, 0), NULL),
          make_argument(make_target(REG, 2, NULL), make_mode(DIRECT, 0), NULL),
          NULL, "carry out the operation"

          ));
  push_to_instruction_array(
      status, make_instruction(O_PUSH,
                               make_argument(make_target(REG, 2, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, NULL, "push the resulting value"

                               ));
}

/* Defines a comparison operation */
void comparison_operator(struct CODE_GENERATION_STATUS *status,
                         struct AST_NODE *operation, enum Operator jump_type) {
  /*  pop reg2
      pop reg1
      cmp reg2, reg1
      cond_jump true_jump
      push false
      jmp end_label
  true_label:
     push true
  end_label:
*/

  int label = generate_label(status);
  char *true_label = label_to_string(label, "cmp_true");
  char *end_label = label_to_string(label, "cmp_end");

  push_to_instruction_array(
      status, make_instruction(O_POP,
                               make_argument(make_target(REG, 2, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, NULL, "pop 2nd expression to register"));

  push_to_instruction_array(
      status, make_instruction(O_POP,
                               make_argument(make_target(REG, 1, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, NULL, "pop 1st expression to register"

                               ));

  if (get_evaluation_type_of_subtree(operation->left)->type == A_CHAR) {
    push_to_instruction_array(
        status, make_instruction(O_CMPB,
                                 make_argument(make_target(REG, 52, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 make_argument(make_target(REG, 51, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, "compare values"

                                 ));
  } else {
    push_to_instruction_array(
        status, make_instruction(O_CMP,
                                 make_argument(make_target(REG, 2, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 make_argument(make_target(REG, 1, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, "compare values"

                                 ));
  }

  push_to_instruction_array(
      status, make_instruction(jump_type,
                               make_argument(make_target(MEM, 2, true_label),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, NULL, operators_as_strings[jump_type]));
  push_to_instruction_array(
      status, make_instruction(O_PUSH,
                               make_argument(make_target(IMI, 0, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, NULL, "push false"

                               ));
  push_to_instruction_array(
      status, make_instruction(O_JMP,
                               make_argument(make_target(MEM, NULL, end_label),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, NULL, "done with comparison"

                               ));
  push_to_instruction_array(
      status, make_instruction(O_LABEL,
                               make_argument(make_target(MEM, NULL, true_label),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, NULL, "label"

                               ));
  push_to_instruction_array(
      status, make_instruction(O_PUSH,
                               make_argument(make_target(IMI, 1, NULL),
                                             make_mode(DIRECT, 0), 1),
                               NULL, NULL, "push true"

                               ));
  push_to_instruction_array(
      status, make_instruction(O_LABEL,
                               make_argument(make_target(MEM, NULL, end_label),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, NULL, "endlabel"

                               ));
}
/* Defines the operations for while loops */
void while_operator(struct CODE_GENERATION_STATUS *status,
                    struct AST_NODE *whilenode) {
  int label_id = generate_label(status);
  char *start_cond_label =
      label_to_string(label_id, "start_of_while_conditional");
  char *end_cond_label = label_to_string(label_id, "end_of_while_conditional");
  char *start_while_block_label =
      label_to_string(label_id, "start_of_while_block");
  char *end_while_block_label = label_to_string(label_id, "end_of_while_block");

  push_to_instruction_array(
      status, make_instruction(
                  O_WHILE,
                  make_argument(make_target(MEM, NULL, end_while_block_label),
                                make_mode(DIRECT, 0), NULL),
                  NULL, NULL, "STARTING TO GENERATE WHILE CODE"

                  ));

  push_to_instruction_array(
      status,
      make_instruction(O_JMP,
                       make_argument(make_target(MEM, NULL, start_cond_label),
                                     make_mode(DIRECT, 0), NULL),
                       NULL, NULL, "Jump to start of while conditional"

                       ));

  push_to_instruction_array(
      status, make_instruction(
                  O_LABEL,
                  make_argument(make_target(MEM, NULL, start_while_block_label),
                                make_mode(DIRECT, 0), NULL),
                  NULL, NULL, "label"

                  ));

  generate_code_inner(status, whilenode->right);

  push_to_instruction_array(
      status,
      make_instruction(O_LABEL,
                       make_argument(make_target(MEM, NULL, start_cond_label),
                                     make_mode(DIRECT, 0), NULL),
                       NULL, NULL, "label"

                       ));

  generate_code_inner(status, whilenode->left->left);
  generate_code_inner(status, whilenode->left->right);

  push_to_instruction_array(
      status, make_instruction(O_POP,
                               make_argument(make_target(REG, 2, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, NULL, "pop 2nd expression to register"));

  push_to_instruction_array(
      status, make_instruction(O_POP,
                               make_argument(make_target(REG, 1, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, NULL, "pop 1st expression to register"

                               ));
  if (get_evaluation_type_of_subtree(whilenode->left->left)->type == A_CHAR) {
    push_to_instruction_array(
        status, make_instruction(O_CMPB,
                                 make_argument(make_target(REG, 52, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 make_argument(make_target(REG, 51, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, "compare values"

                                 ));
  } else {
    push_to_instruction_array(
        status, make_instruction(O_CMP,
                                 make_argument(make_target(REG, 2, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 make_argument(make_target(REG, 1, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, "compare values"

                                 ));
  }

  enum Operator jump_type = ast_node_to_operator(whilenode->left);

  push_to_instruction_array(
      status, make_instruction(
                  jump_type,
                  make_argument(make_target(MEM, 2, start_while_block_label),
                                make_mode(DIRECT, 0), NULL),
                  NULL, NULL, operators_as_strings[jump_type]));

  push_to_instruction_array(
      status, make_instruction(
                  O_LABEL,
                  make_argument(make_target(MEM, NULL, end_while_block_label),
                                make_mode(DIRECT, 0), NULL),
                  NULL, NULL, "label"

                  ));
}

/* Defines the instructions needed to implement an if statement */
void if_operator(struct CODE_GENERATION_STATUS *status,
                 struct AST_NODE *ifnode) {
  int label_id = generate_label(status);
  char *start_label = label_to_string(label_id, "start_of_if_block");
  char *end_label = label_to_string(label_id, "end_of_if_block");
  char *start_cond = label_to_string(label_id, "start_of_if_conditional");

  push_to_instruction_array(
      status, make_instruction(O_JMP,
                               make_argument(make_target(MEM, NULL, start_cond),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, NULL, "Jump to start of while conditional"

                               ));

  push_to_instruction_array(
      status,
      make_instruction(O_LABEL,
                       make_argument(make_target(MEM, NULL, start_label),
                                     make_mode(DIRECT, 0), NULL),
                       NULL, NULL, "label"

                       ));

  generate_code_inner(status, ifnode->right);

  push_to_instruction_array(
      status, make_instruction(O_JMP,
                               make_argument(make_target(MEM, NULL, end_label),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, NULL, "Jump to start of while conditional"

                               ));

  push_to_instruction_array(
      status, make_instruction(O_LABEL,
                               make_argument(make_target(MEM, NULL, start_cond),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, NULL, "label"

                               ));

  generate_code_inner(status, ifnode->left->left);
  generate_code_inner(status, ifnode->left->right);

  push_to_instruction_array(
      status, make_instruction(O_POP,
                               make_argument(make_target(REG, 2, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, NULL, "pop 2nd expression to register"));

  push_to_instruction_array(
      status, make_instruction(O_POP,
                               make_argument(make_target(REG, 1, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, NULL, "pop 1st expression to register"

                               ));

  if (get_evaluation_type_of_subtree(ifnode->left->left)->type == A_CHAR) {
    push_to_instruction_array(
        status, make_instruction(O_CMPB,
                                 make_argument(make_target(REG, 52, "b"),
                                               make_mode(DIRECT, 0), NULL),
                                 make_argument(make_target(REG, 51, "b"),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, "compare byte values"

                                 ));
  } else {
    push_to_instruction_array(
        status, make_instruction(O_CMP,
                                 make_argument(make_target(REG, 2, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 make_argument(make_target(REG, 1, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, "compare values"

                                 ));
  }

  enum Operator jump_type = ast_node_to_operator(ifnode->left);
  push_to_instruction_array(
      status, make_instruction(jump_type,
                               make_argument(make_target(MEM, 2, start_label),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, NULL, operators_as_strings[jump_type]));

  push_to_instruction_array(
      status, make_instruction(O_LABEL,
                               make_argument(make_target(MEM, NULL, end_label),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, NULL, "label"

                               ));
}

/* Defines operations needed for dividing two numbers */
void division_operator(struct CODE_GENERATION_STATUS *status,
                       struct AST_NODE *tree) {
  push_to_instruction_array(
      status, make_instruction(O_DIV, NULL, NULL, NULL, "Division operation"));
}

void mod_operator(struct CODE_GENERATION_STATUS *status,
                  struct AST_NODE *node) {
  push_to_instruction_array(
      status, make_instruction(O_MOD, NULL, NULL, NULL, "Division operation"));
}

/* Defines floating point basic arithmetic operations */
void floating_arithmetic_operator(struct CODE_GENERATION_STATUS *status,
                                  struct AST_NODE *opcode) {
  push_to_instruction_array(
      status, make_instruction(O_POP,
                               make_argument(make_target(RRT, NULL, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, NULL, "pop 2nd expression to register"

                               ));

  push_to_instruction_array(
      status, make_instruction(O_MOVE,
                               make_argument(make_target(RRT, NULL, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               make_argument(make_target(REG, 100, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, " Move 2nd expression to SSE 1 register"));

  push_to_instruction_array(
      status, make_instruction(O_POP,
                               make_argument(make_target(RRT, NULL, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, NULL, "pop 1nd expression to register"

                               ));

  push_to_instruction_array(
      status, make_instruction(O_MOVE,
                               make_argument(make_target(RRT, NULL, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               make_argument(make_target(REG, 101, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, " Move 1nd expression to SSE 2 register"));

  push_to_instruction_array(
      status, make_instruction(ast_node_to_operator(opcode),
                               make_argument(make_target(REG, 100, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               make_argument(make_target(REG, 101, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, "carry out the operation"

                               ));

  push_to_instruction_array(
      status, make_instruction(O_MOVE,
                               make_argument(make_target(REG, 101, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               make_argument(make_target(RRT, NULL, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, " move result in xmm0 to RRT "));

  push_to_instruction_array(
      status, make_instruction(O_PUSH,
                               make_argument(make_target(RRT, NULL, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, NULL, "push the resulting value"

                               ));
}

/* Defines the operation of the continue command, which is simply and
 * unconditional jmp */
void operand_continue(struct CODE_GENERATION_STATUS *status,
                      struct AST_NODE *node) {
  int temp = status->instructions_length - 1;

  while (status->instruction_array[temp]->operator!= O_WHILE) {
    temp--;
  }
  temp++;

  char *while_start_label;
  asprintf(&while_start_label, "%s",
           status->instruction_array[temp]->arg1->target->label);

  push_to_instruction_array(
      status,
      make_instruction(O_JMP,
                       make_argument(make_target(MEM, NULL, while_start_label),
                                     make_mode(DIRECT, 0), NULL),
                       NULL, NULL, "jump to start_while_label"

                       ));
}

/* Defines the operation of the break command, which is simply and unconditional
 * jmp */
void operand_break(struct CODE_GENERATION_STATUS *status,
                   struct AST_NODE *node) {
  int temp = status->instructions_length - 1;

  while (status->instruction_array[temp]->operator!= O_WHILE) {
    temp--;
  }

  push_to_instruction_array(
      status,
      make_instruction(
          O_JMP,
          make_argument(
              make_target(MEM, NULL,
                          status->instruction_array[temp]->arg1->target->label),
              make_mode(DIRECT, 0), NULL),
          NULL, NULL, "jump to end_while label"

          ));
}

/*                            START OF SCOPING CODE */

/* for each variable's in the current scope, we increment the stack */
void push_variables_to_stack(struct CODE_GENERATION_STATUS *status,
                             struct AST_NODE *tree) {
  struct SYMBOL_TABLE *table = status->current_symbol_table;
  int size_to_add = 0;

  if (tree == NULL) {

    return;
  }

  // Iterativly find the number of symbols in the symbol table, while ignoring
  // those classified as functions
  for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
    if (table->symbols[i] == NULL || table->symbols[i]->category == S_FUNC ||
        table->symbols[i]->category == S_PARAM) {
      continue;
    }

    struct SYMBOL *head = table->symbols[i];

    do {
      size_to_add++;

      head = head->next;

    } while (head != NULL);
  }
  if (size_to_add == 0) {
    return;
  }
  push_to_instruction_array(
      status,
      make_instruction(O_SUB,
                       make_argument(make_target(IMI, NULL, NULL),
                                     make_mode(DIRECT, 0), size_to_add * 8),
                       make_argument(make_target(RSP, NULL, NULL),
                                     make_mode(DIRECT, 0), NULL),
                       NULL, "Push stack space to account for variables "));
}

/* Restores the stack pointer and base frame pointer */
void caller_restore(struct CODE_GENERATION_STATUS *status) {
  push_to_instruction_array(
      status,
      make_instruction(
          O_MOVE,
          make_argument(make_target(RBP, NULL, NULL), make_mode(DIRECT, 0),
                        NULL),
          make_argument(make_target(RSP, NULL, NULL), make_mode(DIRECT, 0),
                        NULL),
          NULL, "Restore stack pointer and deallocate space taken by block"));

  push_to_instruction_array(
      status, make_instruction(O_ADD,
                               make_argument(make_target(IMI, NULL, NULL),
                                             make_mode(DIRECT, 0), 16),
                               make_argument(make_target(RSP, NULL, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, "remove RBP from the top of the stack "

                               ));

  push_to_instruction_array(
      status,
      make_instruction(O_MOVE,
                       make_argument(make_target(RBP, NULL, NULL),
                                     make_mode(INDIRECT_RELATIVE, 8), NULL),
                       make_argument(make_target(RBP, NULL, NULL),
                                     make_mode(DIRECT, 0), NULL),
                       NULL, "Restore frame pointer from static link "));
}

/* When we enter a new scope, we save the stack frame base pointer and static
 * link pointer on the top of the stack */
void caller_save(struct CODE_GENERATION_STATUS *status, struct AST_NODE *node) {
  push_to_instruction_array(
      status, make_instruction(
                  O_PUSH,
                  make_argument(make_target(RBP, NULL, NULL),
                                make_mode(DIRECT, 0), NULL),
                  NULL, NULL, "save RBP on the stack for static link traversel "

                  ));

  push_to_instruction_array(
      status,
      make_instruction(
          O_SUB,
          make_argument(make_target(IMI, NULL, NULL), make_mode(DIRECT, 0), 8),
          make_argument(make_target(RSP, NULL, NULL), make_mode(DIRECT, 0),
                        NULL),
          NULL, "Provide extra stack space to account for variables "

          ));
}
/* When we enter the callee's scope, we set up the base frame pointer from RSP
 * and make the previous RBP value the new RSL */
void callee_prologue(struct CODE_GENERATION_STATUS *status,
                     struct AST_NODE *node) {
  push_to_instruction_array(
      status, make_instruction(O_MOVE,
                               make_argument(make_target(RSP, NULL, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               make_argument(make_target(RBP, NULL, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, "Updating current stack frame pointer"));

  push_variables_to_stack(status, node);
}

/* Simply updates the current simple table by the second argument's symbol table
 */
void update_symbol_table(struct CODE_GENERATION_STATUS *status,
                         struct AST_NODE *node) {
  status->current_symbol_table = node->symbol_table;
}

/* Defines the operations for maintaining the stack frame, when entering a new
 * scope */
void scope_previsit(struct CODE_GENERATION_STATUS *status,
                    struct AST_NODE *node) {
  update_symbol_table(status, node);
  push_to_function_stack(status, node->symbol_table);

  // these O_CALLxx_XXX are just for debug prints atm
  push_to_instruction_array(
      status, make_instruction(O_CALLER_SAVE, NULL, NULL, NULL,
                               "Saving state before entering a new scope"));

  caller_save(status, node);

  push_to_instruction_array(status,
                            make_instruction(O_CALLEE_PROLOGUE, NULL, NULL,
                                             NULL, "Prologue for new scope"));

  callee_prologue(status, node);
}

/* Defines the operations for maintaining the stack frame, when leaving the
 * current scope */
void scope_postvisit(struct CODE_GENERATION_STATUS *status) {
  status->current_symbol_table = status->current_symbol_table->parent;

  pop_from_function_stack(status);
  push_to_instruction_array(
      status, make_instruction(O_CALLEE_EPILOGUE, NULL, NULL, NULL,
                               "Teardown before exiting scope"));

  push_to_instruction_array(status,
                            make_instruction(O_CALLER_RESTORE, NULL, NULL, NULL,
                                             "Restoring parent scope"));

  caller_restore(status);
}

/* Setup base frame pointer, this is the A_PROGRAM prologue */
void program_setup(struct CODE_GENERATION_STATUS *status,
                   struct AST_NODE *tree) {
  push_to_instruction_array(status,
                            make_instruction(O_PROGRAM_PROLOGUE, NULL, NULL,
                                             NULL, "Initializing Program"));

  // Set base stack pointer to 0
  push_to_instruction_array(
      status, make_instruction(O_MOVE,
                               make_argument(make_target(RSP, NULL, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               make_argument(make_target(RBP, NULL, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               NULL,
                               "Set the base frame pointer for the global "
                               "scope to be the start of the stack "

                               ));

  push_variables_to_stack(status, tree);

  if (lookup_in_scope("arguments", status->current_symbol_table) != NULL) {

    int offset = get_symbol_offset_in_stackframe("arguments",
                                                 status->current_symbol_table) +
                 1;

    push_to_instruction_array(
        status,
        make_instruction(
            O_MOVE,
            make_argument(make_target(RSI, NULL, NULL), make_mode(DIRECT, 0),
                          NULL),
            make_argument(make_target(RBP, NULL, NULL),
                          make_mode(INDIRECT_RELATIVE, (-offset) * 8), NULL),
            NULL,
            " Move command line arguments into 'argument' builtin global scope "
            "str pointer"));
  }
}

/* End of program is reached, so the standard exit codes are sent to the OS */
void program_teardown(struct CODE_GENERATION_STATUS *status) {
  push_to_instruction_array(status,
                            make_instruction(O_PROGRAM_EPILOGUE, NULL, NULL,
                                             NULL, "Program Teardown"));
}

/* Saves the current register values before following the linked-list of scopes
 * found in the stack */
void follow_static_link_previsit(struct CODE_GENERATION_STATUS *status) {
  push_to_instruction_array(
      status,
      make_instruction(
          O_MOVE,
          make_argument(make_target(RBP, NULL, NULL), make_mode(DIRECT, 0),
                        NULL),
          make_argument(make_target(REG, 14, NULL), make_mode(DIRECT, 0), NULL),
          NULL,
          "Save pointer to this stack frame before following static link"));
}

/* Follows the linked-list of scopes on the stack once */
void follow_static_link(struct CODE_GENERATION_STATUS *status) {
#if DEBUG
  printf(
      "codegen.c :: follow_static_link :: Attempting to follow static link\n");
#endif

  int checkVal = (status->current_symbol_table->number_of_variables +
                  status->current_symbol_table->number_of_parameters);
  if (checkVal == 0) {
    checkVal = 0;
  } else {
    checkVal = 8 * checkVal;
  }
  push_to_instruction_array(
      status,
      make_instruction(O_MOVE,
                       make_argument(make_target(RBP, NULL, NULL),
                                     make_mode(INDIRECT_RELATIVE, 8), NULL),
                       make_argument(make_target(RBP, NULL, NULL),
                                     make_mode(DIRECT, 0), NULL),
                       NULL, "Performing static link computation"));
}

/* Restores the original register values after having followed a static link,
 * which allows for traversel of scopes in both directions */
void follow_static_link_postvisit(struct CODE_GENERATION_STATUS *status) {
  push_to_instruction_array(
      status, make_instruction(O_MOVE,
                               make_argument(make_target(REG, 14, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               make_argument(make_target(RBP, NULL, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               NULL,
                               "restore pointer to this stack frame after "
                               "finishing following static link"));
}

/* Emits the intermediate code finding the given identifier in the static nested
 * scoping, and pushes its value to the stack */
void identifier(struct CODE_GENERATION_STATUS *status,
                struct AST_NODE *identifier) {
  struct SYMBOL_TABLE *head = status->current_symbol_table;

  int count = 0;

  if (head == NULL) {
    return;
  }
  // Finds the scope out if the identifier is in the current scope or a parent
  // scope
  do {
    if (lookup_in_scope(identifier->id, head) != NULL) {
      break;
    }
    head = head->parent;
    count++;
  } while (head != NULL);
  struct SYMBOL *id_symbol =
      lookup(identifier->id, status->current_symbol_table);

  // If count was not zero, we have to follow the static link
  if (count != 0) {
    follow_static_link_previsit(status);
    struct SYMBOL_TABLE *tmp = status->current_symbol_table;
    for (int i = 0; i < count; i++) {
      follow_static_link(status);
      status->current_symbol_table = status->current_symbol_table->parent;
    }

    // We are now in the correct stack frame
    int offset = get_symbol_offset_in_stackframe(identifier->id, head) + 1;

    // If a function is being used as a variable, we return a pointer to the
    // function's first instruction
    if (id_symbol->category == S_FUNC) {
      char *temp = malloc(sizeof(char *));
      struct SYMBOL *func =
          lookup_in_scope(identifier->id, status->current_symbol_table);
      asprintf(&temp, "leaq %s(%rip) , %r13  ", id_symbol->function_label,
               identifier->id);
      push_to_instruction_array(
          status, make_instruction(O_EMPTY,
                                   make_argument(make_target(-1, NULL, temp),
                                                 make_mode(DIRECT, 0), NULL),
                                   NULL, NULL,
                                   " #Moving address of first instruction of "
                                   "function into temp register"));

      push_to_instruction_array(
          status, make_instruction(O_PUSH,
                                   make_argument(make_target(REG, 13, 0),
                                                 make_mode(DIRECT, 0), NULL),
                                   NULL, NULL,
                                   "Push pointer value to top of the stack"));
    } else if (id_symbol->category ==
               S_PARAM) { // If the identifier is a parameter, we find it's
                          // offset in the previous stackframe
      int param_offset =
          ((status->current_symbol_table->number_of_parameters) - (offset - 1));

      push_to_instruction_array(
          status,
          make_instruction(
              O_PUSH,
              make_argument(
                  make_target(RBP, 1, NULL),
                  make_mode(INDIRECT_RELATIVE, 24 + (param_offset) * 8), NULL),
              NULL, NULL,
              "Push the variable at paramoffset to the top of the stack "));
    } else { // Else we can simple push the value at the offset

      push_to_instruction_array(
          status, make_instruction(
                      O_PUSH,
                      make_argument(
                          make_target(RBP, 1, NULL),
                          make_mode(INDIRECT_RELATIVE, ((-offset) * 8)), NULL),
                      NULL, NULL, "Push the variable to the top of the stack"));
    }

    status->current_symbol_table = tmp;
    follow_static_link_postvisit(status);
  }

  else {
    int offset = get_symbol_offset_in_stackframe(identifier->id,
                                                 status->current_symbol_table) +
                 1;
#if DEBUG

#endif
    if (id_symbol->category ==
        S_FUNC) { // If a function is being used as a variable, we return a
                  // pointer to the function's first instruction
      char *temp = malloc(sizeof(char *));
      asprintf(&temp, "leaq %s(%rip) , %r13", id_symbol->function_label,
               identifier->id);
      push_to_instruction_array(
          status, make_instruction(O_EMPTY,
                                   make_argument(make_target(-1, NULL, temp),
                                                 make_mode(DIRECT, 0), NULL),
                                   NULL, NULL,
                                   " #Moving address of first instruction of "
                                   "function into temp register"));

      push_to_instruction_array(
          status, make_instruction(O_PUSH,
                                   make_argument(make_target(REG, 13, 0),
                                                 make_mode(DIRECT, 0), NULL),
                                   NULL, NULL,
                                   "Push pointer value to top of the stack"));
    } else if (id_symbol->category ==
               S_PARAM) { // If the identifier is a parameter, we find it's
                          // offset in the previous stackframe

      int param_offset =
          ((status->current_symbol_table->number_of_parameters) - (offset - 1));

      push_to_instruction_array(
          status,
          make_instruction(
              O_PUSH,
              make_argument(
                  make_target(RBP, 1, NULL),
                  make_mode(INDIRECT_RELATIVE, 24 + (param_offset) * 8), NULL),
              NULL, NULL, "Push the variable to the top of the stack"));
    } else { // Else we can simple push the value at the offset
      push_to_instruction_array(
          status,
          make_instruction(
              O_PUSH,
              make_argument(make_target(RBP, 1, NULL),
                            make_mode(INDIRECT_RELATIVE, (-offset) * 8), NULL),
              NULL, NULL, "Push the variable to the top of the stack"));
    }
  }
}

/* Assigns a value to a variable through the stack */
void assignment_operator(struct CODE_GENERATION_STATUS *status,
                         struct AST_NODE *operator) {

  struct SYMBOL_TABLE *head = status->current_symbol_table;
  int offset = 0;
  struct SYMBOL *id_symbol;
  struct AST_NODE *tmp_node = operator;
  struct AST_NODE *tmp_node2 = operator;
  if (operator->left->kind == A_DEREF || operator->left->kind ==
      A_INDEX) { // If we are a dereference node, or index node, the id must be
                 // checked in the lowest dereference level for the offsets to
                 // work
    while (tmp_node->left->kind != A_ID) {
      tmp_node = tmp_node->left;
    }
  }
  if (operator->left->kind == A_DEREF || operator->left->kind == A_INDEX) {
    while (tmp_node2->left->left->kind != A_ID) {
      tmp_node2 = tmp_node2->left;
    }
  }

  if (operator->left->kind == A_DOT) {
    tmp_node = operator->left;
  }

  int count = 0;

  struct SYMBOL_TABLE *tmp = status->current_symbol_table;

  if (head == NULL) {
    return;
  }
  // Finds the scope out if the identifier is in the current scope or a parent
  // scope
  do {
    if (lookup_in_scope(tmp_node->left->id, head) != NULL) {
      break;
    }
    head = head->parent;
    count++;
  } while (head != NULL);

  push_to_instruction_array(
      status,
      make_instruction(
          O_POP,
          make_argument(make_target(RRT, 1, NULL), make_mode(DIRECT, 0), NULL),
          NULL, NULL, "Pop value from right hand side to rax"));

  if (tmp_node->kind == A_INDEX) {
    struct SYMBOL_TABLE *tmp2 = status->current_symbol_table;
    status->current_symbol_table = tmp;
    generate_code_inner(status, tmp_node->right);
    status->current_symbol_table = tmp2;
  }

  if (count != 0) {
    follow_static_link_previsit(status);
    for (int i = 0; i < count;
         i++) { // Follows the the static link the according to count
      follow_static_link(status);
      status->current_symbol_table = status->current_symbol_table->parent;
    }
    offset = get_symbol_offset_in_stackframe(tmp_node->left->id, head) + 1;
  } else {
    offset = get_symbol_offset_in_stackframe(tmp_node->left->id,
                                             status->current_symbol_table) +
             1;
  }

  id_symbol = lookup(tmp_node->left->id, status->current_symbol_table);

  if (tmp_node->kind == A_DEREF ||
      tmp_node2->kind == A_DEREF) { // if we tried to dereference a pointer
    if (tmp_node2->left->kind ==
        A_INDEX) { // check if this pointer is an array index

      push_to_instruction_array(
          status, make_instruction(
                      O_POP,
                      make_argument(make_target(REG, 12, NULL),
                                    make_mode(DIRECT, 0), NULL),
                      NULL, NULL, "popping index expression to temp register"));
      if (id_symbol->category ==
          S_PARAM) { // Check if this array index is a parameter
        int param_offset =
            ((status->current_symbol_table->number_of_parameters) -
             (offset - 1));
        push_to_instruction_array(
            status,
            make_instruction(O_MOVE,
                             make_argument(make_target(RBP, NULL, NULL),
                                           make_mode(INDIRECT_RELATIVE,
                                                     24 + param_offset * 8),
                                           NULL),
                             make_argument(make_target(REG, 13, NULL),
                                           make_mode(DIRECT, 0), NULL),
                             NULL, "Move array pointer to temp register "));
      } else {
        push_to_instruction_array(
            status, make_instruction(
                        O_MOVE,
                        make_argument(
                            make_target(RBP, NULL, NULL),
                            make_mode(INDIRECT_RELATIVE, (-offset) * 8), NULL),
                        make_argument(make_target(REG, 13, NULL),
                                      make_mode(DIRECT, 0), NULL),
                        NULL, "Move array pointer to temp register "));
      }

      if (get_evaluation_type_of_subtree(tmp_node2->left->left)->type ==
          A_STRING) {
        push_to_instruction_array(
            status, make_instruction(O_MOVE,
                                     make_argument(make_target(IMI, NULL, NULL),
                                                   make_mode(DIRECT, 0), 1),
                                     make_argument(make_target(REG, 2, NULL),
                                                   make_mode(DIRECT, 0), NULL),
                                     NULL,
                                     "Move value of 1 for multiplying into "
                                     "register to calculate offset "));

      } else {
        push_to_instruction_array(
            status, make_instruction(O_MOVE,
                                     make_argument(make_target(IMI, NULL, NULL),
                                                   make_mode(DIRECT, 0), 8),
                                     make_argument(make_target(REG, 2, NULL),
                                                   make_mode(DIRECT, 0), NULL),
                                     NULL,
                                     "Move value of 8 for multiplying into "
                                     "register to calculate offset "));
      }

      push_to_instruction_array(
          status, make_instruction(O_MUL,
                                   make_argument(make_target(REG, 12, NULL),
                                                 make_mode(DIRECT, 0), NULL),
                                   make_argument(make_target(REG, 2, NULL),
                                                 make_mode(DIRECT, 0), NULL),
                                   NULL, "carry out the operation"));

      push_to_instruction_array(
          status, make_instruction(O_ADD,
                                   make_argument(make_target(REG, 2, NULL),
                                                 make_mode(DIRECT, 0), NULL),
                                   make_argument(make_target(REG, 13, NULL),
                                                 make_mode(DIRECT, 0), NULL),
                                   NULL,
                                   "Move value into register move dereferenced "
                                   "pointer reference to offset "));

      if (get_evaluation_type_of_subtree(tmp_node2->left->left)->type ==
          A_STRING) {
        push_to_instruction_array(
            status,
            make_instruction(O_MOVEZ,
                             make_argument(make_target(REG, 13, NULL),
                                           make_mode(INDIRECT, 0), NULL),
                             make_argument(make_target(REG, 13, NULL),
                                           make_mode(DIRECT, 0), NULL),
                             NULL, "Dereferencing index position once "));
      } else {
        push_to_instruction_array(
            status,
            make_instruction(O_MOVE,
                             make_argument(make_target(REG, 13, NULL),
                                           make_mode(INDIRECT, 0), NULL),
                             make_argument(make_target(REG, 13, NULL),
                                           make_mode(DIRECT, 0), NULL),
                             NULL, "Dereferencing index position once "));
      }

      push_to_instruction_array(
          status, make_instruction(
                      O_MOVE,
                      make_argument(make_target(RRT, 1, NULL),
                                    make_mode(DIRECT, 0), NULL),
                      make_argument(make_target(REG, 13, NULL),
                                    make_mode(INDIRECT, 0), NULL),
                      NULL, "Move the value from reg 1 into index pointer "));
    } else {
      push_to_instruction_array(
          status,
          make_instruction(
              O_MOVE,
              make_argument(make_target(RBP, 1, NULL),
                            make_mode(INDIRECT_RELATIVE, (-offset) * 8), NULL),
              make_argument(make_target(REG, 13, NULL), make_mode(DIRECT, 0),
                            NULL),
              NULL, "Move the value from reg 1 into the address at "));

      push_to_instruction_array(
          status,
          make_instruction(
              O_MOVE,
              make_argument(make_target(RRT, 1, NULL), make_mode(DIRECT, 0),
                            NULL),
              make_argument(make_target(REG, 13, NULL), make_mode(INDIRECT, 0),
                            NULL),
              NULL, "Move the value from reg 1 into dereferenced pointer"));
    }
  } else if (tmp_node->kind ==
             A_INDEX) { // If we are an array and trying to index into it

    push_to_instruction_array(
        status, make_instruction(O_POP,
                                 make_argument(make_target(REG, 12, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, NULL,
                                 "popping index expression to temp register"));

    if (id_symbol->category == S_PARAM) { // if this array is stored as a
                                          // parameter of the current function

      int param_offset =
          ((status->current_symbol_table->number_of_parameters) - (offset - 1));
      push_to_instruction_array(
          status,
          make_instruction(
              O_MOVE,
              make_argument(make_target(RBP, NULL, NULL),
                            make_mode(INDIRECT_RELATIVE, 24 + param_offset * 8),
                            NULL),
              make_argument(make_target(REG, 13, NULL), make_mode(DIRECT, 0),
                            NULL),
              NULL, "Move array pointer to temp register "));
    } else {
      push_to_instruction_array(
          status,
          make_instruction(
              O_MOVE,
              make_argument(make_target(RBP, NULL, NULL),
                            make_mode(INDIRECT_RELATIVE, (-offset) * 8), NULL),
              make_argument(make_target(REG, 13, NULL), make_mode(DIRECT, 0),
                            NULL),
              NULL, "Move array pointer to temp register "));
    }

    if (get_evaluation_type_of_subtree(tmp_node->left)->type == A_STRING ||
        get_evaluation_type_of_subtree(tmp_node->left)->pointer->type ==
            A_CHAR) {
      push_to_instruction_array(
          status, make_instruction(O_MOVE,
                                   make_argument(make_target(IMI, NULL, NULL),
                                                 make_mode(DIRECT, 0), 1),
                                   make_argument(make_target(REG, 2, NULL),
                                                 make_mode(DIRECT, 0), NULL),
                                   NULL,
                                   "Move value of 1 for multiplying into "
                                   "register to calculate offset "));

    } else {
      push_to_instruction_array(
          status, make_instruction(O_MOVE,
                                   make_argument(make_target(IMI, NULL, NULL),
                                                 make_mode(DIRECT, 0), 8),
                                   make_argument(make_target(REG, 2, NULL),
                                                 make_mode(DIRECT, 0), NULL),
                                   NULL,
                                   "Move value of 8 for multiplying into "
                                   "register to calculate offset "));
    }

    push_to_instruction_array(
        status, make_instruction(O_MUL,
                                 make_argument(make_target(REG, 12, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 make_argument(make_target(REG, 2, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, "carry out the operation"));

    push_to_instruction_array(
        status,
        make_instruction(
            O_ADD,
            make_argument(make_target(REG, 2, NULL), make_mode(DIRECT, 0),
                          NULL),
            make_argument(make_target(REG, 13, NULL), make_mode(DIRECT, 0),
                          NULL),
            NULL,
            "Move value into register move pointer reference to offset "));

    push_to_instruction_array(
        status,
        make_instruction(
            O_MOVE,
            make_argument(make_target(RRT, 1, NULL), make_mode(DIRECT, 0),
                          NULL),
            make_argument(make_target(REG, 13, NULL), make_mode(INDIRECT, 0),
                          NULL),
            NULL, "Move the value from reg 1 into index pointer DIRECT"));

  } else if (operator->left->kind == A_DOT) {
    id_symbol = lookup(operator->left->left->id, status->current_symbol_table);
    if (id_symbol->category ==
        S_PARAM) { // If we are a parameter, we need to find out offset in the
                   // previous stack frame instead of for a local var

      int param_offset =
          ((status->current_symbol_table->number_of_parameters) - (offset - 1));
      push_to_instruction_array(
          status,
          make_instruction(
              O_MOVE,
              make_argument(make_target(RBP, NULL, NULL),
                            make_mode(INDIRECT_RELATIVE, 24 + param_offset * 8),
                            NULL),
              make_argument(make_target(REG, 13, NULL), make_mode(DIRECT, 0),
                            NULL),
              NULL, "Move struct pointer to temp register "));

    } else {
      push_to_instruction_array(
          status,
          make_instruction(
              O_MOVE,
              make_argument(make_target(RBP, NULL, NULL),
                            make_mode(INDIRECT_RELATIVE, (-offset) * 8), NULL),
              make_argument(make_target(REG, 13, NULL), make_mode(DIRECT, 0),
                            NULL),
              NULL, "Move struct pointer to temp register "));
    }

    int struct_member_offset = get_field_offset_in_struct(
        operator->left->left->id, operator->left->right->id,
        tmp_node->left->symbol_table);

    push_to_instruction_array(
        status,
        make_instruction(O_MOVE,
                         make_argument(make_target(RRT, 1, NULL),
                                       make_mode(DIRECT, 0), NULL),
                         make_argument(make_target(REG, 13, NULL),
                                       make_mode(INDIRECT_RELATIVE,
                                                 struct_member_offset * 8),
                                       NULL),
                         NULL, "Move the value from reg 1 into struct member"));

  } else if (id_symbol->category ==
             S_PARAM) { // If the identifier is a parameter, we find it's offset
                        // in the previous stackframe
    int param_offset =
        ((status->current_symbol_table->number_of_parameters) - (offset - 1));
    push_to_instruction_array(
        status,
        make_instruction(
            O_MOVE,
            make_argument(make_target(RRT, 1, NULL), make_mode(DIRECT, 0),
                          NULL),
            make_argument(make_target(RBP, 1, NULL),
                          make_mode(INDIRECT_RELATIVE, 24 + (param_offset) * 8),
                          NULL),
            NULL,
            "Move the value from reg 1 into the address at RBP + offset"));
  } else {
    offset = get_symbol_offset_in_stackframe(tmp_node->left->id,
                                             status->current_symbol_table) +
             1;

    push_to_instruction_array(
        status,
        make_instruction(
            O_MOVE,
            make_argument(make_target(RRT, 1, NULL), make_mode(DIRECT, 0),
                          NULL),
            make_argument(make_target(RBP, 1, NULL),
                          make_mode(INDIRECT_RELATIVE, (-offset) * 8), NULL),
            NULL,
            "Move the value from reg 1 into the address at RBP + offset"));
  }

  if (count != 0) {
    status->current_symbol_table = tmp;
    follow_static_link_postvisit(status);
  }
}

/*                            END OF SCOPING CODE */

/*                            START OF FUNCTION CODE */

/* Defines the operations needed to declare a function */
void function_operator(struct CODE_GENERATION_STATUS *status,
                       struct AST_NODE *func) {
  push_to_instruction_array(status,
                            make_instruction(O_FUNC_DEF, NULL, NULL, NULL,
                                             "Initialize function definition"

                                             ));

  char *function_label =
      label_to_string(status->current_symbol_table->id, func->left->right->id);
  char *func_end_label;
  asprintf(&func_end_label, "end_%s", function_label);
  struct SYMBOL *func_symbol =
      lookup_in_scope(func->left->right->id, status->current_symbol_table);
  func_symbol->function_label = function_label;

  push_to_instruction_array(
      status,
      make_instruction(O_LABEL,
                       make_argument(make_target(MEM, NULL, function_label),
                                     make_mode(DIRECT, 0), NULL),
                       NULL, NULL, "func start label"

                       ));

  scope_previsit(status, func->right);

  generate_code_inner(status, func->right->left);

  generate_code_inner(status, func->right->mid);

  generate_code_inner(status, func->right->right);

  push_to_instruction_array(
      status,
      make_instruction(O_LABEL,
                       make_argument(make_target(MEM, NULL, func_end_label),
                                     make_mode(DIRECT, 0), NULL),
                       NULL, NULL, "func end label"

                       ));

  scope_postvisit(status);

  push_to_instruction_array(
      status, make_instruction(O_RET, NULL, NULL, NULL, "return to caller"

                               ));

  push_to_instruction_array(status,
                            make_instruction(O_FUNC_DEF_END, NULL, NULL, NULL,
                                             "end function definition"

                                             ));
}

/* Defines the operations to be performed when returning from a function */
void ret_operator(struct CODE_GENERATION_STATUS *status) {
  int temp = status->instructions_length - 1;

  while (status->instruction_array[temp]->operator!=
         O_FUNC_DEF) { // Find the end of function label
    temp--;
  }
  temp++;

  char *func_end_label;
  asprintf(&func_end_label, "end_%s",
           status->instruction_array[temp]->arg1->target->label);

  push_to_instruction_array(
      status,
      make_instruction(
          O_POP,
          make_argument(make_target(REG, 13, NULL), make_mode(DIRECT, 0), NULL),
          NULL, NULL, "popping return expression value to return register"));

  push_to_instruction_array(
      status,
      make_instruction(O_JMP,
                       make_argument(make_target(MEM, NULL, func_end_label),
                                     make_mode(DIRECT, 0), NULL),
                       NULL, NULL, "jump to end_function label"

                       ));
}

/* Emits intermediate code for callables, in short, it pushes parameters to the
 * stack which are inserted into the function being called's scope as local
 * variables (call-by-value) */
void call_function(struct CODE_GENERATION_STATUS *status,
                   struct AST_NODE *call) {
  struct SYMBOL_TABLE *head = status->current_symbol_table;
  int count = 0;

  if (head == NULL) {
    return;
  }
  do {
    if (lookup_in_scope(call->left->id, head) != NULL) {
      break;
    }
    head = head->parent;
    count++;
  } while (head != NULL);

  char *function_label = label_to_string(head->id, call->left->id);

  generate_code_inner(status, call->right);

  push_to_instruction_array(
      status, make_instruction(O_PUSH,
                               make_argument(make_target(RBP, NULL, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, NULL,
                               "pushing context for return call in function"));

  if (get_evaluation_type_of_subtree(call->left)->type == A_CALLABLE &&
      lookup(call->left->id, status->current_symbol_table)->category !=
          S_FUNC) { // Check if we tried to call a callable var instead of
                    // direct to a func
    int offset = get_symbol_offset_in_stackframe(call->left->id,
                                                 status->current_symbol_table) +
                 1;
    char *temp;
    asprintf(&temp, "Calling function stored in variable %s ", call->left->id);
    struct SYMBOL *id_symbol;
    id_symbol = lookup(call->left->id, status->current_symbol_table);
    if (id_symbol->category ==
        S_PARAM) { // Check if the callable is a parameter instead of a local
                   // variable

      int param_offset =
          ((status->current_symbol_table->number_of_parameters) - (offset - 1));

      push_to_instruction_array(
          status,
          make_instruction(
              O_CALL,
              make_argument(make_target(RBP, -1, NULL),
                            make_mode(INDIRECT_RELATIVE, 24 + param_offset * 8),
                            NULL),
              NULL, NULL, temp));
    } else {
      push_to_instruction_array(
          status,
          make_instruction(
              O_CALL,
              make_argument(make_target(RBP, -1, NULL),
                            make_mode(INDIRECT_RELATIVE, (-offset) * 8), NULL),
              NULL, NULL, temp));
    }
  } else {
    push_to_instruction_array(
        status,
        make_instruction(O_CALL,
                         make_argument(make_target(MEM, NULL, function_label),
                                       make_mode(DIRECT, 0), NULL),
                         NULL, NULL, "done with function"

                         ));
  }

  struct AST_NODE *params = call->right->left;
  int num_params = 0;
  while (params != NULL &&
         params->kind !=
             A_EMPTY) { // Traverse the list of parameters to know how much
                        // space to dealloc from the stack after a call
    if (params->kind == A_GLUE) {
      num_params++;
      params = params->right;

    } else if (params->left->kind == A_ID || A_PARAM) {

      num_params++;
      break;
    }
  }
  push_to_instruction_array(
      status,
      make_instruction(
          O_ADD,
          make_argument(make_target(IMI, NULL, NULL), make_mode(DIRECT, 0),
                        8 + (num_params * 8)),
          make_argument(make_target(RSP, NULL, NULL), make_mode(DIRECT, 0),
                        NULL),
          NULL,
          "remove RBP and extra space from top of stack after function call "));

  push_to_instruction_array(
      status,
      make_instruction(
          O_PUSH,
          make_argument(make_target(REG, 13, 0), make_mode(DIRECT, 0), NULL),
          NULL, NULL, "pushing return value from function to top of stack"));
}

/*                            END OF FUNCTION CODE */

/*                            BEGGINING OF DYNAMIC MEMORY CODE */

/* Pushes an adress to the top of the stack */
void address_of(struct CODE_GENERATION_STATUS *status,
                struct AST_NODE *address_node) {

  int offset = get_symbol_offset_in_stackframe(address_node->left->id,
                                               status->current_symbol_table) +
               1;
  char *temp;
  asprintf(&temp, "Moving address of variable %s into temp register ",
           address_node->left->id);
  push_to_instruction_array(
      status,
      make_instruction(
          O_LEAQ,
          make_argument(make_target(RBP, 1, NULL),
                        make_mode(INDIRECT_RELATIVE, (-offset) * 8), NULL),
          make_argument(make_target(REG, 13, NULL), make_mode(DIRECT, 0), NULL),
          NULL, temp));

  push_to_instruction_array(
      status,
      make_instruction(
          O_PUSH,
          make_argument(make_target(REG, 13, 0), make_mode(DIRECT, 0), NULL),
          NULL, NULL, "Push pointer value to top of the stack"));
}

/* Tries to dereference a pointer assumed to be a the top of the stack */
void deref_operator(struct CODE_GENERATION_STATUS *status,
                    struct AST_NODE *deref_node) {
  struct SYMBOL_TABLE *head = status->current_symbol_table;
  struct AST_NODE *tmp = deref_node;
  while (tmp->left->kind != A_ID) {
    tmp = tmp->left;
  }
  if (tmp->kind == A_INDEX) { // Check if we are trying to dereference an index
                              // (array) node instead of a normal pointer

    generate_code_inner(status, tmp->right);
    int count = 0;
    int offset = 0;

    if (head == NULL) {
      return;
    }
    // Finds the scope out if the identifier is in the current scope or a parent
    // scope
    do {

      if (lookup_in_scope(tmp->left->id, head) != NULL) {

        break;
      }
      head = head->parent;
      count++;
    } while (head != NULL);
    struct SYMBOL_TABLE *tmp_sym = status->current_symbol_table;
    if (count != 0) {
      follow_static_link_previsit(status);
      for (int i = 0; i < count; i++) {
        follow_static_link(status);
        status->current_symbol_table = status->current_symbol_table->parent;
      }
      offset = get_symbol_offset_in_stackframe(tmp->left->id, head) + 1;
    } else {
      offset = get_symbol_offset_in_stackframe(tmp->left->id,
                                               status->current_symbol_table) +
               1;
    }

    struct SYMBOL *id_symbol;
    id_symbol = lookup(tmp->left->id, status->current_symbol_table);

    push_to_instruction_array(
        status, make_instruction(O_POP,
                                 make_argument(make_target(REG, 12, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, NULL,
                                 "popping index expression to temp register"));
    if (id_symbol->category ==
        S_PARAM) { // This array might be a parameter, and if so, we need to get
                   // the param offset instead
      int param_offset =
          ((status->current_symbol_table->number_of_parameters) - (offset - 1));
      push_to_instruction_array(
          status,
          make_instruction(
              O_MOVE,
              make_argument(make_target(RBP, NULL, NULL),
                            make_mode(INDIRECT_RELATIVE, 24 + param_offset * 8),
                            NULL),
              make_argument(make_target(REG, 13, NULL), make_mode(DIRECT, 0),
                            NULL),
              NULL, "Move array pointer to temp register "));

    } else {
      push_to_instruction_array(
          status,
          make_instruction(
              O_MOVE,
              make_argument(make_target(RBP, NULL, NULL),
                            make_mode(INDIRECT_RELATIVE, (-offset) * 8), NULL),
              make_argument(make_target(REG, 13, NULL), make_mode(DIRECT, 0),
                            NULL),
              NULL, "Move array pointer to temp register "));
    }

    if (get_evaluation_type_of_subtree(tmp->left)->type == A_STRING) {
      push_to_instruction_array(
          status, make_instruction(O_MOVE,
                                   make_argument(make_target(IMI, NULL, NULL),
                                                 make_mode(DIRECT, 0), 1),
                                   make_argument(make_target(REG, 2, NULL),
                                                 make_mode(DIRECT, 0), NULL),
                                   NULL,
                                   "Move value of 1 for multiplying into "
                                   "register to calculate offset "));

    } else {
      push_to_instruction_array(
          status, make_instruction(O_MOVE,
                                   make_argument(make_target(IMI, NULL, NULL),
                                                 make_mode(DIRECT, 0), 8),
                                   make_argument(make_target(REG, 2, NULL),
                                                 make_mode(DIRECT, 0), NULL),
                                   NULL,
                                   "Move value of 8 for multiplying into "
                                   "register to calculate offset "));
    }

    push_to_instruction_array(
        status, make_instruction(O_MUL,
                                 make_argument(make_target(REG, 12, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 make_argument(make_target(REG, 2, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, "carry out the operation"));

    push_to_instruction_array(
        status,
        make_instruction(
            O_ADD,
            make_argument(make_target(REG, 2, NULL), make_mode(DIRECT, 0),
                          NULL),
            make_argument(make_target(REG, 13, NULL), make_mode(DIRECT, 0),
                          NULL),
            NULL,
            "Move value into register move pointer reference to offset "));

    if (get_evaluation_type_of_subtree(tmp->left)->type == A_STRING) {
      push_to_instruction_array(
          status, make_instruction(O_MOVEZ,
                                   make_argument(make_target(REG, 13, NULL),
                                                 make_mode(INDIRECT, 0), NULL),
                                   make_argument(make_target(REG, 13, NULL),
                                                 make_mode(DIRECT, 0), NULL),
                                   NULL, "Dereferencing index position once "));
    } else {
      push_to_instruction_array(
          status, make_instruction(O_MOVE,
                                   make_argument(make_target(REG, 13, NULL),
                                                 make_mode(INDIRECT, 0), NULL),
                                   make_argument(make_target(REG, 13, NULL),
                                                 make_mode(DIRECT, 0), NULL),
                                   NULL, "Dereferencing index position once "));
    }

    push_to_instruction_array(
        status, make_instruction(O_MOVE,
                                 make_argument(make_target(REG, 13, NULL),
                                               make_mode(INDIRECT, 0), NULL),
                                 make_argument(make_target(REG, 13, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, "derefencing pointer at index offset"));

    push_to_instruction_array(
        status,
        make_instruction(
            O_PUSH,
            make_argument(make_target(REG, 13, 0), make_mode(DIRECT, 0), NULL),
            NULL, NULL, "Push pointer dereferenced value to top of the stack"));

    if (count != 0) {
      status->current_symbol_table = tmp_sym;
      follow_static_link_postvisit(status);
    }

    return;
  }

  int count = 0;
  int offset = 0;

  if (head == NULL) {
    return;
  }
  // Finds the scope out if the identifier is in the current scope or a parent
  // scope
  do {
    if (lookup_in_scope(deref_node->left->id, head) != NULL) {

      break;
    }
    head = head->parent;
    count++;
  } while (head != NULL);
  struct SYMBOL_TABLE *tmp_sym = status->current_symbol_table;
  if (count != 0) {
    follow_static_link_previsit(status);
    for (int i = 0; i < count; i++) {
      follow_static_link(status);
      status->current_symbol_table = status->current_symbol_table->parent;
    }
    offset = get_symbol_offset_in_stackframe(deref_node->left->id, head) + 1;
  } else {
    offset = get_symbol_offset_in_stackframe(deref_node->left->id,
                                             status->current_symbol_table) +
             1;
  }

  char *temp;
  asprintf(&temp, "Moving pointer %s into temp register ", tmp->left->id);

  push_to_instruction_array(
      status,
      make_instruction(
          O_MOVE,
          make_argument(make_target(RBP, 1, NULL),
                        make_mode(INDIRECT_RELATIVE, (-offset) * 8), NULL),
          make_argument(make_target(REG, 13, NULL), make_mode(DIRECT, 0), NULL),
          NULL, temp));
  push_to_instruction_array(
      status, make_instruction(O_MOVE,
                               make_argument(make_target(REG, 13, NULL),
                                             make_mode(INDIRECT, 0), NULL),
                               make_argument(make_target(REG, 13, NULL),
                                             make_mode(DIRECT, 0), NULL),
                               NULL, "derefencing temp register"));

  push_to_instruction_array(
      status,
      make_instruction(
          O_PUSH,
          make_argument(make_target(REG, 13, 0), make_mode(DIRECT, 0), NULL),
          NULL, NULL, "Push pointer dereferenced value to top of the stack"));

  if (count != 0) {
    status->current_symbol_table = tmp_sym;
    follow_static_link_postvisit(status);
  }
}

/* Reserves the specified amount from the heap pointer */
void allocate_operator(struct CODE_GENERATION_STATUS *status,
                       struct AST_NODE *node) {

  if (node->left->kind == A_STRUCT) {
    struct SYMBOL *sym =
        lookup(node->left->left->id, status->current_symbol_table);

    push_to_instruction_array(
        status, make_instruction(
                    O_POP,
                    make_argument(make_target(REG, 12, NULL),
                                  make_mode(DIRECT, 0), NULL),
                    NULL, NULL, "popping amount to allocate to temp register"));

    push_to_instruction_array(
        status, make_instruction(
                    O_MOVE,
                    make_argument(make_target(IMI, NULL, NULL),
                                  make_mode(DIRECT, 0), sym->type->size),
                    make_argument(make_target(REG, 1, NULL),
                                  make_mode(DIRECT, 0), NULL),
                    NULL, "Move requested size parameter intro register "));

    push_to_instruction_array(
        status, make_instruction(O_MUL,
                                 make_argument(make_target(REG, 1, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 make_argument(make_target(REG, 12, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, "carry out the operation"));

  } else {
    push_to_instruction_array(
        status, make_instruction(O_POP,
                                 make_argument(make_target(REG, 12, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, NULL,
                                 "popping size to allocate to temp register"));
  }

  push_to_instruction_array(
      status,
      make_instruction(
          O_PUSH,
          make_argument(make_target(RSL, 0, 0), make_mode(DIRECT, 0), NULL),
          NULL, NULL, "Push heap frame value to top of the stack"));

  push_to_instruction_array(
      status,
      make_instruction(
          O_MOVE,
          make_argument(make_target(REG, 12, NULL), make_mode(DIRECT, 0), 8),
          make_argument(make_target(REG, 1, NULL), make_mode(DIRECT, 0), NULL),
          NULL, "Move requested size parameter intro register "));

  if (node->left->kind == A_CHAR) {
    push_to_instruction_array(
        status, make_instruction(O_MOVE,
                                 make_argument(make_target(IMI, NULL, NULL),
                                               make_mode(DIRECT, 0), 1),
                                 make_argument(make_target(REG, 2, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL,
                                 "Move value of 1 for multiplying into "
                                 "register to calculate offset "));

  } else {
    push_to_instruction_array(
        status, make_instruction(O_MOVE,
                                 make_argument(make_target(IMI, NULL, NULL),
                                               make_mode(DIRECT, 0), 8),
                                 make_argument(make_target(REG, 2, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL,
                                 "Move value of 8 for multiplying into "
                                 "register to calculate offset "));
  }

  push_to_instruction_array(
      status,
      make_instruction(
          O_MUL,
          make_argument(make_target(REG, 1, NULL), make_mode(DIRECT, 0), NULL),
          make_argument(make_target(REG, 2, NULL), make_mode(DIRECT, 0), NULL),
          NULL, "carry out the operation"));

  push_to_instruction_array(
      status,
      make_instruction(
          O_ADD,
          make_argument(make_target(REG, 2, NULL), make_mode(DIRECT, 0), NULL),
          make_argument(make_target(RSL, NULL, NULL), make_mode(DIRECT, 0),
                        NULL),
          NULL, " Push heap space to account for requested block of memory "));
}

/* Pushes a pointer to an array at a specific index to the stack */
void index_operator(struct CODE_GENERATION_STATUS *status,
                    struct AST_NODE *node) {
  generate_code_inner(status, node->right);
  struct SYMBOL_TABLE *head = status->current_symbol_table;

  int count = 0;
  int offset = 0;

  if (head == NULL) {
    return;
  }
  // Finds the scope out if the identifier is in the current scope or a parent
  // scope
  do {
    if (lookup_in_scope(node->left->id, head) != NULL) {

      break;
    }
    head = head->parent;
    count++;
  } while (head != NULL);
  struct SYMBOL_TABLE *tmp = status->current_symbol_table;
  if (count != 0) {
    follow_static_link_previsit(status);
    for (int i = 0; i < count; i++) {
      follow_static_link(status);
      status->current_symbol_table = status->current_symbol_table->parent;
    }
    offset = get_symbol_offset_in_stackframe(node->left->id, head) + 1;
  } else {
    offset = get_symbol_offset_in_stackframe(node->left->id,
                                             status->current_symbol_table) +
             1;
  }

  struct SYMBOL *id_symbol;
  id_symbol = lookup(node->left->id, status->current_symbol_table);

  push_to_instruction_array(
      status,
      make_instruction(
          O_POP,
          make_argument(make_target(REG, 12, NULL), make_mode(DIRECT, 0), NULL),
          NULL, NULL, "popping index expression to temp register"));
  if (id_symbol->category ==
      S_PARAM) { // I we are a parameter, we need to find out offset in the
                 // previous stack frame instead of for a local var

    int param_offset =
        ((status->current_symbol_table->number_of_parameters) - (offset - 1));
    push_to_instruction_array(
        status, make_instruction(O_MOVE,
                                 make_argument(make_target(RBP, NULL, NULL),
                                               make_mode(INDIRECT_RELATIVE,
                                                         24 + param_offset * 8),
                                               NULL),
                                 make_argument(make_target(REG, 13, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, "Move array pointer to temp register "));

  } else {
    push_to_instruction_array(
        status,
        make_instruction(
            O_MOVE,
            make_argument(make_target(RBP, NULL, NULL),
                          make_mode(INDIRECT_RELATIVE, (-offset) * 8), NULL),
            make_argument(make_target(REG, 13, NULL), make_mode(DIRECT, 0),
                          NULL),
            NULL, "Move array pointer to temp register "));
  }

  if (get_evaluation_type_of_subtree(node->left)->type == A_STRING ||
      get_evaluation_type_of_subtree(node->left)->pointer->type == A_CHAR) {
    push_to_instruction_array(
        status, make_instruction(O_MOVE,
                                 make_argument(make_target(IMI, NULL, NULL),
                                               make_mode(DIRECT, 0), 1),
                                 make_argument(make_target(REG, 2, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL,
                                 "Move value of 1 for multiplying into "
                                 "register to calculate offset "));

  } else {
    push_to_instruction_array(
        status, make_instruction(O_MOVE,
                                 make_argument(make_target(IMI, NULL, NULL),
                                               make_mode(DIRECT, 0), 8),
                                 make_argument(make_target(REG, 2, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL,
                                 "Move value of 8 for multiplying into "
                                 "register to calculate offset "));
  }

  push_to_instruction_array(
      status,
      make_instruction(
          O_MUL,
          make_argument(make_target(REG, 12, NULL), make_mode(DIRECT, 0), NULL),
          make_argument(make_target(REG, 2, NULL), make_mode(DIRECT, 0), NULL),
          NULL, "carry out the operation"));

  push_to_instruction_array(
      status,
      make_instruction(
          O_ADD,
          make_argument(make_target(REG, 2, NULL), make_mode(DIRECT, 0), NULL),
          make_argument(make_target(REG, 13, NULL), make_mode(DIRECT, 0), NULL),
          NULL, "Move value into register move pointer reference to offset "));
  if (get_evaluation_type_of_subtree(node->left)->type == A_STRING) {
    push_to_instruction_array(
        status, make_instruction(O_MOVEZ,
                                 make_argument(make_target(REG, 13, NULL),
                                               make_mode(INDIRECT, 0), NULL),
                                 make_argument(make_target(REG, 13, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, "Dereferencing index position once "));
  } else {
    push_to_instruction_array(
        status, make_instruction(O_MOVE,
                                 make_argument(make_target(REG, 13, NULL),
                                               make_mode(INDIRECT, 0), NULL),
                                 make_argument(make_target(REG, 13, NULL),
                                               make_mode(DIRECT, 0), NULL),
                                 NULL, "Dereferencing index position once "));
  }
  push_to_instruction_array(
      status,
      make_instruction(
          O_PUSH,
          make_argument(make_target(REG, 13, 0), make_mode(DIRECT, 0), NULL),
          NULL, NULL, "Push index pointer value to top of the stack"));

  if (count != 0) {
    status->current_symbol_table = tmp;
    follow_static_link_postvisit(status);
  }
}

/* Retrived the specified element from a data structure */
void dot_operator(struct CODE_GENERATION_STATUS *status,
                  struct AST_NODE *node) {

  struct SYMBOL_TABLE *head = status->current_symbol_table;

  int count = 0;
  int offset = 0;

  if (head == NULL) {
    return;
  }
  // Finds the scope out if the identifier is in the current scope or a parent
  // scope
  do {
    if (lookup_in_scope(node->left->id, head) != NULL) {

      break;
    }
    head = head->parent;
    count++;
  } while (head != NULL);
  struct SYMBOL_TABLE *tmp = status->current_symbol_table;
  if (count != 0) {
    follow_static_link_previsit(status);
    for (int i = 0; i < count; i++) {
      follow_static_link(status);
      status->current_symbol_table = status->current_symbol_table->parent;
    }
    offset = get_symbol_offset_in_stackframe(node->left->id, head) + 1;
  } else {
    offset = get_symbol_offset_in_stackframe(node->left->id,
                                             status->current_symbol_table) +
             1;
  }

  struct SYMBOL *id_symbol;
  id_symbol = lookup(node->left->id, status->current_symbol_table);

  if (id_symbol->category ==
      S_PARAM) { // If we are a parameter, we need to find out offset in the
                 // previous stack frame instead of for a local var

    int param_offset =
        ((status->current_symbol_table->number_of_parameters) - (offset - 1));
    push_to_instruction_array(
        status,
        make_instruction(
            O_MOVE,
            make_argument(make_target(RBP, NULL, NULL),
                          make_mode(INDIRECT_RELATIVE, 24 + param_offset * 8),
                          NULL),
            make_argument(make_target(REG, 13, NULL), make_mode(DIRECT, 0),
                          NULL),
            NULL, "Move struct pointer to temp register "));

  } else {
    push_to_instruction_array(
        status,
        make_instruction(
            O_MOVE,
            make_argument(make_target(RBP, NULL, NULL),
                          make_mode(INDIRECT_RELATIVE, (-offset) * 8), NULL),
            make_argument(make_target(REG, 13, NULL), make_mode(DIRECT, 0),
                          NULL),
            NULL, "Move struct pointer to temp register "));
  }

  int struct_member_offset = get_field_offset_in_struct(
      node->left->id, node->right->id, status->current_symbol_table);

  push_to_instruction_array(
      status,
      make_instruction(
          O_MOVE,
          make_argument(make_target(REG, 13, NULL),
                        make_mode(INDIRECT_RELATIVE, struct_member_offset * 8),
                        NULL),
          make_argument(make_target(REG, 13, NULL), make_mode(DIRECT, 0), NULL),
          NULL, "Deref once to get to value pointed to at index "));

  push_to_instruction_array(
      status,
      make_instruction(
          O_PUSH,
          make_argument(make_target(REG, 13, 0), make_mode(DIRECT, 0), NULL),
          NULL, NULL, "Push index pointer value to top of the stack"));

  if (count != 0) {
    status->current_symbol_table = tmp;
    follow_static_link_postvisit(status);
  }
}

/*                            END OF DYNAMIC MEMORY CODE */

/* Main working recursion of codegen. Takes in the status from the entry point,
   and traverses the AST, generating instructions along the way*/
void generate_code_inner(struct CODE_GENERATION_STATUS *status,
                         struct AST_NODE *tree) {
  if (tree == NULL || tree->kind == NULL ||
      status->current_symbol_table == NULL) {
    return;
  }

  if (tree->kind == A_BLOCK) {
    scope_previsit(status, tree);
    generate_code_inner(status, tree->left);
    generate_code_inner(status, tree->mid);
    generate_code_inner(status, tree->right);
    scope_postvisit(status);
    return;
  }

  if (tree->kind == A_DEF_FUNC) {
    function_operator(status, tree);
    return;
  }

  if (tree->kind == A_DECL) {
    return;
  }

  if (tree->kind == A_STRUCT) {
    return;
  }

  if (tree->kind == A_IF) {
    if_operator(status, tree);
    return;
  }

  if (tree->kind == A_CALL_FUNC) {
    call_function(status, tree);
    return;
  }

  if (tree->kind == A_WHILE) {
    while_operator(status, tree);
    return;
  }

  if (tree->kind == A_ASSIGN) {
    generate_code_inner(status, tree->right);
    assignment_operator(status, tree);
    return;
  }

  if (tree->kind == A_DEREF) {
    deref_operator(status, tree);
    return;
  }

  if (tree->kind == A_INDEX) {
    index_operator(status, tree);
    return;
  }

  if (tree->kind == A_DOT) {
    dot_operator(status, tree);
    return;
  }

  generate_code_inner(status, tree->left);
  generate_code_inner(status, tree->mid);
  generate_code_inner(status, tree->right);

  switch (tree->kind) {
  case A_MOD:
    mod_operator(status, tree);
    break;
  case A_ADDRESSOF:
    address_of(status, tree);
    break;
  case A_DIV:
    if (get_evaluation_type_of_subtree(tree->left)->type == A_FLOAT) {
      floating_arithmetic_operator(status, tree);
    } else {
      division_operator(status, tree);
    }
    break;
  case A_PRINT:
    print_operator(status, tree);
    break;
  case A_ADD:
  case A_MUL:
  case A_SUB:
    if (get_evaluation_type_of_subtree(tree->left)->type == A_FLOAT) {
      floating_arithmetic_operator(status, tree);
    } else {
      arithmetic_operator(status, tree);
    }
    break;
  case A_INTVAL:
    integer_literal(status, tree);
    break;
  case A_CHARVAL:
    char_literal(status, tree);
    break;
  case A_FLOATVAL:
    float_literal(status, tree);
    break;
  case A_STRINGVAL:
    string_literal(status, tree);
    break;
  case A_LESS:
    comparison_operator(status, tree, O_JL);
    break;
  case A_GREAT:
    comparison_operator(status, tree, O_JG);
    break;
  case A_EQUAL:
    comparison_operator(status, tree, O_JE);
    break;
  case A_NEQUAL:
    comparison_operator(status, tree, O_JNE);
    break;
  case A_LEQ:
    comparison_operator(status, tree, O_JLE);
    break;
  case A_GEQ:
    comparison_operator(status, tree, O_JGE);
    break;
  case A_ID:
    identifier(status, tree);
    break;
  case A_RETURN:
    ret_operator(status);
    break;
  case A_CONTINUE:
    operand_continue(status, tree);
    break;
  case A_BREAK:
    operand_break(status, tree);
    break;
  case A_CAST:
    cast_operator(status, tree);
    break;
  case A_ALLOCATE:
    allocate_operator(status, tree);
    break;
  default:
    return;
  }
}

/* Main entry point of the code generation, which sets of the status structure
   to be used for the rest of the compilers run-time*/
struct CODE_GENERATION_STATUS *
generate_code(struct CODE_GENERATION_STATUS *status, struct AST_NODE *tree) {
#if DEBUG
  printf("codegen.c :: generate_code :: Beginning the intermediate code "
         "generation\n");
#endif

  program_setup(status, tree);
  generate_code_inner(status, tree);
  program_teardown(status);
}
