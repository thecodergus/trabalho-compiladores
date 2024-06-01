#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc/ast_visit.h"
#include "mcc/tac.h"

#define BUFFER_SIZE 64

static enum mcc_tac_statement_type mcc_binary_op_type_to_three_address_type(enum mcc_ast_binary_op operation,
                                                                            bool is_float)
{

	switch (operation) {
	case MCC_AST_BINARY_OP_ADD:
		if (is_float) {
			return MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_PLUS_FLOAT;
		}
		return MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_PLUS_INT;

	case MCC_AST_BINARY_OP_SUB:
		if (is_float) {
			return MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_MINUS_FLOAT;
		}
		return MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_MINUS_INT;

	case MCC_AST_BINARY_OP_MUL:
		if (is_float) {
			return MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_MULTIPLY_FLOAT;
		}
		return MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_MULTIPLY_INT;

	case MCC_AST_BINARY_OP_DIV:
		if (is_float) {
			return MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_DIVISION_FLOAT;
		}
		return MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_DIVISION_INT;

	case MCC_AST_BINARY_OP_LESS:
		if (is_float) {
			return MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_LT_FLOAT;
		}
		return MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_LT_INT;

	case MCC_AST_BINARY_OP_LESS_EQ:
		if (is_float) {
			return MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_LEQ_FLOAT;
		}
		return MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_LEQ_INT;

	case MCC_AST_BINARY_OP_GREATER:
		if (is_float) {
			return MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_GT_FLOAT;
		}
		return MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_GT_INT;

	case MCC_AST_BINARY_OP_GREATER_EQ:
		if (is_float) {
			return MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_GEQ_FLOAT;
		}
		return MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_GEQ_INT;

	case MCC_AST_BINARY_OP_EQ:
		if (is_float) {
			return MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_EQ_FLOAT;
		}
		return MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_EQ_INT;

	case MCC_AST_BINARY_OP_NOT_EQ:
		if (is_float) {
			return MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_NEQ_FLOAT;
		}
		return MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_NEQ_INT;

	case MCC_AST_BINARY_OP_AND:
		return MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_AND;

	case MCC_AST_BINARY_OP_OR:
		return MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_OR;

	default:
		return -1;
	}
}

static void *get_label(void *data)
{
	assert(data);

	struct userdata *userdata = data;

	int counter = userdata->label_counter;
	userdata->label_counter = userdata->label_counter + 1;

	char *label_name = malloc(BUFFER_SIZE);
	snprintf(label_name, BUFFER_SIZE, "T%d", counter);

	return label_name;
}

static void mcc_basic_block_add_instruction(struct mcc_tac_basic_block *basic_block,
                                            char *argument1,
                                            char *argument2,
                                            char *result,
                                            enum mcc_tac_statement_type type,
                                            int statement_counter,
                                            bool preorder)
{
	assert(basic_block);

	struct mcc_tac_instruction *new_instruction = malloc(sizeof(*new_instruction));

	new_instruction->argument1 = argument1;
	new_instruction->argument2 = argument2;
	new_instruction->result = result;
	new_instruction->statement_type = type;
	new_instruction->counter = statement_counter;
	new_instruction->next_instruction = NULL;
	new_instruction->previous_instruction = NULL;
	new_instruction->label = NULL;

	basic_block->last_statement_number = statement_counter;

	if (basic_block->first_instruction == NULL) {
		basic_block->first_instruction = new_instruction;
		basic_block->last_instruction = new_instruction;
	} else if (preorder) {
		new_instruction->next_instruction = basic_block->first_instruction;
		basic_block->first_instruction->previous_instruction = new_instruction;
		basic_block->first_instruction = new_instruction;
	} else {
		basic_block->last_instruction->next_instruction = new_instruction;
		new_instruction->previous_instruction = basic_block->last_instruction;
		basic_block->last_instruction = new_instruction;
	}
}

static struct mcc_tac_basic_block *mcc_basic_block_add_new_block(void *data, bool is_child)
{
	assert(data);
	struct userdata *userdata = data;

	struct mcc_tac_basic_block *new_block = malloc(sizeof(*new_block));

	new_block->child = NULL;
	new_block->first_element = NULL;
	new_block->first_instruction = NULL;
	new_block->label = NULL;
	new_block->last_element = NULL;
	new_block->last_instruction = NULL;
	new_block->last_statement_number = 0;
	new_block->next_block = NULL;
	new_block->next_child = NULL;
	new_block->parent = NULL;
	new_block->jump_block = NULL;

	if (!is_child) {
		new_block->parent = userdata->current_function->last_basic_block->parent;
		userdata->current_function->last_basic_block->next_block = new_block;
		userdata->current_function->last_basic_block = new_block;

		return new_block;
	}

	struct mcc_tac_basic_block *old_block = userdata->current_function->last_basic_block;
	if (old_block->child == NULL) {
		old_block->child = new_block;
	} else {
		struct mcc_tac_basic_block *tmp = old_block->child;
		while (tmp->next_child != NULL) {
			tmp = tmp->next_child;
		}
		tmp->next_child = new_block;
	}

	userdata->current_function->last_basic_block = new_block;
	new_block->parent = old_block;

	return new_block;
}

