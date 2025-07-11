/* Uses the CODE_GENERATION_STATUS struct from codegen.c to recursivly generate
   assembly code corrosponding to the intermidiate form */

#include "emit.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "codegen.h"

static int func_cursor = 0;
static char *tmp;
static char *code_sections[MAX_NUM_FUNCS];
static int hash_stack[MAX_NUM_FUNCS];
static int hash_stack_cursor = 1;

/* Convert simple codegen.c :: instructions into assembly instruction, */
char *intermediate_to_x86(struct INSTRUCTION *instruction,
                          struct CODE_GENERATION_STATUS *status) {
  switch (instruction->operator) {
  case O_MOVE:
    return "movq";
  case O_MOVEZ:
    return "movzbq";
  case O_CALL:
    return "callq";
  case O_PUSH:
    return "pushq";
  case O_POP:
    return "popq";
  case O_CMP:
    return "cmpq";
  case O_CMPB:
    return "cmpb";
  case O_JMP:
    return "jmp";
  case O_JE:
    return "je";
  case O_JNE:
    return "jne";
  case O_JL:
    return "jl";
  case O_JLE:
    return "jle";
  case O_JG:
    return "jg";
  case O_JGE:
    return "jge";
  case O_ADD:
    return "addq";
  case O_SUB:
    return "subq";
  case O_MUL:
    return "imulq";
  case O_LEAQ:
    return "leaq";
  case O_EMPTY:

    asprintf(&tmp, "%s", instruction->arg1->target->label);
    return tmp;
  case O_FADD:
    return "addss";
  case O_FSUB:
    return "subss";
  case O_FMUL:
    return "mulss";
  case O_FDIV:
    return "divss";
  default:
    return NULL;
  }
}

/* This is a pretty fast hashing function,
 * so we're happy with it currently */
unsigned int crc32b_hash_emit(unsigned char *message) {
  int i, j;
  unsigned int byte, crc, mask;

  i = 0;
  crc = 0xFFFFFFFF;
  while (message[i] != 0) {
    byte = message[i]; // Get next byte.
    crc = crc ^ byte;
    for (j = 7; j >= 0; j--) { // Do eight times.
      mask = -(crc & 1);
      crc = (crc >> 1) ^ (0xEDB88320 & mask);
    }
    i = i + 1;
  }
  return ~crc % MAX_NUM_FUNCS;
}

/* Format a codegen.c :: ARGUMENT struct into assembly compatible arguments wrt.
 * spacing, commas' etc.*/
