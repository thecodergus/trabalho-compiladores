#include "mcc/ast.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

// ############################################## Expressions ##############################################

struct mcc_ast_expression *mcc_ast_new_expression_literal(struct mcc_ast_literal *literal)
{
	assert(literal);

	struct mcc_ast_expression *expr = malloc(sizeof(*expr));
	if (!expr) {
		return NULL;
	}

	expr->type = MCC_AST_EXPRESSION_TYPE_LITERAL;
	expr->literal = literal;
	expr->next_expression = NULL;
	return expr;
}

struct mcc_ast_expression *mcc_ast_new_expression_binary_op(enum mcc_ast_binary_op op,
                                                            struct mcc_ast_expression *lhs,
                                                            struct mcc_ast_expression *rhs)
{
	assert(lhs);
	assert(rhs);

	struct mcc_ast_expression *expr = malloc(sizeof(*expr));
	if (!expr) {
		return NULL;
	}

	expr->type = MCC_AST_EXPRESSION_TYPE_BINARY_OP;
	expr->op = op;
	expr->lhs = lhs;
	expr->rhs = rhs;
	expr->next_expression = NULL;
	return expr;
}

struct mcc_ast_expression *mcc_ast_new_expression_unary_op(enum mcc_ast_unary_op unary_op,
                                                           struct mcc_ast_expression *unary_expression)
{
	assert(unary_expression);

	struct mcc_ast_expression *expr = malloc(sizeof(*expr));

	if (!expr) {
		return NULL;
	}

	expr->type = MCC_AST_EXPRESSION_TYPE_UNARY_OP;
	expr->unary_op = unary_op;
	expr->unary_expression = unary_expression;
	expr->next_expression = NULL;
	return expr;
}

struct mcc_ast_expression *mcc_ast_new_expression_parenth(struct mcc_ast_expression *expression)
{
	assert(expression);

	struct mcc_ast_expression *expr = malloc(sizeof(*expr));
	if (!expr) {
		return NULL;
	}

	expr->type = MCC_AST_EXPRESSION_TYPE_PARENTH;
	expr->expression = expression;
	expr->next_expression = NULL;
	return expr;
}

struct mcc_ast_expression *mcc_ast_new_expression_identifier(char *identifier, struct mcc_ast_expression *index)
{

	assert(identifier);

	struct mcc_ast_expression *expr = malloc(sizeof(*expr));
	if (!expr) {
		return NULL;
	}

	expr->type = MCC_AST_EXPRESSION_TYPE_IDENTIFIER;
	expr->identifier = identifier;
	expr->index = index;
	expr->next_expression = NULL;

	return expr;
}

struct mcc_ast_expression *mcc_ast_new_expression_function_call(struct mcc_ast_function_call *function_call)
{
	assert(function_call);

	struct mcc_ast_expression *expr = malloc(sizeof(*expr));
	if (!expr) {
		return NULL;
	}

	expr->type = MCC_AST_EXPRESSION_TYPE_FUNCTION_CALL;
	expr->function_call = function_call;
	expr->next_expression = NULL;

	return expr;
}

struct mcc_ast_expression *mcc_ast_new_expression_linked_argument(struct mcc_ast_expression *first_expression,
                                                                  struct mcc_ast_expression *next_expression)
{

	assert(first_expression);
	assert(next_expression);

	first_expression->next_expression = next_expression;
	return first_expression;
}

void mcc_ast_delete_expression(struct mcc_ast_expression *expression)
{
	assert(expression);

	switch (expression->type) {
	case MCC_AST_EXPRESSION_TYPE_LITERAL:
		mcc_ast_delete_literal(expression->literal);
		break;

	case MCC_AST_EXPRESSION_TYPE_BINARY_OP:
		mcc_ast_delete_expression(expression->lhs);
		mcc_ast_delete_expression(expression->rhs);
		break;

	case MCC_AST_EXPRESSION_TYPE_UNARY_OP:
		mcc_ast_delete_expression(expression->unary_expression);
		break;

	case MCC_AST_EXPRESSION_TYPE_IDENTIFIER:
		free(expression->identifier);
		if (expression->index != NULL) {
			mcc_ast_delete_expression(expression->index);
		}
		break;

	case MCC_AST_EXPRESSION_TYPE_PARENTH:
		mcc_ast_delete_expression(expression->expression);
		break;
	case MCC_AST_EXPRESSION_TYPE_FUNCTION_CALL:
		mcc_ast_delete_function_call(expression->function_call);
		break;
	}

	if (expression->next_expression != NULL) {
		mcc_ast_delete_expression(expression->next_expression);
	}

	free(expression);
}