void mcc_variable_stack_push(struct mcc_tac_variable_stack_element *element, void *data)
{
	assert(element);
	assert(data);

	struct userdata *userdata = data;
	struct mcc_tac_variable_stack_element *current_variable = element;
	struct mcc_tac_variable_stack_element *last_variable =
	    userdata->current_function->last_basic_block->last_element;

	if (last_variable != NULL) {
		last_variable->next_element = element;
		current_variable->prev_element = last_variable;
	} else {
		userdata->current_function->last_basic_block->first_element = element;
	}

	userdata->current_function->last_basic_block->last_element = element;
}

struct mcc_tac_variable_stack_element *mcc_variable_stack_pop(void *data)
{
	assert(data);
	struct userdata *userdata = data;
	struct mcc_tac_variable_stack_element *last_variable =
	    userdata->current_function->last_basic_block->last_element;

	if (last_variable == NULL) {
		return NULL;
	}

	if (last_variable->prev_element != NULL) {
		userdata->current_function->last_basic_block->last_element = last_variable->prev_element;
		userdata->current_function->last_basic_block->last_element->next_element = NULL;
	} else {
		userdata->current_function->last_basic_block->first_element = NULL;
		userdata->current_function->last_basic_block->last_element = NULL;
	}

	return last_variable;
}

static char *mcc_convert_type_to_str(enum mcc_ast_type type)
{

	char *type_string = malloc(sizeof(type_string));
	if (!type_string) {
		assert(type_string);
	}

	switch (type) {
	case MCC_AST_TYPE_INT:
		strcpy(type_string, "INT");
		break;

	case MCC_AST_TYPE_BOOL:
		strcpy(type_string, "INT");
		break;

	case MCC_AST_TYPE_FLOAT:
		strcpy(type_string, "FLOAT");
		break;

	case MCC_AST_TYPE_STRING:
		strcpy(type_string, "STRING");
		break;

	default:
		break;
	}

	return type_string;
}

static char *mcc_get_identifier_variable_name(char *identifier,
                                              struct mcc_symbol_table *symbol_table,
                                              void *data,
                                              struct mcc_ast_node node)
{
	assert(identifier);
	assert(symbol_table);
	assert(data);

	struct userdata *userdata = data;

	struct mcc_symbol_table_entry *entry = mcc_symbol_table_lookup_variable_entry(symbol_table, identifier, node);
	assert(entry);

	if (entry->tac_name == NULL) {

		int count = userdata->variable_counter;
		userdata->variable_counter = userdata->variable_counter + 1;

		char *variable_name = malloc(BUFFER_SIZE);

		snprintf(variable_name, BUFFER_SIZE, "v%d", count);
		entry->tac_name = variable_name;
	}

	// copy name to make freeing easier
	char *new_name = malloc(strlen(entry->tac_name) + 1);
	strcpy(new_name, entry->tac_name);
	return new_name;
}

static char *mcc_get_temporary_variable_name(void *data)
{
	assert(data);
	struct userdata *userdata = data;

	int count = userdata->variable_counter;
	userdata->variable_counter = userdata->variable_counter + 1;

	char *var_name = malloc(BUFFER_SIZE);

	snprintf(var_name, BUFFER_SIZE, "t%d", count);

	return var_name;
}

static void mcc_tac_add_literal_int(struct mcc_ast_literal *literal, void *data)
{
	assert(literal);
	assert(data);

	struct userdata *userdata = data;
	struct mcc_tac_variable_stack_element *variable = malloc(sizeof(*variable));

	char *var_val = malloc(BUFFER_SIZE);
	snprintf(var_val, BUFFER_SIZE, "%ld", literal->i_value);

	variable->is_array = false;
	variable->next_element = NULL;
	variable->prev_element = NULL;
	variable->type = MCC_AST_TYPE_INT;
	variable->name = var_val;

	mcc_variable_stack_push(variable, userdata);
}

static void mcc_tac_add_literal_float(struct mcc_ast_literal *literal, void *data)
{
	assert(literal);
	assert(data);

	struct userdata *userdata = data;
	struct mcc_tac_variable_stack_element *variable = malloc(sizeof(*variable));

	variable->is_array = false;
	variable->next_element = NULL;
	variable->prev_element = NULL;

	int counter = userdata->variable_counter;
	userdata->variable_counter = userdata->variable_counter + 1;

	char *var_name = malloc(BUFFER_SIZE);
	snprintf(var_name, BUFFER_SIZE, "t%d", counter);

	char *var_val = malloc(BUFFER_SIZE);
	snprintf(var_val, BUFFER_SIZE, "%lf", literal->f_value);

	variable->type = MCC_AST_TYPE_FLOAT;
	char *tmp_var_name = malloc(strlen(var_name) + 1);
	strcpy(tmp_var_name, var_name);
	variable->name = tmp_var_name;

	mcc_variable_stack_push(variable, userdata);

	struct mcc_tac_basic_block *last_block = userdata->current_function->last_basic_block;
	mcc_basic_block_add_instruction(last_block, var_val, NULL, var_name,
	                                MCC_TAC_STATEMENT_TYPE_OPERATION_COPY_FLOAT, userdata->statement_counter,
	                                false);
}

