#ifndef MCC_ASSEMBLY_H
#define MCC_ASSEMBLY_H

#include "mcc/tac.h"

enum mcc_asm_constant_type {
	MCC_ASM_CONSTANT_TYPE_FLOAT,
	MCC_ASM_CONSTANT_TYPE_STRING,
};

struct mcc_asm_constant {
	char *value;
	enum mcc_asm_constant_type type;
	struct mcc_asm_constant *next_constant;
};

struct mcc_asm_variable {
	char *name;
	struct mcc_asm_variable *next_variable;

	int variable_offset;
};

struct mcc_asm_program_contents {
	struct mcc_asm_constant *first_constant;
	struct mcc_asm_constant *last_constant;
	struct mcc_asm_variable *first_variable;
	struct mcc_asm_variable *last_variable;

	int constant_counter;
	int parameter_counter;
	int variable_offset;
};

struct mcc_asm_program_contents *mcc_asm_code_generation(FILE *out, struct mcc_tac_function_blocks *tac);

void mcc_assembly_delete_asm(struct mcc_asm_program_contents *asm_code);

#endif
