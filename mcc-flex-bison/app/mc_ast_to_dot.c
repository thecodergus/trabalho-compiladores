#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc/ast.h"
#include "mcc/ast_print.h"
#include "mcc/parser.h"

void print_usage()
{
	printf("usage: mc_ast_to_dot [OPTIONS] <file>\n\n");
	printf("Utility for printing an abstract syntax tree in the DOT format. The output can be visualised using "
	       "Graphviz. Errors are reported on invalid inputs. \n\n\n");
	printf("OPTIONS:\n");
	printf("-h, --help\t\t\tdisplay this help message\n");
	printf("-o, --output <out-file>\t\twrite the output to <out-file> (defaults to 'stdout')\n\n");
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		print_usage();
		return EXIT_FAILURE;
	}

	// ########## COMMAND LINE ARGUMENT PARSING ##########
	static struct option long_options[] = {{"help", no_argument, NULL, 'h'},
	                                       {"output", required_argument, NULL, 'o'}};

	int opt;
	FILE *out = NULL;

	while ((opt = getopt_long(argc, argv, "ho:", long_options, NULL)) != -1) {
		switch (opt) {
		case 'h':
			print_usage();
			return EXIT_SUCCESS;
		case 'o':
			out = fopen(optarg, "w");
			if (!out) {
				perror("fopen");
				return EXIT_FAILURE;
			}
			break;
		default:
			print_usage();
			return EXIT_FAILURE;
		}
	}

	if (out == NULL) {
		out = stdout;
	}

	// ########## DETERMINE INPUT SOURCE ##########
	FILE *in;
	if (optind >= argc) {
		print_usage();
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

	mcc_ast_print_dot_program(out, program);

	// ########## CLEANUP ##########
	mcc_ast_delete_function_definition(program);

	return EXIT_SUCCESS;
}
