#include "ast.h"

AST *criar_programa(AST *funcoes, AST *blocoPrincipal) {
  AST *programa = (AST *)malloc(sizeof(AST));
  programa->tipo = Arvore;
  programa->u.arvore.left = funcoes;
  programa->u.arvore.right = blocoPrincipal;
  return programa;
}

AST *criar_lista_funcoes(AST *funcoes, AST *funcao) {
  AST *listaFuncoes = (AST *)malloc(sizeof(AST));
  listaFuncoes->tipo = Vetor;
  listaFuncoes->u.filhos = funcoes;
  cvector_push_back(listaFuncoes->u.filhos, funcao);
  return listaFuncoes;
}

AST *criar_parametro(AST *tipo, AST *id) {
  AST *parametro = (AST *)malloc(sizeof(AST));
  parametro->tipo = Arvore;
  parametro->u.arvore.left = tipo;
  parametro->u.arvore.right = id;
  return parametro;
}

AST *criar_declaracao_parametros(AST *declaracaoAnterior, AST *parametro) {
  if (declaracaoAnterior == NULL) {
    return parametro;
  }
  cvector_push_back(declaracaoAnterior->u.filhos, parametro);
  return declaracaoAnterior;
}

AST *criar_funcao(AST *tipo, AST *id, AST *parametros, AST *bloco) {
  AST *funcao = (AST *)malloc(sizeof(AST));
  funcao->tipo = Vetor;
  funcao->u.filhos = NULL;
  cvector_push_back(funcao->u.filhos, tipo);
  cvector_push_back(funcao->u.filhos, id);
  cvector_push_back(funcao->u.filhos, parametros);
  cvector_push_back(funcao->u.filhos, bloco);
  return funcao;
}

AST *criar_funcao_input_void(AST *tipo, AST *id, AST *bloco) {
  AST *funcao = (AST *)malloc(sizeof(AST));
  funcao->tipo = Vetor;
  funcao->u.filhos = NULL;
  cvector_push_back(funcao->u.filhos, tipo);
  cvector_push_back(funcao->u.filhos, id);
  cvector_push_back(funcao->u.filhos, bloco);
  return funcao;
}

AST *criar_tipo(enum TipoDados tipo) {
  AST *tipoAST = (AST *)malloc(sizeof(AST));
  tipoAST->tipo = Folha;
  tipoAST->token.tipo = Type;
  tipoAST->token.u.type.tipo = tipo;
  return tipoAST;
}

AST *criar_constante_int(const char *input) {
  AST *constanteInt = (AST *)malloc(sizeof(AST));
  constanteInt->tipo = Folha;
  constanteInt->token.tipo = ConstantInt;
  constanteInt->token.u.constInt.valor = atoi(input);
  return constanteInt;
}

AST *criar_constante_float(const char *input) {
  AST *constanteFloat = (AST *)malloc(sizeof(AST));
  constanteFloat->tipo = Folha;
  constanteFloat->token.tipo = ConstantFloat;
  constanteFloat->token.u.constFloat.valor = atof(input);
  return constanteFloat;
}

AST *criar_constante_string(const char *input) {
  AST *constanteString = (AST *)malloc(sizeof(AST));
  constanteString->tipo = Folha;
  constanteString->token.tipo = ConstantString;
  constanteString->token.u.constString.valor = str_acquire(input);
  return constanteString;
}

AST *criar_constante_void() {
  AST *constanteVoid = (AST *)malloc(sizeof(AST));
  constanteVoid->tipo = Folha;
  constanteVoid->token.tipo = ConstantVoid;
  constanteVoid->token.u.nada = NULL;
  return constanteVoid;
}

AST *criar_idenfier(const char *input) {
  AST *idenfier = (AST *)malloc(sizeof(AST));
  idenfier->tipo = Folha;
  idenfier->token.tipo = Identifier;
  idenfier->token.u.idenfier.id = str_acquire(input);
  return idenfier;
}

AST *criar_bloco_principal(AST *declaracoes, AST *comandos) {
  AST *blocoPrincipal = (AST *)malloc(sizeof(AST));
  blocoPrincipal->tipo = Arvore;
  blocoPrincipal->u.arvore.left = declaracoes;
  blocoPrincipal->u.arvore.right = comandos;
  return blocoPrincipal;
}