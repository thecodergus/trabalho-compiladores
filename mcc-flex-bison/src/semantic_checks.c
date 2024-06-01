#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc/ast.h"
#include "mcc/ast_visit.h"
#include "mcc/semantic_checks.h"

// Forward declarations
static bool check_expression_has_type(struct mcc_ast_expression *expression,
                                      enum mcc_ast_type required_type,
                                      bool arr,
                                      struct mcc_symbol_table *symbol_table,
                                      struct mcc_semantic_error *error);
static bool check_type_expression_identifier_has_type(struct mcc_ast_expression *expression,
                                                      enum mcc_ast_type type,
                                                      bool arr,
                                                      int arr_size,
                                                      struct mcc_symbol_table *symbol_table,
                                                      struct mcc_semantic_error *error);

static char *get_type_string(enum mcc_ast_type data_type)
{

	switch (data_type) {

	case MCC_AST_TYPE_INT:
		return "int";

	case MCC_AST_TYPE_FLOAT:
		return "float";

	case MCC_AST_TYPE_BOOL:
		return "bool";

	case MCC_AST_TYPE_STRING:
		return "string";

	case MCC_AST_TYPE_VOID:
		return "void";

	default:
		return "getTypeStringError";
	}
}

// ------------------------------- begin check types ---------------------------------------------

static bool check_literal_has_type(struct mcc_ast_literal *literal,
                                   enum mcc_ast_type required_type,
                                   struct mcc_semantic_error *error)
{
	assert(literal);

	switch (literal->type) {
	case MCC_AST_LITERAL_TYPE_INT:
		if (required_type == MCC_AST_TYPE_INT) {
			return true;
		}
		break;

	case MCC_AST_LITERAL_TYPE_FLOAT:
		if (required_type == MCC_AST_TYPE_FLOAT) {
			return true;
		}
		break;

	case MCC_AST_LITERAL_TYPE_BOOL:
		if (required_type == MCC_AST_TYPE_BOOL)
			return true;
		break;

	case MCC_AST_LITERAL_TYPE_STRING:
		if (required_type == MCC_AST_TYPE_STRING)
			return true;
		break;
	}

	error->node = &literal->node;
	error->hasError = true;
	snprintf(error->error_msg, sizeof(error->error_msg), "Literal has wrong type\n");
	error->error_type = MCC_SEMANTIC_ERROR_TYPE_MISMATCH_LITERAL;

	return false;
}

bool check_binary_op_has_type_int_float(struct mcc_ast_expression *expression,
                                        enum mcc_ast_type required_type,
                                        struct mcc_symbol_table *symbol_table,
                                        struct mcc_semantic_error *error)
{

	assert(expression);
	assert(symbol_table);
	assert(error);

	if ((required_type != MCC_AST_TYPE_INT) && (required_type != MCC_AST_TYPE_FLOAT) &&
	    (required_type != MCC_AST_TYPE_BOOL) && (required_type != MCC_AST_TYPE_VOID)) {

		error->node = &expression->node;
		error->hasError = true;
		snprintf(error->error_msg, sizeof(error->error_msg),
		         "Binary operation can only return type int/float but type %s required\n",
		         get_type_string(required_type));
		error->error_type = MCC_SEMANTIC_ERROR_TYPE_MISMATCH_BINARY_OP;

		return false;
	}

	// if lhs and rhs have same type
	if (check_expression_has_type(expression->lhs, MCC_AST_TYPE_INT, false, symbol_table, error) &&
	    check_expression_has_type(expression->rhs, MCC_AST_TYPE_INT, false, symbol_table, error)) {

		return true;

	} else {

		error->hasError = false;

		if (check_expression_has_type(expression->lhs, MCC_AST_TYPE_FLOAT, false, symbol_table, error) &&
		    check_expression_has_type(expression->rhs, MCC_AST_TYPE_FLOAT, false, symbol_table, error)) {

			return true;

		} else {

			error->node = &expression->node;
			error->hasError = true;
			snprintf(error->error_msg, sizeof(error->error_msg),
			         "Binary operator type mismatch of lhs and rhs\n");
			error->error_type = MCC_SEMANTIC_ERROR_TYPE_MISMATCH_BINARY_OP;

			return false;
		}
	}
}

bool check_binary_logic_op_has_type(struct mcc_ast_expression *expression,
                                    enum mcc_ast_type required_type,
                                    struct mcc_symbol_table *symbol_table,
                                    struct mcc_semantic_error *error)
{

