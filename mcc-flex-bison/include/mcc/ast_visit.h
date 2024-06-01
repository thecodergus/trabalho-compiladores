// AST Visitor Infrastructure
//
// This module defines a visitor utility for traversing the AST.
//
// Instantiate the `mcc_ast_visitor` struct with the desired configuration and
// callbacks. Use this instance with the functions declared below. Each
// callback is optional, just set it to NULL.

#ifndef MCC_AST_VISIT_H
#define MCC_AST_VISIT_H

#include <stdbool.h>

#include "mcc/ast.h"

enum mcc_ast_visit_traversal {
	MCC_AST_VISIT_DEPTH_FIRST,
};

enum mcc_ast_visit_order {
	MCC_AST_VISIT_PRE_ORDER,
	MCC_AST_VISIT_POST_ORDER,
};

// Callbacks
typedef void (*mcc_ast_visit_expression_cb)(struct mcc_ast_expression *, void *userdata);
typedef void (*mcc_ast_visit_literal_cb)(struct mcc_ast_literal *, void *userdata);
typedef void (*mcc_ast_visit_declaration_cb)(struct mcc_ast_declaration *, void *userdata);
typedef void (*mcc_ast_visit_assignment_cb)(struct mcc_ast_assignment *, void *userdata);
typedef void (*mcc_ast_visit_statement_cb)(struct mcc_ast_statement *, void *userdata);
typedef void (*mcc_ast_visit_function_definition_cb)(struct mcc_ast_function_definition *, void *userdata);
typedef void (*mcc_ast_visit_userdata_cb)(void *, void *userdata);

struct mcc_ast_visitor {
	enum mcc_ast_visit_traversal traversal;
	enum mcc_ast_visit_order order;

	// This will be passed to every callback along with the corresponding AST
	// node. Use it to share data while traversing the tree.
	void *userdata;

	mcc_ast_visit_expression_cb expression;
	mcc_ast_visit_expression_cb expression_literal;
	mcc_ast_visit_expression_cb expression_binary_op;
	mcc_ast_visit_expression_cb expression_unary_op;
	mcc_ast_visit_expression_cb expression_parenth;
	mcc_ast_visit_expression_cb expression_identifier;
	mcc_ast_visit_expression_cb expression_function_call;

	mcc_ast_visit_literal_cb literal;
	mcc_ast_visit_literal_cb literal_int;
	mcc_ast_visit_literal_cb literal_float;
	mcc_ast_visit_literal_cb literal_bool;
	mcc_ast_visit_literal_cb literal_string;

	mcc_ast_visit_declaration_cb declaration;
	mcc_ast_visit_declaration_cb declaration_type;
	mcc_ast_visit_declaration_cb declaration_literal;
	mcc_ast_visit_declaration_cb declaration_identifier;

	mcc_ast_visit_assignment_cb assignment;
	mcc_ast_visit_assignment_cb assignment_identifier;

	mcc_ast_visit_statement_cb statement;
	mcc_ast_visit_statement_cb statement_if;
	mcc_ast_visit_statement_cb statement_while;
	mcc_ast_visit_statement_cb statement_return;
	mcc_ast_visit_statement_cb statement_compound;

	mcc_ast_visit_function_definition_cb function_definition;

	// used for symbol table creation
	mcc_ast_visit_userdata_cb enter_while_scope;
	mcc_ast_visit_userdata_cb enter_if_scope;
	mcc_ast_visit_userdata_cb enter_compound_scope;
	mcc_ast_visit_userdata_cb leave_while_scope;
	mcc_ast_visit_userdata_cb leave_if_scope;
	mcc_ast_visit_userdata_cb leave_compound_scope;
};

void mcc_ast_visit_expression(struct mcc_ast_expression *expression, struct mcc_ast_visitor *visitor);

void mcc_ast_visit_literal(struct mcc_ast_literal *literal, struct mcc_ast_visitor *visitor);

void mcc_ast_visit_declaration(struct mcc_ast_declaration *declaration, struct mcc_ast_visitor *visitor);

void mcc_ast_visit_assignment(struct mcc_ast_assignment *assignment, struct mcc_ast_visitor *visitor);

void mcc_ast_visit_statement(struct mcc_ast_statement *statement,
                             struct mcc_ast_visitor *visitor,
                             bool isFunctionStatement);

void mcc_ast_visit_function_definition(struct mcc_ast_function_definition *function_definition,
                                       struct mcc_ast_visitor *visitor);

#endif // MCC_AST_VISIT_H
