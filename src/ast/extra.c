#include "ast.h"

void reescrever_codigo(AST* no) {
  if (no) {
    switch (no->tipo) {
      case Programa: {
        for (AST** it = cvector_begin(no->programa.funcoes); it != cvector_end(no->programa.funcoes); it++) {
          reescrever_codigo(*it);
        }

        printf("{\n");
        reescrever_codigo(no->programa.bloco);
        printf("}\n");

      } break;
      case Funcao: {
        int i = 0;
        printf("%s %s(", tipo_para_str_(no->funcao.retorno), no->funcao.id);
        for (AST** it = cvector_begin(no->funcao.parametros); it != cvector_end(no->funcao.parametros); it++, i++) {
          reescrever_codigo(*it);
          if (i < cvector_size(no->funcao.parametros) - 1) {
            printf(",");
          }
        }
        printf("){\n");
        reescrever_codigo(no->funcao.bloco);
        printf("}\n");
      } break;
      case Parametro: {
        printf("%s %s", tipo_para_str_(no->parametro.tipo), no->parametro.id);
      } break;
      case Bloco: {
        // Declarações
        for (AST** it = cvector_begin(no->bloco.declaracoes); it != cvector_end(no->bloco.declaracoes); it++) {
          reescrever_codigo(*it);
        }

        // Comandos
        for (AST** it = cvector_begin(no->bloco.comandos); it != cvector_end(no->bloco.comandos); it++) {
          reescrever_codigo(*it);
          printf(";\n");
        }
      } break;
      case Atribuicao: {
        printf("%s = ", no->atribuicao.id);
        reescrever_codigo(no->atribuicao.expressao);
      } break;
      case If: {
        printf("if(");
        reescrever_codigo(no->if_.codicao);
        printf("){\n");

        // Bloco If
        for (AST** it = cvector_begin(no->if_.comandosIf); it != cvector_end(no->if_.comandosIf); it++) {
          reescrever_codigo(*it);
          printf(";\n");
        }

        // Bloco Else
        if (no->if_.comandosElse) {
          printf("}else{");
          for (AST** it = cvector_begin(no->if_.comandosElse); it != cvector_end(no->if_.comandosElse); it++) {
            reescrever_codigo(*it);
            printf(";\n");
          }
          printf("}\n");
        } else {
          printf("}\n");
        }

      } break;
      case While: {
        printf("while(");
        reescrever_codigo(no->while_.codicao);
        printf("){\n");
        for (AST** it = cvector_begin(no->while_.bloco); it != cvector_end(no->while_.bloco); it++) {
          reescrever_codigo(*it);
          printf(";\n");
        }
        printf("}\n");
      } break;
      case Retorno: {
        printf("return ");
        reescrever_codigo(no->retorno.ret);
      } break;
      case Print: {
        printf("print(");
        reescrever_codigo(no->print.parametro);
        printf(")");
      } break;
      case Read: {
        printf("read(%s)", no->read.id);
      } break;
      case ChamadaFuncao: {
        int i = 0;
        printf("%s(", no->chamada_funcao.id);
        for (AST** it = cvector_begin(no->chamada_funcao.parametros); it != cvector_end(no->chamada_funcao.parametros); it++, i++) {
          reescrever_codigo(*it);

          if (i < cvector_size(no->chamada_funcao.parametros) - 1) {
            printf(", ");
          }
        }
        printf(")");
      } break;
      case ExpressaoRelacional: {
        reescrever_codigo(no->relacional.esquerda);
        printf(" %s ", no->relacional.simbolo);
        reescrever_codigo(no->relacional.direita);
      } break;
      case ExpressaoLogica: {
        if (strcmp("!", no->logica.simbolo) == 0) {
          printf("!(");
          reescrever_codigo(no->logica.esquerda);
          printf(")");
        } else {
          reescrever_codigo(no->logica.esquerda);
          printf(" %s ", no->logica.simbolo);
          reescrever_codigo(no->logica.direita);
        }
      } break;
      case ExpressaoAritmetica: {
        reescrever_codigo(no->aritmetica.esquerda);
        printf(" %s ", no->aritmetica.simbolo);
        reescrever_codigo(no->aritmetica.direita);
      } break;
      case Tipo: {
        printf("%s", tipo_para_str_(no->declaracao_tipo));
      } break;
      case ConsanteInt: {
        printf("%d", no->inteiro);
      } break;
      case ConsanteFloat: {
        printf("%.2f", no->flutuante);
      } break;
      case ConsanteString: {
        printf("%s", no->string);
      } break;
      case Id: {
        printf("%s", no->id);
      } break;
      case Variavel: {
        printf("%s ", tipo_para_str_(no->variavel.tipo));

        int i = 0;
        for (AST** it = cvector_begin(no->variavel.ids); it != cvector_end(no->variavel.ids); it++, i++) {
          reescrever_codigo(*it);

          if (i < cvector_size(no->variavel.ids) - 1) {
            printf(", ");
          }
        }
        printf(";\n");
      } break;
      case Desconhecido: {
      } break;

      default:
        break;
    }
  }
}