static void mcc_tac_add_literal_bool(struct mcc_ast_literal *literal, void *data)
{
	assert(literal);
	assert(data);

	struct userdata *userdata = data;
	struct mcc_tac_variable_stack_element *variable = malloc(sizeof(*variable));

	variable->is_array = false;
	variable->next_element = NULL;
	variable->prev_element = NULL;
	char *var_val = malloc(BUFFER_SIZE);

	if (literal->b_value) {
		if (strcmp(literal->b_value, "true") == 0) {
			snprintf(var_val, BUFFER_SIZE, "1");
			variable->name = var_val;
			variable->type = MCC_AST_TYPE_INT; // bool is internally handled as int
		} else if (strcmp(literal->b_value, "false") == 0) {
			snprintf(var_val, BUFFER_SIZE, "0");
			variable->name = var_val;
			variable->type = MCC_AST_TYPE_INT; // bool is internally handled as int
		}
	}

	mcc_variable_stack_push(variable, userdata);
}

static void mcc_tac_add_literal_string(struct mcc_ast_literal *literal, void *data)
{
	assert(literal);
	assert(data);

	struct userdata *userdata = data;
	struct mcc_tac_variable_stack_element *variable = malloc(sizeof(*variable));
	variable->next_element = NULL;
	variable->prev_element = NULL;
	variable->is_array = false;

	struct mcc_tac_basic_block *last_block = userdata->current_function->last_basic_block;

	int counter = userdata->variable_counter;
	userdata->variable_counter = userdata->variable_counter + 1;

	char *var_name = malloc(BUFFER_SIZE);
	snprintf(var_name, BUFFER_SIZE, "t%d", counter);

	char *tmp_var_name = malloc(strlen(var_name) + 1);
	strcpy(tmp_var_name, var_name);
	variable->name = tmp_var_name;
	variable->type = MCC_AST_TYPE_STRING;

	mcc_variable_stack_push(variable, userdata);

	char *str_val_copy = malloc(strlen(literal->s_value) + 1);
	strcpy(str_val_copy, literal->s_value);

	mcc_basic_block_add_instruction(last_block, str_val_copy, NULL, var_name,
	                                MCC_TAC_STATEMENT_TYPE_OPERATION_COPY_STRING, userdata->statement_counter,
	                                false);
}

static void mcc_tac_add_declaration(struct mcc_ast_declaration *declaration, void *data)
{

	assert(declaration);
	assert(data);

	struct userdata *userdata = data;
	struct mcc_tac_basic_block *last_block = userdata->current_function->last_basic_block;

	char *var_name = mcc_get_identifier_variable_name(declaration->identifier, declaration->symbol_table, data,
	                                                  declaration->node);

	char *var_val = malloc(BUFFER_SIZE);

	// if array declaration
	if (declaration->literal) {
		snprintf(var_val, BUFFER_SIZE, "%ld", declaration->literal->i_value);
	} else {
		free(var_val);
		free(var_name);
		return;
	}
	enum mcc_tac_statement_type type;
	switch (declaration->type) {
	case MCC_AST_TYPE_INT:
		type = MCC_TAC_STATEMENT_TYPE_OPERATION_DECLARATION_ARR_INT;
		break;
	case MCC_AST_TYPE_BOOL:
		type = MCC_TAC_STATEMENT_TYPE_OPERATION_DECLARATION_ARR_INT;
		break;
	case MCC_AST_TYPE_FLOAT:
		type = MCC_TAC_STATEMENT_TYPE_OPERATION_DECLARATION_ARR_FLOAT;
		break;
	case MCC_AST_TYPE_STRING:
		type = MCC_TAC_STATEMENT_TYPE_OPERATION_DECLARATION_ARR_STRING;
		break;
	default:
		assert(NULL);
		break;
	}

	mcc_basic_block_add_instruction(last_block, var_name, var_val, NULL, type, userdata->statement_counter, true);
}

static void mcc_tac_add_assignment(struct mcc_ast_assignment *assignment, void *data)
{
	assert(assignment);
	assert(data);

	struct userdata *userdata = data;
	struct mcc_tac_basic_block *last_block = userdata->current_function->last_basic_block;

	struct mcc_symbol_table_entry *symbol_entry =
	    mcc_symbol_table_lookup_variable_entry(assignment->symbol_table, assignment->identifier, assignment->node);
	if (symbol_entry == NULL) {
		return;
	}
	char *var_name =
	    mcc_get_identifier_variable_name(assignment->identifier, assignment->symbol_table, data, assignment->node);

	// if array
	if (assignment->index != NULL) {
		char *type = mcc_convert_type_to_str(symbol_entry->data_type);
		struct mcc_tac_variable_stack_element *assignment_value = mcc_variable_stack_pop(data);
		struct mcc_tac_variable_stack_element *index_value = mcc_variable_stack_pop(data);

		char *index_address = mcc_get_temporary_variable_name(data);
		char *size = mcc_get_temporary_variable_name(data);
		char *mul_size = mcc_get_temporary_variable_name(data);

		mcc_basic_block_add_instruction(last_block, type, NULL, size, MCC_TAC_STATEMENT_TYPE_OPERATION_ARR_SIZE,
		                                userdata->statement_counter, false);
		char *tmp_size = malloc(strlen(size) + 1);
		strcpy(tmp_size, size);
		mcc_basic_block_add_instruction(last_block, tmp_size, index_value->name, mul_size,
		                                MCC_TAC_STATEMENT_TYPE_OPERATION_MUL_INT, userdata->statement_counter,
		                                false);
		char *tmp_mul_size = malloc(strlen(mul_size) + 1);
		strcpy(tmp_mul_size, mul_size);
		mcc_basic_block_add_instruction(last_block, var_name, tmp_mul_size, index_address,
		                                MCC_TAC_STATEMENT_TYPE_OPERATION_ADD_INT, userdata->statement_counter,
		                                false);
		char *tmp_address = malloc(strlen(index_address) + 1);
		strcpy(tmp_address, index_address);
		mcc_basic_block_add_instruction(last_block, tmp_address, assignment_value->name, NULL,
		                                MCC_TAC_STATEMENT_TYPE_OPERATION_STORE_ARRAY_VALUE,
		                                userdata->statement_counter, false);
		free(assignment_value);
		free(index_value);
	} else {
		struct mcc_tac_variable_stack_element *assignment_value = mcc_variable_stack_pop(data);
		enum mcc_tac_statement_type type;
		switch (assignment_value->type) {
		case MCC_AST_TYPE_INT:
			type = MCC_TAC_STATEMENT_TYPE_OPERATION_COPY_INT;
			break;
		case MCC_AST_TYPE_BOOL:
			type = MCC_TAC_STATEMENT_TYPE_OPERATION_COPY_INT;
			break;
		case MCC_AST_TYPE_FLOAT:
			type = MCC_TAC_STATEMENT_TYPE_OPERATION_COPY_FLOAT;
			break;
		case MCC_AST_TYPE_STRING:
			type = MCC_TAC_STATEMENT_TYPE_OPERATION_COPY_STRING;
			break;
		default:
			assert(NULL);
		}

		mcc_basic_block_add_instruction(last_block, assignment_value->name, NULL, var_name, type,
		                                userdata->statement_counter, false);

		free(assignment_value);
	}
}

