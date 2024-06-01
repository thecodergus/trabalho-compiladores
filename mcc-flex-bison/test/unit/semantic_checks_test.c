#include <CuTest.h>
#include <stdlib.h>

#include "mcc/ast.h"
#include "mcc/parser.h"
#include "mcc/semantic_checks.h"
#include "mcc/symbol_table.h"
#include "unit_test.h"

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

/******************************** TEST FUNCTIONS ********************************/

void MandelBrotSemanticChecksTest(CuTest *tc)
{

	char *testfile_path = "../test/integration/mandelbrot/mandelbrot.mc";

	struct mcc_symbol_table *symbol_table;
	struct mcc_ast_function_definition *program = NULL;
	get_symbol_table_by_file(testfile_path, &symbol_table, &program);
	CuAssertPtrNotNull(tc, symbol_table);
	CuAssertIntEquals(tc, sizeof(struct mcc_symbol_table), sizeof(*symbol_table));

	struct mcc_semantic_error *semantic_error = mcc_check_semantics(program, symbol_table);
	CuAssertIntEquals(tc, 0, semantic_error->hasError);

	mcc_symbol_table_delete_table(&symbol_table);
	mcc_ast_delete_function_definition(program);
	free(semantic_error);
}

void NoMainTest(CuTest *tc)
{
	const char input[] = "void foo(){;}";

	struct mcc_ast_function_definition *program = NULL;
	struct mcc_parser_result result = mcc_parse_string(input);
	program = result.program;
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);
	struct mcc_semantic_error *semantic_error = mcc_check_semantics(program, symbol_table);
	CuAssertIntEquals(tc, 1, semantic_error->hasError);
	CuAssertIntEquals(tc, MCC_SEMANTIC_ERROR_NO_MAIN, semantic_error->error_type);

	free(semantic_error);
	mcc_symbol_table_delete_table(&symbol_table);
	mcc_parser_delete_result(result);
}

void WrongSignatureMainTest_1(CuTest *tc)
{
	const char input[] = "void main(){;}";

	struct mcc_ast_function_definition *program = NULL;
	struct mcc_parser_result result = mcc_parse_string(input);
	program = result.program;
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);
	struct mcc_semantic_error *semantic_error = mcc_check_semantics(program, symbol_table);
	CuAssertIntEquals(tc, 1, semantic_error->hasError);
	CuAssertIntEquals(tc, MCC_SEMANTIC_ERROR_WRONG_SIGNATURE_MAIN, semantic_error->error_type);

	free(semantic_error);
	mcc_symbol_table_delete_table(&symbol_table);
	mcc_parser_delete_result(result);
}

void WrongSignatureMainTest_2(CuTest *tc)
{
	const char input[] = "int main(int x){return 0;}";

	struct mcc_ast_function_definition *program = NULL;
	struct mcc_parser_result result = mcc_parse_string(input);
	program = result.program;
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);
	struct mcc_semantic_error *semantic_error = mcc_check_semantics(program, symbol_table);
	CuAssertIntEquals(tc, 1, semantic_error->hasError);
	CuAssertIntEquals(tc, MCC_SEMANTIC_ERROR_WRONG_SIGNATURE_MAIN, semantic_error->error_type);

	free(semantic_error);
	mcc_symbol_table_delete_table(&symbol_table);
	mcc_parser_delete_result(result);
}

void MissingReturnMain(CuTest *tc)
{
	const char input[] = "int main(){;}";

	struct mcc_ast_function_definition *program = NULL;
	struct mcc_parser_result result = mcc_parse_string(input);
	program = result.program;
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);
	struct mcc_semantic_error *semantic_error = mcc_check_semantics(program, symbol_table);
	CuAssertIntEquals(tc, 1, semantic_error->hasError);
	CuAssertIntEquals(tc, MCC_SEMANTIC_CHECK_FUNCTION_HAS_PATH_WITHOUT_RETURN, semantic_error->error_type);

	free(semantic_error);
	mcc_symbol_table_delete_table(&symbol_table);
	mcc_parser_delete_result(result);
}

void Test_1(CuTest *tc)
{
	const char input[] = "int main() {foo(2.4); return 0;} void foo(int i){;}";
	struct mcc_ast_function_definition *program = NULL;
	struct mcc_parser_result result = mcc_parse_string(input);
	program = result.program;
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);

	struct mcc_semantic_error *error = mcc_check_semantics(program, symbol_table);
	CuAssertIntEquals(tc, MCC_SEMANTIC_ERROR_TYPE_MISMATCH_LITERAL, error->error_type);

	free(error);
	mcc_symbol_table_delete_table(&symbol_table);
	mcc_parser_delete_result(result);
}

