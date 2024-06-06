#ifndef CCDA0A80_6C57_42F9_805F_FE00EACEF858
#define CCDA0A80_6C57_42F9_805F_FE00EACEF858

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

char* operador_logico_str_original(enum LogicalOperators op);

char* operador_aritmetico_str_original(enum ArithmeticOperators op);

char* operador_relacional_str_original(enum RelationsOperators op);

char* get_substring_before_delimiter(char* str, const char* delimiters);

void imprimir_codigo(AST* arvore);

char* get_id_from_ID(AST* id);

char* tipo_dado_str_original(enum TipoDados tipo);

float intToFloat(int num);

str intToString(int num);

float stringToFloat(str string);

int stringToInt(str string);

int floatToInt(float num);

str floatToString(float num);

#endif /* CCDA0A80_6C57_42F9_805F_FE00EACEF858 */