static void mcc_tac_add_expression_identifier(struct mcc_ast_expression *expression, void *data)
{
	assert(expression);
	assert(data);

	struct userdata *userdata = data;

	struct mcc_tac_basic_block *current_block = userdata->current_function->last_basic_block;
	assert(current_block);

	char *identifier_name =
	    mcc_get_identifier_variable_name(expression->identifier, expression->symbol_table, data, expression->node);

	struct mcc_symbol_table_entry *entry =
	    mcc_symbol_table_lookup_variable_entry(expression->symbol_table, expression->identifier, expression->node);
	assert(entry);

	struct mcc_tac_variable_stack_element *new_element = malloc(sizeof(*new_element));

	new_element->name = identifier_name;
	new_element->type = entry->data_type;
	new_element->next_element = NULL;
	new_element->is_array = false;
	new_element->prev_element = NULL;

	if (expression->index != NULL) {

		char *str_type = mcc_convert_type_to_str(entry->data_type);

		char *index_address = mcc_get_temporary_variable_name(data);
		char *size = mcc_get_temporary_variable_name(data);
		char *mul_size = mcc_get_temporary_variable_name(data);
		char *result = mcc_get_temporary_variable_name(data);

		struct mcc_tac_variable_stack_element *index = mcc_variable_stack_pop(data);
		assert(index);

		mcc_basic_block_add_instruction(current_block, str_type, NULL, size,
		                                MCC_TAC_STATEMENT_TYPE_OPERATION_ARR_SIZE, userdata->statement_counter,
		                                false);

		char *size_copy = malloc(strlen(size) + 1);
		strcpy(size_copy, size);

		// Multiply size with index
		mcc_basic_block_add_instruction(current_block, size_copy, index->name, mul_size,
		                                MCC_TAC_STATEMENT_TYPE_OPERATION_MUL_INT, userdata->statement_counter,
		                                false);

		char *mul_size_copy = malloc(strlen(mul_size) + 1);
		strcpy(mul_size_copy, mul_size);

		mcc_basic_block_add_instruction(current_block, identifier_name, mul_size_copy, index_address,
		                                MCC_TAC_STATEMENT_TYPE_OPERATION_ADD_INT, userdata->statement_counter,
		                                false);

		char *index_address_copy = malloc(strlen(index_address) + 1);
		strcpy(index_address_copy, index_address);

		// Get value at calculated address
		mcc_basic_block_add_instruction(current_block, index_address_copy, NULL, result,
		                                MCC_TAC_STATEMENT_TYPE_OPERATION_LOAD_ARRAY_VALUE,
		                                userdata->statement_counter, false);

		char *result_copy = malloc(strlen(result) + 1);
		strcpy(result_copy, result);

		new_element->name = result_copy;

		free(index);

	} else if (entry->array_index != NULL) {

		new_element->is_array = true;
	}

	// Push result on stack
	mcc_variable_stack_push(new_element, userdata);
}

