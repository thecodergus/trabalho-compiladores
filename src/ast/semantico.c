#include "ast.h"

void avaliar_programa(AST *nodo) {
  if (nodo && nodo->tipo == Programa) {
    // Avaliar Funções
    for (AST **it = cvector_begin(nodo->programa.funcoes); it != cvector_end(nodo->programa.funcoes); it++) {
      avaliar_funcao(*it);
    }

    // Registrar a função main
    const char *id = "main";
    adicionar_funcao(Int, id, NULL);

    // Avaliar retorno da função
    avaliar_existencia_retorno_funcao(id, nodo->programa.bloco);

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
    avaliar_existencia_retorno_funcao(id, nodo->funcao.bloco);
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
        avaliar_retorno(contexto, comando);
      } break;
      case Print: {
      } break;
      case Read: {
      } break;
      case ChamadaFuncao: {
        avaliar_chamada_funcao(comando);
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
        // Pegar o tipo da varivel
        enum TipoDado variavel_tipo = get_tipo_dado_variavel(contexto, variavel_id);

        switch (valor_atribuido->tipo) {
          case ConsanteFloat: {
            if (variavel_tipo == Int) {
              constantInt_para_constantFloat(valor_atribuido);
              sprintf(msg, "A variavel '%s' da função '%s' tem tipo '%s', logo, iremos converter a atribuição de 'Float' para 'Int'!",
                      variavel_id, contexto, tipo_para_str(variavel_tipo));
              exibir_warning(msg);
            } else if (variavel_tipo == String || variavel_tipo == Void) {
              sprintf(msg, "A variavel '%s' da função '%s' tem tipo '%s' mas foi atribuido a ela o tipo 'Float'", variavel_id, contexto,
                      tipo_para_str(variavel_tipo));
              exibir_erro(msg);
            }
          } break;
          case ConsanteInt: {
            if (variavel_tipo == Float) {
              constantFloat_para_constantInt(valor_atribuido);
              sprintf(msg, "A variavel '%s' da função '%s' tem tipo '%s', logo, iremos converter a atribuição de 'Int' para 'Float'!",
                      variavel_id, contexto, tipo_para_str(variavel_tipo));
              exibir_warning(msg);
            } else if (variavel_tipo == String || variavel_tipo == Void) {
              sprintf(msg, "A variavel '%s' da função '%s' tem tipo '%s' mas foi atribuido a ela o tipo 'Int'", variavel_id, contexto,
                      tipo_para_str(variavel_tipo));
              exibir_erro(msg);
            }
          } break;
          case ConsanteString: {
            if (variavel_tipo != String) {
              sprintf(msg, "A variavel '%s' da funcao '%s' tem tipo '%s' mas recebeu um tipo String!", variavel_id, contexto,
                      tipo_para_str(variavel_tipo));
              exibir_erro(msg);
            }
          } break;
          case ExpressaoAritmetica: {
            enum TipoDado expressao_tipo = descobrir_tipo_expressao_com_contexto(contexto, valor_atribuido);
            if (variavel_tipo != expressao_tipo && (variavel_tipo != String && variavel_tipo != Void)) {
              if (variavel_tipo == Int && expressao_tipo == Float) {
                expressaoAritmetica_para_Int(valor_atribuido);
                sprintf(msg,
                        "A variavel '%s' da função '%s' tem tipo 'Int' mas recebeu uma expressão de tipo 'Float', iremos converter a "
                        "expressão para o "
                        "tipo 'Int'!",
                        variavel_id, contexto);
                exibir_warning(msg);
              } else if (variavel_tipo == Float && expressao_tipo == Int) {
                expressaoAritmetica_para_Float(valor_atribuido);
                sprintf(msg,
                        "A variavel '%s' da função '%s' tem tipo 'Float' mas recebeu uma expressão de tipo 'Int', iremos converter a "
                        "expressão para o "
                        "tipo 'Float'!",
                        variavel_id, contexto);
                exibir_warning(msg);
              }
            } else {
              sprintf(msg, "A variavel '%s' tem tipo '%s' mas recebeu uma expressão aritmetica do tipo '%s'!", variavel_id,
                      tipo_para_str(variavel_tipo), tipo_para_str(expressao_tipo));
              exibir_erro(msg);
            }
          } break;
          case Id: {
            if (!id_sendo_usado_por_variavel(contexto, valor_atribuido->id)) {
              sprintf(msg, "A variavel '%s' esta recebendo o valor da variavel inexistente '%s'!", variavel_id, valor_atribuido->id);
              exibir_erro(msg);
            } else {
              enum TipoDado variavel2_tipo = get_tipo_dado_variavel(contexto, valor_atribuido->id);
              if (variavel_tipo != variavel2_tipo) {
                sprintf(msg, "A variavel '%s' tem tipo '%s' mas esta recebendo a variavel '%s' que tem tipo '%s' na função '%s'!",
                        variavel_id, tipo_para_str(variavel_tipo), valor_atribuido->id, tipo_para_str(variavel2_tipo), contexto);
                exibir_erro(msg);
              }
            }
          } break;
          case ChamadaFuncao: {
            if (!id_sendo_usado_por_funcao(valor_atribuido->chamada_funcao.id)) {
              sprintf(msg, "A função chamada '%s' na atribução para a variavel '%s' na função '%s' não existe!",
                      valor_atribuido->chamada_funcao.id, variavel_id, contexto);
              exibir_erro(msg);
            } else {
              enum TipoDado funcao_tipo = get_tipo_dado_funcao(valor_atribuido->chamada_funcao.id);

              if (variavel_tipo != funcao_tipo) {
                sprintf(msg, "A variavel '%s' tem tipo '%s' enquanto que função chamada '%s' tem tipo '%s'", variavel_id,
                        tipo_para_str(variavel_tipo), valor_atribuido->chamada_funcao.id, tipo_para_str(funcao_tipo));
                exibir_erro(msg);
              }
              // Fazer o resto das avaliações também
              avaliar_chamada_funcao(valor_atribuido);
            }
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

void avaliar_chamada_funcao(AST *chamada) {
  if (chamada && chamada->tipo == ChamadaFuncao) {
  }
}

void avaliar_existencia_retorno_funcao(const char *id, AST *bloco) {
  bool encontrou_return = false;
  enum TipoDado tipo = get_tipo_dado_funcao(id);
  percorrer(bloco, lambda(void, (AST * no), {
              if (no && no->tipo == Retorno) {
                encontrou_return = true;
              }
            }));

  if (!encontrou_return && tipo != Void) {
    char msg[1000];
    sprintf(msg, "A funcao '%s' retorna o tipo '%s' mas não encontrou nenhum retorno", id, tipo_para_str(tipo));
    exibir_erro(msg);
  }
}

void avaliar_retorno(const char *id, AST *retorno) {
  if (retorno && retorno->tipo == Retorno) {
    char msg[1000];
    enum TipoDado tipo = get_tipo_dado_funcao(id);
    if (tipo != Void && tipo != String && retorno->retorno.ret) {
      switch (retorno->retorno.ret->tipo) {
        case Id: {
          if (!id_sendo_usado_por_variavel(id, retorno->retorno.ret->id)) {
            sprintf(msg, "A variavel '%s' que esta no retorno da função '%s' não existe!", retorno->retorno.ret->id, id);
            exibir_erro(msg);
          } else if (get_tipo_dado_variavel(id, retorno->retorno.ret->id) != tipo) {
            sprintf(msg, "A função '%s' tem retorno do tipo '%s' mas esta retornando a variavel '%s' que é do tipo '%s'", id,
                    tipo_para_str(tipo), retorno->retorno.ret->id, tipo_para_str(get_tipo_dado_variavel(id, retorno->retorno.ret->id)));
            exibir_erro(msg);
          }
        } break;
        case ConsanteFloat: {
          if (tipo == Int) {
            constantFloat_para_constantInt(retorno->retorno.ret);
            sprintf(msg, "Convertendo o retorno da função '%s' que tem tipo '%s' de Float para Int", id,
                    tipo_para_str(get_tipo_dado_funcao(id)));
            exibir_warning(msg);
          }
        } break;
        case ConsanteInt: {
          if (tipo == Float) {
            constantInt_para_constantFloat(retorno->retorno.ret);
            sprintf(msg, "Convertendo o retorno da função '%s' que tem tipo '%s' de Int para Float", id,
                    tipo_para_str(get_tipo_dado_funcao(id)));
            exibir_warning(msg);
          }
        } break;
        case ConsanteString: {
          sprintf(msg, "A função '%s' tem tipo '%s' mas esta retornado o tipo '%s'!", id, tipo_para_str(tipo),
                  tipo_para_str(retorno->retorno.tipo));
          exibir_erro(msg);
        } break;
        case ChamadaFuncao: {
          if (!id_sendo_usado_por_funcao(retorno->retorno.ret->id)) {
            sprintf(msg, "A função '%s' que é chamada no retorno da função '%s' nao existe!", retorno->retorno.ret->id, id);
            exibir_erro(msg);
          } else {
            sprintf(msg, "A função '%s' tem tipo '%s', e é chamada no retorno da função '%s' que tem tipo '%s'!", retorno->retorno.ret->id,
                    tipo_para_str(get_tipo_dado_funcao(retorno->retorno.ret->id)), id, tipo_para_str(tipo));
            exibir_erro(msg);
          }
        } break;
        case ExpressaoAritmetica: {
          enum TipoDado tipo_expr = descobrir_tipo_expressao_com_contexto(id, retorno->retorno.ret);

          if (tipo_expr == Int || tipo_expr == Float) {
            char msg[1000];
            if (tipo_expr == Int && tipo == Float) {
              expressaoAritmetica_para_Float(retorno->retorno.ret);
              sprintf(msg,
                      "A função '%s' tem retorno do tipo '%s', logo, a expressão que esta com tipo 'Int' no retorno da função "
                      "sera convertida para 'Float'",
                      id, tipo_para_str(tipo));
              exibir_warning(msg);

            } else if (tipo_expr == Float && tipo == Int) {
              expressaoAritmetica_para_Int(retorno->retorno.ret);
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
              tipo_para_str(retorno->retorno.tipo));
      exibir_erro(msg);
    }
  }
}