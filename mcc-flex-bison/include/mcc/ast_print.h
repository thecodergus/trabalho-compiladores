// AST Print Infrastructure
//
// This module provides basic printing infrastructure for the AST data
// structure. The DOT printer enables easy visualisation of an AST.

#ifndef MCC_AST_PRINT_H
#define MCC_AST_PRINT_H

#include <stdio.h>

#include "mcc/ast.h"

const char *mcc_ast_print_binary_op(enum mcc_ast_binary_op op);

// ---------------------------------------------------------------- DOT Printer

void mcc_ast_print_dot_expression(FILE *out, struct mcc_ast_expression *expression);

void mcc_ast_print_dot_declaration(FILE *out, struct mcc_ast_declaration *declaration);

void mcc_ast_print_dot_assignment(FILE *out, struct mcc_ast_assignment *assignment);

void mcc_ast_print_dot_statement(FILE *out, struct mcc_ast_statement *statement);

void mcc_ast_print_dot_literal(FILE *out, struct mcc_ast_literal *literal);

void mcc_ast_print_dot_program(FILE *out, struct mcc_ast_function_definition *function_definition);

#endif // MCC_AST_PRINT_H
