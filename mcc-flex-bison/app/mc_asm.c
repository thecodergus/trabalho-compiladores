#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc/assembly.h"
#include "mcc/ast.h"
#include "mcc/parser.h"
#include "mcc/semantic_checks.h"
#include "mcc/symbol_table.h"
#include "mcc/symbol_table_print.h"
#include "mcc/tac.h"

void print_usage(const char *prg)
{
	printf("\n");
	printf("usage: %s <FILE>\n\n", prg);
	printf("Utility for printing the generated assembly code. Errors are reported on invalid inputs.\n\n");
	printf("Use '-' as input file to read from stdin.\n\n");
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

	struct mcc_ast_function_definition *program = NULL;

	// ########## PARSING PHASE ##########
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
	
	// ########## OUTPUT ASSEMBLY CODE ##########
	struct mcc_asm_program_contents *assembly = mcc_asm_code_generation(out_file, tac);

	// ########## CLEANUP ##########
	mcc_assembly_delete_asm(assembly);
	mcc_tac_delete_tac(tac);
	mcc_symbol_table_delete_table(&symbol_table);
	mcc_ast_delete_function_definition(program);
	free(semantic_error);

	return EXIT_SUCCESS;
}
