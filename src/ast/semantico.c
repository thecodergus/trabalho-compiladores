#include "ast.h"

void avaliar_programa(AST *nodo) {
  if (nodo && nodo->tipo == Programa) {
    for (AST **it = cvector_begin(nodo->programa.funcoes); it != cvector_end(nodo->programa.funcoes); it++) {
      avaliar_funcao(*it);
    }
  }
}

void avaliar_funcao(AST *nodo) {
  enum TipoDado tipo = nodo->funcao.retorno;
  const char *id = strdup(nodo->funcao.id);
  vector(struct Par) parametros = NULL;

  if (id_sendo_usado_por_funcao(id)) {
    char msg[1000];
    sprintf(msg, "O id '%s' ja esta sendo usado por uma funcao", id);
    exibir_erro(msg);
    return;
  } else {
    // Adicionando a função e os parametros na tabela de simbolos
    for (AST **it = cvector_begin(nodo->funcao.parametros); it != cvector_end(nodo->funcao.parametros); it++) {
      struct Par par;

      par.id = strdup((*it)->parametro.id);
      par.tipo = (*it)->parametro.tipo;

      cvector_push_back(parametros, par);

      // Adicionando parametro
      if (id_sendo_usado_por_funcao((*it)->parametro.id)) {
        char msg[1000];
        sprintf(msg, "O id '%s' do parametro da funcao '%s' ja eh usado por outra funcao", (*it)->parametro.id, id);
        exibir_erro(msg);
      } else if (id_sendo_usado_por_variavel(id, (*it)->parametro.id)) {
        char msg[1000];
        sprintf(msg, "O id '%s' ja esta sendo usado por uma variavel no mesmo escopo", (*it)->parametro.id);
        exibir_erro(msg);
      } else {
        adicionar_variavel(id, (*it)->parametro.tipo, (*it)->parametro.id);
      }
    }

    // Adicionando a tabela de simbolos
    adicionar_funcao(tipo, id, parametros);

    // Avaliar Bloco Principal
    avaliar_bloco(nodo->funcao.bloco, id);

    // Avaliar retorno da função
    bool encontrou_return = false;
    percorrer(
        nodo->funcao.bloco, lambda(void, (AST * no), {
          if (no && no->tipo == Retorno) {
            encontrou_return = true;

            if (tipo != no->retorno.tipo && no->retorno.ret) {
              char msg[1000];
              if (tipo != Void && tipo != String) {
                switch (no->retorno.ret->tipo) {
                  case Id: {
                    if (!id_sendo_usado_por_variavel(id, no->retorno.ret->id)) {
                      sprintf(msg, "A variavel '%s' que esta no retorno da funcao '%s' nao existe!", no->retorno.ret->id, id);
                      exibir_erro(msg);
                    } else {
                      sprintf(msg, "A funcao '%s' tem retorno do tipo '%s' mas esta retornando a variavel '%s' que eh do tipo '%s'", id,
                              tipo_para_str(tipo), no->retorno.ret->id, tipo_para_str(get_tipo_dado_variavel(id, no->retorno.ret->id)));
                      exibir_erro(msg);
                    }
                  } break;
                  case ConsanteFloat: {
                    constantFloat_para_constantInt(no->retorno.ret);
                    sprintf(msg, "Convertendo o retorno da funcao '%s' que tem tipo '%s' de Float para Int", id,
                            tipo_para_str(get_tipo_dado_funcao(id)));
                    exibir_warning(msg);
                  } break;
                  case ConsanteInt: {
                    constantInt_para_constantFloat(no->retorno.ret);
                    sprintf(msg, "Convertendo o retorno da funcao '%s' que tem tipo '%s' de Int para Float", id,
                            tipo_para_str(get_tipo_dado_funcao(id)));
                    exibir_warning(msg);
                  } break;
                  case ConsanteString: {
                    sprintf(msg, "A funcao '%s' tem tipo '%s' mas esta retornado o tipo '%s'!", id, tipo_para_str(tipo),
                            tipo_para_str(no->retorno.tipo));
                    exibir_erro(msg);
                  } break;
                  case ChamadaFuncao: {
                    if (!id_sendo_usado_por_funcao(no->retorno.ret->id)) {
                      sprintf(msg, "A funcao '%s' que eh chamada no retorno da funcao '%s' nao existe!", no->retorno.ret->id, id);
                      exibir_erro(msg);
                    } else {
                      sprintf(msg, "A funcao '%s' tem tipo '%s', e eh chamada no retorno da funcao '%s' que tem tipo '%s'!",
                              no->retorno.ret->id, tipo_para_str(get_tipo_dado_funcao(no->retorno.ret->id)), id, tipo_para_str(tipo));
                      exibir_erro(msg);
                    }
                  } break;
                  case ExpressaoAritmetica: {
                    enum TipoDado tipo_expr = descobrir_tipo_expressao_com_contexto(id, no->retorno.ret);

                    if (tipo_expr == Int || tipo_expr == Float) {
                      char msg[1000];
                      if (tipo_expr == Int && tipo == Float) {
                        expressaoAritmetica_para_Float(no->retorno.ret);
                        sprintf(
                            msg,
                            "A funcao '%s' tem retorno do tipo '%s', logo, a expressao no retorno da funcao sera convertida para 'Float'",
                            id, tipo_para_str(tipo));
                        exibir_warning(msg);

                      } else if (tipo_expr == Float && tipo == Int) {
                        expressaoAritmetica_para_Int(no->retorno.ret);
                        sprintf(msg,
                                "A funcao '%s' tem retorno do tipo '%s', logo, a expressao no retorno da funcao sera convertida para "
                                "'Int'",
                                id, tipo_para_str(tipo));
                        exibir_warning(msg);
                      }
                    } else if (tipo_expr == String) {
                    }

                  } break;

                  default:
                    break;
                }
              } else {
                sprintf(msg, "A funcao '%s' tem tipo '%s' mas esta retornado o tipo '%s'!", id, tipo_para_str(tipo),
                        tipo_para_str(no->retorno.tipo));
                exibir_erro(msg);
              }
            }
          }
        }));

    if (!encontrou_return && tipo != Void) {
      char msg[1000];
      sprintf(msg, "A funcao '%s' retorna o tipo '%s' mas nao encontrou nenhum retorno", id, tipo_para_str(tipo));
      exibir_erro(msg);
    }
  }
}

void avaliar_main(AST *nodo) {}

void avaliar_bloco(AST *bloco, const char *contexto) {}