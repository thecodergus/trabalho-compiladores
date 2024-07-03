#include "ast.h"

int LABEL_NUM = 1;

void criar_arquivo_class(const char* nome, AST* programa) {
  FILE* arquivo;
  arquivo = fopen(nome, "w");

  gerar_bytecode(arquivo, programa, NULL);

  fclose(arquivo);

  printf("Arquivo criado com sucesso!\n");
}

void gerar_bytecode(FILE* arquivo, AST* no, const char* contexto) {
  // TODO
  // Implementar a geração de bytecode
  // para a linguagem de máquina virtual Java
  // Dica: use a função reescrever_codigo
  // para reescrever a AST em código de máquina
  if (arquivo && no) {
    switch (no->tipo) {
      case Programa: {
        fprintf(arquivo,
                ".class public Main\n.super java/lang/Object\n\n.method public <init>()V\n\taload_0\n\tinvokenonvirtual "
                "java/lang/Object/<init>()V\n\treturn\n.end method\n\n");

        for (AST** it = cvector_begin(no->programa.funcoes); it != cvector_end(no->programa.funcoes); it++) {
          gerar_bytecode(arquivo, *it, NULL);
        }

        fprintf(arquivo,
                ".method public static main(Ljava/lang/String;)V\n\t.limit stack 4"
                "\n\t.limit locals 8\n\n");
        gerar_bytecode(arquivo, no->programa.bloco, "main");

        fprintf(arquivo, ".end method\n\n");

      } break;
      case Funcao: {
        fprintf(arquivo, ".method public static %s(", no->funcao.id);

        int i = 0;
        for (AST** it = cvector_begin(no->funcao.parametros); it != cvector_end(no->funcao.parametros); it++, i++) {
          gerar_bytecode(arquivo, *it, contexto);
          if (i < cvector_size(no->funcao.parametros) - 1) {
            fprintf(arquivo, " ");
          }
        }

        fprintf(arquivo, ") %s\n\t.limit stack 4\n\t.limit locals 8\n\n", tipo_para_java_argumento(no->funcao.retorno));

        gerar_bytecode(arquivo, no->funcao.bloco, no->funcao.id);

        fprintf(arquivo, ".end method\n\n");
      } break;
      case Parametro: {
        fprintf(arquivo, "%s", tipo_para_java_argumento(no->parametro.tipo));
      } break;
      case Bloco: {
        // Declarações
        for (AST** it = cvector_begin(no->bloco.declaracoes); it != cvector_end(no->bloco.declaracoes); it++) {
          gerar_bytecode(arquivo, *it, contexto);
        }

        // Comandos
        for (AST** it = cvector_begin(no->bloco.comandos); it != cvector_end(no->bloco.comandos); it++) {
          gerar_bytecode(arquivo, *it, contexto);
        }
      } break;
      case Atribuicao: {
        enum TipoDado id_tipo = get_tipo_dado_variavel(contexto, no->atribuicao.id);
        gerar_bytecode(arquivo, no->atribuicao.expressao, contexto);

        fprintf(arquivo, "\t%sstore %d\n", tipo_para_java(id_tipo), get_numero_variavel_posicao(contexto, no->atribuicao.id));
      } break;
      case If: {
        int label_bloco_else = LABEL_NUM++;
        int label_final = LABEL_NUM++;
        int label_condicao = LABEL_NUM++;

        // Condicao
        fprintf(arquivo, "\tl%d:\n", label_condicao);
        gerar_expressao_logica(arquivo, no->if_.codicao, contexto, label_condicao, label_final);
        // Bloco If
        for (AST** it = cvector_begin(no->if_.comandosIf); it != cvector_end(no->if_.comandosIf); it++) {
          gerar_bytecode(arquivo, *it, contexto);
        }

        if (no->if_.comandosElse != NULL) {
          fprintf(arquivo, "\tgoto l%d\n\tl%d:\n", label_condicao, label_final);
          // Bloco Else
          for (AST** it = cvector_begin(no->if_.comandosElse); it != cvector_end(no->if_.comandosElse); it++) {
            gerar_bytecode(arquivo, *it, contexto);
          }
          fprintf(arquivo, "\tl%d:\n", label_condicao);
        } else {
          fprintf(arquivo, "\tl%d:\n", label_final);
        }
      }

      break;
      case While: {
        int label_condicional = LABEL_NUM++;
        int label_bloco = LABEL_NUM++;
        int label_final = LABEL_NUM++;

        fprintf(arquivo, "\tl%d:\n", label_condicional);
        gerar_expressao_logica(arquivo, no->while_.codicao, contexto, label_bloco, label_final);

        fprintf(arquivo, "\tl%d:\n", label_bloco);
        for (AST** it = cvector_begin(no->while_.bloco); it != cvector_end(no->while_.bloco); it++) {
          gerar_bytecode(arquivo, *it, contexto);
        }

        fprintf(arquivo, "\tgoto %d\n\tl%d:\n", label_condicional, label_final);
      } break;
      case Retorno: {
        gerar_bytecode(arquivo, no->retorno.ret, contexto);
      } break;
      case Print: {
        gerar_bytecode(arquivo, no->print.parametro, contexto);
      } break;
      case Read: {
      } break;
      case ChamadaFuncao: {
        int i = 0;
        for (AST** it = cvector_begin(no->chamada_funcao.parametros); it != cvector_end(no->chamada_funcao.parametros); it++, i++) {
          gerar_bytecode(arquivo, *it, contexto);

          if (i < cvector_size(no->chamada_funcao.parametros) - 1) {
          }
        }
      } break;
      case ExpressaoRelacional: {
       
      } break;
      case ExpressaoLogica: {
        // if (strcmp("!", no->logica.simbolo) == 0) {
        //   gerar_bytecode(arquivo, no->logica.esquerda, contexto);
        // } else {
        //   gerar_bytecode(arquivo, no->logica.esquerda, contexto);
        //   gerar_bytecode(arquivo, no->logica.direita, contexto);
        // }
      } break;
      case ExpressaoAritmetica: {
        enum TipoDado expr_tipo = descobrir_tipo_expressao_com_contexto(contexto, no->aritmetica.esquerda);

        gerar_bytecode(arquivo, no->aritmetica.esquerda, contexto);
        gerar_bytecode(arquivo, no->aritmetica.direita, contexto);

        fprintf(arquivo, "\t%s", tipo_para_java(expr_tipo));
        if (strchr(no->aritmetica.simbolo, '+') != NULL) {
          fprintf(arquivo, "add\n");
        } else if (strchr(no->aritmetica.simbolo, '-') != NULL) {
          fprintf(arquivo, "sub\n");
        } else if (strchr(no->aritmetica.simbolo, '*') != NULL) {
          fprintf(arquivo, "mul\n");
        } else if (strchr(no->aritmetica.simbolo, '/') != NULL) {
          fprintf(arquivo, "div\n");
        }

      } break;
      case ExpressaoAritmeticaConvertida: {
        fprintf(arquivo, "\t%s2%s\n", tipo_para_java(no->aritmetica_convertida.de), tipo_para_java(no->aritmetica_convertida.para));

        gerar_bytecode(arquivo, no->aritmetica_convertida.expr, contexto);
        fprintf(arquivo, "\n");
      } break;
      case Tipo: {
      } break;
      case ConsanteInt: {
        if (no->inteiro < 6) {
          fprintf(arquivo, "\ticonst_%d\n", no->inteiro);
        } else if (no->inteiro < 128) {
          fprintf(arquivo, "\ttbipush %d\n", no->inteiro);
        } else {
          fprintf(arquivo, "\tldc %d\n", no->inteiro);
        }
      } break;
      case ConsanteFloat: {
        char fl[1000];
        sprintf(fl, "%f", no->flutuante);
        replace_char(fl, ',', '.');
        fprintf(arquivo, "\tldc %s\n", fl);
      } break;
      case ConsanteString: {
        fprintf(arquivo, "\tldc %s\n", no->string);
      } break;
      case Id: {
        enum TipoDado id_tipo = get_tipo_dado_variavel(contexto, no->id);
        int id_posicao_pilha = get_numero_variavel_posicao(contexto, no->id);
        fprintf(arquivo, "\t%sload %d\n", tipo_para_java(id_tipo), id_posicao_pilha);
      } break;
      case Variavel: {
        int i = 0;
        for (AST** it = cvector_begin(no->variavel.ids); it != cvector_end(no->variavel.ids); it++, i++) {
          gerar_bytecode(arquivo, *it, contexto);

          if (i < cvector_size(no->variavel.ids) - 1) {
          }
        }
      } break;
      case Desconhecido: {
        fprintf(arquivo, "(NULL)");
      } break;

      default: {
      } break;
    }
  }
}

