#ifndef CCDA0A80_6C57_42F9_805F_FE00EACEF858
#define CCDA0A80_6C57_42F9_805F_FE00EACEF858

#include "ast.h"
#include "libraries/cvector.h"
#include "libraries/str.h"

void imprimir_arvore(AST* arvore, int nivel);

char* tipo_dado_str(enum TipoDados tipo);

void printar_folhas(AST* arvore);

void imprimir_token(struct Token token, int nivel)

#endif /* CCDA0A80_6C57_42F9_805F_FE00EACEF858 */