	if ((required_type != MCC_AST_TYPE_BOOL) && (required_type != MCC_AST_TYPE_VOID)) {

		error->node = &expression->node;
		error->hasError = true;
		snprintf(error->error_msg, sizeof(error->error_msg),
		         "Logic binary operation can only return type bool\n");
		error->error_type = MCC_SEMANTIC_ERROR_TYPE_MISMATCH_BINARY_OP;

		return false;
	}

	if ((check_expression_has_type(expression->lhs, MCC_AST_TYPE_BOOL, false, symbol_table, error) &&
	     check_expression_has_type(expression->rhs, MCC_AST_TYPE_BOOL, false, symbol_table, error))) {

		return true;

	} else {

		check_expression_has_type(expression->lhs, MCC_AST_TYPE_BOOL, false, symbol_table, error);
		check_expression_has_type(expression->rhs, MCC_AST_TYPE_BOOL, false, symbol_table, error);

		error->node = &expression->node;
		error->hasError = true;
		snprintf(error->error_msg, sizeof(error->error_msg), "Binary operation: unsupported types\n");
		error->error_type = MCC_SEMANTIC_ERROR_TYPE_MISMATCH_BINARY_OP;

		return false;
	}

	return false;
}

bool check_binary_eq_op_has_type(struct mcc_ast_expression *expression,
                                 enum mcc_ast_type required_type,
                                 struct mcc_symbol_table *symbol_table,
                                 struct mcc_semantic_error *error)
{

	if ((required_type != MCC_AST_TYPE_BOOL) && (required_type != MCC_AST_TYPE_VOID)) {

		error->node = &expression->node;
		error->hasError = true;
		snprintf(error->error_msg, sizeof(error->error_msg),
		         "Comparison binary operation can only return type bool\n");
		error->error_type = MCC_SEMANTIC_ERROR_TYPE_MISMATCH_BINARY_OP;

		return false;
	}

	if ((check_expression_has_type(expression->lhs, MCC_AST_TYPE_BOOL, false, symbol_table, error) &&
	     check_expression_has_type(expression->rhs, MCC_AST_TYPE_BOOL, false, symbol_table, error))) {
		return true;
	}

	error->hasError = false;

	if ((check_expression_has_type(expression->lhs, MCC_AST_TYPE_INT, false, symbol_table, error) &&
	     check_expression_has_type(expression->rhs, MCC_AST_TYPE_INT, false, symbol_table, error))) {
		return true;
	}

	error->hasError = false;

	if ((check_expression_has_type(expression->lhs, MCC_AST_TYPE_FLOAT, false, symbol_table, error) &&
	     check_expression_has_type(expression->rhs, MCC_AST_TYPE_FLOAT, false, symbol_table, error))) {
		return true;
	}

	error->node = &expression->node;
	error->hasError = true;
	snprintf(error->error_msg, sizeof(error->error_msg), "Binary operator type mismatch of lhs and rhs\n");
	error->error_type = MCC_SEMANTIC_ERROR_TYPE_MISMATCH_BINARY_OP;

	return false;
}

static bool check_binary_op_has_type(struct mcc_ast_expression *expression,
                                     enum mcc_ast_type required_type,
                                     struct mcc_symbol_table *symbol_table,
                                     struct mcc_semantic_error *error)
{
	assert(expression);
	assert(symbol_table);
	assert(error);

	enum mcc_ast_binary_op bin_op = expression->op;

	if (bin_op == MCC_AST_BINARY_OP_ADD || bin_op == MCC_AST_BINARY_OP_SUB || bin_op == MCC_AST_BINARY_OP_MUL ||
	    bin_op == MCC_AST_BINARY_OP_DIV) {

		return check_binary_op_has_type_int_float(expression, required_type, symbol_table, error);

	} else {

		if ((required_type != MCC_AST_TYPE_BOOL) && (required_type != MCC_AST_TYPE_VOID)) {

			error->node = &expression->node;
			error->hasError = true;
			snprintf(error->error_msg, sizeof(error->error_msg),
			         "Binary operation can only return type bool\n");
			error->error_type = MCC_SEMANTIC_ERROR_TYPE_MISMATCH_BINARY_OP;

			return false;

		} else {

			if (bin_op == MCC_AST_BINARY_OP_LESS || bin_op == MCC_AST_BINARY_OP_GREATER ||
			    bin_op == MCC_AST_BINARY_OP_LESS_EQ || bin_op == MCC_AST_BINARY_OP_GREATER_EQ) {

				return check_binary_op_has_type_int_float(expression, required_type, symbol_table,
				                                          error);

			} else if (bin_op == MCC_AST_BINARY_OP_AND || bin_op == MCC_AST_BINARY_OP_OR) {

				return check_binary_logic_op_has_type(expression, required_type, symbol_table, error);

			} else if (bin_op == MCC_AST_BINARY_OP_EQ || bin_op == MCC_AST_BINARY_OP_NOT_EQ) {

				return check_binary_eq_op_has_type(expression, required_type, symbol_table, error);

			} else {

				return false;
			}
		}
	}
}