void gerar_expressao_logica(FILE *arquivo, AST *no, const char *contexto, int v, int f)
{
  if(no && no->tipo == ExpressaoLogica){
    if (strcmp("!", no->logica.simbolo) == 0)
    {
      gerar_expressao_logica(arquivo, no->logica.esquerda, contexto, v, f);
    }
    else
    {
      if(strcmp("||", no->logica.simbolo) == 0){
        int l1 = LABEL_NUM++;
        gerar_expressao_logica(arquivo, no->logica.esquerda, contexto, v, l1);
        fprintf(arquivo, "l%d:\n", l1);
        gerar_expressao_logica(arquivo, no->logica.esquerda, contexto, v, f);
      }
      else if (strcmp("&&", no->logica.simbolo) == 0){
        int l1 = LABEL_NUM++;
        gerar_expressao_logica(arquivo, no->logica.esquerda, contexto, v, l1);
        fprintf(arquivo, "l%d:\n", l1);
        gerar_expressao_logica(arquivo, no->logica.esquerda, contexto, v, f);
      }

        gerar_bytecode(arquivo, no->logica.esquerda, contexto);
      gerar_bytecode(arquivo, no->logica.direita, contexto);
    }
  }else{
    gerar_expressao_relacional(arquivo, no, contexto, f, v);
  }
}

