#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc/assembly.h"
#include "mcc/tac.h"

#define VAR_SIZE 4

void mcc_delete_asm_variable(struct mcc_asm_variable *variable);

struct mcc_asm_variable *get_variable(char *variablename, struct mcc_asm_program_contents *code)
{

	assert(variablename);
	assert(code);

	struct mcc_asm_variable *tmp = code->first_variable;

	while (tmp != NULL) {
		if (strcmp(variablename, tmp->name) == 0) {
			return tmp;
		}
		tmp = tmp->next_variable;
	}

	return NULL;
}

struct mcc_asm_variable *
get_variable_or_create_if_not_exist(char *variablename, struct mcc_asm_program_contents *code, int size)
{

	assert(variablename);
	assert(code);

	struct mcc_asm_variable *tmp = get_variable(variablename, code);

	// if variable does not exist
	if (tmp == NULL) {

		// if it is a variable and not a literal
		if ((((strncmp(variablename, "v", 1) == 0) || (strncmp(variablename, "t", 1) == 0)) &&
		     (isdigit(variablename[1]))) ||
		    (strncmp(variablename, "content_", 8) == 0)) {

			struct mcc_asm_variable *variable = malloc(sizeof(*variable));
			variable->name = strdup(variablename);
			variable->next_variable = NULL;

			code->variable_offset -= size;

			variable->variable_offset = code->variable_offset;

			if (code->last_variable == NULL) {
				code->first_variable = variable;
			} else {
				code->last_variable->next_variable = variable;
			}

			code->last_variable = variable;

			return variable;

		} else {

			return NULL;
		}

	} else {
		return tmp;
	}
}

void create_constant(char *value, enum mcc_asm_constant_type type, struct mcc_asm_program_contents *code)
{

	assert(code);

	struct mcc_asm_constant *constant = malloc(sizeof(*constant));

	constant->type = type;
	constant->value = value;
	constant->next_constant = NULL;

	if (code->first_constant != NULL) {
		code->last_constant->next_constant = constant;
	} else {
		code->first_constant = constant;
	}

	code->last_constant = constant;
}

void delete_constant(struct mcc_asm_constant *constant)
{
	assert(constant);

	if (constant->next_constant != NULL) {
		delete_constant(constant->next_constant);
	}

	free(constant);
}

void generate_constant_assembly_code(FILE *out, struct mcc_asm_program_contents *code)
{
	assert(code);

	struct mcc_asm_constant *constant = code->first_constant;
	int counter = 0;
	while (constant != NULL) {
		fprintf(out, ".LC%d:\n", counter);
		if (constant->type == MCC_ASM_CONSTANT_TYPE_FLOAT) {
			fprintf(out, "\t.float %s\n", constant->value);
		} else if (constant->type == MCC_ASM_CONSTANT_TYPE_STRING) {
			fprintf(out, "\t.string \"%s\"\n", constant->value);
		}
		counter++;
		constant = constant->next_constant;
	}

	delete_constant(code->first_constant);
	code->first_constant = NULL;
	code->last_constant = NULL;
}

void mcc_add_parameter_variable(char *var_name, struct mcc_asm_program_contents *asm_code)
{

	assert(var_name);
	assert(asm_code);

	struct mcc_asm_variable *asm_variable = malloc(sizeof(*asm_variable));

	asm_variable->name = strdup(var_name);
	asm_variable->variable_offset = 8 + (asm_code->parameter_counter) * 4;
	asm_variable->next_variable = NULL;

	if (asm_code->last_variable == NULL)
		asm_code->first_variable = asm_variable;
	else
		asm_code->last_variable->next_variable = asm_variable;

	asm_code->last_variable = asm_variable;
	asm_code->parameter_counter += 1;
}

void mcc_asm_generate_parameter_declaration(FILE *out,
                                            struct mcc_tac_instruction *instruction,
                                            struct mcc_asm_program_contents *asm_code)
{

	assert(out);
	assert(instruction);
	assert(asm_code);

	assert(instruction->argument1);

	// create variable (we assume that it does not exist)
	// get_variable_or_create_if_not_exist(instruction->argument1, asm_code,VAR_SIZE);
	mcc_add_parameter_variable(instruction->argument1, asm_code);
}

void mcc_asm_generate_copy_int(FILE *out,
                               struct mcc_tac_instruction *instruction,
                               struct mcc_asm_program_contents *code)
{
	assert(code);
	assert(instruction);

	struct mcc_asm_variable *argument1 =
	    get_variable_or_create_if_not_exist(instruction->argument1, code, VAR_SIZE);
	struct mcc_asm_variable *result = get_variable_or_create_if_not_exist(instruction->result, code, VAR_SIZE);