void format_arg(struct ARGUMENT *arg, int comma,
                struct CODE_GENERATION_STATUS *status) {
  if (arg == NULL) {
    return;
  }

  if (comma == 1) {
    asprintf(&tmp, ",");
    ins(status, tmp, func_cursor);
  }

  switch (arg->target->arg_target) {
  case IMI:
    asprintf(&tmp, " $%d ", arg->literal);
    ins(status, tmp, func_cursor);
    break;
  case IMF:
    asprintf(&tmp, " %s(%rip) ", arg->target->label);
    ins(status, tmp, func_cursor);
    break;
  case IMS:
    asprintf(&tmp, " $%s ", arg->target->label);
    ins(status, tmp, func_cursor);
    break;
  case REG:
    if (arg->target->register_number == 1) {
      if (arg->target->label == "b") {
        asprintf(&tmp, " %bl ");
        ins(status, tmp, func_cursor);
      } else {
        asprintf(&tmp, " %rbx ");
        ins(status, tmp, func_cursor);
      }
    } else if (arg->target->register_number == 51) {
      asprintf(&tmp, " %bl ");
      ins(status, tmp, func_cursor);
    } else if (arg->target->register_number == 52) {
      asprintf(&tmp, " %%cl ");
      ins(status, tmp, func_cursor);
    } else if (arg->target->register_number == 2) {
      if (arg->target->label == "b") {
        asprintf(&tmp, " \%cl ");
        ins(status, tmp, func_cursor);
      } else {
        asprintf(&tmp, " %rcx ");
        ins(status, tmp, func_cursor);
      }
    } else if (arg->target->register_number == 100) {
      asprintf(&tmp, " %smm0 ", "%x");
      ins(status, tmp, func_cursor);
    } else if (arg->target->register_number == 101) {
      asprintf(&tmp, " %smm1 ", "%x");
      ins(status, tmp, func_cursor);
    } else if (arg->target->register_number == -1) {
      asprintf(&tmp, "");
      ins(status, tmp, func_cursor);
    } else {
      if (arg->mode->mode == INDIRECT) {
        asprintf(&tmp, " (%r%d) ", arg->target->register_number);
        ins(status, tmp, func_cursor);
        break;
      } else if (arg->mode->mode == INDIRECT_RELATIVE) {
        asprintf(&tmp, " %d(%r%d) ", arg->mode->offset,
                 arg->target->register_number);
        ins(status, tmp, func_cursor);
        break;
      }
      asprintf(&tmp, " %r%d ", arg->target->register_number);
      ins(status, tmp, func_cursor);
    }
    break;
  case MEM:
    asprintf(&tmp, " %s", arg->target->label);
    ins(status, tmp, func_cursor);
    break;
  case RBP:
    if (arg->target->register_number == -1) {
      asprintf(&tmp, " *%d(%rbp) ", arg->mode->offset);
      ins(status, tmp, func_cursor);
      break;
    } else if (arg->mode->mode == INDIRECT_RELATIVE) {
      // printf("offset for RBP %d\n", arg->mode->offset);
      asprintf(&tmp, " %d(%rbp) ", arg->mode->offset);
      ins(status, tmp, func_cursor);
      break;
    }
    asprintf(&tmp, " %rbp ");
    ins(status, tmp, func_cursor);
    break;
  case RSL:
    if (arg->mode->mode == INDIRECT) {
      asprintf(&tmp, " (%r%d) ", arg->target->register_number);
      ins(status, tmp, func_cursor);
      break;
    }
    asprintf(&tmp, " %rdx ");
    ins(status, tmp, func_cursor);
    break;
  case RSP:
    if (arg->mode->mode == INDIRECT) {
      asprintf(&tmp, " (%rsp) ");
      ins(status, tmp, func_cursor);
      break;
    }
    asprintf(&tmp, " %rsp ");
    ins(status, tmp, func_cursor);
    break;
  case RRT:
    asprintf(&tmp, " %rax ");
    ins(status, tmp, func_cursor);
    break;
  case RSI:
    asprintf(&tmp, " %rsi ");
    ins(status, tmp, func_cursor);
  default:
    break;
  }
}

/* Pushes a 0 onto the current stack position as represented by the %rsp
 * register. Factored into its own function as to make the code more
 * self-documenting.*/
void allocate_stack_space(struct CODE_GENERATION_STATUS *status) {
  asprintf(&tmp, "pushq (%rsp)\n");
  ins(status, tmp, func_cursor);
}

void print_instruction_function(struct CODE_GENERATION_STATUS *status) {
  static int label_number = 0;

  /* Ugly but works */

  asprintf(&tmp, "movq 0(%rsp), %rsi\n");
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "movq %rsp, %rcx\n");
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "andq $-16, %rsp\n");
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "movq $0, %rbx\n");
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "cmpq %rsp, %rcx\n");
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "je F%d_aligned\n", label_number);
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "incq %rbx\n");
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "F%d_aligned:\n", label_number++);
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "pushq %rbx\n");
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "subq $8, %rsp\n");
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "callq printf@plt\n");
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "addq $8, %rsp\n");
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "popq %rbx\n");
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "cmpq $0, %rbx\n");
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "je F%d_aligned\n", label_number);
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "addq $8, %rsp\n");
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "F%d_aligned:\n", label_number++);
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "addq $8, %rsp\n\n");
  ins(status, tmp, func_cursor);
}

/* Convert a simple codegen.c :: INSTRUCTION struct into valid assemvly
 * instructions */
void simple_instruction(struct INSTRUCTION *instruction,
                        struct CODE_GENERATION_STATUS *status) {
  asprintf(&tmp, intermediate_to_x86(instruction, status));
  ins(status, tmp, func_cursor);
  format_arg(instruction->arg1, 0, status);
  format_arg(instruction->arg2, 1, status);
  format_arg(instruction->arg3, 1, status);
}

/* It is easier to describe divsion strictly rather than through codegen and
 * simple instructions */
void division_operator_emit(struct CODE_GENERATION_STATUS *status) {
  asprintf(&tmp, "movq %rdx , %r14 # Saving heap pointer before dividing\n");
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "popq %rbx # Popping first value into dividend\n");
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "popq %rax # Popping second value into divider\n");
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "cqo # sign extension\n");
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "idivq %rbx # Carry out division\n");
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "pushq %rax # Push result to stack\n");
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "movq %r14 , %rdx # Restoring heap pointer after dividing\n");
  ins(status, tmp, func_cursor);
}

