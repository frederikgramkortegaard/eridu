#pragma once

#include "codegen.h"

char *intermediate_to_x86(struct INSTRUCTION *instruction,
                          struct CODE_GENERATION_STATUS *status);
unsigned int crc32b_hash_emit(unsigned char *message);
void format_arg(struct ARGUMENT *arg, int comma,
                struct CODE_GENERATION_STATUS *status);
void allocate_stack_space(struct CODE_GENERATION_STATUS *status);
void print_instruction_function(struct CODE_GENERATION_STATUS *status);
void simple_instruction(struct INSTRUCTION *instruction,
                        struct CODE_GENERATION_STATUS *status);
void division_operator_emit(struct CODE_GENERATION_STATUS *status);
void ins(struct CODE_GENERATION_STATUS *status, char *str, int cursor);
void emit_asm(struct CODE_GENERATION_STATUS *status,
              struct FLOAT_DATA **float_data_array);