	if (argument1 != NULL) {
		fprintf(out, "\tmovl\t%d(%%ebp), %%eax\n", argument1->variable_offset);
		fprintf(out, "\tmovl\t%%eax, %d(%%ebp)\n", result->variable_offset);
	} else {
		fprintf(out, "\tmovl\t$%s, %d(%%ebp)\n", instruction->argument1, result->variable_offset);
	}
}
void mcc_asm_generate_copy_float(FILE *out,
                                 struct mcc_tac_instruction *instruction,
                                 struct mcc_asm_program_contents *code)
{
	assert(code);
	assert(instruction);

	struct mcc_asm_variable *argument1 =
	    get_variable_or_create_if_not_exist(instruction->argument1, code, VAR_SIZE);
	struct mcc_asm_variable *result = get_variable_or_create_if_not_exist(instruction->result, code, VAR_SIZE);

	if (argument1 != NULL) {
		fprintf(out, "\tflds\t%d(%%ebp)\n", argument1->variable_offset);

	} else {
		create_constant(instruction->argument1, MCC_ASM_CONSTANT_TYPE_FLOAT, code);
		fprintf(out, "\tflds\t.LC%d\n", code->constant_counter);
		code->constant_counter++;
	}

	fprintf(out, "\tfstps\t%d(%%ebp)\n", result->variable_offset);
}

void mcc_asm_generate_copy_string(FILE *out,
                                  struct mcc_tac_instruction *instruction,
                                  struct mcc_asm_program_contents *code)
{
	assert(code);
	assert(instruction);

	struct mcc_asm_variable *argument1 =
	    get_variable_or_create_if_not_exist(instruction->argument1, code, VAR_SIZE);
	struct mcc_asm_variable *result = get_variable_or_create_if_not_exist(instruction->result, code, VAR_SIZE);

	if (argument1 != NULL) {
		fprintf(out, "\tmovl\t%d(%%ebp), %%eax\n", argument1->variable_offset);
		fprintf(out, "\tmovl\t%%eax, %d(%%ebp)\n", result->variable_offset);
	} else {
		create_constant(instruction->argument1, MCC_ASM_CONSTANT_TYPE_STRING, code);
		fprintf(out, "\tmovl\t$.LC%d, %d(%%ebp)\n", code->constant_counter, result->variable_offset);
		code->constant_counter++;
	}
}

void mcc_asm_generate_unary_not(FILE *out,
                                struct mcc_tac_instruction *instruction,
                                struct mcc_asm_program_contents *code)
{
	assert(code);
	assert(instruction);

	struct mcc_asm_variable *argument1 =
	    get_variable_or_create_if_not_exist(instruction->argument1, code, VAR_SIZE);
	struct mcc_asm_variable *result = get_variable_or_create_if_not_exist(instruction->result, code, VAR_SIZE);

	fprintf(out, "\tcmpl\t$0, %d(%%ebp)\n", argument1->variable_offset);
	fprintf(out, "\tsete\t%%al\n");
	fprintf(out, "\tmovzbl\t%%al, %%eax\n");
	fprintf(out, "\tmovl\t%%eax, %d(%%ebp)\n", result->variable_offset);
}

void mcc_asm_generate_unary_minus_int(FILE *out,
                                      struct mcc_tac_instruction *instruction,
                                      struct mcc_asm_program_contents *code)
{

	assert(code);
	assert(instruction);

	struct mcc_asm_variable *argument1 =
	    get_variable_or_create_if_not_exist(instruction->argument1, code, VAR_SIZE);
	struct mcc_asm_variable *result = get_variable_or_create_if_not_exist(instruction->result, code, VAR_SIZE);

	if (argument1 != NULL) {
		fprintf(out, "\tmovl\t%d(%%ebp), %%eax\n", argument1->variable_offset);
		fprintf(out, "\tnegl\t%%eax\n");
		fprintf(out, "\tmovl\t%%eax, %d(%%ebp)\n", result->variable_offset);
	} else {
		fprintf(out, "\tmovl\t$-%s, %d(%%ebp)\n", instruction->argument1, result->variable_offset);
	}
}

void mcc_asm_generate_unary_minus_float(FILE *out,
                                        struct mcc_tac_instruction *instruction,
                                        struct mcc_asm_program_contents *code)
{

	assert(out);
	assert(instruction);
	assert(code);

	assert(instruction->result);
	assert(instruction->argument1);

	struct mcc_asm_variable *argument1 =
	    get_variable_or_create_if_not_exist(instruction->argument1, code, VAR_SIZE);
	struct mcc_asm_variable *result = get_variable_or_create_if_not_exist(instruction->result, code, VAR_SIZE);

	fprintf(out, "\tflds\t%d(%%ebp)\n", argument1->variable_offset);
	fprintf(out, "\tfchs\n");
	fprintf(out, "\tfstps\t%d(%%ebp)\n", result->variable_offset);
}

void mcc_asm_generate_arithmetic_op_int(FILE *out,
                                        struct mcc_tac_instruction *instruction,
                                        struct mcc_asm_program_contents *asm_code)
{