static void mcc_tac_add_binary_op(struct mcc_ast_expression *expression, void *data)
{
	assert(expression);
	assert(data);

	struct userdata *userdata = data;

	char *result_name = mcc_get_temporary_variable_name(data);

	// pop lhs and rhs from stack
	struct mcc_tac_variable_stack_element *element_lhs = mcc_variable_stack_pop(data);
	struct mcc_tac_variable_stack_element *element_rhs = mcc_variable_stack_pop(data);
	assert(element_lhs);
	assert(element_rhs);

	enum mcc_tac_statement_type operation_type = -1;

	if (element_lhs->type == MCC_AST_TYPE_FLOAT) {
		operation_type = mcc_binary_op_type_to_three_address_type(expression->op, true);
	} else {
		operation_type = mcc_binary_op_type_to_three_address_type(expression->op, false);
	}

	struct mcc_tac_basic_block *current_block = userdata->current_function->last_basic_block;

	mcc_basic_block_add_instruction(current_block, element_rhs->name, element_lhs->name, result_name,
	                                operation_type, userdata->statement_counter, false);

	// put result on stack
	struct mcc_tac_variable_stack_element *new_element = malloc(sizeof(*new_element));

	new_element->next_element = NULL;
	new_element->prev_element = NULL;
	new_element->is_array = false;

	char *result_name_copy = malloc(strlen(result_name) + 1);
	strcpy(result_name_copy, result_name);

	new_element->name = result_name_copy;
	new_element->type = element_lhs->type;

	free(element_lhs);
	free(element_rhs);

	mcc_variable_stack_push(new_element, userdata);
}

static void mcc_tac_add_unary_op(struct mcc_ast_expression *expression, void *data)
{
	assert(expression);
	assert(data);

	struct userdata *userdata = data;

	// pop inner exp from stack
	struct mcc_tac_variable_stack_element *stack_element = mcc_variable_stack_pop(data);
	assert(stack_element);

	char *result_name = mcc_get_temporary_variable_name(data);
	struct mcc_tac_basic_block *current_block = userdata->current_function->last_basic_block;

	switch (expression->unary_op) {
	case MCC_AST_UNARY_OP_MINUS:

		if (stack_element->type == MCC_AST_TYPE_FLOAT) {
			mcc_basic_block_add_instruction(current_block, stack_element->name, NULL, result_name,
			                                MCC_TAC_STATEMENT_TYPE_UNARY_MINUS_FLOAT,
			                                userdata->statement_counter, false);
		} else {
			mcc_basic_block_add_instruction(current_block, stack_element->name, NULL, result_name,
			                                MCC_TAC_STATEMENT_TYPE_UNARY_MINUS_INT,
			                                userdata->statement_counter, false);
		}
		break;

	case MCC_AST_UNARY_OP_NOT:
		mcc_basic_block_add_instruction(current_block, stack_element->name, NULL, result_name,
		                                MCC_TAC_STATEMENT_TYPE_UNARY_NOT, userdata->statement_counter, false);
		break;

	default:
		free(stack_element->name);
		break;
	}

	// push result on stack
	struct mcc_tac_variable_stack_element *new_element = malloc(sizeof(*new_element));

	new_element->is_array = false;
	new_element->prev_element = NULL;
	new_element->next_element = NULL;

	char *result_name_copy = malloc(strlen(result_name) + 1);
	strcpy(result_name_copy, result_name);

	new_element->name = result_name_copy;
	new_element->type = stack_element->type;

	mcc_variable_stack_push(new_element, userdata);
	free(stack_element);
}