// ############################################## Literals ##############################################

struct mcc_ast_literal *mcc_ast_new_literal_int(long value)
{
	struct mcc_ast_literal *lit = malloc(sizeof(*lit));
	if (!lit) {
		return NULL;
	}

	lit->type = MCC_AST_LITERAL_TYPE_INT;
	lit->i_value = value;
	return lit;
}

struct mcc_ast_literal *mcc_ast_new_literal_float(double value)
{
	struct mcc_ast_literal *lit = malloc(sizeof(*lit));
	if (!lit) {
		return NULL;
	}

	lit->type = MCC_AST_LITERAL_TYPE_FLOAT;
	lit->f_value = value;
	return lit;
}

struct mcc_ast_literal *mcc_ast_new_literal_string(char *value)
{
	struct mcc_ast_literal *lit = malloc(sizeof(*lit));
	if (!lit) {
		return NULL;
	}

	memmove(value, value + 1, strlen(value));
	value[strlen(value) - 1] = 0;

	lit->type = MCC_AST_LITERAL_TYPE_STRING;
	lit->s_value = value;
	return lit;
}

struct mcc_ast_literal *mcc_ast_new_literal_bool(char *value)
{
	struct mcc_ast_literal *lit = malloc(sizeof(*lit));

	if (!lit) {
		return NULL;
	}

	lit->type = MCC_AST_LITERAL_TYPE_BOOL;
	lit->b_value = value;
	return lit;
}

void mcc_ast_delete_literal(struct mcc_ast_literal *literal)
{
	assert(literal);

	switch (literal->type) {
	case MCC_AST_LITERAL_TYPE_STRING:
		free(literal->s_value);
		break;

	case MCC_AST_LITERAL_TYPE_BOOL:
		free(literal->b_value);
		break;

	default:
		break;
	}

	free(literal);
}
// ############################################## Declarations ##############################################

struct mcc_ast_declaration *
mcc_ast_new_declaration(enum mcc_ast_type type, struct mcc_ast_literal *literal, char *identifier)
{

	assert(identifier);

	struct mcc_ast_declaration *declaration = malloc(sizeof(*declaration));
	if (!declaration) {
		return NULL;
	}

	declaration->type = type;
	declaration->literal = literal;
	declaration->identifier = identifier;
	declaration->next_parameter = NULL;

	return declaration;
}

struct mcc_ast_declaration *mcc_ast_new_declaration_linked_parameter(struct mcc_ast_declaration *first_parameter,
                                                                     struct mcc_ast_declaration *next_parameter)
{

	assert(first_parameter);
	assert(next_parameter);

	first_parameter->next_parameter = next_parameter;

	return first_parameter;
}

void mcc_ast_delete_declaration(struct mcc_ast_declaration *declaration)
{

	assert(declaration);
	free(declaration->identifier);

	if (declaration->literal != NULL) {
		mcc_ast_delete_literal(declaration->literal);
	}
	if (declaration->next_parameter != NULL) {
		mcc_ast_delete_declaration(declaration->next_parameter);
	}

	free(declaration);
}

// ############################################## Assignments ##############################################

struct mcc_ast_assignment *
mcc_ast_new_assignment(char *identifier, struct mcc_ast_expression *index, struct mcc_ast_expression *value)
{
	assert(identifier);
	assert(value);

	struct mcc_ast_assignment *assignment = malloc(sizeof(*assignment));
	if (!assignment) {
		return NULL;
	}

	assignment->identifier = identifier;
	assignment->value = value;
	assignment->index = index;

	return assignment;
}

