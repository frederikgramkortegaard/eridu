/* Definitions used in codegen.c*/

#pragma once

#include "symbols.h"
#include "translater.h"
#include "tree.h"
#include "typechecking.h"
#include "types.h"

#define MAX_NUM_FUNCS 2056

enum Operator {

  /* Standard ops */

  O_JE,
  O_JG,
  O_JL,
  O_ADD,
  O_CMP,
  O_DIV,
  O_JGE,
  O_JLE,
  O_JMP,
  O_JNE,
  O_MUL,
  O_POP,
  O_RET,
  O_SUB,
  O_CALL,
  O_MOVE,
  O_LABEL,
  O_PUSH,

  /* Metas */

  O_CALL_PRINTF,
  O_CALLER_SAVE,
  O_CALLER_RESTORE,
  O_CALLER_EPILOGUE,
  O_CALLER_PROLOGUE,
  O_CALLEE_SAVE,
  O_CALLEE_RESTORE,
  O_CALLEE_EPILOGUE,
  O_CALLEE_PROLOGUE,
  O_MAIN_CALLEE_SAVE,
  O_PROGRAM_EPILOGUE,
  O_PROGRAM_PROLOGUE,
  O_ALLOC_STACK_SPACE,
  O_DEALLOC_STACK_SPACE,
  O_MAIN_CALLEE_RESTORE,
  O_REVERSE_PUSH_ARGUMENTS,
  O_FUNC_DEF,
  O_FUNC_DEF_END,
  O_LEAQ,
  O_FADD,
  O_FSUB,
  O_FMUL,
  O_FDIV,
  O_WHILE,
  O_INT_TO_FLOAT,
  O_FLOAT_TO_INT,
  O_CMPB,
  O_MOVEZ,
  O_MOD,

  /* for use in default for ast_node_to_operator conversions */
  O_EMPTY
};

enum Argument_Target {
  IMS, // Imediate String
  IMF, // Imediate Float
  IMI, // Imediate Int
  IML, // Imediate Label
  MEM, // Memory (a label)
  RBP, // register: base (frame) pointer
  RSP, // register: stack pointer
  RRT, // register: return value
  RSL, // register: static link computation
  REG, // general-purpose registers
  RSI  // Initial Command Line Arguments
};

enum Addresing_Mode { DIRECT, INDIRECT, INDIRECT_RELATIVE };
typedef struct TARGET {
  enum Argument_Target arg_target;
  int register_number;
  char *label;
};

typedef struct MODE {
  enum Addresing_Mode mode;
  int offset;
};

typedef struct ARGUMENT {
  struct TARGET *target;
  struct MODE *mode;
  int literal;
};

typedef struct INSTRUCTION {
  enum Operator operator;

  struct ARGUMENT *arg1;
  struct ARGUMENT *arg2;
  struct ARGUMENT *arg3;

  char *comment;
};

typedef struct FLOAT_DATA {
  char *label;
  float floatval;
} FLOAT_DATA;

typedef struct STRING_DATA {
  char *label;
  char *stringval;
} STRING_DATA;

typedef struct CODE_GENERATION_STATUS {
  struct SYMBOL_TABLE *current_symbol_table;

  struct INSTRUCTION **instruction_array;
  int max_instructions_length;
  int instructions_length;

  struct SYMBOL_TABLE **function_stack;
  int function_stack_max_length;
  int function_stack_length;

  char **labels;
  int max_num_labels;
  int num_labels;

  struct FLOAT_DATA **float_data_array;
  int max_float_data_length;
  int float_array_length;

  struct STRING_DATA **string_data_array;
  int max_string_data_length;
  int string_array_length;

  char **code_sections;

} CODE_GENERATION_STATUS;