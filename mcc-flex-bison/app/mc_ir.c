/*

Utility for viewing the generated intermediate representation.
Errors are reported on invalid inputs.

*/

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc/ast.h"
#include "mcc/parser.h"
#include "mcc/semantic_checks.h"
#include "mcc/symbol_table.h"
#include "mcc/symbol_table_print.h"
#include "mcc/tac.h"

static char *mcc_get_instruction_type_string(enum mcc_tac_statement_type instruction_type)
{

	switch (instruction_type) {

	case MCC_TAC_STATEMENT_TYPE_OPERATION_COPY_INT:
		return "copy [int]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_COPY_FLOAT:
		return "copy [float]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_COPY_STRING:
		return "copy [string]";

	case MCC_TAC_STATEMENT_TYPE_UNARY_NOT:
		return "! (unary)";

	case MCC_TAC_STATEMENT_TYPE_UNARY_MINUS_INT:
		return "- (unary) [int]";

	case MCC_TAC_STATEMENT_TYPE_UNARY_MINUS_FLOAT:
		return "- (unary) [float]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_PLUS_INT:
		return "+ [int]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_PLUS_FLOAT:
		return "+ [float]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_MINUS_INT:
		return "- [int]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_MINUS_FLOAT:
		return "- [float]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_MULTIPLY_INT:
		return "* [int]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_MULTIPLY_FLOAT:
		return "* [float]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_DIVISION_INT:
		return "/ [int]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_DIVISION_FLOAT:
		return "/ [float]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_LT_INT:
		return "< [int]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_LT_FLOAT:
		return "< [float]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_GT_INT:
		return "> [int]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_GT_FLOAT:
		return "> [float]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_LEQ_INT:
		return "<= [int]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_LEQ_FLOAT:
		return "<= [float]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_GEQ_INT:
		return ">= [int]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_GEQ_FLOAT:
		return ">= [float]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_EQ_INT:
		return "== [int]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_EQ_FLOAT:
		return "== [float]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_NEQ_INT:
		return "!= [int]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_NEQ_FLOAT:
		return "!= [float]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_AND:
		return "&&";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_OR:
		return "||";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_POP_INT:
		return "pop [int]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_POP_FLOAT:
		return "pop [float]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_POP_STRING:
		return "pop [string]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_DECLARATION_ARR_INT:
		return "array declaration [int]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_DECLARATION_ARR_FLOAT:
		return "array declaration [float]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_DECLARATION_ARR_STRING:
		return "array declaration [string]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_ARR_SIZE:
		return "array size";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_MUL_INT:
		return "* (non binary op mul)";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_ADD_INT:
		return "+ (non binary op add)";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_VOID:
		return "return void";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_INT:
		return "return int";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_FLOAT:
		return "return float";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_STRING:
		return "return string";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_LOAD_ARRAY_VALUE:
		return "load array value";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_STORE_ARRAY_VALUE:
		return "store array value";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_INT:
		return "parameter push [int]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_FLOAT:
		return "parameter push [float]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_STRING:
		return "parameter push [string]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_ARRAY_INT:
		return "parameter push array [int]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_ARRAY_FLOAT:
		return "parameter push array [float]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_ARRAY_STRING:
		return "parameter push array [string]";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_PROCEDURE_CALL:
		return "procedure call";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_DECLARATION:
		return "parameter declaration";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_JUMP:
		return "jump";

	case MCC_TAC_STATEMENT_TYPE_OPERATION_IF:
		return "condition";

	default:
		return "missing operator in print tac";
	}
}

static void mcc_tac_print_instruction(FILE *out, struct mcc_tac_instruction *instruction)
{

	char *arg1 = "-";
	char *arg2 = "-";
	char *res = "-";

	if (instruction->argument1 != NULL) {
		arg1 = instruction->argument1;
	}

	if (instruction->argument2 != NULL) {
		arg2 = instruction->argument2;
	}

	if (instruction->result != NULL) {
		res = instruction->result;
	}

	fprintf(out, "| \t%-25s\t | \t%-20s\t | \t%-10s | \t%-10s|\n",
	        mcc_get_instruction_type_string(instruction->statement_type), arg1, arg2, res);
}

