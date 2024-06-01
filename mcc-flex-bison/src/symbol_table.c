#include "mcc/symbol_table.h"
#include "mcc/ast_visit.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void add_declaration(struct mcc_ast_declaration *declaration, void *data)
{
	assert(declaration);
	assert(data);

	struct mcc_symbol_table_userdata *userdata = data;
	struct mcc_symbol_table *symbol_table = userdata->symbol_table;
	assert(symbol_table);

	declaration->symbol_table = symbol_table;

	struct mcc_symbol_table_entry *entry = malloc(sizeof(*entry));
	if (!entry) {
		return;
	}

	entry->node = declaration->node;
	entry->symbol_type = MCC_SYMBOL_TABLE_ENTRY_TYPE_VARIABLE;
	entry->name = declaration->identifier;
	entry->data_type = declaration->type;
	entry->array_index = declaration->literal;
	entry->next = NULL;
	entry->symbol_table = symbol_table;
	entry->tac_name = NULL;

	// if symbol_table is empty
	if (symbol_table->last_entry == NULL) {
		symbol_table->first_entry = entry;
		symbol_table->last_entry = entry;
	} else {
		struct mcc_symbol_table_entry *last = symbol_table->last_entry;
		last->next = entry;
		symbol_table->last_entry = entry;
	}
}

static void add_function_definition(struct mcc_ast_function_definition *function, void *data)
{
	assert(function);
	assert(data);

	struct mcc_symbol_table_userdata *userdata = data;
	struct mcc_symbol_table *symbol_table = userdata->symbol_table;
	assert(symbol_table);

	struct mcc_symbol_table_entry *entry = malloc(sizeof(*entry));
	if (!entry) {
		return;
	}

	entry->node = function->node;
	entry->symbol_type = MCC_SYMBOL_TABLE_ENTRY_TYPE_FUNCTION;
	entry->name = function->identifier;
	entry->data_type = function->return_type;
	entry->parameters = function->parameters;
	entry->next = NULL;
	entry->symbol_table = symbol_table;
	entry->tac_name = NULL;

	// if symbol_table is empty
	if (symbol_table->last_entry == NULL) {
		symbol_table->first_entry = entry;
		symbol_table->last_entry = entry;
	} else {
		struct mcc_symbol_table_entry *last = symbol_table->last_entry;
		last->next = entry;
		symbol_table->last_entry = entry;
	}

	// create child symbol table for function stack
	struct mcc_symbol_table *child = mcc_symbol_table_new_table(symbol_table);

	// fill parameters into child symbol table
	if (function->parameters != NULL) {
		struct mcc_ast_declaration *declaration = function->parameters;

		while (declaration != NULL) {
			add_declaration(declaration, (void *)&child);
			declaration = declaration->next_parameter;
		}
	}

	userdata->symbol_table = child;
}

static void enter_while_scope(void *statement, void *data)
{
	assert(statement);
	assert(data);

	struct mcc_ast_statement *stat = statement;

	struct mcc_symbol_table_userdata *userdata = data;
	struct mcc_symbol_table *symbol_table = userdata->symbol_table;

	if (stat->type == MCC_AST_STATEMENT_TYPE_COMPOUND) {
		// compound is created seperately
		return;
	} else {
		assert(symbol_table);
		userdata->symbol_table = mcc_symbol_table_new_table(symbol_table);
	}
}

static void enter_if_scope(void *statement, void *data)
{
	assert(statement);
	assert(data);

	struct mcc_ast_statement *stat = statement;

	struct mcc_symbol_table_userdata *userdata = data;
	struct mcc_symbol_table *symbol_table = userdata->symbol_table;

	if (stat->type == MCC_AST_STATEMENT_TYPE_COMPOUND) {
		// compound is created seperately
		return;
	} else {
		assert(symbol_table);
		userdata->symbol_table = mcc_symbol_table_new_table(symbol_table);
	}
}

static void enter_compound_scope(void *statement, void *data)
{
	assert(statement);
	assert(data);

	struct mcc_symbol_table_userdata *userdata = data;
	struct mcc_symbol_table *symbol_table = userdata->symbol_table;

	assert(symbol_table);
	userdata->symbol_table = mcc_symbol_table_new_table(symbol_table);
}

