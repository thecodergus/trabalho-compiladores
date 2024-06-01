#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "mcc/ast_print.h"
#include "mcc/ast_visit.h"

const char *mcc_ast_print_binary_op(enum mcc_ast_binary_op op)
{
	switch (op) {
	case MCC_AST_BINARY_OP_ADD:
		return "+";
	case MCC_AST_BINARY_OP_SUB:
		return "-";
	case MCC_AST_BINARY_OP_MUL:
		return "*";
	case MCC_AST_BINARY_OP_DIV:
		return "/";
	case MCC_AST_BINARY_OP_LESS:
		return "<";
	case MCC_AST_BINARY_OP_LESS_EQ:
		return "<=";
	case MCC_AST_BINARY_OP_GREATER:
		return ">";
	case MCC_AST_BINARY_OP_GREATER_EQ:
		return ">=";
	case MCC_AST_BINARY_OP_EQ:
		return "==";
	case MCC_AST_BINARY_OP_NOT_EQ:
		return "!=";
	case MCC_AST_BINARY_OP_AND:
		return "&&";
	case MCC_AST_BINARY_OP_OR:
		return "||";
	}

	return "unknown op";
}

const char *mcc_ast_print_unary_op(enum mcc_ast_unary_op op)
{
	switch (op) {
	case MCC_AST_UNARY_OP_MINUS:
		return "-";
	case MCC_AST_UNARY_OP_NOT:
		return "!";
	}

	return "unknown op";
}

static char *print_dot_type(enum mcc_ast_type type)
{
	switch (type) {
	case MCC_AST_TYPE_STRING:
		return "string";
	case MCC_AST_TYPE_BOOL:
		return "bool";
	case MCC_AST_TYPE_INT:
		return "int";
	case MCC_AST_TYPE_FLOAT:
		return "float";
	case MCC_AST_TYPE_VOID:
		return "void";
	}

	return "type unknown";
}

void *get_expression_edge(struct mcc_ast_expression *expression)
{
	assert(expression);

	if (expression->type == MCC_AST_EXPRESSION_TYPE_LITERAL)
		return expression->literal;
	else
		return expression;
}

void *get_statement_edge(struct mcc_ast_statement *statement)
{
	assert(statement);

	switch (statement->type) {
	case MCC_AST_STATEMENT_TYPE_IF:
		return statement;
	case MCC_AST_STATEMENT_TYPE_WHILE:
		return statement;
	case MCC_AST_STATEMENT_TYPE_RETURN:
		return statement;
	case MCC_AST_STATEMENT_TYPE_DECLARATION:
		return statement->declaration;
	case MCC_AST_STATEMENT_TYPE_ASSIGNMENT:
		return statement->declaration;
	case MCC_AST_STATEMENT_TYPE_EXPRESSION:
		return statement->expression;
	default:
		return statement;
	}
}

// ---------------------------------------------------------------- DOT Printer

#define LABEL_SIZE 64

static void print_dot_begin(FILE *out)
{
	assert(out);

	fprintf(out, "digraph \"AST\" {\n"
	             "\tnodesep=0.6\n");
}

static void print_dot_end(FILE *out)
{
	assert(out);

	fprintf(out, "}\n");
}

static void print_dot_node(FILE *out, const void *node, const char *label)
{
	assert(out);
	assert(node);
	assert(label);

	fprintf(out, "\t\"%p\" [shape=box, label=\"%s\"];\n", node, label);
}

static void print_dot_edge(FILE *out, const void *src_node, const void *dst_node, const char *label)
{
	assert(out);
	assert(src_node);
	assert(dst_node);
	assert(label);

	fprintf(out, "\t\"%p\" -> \"%p\" [label=\"%s\"];\n", src_node, dst_node, label);
}

static void print_dot_expression_literal(struct mcc_ast_expression *expression, void *data)
{
	assert(expression);
	assert(data);

	FILE *out = data;
	print_dot_node(out, expression, "expr: lit");
	print_dot_edge(out, expression, expression->literal, "literal");
}

