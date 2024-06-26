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
      percorrer(&no->programa.bloco, fn);
    } break;
    case Funcao: {
      for (AST **it = cvector_begin(no->funcao.parametros);
           it != cvector_end(no->funcao.parametros); it++) {
        percorrer(*it, fn);
      }
      percorrer(&no->funcao.bloco, fn);

    } break;
    case Parametro: {

    } break;
    case Bloco: {

      for (AST **it = cvector_begin(no->bloco.comandos);
           it != cvector_end(no->bloco.comandos); it++) {
        percorrer(*it, fn);
      }

      for (AST **it = cvector_begin(no->bloco.declaracoes);
           it != cvector_end(no->bloco.declaracoes); it++) {
        percorrer(*it, fn);
      }

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