static void mcc_tac_add_expression_func_call(struct mcc_ast_expression *expression, void *data)
{
	assert(expression);
	assert(data);

	struct userdata *userdata = data;
	struct mcc_tac_basic_block *current_block = userdata->current_function->last_basic_block;
	struct mcc_ast_expression *argument = expression->function_call->arguments;

	int argument_count = 0;

	// loop over arguments
	while (argument != NULL) {
		struct mcc_tac_variable_stack_element *stack_argument = mcc_variable_stack_pop(data);
		assert(stack_argument);

		if (stack_argument->is_array) {

			// add instruction for argument array
			switch (stack_argument->type) {

			case MCC_AST_TYPE_BOOL:
				mcc_basic_block_add_instruction(
				    current_block, stack_argument->name, NULL, NULL,
				    MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_ARRAY_INT,
				    userdata->statement_counter, false);
				break;

			case MCC_AST_TYPE_INT:
				mcc_basic_block_add_instruction(
				    current_block, stack_argument->name, NULL, NULL,
				    MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_ARRAY_INT,
				    userdata->statement_counter, false);
				break;

			case MCC_AST_TYPE_FLOAT:
				mcc_basic_block_add_instruction(
				    current_block, stack_argument->name, NULL, NULL,
				    MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_ARRAY_FLOAT,
				    userdata->statement_counter, false);
				break;

			case MCC_AST_TYPE_STRING:
				mcc_basic_block_add_instruction(
				    current_block, stack_argument->name, NULL, NULL,
				    MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_ARRAY_STRING,
				    userdata->statement_counter, false);
				break;

			default:
				free(stack_argument->name);
				break;
			}

		} else {
			// add instruction for argument
			switch (stack_argument->type) {
			case MCC_AST_TYPE_BOOL:
				mcc_basic_block_add_instruction(current_block, stack_argument->name, NULL, NULL,
				                                MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_INT,
				                                userdata->statement_counter, false);
				break;

			case MCC_AST_TYPE_INT:
				mcc_basic_block_add_instruction(current_block, stack_argument->name, NULL, NULL,
				                                MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_INT,
				                                userdata->statement_counter, false);
				break;

			case MCC_AST_TYPE_FLOAT:
				mcc_basic_block_add_instruction(current_block, stack_argument->name, NULL, NULL,
				                                MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_FLOAT,
				                                userdata->statement_counter, false);
				break;

			case MCC_AST_TYPE_STRING:
				mcc_basic_block_add_instruction(current_block, stack_argument->name, NULL, NULL,
				                                MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_STRING,
				                                userdata->statement_counter, false);
				break;

			default:
				free(stack_argument->name);
				break;
			}
		}

		free(stack_argument);
		argument = argument->next_expression;
		argument_count = argument_count + 1;
	}

	// get symbol table entry for function
	struct mcc_symbol_table_entry *entry =
	    mcc_symbol_table_lookup_function_entry(expression->symbol_table, expression->function_call->identifier);
	assert(entry);

	char *argument_count_string = malloc(BUFFER_SIZE);
	snprintf(argument_count_string, BUFFER_SIZE, "%d", argument_count);

	enum mcc_ast_type return_type = entry->data_type;

	char *identifier_copy = malloc(strlen(expression->function_call->identifier) + 1);
	strcpy(identifier_copy, expression->function_call->identifier);

	// add instruction for call
	mcc_basic_block_add_instruction(current_block, identifier_copy, argument_count_string, NULL,
	                                MCC_TAC_STATEMENT_TYPE_OPERATION_PROCEDURE_CALL, userdata->statement_counter,
	                                false);

	if (return_type != MCC_AST_TYPE_VOID) {

		char *result = mcc_get_temporary_variable_name(data);

		switch (return_type) {
		case MCC_AST_TYPE_BOOL:
			mcc_basic_block_add_instruction(current_block, NULL, NULL, result,
			                                MCC_TAC_STATEMENT_TYPE_OPERATION_POP_INT,
			                                userdata->statement_counter, false);
			break;

		case MCC_AST_TYPE_INT:
			mcc_basic_block_add_instruction(current_block, NULL, NULL, result,
			                                MCC_TAC_STATEMENT_TYPE_OPERATION_POP_INT,
			                                userdata->statement_counter, false);
			break;

		case MCC_AST_TYPE_FLOAT:
			mcc_basic_block_add_instruction(current_block, NULL, NULL, result,
			                                MCC_TAC_STATEMENT_TYPE_OPERATION_POP_FLOAT,
			                                userdata->statement_counter, false);
			break;

		case MCC_AST_TYPE_STRING:
			mcc_basic_block_add_instruction(current_block, NULL, NULL, result,
			                                MCC_TAC_STATEMENT_TYPE_OPERATION_POP_STRING,
			                                userdata->statement_counter, false);
			break;

		default:
			break;
		}

		// push result on stack
		struct mcc_tac_variable_stack_element *new_element = malloc(sizeof(*new_element));

		char *result_copy = malloc(strlen(result) + 1);
		strcpy(result_copy, result);
		new_element->next_element = NULL;
		new_element->prev_element = NULL;
		new_element->name = result_copy;
		new_element->type = entry->data_type;
		new_element->is_array = false;

		// Push variable name for next statement
		mcc_variable_stack_push(new_element, userdata);
	}
}

static void mcc_tac_add_statement_return(struct mcc_ast_statement *statement, void *data)
{
	assert(statement);
	assert(data);

	struct userdata *userdata = data;
	struct mcc_tac_basic_block *last_block = userdata->current_function->last_basic_block;

	// if void function
	if (statement->return_expression == NULL) {
		mcc_basic_block_add_instruction(last_block, NULL, NULL, NULL,
		                                MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_VOID,
		                                userdata->statement_counter, false);
		return;
	}

	struct mcc_tac_variable_stack_element *return_expression = mcc_variable_stack_pop(data);

	if ((return_expression->type == MCC_AST_TYPE_INT) || (return_expression->type == MCC_AST_TYPE_BOOL)) {
		mcc_basic_block_add_instruction(last_block, return_expression->name, NULL, NULL,
		                                MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_INT,
		                                userdata->statement_counter, false);
	} else if (return_expression->type == MCC_AST_TYPE_FLOAT) {
		mcc_basic_block_add_instruction(last_block, return_expression->name, NULL, NULL,
		                                MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_FLOAT,
		                                userdata->statement_counter, false);
	} else if (return_expression->type == MCC_AST_TYPE_STRING) {
		mcc_basic_block_add_instruction(last_block, return_expression->name, NULL, NULL,
		                                MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_STRING,
		                                userdata->statement_counter, false);
	} else {
		free(return_expression->name);
	}
	free(return_expression);
}

static void mcc_tac_add_statement_if(struct mcc_ast_statement *statement, void *data)
{

	assert(statement);
	assert(data);

	struct userdata *userdata = data;
	struct mcc_tac_variable_stack_element *if_condition = mcc_variable_stack_pop(data);
	struct mcc_tac_basic_block *last_block = userdata->current_function->last_basic_block;

	if (statement->else_statement != NULL) {
		char *else_label = get_label(data);
		char *label_end = get_label(data);

		// if condition
		mcc_basic_block_add_instruction(last_block, if_condition->name, else_label, NULL,
		                                MCC_TAC_STATEMENT_TYPE_OPERATION_IF, userdata->statement_counter,
		                                false);
		// go to end of if
		struct mcc_tac_basic_block *if_block = last_block->child;
		while (if_block->next_block != NULL) {
			if_block = if_block->next_block;
		}
		mcc_basic_block_add_instruction(if_block, label_end, NULL, NULL, MCC_TAC_STATEMENT_TYPE_OPERATION_JUMP,
		                                userdata->statement_counter, false);

		struct mcc_tac_basic_block *else_block = last_block->child->next_child;
		else_block->label = else_label;

		last_block = mcc_basic_block_add_new_block(data, false);
		last_block->label = label_end;
		if_block->jump_block = last_block;
	} else {
		char *label_end = get_label(data);

		mcc_basic_block_add_instruction(last_block, if_condition->name, label_end, NULL,
		                                MCC_TAC_STATEMENT_TYPE_OPERATION_IF, userdata->statement_counter,
		                                false);

		last_block = mcc_basic_block_add_new_block(data, false);
		last_block->label = label_end;
	}
	free(if_condition);
}