static void print_dot_expression_binary_op(struct mcc_ast_expression *expression, void *data)
{
	assert(expression);
	assert(data);

	char label[LABEL_SIZE] = {0};
	snprintf(label, sizeof(label), "expr: %s", mcc_ast_print_binary_op(expression->op));

	FILE *out = data;
	print_dot_node(out, expression, label);
	print_dot_edge(out, expression, get_expression_edge(expression->lhs), "lhs");
	print_dot_edge(out, expression, get_expression_edge(expression->rhs), "rhs");
}

static void print_dot_expression_unary_op(struct mcc_ast_expression *expression, void *data)
{
	assert(expression);
	assert(data);

	char label[LABEL_SIZE] = {0};
	snprintf(label, sizeof(label), "%s", mcc_ast_print_unary_op(expression->unary_op));

	FILE *out = data;
	print_dot_node(out, expression, label);
	print_dot_edge(out, expression, get_expression_edge(expression->unary_expression), "expression");
}

static void print_dot_expression_function_call(struct mcc_ast_expression *expression, void *data)
{

	assert(expression);
	assert(data);

	char label[LABEL_SIZE] = {0};
	snprintf(label, sizeof(label), "%s()", expression->function_call->identifier);

	FILE *out = data;
	print_dot_node(out, expression, label);

	struct mcc_ast_expression *arguments = expression->function_call->arguments;

	while (arguments != NULL) {
		print_dot_edge(out, expression, get_expression_edge(arguments), "arg");
		arguments = arguments->next_expression;
	}
}

static void print_dot_expression_parenth(struct mcc_ast_expression *expression, void *data)
{
	assert(expression);
	assert(data);

	FILE *out = data;
	print_dot_node(out, expression, "( )");
	print_dot_edge(out, expression, get_expression_edge(expression->expression), "expression");
}

static void print_dot_expression_identifier(struct mcc_ast_expression *expression, void *data)
{
	assert(expression);
	assert(data);

	char label[LABEL_SIZE] = {0};
	snprintf(label, sizeof(label), "%s", expression->identifier);

	FILE *out = data;
	print_dot_node(out, expression, label);
	// array
	if (expression->index != NULL) {
		print_dot_edge(out, expression, expression->index, "index");
	}
}

static void print_dot_literal_int(struct mcc_ast_literal *literal, void *data)
{
	assert(literal);
	assert(data);

	char label[LABEL_SIZE] = {0};
	snprintf(label, sizeof(label), "%ld", literal->i_value);

	FILE *out = data;
	print_dot_node(out, literal, label);
}

static void print_dot_literal_float(struct mcc_ast_literal *literal, void *data)
{
	assert(literal);
	assert(data);

	char label[LABEL_SIZE] = {0};
	snprintf(label, sizeof(label), "%f", literal->f_value);

	FILE *out = data;
	print_dot_node(out, literal, label);
}

static void print_dot_literal_bool(struct mcc_ast_literal *literal, void *data)
{
	assert(literal);
	assert(data);

	char label[LABEL_SIZE] = {0};
	snprintf(label, sizeof(label), "%s", literal->b_value);

	FILE *out = data;
	print_dot_node(out, literal, label);
}

static void print_dot_literal_string(struct mcc_ast_literal *literal, void *data)
{
	assert(literal);
	assert(data);

	char label[LABEL_SIZE] = {0};
	snprintf(label, sizeof(label), "%s", literal->s_value);

	FILE *out = data;
	print_dot_node(out, literal, label);
}

static void print_dot_declaration_type(struct mcc_ast_declaration *declaration, void *data)
{
	assert(declaration);
	assert(data);

	FILE *out = data;

	char label[LABEL_SIZE] = {0};
	snprintf(label, sizeof(label), "declaration");

	char label1[LABEL_SIZE] = {0};
	snprintf(label1, sizeof(label1), "%s", print_dot_type(declaration->type));

	print_dot_node(out, declaration, label);
	print_dot_edge(out, declaration, label1, "type");
	print_dot_edge(out, declaration, declaration->identifier, "identifier");
	if (declaration->literal != NULL)
		print_dot_edge(out, declaration, declaration->literal, "size");

	print_dot_node(out, label1, label1);
}

