#include <CuTest.h>

#include "mcc/ast.h"
#include "mcc/parser.h"
#include "unit_test.h"
#include <stdlib.h>
#include <unistd.h>

// Threshold for floating point comparisions.
static const double EPS = 1e-3;

void mcc_parser_delete_result(struct mcc_parser_result result)
{
	if (result.error_message)
		free(result.error_message);
	if (result.program)
		mcc_ast_delete_function_definition(result.program);
	if (result.expression)
		mcc_ast_delete_expression(result.expression);
}

void FunctionTestMain(CuTest *tc)
{
	const char input[] = "int main(){return 0;}";
	struct mcc_parser_result result = mcc_parse_string(input);

	CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

	struct mcc_ast_function_definition *definition = result.program;

	CuAssertIntEquals(tc, MCC_AST_TYPE_INT, definition->return_type);
	CuAssertStrEquals(tc, "main", definition->identifier);

	struct mcc_ast_statement *statement = definition->statements;
	CuAssertIntEquals(tc, MCC_AST_STATEMENT_TYPE_COMPOUND, statement->type);
	statement = statement->compound_statement;

	CuAssertIntEquals(tc, MCC_AST_STATEMENT_TYPE_RETURN, statement->type);
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, statement->return_expression->type);

	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, statement->return_expression->literal->type);
	CuAssertIntEquals(tc, 0, statement->return_expression->literal->i_value);

	mcc_parser_delete_result(result);
}

void FunctionTestFloat(CuTest *tc)
{
	const char input[] = "float foo(){return 3.231;}";

	struct mcc_parser_result result = mcc_parse_string(input);

	CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

	struct mcc_ast_function_definition *definition = result.program;

	CuAssertIntEquals(tc, MCC_AST_TYPE_FLOAT, definition->return_type);
	CuAssertStrEquals(tc, "foo", definition->identifier);

	struct mcc_ast_statement *statement = definition->statements;
	CuAssertIntEquals(tc, MCC_AST_STATEMENT_TYPE_COMPOUND, statement->type);
	statement = statement->compound_statement;

	CuAssertIntEquals(tc, MCC_AST_STATEMENT_TYPE_RETURN, statement->type);
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, statement->return_expression->type);

	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_FLOAT, statement->return_expression->literal->type);
	CuAssertDblEquals(tc, 3.231, statement->return_expression->literal->f_value, EPS);

	mcc_parser_delete_result(result);
}

void FunctionTestString(CuTest *tc)
{
	const char input[] = "string foo(){return \"Hello World\";}";

	struct mcc_parser_result result = mcc_parse_string(input);

	CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

	struct mcc_ast_function_definition *definition = result.program;

	CuAssertIntEquals(tc, MCC_AST_TYPE_STRING, definition->return_type);
	CuAssertStrEquals(tc, "foo", definition->identifier);

	struct mcc_ast_statement *statement = definition->statements;
	CuAssertIntEquals(tc, MCC_AST_STATEMENT_TYPE_COMPOUND, statement->type);
	statement = statement->compound_statement;

	CuAssertIntEquals(tc, MCC_AST_STATEMENT_TYPE_RETURN, statement->type);
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, statement->return_expression->type);

	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_STRING, statement->return_expression->literal->type);
	CuAssertStrEquals(tc, "Hello World", statement->return_expression->literal->s_value);

	mcc_parser_delete_result(result);
}

void FunctionTestBool(CuTest *tc)
{
	const char input[] = "bool foo(){return false;}";

	struct mcc_parser_result result = mcc_parse_string(input);

	CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

	struct mcc_ast_function_definition *definition = result.program;

	CuAssertIntEquals(tc, MCC_AST_TYPE_BOOL, definition->return_type);
	CuAssertStrEquals(tc, "foo", definition->identifier);

	struct mcc_ast_statement *statement = definition->statements;
	CuAssertIntEquals(tc, MCC_AST_STATEMENT_TYPE_COMPOUND, statement->type);
	statement = statement->compound_statement;

	// return statement
	CuAssertIntEquals(tc, MCC_AST_STATEMENT_TYPE_RETURN, statement->type);
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, statement->return_expression->type);

	// return statement -> literal
	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_BOOL, statement->return_expression->literal->type);
	CuAssertStrEquals(tc, "false", statement->return_expression->literal->b_value);

	mcc_parser_delete_result(result);
}

