#include "ast.h"

void gerar_bytecode(AST* no) {
  // TODO
  // Implementar a geração de bytecode
  // para a linguagem de máquina virtual Java
  // Dica: use a função reescrever_codigo
  // para reescrever a AST em código de máquina
  if (no) {
    switch (no->tipo) {
      case Programa: {
        for (AST** it = cvector_begin(no->programa.funcoes); it != cvector_end(no->programa.funcoes); it++) {
          reescrever_codigo(*it);
        }

        reescrever_codigo(no->programa.bloco);

      } break;
      case Funcao: {
        int i = 0;
        for (AST** it = cvector_begin(no->funcao.parametros); it != cvector_end(no->funcao.parametros); it++, i++) {
          reescrever_codigo(*it);
          if (i < cvector_size(no->funcao.parametros) - 1) {
          }
        }
        reescrever_codigo(no->funcao.bloco);
      } break;
      case Parametro: {
      } break;
      case Bloco: {
        // Declarações
        for (AST** it = cvector_begin(no->bloco.declaracoes); it != cvector_end(no->bloco.declaracoes); it++) {
          reescrever_codigo(*it);
        }

        // Comandos
        for (AST** it = cvector_begin(no->bloco.comandos); it != cvector_end(no->bloco.comandos); it++) {
          reescrever_codigo(*it);
        }
      } break;
      case Atribuicao: {
        reescrever_codigo(no->atribuicao.expressao);
      } break;
      case If: {
        reescrever_codigo(no->if_.codicao);

        // Bloco If
        for (AST** it = cvector_begin(no->if_.comandosIf); it != cvector_end(no->if_.comandosIf); it++) {
          reescrever_codigo(*it);
        }

        // Bloco Else
        if (no->if_.comandosElse) {
          for (AST** it = cvector_begin(no->if_.comandosElse); it != cvector_end(no->if_.comandosElse); it++) {
            reescrever_codigo(*it);
          }
        } else {
        }

      } break;
      case While: {
        reescrever_codigo(no->while_.codicao);
        for (AST** it = cvector_begin(no->while_.bloco); it != cvector_end(no->while_.bloco); it++) {
          reescrever_codigo(*it);
        }
      } break;
      case Retorno: {
        reescrever_codigo(no->retorno.ret);
      } break;
      case Print: {
        reescrever_codigo(no->print.parametro);
      } break;
      case Read: {
      } break;
      case ChamadaFuncao: {
        int i = 0;
        printf("%s(", no->chamada_funcao.id);
        for (AST** it = cvector_begin(no->chamada_funcao.parametros); it != cvector_end(no->chamada_funcao.parametros); it++, i++) {
          reescrever_codigo(*it);

          if (i < cvector_size(no->chamada_funcao.parametros) - 1) {
          }
        }
      } break;
      case ExpressaoRelacional: {
        reescrever_codigo(no->relacional.esquerda);
        reescrever_codigo(no->relacional.direita);
      } break;
      case ExpressaoLogica: {
        if (strcmp("!", no->logica.simbolo) == 0) {
          reescrever_codigo(no->logica.esquerda);
        } else {
          reescrever_codigo(no->logica.esquerda);
          reescrever_codigo(no->logica.direita);
        }
      } break;
      case ExpressaoAritmetica: {
        reescrever_codigo(no->aritmetica.esquerda);
        reescrever_codigo(no->aritmetica.direita);
      } break;
      case ExpressaoAritmeticaConvertida: {
        reescrever_codigo(no->aritmetica_convertida.expr);
      } break;
      case Tipo: {
      } break;
      case ConsanteInt: {
      } break;
      case ConsanteFloat: {
      } break;
      case ConsanteString: {
      } break;
      case Id: {
      } break;
      case Variavel: {
        int i = 0;
        for (AST** it = cvector_begin(no->variavel.ids); it != cvector_end(no->variavel.ids); it++, i++) {
          reescrever_codigo(*it);

          if (i < cvector_size(no->variavel.ids) - 1) {
          }
        }
      } break;
      case Desconhecido: {
      } break;

      default:
        break;
    }
  }
}