	assert(out);
	assert(instruction);
	assert(asm_code);

	assert(instruction->argument1);
	assert(instruction->argument2);
	assert(instruction->result);

	struct mcc_asm_variable *argument1 =
	    get_variable_or_create_if_not_exist(instruction->argument1, asm_code, VAR_SIZE);
	struct mcc_asm_variable *argument2 =
	    get_variable_or_create_if_not_exist(instruction->argument2, asm_code, VAR_SIZE);
	struct mcc_asm_variable *result = get_variable_or_create_if_not_exist(instruction->result, asm_code, VAR_SIZE);

	// if argument 1 is not a literal
	if (argument1 != NULL) {
		fprintf(out, "\tmovl\t%d(%%ebp), %%eax\n", argument1->variable_offset);
	} else {
		fprintf(out, "\tmovl\t$%s, %%eax\n", instruction->argument1);
	}

	switch (instruction->statement_type) {
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_PLUS_INT:

		// if argument 2 is not a literal
		if (argument2 != NULL) {
			fprintf(out, "\taddl\t%d(%%ebp), %%eax\n", argument2->variable_offset);
		} else {
			fprintf(out, "\taddl\t$%s, %%eax\n", instruction->argument2);
		}

		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_MINUS_INT:

		// if argument 2 is not a literal
		if (argument2 != NULL) {
			fprintf(out, "\tsubl\t%d(%%ebp), %%eax\n", argument2->variable_offset);
		} else {
			fprintf(out, "\tsubl\t$%s, %%eax\n", instruction->argument2);
		}

		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_MULTIPLY_INT:

		// if argument 2 is not a literal
		if (argument2 != NULL) {
			fprintf(out, "\timull\t%d(%%ebp), %%eax\n", argument2->variable_offset);
		} else {
			fprintf(out, "\timull\t$%s, %%eax\n", instruction->argument2);
		}

		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_DIVISION_INT:
		fprintf(out, "\tcltd\n");
		// if argument 2 is not a literal
		if (argument2 != NULL) {
			fprintf(out, "\tidivl\t%d(%%ebp)\n", argument2->variable_offset);
		} else {
			fprintf(out, "\tmovl\t$%s, %%ecx\n", instruction->argument2);
			fprintf(out, "\tidivl\t%%ecx\n");
		}

		break;

	default:
		break;
	}

	fprintf(out, "\tmovl\t%%eax, %d(%%ebp)\n", result->variable_offset);
}

void mcc_asm_generate_arithmetic_op_float(FILE *out,
                                          struct mcc_tac_instruction *instruction,
                                          struct mcc_asm_program_contents *asm_code)
{

	assert(out);
	assert(instruction);
	assert(asm_code);

	assert(instruction->argument1);
	assert(instruction->argument2);
	assert(instruction->result);

	struct mcc_asm_variable *argument1 =
	    get_variable_or_create_if_not_exist(instruction->argument1, asm_code, VAR_SIZE);
	struct mcc_asm_variable *argument2 =
	    get_variable_or_create_if_not_exist(instruction->argument2, asm_code, VAR_SIZE);
	struct mcc_asm_variable *result = get_variable_or_create_if_not_exist(instruction->result, asm_code, VAR_SIZE);

	fprintf(out, "\tflds\t%d(%%ebp)\n", argument1->variable_offset);

	switch (instruction->statement_type) {

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_PLUS_FLOAT:
		fprintf(out, "\tfadds\t%d(%%ebp)\n", argument2->variable_offset);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_MINUS_FLOAT:
		fprintf(out, "\tfsubs\t%d(%%ebp)\n", argument2->variable_offset);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_MULTIPLY_FLOAT:
		fprintf(out, "\tfmuls\t%d(%%ebp)\n", argument2->variable_offset);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_DIVISION_FLOAT:
		fprintf(out, "\tfdivs\t%d(%%ebp)\n", argument2->variable_offset);
		break;

	default:
		break;
	}

	fprintf(out, "\tfstps\t%d(%%ebp)\n", result->variable_offset);
}

void mcc_asm_generate_comparison_op_int(FILE *out,
                                        struct mcc_tac_instruction *instruction,
                                        struct mcc_asm_program_contents *asm_code)
{

	assert(out);
	assert(instruction);
	assert(asm_code);

	assert(instruction->argument1);
	assert(instruction->argument2);
	assert(instruction->result);

	struct mcc_asm_variable *argument1 =
	    get_variable_or_create_if_not_exist(instruction->argument1, asm_code, VAR_SIZE);
	struct mcc_asm_variable *argument2 =
	    get_variable_or_create_if_not_exist(instruction->argument2, asm_code, VAR_SIZE);
	struct mcc_asm_variable *result = get_variable_or_create_if_not_exist(instruction->result, asm_code, VAR_SIZE);

