#include "ast.h"

void percorrer(AST *no, void (*fn)(AST *)) {
  if (no && fn) {
    fn(no);

    switch (no->tipo) {
    case Programa: {
      for (AST **it = cvector_begin(no->programa.funcoes);
           it != cvector_end(no->programa.funcoes); it++) {
        percorrer(*it, fn);
      }
      percorrer(no->programa.bloco, fn);
    } break;
    case Funcao: {
      for (AST **it = cvector_begin(no->funcao.parametros);
           it != cvector_end(no->funcao.parametros); it++) {
        percorrer(*it, fn);
      }
      percorrer(no->funcao.bloco, fn);

    } break;
    case Parametro: {

    } break;
    case Bloco: {

      for (AST **it = cvector_begin(no->bloco.declaracoes);
           it != cvector_end(no->bloco.declaracoes); it++) {
        percorrer(*it, fn);
      }

      for (AST **it = cvector_begin(no->bloco.comandos);
           it != cvector_end(no->bloco.comandos); it++) {
        percorrer(*it, fn);
      }

    } break;
    case Atribuicao: {
      percorrer(no->atribuicao.expressao, fn);

    } break;
    case If: {

      for (AST **it = cvector_begin(no->if_.blocoIf);
           it != cvector_end(no->if_.blocoIf); it++) {
        percorrer(*it, fn);
      }

      for (AST **it = cvector_begin(no->if_.blocoElse);
           it != cvector_end(no->if_.blocoElse); it++) {
        percorrer(*it, fn);
      }

    } break;
    case While: {

      for (AST **it = cvector_begin(no->while_.bloco);
           it != cvector_end(no->while_.bloco); it++) {
        percorrer(*it, fn);
      }

    } break;
    case Retorno: {
      percorrer(no->retorno.ret, fn);

    } break;
    case Print: {

    } break;
    case Read: {

    } break;
    case ChamadaFuncao: {

      for (AST **it = cvector_begin(no->chamada_funcao.parametros);
           it != cvector_end(no->chamada_funcao.parametros); it++) {
        percorrer(*it, fn);
      }

    } break;
    case ExpressaoRelacional: {
      percorrer(no->relacional.esquerda, fn);
      percorrer(no->relacional.direita, fn);

    } break;
    case ExpressaoLogica: {
      percorrer(no->logica.esquerda, fn);
      percorrer(no->logica.direita, fn);

    } break;
    case ExpressaoAritmetica: {
      percorrer(no->aritmetica.esquerda, fn);
      percorrer(no->aritmetica.direita, fn);

    } break;
    case ConsanteInt: {

    } break;
    case ConsanteFloat: {

    } break;
    case ConsanteString: {

    } break;
    case Id: {

    } break;
    case SituacaoTransicao: {

    } break;
    case Desconhecido: {

    } break;

    default:
      break;
    }
  }
}

const char *tipoToken_para_str(enum TipoToken token) {
  switch (token) {
  case Programa:
    return "Programa";
    break;
  case Funcao:
    return "Funcao";
    break;
  case Parametro:
    return "Parametro";
    break;
  case Bloco:
    return "Bloco";
    break;
  case Atribuicao:
    return "Atribuicao";
    break;
  case If:
    return "If";
    break;
  case While:
    return "While";
    break;
  case Retorno:
    return "Retorno";
    break;
  case Print:
    return "Print";
    break;
  case Read:
    return "Read";
    break;
  case ChamadaFuncao:
    return "Chamada de Funcao";
    break;
  case ExpressaoRelacional:
    return "Expressao Relacional";
    break;
  case ExpressaoLogica:
    return "Expressao Logica";
    break;
  case ExpressaoAritmetica:
    return "Expressao Aritmetica";
    break;
  case Tipo:
    return "Tipo";
    break;
  case ConsanteInt:
    return "Consante Int";
    break;
  case ConsanteFloat:
    return "Consante Flutuante";
    break;
  case ConsanteString:
    return "Consante String";
    break;
  case Id:
    return "Id";
    break;
  case Variavel:
    return "Variavel";
    break;
  case Desconhecido:
    return "Desconhecido";
    break;

  default:
    break;
  }

  return NULL;
}

void exibir_arvore(AST *no, int profundidade) {

  if (no) {

    printf("{%d}%s{%s}\n", profundidade, repeat_char(profundidade),
           tipoToken_para_str(no->tipo));

    switch (no->tipo) {
    case Programa: {
      for (AST **it = cvector_begin(no->programa.funcoes);
           it != cvector_end(no->programa.funcoes); it++) {
        exibir_arvore(*it, profundidade + 1);
      }
      exibir_arvore(no->programa.bloco, profundidade + 1);
    } break;
    case Funcao: {
      for (AST **it = cvector_begin(no->funcao.parametros);
           it != cvector_end(no->funcao.parametros); it++) {
        exibir_arvore(*it, profundidade + 1);
      }
      exibir_arvore(no->funcao.bloco, profundidade + 1);

    } break;
    case Bloco: {
      for (AST **it = cvector_begin(no->bloco.declaracoes);
           it != cvector_end(no->bloco.declaracoes); it++) {
        exibir_arvore(*it, profundidade + 1);
      }

      for (AST **it = cvector_begin(no->bloco.comandos);
           it != cvector_end(no->bloco.comandos); it++) {
        exibir_arvore(*it, profundidade + 1);
      }

    } break;
    case Atribuicao: {
      exibir_arvore(no->atribuicao.expressao, profundidade + 1);

    } break;
    case If: {

      for (AST **it = cvector_begin(no->if_.blocoIf);
           it != cvector_end(no->if_.blocoIf); it++) {
        exibir_arvore(*it, profundidade + 1);
      }

      for (AST **it = cvector_begin(no->if_.blocoElse);
           it != cvector_end(no->if_.blocoElse); it++) {
        exibir_arvore(*it, profundidade + 1);
      }

    } break;
    case While: {

      for (AST **it = cvector_begin(no->while_.bloco);
           it != cvector_end(no->while_.bloco); it++) {
        exibir_arvore(*it, profundidade + 1);
      }

    } break;
    case Retorno: {
      exibir_arvore(no->retorno.ret, profundidade + 1);

    } break;
    case ChamadaFuncao: {

      for (AST **it = cvector_begin(no->chamada_funcao.parametros);
           it != cvector_end(no->chamada_funcao.parametros); it++) {
        exibir_arvore(*it, profundidade + 1);
      }

    } break;
    case ExpressaoRelacional: {
      exibir_arvore(no->relacional.esquerda, profundidade + 1);
      exibir_arvore(no->relacional.direita, profundidade + 1);

    } break;
    case ExpressaoLogica: {
      exibir_arvore(no->logica.esquerda, profundidade + 1);
      exibir_arvore(no->logica.direita, profundidade + 1);

    } break;
    case ExpressaoAritmetica: {
      exibir_arvore(no->aritmetica.esquerda, profundidade + 1);
      exibir_arvore(no->aritmetica.direita, profundidade + 1);

    } break;

    default:
      break;
    }
  }
}

char *repeat_char(int n) {
  if (n <= 0) {
    return "";
  }

  char *str = malloc(n + 1); // +1 for the null terminator
  if (!str) {
    return NULL; // Failed to allocate memory
  }

  memset(str, '-', n);
  str[n] = '\0'; // Null terminate the string

  return str;
}