static void leave_while_scope(void *statement, void *data)
{
	assert(statement);
	assert(data);

	struct mcc_ast_statement *stat = statement;

	struct mcc_symbol_table_userdata *userdata = data;
	struct mcc_symbol_table *symbol_table = userdata->symbol_table;

	if (stat->type == MCC_AST_STATEMENT_TYPE_COMPOUND) {
		// compound is left seperately
		return;
	} else {
		assert(symbol_table);
		userdata->symbol_table = symbol_table->parent_symbol_table;
	}
}

static void leave_if_scope(void *statement, void *data)
{
	assert(statement);
	assert(data);

	struct mcc_ast_statement *stat = statement;

	struct mcc_symbol_table_userdata *userdata = data;
	struct mcc_symbol_table *symbol_table = userdata->symbol_table;

	if (stat->type == MCC_AST_STATEMENT_TYPE_COMPOUND) {
		// compound is left seperately
		return;
	} else {
		assert(symbol_table);
		userdata->symbol_table = symbol_table->parent_symbol_table;
	}
}

static void leave_compound_scope(void *statement, void *data)
{
	assert(statement);
	assert(data);

	struct mcc_symbol_table_userdata *userdata = data;
	struct mcc_symbol_table *symbol_table = userdata->symbol_table;

	assert(symbol_table);
	userdata->symbol_table = symbol_table->parent_symbol_table;
}

static void mcc_add_expression(struct mcc_ast_expression *expression, void *data)
{
	assert(expression);
	assert(data);

	struct mcc_symbol_table_userdata *userdata = data;
	expression->symbol_table = userdata->symbol_table;
}

static void mcc_add_statement(struct mcc_ast_statement *statement, void *data)
{
	assert(statement);
	assert(data);

	struct mcc_symbol_table_userdata *userdata = data;
	statement->symbol_table = userdata->symbol_table;
}

static void mcc_add_assignment(struct mcc_ast_assignment *assignment, void *data)
{
	assert(assignment);
	assert(data);

	struct mcc_symbol_table_userdata *userdata = data;
	assignment->symbol_table = userdata->symbol_table;
}

static bool check_if_declaration_before_usage(struct mcc_ast_node declaration_node, struct mcc_ast_node usage_node)
{
	if (declaration_node.sloc.start_line < usage_node.sloc.start_line)
		return true;
	if (declaration_node.sloc.start_line > usage_node.sloc.start_line)
		return false;
	if (declaration_node.sloc.start_col <= usage_node.sloc.start_col)
		return true;

	return false;
}

struct mcc_symbol_table_entry *
lookup_entry(struct mcc_symbol_table *symbol_table, char *identifier, enum mcc_symbol_table_type entry_type)
{
	assert(symbol_table);
	assert(identifier);

	struct mcc_symbol_table_entry *current_entry = symbol_table->first_entry;

	while (current_entry) {

		if ((current_entry->symbol_type == entry_type) && (strcmp(current_entry->name, identifier) == 0)) {

			return current_entry;
		}
		current_entry = current_entry->next;
	}

	return NULL;
}