void gerar_expressao_relacional(FILE *arquivo, AST *no, const char *contexto, int f, int v){
  if(no && no->tipo == ExpressaoRelacional){
    gerar_bytecode(arquivo, no->relacional.esquerda, contexto);
    gerar_bytecode(arquivo, no->relacional.direita, contexto);

    enum TipoDado esq_tipo = descobrir_tipo_expressao_com_contexto(contexto, no->relacional.esquerda),
                  dir_tipo = descobrir_tipo_expressao_com_contexto(contexto, no->relacional.direita);

    if (strcmp(no->relacional.simbolo, "==") == 0)
    {
      if (esq_tipo == String)
      {
        fprintf(arquivo, "\tif_acmpeq");
      }
      else
      {
        fprintf(arquivo, "\tif_icmpeq%d\n\tgoto l%d:\n", v, f);
      }
    }
    else if (strcmp(no->relacional.simbolo, "/=") == 0)
    {
      if (esq_tipo == String)
      {
        fprintf(arquivo, "\tif_acmpne");
      }
      else
      {
        fprintf(arquivo, "\tif_icmpne%d\n\tgoto l%d:\n", v, f);
      }
    }
    else if (strcmp(no->relacional.simbolo, "<") == 0)
    {
      if (esq_tipo == String)
      {
      }
      else
      {
        fprintf(arquivo, "\tif_icmplt%d\n\tgoto l%d:\n", v, f);
      }
    }
    else if (strcmp(no->relacional.simbolo, "<=") == 0)
    {
      if (esq_tipo == String)
      {
      }
      else
      {
        fprintf(arquivo, "\tif_icmple%d\n\tgoto l%d:\n", v, f);
      }
    }
    else if (strcmp(no->relacional.simbolo, ">") == 0)
    {
      if (esq_tipo == String)
      {
      }
      else
      {
        fprintf(arquivo, "\tif_icmpgt%d\n\tgoto l%d:\n", v, f);
      }
    }
    else if (strcmp(no->relacional.simbolo, ">=") == 0)
    {
      if (esq_tipo == String)
      {
      }
      else
      {
        fprintf(arquivo, "\tif_icmpge%d\n\tgoto l%d:\n", v, f);
      }
    }
  }else{
    gerar_bytecode(arquivo, no, contexto);
  }
}