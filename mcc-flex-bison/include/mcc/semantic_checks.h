#ifndef MCC_SEMANTIC_CHECK_H
#define MCC_SEMANTIC_CHECK_H

#include <stdbool.h>

#include "mcc/ast.h"
#include "mcc/symbol_table.h"

#define ERROR_MSG_SIZE 128

enum semantic_error_type {
	MCC_SEMANTIC_ERROR_NO_MAIN,
	MCC_SEMANTIC_ERROR_WRONG_SIGNATURE_MAIN,
	MCC_SEMANTIC_ERROR_FUNCTION_DEFINITION_DUPLICATE,
	MCC_SEMANTIC_ERROR_VARIABLE_DUPLICATE,
	MCC_SEMANTIC_ERROR_ASSIGN_VALUE_ARRAY,
	MCC_SEMANTIC_ERROR_TYPE_MISMATCH_IDENTIFIER,
	MCC_SEMANTIC_ERROR_TYPE_MISMATCH_LITERAL,
	MCC_SEMANTIC_ERROR_TYPE_MISMATCH_BINARY_OP,
	MCC_SEMANTIC_ERROR_TYPE_MISMATCH_UNARY_OP,
	MCC_SEMANTIC_ERROR_TYPE_MISMATCH_FUNCTION_CALL,
	MCC_SEMANTIC_ERROR_TYPE_MISMATCH_DECLARATION,
	MCC_SEMANTIC_ERROR_MISSING_DECLARATION,
	MCC_SEMANTIC_ERROR_NOT_IN_SYMBOL_TABLE,
	MCC_SEMANTIC_ERROR_TOO_MANY_ARGUMENTS,
	MCC_SEMANTIC_ERROR_TOO_FEW_ARGUMENTS,
	MCC_SEMANTIC_ERROR_ARRAY_EXPECTED,
	MCC_SEMANTIC_ERROR_VALUE_EXPECTED,
	MCC_SEMANTIC_ERROR_ARRAY_WRONG_SIZE,
	MCC_SEMANTIC_ERROR_FUNCTION_NON_EMPTY_RETURN,
	MCC_SEMANTIC_ERROR_FUNCTION_EMPTY_RETURN,
	MCC_SEMANTIC_ERROR_FUNCTION_WRONG_RETURN_TYPE,
	MCC_SEMANTIC_CHECK_FUNCTION_HAS_PATH_WITHOUT_RETURN
};

struct mcc_semantic_error {
	struct mcc_ast_node *node;
	bool hasError;
	enum semantic_error_type error_type;
	char error_msg[ERROR_MSG_SIZE];
};

struct mcc_semantic_check {
	struct mcc_symbol_table *symbol_table;
	enum mcc_ast_type type;
	struct mcc_ast_function_definition *current_function;
	struct mcc_semantic_error *error;
	struct mcc_ast_statement *last_statement_in_scope;
};

struct mcc_semantic_error *mcc_check_semantics(struct mcc_ast_function_definition *function_definition,
                                               struct mcc_symbol_table *symbol_table);

#endif
