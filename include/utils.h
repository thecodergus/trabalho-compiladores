#ifndef CCDA0A80_6C57_42F9_805F_FE00EACEF858
#define CCDA0A80_6C57_42F9_805F_FE00EACEF858

#include "ast.h"
#include "libraries/cvector.h"
#include "libraries/str.h"

void imprimir_arvore_estilo_arvore(AST* arvore, int nivel);

char* tipo_dado_str(enum TipoDados tipo);

void printar_folhas(AST* arvore);

void imprimir_token(Token token, int nivel);

void imprimir_traco(int nivel);

char* operador_logico_str(enum LogicalOperators op);

char* operador_aritmetico_str(enum ArithmeticOperators op);

char* operador_relacional_str(enum RelationsOperators op);

#endif /* CCDA0A80_6C57_42F9_805F_FE00EACEF858 */