void WrongReturnTypeTest_1(CuTest *tc)
{
	const char input[] = "int main() {foo(2.4); return 0;} string foo(float i){return 2;}";
	struct mcc_ast_function_definition *program = NULL;
	struct mcc_parser_result result = mcc_parse_string(input);
	program = result.program;
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);

	struct mcc_semantic_error *error = mcc_check_semantics(program, symbol_table);
	CuAssertIntEquals(tc, MCC_SEMANTIC_ERROR_FUNCTION_WRONG_RETURN_TYPE, error->error_type);

	free(error);
	mcc_symbol_table_delete_table(&symbol_table);
	mcc_parser_delete_result(result);
}

void WrongReturnTypeTest_2(CuTest *tc)
{
	const char input[] = "int main() {foo(2.4); return 0;} float foo(float i){return 2;}";
	struct mcc_ast_function_definition *program = NULL;
	struct mcc_parser_result result = mcc_parse_string(input);
	program = result.program;
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);

	struct mcc_semantic_error *error = mcc_check_semantics(program, symbol_table);
	CuAssertIntEquals(tc, MCC_SEMANTIC_ERROR_FUNCTION_WRONG_RETURN_TYPE, error->error_type);

	free(error);
	mcc_symbol_table_delete_table(&symbol_table);
	mcc_parser_delete_result(result);
}

void WrongReturnTypeTest_3(CuTest *tc)
{
	const char input[] = "int main() {foo(2.4); return 0;} int foo(float i){return 2.1;}";
	struct mcc_ast_function_definition *program = NULL;
	struct mcc_parser_result result = mcc_parse_string(input);
	program = result.program;
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);

	struct mcc_semantic_error *error = mcc_check_semantics(program, symbol_table);
	CuAssertIntEquals(tc, MCC_SEMANTIC_ERROR_FUNCTION_WRONG_RETURN_TYPE, error->error_type);

	free(error);
	mcc_symbol_table_delete_table(&symbol_table);
	mcc_parser_delete_result(result);
}

void WrongReturnTypeTest_4(CuTest *tc)
{
	const char input[] = "int main() {foo(2.4); return 0;} bool foo(float i){return 2.1;}";
	struct mcc_ast_function_definition *program = NULL;
	struct mcc_parser_result result = mcc_parse_string(input);
	program = result.program;
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);

	struct mcc_semantic_error *error = mcc_check_semantics(program, symbol_table);
	CuAssertIntEquals(tc, MCC_SEMANTIC_ERROR_FUNCTION_WRONG_RETURN_TYPE, error->error_type);

	free(error);
	mcc_symbol_table_delete_table(&symbol_table);
	mcc_parser_delete_result(result);
}

void BinaryOperatorDifferentTypesTest_1(CuTest *tc)
{

	const char input[] = "int main() {int a; a= 1+1.1; return 0;}";
	struct mcc_ast_function_definition *program = NULL;
	struct mcc_parser_result result = mcc_parse_string(input);
	program = result.program;
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);

	struct mcc_semantic_error *error = mcc_check_semantics(program, symbol_table);
	CuAssertIntEquals(tc, MCC_SEMANTIC_ERROR_TYPE_MISMATCH_BINARY_OP, error->error_type);

	free(error);
	mcc_symbol_table_delete_table(&symbol_table);
	mcc_parser_delete_result(result);
}

void BinaryOperatorDifferentTypesTest_2(CuTest *tc)
{

	const char input[] = "int main() {int a; a= 1-1.1; return 0;}";
	struct mcc_ast_function_definition *program = NULL;
	struct mcc_parser_result result = mcc_parse_string(input);
	program = result.program;
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);

	struct mcc_semantic_error *error = mcc_check_semantics(program, symbol_table);
	CuAssertIntEquals(tc, MCC_SEMANTIC_ERROR_TYPE_MISMATCH_BINARY_OP, error->error_type);

	free(error);
	mcc_symbol_table_delete_table(&symbol_table);
	mcc_parser_delete_result(result);
}

void BinaryOperatorDifferentTypesTest_3(CuTest *tc)
{

	const char input[] = "int main() {int a; a= 1*1.1; return 0;}";
	struct mcc_ast_function_definition *program = NULL;
	struct mcc_parser_result result = mcc_parse_string(input);
	program = result.program;
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);

	struct mcc_semantic_error *error = mcc_check_semantics(program, symbol_table);
	CuAssertIntEquals(tc, MCC_SEMANTIC_ERROR_TYPE_MISMATCH_BINARY_OP, error->error_type);

	free(error);
	mcc_symbol_table_delete_table(&symbol_table);
	mcc_parser_delete_result(result);
}

