#ifndef MCC_TAC_H
#define MCC_TAC_H

#include <stdbool.h>

#include "mcc/ast.h"
#include "mcc/symbol_table.h"

enum mcc_tac_statement_type {
	// =====================================================  x = y
	MCC_TAC_STATEMENT_TYPE_OPERATION_COPY_INT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_COPY_STRING,
	MCC_TAC_STATEMENT_TYPE_OPERATION_COPY_FLOAT,
	// =====================================================
	MCC_TAC_STATEMENT_TYPE_OPERATION_JUMP,
	MCC_TAC_STATEMENT_TYPE_OPERATION_IF,

	MCC_TAC_STATEMENT_TYPE_UNARY_NOT,
	MCC_TAC_STATEMENT_TYPE_UNARY_MINUS_INT,
	MCC_TAC_STATEMENT_TYPE_UNARY_MINUS_FLOAT,

	MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_PLUS_INT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_PLUS_FLOAT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_MINUS_INT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_MINUS_FLOAT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_MULTIPLY_INT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_MULTIPLY_FLOAT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_DIVISION_INT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_DIVISION_FLOAT,

	MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_LT_INT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_LT_FLOAT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_GT_INT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_GT_FLOAT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_LEQ_INT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_LEQ_FLOAT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_GEQ_INT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_GEQ_FLOAT,

	MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_EQ_INT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_EQ_FLOAT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_NEQ_INT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_NEQ_FLOAT,

	MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_AND,
	MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_OR,

	MCC_TAC_STATEMENT_TYPE_OPERATION_POP_INT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_POP_FLOAT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_POP_STRING,

	MCC_TAC_STATEMENT_TYPE_OPERATION_DECLARATION_ARR_INT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_DECLARATION_ARR_FLOAT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_DECLARATION_ARR_STRING,

	MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_VOID,
	MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_INT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_FLOAT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_STRING,

	MCC_TAC_STATEMENT_TYPE_OPERATION_ARR_SIZE,
	MCC_TAC_STATEMENT_TYPE_OPERATION_STORE_ARRAY_VALUE,
	MCC_TAC_STATEMENT_TYPE_OPERATION_LOAD_ARRAY_VALUE,

	MCC_TAC_STATEMENT_TYPE_OPERATION_MUL_INT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_ADD_INT,

	MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_INT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_FLOAT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_STRING,
	MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_ARRAY_INT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_ARRAY_FLOAT,
	MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_ARRAY_STRING,

	MCC_TAC_STATEMENT_TYPE_OPERATION_PROCEDURE_CALL,

	MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_DECLARATION

};

struct mcc_tac_instruction {
	enum mcc_tac_statement_type statement_type;
	char *argument1;
	char *argument2;
	char *result;
	char *label;
	int counter;

	struct mcc_tac_instruction *next_instruction;
	struct mcc_tac_instruction *previous_instruction;
};

struct mcc_tac_variable_stack_element {
	char *name;
	enum mcc_ast_type type;
	bool is_array;

	struct mcc_tac_variable_stack_element *next_element;
	struct mcc_tac_variable_stack_element *prev_element;
};

struct mcc_tac_basic_block {

	struct mcc_tac_instruction *first_instruction;
	struct mcc_tac_instruction *last_instruction;

	struct mcc_tac_basic_block *next_block;
	struct mcc_tac_basic_block *parent;
	struct mcc_tac_basic_block *child;
	struct mcc_tac_basic_block *next_child;

	struct mcc_tac_variable_stack_element *first_element;
	struct mcc_tac_variable_stack_element *last_element;

	struct mcc_tac_basic_block *jump_block;

	char *label;
	int last_statement_number;
};

struct mcc_tac_function_blocks {

	char *function_name;

	struct mcc_tac_basic_block *first_basic_block;
	struct mcc_tac_basic_block *last_basic_block;

	struct mcc_tac_function_blocks *next_function;
};

struct userdata {
	struct mcc_symbol_table *symbol_table;

	struct mcc_tac_function_blocks *current_function;
	struct mcc_tac_function_blocks *first_function;

	int variable_counter;
	int statement_counter;
	int label_counter;
};

struct mcc_tac_function_blocks *mcc_ast_to_tac(struct mcc_ast_function_definition *program);
void mcc_tac_delete_tac(struct mcc_tac_function_blocks *function);
void mcc_tac_delete_block(struct mcc_tac_basic_block *block);
void mcc_tac_delete_tac_instruction(struct mcc_tac_instruction *tac);
void mcc_tac_delete_stack_element(struct mcc_tac_variable_stack_element *stack_element);

#endif // MCC_TAC_H
