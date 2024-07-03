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

const char *tipo_para_str_(enum TipoDado tipo) {
  switch (tipo) {
    case Int:
      return "int";
      break;
    case Float:
      return "float";
      break;
    case String:
      return "string";
      break;
    case Void:
      return "void";
      break;
    default:
      return NULL;
      break;
  }
}

const char *tipo_para_java_argumento(enum TipoDado t) {
  switch (t) {
    case Int:
      return "I";
      break;
    case Float:
      return "F";
      break;
    case String:
      return "Ljava/lang/String;";
      break;
    case Void:
      return "V";
      break;
    default:
      return NULL;
      break;
  }
}

const char *tipo_para_java(enum TipoDado t) {
  switch (t) {
    case Int:
      return "i";
      break;
    case Float:
      return "f";
      break;
    case String:
      return "a";
      break;
    case Void:
    default:
      return NULL;
      break;
  }
}