void BinaryOperatorDifferentTypesTest_4(CuTest *tc)
{

	const char input[] = "int main() {int a; a= 1/1.1; return 0;}";
	struct mcc_ast_function_definition *program = NULL;
	struct mcc_parser_result result = mcc_parse_string(input);
	program = result.program;
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);

	struct mcc_semantic_error *error = mcc_check_semantics(program, symbol_table);
	CuAssertIntEquals(tc, MCC_SEMANTIC_ERROR_TYPE_MISMATCH_BINARY_OP, error->error_type);

	free(error);
	mcc_symbol_table_delete_table(&symbol_table);
	mcc_parser_delete_result(result);
}

void MissingDeclarationTest_1(CuTest *tc)
{

	const char input[] = "int main() {a= 1; return 0;}";
	struct mcc_ast_function_definition *program = NULL;
	struct mcc_parser_result result = mcc_parse_string(input);
	program = result.program;
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);

	struct mcc_semantic_error *error = mcc_check_semantics(program, symbol_table);
	CuAssertIntEquals(tc, MCC_SEMANTIC_ERROR_MISSING_DECLARATION, error->error_type);

	free(error);
	mcc_symbol_table_delete_table(&symbol_table);
	mcc_parser_delete_result(result);
}

void MissingDeclarationTest_2(CuTest *tc)
{

	const char input[] = "int main() {{int a;} a= 1; return 0;}";
	struct mcc_ast_function_definition *program = NULL;
	struct mcc_parser_result result = mcc_parse_string(input);
	program = result.program;
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);

	struct mcc_semantic_error *error = mcc_check_semantics(program, symbol_table);
	CuAssertIntEquals(tc, MCC_SEMANTIC_ERROR_MISSING_DECLARATION, error->error_type);

	free(error);
	mcc_symbol_table_delete_table(&symbol_table);
	mcc_parser_delete_result(result);
}

void WrongLiteralTypeTest_1(CuTest *tc)
{

	const char input[] = "int main() {int a; a = 2.0; return 0;}";
	struct mcc_ast_function_definition *program = NULL;
	struct mcc_parser_result result = mcc_parse_string(input);
	program = result.program;
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);

	struct mcc_semantic_error *error = mcc_check_semantics(program, symbol_table);
	CuAssertIntEquals(tc, MCC_SEMANTIC_ERROR_TYPE_MISMATCH_LITERAL, error->error_type);

	free(error);
	mcc_symbol_table_delete_table(&symbol_table);
	mcc_parser_delete_result(result);
}

void WrongLiteralTypeTest_2(CuTest *tc)
{

	const char input[] = "int main() {float a; a = 2; return 0;}";
	struct mcc_ast_function_definition *program = NULL;
	struct mcc_parser_result result = mcc_parse_string(input);
	program = result.program;
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);

	struct mcc_semantic_error *error = mcc_check_semantics(program, symbol_table);
	CuAssertIntEquals(tc, MCC_SEMANTIC_ERROR_TYPE_MISMATCH_LITERAL, error->error_type);

	free(error);
	mcc_symbol_table_delete_table(&symbol_table);
	mcc_parser_delete_result(result);
}

void WrongLiteralTypeTest_3(CuTest *tc)
{

	const char input[] = "int main() {bool a; a = 2; return 0;}";
	struct mcc_ast_function_definition *program = NULL;
	struct mcc_parser_result result = mcc_parse_string(input);
	program = result.program;
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);

	struct mcc_semantic_error *error = mcc_check_semantics(program, symbol_table);
	CuAssertIntEquals(tc, MCC_SEMANTIC_ERROR_TYPE_MISMATCH_LITERAL, error->error_type);

	free(error);
	mcc_symbol_table_delete_table(&symbol_table);
	mcc_parser_delete_result(result);
}

void WrongLiteralTypeTest_4(CuTest *tc)
{

	const char input[] = "int main() {string a; a = true; return 0;}";
	struct mcc_ast_function_definition *program = NULL;
	struct mcc_parser_result result = mcc_parse_string(input);
	program = result.program;
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);

	struct mcc_semantic_error *error = mcc_check_semantics(program, symbol_table);
	CuAssertIntEquals(tc, MCC_SEMANTIC_ERROR_TYPE_MISMATCH_LITERAL, error->error_type);

	free(error);
	mcc_symbol_table_delete_table(&symbol_table);
	mcc_parser_delete_result(result);
}

void WrongIdentifierTypeTest_1(CuTest *tc)
{

	const char input[] = "int main() {int a; float b; a = b; return 0;}";
	struct mcc_ast_function_definition *program = NULL;
	struct mcc_parser_result result = mcc_parse_string(input);
	program = result.program;
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);

	struct mcc_semantic_error *error = mcc_check_semantics(program, symbol_table);
	CuAssertIntEquals(tc, MCC_SEMANTIC_ERROR_TYPE_MISMATCH_IDENTIFIER, error->error_type);

	free(error);
	mcc_symbol_table_delete_table(&symbol_table);
	mcc_parser_delete_result(result);
}

