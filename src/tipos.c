#include "tipos.h"

const char *tipo_para_str(enum TipoDado t) {
  switch (t) {
  case Int:
    return "Inteiro";
    break;
  case Float:
    return "Flutuante";
    break;
  case String:
    return "String";
    break;
  case Void:
    return "Void";
    break;
  default:
    return NULL;
    break;
  }
}