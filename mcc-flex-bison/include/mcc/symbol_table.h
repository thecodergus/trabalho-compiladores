#ifndef MCC_SYMBOL_TABLE_H
#define MCC_SYMBOL_TABLE_H

#include "mcc/ast.h"

struct mcc_symbol_table_userdata {
	struct mcc_symbol_table *symbol_table;
};

struct mcc_symbol_table {
	struct mcc_symbol_table_entry *first_entry;
	struct mcc_symbol_table_entry *last_entry;
	struct mcc_symbol_table *parent_symbol_table;
	struct mcc_symbol_table *first_child;
	struct mcc_symbol_table *next;
};

enum mcc_symbol_table_type { MCC_SYMBOL_TABLE_ENTRY_TYPE_VARIABLE, MCC_SYMBOL_TABLE_ENTRY_TYPE_FUNCTION };

struct mcc_symbol_table_entry {

	struct mcc_ast_node node;
	struct mcc_symbol_table *symbol_table;
	char *name;
	enum mcc_ast_type data_type;
	enum mcc_symbol_table_type symbol_type;

	union {
		// MCC_SYMBOL_TABLE_ENTRY_TYPE_FUNCTION
		struct mcc_ast_declaration *parameters;
		// MCC_SYMBOL_TABLE_ENTRY_TYPE_VARIABLE
		struct mcc_ast_literal *array_index;
	};

	struct mcc_symbol_table_entry *next;

	char *tac_name;
};

struct mcc_symbol_table *mcc_symbol_table_new_table(struct mcc_symbol_table *parent);

struct mcc_symbol_table *mcc_symbol_table_create(struct mcc_ast_function_definition *function_definition);

void mcc_symbol_table_delete_table_entry(struct mcc_symbol_table_entry **ptr);

void mcc_symbol_table_delete_table(struct mcc_symbol_table **ptr);

struct mcc_symbol_table_entry *mcc_symbol_table_lookup_variable_entry(struct mcc_symbol_table *symbol_table,
                                                                      char *identifier,
                                                                      struct mcc_ast_node usage_node);

struct mcc_symbol_table_entry *mcc_symbol_table_lookup_function_entry(struct mcc_symbol_table *symbol_table,
                                                                      char *identifier);

int check_for_builtin(char *entry_name);
#endif