void mod_operator_emit(struct CODE_GENERATION_STATUS *status) {
  asprintf(&tmp, "movq %rdx , %r14 # Saving heap pointer before dividing\n");
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "popq %rbx # Popping first value into dividend\n");
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "popq %rax # Popping second value into divider\n");
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "cqo # sign extension\n");
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "idivq %rbx # Carry out division\n");
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "pushq %rdx # Push result to stack\n");
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "movq %r14 , %rdx # Restoring heap pointer after dividing\n");
  ins(status, tmp, func_cursor);
}

void ins(struct CODE_GENERATION_STATUS *status, char *str, int cursor) {
  code_sections[cursor] = realloc(
      code_sections[cursor], strlen(code_sections[cursor]) + strlen(str) + 1);
  strcat(code_sections[cursor], str);
}

void malloc_setup(struct CODE_GENERATION_STATUS *status) {

  asprintf(
      &tmp,
      "movq %rsi , %r14 # Saving potential cmd line arguments before malloc\n");
  ins(status, tmp, func_cursor);

  asprintf(&tmp,
           "movq $4000 , %rdi # Moving size of initial allocation into RDI\n");
  ins(status, tmp, func_cursor);

  asprintf(&tmp, "callq malloc # calling C-binary for allocating stackspace on "
                 "multiple platforms\n");
  ins(status, tmp, func_cursor);

  asprintf(&tmp, "movq %rax , %rdx # Moving heap pointer into RSL\n");
  ins(status, tmp, func_cursor);

  asprintf(&tmp, "movq %r14 , %rsi # Restoring potential cmd line arguments "
                 "after malloc\n");
  ins(status, tmp, func_cursor);
}

