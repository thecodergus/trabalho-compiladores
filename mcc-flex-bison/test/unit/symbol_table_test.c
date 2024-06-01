#include <CuTest.h>

#include "mcc/ast.h"
#include "mcc/ast_visit.h"
#include "mcc/parser.h"
#include "mcc/symbol_table.h"
#include <stdlib.h>

void get_symbol_table_by_file(char *path,
                              struct mcc_symbol_table **symbol_table,
                              struct mcc_ast_function_definition **program)
{
	struct mcc_parser_result result;
	FILE *in = fopen(path, "r");
	{
		result = mcc_parse_file(in);
		fclose(in);
		if (result.status != MCC_PARSER_STATUS_OK) {
			printf("Error: %s\n", result.error_message);
			return;
		}
	}
	*program = result.program;
	*symbol_table = mcc_symbol_table_create(result.program);
}

void mcc_parser_delete_result(struct mcc_parser_result result)
{
	if (result.error_message)
		free(result.error_message);
	if (result.program)
		mcc_ast_delete_function_definition(result.program);
	if (result.expression)
		mcc_ast_delete_expression(result.expression);
}

void FibSymbolTableCreationTest(CuTest *tc)
{

	char *testfile_path = "../test/integration/fib/fib.mc";

	struct mcc_symbol_table *symbol_table;
	struct mcc_ast_function_definition *program = NULL;
	get_symbol_table_by_file(testfile_path, &symbol_table, &program);
	CuAssertPtrNotNull(tc, symbol_table);
	CuAssertIntEquals(tc, sizeof(struct mcc_symbol_table), sizeof(*symbol_table));

	mcc_symbol_table_delete_table(&symbol_table);
	mcc_ast_delete_function_definition(program);
}

void BuiltinFunctionsTest(CuTest *tc)
{
	struct mcc_ast_function_definition *program = NULL;
	const char input[] = "int main(){return 0;}";
	struct mcc_parser_result result = mcc_parse_string(input);
	program = result.program;
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);

	CuAssertPtrNotNull(tc, symbol_table);
	CuAssertIntEquals(tc, sizeof(struct mcc_symbol_table), sizeof(*symbol_table));

	struct mcc_symbol_table_entry *table_entry = symbol_table->first_entry;
	CuAssertStrEquals(tc, "print", table_entry->name);
	CuAssertIntEquals(tc, MCC_AST_TYPE_VOID, table_entry->data_type);
	CuAssertIntEquals(tc, MCC_SYMBOL_TABLE_ENTRY_TYPE_FUNCTION, table_entry->symbol_type);
	CuAssertIntEquals(tc, MCC_AST_TYPE_STRING, table_entry->parameters->type);
	CuAssertStrEquals(tc, "parameter", table_entry->parameters->identifier);

	table_entry = table_entry->next;
	CuAssertStrEquals(tc, "print_nl", table_entry->name);
	CuAssertIntEquals(tc, MCC_AST_TYPE_VOID, table_entry->data_type);
	CuAssertIntEquals(tc, MCC_SYMBOL_TABLE_ENTRY_TYPE_FUNCTION, table_entry->symbol_type);

	table_entry = table_entry->next;
	CuAssertStrEquals(tc, "print_int", table_entry->name);
	CuAssertIntEquals(tc, MCC_AST_TYPE_VOID, table_entry->data_type);
	CuAssertIntEquals(tc, MCC_SYMBOL_TABLE_ENTRY_TYPE_FUNCTION, table_entry->symbol_type);
	CuAssertIntEquals(tc, MCC_AST_TYPE_INT, table_entry->parameters->type);
	CuAssertStrEquals(tc, "parameter", table_entry->parameters->identifier);

	table_entry = table_entry->next;
	CuAssertStrEquals(tc, "print_float", table_entry->name);
	CuAssertIntEquals(tc, MCC_AST_TYPE_VOID, table_entry->data_type);
	CuAssertIntEquals(tc, MCC_SYMBOL_TABLE_ENTRY_TYPE_FUNCTION, table_entry->symbol_type);
	CuAssertIntEquals(tc, MCC_AST_TYPE_FLOAT, table_entry->parameters->type);
	CuAssertStrEquals(tc, "parameter", table_entry->parameters->identifier);

	table_entry = table_entry->next;
	CuAssertStrEquals(tc, "read_int", table_entry->name);
	CuAssertIntEquals(tc, MCC_AST_TYPE_INT, table_entry->data_type);
	CuAssertIntEquals(tc, MCC_SYMBOL_TABLE_ENTRY_TYPE_FUNCTION, table_entry->symbol_type);

	table_entry = table_entry->next;
	CuAssertStrEquals(tc, "read_float", table_entry->name);
	CuAssertIntEquals(tc, MCC_AST_TYPE_FLOAT, table_entry->data_type);
	CuAssertIntEquals(tc, MCC_SYMBOL_TABLE_ENTRY_TYPE_FUNCTION, table_entry->symbol_type);

	mcc_symbol_table_delete_table(&symbol_table);
	mcc_parser_delete_result(result);
}

