#include "ast.h"

void analise_semantica_variaveis(vector(AST *) declaracoes_variaveis, AST *bloco) {
  if (!bloco) return;

  for (AST **item = cvector_begin(declaracoes_variaveis); item != cvector_end(declaracoes_variaveis); ++item) {
    enum TipoDados tipo = (*item)->u.arvore.left->token.u.type.tipo;
    vector(AST *) ids = (*item)->u.arvore.right->u.filhos;

    for (AST **id = cvector_begin(ids); id != cvector_end(ids); ++id) {
      analise_semantica_verificar_variavel((*id)->token.u.idenfier.id, tipo, bloco);
    }
  }
}

void analise_semantica_verificar_variavel(str id, enum TipoDados tipo, AST *ast) {
  if (!ast) return;

  switch (ast->tipo) {
    case Arvore: {
      if (ast->token.tipo == Assignment && str_eq(ast->u.arvore.left->token.u.idenfier.id, id) &&
          ast->u.arvore.right->token.u.constante.tipo != tipo) {
        converter_constant_para(ast->u.arvore.right, tipo);
      } else {
        analise_semantica_verificar_variavel(id, tipo, ast->u.arvore.left);
        analise_semantica_verificar_variavel(id, tipo, ast->u.arvore.right);
      }
    }

    break;
    case Vetor: {
      for (AST **it = cvector_begin(ast->u.filhos); it != cvector_end(ast->u.filhos); ++it) {
        analise_semantica_verificar_variavel(id, tipo, *it);
      }
    } break;
    case Folha: {
      return;
    };

    default:
      break;
  }
}

void analise_semantica_funcoes(vector(AST *) declaracoes_funcoes) {
  for (AST **f = cvector_begin(declaracoes_funcoes); f != cvector_end(declaracoes_funcoes); f++) {
    AST *tipo = (*f)->u.filhos[0];
    analise_semantica_verificar_retorno_funcao(tipo->token.u.type.tipo, *f);
  }
}

void analise_semantica_verificar_retorno_funcao(enum TipoDados tipo, AST *bloco) {
  if (!bloco) return;

  if (bloco->token.tipo == Return) {
    AST *expressao = bloco->u.arvore.left;

    if (expressao->token.tipo == Constant) {
      if (expressao->token.u.constante.tipo != tipo) {
        converter_constant_para(expressao, tipo);
      }
    }
  }

  switch (bloco->tipo) {
    case Arvore:
      analise_semantica_verificar_retorno_funcao(tipo, bloco->u.arvore.left);
      analise_semantica_verificar_retorno_funcao(tipo, bloco->u.arvore.right);
      break;
    case Vetor:
      for (AST **it = cvector_begin(bloco->u.filhos); it != cvector_end(bloco->u.filhos); it++) {
        analise_semantica_verificar_retorno_funcao(tipo, *it);
      }
      break;

    default:
      break;
  }
}

void converter_constant_para(AST *constante, enum TipoDados tipo) {
  if (!constante) return;

  switch (constante->token.u.constante.tipo) {
    case Int: {
      switch (tipo) {
        case Float: {
          constante->token.u.constante.tipo = Float;
          constante->token.u.constante.valor.flutuante = intToFloat(constante->token.u.constante.valor.inteiro);
        } break;
        case String:
        default:
          break;
      }
    } break;
    case Float: {
      switch (tipo) {
        case Int: {
          constante->token.u.constante.tipo = Int;
          constante->token.u.constante.valor.inteiro = floatToInt(constante->token.u.constante.valor.flutuante);

          exibir_warning("Tipo inteiro nao eh equivalente a ponto flutuante!");
        } break;
        case String:
        default:
          break;
      }
    } break;
    case String: {
      switch (constante->token.u.constante.tipo) {
        case Int:
        case Float:
        default:
          break;
      }
    };

    default:
      break;
  }
}

