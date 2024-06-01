#include "mcc/ast_visit.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#define visit(node, callback, visitor) \
	if (callback) { \
		(callback)(node, (visitor)->userdata); \
	}

#define visit_if(cond, node, callback, visitor) \
	if (cond) { \
		visit(node, callback, visitor); \
	}

#define visit_if_pre_order(node, callback, visitor) \
	visit_if((visitor)->order == MCC_AST_VISIT_PRE_ORDER, node, callback, visitor)

#define visit_if_post_order(node, callback, visitor) \
	visit_if((visitor)->order == MCC_AST_VISIT_POST_ORDER, node, callback, visitor)

void mcc_ast_visit_expression(struct mcc_ast_expression *expression, struct mcc_ast_visitor *visitor)
{
	assert(expression);
	assert(visitor);

	visit_if_pre_order(expression, visitor->expression, visitor);

	switch (expression->type) {
	case MCC_AST_EXPRESSION_TYPE_LITERAL:
		// visit_if_pre_order(expression, visitor->expression_literal, visitor);
		mcc_ast_visit_literal(expression->literal, visitor);
		// visit_if_post_order(expression, visitor->expression_literal, visitor);
		break;

	case MCC_AST_EXPRESSION_TYPE_BINARY_OP:
		visit_if_pre_order(expression, visitor->expression_binary_op, visitor);
		mcc_ast_visit_expression(expression->lhs, visitor);
		mcc_ast_visit_expression(expression->rhs, visitor);
		visit_if_post_order(expression, visitor->expression_binary_op, visitor);
		break;

	case MCC_AST_EXPRESSION_TYPE_UNARY_OP:
		visit_if_pre_order(expression, visitor->expression_unary_op, visitor);
		mcc_ast_visit_expression(expression->unary_expression, visitor);
		visit_if_post_order(expression, visitor->expression_unary_op, visitor);
		break;

	case MCC_AST_EXPRESSION_TYPE_IDENTIFIER:
		visit_if_pre_order(expression, visitor->expression_identifier, visitor);
		if (expression->index != NULL) {
			mcc_ast_visit_expression(expression->index, visitor);
		}
		visit_if_post_order(expression, visitor->expression_identifier, visitor);
		break;

	case MCC_AST_EXPRESSION_TYPE_PARENTH:
		visit_if_pre_order(expression, visitor->expression_parenth, visitor);
		mcc_ast_visit_expression(expression->expression, visitor);
		visit_if_post_order(expression, visitor->expression_parenth, visitor);
		break;

	case MCC_AST_EXPRESSION_TYPE_FUNCTION_CALL:

		visit_if_pre_order(expression, visitor->expression_function_call, visitor);
		struct mcc_ast_expression *arguments = expression->function_call->arguments;

		while (arguments != NULL) {
			mcc_ast_visit_expression(arguments, visitor);
			arguments = arguments->next_expression;
		}
		visit_if_post_order(expression, visitor->expression_function_call, visitor);
		break;
	}

	visit_if_post_order(expression, visitor->expression, visitor);
}

void mcc_ast_visit_literal(struct mcc_ast_literal *literal, struct mcc_ast_visitor *visitor)
{
	assert(literal);
	assert(visitor);

	visit_if_pre_order(literal, visitor->literal, visitor);

	switch (literal->type) {
	case MCC_AST_LITERAL_TYPE_INT:
		visit(literal, visitor->literal_int, visitor);
		break;

	case MCC_AST_LITERAL_TYPE_FLOAT:
		visit(literal, visitor->literal_float, visitor);
		break;

	case MCC_AST_LITERAL_TYPE_BOOL:
		visit(literal, visitor->literal_bool, visitor);
		break;

	case MCC_AST_LITERAL_TYPE_STRING:
		visit(literal, visitor->literal_string, visitor);
		break;
	}

	visit_if_post_order(literal, visitor->literal, visitor);
}

void mcc_ast_visit_declaration(struct mcc_ast_declaration *declaration, struct mcc_ast_visitor *visitor)
{
	assert(declaration);
	assert(visitor);

	visit_if_pre_order(declaration, visitor->declaration, visitor);

	visit(declaration, visitor->declaration_type, visitor);
	visit(declaration, visitor->declaration_identifier, visitor);

	if (declaration->literal != NULL) {
		visit(declaration, visitor->declaration_literal, visitor);
	}

	visit_if_post_order(declaration, visitor->declaration, visitor);
}