void AssociativityTest_1(CuTest *tc)
{
	const char input[] = "int main(){false && false || true; return 0;}";
	struct mcc_parser_result result = mcc_parse_string(input);

	CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

	struct mcc_ast_expression *expr = result.program->statements->compound_statement->expression;

	// root
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
	CuAssertIntEquals(tc, MCC_AST_BINARY_OP_OR, expr->op);

	// root -> rhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->rhs->type);
	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_BOOL, expr->rhs->literal->type);
	CuAssertStrEquals(tc, "true", expr->rhs->literal->b_value);

	// root -> lhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->lhs->type);
	CuAssertIntEquals(tc, MCC_AST_BINARY_OP_AND, expr->lhs->op);

	// root -> lhs -> rhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->rhs->type);
	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_BOOL, expr->lhs->rhs->literal->type);
	CuAssertStrEquals(tc, "false", expr->lhs->rhs->literal->b_value);

	// root -> lhs -> lhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->lhs->type);
	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_BOOL, expr->lhs->lhs->literal->type);
	CuAssertStrEquals(tc, "false", expr->lhs->lhs->literal->b_value);

	mcc_parser_delete_result(result);
}

void AssociativityTest_2(CuTest *tc)
{
	const char input[] = "int main(){false == false != true; return 0;}";
	struct mcc_parser_result result = mcc_parse_string(input);

	CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

	struct mcc_ast_expression *expr = result.program->statements->compound_statement->expression;

	// root
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
	CuAssertIntEquals(tc, MCC_AST_BINARY_OP_NOT_EQ, expr->op);

	// root -> rhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->rhs->type);
	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_BOOL, expr->rhs->literal->type);
	CuAssertStrEquals(tc, "true", expr->rhs->literal->b_value);

	// root -> lhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->lhs->type);
	CuAssertIntEquals(tc, MCC_AST_BINARY_OP_EQ, expr->lhs->op);

	// root -> lhs -> rhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->rhs->type);
	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_BOOL, expr->lhs->rhs->literal->type);
	CuAssertStrEquals(tc, "false", expr->lhs->rhs->literal->b_value);

	// root -> lhs -> lhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->lhs->type);
	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_BOOL, expr->lhs->lhs->literal->type);
	CuAssertStrEquals(tc, "false", expr->lhs->lhs->literal->b_value);

	mcc_parser_delete_result(result);
}

void AssociativityTest_3(CuTest *tc)
{
	const char input[] = "int main(){1 < 2 <= 3; return 0;}";
	struct mcc_parser_result result = mcc_parse_string(input);

	CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

	struct mcc_ast_expression *expr = result.program->statements->compound_statement->expression;

	// root
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
	CuAssertIntEquals(tc, MCC_AST_BINARY_OP_LESS_EQ, expr->op);

	// root -> rhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->rhs->type);
	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, expr->rhs->literal->type);
	CuAssertIntEquals(tc, 3, expr->rhs->literal->i_value);

	// root -> lhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->lhs->type);
	CuAssertIntEquals(tc, MCC_AST_BINARY_OP_LESS, expr->lhs->op);

	// root -> lhs -> rhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->rhs->type);
	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, expr->lhs->rhs->literal->type);
	CuAssertIntEquals(tc, 2, expr->lhs->rhs->literal->i_value);

	// root -> lhs -> lhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->lhs->type);
	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, expr->lhs->lhs->literal->type);
	CuAssertIntEquals(tc, 1, expr->lhs->lhs->literal->i_value);

	mcc_parser_delete_result(result);
}