bool check_unary_op(struct mcc_ast_expression *expression,
                    enum mcc_ast_type type,
                    struct mcc_symbol_table *symbol_table,
                    struct mcc_semantic_error *error,
                    bool arr)
{

	if (expression->unary_op == MCC_AST_UNARY_OP_NOT) {

		if (type == MCC_AST_TYPE_BOOL) {

			return check_expression_has_type(expression->unary_expression, type, arr, symbol_table, error);

		} else {

			error->node = &expression->node;
			error->hasError = true;
			snprintf(error->error_msg, sizeof(error->error_msg),
			         "Unary operator not can only return type bool");
			error->error_type = MCC_SEMANTIC_ERROR_TYPE_MISMATCH_UNARY_OP;

			return false;
		}
	} else {
		// Allow expression if not depending on return type (void)
		if (type == MCC_AST_TYPE_INT || type == MCC_AST_TYPE_FLOAT || type == MCC_AST_TYPE_VOID)

			return check_expression_has_type(expression->unary_expression, type, arr, symbol_table, error);

		else {

			error->node = &expression->node;
			error->hasError = true;
			snprintf(error->error_msg, sizeof(error->error_msg),
			         "Unary operator - can only return type int or float");
			error->error_type = MCC_SEMANTIC_ERROR_TYPE_MISMATCH_UNARY_OP;

			return false;
		}
	}
}

struct mcc_symbol_table_entry *
get_symbol_entry_in_scope(struct mcc_symbol_table *node_symbol_table, char *identifier, struct mcc_ast_node node)
{
	struct mcc_symbol_table_entry *declaration_entry = NULL;

	declaration_entry = mcc_symbol_table_lookup_variable_entry(node_symbol_table, identifier, node);

	return declaration_entry;
}

// checks if identifiers are correct
static bool check_type_expression_identifier_has_type(struct mcc_ast_expression *expression,
                                                      enum mcc_ast_type type,
                                                      bool arr,
                                                      int arr_size,
                                                      struct mcc_symbol_table *symbol_table,
                                                      struct mcc_semantic_error *error)
{

	assert(expression);
	assert(symbol_table);

	struct mcc_symbol_table_entry *declaration_entry =
	    get_symbol_entry_in_scope(symbol_table, expression->identifier, expression->node);

	if (declaration_entry == NULL) {

		error->node = &expression->node;
		error->hasError = true;
		snprintf(error->error_msg, sizeof(error->error_msg),
		         "No declaration found in scope for identifier %s \n", expression->identifier);
		error->error_type = MCC_SEMANTIC_ERROR_NOT_IN_SYMBOL_TABLE;

		return false;

		// if not datatypes matching
	} else if (declaration_entry->data_type != type && type != MCC_AST_TYPE_VOID) {

		error->node = &expression->node;
		error->hasError = true;
		snprintf(error->error_msg, sizeof(error->error_msg), "Identifier %s has wrong type \n",
		         expression->identifier);
		error->error_type = MCC_SEMANTIC_ERROR_TYPE_MISMATCH_IDENTIFIER;

		return false;
	}

	if (arr == false) {
		// declaration is array
		if (declaration_entry->array_index != NULL) {
			// expression is not an array
			if (expression->index == NULL) {

				error->node = &expression->node;
				error->hasError = true;
				snprintf(error->error_msg, sizeof(error->error_msg),
				         "Identifier %s is value but array is expeceted \n", expression->identifier);
				error->error_type = MCC_SEMANTIC_ERROR_ARRAY_EXPECTED;

				return false;
			} else {

				assert(expression->symbol_table);

				if (!check_expression_has_type(expression->index, MCC_AST_TYPE_INT, arr,
				                               expression->symbol_table, error)) {
					return false;
				}

				return true;
			}
		} else {
			// Declaration is no array
			if (expression->index != NULL) {

				error->node = &expression->node;
				error->hasError = true;
				snprintf(error->error_msg, sizeof(error->error_msg),
				         "Identifier %s is array but value is expeceted \n", expression->identifier);
				error->error_type = MCC_SEMANTIC_ERROR_VALUE_EXPECTED;

				return false;
			} else {

				return true;
			}
		}
	}