static void print_dot_declaration_literal(struct mcc_ast_declaration *declaration, void *data)
{
	assert(declaration);
	assert(data);

	char label[LABEL_SIZE] = {0};
	snprintf(label, sizeof(label), "[%ld]", declaration->literal->i_value);

	FILE *out = data;
	print_dot_node(out, declaration->literal, label);
}

static void print_dot_declaration_identifier(struct mcc_ast_declaration *declaration, void *data)
{
	assert(declaration);
	assert(data);

	char label[LABEL_SIZE] = {0};
	snprintf(label, sizeof(label), "%s", declaration->identifier);

	FILE *out = data;
	print_dot_node(out, declaration->identifier, label);
}

static void print_dot_assignment(struct mcc_ast_assignment *assignment, void *data)
{
	assert(assignment);
	assert(data);

	char label[LABEL_SIZE] = {0};
	snprintf(label, sizeof(label), "assignment");

	FILE *out = data;
	print_dot_node(out, assignment, label);
	print_dot_edge(out, assignment, assignment->identifier, "identifier");

	if (assignment->index != NULL) {
		print_dot_edge(out, assignment, get_expression_edge(assignment->index), "index");
	}

	print_dot_edge(out, assignment, get_expression_edge(assignment->value), "value");
}

static void print_dot_assignment_identifier(struct mcc_ast_assignment *assignment, void *data)
{
	assert(assignment);
	assert(data);

	char label[LABEL_SIZE] = {0};
	snprintf(label, sizeof(label), "%s", assignment->identifier);

	FILE *out = data;
	print_dot_node(out, assignment->identifier, label);
}

static void print_dot_statement_if(struct mcc_ast_statement *statement, void *data)
{
	assert(statement);
	assert(data);

	char label[LABEL_SIZE] = {0};
	snprintf(label, sizeof(label), "if");

	FILE *out = data;
	print_dot_node(out, statement, label);
	print_dot_edge(out, statement, get_expression_edge(statement->if_expression), "if_condition");
	print_dot_edge(out, statement, get_statement_edge(statement->if_statement), "if_block");
	if (statement->else_statement != NULL) {
		print_dot_edge(out, statement, get_statement_edge(statement->else_statement), "else_block");
	}
}

static void print_dot_statement_while(struct mcc_ast_statement *statement, void *data)
{
	assert(statement);
	assert(data);

	char label[LABEL_SIZE] = {0};
	snprintf(label, sizeof(label), "while");

	FILE *out = data;
	print_dot_node(out, statement, label);
	print_dot_edge(out, statement, statement->while_expression, "condition");
	print_dot_edge(out, statement, statement->while_statement, "loop");
}

static void print_dot_statement_return(struct mcc_ast_statement *statement, void *data)
{
	assert(statement);
	assert(data);

	char label[LABEL_SIZE] = {0};
	snprintf(label, sizeof(label), "return");

	FILE *out = data;
	print_dot_node(out, statement, label);

	if (statement->return_expression != NULL) {
		print_dot_edge(out, statement, get_expression_edge(statement->return_expression), "expr");
	}
}

static void print_dot_statement_compound(struct mcc_ast_statement *statement, void *data)
{
	assert(statement);
	assert(data);

	char label[LABEL_SIZE] = {0};
	snprintf(label, sizeof(label), "{}");

	FILE *out = data;
	print_dot_node(out, statement, label);

	struct mcc_ast_statement *next_statement = statement->compound_statement;
	int statement_count = 0;
	char statement_label[LABEL_SIZE] = {0};

	while (next_statement != NULL) {
		snprintf(statement_label, sizeof(statement_label), "statement %d", statement_count);
		print_dot_edge(out, statement, get_statement_edge(next_statement), statement_label);
		next_statement = next_statement->next_statement;
		statement_count++;
	}
}