void AssociativityTest_4(CuTest *tc)
{
	const char input[] = "int main(){3 >= 2 > 1; return 0;}";
	struct mcc_parser_result result = mcc_parse_string(input);

	CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

	struct mcc_ast_expression *expr = result.program->statements->compound_statement->expression;

	// root
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
	CuAssertIntEquals(tc, MCC_AST_BINARY_OP_GREATER, expr->op);

	// root -> rhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->rhs->type);
	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, expr->rhs->literal->type);
	CuAssertIntEquals(tc, 1, expr->rhs->literal->i_value);

	// root -> lhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->lhs->type);
	CuAssertIntEquals(tc, MCC_AST_BINARY_OP_GREATER_EQ, expr->lhs->op);

	// root -> lhs -> rhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->rhs->type);
	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, expr->lhs->rhs->literal->type);
	CuAssertIntEquals(tc, 2, expr->lhs->rhs->literal->i_value);

	// root -> lhs -> lhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->lhs->type);
	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, expr->lhs->lhs->literal->type);
	CuAssertIntEquals(tc, 3, expr->lhs->lhs->literal->i_value);

	mcc_parser_delete_result(result);
}

void AssociativityTest_5(CuTest *tc)
{
	const char input[] = "int main(){3 + 2 - 1; return 0;}";
	struct mcc_parser_result result = mcc_parse_string(input);

	CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

	struct mcc_ast_expression *expr = result.program->statements->compound_statement->expression;

	// root
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
	CuAssertIntEquals(tc, MCC_AST_BINARY_OP_SUB, expr->op);

	// root -> rhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->rhs->type);
	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, expr->rhs->literal->type);
	CuAssertIntEquals(tc, 1, expr->rhs->literal->i_value);

	// root -> lhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->lhs->type);
	CuAssertIntEquals(tc, MCC_AST_BINARY_OP_ADD, expr->lhs->op);

	// root -> lhs -> rhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->rhs->type);
	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, expr->lhs->rhs->literal->type);
	CuAssertIntEquals(tc, 2, expr->lhs->rhs->literal->i_value);

	// root -> lhs -> lhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->lhs->type);
	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, expr->lhs->lhs->literal->type);
	CuAssertIntEquals(tc, 3, expr->lhs->lhs->literal->i_value);

	mcc_parser_delete_result(result);
}

void AssociativityTest_6(CuTest *tc)
{
	const char input[] = "int main(){3 / 2 * 1; return 0;}";
	struct mcc_parser_result result = mcc_parse_string(input);

	CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

	struct mcc_ast_expression *expr = result.program->statements->compound_statement->expression;

	// root
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
	CuAssertIntEquals(tc, MCC_AST_BINARY_OP_MUL, expr->op);

	// root -> rhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->rhs->type);
	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, expr->rhs->literal->type);
	CuAssertIntEquals(tc, 1, expr->rhs->literal->i_value);

	// root -> lhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->lhs->type);
	CuAssertIntEquals(tc, MCC_AST_BINARY_OP_DIV, expr->lhs->op);

	// root -> lhs -> rhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->rhs->type);
	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, expr->lhs->rhs->literal->type);
	CuAssertIntEquals(tc, 2, expr->lhs->rhs->literal->i_value);

	// root -> lhs -> lhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->lhs->type);
	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, expr->lhs->lhs->literal->type);
	CuAssertIntEquals(tc, 3, expr->lhs->lhs->literal->i_value);

	mcc_parser_delete_result(result);
}