	// if argument 1 is not a literal
	if (argument1 != NULL) {
		fprintf(out, "\tmovl\t%d(%%ebp), %%eax\n", argument1->variable_offset);
	} else {
		fprintf(out, "\tmovl\t$%s, %%eax\n", instruction->argument1);
	}

	if (argument2 != NULL) {
		fprintf(out, "\tcmpl\t%d(%%ebp), %%eax\n", argument2->variable_offset);
	} else {
		fprintf(out, "\tcmpl\t$%s, %%eax\n", instruction->argument2);
	}

	switch (instruction->statement_type) {
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_LT_INT:
		fprintf(out, "\tsetl\t%%al\n");
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_GT_INT:
		fprintf(out, "\tsetg\t%%al\n");
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_LEQ_INT:
		fprintf(out, "\tsetle\t%%al\n");
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_GEQ_INT:
		fprintf(out, "\tsetge\t%%al\n");
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_EQ_INT:
		fprintf(out, "\tsete\t%%al\n");
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_NEQ_INT:
		fprintf(out, "\tsetne\t%%al\n");
		break;

	default:
		break;
	}

	fprintf(out, "\tmovzbl\t%%al, %%eax\n");
	fprintf(out, "\tmovl\t%%eax, %d(%%ebp)\n", result->variable_offset);
}

void mcc_asm_generate_comparison_op_float(FILE *out,
                                          struct mcc_tac_instruction *instruction,
                                          struct mcc_asm_program_contents *asm_code)
{

	assert(out);
	assert(instruction);
	assert(asm_code);

	assert(instruction->argument1);
	assert(instruction->argument2);
	assert(instruction->result);

	struct mcc_asm_variable *argument1 =
	    get_variable_or_create_if_not_exist(instruction->argument1, asm_code, VAR_SIZE);
	struct mcc_asm_variable *argument2 =
	    get_variable_or_create_if_not_exist(instruction->argument2, asm_code, VAR_SIZE);
	struct mcc_asm_variable *result = get_variable_or_create_if_not_exist(instruction->result, asm_code, VAR_SIZE);

	fprintf(out, "\tflds\t%d(%%ebp)\n", argument2->variable_offset);
	fprintf(out, "\tflds\t%d(%%ebp)\n", argument1->variable_offset);
	fprintf(out, "\tfcomip\t%%st(1), %%st\n");
	fprintf(out, "\tfstp\t%%st(0)\n");

	switch (instruction->statement_type) {

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_LT_FLOAT:
		fprintf(out, "\tsetb\t%%al\n");
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_GT_FLOAT:
		fprintf(out, "\tseta\t%%al\n");
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_LEQ_FLOAT:
		fprintf(out, "\tsetbe\t%%al\n");
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_GEQ_FLOAT:
		fprintf(out, "\tsetae\t%%al\n");
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_EQ_FLOAT:
		fprintf(out, "\tsete\t%%al\n");
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_NEQ_FLOAT:
		fprintf(out, "\tsetne\t%%al\n");
		break;

	default:
		break;
	}

	fprintf(out, "\tmovzbl\t%%al, %%eax\n");
	fprintf(out, "\tmovl\t%%eax, %d(%%ebp)\n", result->variable_offset);
}

void mcc_asm_generate_logic_op(FILE *out,
                               struct mcc_tac_instruction *instruction,
                               struct mcc_asm_program_contents *asm_code)
{
	assert(out);
	assert(instruction);
	assert(asm_code);

	assert(instruction->argument1);
	assert(instruction->argument2);
	assert(instruction->result);

	struct mcc_asm_variable *argument1 =
	    get_variable_or_create_if_not_exist(instruction->argument1, asm_code, VAR_SIZE);
	struct mcc_asm_variable *argument2 =
	    get_variable_or_create_if_not_exist(instruction->argument2, asm_code, VAR_SIZE);
	struct mcc_asm_variable *result = get_variable_or_create_if_not_exist(instruction->result, asm_code, VAR_SIZE);

	fprintf(out, "\tmovl\t%d(%%ebp), %%eax\n", argument1->variable_offset);

	if (instruction->statement_type == MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_AND) {
		fprintf(out, "\tand\t%d(%%ebp), %%eax\n", argument2->variable_offset);
	} else {
		fprintf(out, "\tor\t%d(%%ebp), %%eax\n", argument2->variable_offset);
	}

	fprintf(out, "\tmovl\t%%eax, %d(%%ebp)\n", result->variable_offset);
}

void mcc_asm_generate_return(FILE *out,
                             struct mcc_tac_instruction *instruction,
                             struct mcc_asm_program_contents *code,
                             enum mcc_tac_statement_type type)
{

	assert(out);
	assert(instruction);
	assert(code);
	assert(type);