	// Check array identifier
	if (declaration_entry->array_index == NULL) {
		// If array should be returned, it has to be declared with an index

		error->node = &expression->node;
		error->hasError = true;
		snprintf(error->error_msg, sizeof(error->error_msg), "Identifier %s is value but array is expeceted \n",
		         expression->identifier);
		error->error_type = MCC_SEMANTIC_ERROR_ARRAY_EXPECTED;

		return false;
	} else if (expression->index != NULL) {

		error->node = &expression->node;
		error->hasError = true;
		snprintf(error->error_msg, sizeof(error->error_msg), "Identifier %s should be array \n",
		         expression->identifier);
		error->error_type = MCC_SEMANTIC_ERROR_ARRAY_EXPECTED;

		return false;

	} else {

		// Variable is array but no indexing
		if (arr_size > 0) {
			// Expression array needs certain size (e.g. function call)
			if (declaration_entry->array_index->i_value == arr_size) { // Size is correct
				return true;
			} else {
				// Size is not correct

				error->node = &expression->node;
				error->hasError = true;
				snprintf(error->error_msg, sizeof(error->error_msg), "Array %s has wrong size %ld \n",
				         expression->identifier, declaration_entry->array_index->i_value);
				error->error_type = MCC_SEMANTIC_ERROR_ARRAY_WRONG_SIZE;

				return false;
			}
		} else {

			return true;
		}
	}
}

bool check_function_call_array_argument(struct mcc_ast_expression *expression,
                                        struct mcc_ast_expression *function_call_argument,
                                        struct mcc_ast_declaration *function_definition_parameter,
                                        struct mcc_symbol_table *symbol_table,
                                        struct mcc_semantic_error *error)
{

	assert(expression);
	assert(function_call_argument);
	assert(function_definition_parameter);
	assert(symbol_table);

	if (function_call_argument->type == MCC_AST_EXPRESSION_TYPE_IDENTIFIER) {
		// check if array
		bool array_check = check_type_expression_identifier_has_type(
		    function_call_argument, function_definition_parameter->type, true,
		    function_definition_parameter->literal->i_value, symbol_table, error);
		return array_check;

	} else {

		error->node = &expression->node;
		error->hasError = true;
		snprintf(error->error_msg, sizeof(error->error_msg),
		         "Function call %s argument %s was expected to be an array\n",
		         expression->function_call->identifier, function_definition_parameter->identifier);
		error->error_type = MCC_SEMANTIC_ERROR_ARRAY_EXPECTED;

		return false;
	}
}

bool check_function_call_exp_argument(struct mcc_ast_expression *expression,
                                      struct mcc_ast_expression *function_call_argument,
                                      struct mcc_ast_declaration *function_definition_parameter,
                                      struct mcc_symbol_table *symbol_table,
                                      struct mcc_semantic_error *error)
{

	assert(expression);
	assert(function_call_argument);
	assert(function_definition_parameter);
	assert(symbol_table);

	if (!check_expression_has_type(function_call_argument, function_definition_parameter->type, false, symbol_table,
	                               error)) {

		if (error->hasError == false) {

			error->node = &expression->node;
			error->hasError = true;
			snprintf(error->error_msg, sizeof(error->error_msg),
			         "Function call %s argument %s has wrong type\n", expression->function_call->identifier,
			         function_definition_parameter->identifier);
			error->error_type = MCC_SEMANTIC_ERROR_TYPE_MISMATCH_FUNCTION_CALL;
		}
		return false;
	}
	return true;
}

bool check_function_call_arguments(struct mcc_ast_expression *expression,
                                   struct mcc_symbol_table *symbol_table,
                                   struct mcc_semantic_error *error)
{

	assert(expression);
	assert(symbol_table);
	assert(expression->function_call);

	struct mcc_symbol_table_entry *function_entry =
	    mcc_symbol_table_lookup_function_entry(symbol_table, expression->function_call->identifier);