void emit_asm(struct CODE_GENERATION_STATUS *status,
              struct FLOAT_DATA **float_data_array) {
  hash_stack[0] = 0;

  char *temp = malloc(sizeof(char));
  code_sections[func_cursor] = malloc(sizeof(char *));

  strcpy(code_sections[func_cursor], "");

  asprintf(&tmp, ".data\n");
  int i = 0;
  while (float_data_array[i] != NULL) {
    asprintf(&tmp, "%s: .float %f\n", float_data_array[i]->label,
             float_data_array[i]->floatval);
    ins(status, tmp, func_cursor);
    i++;
  }
  i = 0;
  while (status->string_data_array[i] != NULL) {
    asprintf(&tmp, "%s: .string %s\n", status->string_data_array[i]->label,
             status->string_data_array[i]->stringval);
    ins(status, tmp, func_cursor);
    i++;
  }

  asprintf(&tmp, "iform: \n .string %s\n", "\"\%d\\n\"");
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "fform: \n .string %s\n", "\"\%f\\n\"");
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "cform: \n .string %s\n", "\"\%c\\n\"");
  ins(status, tmp, func_cursor);
  asprintf(&tmp, "sform: \n .string %s\n", "\"\%s\\n\"");
  ins(status, tmp, func_cursor);
  /* Program Prologue */
  asprintf(&tmp, ".text\n.globl main\nmain:\n");
  ins(status, tmp, func_cursor);

  /* temporary test bench for instructions */
  for (int i = 0; i < status->instructions_length; i++) {
    if (status->instruction_array[i] == NULL) {
      continue;
    }

    switch (status->instruction_array[i]->operator) {
    case O_CALLEE_PROLOGUE:
      asprintf(&tmp, "\n\n# O_CALLEE_PROLOGUE\n");
      ins(status, tmp, func_cursor);
      break;
    case O_CALLEE_SAVE:
      asprintf(&tmp, "\n\n# O_CALLEE_SAVE\n");
      ins(status, tmp, func_cursor);
      break;
    case O_CALLEE_RESTORE:
      asprintf(&tmp, "\n\n# O_CALLEE_RESTORE\n");
      ins(status, tmp, func_cursor);
      break;
    case O_CALLEE_EPILOGUE:
      asprintf(&tmp, "\n\n# O_CALLEE_EPILOGUE\n");
      ins(status, tmp, func_cursor);
      break;
    case O_CALLER_PROLOGUE:
      asprintf(&tmp, "\n\n# O_CALLER_PROLOGUE\n");
      ins(status, tmp, func_cursor);
      break;
    case O_CALLER_SAVE:
      asprintf(&tmp, "\n\n# O_CALLER_SAVE\n");
      ins(status, tmp, func_cursor);
      break;
    case O_CALLER_RESTORE:
      asprintf(&tmp, "\n\n# O_CALLER_RESTORE\n");
      ins(status, tmp, func_cursor);
      break;
    case O_CALLER_EPILOGUE:
      asprintf(&tmp, "\n\n# O_CALLER_EPILOGUE\n");
      ins(status, tmp, func_cursor);
      break;
    case O_PROGRAM_PROLOGUE:
      malloc_setup(status);
      asprintf(&tmp, "\n\n# O_CALLER_PROLOGUE\n");
      ins(status, tmp, func_cursor);
      break;
    case O_PROGRAM_EPILOGUE:
      asprintf(&tmp, "\n\n# O_PROGRAM_EPILOGUE\n");
      ins(status, tmp, func_cursor);
      break;
    case O_CALL_PRINTF:
      asprintf(&tmp, "\n\n# PRINTING\n");
      ins(status, tmp, func_cursor);
      print_instruction_function(status);
      break;
    case O_ALLOC_STACK_SPACE:
      asprintf(&tmp, "\n# ALLOCATING STACK SPACE\n");
      ins(status, tmp, func_cursor);
      allocate_stack_space(status);
      break;
    case O_FUNC_DEF:

      // Each function is stored in-memory as different char arrays, here we
      // assign a given function a specific index in the list of char arrays
      // based on the hash of the function ID
      func_cursor = crc32b_hash_emit(
          status->instruction_array[i + 1]->arg1->target->label);

      // Hash collision, find available space
      if (code_sections[func_cursor] != NULL) {
        for (int i = 0; i < MAX_NUM_FUNCS; i++) {
          if (code_sections[i] == NULL) {
            func_cursor = i;
            break;
          }

#if DEBUG
          fprintf(stderr,
                  "emit.c :: emi_asm :: Too many functions defined, function "
                  "'%s' could not be given a code "
                  "section to store itself in\n",
                  status->instruction_array[i + 1]->arg1->target->label);
          exit(-1);
#endif
        }
      }

      hash_stack[hash_stack_cursor] = func_cursor;
      hash_stack_cursor++;
      // Dummy setup
      code_sections[func_cursor] = (char *)malloc(sizeof(char));
      strcpy(code_sections[func_cursor], "");

      break;
    case O_FUNC_DEF_END:
      hash_stack_cursor--;

      func_cursor = hash_stack[hash_stack_cursor - 1];
      break;
    case O_FADD:
    case O_FSUB:
    case O_FMUL:
    case O_FDIV:
    case O_ADD:
    case O_SUB:
    case O_MUL:
    case O_CMPB:
    case O_JMP:
    case O_JNE:
    case O_JE:
    case O_JL:
    case O_JG:
    case O_JGE:
    case O_JLE:
    case O_PUSH:
    case O_MOVE:
    case O_MOVEZ:
    case O_CMP:
    case O_POP:
    case O_LEAQ:
    case O_EMPTY:
    case O_CALL:
      asprintf(&tmp, "\n");
      ins(status, tmp, func_cursor);
      simple_instruction(status->instruction_array[i], status);
      asprintf(&tmp, "# %s\n", status->instruction_array[i]->comment);
      ins(status, tmp, func_cursor);
      break;
    case O_LABEL:
      asprintf(&tmp, "%s:\n",
               status->instruction_array[i]->arg1->target->label);
      ins(status, tmp, func_cursor);
      break;
    case O_RET:
      asprintf(&tmp, "ret\n");
      ins(status, tmp, func_cursor);
      break;
    case O_DIV:
      division_operator_emit(status);
      break;
    case O_MOD:
      mod_operator_emit(status);
      break;
    default:
      break;
    }
  }

#if DEBUG
  printf("emit.c :: emit_asm :: Generating Program Prologue\n");
#endif

  /* Program Epilogue */
  asprintf(&tmp, "\nmovl $1, %%eax\nmovl $0, %%ebx\nint $0x80\nleave\nret\n");
  ins(status, tmp, func_cursor);

  FILE *fptr;
  fptr = fopen("output.s", "w");
  for (int i = 0; i < MAX_NUM_FUNCS; i++) {
    if (code_sections[i] != NULL) {
      fprintf(fptr, "%s", code_sections[i]);
    }
  }
}
