#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc/ast.h"
#include "mcc/parser.h"
#include "mcc/symbol_table.h"
#include "mcc/symbol_table_print.h"

void print_usage(const char *prg)
{
	printf("usage: %s [OPTIONS] <FILE>\n\n", prg);
	printf("Utility for displaying the generated symbol tables. Errors are reported on invalid inputs. \n\n");
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
	FILE *out_file = NULL;

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

	if (out_file == NULL) {
		out_file = stdout;
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

	// ########## CREATE SYMBOL TABLE ##########
	struct mcc_symbol_table *symbol_table = mcc_symbol_table_create(program);
	mcc_print_symbol_table(out_file, symbol_table);

	// ########## CLEANUP ##########
	mcc_symbol_table_delete_table(&symbol_table);
	mcc_ast_delete_function_definition(program);

	return EXIT_SUCCESS;
}