void analise_semantica_expressao(AST *arvore) {
  if (arvore) {
    bool achouFloat = false;

    percorrer_arvore_aplicando_funcao(arvore, lambda(void, (AST * arv), {
                                        if (arv->tipo == Folha && arv->token.tipo == Constant && arv->token.u.constante.tipo == Float) {
                                          achouFloat = true;
                                        }
                                      }));

    if (achouFloat) {
      percorrer_arvore_aplicando_funcao(arvore, lambda(void, (AST * arv), {
                                          if (arv->tipo == Folha) {
                                            converter_constant_para(arv, Float);
                                          }
                                        }));
    }
  }
}

void analise_semantica_chamada_funcao_existe(vector(AST *) funcoes, AST *nodo) {
  if (nodo && get_tipo_token(nodo) == FunctionCall) {
    if (funcoes) {
      str id = get_id_id(nodo->u.arvore.left);

      for (AST **it = cvector_begin(funcoes); it != cvector_end(funcoes); it++) {
        str fn_id = get_funcao_id(*it);
        if (str_eq(id, fn_id)) {
          return;
        }
      }
    }
    char msg_erro[1000];
    sprintf(msg_erro, "A funcao '%s' nao existe!", str_ptr(get_chamada_funcao_id(nodo)));
    exibir_erro(msg_erro);
  }
}

void analise_semantica_funcao(vector(AST *) funcoes, AST *funcao) {
  percorrer_arvore_aplicando_funcao(funcao, lambda(void, (AST * no), {
                                      analise_semantica_chamada_funcao_existe(funcoes, no);
                                      analise_semantica_chamada_funcao_numero_parametros(funcoes, no);
                                    }));
}

void analise_semantica_chamada_funcao_numero_parametros(vector(AST *) funcoes, AST *nodo) {
  if (nodo && get_tipo_no(nodo) == Arvore && get_tipo_token(nodo) == FunctionCall) {
    if (funcoes && cvector_size(funcoes) > 0) {
      AST *lista_parametros = nodo->u.arvore.right;
      str fn_id = get_id_id(nodo->u.arvore.left);
      AST *fn = procurar_funcao(funcoes, fn_id);

      if (fn) {
        size_t tam_parametros_fn = cvector_size(procurar_tipagem_dos_parametros_funcao(fn)),
               tam_parametros_chamada_fn = cvector_size(get_lista_parametros(lista_parametros));
        if (tam_parametros_fn != tam_parametros_chamada_fn) {
          char msg_erro[1000];
          sprintf(msg_erro, "A lista de parametros da função '%s' é %ld e nao %ld", str_ptr(fn_id), tam_parametros_fn,
                  tam_parametros_chamada_fn);
          exibir_erro(msg_erro);
        }
      }
    }
  }
}

void analise_semantica_funcoes_multiplamente_declaradas(vector(AST *) funcoes) {
  if (funcoes) {
    for (size_t i = 0; i < cvector_size(funcoes); i++) {
      for (size_t j = 0; j < cvector_size(funcoes); j++) {
        if (i != j) {
          if (str_eq(get_funcao_id(funcoes[i]), get_funcao_id(funcoes[j]))) {
            char msg_erro[1000];
            sprintf(msg_erro, "A funcao '%s' foi multiplamente declarada!", str_ptr(get_funcao_id(funcoes[i])));
            exibir_erro(msg_erro);
            break;
          }
        }
      }
    }
  }
}

void analise_semantica_variaveis_multiplamente_declaradas(vector(AST *) declaracoes) {
  if (declaracoes) {
    vector(str) ids_declaracoes = get_ids_declaracoes_variaveis(declaracoes);
    for (size_t i = 0; i < cvector_size(ids_declaracoes); i++) {
      for (size_t j = 0; j < cvector_size(ids_declaracoes); j++) {
        if (j != i) {
          if (str_eq(ids_declaracoes[i], ids_declaracoes[j])) {
            char msg_erro[1000];
            sprintf(msg_erro, "A variavel '%s' foi multiplamente declarada!", str_ptr(ids_declaracoes[i]));
            exibir_erro(msg_erro);
            break;
          }
        }
      }
    }
  }
}