	struct mcc_ast_expression *function_call_argument = expression->function_call->arguments;
	struct mcc_ast_declaration *function_definition_parameter = function_entry->parameters;

	while (function_call_argument != NULL && function_definition_parameter != NULL) {

		if (function_definition_parameter->literal != NULL) {
			bool array_check = check_function_call_array_argument(
			    expression, function_call_argument, function_definition_parameter, symbol_table, error);

			if (!array_check) {
				return false;
			}

		} else {
			bool exp_check = check_function_call_exp_argument(
			    expression, function_call_argument, function_definition_parameter, symbol_table, error);
			if (!exp_check) {
				return false;
			}
		}

		function_call_argument = function_call_argument->next_expression;
		function_definition_parameter = function_definition_parameter->next_parameter;
	}

	if (function_call_argument != NULL) {

		error->node = &expression->node;
		error->hasError = true;
		snprintf(error->error_msg, sizeof(error->error_msg), "Function call %s has too many arguments\n",
		         expression->function_call->identifier);
		error->error_type = MCC_SEMANTIC_ERROR_TOO_MANY_ARGUMENTS;

		return false;

	} else if (function_definition_parameter != NULL) {

		error->node = &expression->node;
		error->hasError = true;
		snprintf(error->error_msg, sizeof(error->error_msg), "Function call %s has too few arguments\n",
		         expression->function_call->identifier);
		error->error_type = MCC_SEMANTIC_ERROR_TOO_FEW_ARGUMENTS;

		return false;
	}

	return true;
}

bool check_function_call_has_type(struct mcc_ast_expression *expression,
                                  enum mcc_ast_type required_type,
                                  struct mcc_symbol_table *symbol_table,
                                  struct mcc_semantic_error *error)
{
	assert(expression);
	assert(symbol_table);
	assert(expression->function_call);

	struct mcc_symbol_table_entry *function_entry =
	    mcc_symbol_table_lookup_function_entry(symbol_table, expression->function_call->identifier);

	if (function_entry == NULL) {
	}

	if (function_entry == NULL) {

		error->node = &expression->node;
		error->hasError = true;
		snprintf(error->error_msg, sizeof(error->error_msg), "Undefined function for function call %s()\n",
		         expression->function_call->identifier);
		error->error_type = MCC_SEMANTIC_ERROR_NOT_IN_SYMBOL_TABLE;

		return false;

	} else if (function_entry->data_type == required_type || required_type == MCC_AST_TYPE_VOID) {

		return check_function_call_arguments(expression, symbol_table, error);

	} else {

		error->node = &expression->node;
		error->hasError = true;
		snprintf(error->error_msg, sizeof(error->error_msg), "Function call %s has wrong return type\n",
		         expression->function_call->identifier);
		error->error_type = MCC_SEMANTIC_ERROR_TYPE_MISMATCH_FUNCTION_CALL;

		return false;
	}
}

static bool check_expression_has_type(struct mcc_ast_expression *expression,
                                      enum mcc_ast_type required_type,
                                      bool arr,
                                      struct mcc_symbol_table *symbol_table,
                                      struct mcc_semantic_error *error)
{
	assert(expression);
	assert(symbol_table);
	assert(error);

	if (error->hasError) {
		return false;
	}

	switch (expression->type) {
	case MCC_AST_EXPRESSION_TYPE_LITERAL:
		return check_literal_has_type(expression->literal, required_type, error);
		break;
	case MCC_AST_EXPRESSION_TYPE_BINARY_OP:
		return check_binary_op_has_type(expression, required_type, symbol_table, error);
		break;

	case MCC_AST_EXPRESSION_TYPE_UNARY_OP:
		return check_unary_op(expression, required_type, symbol_table, error, arr);
		break;

	case MCC_AST_EXPRESSION_TYPE_PARENTH:
		return check_expression_has_type(expression->expression, required_type, arr, symbol_table, error);
		break;
	case MCC_AST_EXPRESSION_TYPE_FUNCTION_CALL:
		return check_function_call_has_type(expression, required_type, symbol_table, error);
		break;
	case MCC_AST_EXPRESSION_TYPE_IDENTIFIER:
		return check_type_expression_identifier_has_type(expression, required_type, arr, -1, symbol_table,
		                                                 error);
		break;
	}
	return false;
}

// ------------------------------- end check types ---------------------------------------------

