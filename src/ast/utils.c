#include "ast.h"

void percorrer(AST *no, void (*fn)(AST *)) {
  if (no && fn) {
    switch (no->tipo) {
    case Programa: {
      for (AST **it = cvector_begin(no->programa.funcoes);
           it != cvector_end(no->programa.funcoes); it++) {
        percorrer(*it, fn);
      }
      for (AST **it = cvector_begin(no->programa.bloco);
           it != cvector_end(no->programa.bloco); it++) {
        percorrer(*it, fn);
      }
    } break;
    case Funcao: {
      for (AST **it = cvector_begin(no->funcao.parametros);
           it != cvector_end(no->funcao.parametros); it++) {
        percorrer(*it, fn);
      }
      for (AST **it = cvector_begin(no->funcao.bloco);
           it != cvector_end(no->funcao.bloco); it++) {
        percorrer(*it, fn);
      }

    } break;
    case Parametro: {

    } break;
    case Bloco: {

    } break;
    case Atribuicao: {
      percorrer(no->atribuicao.expressao.esquerda, fn);
      percorrer(no->atribuicao.expressao.direita, fn);

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

    } break;
    case ExpressaoLogica: {

    } break;
    case ExpressaoAritmetica: {

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