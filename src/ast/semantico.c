#include "ast.h"

void avaliar_programa(AST *nodo) {
  if (nodo && nodo->tipo == Programa) {
    // Avaliar Funções
    for (AST **it = cvector_begin(nodo->programa.funcoes); it != cvector_end(nodo->programa.funcoes); it++) {
      avaliar_funcao(*it);
    }

    // Avaliar bloco principal
    avaliar_bloco("main", nodo->programa.bloco);
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
    avaliar_bloco(id, nodo->funcao.bloco);

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
                      sprintf(msg, "A variavel '%s' que esta no retorno da função '%s' não existe!", no->retorno.ret->id, id);
                      exibir_erro(msg);
                    } else if (get_tipo_dado_variavel(id, no->retorno.ret->id) != tipo) {
                      sprintf(msg, "A função '%s' tem retorno do tipo '%s' mas esta retornando a variavel '%s' que é do tipo '%s'", id,
                              tipo_para_str(tipo), no->retorno.ret->id, tipo_para_str(get_tipo_dado_variavel(id, no->retorno.ret->id)));
                      exibir_erro(msg);
                    }
                  } break;
                  case ConsanteFloat: {
                    constantFloat_para_constantInt(no->retorno.ret);
                    sprintf(msg, "Convertendo o retorno da função '%s' que tem tipo '%s' de Float para Int", id,
                            tipo_para_str(get_tipo_dado_funcao(id)));
                    exibir_warning(msg);
                  } break;
                  case ConsanteInt: {
                    constantInt_para_constantFloat(no->retorno.ret);
                    sprintf(msg, "Convertendo o retorno da função '%s' que tem tipo '%s' de Int para Float", id,
                            tipo_para_str(get_tipo_dado_funcao(id)));
                    exibir_warning(msg);
                  } break;
                  case ConsanteString: {
                    sprintf(msg, "A função '%s' tem tipo '%s' mas esta retornado o tipo '%s'!", id, tipo_para_str(tipo),
                            tipo_para_str(no->retorno.tipo));
                    exibir_erro(msg);
                  } break;
                  case ChamadaFuncao: {
                    if (!id_sendo_usado_por_funcao(no->retorno.ret->id)) {
                      sprintf(msg, "A função '%s' que é chamada no retorno da função '%s' nao existe!", no->retorno.ret->id, id);
                      exibir_erro(msg);
                    } else {
                      sprintf(msg, "A função '%s' tem tipo '%s', e é chamada no retorno da função '%s' que tem tipo '%s'!",
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
                        sprintf(msg,
                                "A função '%s' tem retorno do tipo '%s', logo, a expressão que esta com tipo 'Int' no retorno da função "
                                "sera convertida para 'Float'",
                                id, tipo_para_str(tipo));
                        exibir_warning(msg);

                      } else if (tipo_expr == Float && tipo == Int) {
                        expressaoAritmetica_para_Int(no->retorno.ret);
                        sprintf(msg,
                                "A função '%s' tem retorno do tipo '%s', logo, a expressão que esta com tipo 'Float' no retorno da função "
                                "sera convertida para "
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
                sprintf(msg, "A função '%s' tem tipo '%s' mas esta retornado o tipo '%s'!", id, tipo_para_str(tipo),
                        tipo_para_str(no->retorno.tipo));
                exibir_erro(msg);
              }
            }
          }
        }));

    if (!encontrou_return && tipo != Void) {
      char msg[1000];
      sprintf(msg, "A funcao '%s' retorna o tipo '%s' mas não encontrou nenhum retorno", id, tipo_para_str(tipo));
      exibir_erro(msg);
    }
  }
}

void avaliar_main(AST *nodo) {}

void avaliar_bloco(const char *contexto, AST *bloco) {
  if (bloco && bloco->tipo == Bloco) {
    // Jogar para a tabela de simbolos as declarações de variaveis
    for (AST **declaracao = cvector_begin(bloco->bloco.declaracoes); declaracao != cvector_end(bloco->bloco.declaracoes); declaracao++) {
      if (*declaracao && (*declaracao)->tipo == Variavel) {
        enum TipoDado tipo_variavel = (*declaracao)->variavel.tipo;
        for (AST **id_variavel = cvector_begin((*declaracao)->variavel.ids); id_variavel != cvector_end((*declaracao)->variavel.ids);
             id_variavel++) {
          adicionar_variavel(contexto, tipo_variavel, (*id_variavel)->id);
        }
      }
    }

    // Avaliar Comandos, provavelmente implementar outra função para essa tarefa
    for (AST **comando = cvector_begin(bloco->bloco.comandos); comando != cvector_end(bloco->bloco.comandos); comando++) {
      avaliar_comando(contexto, *comando);
    }
  }
}

void avaliar_comando(const char *contexto, AST *comando) {
  if (comando && (comando->tipo == Atribuicao || comando->tipo == If || comando->tipo == While || comando->tipo == Retorno ||
                  comando->tipo == Print || comando->tipo == Read || comando->tipo == ChamadaFuncao)) {
    switch (comando->tipo) {
      case Atribuicao: {
        avaliar_atribuição(contexto, comando);
      } break;
      case If: {
      } break;
      case While: {
      } break;
      case Retorno: {
      } break;
      case Print: {
      } break;
      case Read: {
      } break;
      case ChamadaFuncao: {
      } break;

      default:
        break;
    }
  }
}

void avaliar_atribuição(const char *contexto, AST *atribuicao) {
  if (atribuicao && atribuicao->tipo == Atribuicao) {
    const char *variavel_id = atribuicao->atribuicao.id;
    AST *valor_atribuido = atribuicao->atribuicao.expressao;
    char msg[1000];

    if (!id_sendo_usado_por_variavel(contexto, variavel_id)) {
      sprintf(msg, "A variavel '%s' nao foi previamente declarada na funcao '%s'!", variavel_id, contexto);
      exibir_erro(msg);
    } else {
      if (valor_atribuido &&
          (valor_atribuido->tipo == ConsanteFloat || valor_atribuido->tipo == ConsanteInt || valor_atribuido->tipo == ConsanteString ||
           valor_atribuido->tipo == ExpressaoAritmetica || valor_atribuido->tipo == Id || valor_atribuido->tipo == ChamadaFuncao)) {
        switch (valor_atribuido->tipo) {
          case ConsanteFloat: {
          } break;
          case ConsanteInt: {
          } break;
          case ConsanteString: {
          } break;
          case ExpressaoAritmetica: {
          } break;
          case Id: {
          } break;
          case ChamadaFuncao: {
          } break;

          default:
            break;
        }
      } else {
        sprintf(msg, "Existem algum erro desconhecido na atribuicao da variavel '%s' na funcao '%s'!", variavel_id, contexto);
        exibir_erro(msg);
      }
    }
  }
}