bool check_main_function_exists(struct mcc_ast_function_definition *function_definition,
                                struct mcc_semantic_error *error)
{
	assert(function_definition);
	assert(error);

	struct mcc_ast_function_definition *fun_def = function_definition;

	while (fun_def) {

		if (strcmp(fun_def->identifier, "main") == 0) {

			if ((fun_def->return_type == MCC_AST_TYPE_INT) && (fun_def->parameters == NULL)) {

				return true;

			} else {

				error->node = &fun_def->node;
				error->hasError = true;
				snprintf(error->error_msg, sizeof(error->error_msg),
				         "Main function has wrong signature");
				error->error_type = MCC_SEMANTIC_ERROR_WRONG_SIGNATURE_MAIN;

				return false;
			}
		}
		fun_def = fun_def->next_definition;
	}

	error->node = &function_definition->node;
	error->hasError = true;
	snprintf(error->error_msg, sizeof(error->error_msg), "Main function does not exist or has wrong signature");
	error->error_type = MCC_SEMANTIC_ERROR_NO_MAIN;

	return false;
}

bool check_for_function_duplicates(struct mcc_ast_function_definition *function_definition,
                                   struct mcc_semantic_error *error)
{
	assert(function_definition);
	assert(error);

	struct mcc_ast_function_definition *current_fun_def = function_definition;

	while (current_fun_def) {

		struct mcc_ast_function_definition *fun_def_iterator = current_fun_def->next_definition;

		while (fun_def_iterator) {

			if (strcmp(current_fun_def->identifier, fun_def_iterator->identifier) == 0) {

				error->node = &current_fun_def->node;
				error->hasError = true;
				snprintf(error->error_msg, sizeof(error->error_msg),
				         "Function %s is declared multiple times", current_fun_def->identifier);
				error->error_type = MCC_SEMANTIC_ERROR_FUNCTION_DEFINITION_DUPLICATE;

				return true;
			}

			fun_def_iterator = fun_def_iterator->next_definition;
		}

		current_fun_def = current_fun_def->next_definition;
	}

	return false;
}

bool check_for_variable_declaration_duplicates(struct mcc_symbol_table *symbol_table, struct mcc_semantic_error *error)
{

	assert(symbol_table);
	assert(error);

	struct mcc_symbol_table_entry *current_entry = symbol_table->first_entry;

	while (current_entry) {

		if (current_entry->symbol_type == MCC_SYMBOL_TABLE_ENTRY_TYPE_FUNCTION) {
			current_entry = current_entry->next;
			continue;
		}

		struct mcc_symbol_table_entry *compare_entry = current_entry->next;

		while (compare_entry) {

			if (compare_entry->symbol_type == MCC_SYMBOL_TABLE_ENTRY_TYPE_FUNCTION) {
				compare_entry = compare_entry->next;
				continue;
			}

			if (strcmp(current_entry->name, compare_entry->name) == 0) {

				error->node = &compare_entry->node;
				error->hasError = true;
				snprintf(error->error_msg, sizeof(error->error_msg),
				         "Variable %s is declared multiple times in the same scope",
				         current_entry->name);
				error->error_type = MCC_SEMANTIC_ERROR_VARIABLE_DUPLICATE;

				return true;
			}
			compare_entry = compare_entry->next;
		}
		current_entry = current_entry->next;
	}

	struct mcc_symbol_table *child_table = symbol_table->first_child;

	while (child_table) {
		if (check_for_variable_declaration_duplicates(child_table, error)) {
			return true;
		};

		child_table = child_table->next;
	}

	return false;
}

static void check_assignment(struct mcc_ast_assignment *assignment, void *data)
{
	assert(assignment);
	assert(data);

	assert(assignment->symbol_table);

	struct mcc_semantic_check *semantic_check = data;

	assert(semantic_check->symbol_table);
	assert(semantic_check->error);

	struct mcc_symbol_table *symbol_table = assignment->symbol_table;
	struct mcc_semantic_error *error = semantic_check->error;

	if (error->hasError) {
		return;
	}

	struct mcc_symbol_table_entry *declaration_entry =
	    mcc_symbol_table_lookup_variable_entry(symbol_table, assignment->identifier, assignment->node);

	if (declaration_entry) {

		if (assignment->index == NULL && declaration_entry->array_index != NULL) {

			error->node = &assignment->node;
			error->hasError = true;
			snprintf(error->error_msg, sizeof(error->error_msg), "Value cannot be assigned to arrray");
			error->error_type = MCC_SEMANTIC_ERROR_ASSIGN_VALUE_ARRAY;
			return;

		} else if (!check_expression_has_type(assignment->value, declaration_entry->data_type, false,
		                                      assignment->symbol_table, semantic_check->error))

			if (assignment->index != NULL) {
				check_expression_has_type(assignment->value, MCC_AST_TYPE_INT, false,
				                          assignment->symbol_table, semantic_check->error);
			}

	} else {

		error->node = &assignment->node;
		error->hasError = true;
		snprintf(error->error_msg, sizeof(error->error_msg),
		         "Missing declaration for identifier %s in this scope", assignment->identifier);
		error->error_type = MCC_SEMANTIC_ERROR_MISSING_DECLARATION;
	}

	return;
}

