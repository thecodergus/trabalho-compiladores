#ifndef DE9912FB_2382_4387_8C42_BD3C683D6A32
#define DE9912FB_2382_4387_8C42_BD3C683D6A32

#include <stdio.h>
#include <stdlib.h>
#include <utils/cvector.h>

#define vector(T) cvector_vector_type(T)

enum Tipo { Arvore, Vetor };

typedef struct AST AST;

struct AST {
  enum Tipo tipo;
  union {
    struct {
      struct AST* left;
      struct AST* right;
    } arvore;
    vector(struct AST*) filhos;
  } u;
};

#endif