	if (type == MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_VOID) {
		fprintf(out, "\tleave\n");
		fprintf(out, "\tret\n");
	} else {

		struct mcc_asm_variable *argument1 =
		    get_variable_or_create_if_not_exist(instruction->argument1, code, VAR_SIZE);

		if (type == MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_FLOAT) {

			fprintf(out, "\tflds\t%d(%%ebp)\n", argument1->variable_offset);

			mcc_asm_generate_return(out, instruction, code, MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_VOID);
		} else if (type == MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_INT ||
		           type == MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_STRING) {

			if (argument1) {
				fprintf(out, "\tmovl\t%d(%%ebp), %%eax\n", argument1->variable_offset);
			} else {
				fprintf(out, "\tmovl\t$%s, %%eax\n", instruction->argument1);
			}

			mcc_asm_generate_return(out, instruction, code, MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_VOID);
		}
	}
}

void mcc_asm_generate_if(FILE *out, struct mcc_tac_instruction *instruction, struct mcc_asm_program_contents *asm_code)
{

	assert(out);
	assert(instruction);
	assert(asm_code);

	assert(instruction->argument1);
	assert(instruction->argument2);

	struct mcc_asm_variable *argument1 =
	    get_variable_or_create_if_not_exist(instruction->argument1, asm_code, VAR_SIZE);

	if (argument1 != NULL) {
		fprintf(out, "\tcmpl\t$0, %d(%%ebp)\n", argument1->variable_offset);
	} else {
		fprintf(out, "\tmovl\t$%s, %%eax\n", instruction->argument1);
		fprintf(out, "\tcmpl\t$0, %%eax\n");
	}

	fprintf(out, "\tje\t.%s\n", instruction->argument2);
}

void mcc_asm_generate_jump(FILE *out, struct mcc_tac_instruction *instruction)
{

	assert(out);
	assert(instruction);

	assert(instruction->argument1);

	fprintf(out, "\tjmp\t.%s\n", instruction->argument1);
}

void mcc_asm_generate_array_declaration(FILE *out,
                                        struct mcc_tac_instruction *instruction,
                                        struct mcc_asm_program_contents *asm_code)
{

	assert(out);
	assert(instruction);
	assert(asm_code);

	assert(instruction->argument1);
	assert(instruction->argument2);

	char temp[15] = "";
	strncat(temp, "content_", 9);
	strncat(temp, instruction->argument1, 9);

	struct mcc_asm_variable *array_address_variable =
	    get_variable_or_create_if_not_exist(instruction->argument1, asm_code, VAR_SIZE);
	struct mcc_asm_variable *array_data_variable =
	    get_variable_or_create_if_not_exist(temp, asm_code, VAR_SIZE * atoi(instruction->argument2));

	fprintf(out, "\tleal\t%d(%%ebp), %%eax\n", array_data_variable->variable_offset);
	fprintf(out, "\tmovl\t%%eax, %d(%%ebp)\n", array_address_variable->variable_offset);
}

void mcc_asm_generate_store_array_value(FILE *out,
                                        struct mcc_tac_instruction *instruction,
                                        struct mcc_asm_program_contents *asm_code)
{

	assert(out);
	assert(instruction);
	assert(asm_code);

	assert(instruction->argument1);
	assert(instruction->argument2);

	struct mcc_asm_variable *argument1 =
	    get_variable_or_create_if_not_exist(instruction->argument1, asm_code, VAR_SIZE);
	struct mcc_asm_variable *argument2 =
	    get_variable_or_create_if_not_exist(instruction->argument2, asm_code, VAR_SIZE);

	fprintf(out, "\tmovl\t%d(%%ebp), %%eax\n", argument1->variable_offset);

	if (argument2 != NULL) {

		fprintf(out, "\tmovl\t%d(%%ebp), %%ebx\n", argument2->variable_offset);
		fprintf(out, "\tmovl\t%%ebx, (%%eax)\n");

	} else {

		fprintf(out, "\tmovl\t$%s, (%%eax)\n", instruction->argument2);
	}
}

void mcc_asm_generate_load_array_value(FILE *out,
                                       struct mcc_tac_instruction *instruction,
                                       struct mcc_asm_program_contents *asm_code)
{

	assert(out);
	assert(instruction);
	assert(asm_code);

	assert(instruction->argument1);
	assert(instruction->result);

	struct mcc_asm_variable *argument1 =
	    get_variable_or_create_if_not_exist(instruction->argument1, asm_code, VAR_SIZE);
	struct mcc_asm_variable *result = get_variable_or_create_if_not_exist(instruction->result, asm_code, VAR_SIZE);

	fprintf(out, "\tmovl\t%d(%%ebp), %%eax\n", argument1->variable_offset);
	fprintf(out, "\tmovl\t(%%eax), %%eax\n");
	fprintf(out, "\tmovl\t%%eax, %d(%%ebp)\n", result->variable_offset);
}

void mcc_asm_generate_array_size(FILE *out,
                                 struct mcc_tac_instruction *instruction,
                                 struct mcc_asm_program_contents *asm_code)
{