static void mcc_tac_print_block(FILE *out, struct mcc_tac_basic_block *block)
{

	struct mcc_tac_instruction *iter_instruction = block->first_instruction;

	fprintf(out,
	        "|=============================================== BLOCK %-6s "
	        "============================================|\n",
	        block->label);
	fprintf(out, "| \t\toperation\t\t | \t\targ 1\t\t | \targ 2\t   | \tresult\t  |\n");
	fprintf(out, "|------------------------------------------------------------------------------------------------"
	             "---------|\n");

	while (iter_instruction != NULL) {

		mcc_tac_print_instruction(out, iter_instruction);

		iter_instruction = iter_instruction->next_instruction;
	}

	fprintf(out, "|================================================================================================"
	             "=========|\n");

	struct mcc_tac_basic_block *child_block = block->child;

	while (child_block != NULL) {
		mcc_tac_print_block(out, child_block);
		child_block = child_block->next_child;
	}

	if (block->next_block != NULL) {
		mcc_tac_print_block(out, block->next_block);
	}
}

void mcc_tac_print_tac(FILE *out, struct mcc_tac_function_blocks *tac)
{
	struct mcc_tac_function_blocks *current_block = tac;

	while (current_block != NULL) {
		fprintf(out, "%s: \n", current_block->function_name);
		mcc_tac_print_block(out, current_block->first_basic_block);
		current_block = current_block->next_function;
	}
}

void print_usage(const char *prg)
{
	printf("\n");
	printf("usage: %s [OPTIONS] <FILE>\n\n", prg);
	printf("Utility for viewing the generated intermediate representation.Errors are reported on invalid inputs. "
	       "\n\n\n");
	printf("OPTIONS:\n");
	printf("-h, --help\t\t\tdisplay this help message\n");
	printf("-o, --output <out-file>\t\twrite the output to <out-file> (defaults to 'stdout')\n\n");
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		print_usage(argv[0]);
		return EXIT_FAILURE;
	}

	// ########## COMMAND LINE ARGUMENT PARSING ##########

	static struct option long_options[] = {{"help", no_argument, NULL, 'h'},
	                                       {"output", required_argument, NULL, 'o'}};

	int opt;
	FILE *out_file = stdout;

	while ((opt = getopt_long(argc, argv, "ho:", long_options, NULL)) != -1) {
		switch (opt) {
		case 'h':
			print_usage(argv[0]);
			return EXIT_SUCCESS;
		case 'o':
			out_file = fopen(optarg, "w");
			if (!out_file) {
				perror("fopen");
				return EXIT_FAILURE;
			}
			break;
		default:
			print_usage(argv[0]);
			return EXIT_FAILURE;
		}
	}

	// ########## DETERMINE INPUT SOURCE ##########
	FILE *in;
	if (optind >= argc) {
		print_usage(argv[0]);
		return EXIT_FAILURE;
	} else if (strcmp("-", argv[optind]) == 0) {
		in = stdin;
	} else {
		in = fopen(argv[optind], "r");

		if (!in) {
			printf("File %s could not be opened\n", argv[optind]);
			return EXIT_FAILURE;
		}
	}

	// ########## PARSING PHASE ##########
	struct mcc_ast_function_definition *program = NULL;

	{
		struct mcc_parser_result result = mcc_parse_file(in);
		fclose(in);

		if (result.status != MCC_PARSER_STATUS_OK) {

			if (result.status == MCC_PARSER_STATUS_UNABLE_TO_OPEN_STREAM) {
				fprintf(stderr, "ERROR: unable to open stream \n");
			} else {
				fprintf(stderr, "Parsing error at %s", result.error_message);
			}

			free(result.error_message);
			return EXIT_FAILURE;
		}

		program = result.program;
	}

	// ########## RUN SEMANTIC CHECKS ##########
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);
	struct mcc_semantic_error *semantic_error = mcc_check_semantics(program, symbol_table);

	if (semantic_error->hasError) {

		if (semantic_error->node) {
			fprintf(stderr, "[Semantic Error]: at %d:%d: %s \n", semantic_error->node->sloc.start_line,
			        semantic_error->node->sloc.start_col, semantic_error->error_msg);
		} else {
			fprintf(stderr, "[Semantic Error]: %s \n", semantic_error->error_msg);
		}

		mcc_symbol_table_delete_table(&symbol_table);
		free(semantic_error);
		return EXIT_FAILURE;
	}


	// ########## CREATE THREE-ADDRESS CODE ##########
	struct mcc_tac_function_blocks *tac = mcc_ast_to_tac(program);

	mcc_tac_print_tac(out_file, tac);

	// ########## CLEANUP ##########
	mcc_tac_delete_tac(tac);
	mcc_symbol_table_delete_table(&symbol_table);
	mcc_ast_delete_function_definition(program);
	free(semantic_error);

	return EXIT_SUCCESS;
}
