#include "mensagens.h"

void exibir_warning(const char *msg) {
  if (msg) {
    printf(YELLOW "[AVISO]:\t%s\t[\AVISO]\n" RESET, msg);
  }
}

void exibir_erro(const char *msg) {
  if (msg) {
    printf(RED "[ERRO]:\t%s\t[\ERRO]\n" RESET, msg);
  }
}