static void split_block(void *data, int statement_count)
{
	assert(data);

	struct userdata *userdata = data;
	struct mcc_tac_basic_block *current_block = userdata->current_function->last_basic_block;
	struct mcc_tac_instruction *last_instruction = current_block->last_instruction;
	assert(current_block);

	if ((current_block->label == NULL) && (current_block->last_instruction == NULL)) {

		return;

	} else if ((current_block->label == NULL) && (statement_count == current_block->first_instruction->counter)) {

		return;

	} else {

		struct mcc_tac_basic_block *new_block = mcc_basic_block_add_new_block(data, false);
		assert(new_block);

		if (last_instruction->counter != statement_count) {
			return;
		}

		// iterate backwards
		new_block->last_instruction = last_instruction;

		while (last_instruction->previous_instruction != NULL &&
		       last_instruction->previous_instruction->counter == statement_count) {
			last_instruction = last_instruction->previous_instruction;
		}

		new_block->first_instruction = last_instruction;
		new_block->last_instruction = current_block->last_instruction;

		// if current block is empty but has label
		if (last_instruction->previous_instruction == NULL) {
			current_block->last_instruction = NULL;
			current_block->first_instruction = NULL;
		} else {
			current_block->last_instruction = last_instruction->previous_instruction;
			current_block->last_instruction->next_instruction = NULL;
		}

		new_block->child = current_block->child;
		current_block->child = NULL;
		return;
	}
}

static void mcc_tac_add_statement_while(struct mcc_ast_statement *statement, void *data)
{
	assert(statement);
	assert(data);

	struct userdata *userdata = data;
	struct mcc_tac_variable_stack_element *while_condition = mcc_variable_stack_pop(data);

	assert(while_condition);

	char *start_label = get_label(data);
	char *end_label = get_label(data);

	split_block(data, userdata->current_function->last_basic_block->last_statement_number);

	struct mcc_tac_basic_block *current_block = userdata->current_function->last_basic_block;

	current_block->label = start_label;

	// add condition instruction
	mcc_basic_block_add_instruction(current_block, while_condition->name, end_label, NULL,
	                                MCC_TAC_STATEMENT_TYPE_OPERATION_IF, userdata->statement_counter, false);

	struct mcc_tac_basic_block *while_block = current_block->child;

	while (while_block->next_block != NULL) {
		while_block = while_block->next_block;
	}

	while_block->jump_block = current_block;
	// add jump instruction
	mcc_basic_block_add_instruction(while_block, start_label, NULL, NULL, MCC_TAC_STATEMENT_TYPE_OPERATION_JUMP,
	                                userdata->statement_counter, false);

	current_block = mcc_basic_block_add_new_block(data, false);

	current_block->label = end_label;

	free(while_condition);
}

static void mcc_tac_add_statement(struct mcc_ast_statement *statement, void *data)
{
	assert(statement);
	assert(data);

	struct userdata *userdata = data;
	userdata->statement_counter = userdata->statement_counter + 1;
}

static void mcc_tac_enter_scope(void *statement, void *data)
{
	assert(statement);
	assert(data);

	mcc_basic_block_add_new_block(data, true);
}

static void mcc_tac_leave_scope(void *statement, void *data)
{
	assert(statement);
	assert(data);

	struct userdata *userdata = data;

	struct mcc_tac_basic_block *last = userdata->current_function->last_basic_block;

	userdata->current_function->last_basic_block = last->parent;
}

void add_parameters(void *data, struct mcc_ast_declaration *parameter, struct mcc_tac_basic_block *block)
{
	assert(data);
	assert(parameter);

	struct userdata *userdata = data;
	char *parameter_name =
	    mcc_get_identifier_variable_name(parameter->identifier, parameter->symbol_table, data, parameter->node);

	// recursively add parameters
	if (parameter->next_parameter != NULL) {
		add_parameters(data, parameter->next_parameter, block);
	}

	mcc_basic_block_add_instruction(block, parameter_name, NULL, NULL,
	                                MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_DECLARATION,
	                                userdata->statement_counter, true);
}