	assert(out);
	assert(instruction);
	assert(asm_code);

	assert(instruction->argument1);
	assert(instruction->result);

	struct mcc_asm_variable *result = get_variable_or_create_if_not_exist(instruction->result, asm_code, VAR_SIZE);
	assert(result);

	if (strncmp(instruction->argument1, "INT", 3) == 0) {
		fprintf(out, "\tmovl\t$%d, %d(%%ebp)\n", 4, result->variable_offset);
	} else if (strncmp(instruction->argument1, "BOOL", 4) == 0) {
		fprintf(out, "\tmovl\t$%d, %d(%%ebp)\n", 4, result->variable_offset);
	} else if (strncmp(instruction->argument1, "FLOAT", 4) == 0) {
		fprintf(out, "\tmovl\t$%d, %d(%%ebp)\n", 4, result->variable_offset);
	} else if (strncmp(instruction->argument1, "STRING", 5) == 0) {
		fprintf(out, "\tmovl\t$%d, %d(%%ebp)\n", 4, result->variable_offset);
	}
}

void mcc_asm_generate_array_mul_add(FILE *out,
                                    struct mcc_tac_instruction *instruction,
                                    struct mcc_asm_program_contents *asm_code)
{

	assert(out);
	assert(instruction);
	assert(asm_code);

	assert(instruction->argument1);
	assert(instruction->argument2);
	assert(instruction->result);

	struct mcc_asm_variable *argument1 =
	    get_variable_or_create_if_not_exist(instruction->argument1, asm_code, VAR_SIZE);
	struct mcc_asm_variable *argument2 =
	    get_variable_or_create_if_not_exist(instruction->argument2, asm_code, VAR_SIZE);
	struct mcc_asm_variable *result = get_variable_or_create_if_not_exist(instruction->result, asm_code, VAR_SIZE);

	// if argument 1 is not a literal
	if (argument1 != NULL) {
		fprintf(out, "\tmovl\t%d(%%ebp), %%eax\n", argument1->variable_offset);
	} else {
		fprintf(out, "\tmovl\t$%s, %%eax\n", instruction->argument1);
	}

	switch (instruction->statement_type) {
	case MCC_TAC_STATEMENT_TYPE_OPERATION_ADD_INT:

		// if argument 2 is not a literal
		if (argument2 != NULL) {
			fprintf(out, "\taddl\t%d(%%ebp), %%eax\n", argument2->variable_offset);
		} else {
			fprintf(out, "\taddl\t$%s, %%eax\n", instruction->argument2);
		}

		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_MUL_INT:

		// if argument 2 is not a literal
		if (argument2 != NULL) {
			fprintf(out, "\timull\t%d(%%ebp), %%eax\n", argument2->variable_offset);
		} else {
			fprintf(out, "\timull\t$%s, %%eax\n", instruction->argument2);
		}

		break;

	default:
		break;
	}

	fprintf(out, "\tmovl\t%%eax, %d(%%ebp)\n", result->variable_offset);
}

void mcc_asm_generate_push(FILE *out,
                           struct mcc_tac_instruction *instruction,
                           struct mcc_asm_program_contents *asm_code)
{

	assert(out);
	assert(instruction);
	assert(asm_code);

	assert(instruction->argument1);

	struct mcc_asm_variable *argument1 =
	    get_variable_or_create_if_not_exist(instruction->argument1, asm_code, VAR_SIZE);

	// if argument1 in not a literal
	if (argument1 != NULL) {
		fprintf(out, "\tpushl\t%d(%%ebp)\n", argument1->variable_offset);
	} else {
		fprintf(out, "\tpushl\t$%s\n", instruction->argument1);
	}
}

void mcc_asm_generate_pop_int(FILE *out,
                              struct mcc_tac_instruction *instruction,
                              struct mcc_asm_program_contents *asm_code)
{

	assert(instruction->result);

	struct mcc_asm_variable *result = get_variable_or_create_if_not_exist(instruction->result, asm_code, VAR_SIZE);

	fprintf(out, "\tmovl\t%%eax, %d(%%ebp)\n", result->variable_offset);
}

void mcc_asm_generate_pop_float(FILE *out,
                                struct mcc_tac_instruction *instruction,
                                struct mcc_asm_program_contents *asm_code)
{

	assert(instruction->result);

	struct mcc_asm_variable *result = get_variable_or_create_if_not_exist(instruction->result, asm_code, VAR_SIZE);

	fprintf(out, "\tfstps\t%d(%%ebp)\n", result->variable_offset);
}

void mcc_asm_generate_function_call(FILE *out,
                                    struct mcc_tac_instruction *instruction,
                                    struct mcc_asm_program_contents *asm_code)
{

	assert(out);
	assert(instruction);
	assert(asm_code);

	assert(instruction->argument1);
	assert(instruction->argument2);