void mcc_ast_delete_assignment(struct mcc_ast_assignment *assignment)
{
	assert(assignment);
	free(assignment->identifier);
	mcc_ast_delete_expression(assignment->value);

	if (assignment->index != NULL) {
		mcc_ast_delete_expression(assignment->index);
	}

	free(assignment);
}

// ############################################## Statements ##############################################

struct mcc_ast_statement *mcc_ast_new_statement_if(struct mcc_ast_expression *if_expression,
                                                   struct mcc_ast_statement *if_statement,
                                                   struct mcc_ast_statement *else_statement)
{

	assert(if_expression);
	assert(if_statement);

	struct mcc_ast_statement *statement = malloc(sizeof(*statement));
	if (!statement) {
		return NULL;
	}

	statement->type = MCC_AST_STATEMENT_TYPE_IF;
	statement->if_expression = if_expression;
	statement->if_statement = if_statement;
	statement->else_statement = else_statement;
	statement->next_statement = NULL;

	return statement;
}

struct mcc_ast_statement *mcc_ast_new_statement_while(struct mcc_ast_expression *while_expression,
                                                      struct mcc_ast_statement *while_statement)
{

	assert(while_expression);
	assert(while_statement);

	struct mcc_ast_statement *statement = malloc(sizeof(*statement));
	if (!statement) {
		return NULL;
	}

	statement->type = MCC_AST_STATEMENT_TYPE_WHILE;
	statement->while_expression = while_expression;
	statement->while_statement = while_statement;
	statement->next_statement = NULL;

	return statement;
}

struct mcc_ast_statement *mcc_ast_new_statement_declaration(struct mcc_ast_declaration *declaration)
{
	assert(declaration);
	struct mcc_ast_statement *statement = malloc(sizeof(*statement));
	if (!statement) {
		return NULL;
	}

	statement->type = MCC_AST_STATEMENT_TYPE_DECLARATION;
	statement->declaration = declaration;
	statement->next_statement = NULL;

	return statement;
}

struct mcc_ast_statement *mcc_ast_new_statement_assignment(struct mcc_ast_assignment *assignment)
{
	assert(assignment);

	struct mcc_ast_statement *statement = malloc(sizeof(*statement));
	if (!statement) {
		return NULL;
	}

	statement->type = MCC_AST_STATEMENT_TYPE_ASSIGNMENT;
	statement->assignment = assignment;
	statement->next_statement = NULL;

	return statement;
}

struct mcc_ast_statement *mcc_ast_new_statement_expression(struct mcc_ast_expression *expression)
{
	assert(expression);

	struct mcc_ast_statement *statement = malloc(sizeof(*statement));
	if (!statement) {
		return NULL;
	}

	statement->type = MCC_AST_STATEMENT_TYPE_EXPRESSION;
	statement->expression = expression;
	statement->next_statement = NULL;

	return statement;
}

struct mcc_ast_statement *mcc_ast_new_statement_return(struct mcc_ast_expression *return_expression)
{

	struct mcc_ast_statement *statement = malloc(sizeof(*statement));
	if (!statement) {
		return NULL;
	}

	statement->type = MCC_AST_STATEMENT_TYPE_RETURN;
	statement->return_expression = return_expression;
	statement->next_statement = NULL;

	return statement;
}

struct mcc_ast_statement *mcc_ast_new_statement_compound(struct mcc_ast_statement *compound_statement)
{
	struct mcc_ast_statement *statement = malloc(sizeof(*statement));
	if (!statement) {
		return NULL;
	}

	statement->type = MCC_AST_STATEMENT_TYPE_COMPOUND;
	statement->compound_statement = compound_statement;
	statement->next_statement = NULL;

	return statement;
}

struct mcc_ast_statement *mcc_ast_new_statement_linked_compound(struct mcc_ast_statement *first_statement,
                                                                struct mcc_ast_statement *next_statement)
{

	assert(first_statement);
	assert(next_statement);

	struct mcc_ast_statement *last_statement = first_statement->compound_statement;

	while (last_statement->next_statement != NULL) {
		last_statement = last_statement->next_statement;
	}
	last_statement->next_statement = next_statement;

	return first_statement;
}