static void check_declaration(struct mcc_ast_declaration *declaration, void *data)
{
	assert(declaration);
	assert(data);

	struct mcc_semantic_check *semantic_check = data;
	struct mcc_semantic_error *error = semantic_check->error;

	if (error->hasError) {
		return;
	}

	if (declaration->literal != NULL) {

		if (declaration->literal->type != MCC_AST_LITERAL_TYPE_INT) {

			error->node = &declaration->node;
			error->hasError = true;
			snprintf(error->error_msg, sizeof(error->error_msg),
			         "Array declaration %s has wrong index type", declaration->identifier);
			error->error_type = MCC_SEMANTIC_ERROR_TYPE_MISMATCH_DECLARATION;

			return;

		} else if (declaration->literal->i_value < 1) {

			error->node = &declaration->node;
			error->hasError = true;
			snprintf(error->error_msg, sizeof(error->error_msg), "Array declaration %s has size smaller 1",
			         declaration->identifier);
			error->error_type = MCC_SEMANTIC_ERROR_ARRAY_WRONG_SIZE;

			return;
		}

		return;
	}
}

static void check_statement(struct mcc_ast_statement *statement, void *data)
{
	assert(statement);
	assert(data);
	assert(statement->symbol_table);

	struct mcc_semantic_check *semantic_check = data;
	struct mcc_semantic_error *error = semantic_check->error;

	assert(semantic_check->error);

	if (error->hasError) {
		return;
	}

	if (statement->type == MCC_AST_STATEMENT_TYPE_EXPRESSION) {

		check_expression_has_type(statement->expression, MCC_AST_TYPE_VOID, false, statement->symbol_table,
		                          semantic_check->error);
	} else {
		return;
	}
}

static void check_statement_if(struct mcc_ast_statement *statement, void *data)
{
	assert(statement);
	assert(data);
	assert(statement->symbol_table);

	struct mcc_semantic_check *semantic_check = data;
	struct mcc_semantic_error *error = semantic_check->error;

	if (error->hasError) {
		return;
	}

	check_expression_has_type(statement->if_expression, MCC_AST_TYPE_BOOL, false, statement->symbol_table,
	                          semantic_check->error);
}

static void check_statement_while(struct mcc_ast_statement *statement, void *data)
{
	assert(statement);
	assert(data);
	assert(statement->symbol_table);

	struct mcc_semantic_check *semantic_check = data;
	struct mcc_semantic_error *error = semantic_check->error;

	if (error->hasError) {
		return;
	}

	check_expression_has_type(statement->while_expression, MCC_AST_TYPE_BOOL, false, statement->symbol_table,
	                          semantic_check->error);
}

bool check_all_paths_return(struct mcc_ast_statement *statement, struct mcc_semantic_check *semantic_check)
{
	if (statement == NULL) {
		return false;
	}

	assert(statement);
	struct mcc_ast_statement *last_statement = statement;

	while (last_statement->next_statement != NULL) {
		last_statement = last_statement->next_statement;
	}

	if (last_statement->type == MCC_AST_STATEMENT_TYPE_RETURN) {

		return true;

	} else if (last_statement->type == MCC_AST_STATEMENT_TYPE_COMPOUND) {

		return check_all_paths_return(last_statement->compound_statement, semantic_check);

	} else if (last_statement->type == MCC_AST_STATEMENT_TYPE_IF) {

		if (last_statement->else_statement == NULL) {

			return false;

		} else {
			return check_all_paths_return(last_statement->if_statement, semantic_check) &&
			       check_all_paths_return(last_statement->else_statement, semantic_check);
		}

	} else {

		return false;
	}
}