void FunctionCallTest(CuTest *tc)
{

	const char input[] = "int main(){foo(3,2); return 0;}";
	struct mcc_parser_result result = mcc_parse_string(input);

	CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

	struct mcc_ast_expression *expr = result.program->statements->compound_statement->expression;

	// root
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_FUNCTION_CALL, expr->type);

	CuAssertStrEquals(tc, "foo", expr->function_call->identifier);

	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->function_call->arguments->type);
	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, expr->function_call->arguments->literal->type);
	CuAssertIntEquals(tc, 3, expr->function_call->arguments->literal->i_value);

	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->function_call->arguments->next_expression->type);
	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, expr->function_call->arguments->next_expression->literal->type);
	CuAssertIntEquals(tc, 2, expr->function_call->arguments->next_expression->literal->i_value);

	mcc_parser_delete_result(result);
}

void OperatorPrecedence_1(CuTest *tc)
{
	const char input[] = "int main(){5+5 == 5*2 && 3*3 != 3+3; return 0;}";
	struct mcc_parser_result result = mcc_parse_string(input);

	CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

	struct mcc_ast_expression *expr = result.program->statements->compound_statement->expression;

	// root
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
	CuAssertIntEquals(tc, MCC_AST_BINARY_OP_AND, expr->op);

	// root -> lhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->lhs->type);
	CuAssertIntEquals(tc, MCC_AST_BINARY_OP_EQ, expr->lhs->op);

	// root -> rhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->rhs->type);
	CuAssertIntEquals(tc, MCC_AST_BINARY_OP_NOT_EQ, expr->rhs->op);

	// root -> lhs -> lhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->lhs->lhs->type);
	CuAssertIntEquals(tc, MCC_AST_BINARY_OP_ADD, expr->lhs->lhs->op);

	// root -> rhs -> rhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->rhs->rhs->type);
	CuAssertIntEquals(tc, MCC_AST_BINARY_OP_ADD, expr->rhs->rhs->op);

	mcc_parser_delete_result(result);
}

void OperatorPrecedence_2(CuTest *tc)
{

	//				   +
	//			 +			4
	//		1        *
	//			2		 3

	const char input[] = "int main(){1 + 2 * 3 + 4;}";
	struct mcc_parser_result result = mcc_parse_string(input);

	CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

	struct mcc_ast_expression *expr = result.program->statements->compound_statement->expression;

	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
	CuAssertIntEquals(tc, MCC_AST_BINARY_OP_ADD, expr->op);
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->lhs->type);
	CuAssertIntEquals(tc, MCC_AST_BINARY_OP_ADD, expr->lhs->op);
	CuAssertIntEquals(tc, MCC_AST_BINARY_OP_MUL, expr->lhs->rhs->op);

	CuAssertIntEquals(tc, 4, expr->rhs->literal->i_value);
	CuAssertIntEquals(tc, 1, expr->lhs->lhs->literal->i_value);
	CuAssertIntEquals(tc, 2, expr->lhs->rhs->lhs->literal->i_value);
	CuAssertIntEquals(tc, 3, expr->lhs->rhs->rhs->literal->i_value);

	mcc_parser_delete_result(result);
}

void BinaryOp_1(CuTest *tc)
{
	const char input[] = "int main(){192 + 3.14;}";
	struct mcc_parser_result result = mcc_parse_string(input);

	CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

	struct mcc_ast_expression *expr = result.program->statements->compound_statement->expression;

	// root
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
	CuAssertIntEquals(tc, MCC_AST_BINARY_OP_ADD, expr->op);

	// root -> lhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->type);

	// root -> lhs -> literal
	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, expr->lhs->literal->type);
	CuAssertIntEquals(tc, 192, expr->lhs->literal->i_value);

	// root -> rhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->rhs->type);

	// root -> rhs -> literal
	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_FLOAT, expr->rhs->literal->type);
	CuAssertDblEquals(tc, 3.14, expr->rhs->literal->f_value, EPS);

	mcc_parser_delete_result(result);
}

