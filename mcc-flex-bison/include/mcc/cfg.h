#ifndef MCC_CFG_H
#define MCC_CFG_H

#include <stdio.h>

#include "mcc/ast.h"
#include "mcc/tac.h"

void mcc_print_cfg(FILE *out, struct mcc_tac_function_blocks *blocks, char *function_name);

#endif
