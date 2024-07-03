#ifndef TIPOS_H
#define TIPOS_H

#include <stdlib.h>
#include <string.h>

enum TipoDado { Int, Float, String, Void, SemTipo };

const char *tipo_para_str(enum TipoDado t);
const char *tipo_para_str_(enum TipoDado tipo);
const char *tipo_para_java_argumento(enum TipoDado t);
const char *tipo_para_java(enum TipoDado t);

#endif