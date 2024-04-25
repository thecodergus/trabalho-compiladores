#include <stdio.h>
#include <stdlib.h>

#include "bison.tab.h"

extern FILE *yyin;

int main(int argc, char *argv[]) {
  FILE *arquivo = fopen("../teste.txt", "r");
  if (!arquivo) {
    printf("Erro ao abrir arquivo %s\n", argv[1]);
    return 1;
  }

  yyin = arquivo;
  yyparse();

  fclose(arquivo);
  return 0;
}
