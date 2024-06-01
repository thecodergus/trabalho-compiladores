#include "mcc/cfg.h"

#include <assert.h>
#include <string.h>

static void print_dot_begin(FILE *out)
{
	assert(out);

	fprintf(out, "digraph \"CFG\" {\n"
	             "\tnodesep=0.6\n");
}

static void print_dot_end(FILE *out)
{
	assert(out);

	fprintf(out, "}\n");
}

static void print_dot_node(FILE *out, const void *node, const char *label)
{
	assert(out);
	assert(node);
	assert(label);

	fprintf(out, "\t\"%p\" [shape=box, xlabel=\"%s\", label=\"", node, label);
}

static void print_dot_edge(FILE *out, const void *src_node, const void *dst_node, const char *label)
{
	assert(out);
	assert(src_node);
	assert(dst_node);
	assert(label);

	fprintf(out, "\t\"%p\" -> \"%p\" [label=\"%s\"];\n", src_node, dst_node, label);
}

static void print_instruction(FILE *out, struct mcc_tac_instruction *instruction)
{
	switch (instruction->statement_type) {
	case MCC_TAC_STATEMENT_TYPE_OPERATION_COPY_INT:
		fprintf(out, "%s = %s", instruction->result, instruction->argument1);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_COPY_FLOAT:
		fprintf(out, "%s = %s", instruction->result, instruction->argument1);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_COPY_STRING:
		fprintf(out, "%s = %s", instruction->result, instruction->argument1);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_JUMP:
		fprintf(out, "jump %s", instruction->argument1);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_IF:
		fprintf(out, "if %s jump %s", instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_UNARY_NOT:
		fprintf(out, "%s = !%s", instruction->result, instruction->argument1);
		break;
	case MCC_TAC_STATEMENT_TYPE_UNARY_MINUS_INT:
		fprintf(out, "%s = -%s", instruction->result, instruction->argument1);
		break;
	case MCC_TAC_STATEMENT_TYPE_UNARY_MINUS_FLOAT:
		fprintf(out, "%s = -%s", instruction->result, instruction->argument1);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_PLUS_INT:
		fprintf(out, "%s = %s + %s", instruction->result, instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_PLUS_FLOAT:
		fprintf(out, "%s = %s + %s", instruction->result, instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_MINUS_INT:
		fprintf(out, "%s = %s - %s", instruction->result, instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_MINUS_FLOAT:
		fprintf(out, "%s = %s - %s", instruction->result, instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_MULTIPLY_INT:
		fprintf(out, "%s = %s * %s", instruction->result, instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_MULTIPLY_FLOAT:
		fprintf(out, "%s = %s * %s", instruction->result, instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_DIVISION_INT:
		fprintf(out, "%s = %s / %s", instruction->result, instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_DIVISION_FLOAT:
		fprintf(out, "%s = %s / %s", instruction->result, instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_LT_INT:
		fprintf(out, "%s = %s < %s", instruction->result, instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_LT_FLOAT:
		fprintf(out, "%s = %s < %s", instruction->result, instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_GT_INT:
		fprintf(out, "%s = %s > %s", instruction->result, instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_GT_FLOAT:
		fprintf(out, "%s = %s > %s", instruction->result, instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_LEQ_INT:
		fprintf(out, "%s = %s <= %s", instruction->result, instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_LEQ_FLOAT:
		fprintf(out, "%s = %s <= %s", instruction->result, instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_GEQ_INT:
		fprintf(out, "%s = %s >= %s", instruction->result, instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_GEQ_FLOAT:
		fprintf(out, "%s = %s >= %s", instruction->result, instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_EQ_INT:
		fprintf(out, "%s = %s == %s", instruction->result, instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_EQ_FLOAT:
		fprintf(out, "%s = %s == %s", instruction->result, instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_NEQ_INT:
		fprintf(out, "%s = %s != %s", instruction->result, instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_NEQ_FLOAT:
		fprintf(out, "%s = %s != %s", instruction->result, instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_AND:
		fprintf(out, "%s = %s && %s", instruction->result, instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_BINARY_OR:
		fprintf(out, "%s = %s || %s", instruction->result, instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_POP_INT:
		fprintf(out, "pop %s\n", instruction->result);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_POP_FLOAT:
		fprintf(out, "pop %s\n", instruction->result);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_POP_STRING:
		fprintf(out, "pop %s\n", instruction->result);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_DECLARATION_ARR_INT:
		fprintf(out, "%s = INT[%s]", instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_DECLARATION_ARR_FLOAT:
		fprintf(out, "%s = FLOAT[%s]", instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_DECLARATION_ARR_STRING:
		fprintf(out, "%s = STRING[%s]", instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_ARR_SIZE:
		fprintf(out, "%s = arr_size(%s)", instruction->result, instruction->argument1);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_MUL_INT:
		fprintf(out, "%s = %s * %s", instruction->result, instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_ADD_INT:
		fprintf(out, "%s = %s + %s", instruction->result, instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_VOID:
		fprintf(out, "ret");
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_INT:
		fprintf(out, "ret %s", instruction->argument1);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_FLOAT:
		fprintf(out, "ret %s", instruction->argument1);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_RETURN_STRING:
		fprintf(out, "ret %s", instruction->argument1);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_LOAD_ARRAY_VALUE:
		fprintf(out, "%s = %s[]", instruction->result, instruction->argument1);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_STORE_ARRAY_VALUE:
		fprintf(out, "%s[] = %s", instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_INT:
		fprintf(out, "push %s", instruction->argument1);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_FLOAT:
		fprintf(out, "push %s", instruction->argument1);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_STRING:
		fprintf(out, "push %s", instruction->argument1);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_ARRAY_INT:
		fprintf(out, "push %s", instruction->argument1);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_ARRAY_FLOAT:
		fprintf(out, "push %s", instruction->argument1);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_PUSH_ARRAY_STRING:
		fprintf(out, "push %s", instruction->argument1);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_PROCEDURE_CALL:
		fprintf(out, "call %s, %s", instruction->argument1, instruction->argument2);
		break;
	case MCC_TAC_STATEMENT_TYPE_OPERATION_PARAMETER_DECLARATION:
		fprintf(out, "pop %s", instruction->argument1);
		break;

	default:
		fprintf(out, "unknown instruction");
		break;
	}
}

static void print_block_content(FILE *out, struct mcc_tac_basic_block *block)
{
	struct mcc_tac_instruction *instruction = block->first_instruction;
	if (block->label) {
		print_dot_node(out, block, block->label);
	} else {
		print_dot_node(out, block, "");
	}

	while (instruction != NULL) {
		print_instruction(out, instruction);
		fprintf(out, "\\l");
		instruction = instruction->next_instruction;
	}

	// print end of node
	fprintf(out, "\"];\n");
}

static void print_block(FILE *out, struct mcc_tac_basic_block *block, struct mcc_tac_basic_block *next_block)
{

	print_block_content(out, block);

	// children of block
	struct mcc_tac_basic_block *child = block->child;
	int loop_counter = 0;
	while (child != NULL) {
		print_block(out, child, block->next_block);

		if (loop_counter == 0) {
			print_dot_edge(out, block, child, "true");
		} else if (loop_counter == 1) {
			print_dot_edge(out, block, child, "false");
		}

		child = child->next_child;
		loop_counter++;
	}

	if (loop_counter == 1) {
		print_dot_edge(out, block, block->next_block, "false");
	}

	if (block->jump_block != NULL) {
		print_dot_edge(out, block, block->jump_block, "jump");
	} else if (block->next_block != NULL) {
		print_block(out, block->next_block, next_block);
		if (block->child == NULL) {
			print_dot_edge(out, block, block->next_block, "");
		}
	} else if (next_block != NULL) {
		if (block->child == NULL) {
			print_dot_edge(out, block, next_block, "");
		}
	}
}

void mcc_print_cfg(FILE *out, struct mcc_tac_function_blocks *blocks, char *function_name)
{

	assert(blocks);

	print_dot_begin(out);

	if (function_name != NULL) {

		struct mcc_tac_function_blocks *function = blocks;

		while (function != NULL) {

			if (strcmp(function->function_name, function_name) == 0) {
				print_block(out, function->first_basic_block, NULL);
				print_dot_end(out);
				return;
			}

			function = function->next_function;
		}

	} else {

		while (blocks != NULL) {
			print_block(out, blocks->first_basic_block, NULL);
			blocks = blocks->next_function;
		}
	}

	print_dot_end(out);
}