void WrongIdentifierTypeTest_2(CuTest *tc)
{

	const char input[] = "int main() {float a; bool b; a = b; return 0;}";
	struct mcc_ast_function_definition *program = NULL;
	struct mcc_parser_result result = mcc_parse_string(input);
	program = result.program;
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);

	struct mcc_semantic_error *error = mcc_check_semantics(program, symbol_table);
	CuAssertIntEquals(tc, MCC_SEMANTIC_ERROR_TYPE_MISMATCH_IDENTIFIER, error->error_type);

	free(error);
	mcc_symbol_table_delete_table(&symbol_table);
	mcc_parser_delete_result(result);
}

void WrongIdentifierTypeTest_3(CuTest *tc)
{

	const char input[] = "int main() {bool a; string b; a = b; return 0;}";
	struct mcc_ast_function_definition *program = NULL;
	struct mcc_parser_result result = mcc_parse_string(input);
	program = result.program;
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);

	struct mcc_semantic_error *error = mcc_check_semantics(program, symbol_table);
	CuAssertIntEquals(tc, MCC_SEMANTIC_ERROR_TYPE_MISMATCH_IDENTIFIER, error->error_type);

	free(error);
	mcc_symbol_table_delete_table(&symbol_table);
	mcc_parser_delete_result(result);
}

void WrongIdentifierTypeTest_4(CuTest *tc)
{

	const char input[] = "int main() {string a; int b; a = b; return 0;}";
	struct mcc_ast_function_definition *program = NULL;
	struct mcc_parser_result result = mcc_parse_string(input);
	program = result.program;
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);

	struct mcc_semantic_error *error = mcc_check_semantics(program, symbol_table);
	CuAssertIntEquals(tc, MCC_SEMANTIC_ERROR_TYPE_MISMATCH_IDENTIFIER, error->error_type);

	free(error);
	mcc_symbol_table_delete_table(&symbol_table);
	mcc_parser_delete_result(result);
}

void ArrayTest_1(CuTest *tc)
{

	const char input[] = "int main() {int[5] a; a = 5; return 0;}";
	struct mcc_ast_function_definition *program = NULL;
	struct mcc_parser_result result = mcc_parse_string(input);
	program = result.program;
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);

	struct mcc_semantic_error *error = mcc_check_semantics(program, symbol_table);
	CuAssertIntEquals(tc, MCC_SEMANTIC_ERROR_ASSIGN_VALUE_ARRAY, error->error_type);

	free(error);
	mcc_symbol_table_delete_table(&symbol_table);
	mcc_parser_delete_result(result);
}

void ArrayTest_2(CuTest *tc)
{

	const char input[] = "int main() {int[5] a; int b; b=a; return 0;}";
	struct mcc_ast_function_definition *program = NULL;
	struct mcc_parser_result result = mcc_parse_string(input);
	program = result.program;
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);

	struct mcc_semantic_error *error = mcc_check_semantics(program, symbol_table);
	CuAssertIntEquals(tc, MCC_SEMANTIC_ERROR_ARRAY_EXPECTED, error->error_type);

	free(error);
	mcc_symbol_table_delete_table(&symbol_table);
	mcc_parser_delete_result(result);
}

#define TESTS \
	TEST(Test_1) \
	TEST(MandelBrotSemanticChecksTest) \
	TEST(NoMainTest) \
	TEST(WrongSignatureMainTest_1) \
	TEST(WrongSignatureMainTest_2) \
	TEST(MissingReturnMain) \
	TEST(WrongReturnTypeTest_1) \
	TEST(WrongReturnTypeTest_2) \
	TEST(WrongReturnTypeTest_3) \
	TEST(WrongReturnTypeTest_4) \
	TEST(BinaryOperatorDifferentTypesTest_1) \
	TEST(BinaryOperatorDifferentTypesTest_2) \
	TEST(BinaryOperatorDifferentTypesTest_3) \
	TEST(BinaryOperatorDifferentTypesTest_4) \
	TEST(MissingDeclarationTest_1) \
	TEST(MissingDeclarationTest_2) \
	TEST(WrongLiteralTypeTest_1) \
	TEST(WrongLiteralTypeTest_2) \
	TEST(WrongLiteralTypeTest_3) \
	TEST(WrongLiteralTypeTest_4) \
	TEST(WrongIdentifierTypeTest_1) \
	TEST(WrongIdentifierTypeTest_2) \
	TEST(WrongIdentifierTypeTest_3) \
	TEST(WrongIdentifierTypeTest_4) \
	TEST(ArrayTest_1) \
	TEST(ArrayTest_2)

#include "main_stub.inc"
#undef TESTS