static void mcc_tac_add_function_definition(struct mcc_ast_function_definition *function_definition, void *data)
{
	assert(function_definition);
	assert(data);

	struct userdata *userdata = data;
	struct mcc_tac_basic_block *first_block = userdata->current_function->first_basic_block;

	userdata->current_function->function_name = function_definition->identifier;

	struct mcc_ast_declaration *parameter = function_definition->parameters;
	if (parameter != NULL) {

		// if first block is already labeled -> prepend new block
		if (first_block->label != NULL) {

			struct mcc_tac_basic_block *new_block = malloc(sizeof(*new_block));
			*new_block = (struct mcc_tac_basic_block){0};

			new_block->next_block = first_block;
			userdata->current_function->first_basic_block = new_block;
		}

		add_parameters(data, parameter, first_block);
	}

	// if function is void => it has no return (add one)
	if (function_definition->return_type == MCC_AST_TYPE_VOID) {
		struct mcc_tac_basic_block *current_block = userdata->current_function->last_basic_block;
		mcc_basic_block_add_instruction(current_block, NULL, NULL, NULL,
		                                MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_VOID,
		                                userdata->statement_counter, false);
	}

	if (function_definition->next_definition != NULL) {

		struct mcc_tac_function_blocks *function = malloc(sizeof(*function));
		function->function_name = NULL;
		function->next_function = NULL;
		struct mcc_tac_basic_block *basic_block = malloc(sizeof(*basic_block));

		*basic_block = (struct mcc_tac_basic_block){0};
		basic_block->child = NULL;

		function->first_basic_block = basic_block;
		function->last_basic_block = basic_block;

		userdata->current_function->next_function = function;
		userdata->current_function = function;
	}
}

static struct mcc_ast_visitor create_tac_visitor(struct userdata *userdata)
{
	assert(userdata);

	return (struct mcc_ast_visitor){.order = MCC_AST_VISIT_POST_ORDER,
	                                .userdata = userdata,
	                                .literal_int = mcc_tac_add_literal_int,
	                                .literal_float = mcc_tac_add_literal_float,
	                                .literal_bool = mcc_tac_add_literal_bool,
	                                .literal_string = mcc_tac_add_literal_string,
	                                .declaration = mcc_tac_add_declaration,
	                                .assignment = mcc_tac_add_assignment,
	                                .expression_identifier = mcc_tac_add_expression_identifier,
	                                .expression_binary_op = mcc_tac_add_binary_op,
	                                .expression_unary_op = mcc_tac_add_unary_op,
	                                .expression_function_call = mcc_tac_add_expression_func_call,
	                                .statement = mcc_tac_add_statement,
	                                .statement_return = mcc_tac_add_statement_return,
	                                .statement_if = mcc_tac_add_statement_if,
	                                .statement_while = mcc_tac_add_statement_while,
	                                .enter_if_scope = mcc_tac_enter_scope,
	                                .leave_if_scope = mcc_tac_leave_scope,
	                                .enter_while_scope = mcc_tac_enter_scope, // same as for if -> use same function
	                                .leave_while_scope = mcc_tac_leave_scope, // same as for if -> use same function
	                                .function_definition = mcc_tac_add_function_definition};
}

struct mcc_tac_function_blocks *mcc_ast_to_tac(struct mcc_ast_function_definition *program)
{
	assert(program);

	struct mcc_tac_function_blocks *function = malloc(sizeof(*function));

	struct mcc_tac_basic_block *basic_block = malloc(sizeof(*basic_block));

	basic_block->child = NULL;
	basic_block->first_element = NULL;
	basic_block->first_instruction = NULL;
	basic_block->last_element = NULL;
	basic_block->last_instruction = NULL;
	basic_block->next_block = NULL;
	basic_block->next_child = NULL;
	basic_block->parent = NULL;
	basic_block->jump_block = NULL;
	basic_block->label = NULL;
	basic_block->last_statement_number = 0;

	function->first_basic_block = basic_block;
	function->last_basic_block = basic_block;
	function->next_function = NULL;
	function->function_name = NULL;

	struct userdata *userdata = malloc(sizeof(*userdata));
	userdata->first_function = function;
	userdata->current_function = function;
	userdata->label_counter = 0;
	userdata->statement_counter = 0;
	userdata->symbol_table = NULL;
	userdata->variable_counter = 0;

	struct mcc_ast_visitor visitor = create_tac_visitor(userdata);
	mcc_ast_visit_function_definition(program, &visitor);

	function = userdata->first_function;

	free(userdata);

	return function;
}

void mcc_tac_delete_stack_element(struct mcc_tac_variable_stack_element *stack_element)
{
	assert(stack_element);
	if (stack_element->next_element) {
		mcc_tac_delete_stack_element(stack_element->next_element);
	}
	if (stack_element->name) {
		free(stack_element->name);
	}

	free(stack_element);
}
void mcc_tac_delete_tac_instruction(struct mcc_tac_instruction *tac)
{
	assert(tac);
	if (tac->argument1) {
		free(tac->argument1);
	}
	if (tac->argument2) {
		free(tac->argument2);
	}
	if (tac->result) {
		free(tac->result);
	}
	if (tac->next_instruction) {
		mcc_tac_delete_tac_instruction(tac->next_instruction);
	}

	free(tac);
}

void mcc_tac_delete_block(struct mcc_tac_basic_block *block)
{
	assert(block);

	if (block->child) {
		mcc_tac_delete_block(block->child);
	}

	if (block->next_child) {
		mcc_tac_delete_block(block->next_child);
	}
	if (block->next_block) {
		mcc_tac_delete_block(block->next_block);
	}
	if (block->first_instruction) {
		mcc_tac_delete_tac_instruction(block->first_instruction);
	}
	if (block->first_element) {
		mcc_tac_delete_stack_element(block->first_element);
	}

	free(block);
}

void mcc_tac_delete_tac(struct mcc_tac_function_blocks *function)
{
	assert(function);

	if (function->first_basic_block != NULL) {
		mcc_tac_delete_block(function->first_basic_block);
	}
	if (function->next_function != NULL) {
		mcc_tac_delete_tac(function->next_function);
	}

	free(function);
}