void analise_semantica_uso_variavel_nao_declarada(vector(AST *) funcoes, vector(AST *) declaracoes, AST *bloco) {
  vector(str) ids_funcoes = get_ids_funcoes(funcoes);
  if (declaracoes) {
    vector(str) ids_declaracoes = get_ids_declaracoes_variaveis(declaracoes);

    percorrer_arvore_aplicando_funcao(bloco, lambda(void, (AST * no), {
                                        if (no && get_tipo_token(no) == Identifier) {
                                          str id = get_id_id(no);
                                          for (size_t i = 0; i < cvector_size(ids_declaracoes); i++) {
                                            if (str_eq(id, ids_declaracoes[i])) {
                                              return;
                                            }
                                          }
                                          for (size_t i = 0; i < cvector_size(ids_funcoes); i++) {
                                            if (str_eq(id, ids_funcoes[i])) {
                                              return;
                                            }
                                          }

                                          char msg_erro[1000];
                                          sprintf(msg_erro, "A variavel '%s' nao foi previamente declarada!", str_ptr(id));
                                          exibir_erro(msg_erro);
                                        }
                                      }));
  }

  if (funcoes) {
    for (size_t i = 0; i < cvector_size(funcoes); i++) {
      vector(str) ids_parametros = get_ids_parametros_funcao(funcoes[i]);
      vector(str) ids_declaracoes =
          cvector_size(funcoes[i]->u.filhos) == 4 ? get_ids_declaracoes_variaveis(funcoes[i]->u.filhos[3]->u.arvore.left) : NULL;

      percorrer_arvore_aplicando_funcao(funcoes[i]->u.filhos[3]->u.arvore.right, lambda(void, (AST * no), {
                                          if (no && get_tipo_token(no) == Identifier) {
                                            str id = get_id_id(no);
                                            for (size_t i = 0; i < cvector_size(ids_declaracoes); i++) {
                                              if (str_eq(id, ids_declaracoes[i])) {
                                                return;
                                              }
                                            }
                                            for (size_t i = 0; i < cvector_size(ids_funcoes); i++) {
                                              if (str_eq(id, ids_funcoes[i])) {
                                                return;
                                              }
                                            }

                                            for (size_t i = 0; i < cvector_size(ids_parametros); i++) {
                                              if (str_eq(id, ids_parametros[i])) {
                                                return;
                                              }
                                            }

                                            char msg_erro[1000];
                                            sprintf(msg_erro, "A variavel '%s' nao foi previamente declarada!", str_ptr(id));
                                            exibir_erro(msg_erro);
                                          }
                                        }));
    }
  }
}

void analise_semantica_chamada_funcao_tipos_entrada(vector(AST *) funcoes, AST *nodo) {
  percorrer_arvore_aplicando_funcao(nodo, lambda(void, (AST * no), {
                                      if (no && get_tipo_no(no) == Arvore && get_tipo_token(no) == FunctionCall) {
                                        AST *id = no->u.arvore.left;
                                        AST *parametros = no->u.arvore.right;

                                        str id_ = get_id_id(id);
                                        AST *fn = procurar_funcao(funcoes, id_);

                                        vector(enum TipoDados) tipos_parametros_fn = procurar_tipagem_dos_parametros_funcao(fn);
                                        vector(enum TipoDados) tipos_parametros_entrada = get_lista_parametros(parametros);

                                        if (cvector_size(tipos_parametros_fn) == cvector_size(tipos_parametros_entrada)) {
                                          for (size_t i = 0; i < cvector_size(tipos_parametros_fn); i++) {
                                            if (tipos_parametros_fn[i] != tipos_parametros_entrada[i]) {
                                              if (tipos_parametros_fn[i] == Int && tipos_parametros_entrada[i] == Float) {
                                                converter_constant_para(parametros->u.filhos[i], Int);
                                              } else if ((tipos_parametros_fn[i] == Float && tipos_parametros_entrada[i] == Int)) {
                                                converter_constant_para(parametros->u.filhos[i], Float);
                                              } else {
                                                char msg_erro[1000];
                                                sprintf(msg_erro, "Tipo de entrada invalida para a funcao '%s'!", str_ptr(id_));
                                                exibir_erro(msg_erro);
                                              }
                                            }
                                          }
                                        }
                                      }
                                    }));
}