void mcc_ast_delete_statement(struct mcc_ast_statement *statement)
{
	assert(statement);

	switch (statement->type) {

	case MCC_AST_STATEMENT_TYPE_IF:
		assert(statement->if_expression);
		assert(statement->if_statement);

		mcc_ast_delete_expression(statement->if_expression);
		mcc_ast_delete_statement(statement->if_statement);

		if (statement->else_statement != NULL) {
			mcc_ast_delete_statement(statement->else_statement);
		}
		break;

	case MCC_AST_STATEMENT_TYPE_WHILE:
		assert(statement->while_expression);
		assert(statement->while_statement);

		mcc_ast_delete_expression(statement->while_expression);
		mcc_ast_delete_statement(statement->while_statement);
		break;

	case MCC_AST_STATEMENT_TYPE_DECLARATION:
		assert(statement->declaration);
		mcc_ast_delete_declaration(statement->declaration);
		break;

	case MCC_AST_STATEMENT_TYPE_ASSIGNMENT:
		assert(statement->assignment);
		mcc_ast_delete_assignment(statement->assignment);
		break;

	case MCC_AST_STATEMENT_TYPE_EXPRESSION:
		assert(statement->expression);
		mcc_ast_delete_expression(statement->expression);
		break;

	case MCC_AST_STATEMENT_TYPE_RETURN:
		if (statement->return_expression != NULL) {
			mcc_ast_delete_expression(statement->return_expression);
		}
		break;

	case MCC_AST_STATEMENT_TYPE_COMPOUND:
		if (statement->compound_statement != NULL) {
			mcc_ast_delete_statement(statement->compound_statement);
		}
	}

	if (statement->next_statement != NULL) {
		mcc_ast_delete_statement(statement->next_statement);
	}

	free(statement);
}

// ############################################## Functions ##############################################

struct mcc_ast_function_definition *mcc_ast_new_function_definition(enum mcc_ast_type return_type,
                                                                    char *identifier,
                                                                    struct mcc_ast_declaration *parameters,
                                                                    struct mcc_ast_statement *statements)
{

	assert(identifier);
	assert(statements);

	struct mcc_ast_function_definition *function_definition = malloc(sizeof(*function_definition));
	if (!function_definition) {
		return NULL;
	}

	function_definition->identifier = identifier;
	function_definition->return_type = return_type;
	function_definition->parameters = parameters;
	function_definition->statements = statements;
	function_definition->next_definition = NULL;

	return function_definition;
}

struct mcc_ast_function_definition *
mcc_ast_new_function_def_linked_function_def(struct mcc_ast_function_definition *first_definition,
                                             struct mcc_ast_function_definition *next_definition)
{

	assert(first_definition);
	assert(next_definition);

	struct mcc_ast_function_definition *last_definition = first_definition;

	while (last_definition->next_definition != NULL) {
		last_definition = last_definition->next_definition;
	}
	last_definition->next_definition = next_definition;

	return first_definition;
}

void mcc_ast_delete_function_definition(struct mcc_ast_function_definition *function_definition)
{

	assert(function_definition);

	free(function_definition->identifier);

	if (function_definition->parameters != NULL) {
		mcc_ast_delete_declaration(function_definition->parameters);
	}
	if (function_definition->statements != NULL) {
		mcc_ast_delete_statement(function_definition->statements);
	}
	if (function_definition->next_definition != NULL) {
		mcc_ast_delete_function_definition(function_definition->next_definition);
	}

	free(function_definition);
}

struct mcc_ast_function_call *mcc_ast_new_function_call(char *identifier, struct mcc_ast_expression *arguments)
{
	assert(identifier);
	struct mcc_ast_function_call *function_call = malloc(sizeof(*function_call));
	if (!function_call) {
		return NULL;
	}

	function_call->arguments = arguments;
	function_call->identifier = identifier;

	return function_call;
}
void mcc_ast_delete_function_call(struct mcc_ast_function_call *function_call)
{
	assert(function_call);
	free(function_call->identifier);
	if (function_call->arguments != NULL) {
		mcc_ast_delete_expression(function_call->arguments);
	}
	free(function_call);
}