void NestedExpression_1(CuTest *tc)
{
	const char input[] = "int main(){42 * (192 + 3.14);}";
	struct mcc_parser_result result = mcc_parse_string(input);

	CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

	struct mcc_ast_expression *expr = result.program->statements->compound_statement->expression;

	// root
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
	CuAssertIntEquals(tc, MCC_AST_BINARY_OP_MUL, expr->op);

	// root -> lhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->type);

	// root -> lhs -> literal
	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, expr->lhs->literal->type);
	CuAssertIntEquals(tc, 42, expr->lhs->literal->i_value);

	// root -> rhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_PARENTH, expr->rhs->type);

	struct mcc_ast_expression *subexpr = expr->rhs->expression;

	// subexpr
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, subexpr->type);
	CuAssertIntEquals(tc, MCC_AST_BINARY_OP_ADD, subexpr->op);

	// subexpr -> lhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, subexpr->lhs->type);

	// subexpr -> lhs -> literal
	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, subexpr->lhs->literal->type);
	CuAssertIntEquals(tc, 192, subexpr->lhs->literal->i_value);

	// subexpr -> rhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, subexpr->rhs->type);

	// subexpr -> rhs -> literal
	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_FLOAT, subexpr->rhs->literal->type);
	CuAssertDblEquals(tc, 3.14, subexpr->rhs->literal->f_value, EPS);

	mcc_parser_delete_result(result);
}

void MissingClosingParenthesis_1(CuTest *tc)
{
	const char input[] = "int main(){(42; return 0;}";
	struct mcc_parser_result result = mcc_parse_string(input);

	CuAssertTrue(tc, MCC_PARSER_STATUS_OK != result.status);
	mcc_parser_delete_result(result);
}

void SourceLocation_SingleLineColumn(CuTest *tc)
{
	const char input[] = "int main(){(42 + 192);}";
	struct mcc_parser_result result = mcc_parse_string(input);

	CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

	struct mcc_ast_expression *expr = result.program->statements->compound_statement->expression;

	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_PARENTH, expr->type);
	CuAssertIntEquals(tc, 12, expr->node.sloc.start_col);

	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->expression->type);
	CuAssertIntEquals(tc, 13, expr->expression->node.sloc.start_col);

	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, expr->expression->lhs->literal->type);
	CuAssertIntEquals(tc, 13, expr->expression->lhs->literal->node.sloc.start_col);

	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, expr->expression->rhs->literal->type);
	CuAssertIntEquals(tc, 18, expr->expression->rhs->literal->node.sloc.start_col);

	mcc_parser_delete_result(result);
}

void ExampleFilesTest(CuTest *tc)
{
	int number_test_files = sizeof(test_files) / sizeof(char *);

	// loop over all example files
	for (int i = 0; i < number_test_files; i++) {

		FILE *in = fopen(test_files[i], "r");
		if (!in) {
			printf("ERROR OPENING FILE: %s ", test_files[i]);
			tc->failed = EXIT_FAILURE;
			return;
		}

		struct mcc_parser_result result = mcc_parse_file(in);

		// close file
		fclose(in);

		// check if status is ok for all files
		CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);
		mcc_parser_delete_result(result);

		// print to testlog.json
		printf("Parsing status of file: %s = %d \n", test_files[i], result.status);
	}
}

#define TESTS \
	TEST(FunctionTestMain) \
	TEST(FunctionTestFloat) \
	TEST(FunctionTestString) \
	TEST(FunctionTestBool) \
	TEST(AssociativityTest_1) \
	TEST(AssociativityTest_2) \
	TEST(AssociativityTest_3) \
	TEST(AssociativityTest_4) \
	TEST(AssociativityTest_5) \
	TEST(AssociativityTest_6) \
	TEST(FunctionCallTest) \
	TEST(OperatorPrecedence_1) \
	TEST(OperatorPrecedence_2) \
	TEST(BinaryOp_1) \
	TEST(MissingClosingParenthesis_1) \
	TEST(NestedExpression_1) \
	TEST(SourceLocation_SingleLineColumn) \
	TEST(ExampleFilesTest)

#include "main_stub.inc"
#undef TESTS