static void print_dot_function_definition(struct mcc_ast_function_definition *function_definition, void *data)
{
	assert(function_definition);
	assert(data);

	struct mcc_ast_declaration *parameter = function_definition->parameters;
	char param_str[LABEL_SIZE * 8] = {0};
	while (parameter != NULL) {
		strcat(param_str, print_dot_type(parameter->type));
		strcat(param_str, " ");
		strcat(param_str, parameter->identifier);
		if (parameter->next_parameter != NULL) {
			strcat(param_str, ", ");
		}
		parameter = parameter->next_parameter;
	}

	char *type = print_dot_type(function_definition->return_type);
	char *identifier = function_definition->identifier;

	char label[LABEL_SIZE * 32] = {0};
	snprintf(label, sizeof(label), "%s %s(%s)", type, identifier, param_str);

	FILE *out = data;
	print_dot_node(out, function_definition, label);
	print_dot_edge(out, function_definition, get_statement_edge(function_definition->statements), "fun_body");
}

// Setup an AST Visitor for printing.
static struct mcc_ast_visitor print_dot_visitor(FILE *out)
{
	assert(out);

	return (struct mcc_ast_visitor){
	    .traversal = MCC_AST_VISIT_DEPTH_FIRST,
	    .order = MCC_AST_VISIT_PRE_ORDER,

	    .userdata = out,

	    .expression_literal = print_dot_expression_literal,
	    .expression_binary_op = print_dot_expression_binary_op,
	    .expression_unary_op = print_dot_expression_unary_op,
	    .expression_parenth = print_dot_expression_parenth,
	    .expression_function_call = print_dot_expression_function_call,
	    .expression_identifier = print_dot_expression_identifier,

	    .literal_int = print_dot_literal_int,
	    .literal_float = print_dot_literal_float,
	    .literal_bool = print_dot_literal_bool,
	    .literal_string = print_dot_literal_string,

	    .declaration_type = print_dot_declaration_type,
	    .declaration_identifier = print_dot_declaration_identifier,
	    .declaration_literal = print_dot_declaration_literal,

	    .assignment = print_dot_assignment,
	    .assignment_identifier = print_dot_assignment_identifier,

	    .statement_if = print_dot_statement_if,
	    .statement_while = print_dot_statement_while,
	    .statement_return = print_dot_statement_return,
	    .statement_compound = print_dot_statement_compound,

	    .function_definition = print_dot_function_definition,
	};
}

void mcc_ast_print_dot_expression(FILE *out, struct mcc_ast_expression *expression)
{
	assert(out);
	assert(expression);

	print_dot_begin(out);

	struct mcc_ast_visitor visitor = print_dot_visitor(out);
	mcc_ast_visit_expression(expression, &visitor);

	print_dot_end(out);
}

void mcc_ast_print_dot_declaration(FILE *out, struct mcc_ast_declaration *declaration)
{
	assert(out);
	assert(declaration);

	print_dot_begin(out);

	struct mcc_ast_visitor visitor = print_dot_visitor(out);
	mcc_ast_visit_declaration(declaration, &visitor);

	print_dot_end(out);
}

void mcc_ast_print_dot_assignment(FILE *out, struct mcc_ast_assignment *assignment)
{
	assert(out);
	assert(assignment);

	print_dot_begin(out);

	struct mcc_ast_visitor visitor = print_dot_visitor(out);
	mcc_ast_visit_assignment(assignment, &visitor);

	print_dot_end(out);
}

void mcc_ast_print_dot_statement(FILE *out, struct mcc_ast_statement *statement)
{
	assert(out);
	assert(statement);
	print_dot_begin(out);

	struct mcc_ast_visitor visitor = print_dot_visitor(out);
	mcc_ast_visit_statement(statement, &visitor, false);

	print_dot_end(out);
}

void mcc_ast_print_dot_literal(FILE *out, struct mcc_ast_literal *literal)
{
	assert(out);
	assert(literal);

	print_dot_begin(out);

	struct mcc_ast_visitor visitor = print_dot_visitor(out);
	mcc_ast_visit_literal(literal, &visitor);

	print_dot_end(out);
}

void mcc_ast_print_dot_program(FILE *out, struct mcc_ast_function_definition *function_definition)
{
	assert(out);
	assert(function_definition);

	print_dot_begin(out);

	struct mcc_ast_visitor visitor = print_dot_visitor(out);
	mcc_ast_visit_function_definition(function_definition, &visitor);

	print_dot_end(out);
}