	fprintf(out, "\tcall\t%s\n", instruction->argument1);
	fprintf(out, "\taddl\t$%d, %%esp\n", atoi(instruction->argument2) * VAR_SIZE);
}

void mcc_generate_asm_instruction(FILE *out,
                                  struct mcc_tac_instruction *instruction,
                                  struct mcc_asm_program_contents *asm_code)
{

	switch (instruction->statement_type) {

	case MCC_TAC_STATEMENT_TYPE_OPERATION_COPY_INT:
		mcc_asm_generate_copy_int(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_COPY_FLOAT:
		mcc_asm_generate_copy_float(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_COPY_STRING:
		mcc_asm_generate_copy_string(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_UNARY_NOT:
		mcc_asm_generate_unary_not(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_UNARY_MINUS_INT:
		mcc_asm_generate_unary_minus_int(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_UNARY_MINUS_FLOAT:
		mcc_asm_generate_unary_minus_float(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_PLUS_INT:
		mcc_asm_generate_arithmetic_op_int(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_PLUS_FLOAT:
		mcc_asm_generate_arithmetic_op_float(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_MINUS_INT:
		mcc_asm_generate_arithmetic_op_int(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_MINUS_FLOAT:
		mcc_asm_generate_arithmetic_op_float(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_MULTIPLY_INT:
		mcc_asm_generate_arithmetic_op_int(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_MULTIPLY_FLOAT:
		mcc_asm_generate_arithmetic_op_float(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_DIVISION_INT:
		mcc_asm_generate_arithmetic_op_int(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_DIVISION_FLOAT:
		mcc_asm_generate_arithmetic_op_float(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_LT_INT:
		mcc_asm_generate_comparison_op_int(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_LT_FLOAT:
		mcc_asm_generate_comparison_op_float(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_GT_INT:
		mcc_asm_generate_comparison_op_int(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_GT_FLOAT:
		mcc_asm_generate_comparison_op_float(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_LEQ_INT:
		mcc_asm_generate_comparison_op_int(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_LEQ_FLOAT:
		mcc_asm_generate_comparison_op_float(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_GEQ_INT:
		mcc_asm_generate_comparison_op_int(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_GEQ_FLOAT:
		mcc_asm_generate_comparison_op_float(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_EQ_INT:
		mcc_asm_generate_comparison_op_int(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_EQ_FLOAT:
		mcc_asm_generate_comparison_op_float(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_NEQ_INT:
		mcc_asm_generate_comparison_op_int(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_NEQ_FLOAT:
		mcc_asm_generate_comparison_op_float(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_AND:
		mcc_asm_generate_logic_op(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_OR:
		mcc_asm_generate_logic_op(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_POP_INT:
		mcc_asm_generate_pop_int(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_POP_FLOAT:
		mcc_asm_generate_pop_float(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_POP_STRING:
		mcc_asm_generate_pop_int(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_DECLARATION_ARR_INT:
		mcc_asm_generate_array_declaration(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_DECLARATION_ARR_FLOAT:
		mcc_asm_generate_array_declaration(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_DECLARATION_ARR_STRING:
		mcc_asm_generate_array_declaration(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_ARR_SIZE:
		mcc_asm_generate_array_size(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_LOAD_ARRAY_VALUE:
		mcc_asm_generate_load_array_value(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_STORE_ARRAY_VALUE:
		mcc_asm_generate_store_array_value(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_MUL_INT:
		mcc_asm_generate_array_mul_add(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_ADD_INT:
		mcc_asm_generate_array_mul_add(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_VOID:
		mcc_asm_generate_return(out, instruction, asm_code, MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_VOID);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_INT:
		mcc_asm_generate_return(out, instruction, asm_code, MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_INT);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_FLOAT:
		mcc_asm_generate_return(out, instruction, asm_code, MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_FLOAT);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_STRING:
		mcc_asm_generate_return(out, instruction, asm_code, MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_STRING);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_INT:
		mcc_asm_generate_push(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_FLOAT:
		mcc_asm_generate_push(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_STRING:
		mcc_asm_generate_push(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_ARRAY_INT:
		mcc_asm_generate_push(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_ARRAY_FLOAT:
		mcc_asm_generate_push(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_ARRAY_STRING:
		mcc_asm_generate_push(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_PROCEDURE_CALL:
		mcc_asm_generate_function_call(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_DECLARATION:
		mcc_asm_generate_parameter_declaration(out, instruction, asm_code);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_JUMP:
		mcc_asm_generate_jump(out, instruction);
		break;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_IF:
		mcc_asm_generate_if(out, instruction, asm_code);
		break;

	default:
		// TODO
		break;
	}
}

int get_size_for_instruction(struct mcc_tac_instruction *instruction)
{
	assert(instruction);

	switch (instruction->statement_type) {
	case MCC_TAC_STATEMENT_TYPE_OPERATION_DECLARATION_ARR_INT:
		return VAR_SIZE * atoi(instruction->argument2);

	case MCC_TAC_STATEMENT_TYPE_OPERATION_DECLARATION_ARR_FLOAT:
		return VAR_SIZE * atoi(instruction->argument2);

	case MCC_TAC_STATEMENT_TYPE_OPERATION_DECLARATION_ARR_STRING:
		return VAR_SIZE * atoi(instruction->argument2);

	case MCC_TAC_STATEMENT_TYPE_OPERATION_JUMP:
		return 0;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_IF:
		return 0;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_DECLARATION:
		return 0;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_INT:
		return 0;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_FLOAT:
		return 0;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_STRING:
		return 0;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_ARRAY_INT:
		return 0;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_ARRAY_FLOAT:
		return 0;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_ARRAY_STRING:
		return 0;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_VOID:
		return 0;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_INT:
		return 0;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_FLOAT:
		return 0;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_STRING:
		return 0;

	case MCC_TAC_STATEMENT_TYPE_OPERATION_STORE_ARRAY_VALUE:
		return 0;

	default:
		return VAR_SIZE;
	}
}
int calculate_stack_size(struct mcc_tac_basic_block *block, int stack_size)
{
	assert(block);
	if (block->first_instruction) {
		struct mcc_tac_instruction *instruction = block->first_instruction;
		while (instruction != NULL) {
			stack_size += get_size_for_instruction(instruction);
			instruction = instruction->next_instruction;
		}
	}
	struct mcc_tac_basic_block *child = block->child;
	while (child != NULL) {
		stack_size = calculate_stack_size(child, stack_size);
		child = child->next_child;
	}
	if (block->next_block != NULL) {
		stack_size = calculate_stack_size(block->next_block, stack_size);
	}

	return stack_size;
}

void mcc_generate_asm_block(FILE *out, struct mcc_tac_basic_block *block, struct mcc_asm_program_contents *asm_code)
{

	assert(block);
	assert(asm_code);

	// print label for block
	if (block->label != NULL) {
		fprintf(out, ".%s:\n", block->label);
	}

	struct mcc_tac_instruction *instruction = block->first_instruction;

	while (instruction != NULL) {
		mcc_generate_asm_instruction(out, instruction, asm_code);
		instruction = instruction->next_instruction;
	}

	struct mcc_tac_basic_block *child_block = block->child;

	while (child_block != NULL) {
		mcc_generate_asm_block(out, child_block, asm_code);
		child_block = child_block->next_child;
	}

	if (block->next_block != NULL) {
		mcc_generate_asm_block(out, block->next_block, asm_code);
	}
}

struct mcc_asm_program_contents *mcc_asm_code_generation(FILE *out, struct mcc_tac_function_blocks *tac)
{
	assert(tac);

	struct mcc_asm_program_contents *asm_code = malloc(sizeof(*asm_code));
	asm_code->first_constant = NULL;
	asm_code->first_variable = NULL;
	asm_code->last_constant = NULL;
	asm_code->last_variable = NULL;
	asm_code->variable_offset = 0;
	asm_code->constant_counter = 0;
	asm_code->parameter_counter = 0;

	while (tac != NULL) {
		// function headers
		fprintf(out, "\t.globl\t%s\n", tac->function_name);
		fprintf(out, "\t.type\t%s, @function\n", tac->function_name);
		fprintf(out, "%s:\n", tac->function_name);
		fprintf(out, "\tpushl\t%%ebp\n");
		fprintf(out, "\tmovl\t%%esp, %%ebp\n");
		// TODO calculate stack size
		fprintf(out, "\tsubl\t$%d, %%esp\n", calculate_stack_size(tac->first_basic_block, VAR_SIZE));

		mcc_generate_asm_block(out, tac->first_basic_block, asm_code);
		tac = tac->next_function;

		asm_code->variable_offset = 0;
		asm_code->parameter_counter = 0;
		if (asm_code->first_variable) {
			mcc_delete_asm_variable(asm_code->first_variable);
			asm_code->first_variable = NULL;
			asm_code->last_variable = NULL;
		}
	}

	if (asm_code->first_constant) {
		generate_constant_assembly_code(out, asm_code);
	}

	return asm_code;
}

void mcc_delete_asm_variable(struct mcc_asm_variable *variable)
{
	assert(variable);

	if (variable->next_variable != NULL) {
		mcc_delete_asm_variable(variable->next_variable);
	}

	free(variable->name);
	free(variable);
}

void mcc_assembly_delete_asm(struct mcc_asm_program_contents *asm_code)
{
	assert(asm_code);

	if (asm_code->first_variable != NULL) {
		mcc_delete_asm_variable(asm_code->first_variable);
	}
	if (asm_code->first_constant != NULL) {
		delete_constant(asm_code->first_constant);
	}

	free(asm_code);
}
