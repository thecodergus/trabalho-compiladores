#include <assert.h>
#include <string.h>

#include "mcc/symbol_table_print.h"

char *get_data_type(enum mcc_ast_type type)
{
	switch (type) {
	case MCC_AST_TYPE_VOID:
		return "void";
	case MCC_AST_TYPE_INT:
		return "int";
	case MCC_AST_TYPE_FLOAT:
		return "float";
	case MCC_AST_TYPE_BOOL:
		return "bool";
	case MCC_AST_TYPE_STRING:
		return "string";
	default:
		return "undefined type";
	}
}

char *get_symbol_table_type(enum mcc_symbol_table_type type)
{
	switch (type) {
	case MCC_SYMBOL_TABLE_ENTRY_TYPE_VARIABLE:
		return "variable";
	case MCC_SYMBOL_TABLE_ENTRY_TYPE_FUNCTION:
		return "function";
	default:
		return "undefined type";
	}
}

static void print_table(FILE *out, struct mcc_symbol_table *table, int depth)
{
	assert(out);
	assert(table);

	if (!table->first_entry) {
		return;
	}

	fprintf(out, "================================================================================================="
	             "=========\n");
	fprintf(out,
	        "|                                          symbol table (depth: %d)                                   "
	        "    |\n",
	        depth);
	fprintf(out, "================================================================================================="
	             "=========\n");
	fprintf(out, "| \tEntry\t | \t%-20s\t | \t\tDataType\t | \tEntryType\t |\n", "Name");
	fprintf(out, "-------------------------------------------------------------------------------------------------"
	             "---------\n");

	struct mcc_symbol_table_entry *entry = table->first_entry;

	int counter = 0;
	while (entry != NULL) {
		char *data_type = get_data_type(entry->data_type);
		char *symbol_type = get_symbol_table_type(entry->symbol_type);

		fprintf(out, "| \t%d\t | \t%-20s\t | \t\t%s\t\t | \t%s\t |\n", counter, entry->name, data_type,
		        symbol_type);

		entry = entry->next;
		counter++;
	}

	fprintf(out, "-------------------------------------------------------------------------------------------------"
	             "---------\n\n");

	if (table->first_child != NULL) {
		print_table(out, table->first_child, depth + 1);
	}
	if (table->next != NULL) {
		print_table(out, table->next, depth);
	}
}

void mcc_print_symbol_table(FILE *out, struct mcc_symbol_table *table)
{
	print_table(out, table, 0);
}