static void add_built_in_functions(struct mcc_symbol_table *symbol_table)
{

	/**
	 *
	 * print functions
	 *
	 **/
	//==========================================================================================

	/**
	 * string
	 **/
	struct mcc_symbol_table_entry *print = malloc(sizeof(*print));
	if (!print) {
		return;
	}
	print->node = (struct mcc_ast_node){{0, 0, 0, 0}};
	print->name = "print";
	print->data_type = MCC_AST_TYPE_VOID;
	print->symbol_type = MCC_SYMBOL_TABLE_ENTRY_TYPE_FUNCTION;
	print->next = NULL;
	print->symbol_table = symbol_table;
	print->tac_name = NULL;

	struct mcc_ast_declaration *string_param = malloc(sizeof(*string_param));
	if (!string_param) {
		return;
	}
	string_param->identifier = "parameter";
	string_param->type = MCC_AST_TYPE_STRING;
	string_param->literal = NULL;
	string_param->next_parameter = NULL;
	string_param->symbol_table = NULL;

	print->parameters = string_param;

	symbol_table->first_entry = print;

	/**
	 * new line
	 **/
	struct mcc_symbol_table_entry *print_nl = malloc(sizeof(*print_nl));
	if (!print_nl) {
		return;
	}
	print_nl->node = (struct mcc_ast_node){{0, 0, 0, 0}};
	print_nl->name = "print_nl";
	print_nl->data_type = MCC_AST_TYPE_VOID;
	print_nl->symbol_type = MCC_SYMBOL_TABLE_ENTRY_TYPE_FUNCTION;
	print_nl->symbol_table = symbol_table;
	print_nl->parameters = NULL;
	print_nl->next = NULL;
	print_nl->tac_name = NULL;
	print->next = print_nl;

	/**
	 * int
	 **/
	struct mcc_symbol_table_entry *print_int = malloc(sizeof(*print_int));
	if (!print_int) {
		return;
	}
	print_int->node = (struct mcc_ast_node){{0, 0, 0, 0}};
	print_int->name = "print_int";
	print_int->data_type = MCC_AST_TYPE_VOID;
	print_int->symbol_type = MCC_SYMBOL_TABLE_ENTRY_TYPE_FUNCTION;
	print_int->next = NULL;
	print_int->tac_name = NULL;

	struct mcc_ast_declaration *int_param = malloc(sizeof(*int_param));
	if (!int_param) {
		return;
	}
	int_param->identifier = "parameter";
	int_param->type = MCC_AST_TYPE_INT;
	int_param->literal = NULL;
	int_param->next_parameter = NULL;
	int_param->symbol_table = NULL;

	print_int->parameters = int_param;
	print_int->symbol_table = symbol_table;
	print_nl->next = print_int;

	/**
	 * float
	 **/

	struct mcc_symbol_table_entry *print_float = malloc(sizeof(*print_float));
	if (!print_float) {
		return;
	}
	print_float->node = (struct mcc_ast_node){{0, 0, 0, 0}};
	print_float->name = "print_float";
	print_float->data_type = MCC_AST_TYPE_VOID;
	print_float->symbol_type = MCC_SYMBOL_TABLE_ENTRY_TYPE_FUNCTION;
	print_float->next = NULL;
	print_float->tac_name = NULL;

	struct mcc_ast_declaration *float_param = malloc(sizeof(*float_param));
	if (!float_param) {
		return;
	}
	float_param->identifier = "parameter";
	float_param->type = MCC_AST_TYPE_FLOAT;
	float_param->literal = NULL;
	float_param->next_parameter = NULL;
	float_param->symbol_table = NULL;

	print_float->parameters = float_param;
	print_float->symbol_table = symbol_table;
	print_int->next = print_float;

	/**
	 *
	 * read functions
	 *
	 **/
	//==========================================================================================

	/**
	 * int
	 **/
	struct mcc_symbol_table_entry *read_int = malloc(sizeof(*read_int));
	if (!read_int) {
		return;
	}
	read_int->node = (struct mcc_ast_node){{0, 0, 0, 0}};
	read_int->name = "read_int";
	read_int->data_type = MCC_AST_TYPE_INT;
	read_int->symbol_type = MCC_SYMBOL_TABLE_ENTRY_TYPE_FUNCTION;
	read_int->symbol_table = symbol_table;
	read_int->parameters = NULL;
	read_int->tac_name = NULL;
	print_float->next = read_int;

	/**
	 * float
	 **/
	struct mcc_symbol_table_entry *read_float = malloc(sizeof(*read_float));
	if (!read_float) {
		return;
	}
	read_float->node = (struct mcc_ast_node){{0, 0, 0, 0}};
	read_float->name = "read_float";
	read_float->data_type = MCC_AST_TYPE_FLOAT;
	read_float->symbol_type = MCC_SYMBOL_TABLE_ENTRY_TYPE_FUNCTION;
	read_float->symbol_table = symbol_table;
	read_float->parameters = NULL;
	read_float->tac_name = NULL;
	read_int->next = read_float;

	symbol_table->last_entry = read_float;
}

static struct mcc_ast_visitor create_symbol_table_visitor(struct mcc_symbol_table_userdata *user_data)
{
	assert(user_data);

	return (struct mcc_ast_visitor){
	    .traversal = MCC_AST_VISIT_DEPTH_FIRST,
	    .order = MCC_AST_VISIT_PRE_ORDER,
	    .userdata = user_data,
	    .function_definition = add_function_definition,
	    .declaration_identifier = add_declaration,
	    .enter_while_scope = enter_while_scope,
	    .enter_if_scope = enter_if_scope,
	    .enter_compound_scope = enter_compound_scope,
	    .leave_while_scope = leave_while_scope,
	    .leave_if_scope = leave_if_scope,
	    .leave_compound_scope = leave_compound_scope,
	    .expression = mcc_add_expression,
	    .statement = mcc_add_statement,
	    .assignment = mcc_add_assignment,
	};
}