void mcc_ast_visit_assignment(struct mcc_ast_assignment *assignment, struct mcc_ast_visitor *visitor)
{
	assert(assignment);
	assert(visitor);

	visit_if_pre_order(assignment, visitor->assignment, visitor);
	visit(assignment, visitor->assignment_identifier, visitor);

	if (assignment->index != NULL) {
		mcc_ast_visit_expression(assignment->index, visitor);
	}
	mcc_ast_visit_expression(assignment->value, visitor);

	visit_if_post_order(assignment, visitor->assignment, visitor);
}

void mcc_ast_visit_statement(struct mcc_ast_statement *statement,
                             struct mcc_ast_visitor *visitor,
                             bool isFunctionStatement)
{
	assert(statement);
	assert(visitor);

	visit_if_pre_order(statement, visitor->statement, visitor);

	switch (statement->type) {

	case MCC_AST_STATEMENT_TYPE_IF:
		visit_if_pre_order(statement, visitor->statement_if, visitor);

		mcc_ast_visit_expression(statement->if_expression, visitor);

		visit(statement->if_statement, visitor->enter_if_scope, visitor);
		mcc_ast_visit_statement(statement->if_statement, visitor, false);
		visit(statement->if_statement, visitor->leave_if_scope, visitor);

		if (statement->else_statement != NULL) {
			visit(statement->else_statement, visitor->enter_if_scope, visitor);
			mcc_ast_visit_statement(statement->else_statement, visitor, false);
			visit(statement->else_statement, visitor->leave_if_scope, visitor);
		}

		visit_if_post_order(statement, visitor->statement_if, visitor);
		break;

	case MCC_AST_STATEMENT_TYPE_WHILE:
		visit_if_pre_order(statement, visitor->statement_while, visitor);

		mcc_ast_visit_expression(statement->while_expression, visitor);

		visit(statement->while_statement, visitor->enter_while_scope, visitor);
		mcc_ast_visit_statement(statement->while_statement, visitor, false);
		visit(statement->while_statement, visitor->leave_while_scope, visitor);

		visit_if_post_order(statement, visitor->statement_while, visitor);
		break;

	case MCC_AST_STATEMENT_TYPE_DECLARATION:
		mcc_ast_visit_declaration(statement->declaration, visitor);
		break;

	case MCC_AST_STATEMENT_TYPE_ASSIGNMENT:
		mcc_ast_visit_assignment(statement->assignment, visitor);
		break;

	case MCC_AST_STATEMENT_TYPE_EXPRESSION:
		mcc_ast_visit_expression(statement->expression, visitor);
		break;

	case MCC_AST_STATEMENT_TYPE_RETURN:
		visit_if_pre_order(statement, visitor->statement_return, visitor);

		if (statement->return_expression != NULL) {
			mcc_ast_visit_expression(statement->return_expression, visitor);
		}

		visit_if_post_order(statement, visitor->statement_return, visitor);
		break;

	case MCC_AST_STATEMENT_TYPE_COMPOUND:
		visit_if_pre_order(statement, visitor->statement_compound, visitor);

		struct mcc_ast_statement *next_statement = statement->compound_statement;

		if (!isFunctionStatement) {
			visit(statement, visitor->enter_compound_scope, visitor);
		}

		while (next_statement != NULL) {
			mcc_ast_visit_statement(next_statement, visitor, false);
			next_statement = next_statement->next_statement;
		}

		visit(statement, visitor->leave_compound_scope, visitor);

		visit_if_post_order(statement, visitor->statement_compound, visitor);
		break;
	}

	visit_if_post_order(statement, visitor->statement, visitor);
}

void mcc_ast_visit_function_definition(struct mcc_ast_function_definition *function_definition,
                                       struct mcc_ast_visitor *visitor)
{
	assert(function_definition);
	assert(visitor);

	while (function_definition != NULL) {
		visit_if_pre_order(function_definition, visitor->function_definition, visitor);
		mcc_ast_visit_statement(function_definition->statements, visitor, true);
		visit_if_post_order(function_definition, visitor->function_definition, visitor);
		function_definition = function_definition->next_definition;
	}
}
