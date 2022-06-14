#pragma once

static inline char * ast_node_kinds_as_strings[] = {
    "A_TEST",     "A_ADD",      "A_ASSIGN",   "A_ID",       "A_INTVAL",    "A_PROGRAM",      "A_PARAM_LIST", "A_RETURN",
    "A_DECL",     "A_GLUE",     "A_DEF_FUNC", "A_BLOCK",    "A_CALL_FUNC", "A_EXPRESSION",   "A_EMPTY",      "A_WHILE",
    "A_IF",       "A_LESS",     "A_GREAT",    "A_LEQ",      "A_GEQ",       "A_NEG",          "A_EQUAL",      "A_NEQUAL",
    "A_SUB",      "A_PRINT",    "A_FLOATVAL", "A_CHARVAL",  "A_CHAR",      "A_FLOAT",        "A_STRINGVAL",  "A_STR",
    "A_CALLABLE", "A_PARAM",    "A_CAST",     "A_CASTABLE", "A_SIZEOF",    "A_ALLOCATE",       
    "A_INT",      "A_MOD",      "A_DIV",      "A_MUL",      "A_ADDRESSOF", "A_END_DEF_FUNC", "A_POINTER",   
    "A_BREAK",    "A_CONTINUE", "A_STRUCT",   "A_END_WHILE", "A_STRING", "A_DEREF", "A_INDEX", "A_DOT"};

static inline char * symbol_catagories_as_strings[] = {"S_VAR", "S_FUNC", "S_PARAM"};



static inline char * operators_as_strings[] = {"O_JE",
                                               "O_JG",
                                               "O_JL",
                                               "O_ADD",
                                               "O_CMP",
                                               "O_DIV",
                                               "O_JGE",
                                               "O_JLE",
                                               "O_JMP",
                                               "O_JNE",
                                               "O_MUL",
                                               "O_POP",
                                               "O_RET",
                                               "O_SUB",
                                               "O_CALL",
                                               "O_MOVE",
                                               "O_LABEL",
                                               "O_PUSH",
                                               "O_CALLER_SAVE",
                                               "O_CALL_PRINTF",
                                               "O_CALLER_RESTORE",
                                               "O_CALLER_EPILOGUE",
                                               "O_CALLER_PROLOGUE",
                                               "O_CALLEE_SAVE",
                                               "O_CALLEE_RESTORE",
                                               "O_CALLEE_EPILOGUE",
                                               "O_CALLEE_PROLOGUE",
                                               "O_MAIN_CALLEE_SAVE",
                                               "O_PROGRAM_EPILOGUE",
                                               "O_PROGRAM_PROLOGUE",
                                               "O_ALLOC_STACK_SPACE",
                                               "O_DEALLOC_STACK_SPACE",
                                               "O_MAIN_CALLEE_RESTORE",
                                               "O_REVERSE_PUSH_ARGUMENTS",
                                               "O_FUNC_DEF",
                                               "O_FUNC_DEF_END",
                                               "O_LEAQ",
                                               "O_FADD",
                                               "O_FSUB",
                                               "O_FMUL",
                                               "O_FDIV",
                                               "O_WHILE",
                                               "O_INT_TO_FLOAT",
                                               "O_FLOAT_TO_INT",
                                               "O_CMPB", 
                                               "O_MOVEZ",
                                               "O_MOD",
                                               "O_EMPTY"

};

static inline char * argument_targets_as_strings[] = {"IMS : Immediate String",
                                                      "IMF : Immediate Float",
                                                      "IMI : Immediate Int",
                                                      "IML : Immediate Label",
                                                      "MEM : Memory (label)",
                                                      "RBP : Register : base (frame) pointer",
                                                      "RSP : Register : stack pointer",
                                                      "RRT : Register : return value",
                                                      "RSL : Register : static link computation",
                                                      "REG : Register : general-purpose"};


static inline char * addressing_modes_as_strings[] = {"Direct", "Indirect", "Indirect Relative"};