static void check_function_definition(struct mcc_ast_function_definition *function_definition, void *data)
{
	assert(function_definition);
	assert(function_definition->statements);
	assert(data);

	struct mcc_semantic_check *semantic_check = data;
	struct mcc_semantic_error *error = semantic_check->error;

	semantic_check->current_function = function_definition;

	if (error->hasError) {
		return;
	}

	if (semantic_check->current_function->return_type != MCC_AST_TYPE_VOID) {

		bool has_return = check_all_paths_return(function_definition->statements, semantic_check);

		if (!has_return) {

			error->node = &function_definition->node;
			error->hasError = true;
			snprintf(error->error_msg, sizeof(error->error_msg),
			         "In function %s exists a path with no return\n",
			         semantic_check->current_function->identifier);
			error->error_type = MCC_SEMANTIC_CHECK_FUNCTION_HAS_PATH_WITHOUT_RETURN;

			return;
		}
	}
}

static void check_statement_return(struct mcc_ast_statement *statement, void *data)
{
	assert(statement);
	assert(data);
	assert(statement->symbol_table);

	struct mcc_semantic_check *semantic_check = data;
	struct mcc_semantic_error *error = semantic_check->error;

	if (error->hasError) {
		return;
	}

	if (semantic_check->current_function->return_type == MCC_AST_TYPE_VOID) {

		if (statement->return_expression != NULL) {

			error->node = &statement->node;
			error->hasError = true;
			snprintf(error->error_msg, sizeof(error->error_msg),
			         "Function %s of return type void has non empty return\n",
			         semantic_check->current_function->identifier);
			error->error_type = MCC_SEMANTIC_ERROR_FUNCTION_NON_EMPTY_RETURN;

			return;
		}

	} else {

		if (statement->return_expression == NULL) {

			error->node = &statement->node;
			error->hasError = true;
			snprintf(error->error_msg, sizeof(error->error_msg),
			         "Function %s is not void but does not return value\n",
			         semantic_check->current_function->identifier);
			error->error_type = MCC_SEMANTIC_ERROR_FUNCTION_EMPTY_RETURN;

			return;
		} else if (!check_expression_has_type(statement->return_expression,
		                                      semantic_check->current_function->return_type, false,
		                                      statement->symbol_table, semantic_check->error)) {

			error->node = &statement->node;
			error->hasError = true;
			snprintf(error->error_msg, sizeof(error->error_msg), "Function %s has wrong return type\n",
			         semantic_check->current_function->identifier);
			error->error_type = MCC_SEMANTIC_ERROR_FUNCTION_WRONG_RETURN_TYPE;

			return;
		}
	}
}

static struct mcc_ast_visitor create_semantic_checks_visitor(struct mcc_symbol_table *table,
                                                             struct mcc_semantic_error *error)
{
	struct mcc_semantic_check *semantic_check = malloc(sizeof(*semantic_check));

	semantic_check->symbol_table = table;
	semantic_check->error = error;
	semantic_check->current_function = NULL;
	semantic_check->last_statement_in_scope = NULL;

	return (struct mcc_ast_visitor){
	    .traversal = MCC_AST_VISIT_DEPTH_FIRST,
	    .order = MCC_AST_VISIT_PRE_ORDER,

	    .userdata = semantic_check,
	    .assignment = check_assignment,
	    .declaration_identifier = check_declaration,
	    .statement = check_statement,
	    .statement_if = check_statement_if,
	    .statement_while = check_statement_while,
	    .function_definition = check_function_definition,
	    .statement_return = check_statement_return,
	};
}

struct mcc_semantic_error *mcc_check_semantics(struct mcc_ast_function_definition *function_definition,
                                               struct mcc_symbol_table *symbol_table)
{
	assert(function_definition);
	assert(symbol_table);

	struct mcc_semantic_error *error = malloc(sizeof(*error));
	if (!error) {
		return NULL;
	}

	error->node = NULL;
	error->hasError = false;

	if (!check_main_function_exists(function_definition, error)) {
		return error;
	}

	if (check_for_function_duplicates(function_definition, error)) {
		return error;
	}

	if (check_for_variable_declaration_duplicates(symbol_table, error)) {
		return error;
	};

	struct mcc_ast_visitor visitor = create_semantic_checks_visitor(symbol_table, error);
	mcc_ast_visit_function_definition(function_definition, &visitor);

	free(visitor.userdata);

	return error;
}