void CheckTableLinking(CuTest *tc)
{
	struct mcc_ast_function_definition *program = NULL;
	const char input[] = "int main(){return 0;}";
	struct mcc_parser_result result = mcc_parse_string(input);
	program = result.program;
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);

	CuAssertPtrNotNull(tc, symbol_table);
	CuAssertIntEquals(tc, sizeof(struct mcc_symbol_table), sizeof(*symbol_table));

	// create child table
	struct mcc_symbol_table *child_table = mcc_symbol_table_new_table(symbol_table);
	CuAssertPtrNotNull(tc, child_table);
	CuAssertIntEquals(tc, sizeof(struct mcc_symbol_table), sizeof(*child_table));

	CuAssertPtrEquals(tc, NULL, child_table->next);

	// check if parent table of child table is the previously created symbol table
	CuAssertPtrEquals(tc, symbol_table, child_table->parent_symbol_table);

	// check if the previously created symbol table has set child table as its child
	CuAssertPtrEquals(tc, child_table, symbol_table->first_child->next);

	mcc_symbol_table_delete_table(&symbol_table);
	mcc_parser_delete_result(result);
}

void CheckLookup(CuTest *tc)
{
	struct mcc_ast_function_definition *program = NULL;
	const char input[] = "int main(){return 0;}";
	struct mcc_parser_result result = mcc_parse_string(input);
	program = result.program;
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);

	CuAssertPtrNotNull(tc, symbol_table);
	CuAssertIntEquals(tc, sizeof(struct mcc_symbol_table), sizeof(*symbol_table));

	// Check if table is root table
	CuAssertPtrEquals(tc, NULL, symbol_table->parent_symbol_table);

	// lookup print
	struct mcc_symbol_table_entry *table_entry = mcc_symbol_table_lookup_function_entry(symbol_table, "print");
	CuAssertPtrNotNull(tc, table_entry);

	CuAssertStrEquals(tc, "print", table_entry->name);
	CuAssertIntEquals(tc, MCC_AST_TYPE_VOID, table_entry->data_type);
	CuAssertIntEquals(tc, MCC_SYMBOL_TABLE_ENTRY_TYPE_FUNCTION, table_entry->symbol_type);
	CuAssertIntEquals(tc, MCC_AST_TYPE_STRING, table_entry->parameters->type);
	CuAssertStrEquals(tc, "parameter", table_entry->parameters->identifier);

	table_entry = mcc_symbol_table_lookup_function_entry(symbol_table, "print_nl");
	CuAssertStrEquals(tc, "print_nl", table_entry->name);
	CuAssertIntEquals(tc, MCC_AST_TYPE_VOID, table_entry->data_type);
	CuAssertIntEquals(tc, MCC_SYMBOL_TABLE_ENTRY_TYPE_FUNCTION, table_entry->symbol_type);

	table_entry = mcc_symbol_table_lookup_function_entry(symbol_table, "print_int");
	CuAssertStrEquals(tc, "print_int", table_entry->name);
	CuAssertIntEquals(tc, MCC_AST_TYPE_VOID, table_entry->data_type);
	CuAssertIntEquals(tc, MCC_SYMBOL_TABLE_ENTRY_TYPE_FUNCTION, table_entry->symbol_type);
	CuAssertIntEquals(tc, MCC_AST_TYPE_INT, table_entry->parameters->type);
	CuAssertStrEquals(tc, "parameter", table_entry->parameters->identifier);

	table_entry = mcc_symbol_table_lookup_function_entry(symbol_table, "print_float");
	CuAssertStrEquals(tc, "print_float", table_entry->name);
	CuAssertIntEquals(tc, MCC_AST_TYPE_VOID, table_entry->data_type);
	CuAssertIntEquals(tc, MCC_SYMBOL_TABLE_ENTRY_TYPE_FUNCTION, table_entry->symbol_type);
	CuAssertIntEquals(tc, MCC_AST_TYPE_FLOAT, table_entry->parameters->type);
	CuAssertStrEquals(tc, "parameter", table_entry->parameters->identifier);

	table_entry = mcc_symbol_table_lookup_function_entry(symbol_table, "read_int");
	CuAssertStrEquals(tc, "read_int", table_entry->name);
	CuAssertIntEquals(tc, MCC_AST_TYPE_INT, table_entry->data_type);
	CuAssertIntEquals(tc, MCC_SYMBOL_TABLE_ENTRY_TYPE_FUNCTION, table_entry->symbol_type);

	table_entry = mcc_symbol_table_lookup_function_entry(symbol_table, "read_float");
	CuAssertStrEquals(tc, "read_float", table_entry->name);
	CuAssertIntEquals(tc, MCC_AST_TYPE_FLOAT, table_entry->data_type);
	CuAssertIntEquals(tc, MCC_SYMBOL_TABLE_ENTRY_TYPE_FUNCTION, table_entry->symbol_type);

	mcc_symbol_table_delete_table(&symbol_table);
	mcc_parser_delete_result(result);
}

#define TESTS \
	TEST(FibSymbolTableCreationTest) \
	TEST(BuiltinFunctionsTest) \
	TEST(CheckTableLinking) \
	TEST(CheckLookup)

#include "main_stub.inc"
#undef TESTS