struct mcc_symbol_table *mcc_symbol_table_new_table(struct mcc_symbol_table *parent)
{
	struct mcc_symbol_table *symbol_table = malloc(sizeof(*symbol_table));
	if (!symbol_table) {
		return NULL;
	}

	symbol_table->first_entry = NULL;
	symbol_table->last_entry = NULL;
	symbol_table->parent_symbol_table = NULL;
	symbol_table->first_child = NULL;
	symbol_table->next = NULL;

	if (parent != NULL) {

		symbol_table->parent_symbol_table = parent;

		if (parent->first_child == NULL) {
			parent->first_child = symbol_table;
		} else {

			struct mcc_symbol_table *child = parent->first_child;
			while (child->next != NULL) {
				child = child->next;
			}
			child->next = symbol_table;
		}

	} else {
		*symbol_table = (struct mcc_symbol_table){0};
	}

	return symbol_table;
}

struct mcc_symbol_table *mcc_symbol_table_create(struct mcc_ast_function_definition *function_definition)
{
	assert(function_definition);

	// create root symbol table
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_new_table(NULL);
	assert(symbol_table);

	struct mcc_symbol_table_userdata *userdata = malloc(sizeof(*userdata));
	if (!userdata) {
		return NULL;
	}

	add_built_in_functions(symbol_table);
	userdata->symbol_table = symbol_table;

	struct mcc_ast_visitor visitor = create_symbol_table_visitor(userdata);

	mcc_ast_visit_function_definition(function_definition, &visitor);

	free(userdata);

	return symbol_table;
}

void mcc_symbol_table_delete_table_entry(struct mcc_symbol_table_entry **ptr)
{
	struct mcc_symbol_table_entry *entry = *ptr;
	if (entry == NULL) {
		return;
	}

	if (entry->next != NULL) {
		mcc_symbol_table_delete_table_entry(&(entry->next));
	}

	if (check_for_builtin(entry->name) == 1) {
		struct mcc_ast_declaration *param = entry->parameters;
		if (param != NULL) {
			free(param);
		}
	}

	if (entry->tac_name != NULL) {
		free(entry->tac_name);
	}

	free(entry);
	*ptr = NULL;
}

void mcc_symbol_table_delete_table(struct mcc_symbol_table **ptr)
{
	struct mcc_symbol_table *table = *ptr;

	if (table == NULL) {
		return;
	}

	if (table->first_entry != NULL) {
		mcc_symbol_table_delete_table_entry(&(table->first_entry));
	}
	if (table->first_child != NULL) {
		mcc_symbol_table_delete_table(&(table->first_child));
	}
	if (table->next != NULL) {
		mcc_symbol_table_delete_table(&(table->next));
	}

	free(table);

	*ptr = NULL;
}

int check_for_builtin(char *entry_name)
{
	char *builtins[] = {"print", "print_nl", "print_int", "print_float", "read_int", "read_float"};
	int built_in_functions_counter = 6;
	int i;
	for (i = 0; i < built_in_functions_counter; i++) {
		if (strcmp(builtins[i], entry_name) == 0) {
			return 1;
		}
	}

	return 0;
}

struct mcc_symbol_table_entry *mcc_symbol_table_lookup_variable_entry(struct mcc_symbol_table *symbol_table,
                                                                      char *identifier,
                                                                      struct mcc_ast_node usage_node)
{
	assert(symbol_table);
	assert(identifier);

	struct mcc_symbol_table_entry *entry =
	    lookup_entry(symbol_table, identifier, MCC_SYMBOL_TABLE_ENTRY_TYPE_VARIABLE);

	if (entry) {
		if (check_if_declaration_before_usage(entry->node, usage_node)) {
			return entry;
		}
	}

	// if not in current symbol table look in parent
	if (symbol_table->parent_symbol_table) {
		return mcc_symbol_table_lookup_variable_entry(symbol_table->parent_symbol_table, identifier,
		                                              usage_node);
	}

	return NULL;
}

struct mcc_symbol_table_entry *mcc_symbol_table_lookup_function_entry(struct mcc_symbol_table *symbol_table,
                                                                      char *identifier)
{
	assert(symbol_table);
	assert(identifier);

	while (symbol_table->parent_symbol_table != NULL) {
		symbol_table = symbol_table->parent_symbol_table;
	}

	return lookup_entry(symbol_table, identifier, MCC_SYMBOL_TABLE_ENTRY_TYPE_FUNCTION);
}
