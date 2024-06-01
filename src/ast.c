#include "ast.h"

struct Programa *criarPrograma(vector_t(AST *) declacaoFuncoes, vector_t(AST *) blocoPrincipal) {
  struct Programa *novo = (struct Programa *)malloc(sizeof(struct Programa));
  novo->declacaoFuncoes = declacaoFuncoes;
  novo->blocoPrincipal = blocoPrincipal;
  return novo;
}

AST *criarAST(NodeTipo tipo) {
  AST *novo = (AST *)malloc(sizeof(AST));
  novo->tipo = tipo;
  return novo;
}

AST *criarASTComandoIf(AST *condicao, AST *comando) {
  AST *novo = criarAST(ComIf);
  novo->token.comandoIf.condicao = condicao;
  novo->token.comandoIf.comando = comando;
  return novo;
}

AST *criarASTComandoElse(AST *comando) {
  AST *novo = criarAST(ComElse);
  novo->token.comandoElse.comando = comando;
  return novo;
}

AST *criarASTComandoWhile(AST *condicao, AST *comando) {
  AST *novo = criarAST(ComWhile);
  novo->token.comandoWhile.condicao = condicao;
  novo->token.comandoWhile.comando = comando;
  return novo;
}

AST *criarASTComandoAtribuicao(char *id, AST *expressao) {
  AST *novo = criarAST(ComAtribuicao);
  novo->token.comandoAtribuicao.id = id;
  novo->token.comandoAtribuicao.expressao = expressao;
  return novo;
}

AST *criarASTComandoPrint(AST *expressao) {
  AST *novo = criarAST(ComPrint);
  novo->token.comandoPrint.expressao = expressao;
  return novo;
}

AST *criarASTComandoRead(char *id) {
  AST *novo = criarAST(ComRead);
  novo->token.comandoRead.id = id;
  return novo;
}

AST *criarASTComandoReturn(AST *expressao) {
  AST *novo = criarAST(ComReturn);
  novo->token.comandoReturn.expressao = expressao;
  return novo;
}

AST *criarASTComandoChamadaFuncao(char *id, vector_t(AST *) * parametros) {
  AST *novo = criarAST(ComChamadaFuncao);
  novo->token.comandoChamadaFuncao.id = id;
  novo->token.comandoChamadaFuncao.parametros = parametros;
  return novo;
}

AST *criarASTExpressaoAritmetica(enum OperadoresAritmeticos operador, AST *esquerda, AST *direita) {
  AST *novo = criarAST(ExprAritmetica);
  novo->token.expressaoAritmetica.operador = operador;
  novo->token.expressaoAritmetica.esquerda = esquerda;
  novo->token.expressaoAritmetica.direita = direita;
  return novo;
}

AST *criarASTExpressaoRelacional(enum OperadoresRelacionais operador, AST *esquerda, AST *direita) {
  AST *novo = criarAST(ExprRelacional);
  novo->token.expressaoRelacional.operador = operador;
  novo->token.expressaoRelacional.esquerda = esquerda;
  novo->token.expressaoRelacional.direita = direita;
  return novo;
}

AST *criarASTExpressaoLogica(enum OperadoresLogicos operador, AST *esquerda, AST *direita) {
  AST *novo = criarAST(ExprLogica);
  novo->token.expressaoLogica.operador = operador;
  novo->token.expressaoLogica.esquerda = esquerda;
  novo->token.expressaoLogica.direita = direita;
  return novo;
}

AST *criarASTExpressaoValorInteiro(int valor) {
  AST *novo = criarAST(ExprValorInteiro);
  novo->token.expressaoValorInteiro.valor = valor;
  return novo;
}

AST *criarASTExpressaoValorFloat(float valor) {
  AST *novo = criarAST(ExprValorFloat);
  novo->token.expressaoValorFloat.valor = valor;
  return novo;
}

AST *criarASTExpressaoValorString(char *valor) {
  AST *novo = criarAST(ExprValorString);
  novo->token.expressaoValorString.valor = valor;
  return novo;
}
