#include "ast.h"

AST *criarAST(Tipo tipo) {
  AST *novo = (AST *)malloc(sizeof(AST));
  novo->tipo = tipo;
  return novo;
}

AST *criarASTComandoIf(AST *condicao, AST *comando) {
  AST *novo = criarAST(ComIf);
  novo->valor.comandoIf.condicao = condicao;
  novo->valor.comandoIf.comando = comando;
  return novo;
}

AST *criarASTComandoElse(AST *comando) {
  AST *novo = criarAST(ComElse);
  novo->valor.comandoElse.comando = comando;
  return novo;
}

AST *criarASTComandoWhile(AST *condicao, AST *comando) {
  AST *novo = criarAST(ComWhile);
  novo->valor.comandoWhile.condicao = condicao;
  novo->valor.comandoWhile.comando = comando;
  return novo;
}

AST *criarASTComandoAtribuicao(char *id, AST *expressao) {
  AST *novo = criarAST(ComAtribuicao);
  novo->valor.comandoAtribuicao.id = id;
  novo->valor.comandoAtribuicao.expressao = expressao;
  return novo;
}

AST *criarASTComandoPrint(AST *expressao) {
  AST *novo = criarAST(ComPrint);
  novo->valor.comandoPrint.expressao = expressao;
  return novo;
}

AST *criarASTComandoRead(char *id) {
  AST *novo = criarAST(ComRead);
  novo->valor.comandoRead.id = id;
  return novo;
}

AST *criarASTComandoReturn(AST *expressao) {
  AST *novo = criarAST(ComReturn);
  novo->valor.comandoReturn.expressao = expressao;
  return novo;
}

AST *criarASTComandoChamadaFuncao(char *id, cvector_vector_type(AST *) * parametros) {
  AST *novo = criarAST(ComChamadaFuncao);
  novo->valor.comandoChamadaFuncao.id = id;
  novo->valor.comandoChamadaFuncao.parametros = parametros;
  return novo;
}

AST *criarASTExpressaoAritmetica(enum OperadoresAritmeticos operador, AST *esquerda, AST *direita) {
  AST *novo = criarAST(ExprAritmetica);
  novo->valor.expressaoAritmetica.operador = operador;
  novo->valor.expressaoAritmetica.esquerda = esquerda;
  novo->valor.expressaoAritmetica.direita = direita;
  return novo;
}

AST *criarASTExpressaoRelacional(enum OperadoresRelacionais operador, AST *esquerda, AST *direita) {
  AST *novo = criarAST(ExprRelacional);
  novo->valor.expressaoRelacional.operador = operador;
  novo->valor.expressaoRelacional.esquerda = esquerda;
  novo->valor.expressaoRelacional.direita = direita;
  return novo;
}

AST *criarASTExpressaoLogica(enum OperadoresLogicos operador, AST *esquerda, AST *direita) {
  AST *novo = criarAST(ExprLogica);
  novo->valor.expressaoLogica.operador = operador;
  novo->valor.expressaoLogica.esquerda = esquerda;
  novo->valor.expressaoLogica.direita = direita;
  return novo;
}

AST *criarASTExpressaoValorInteiro(int valor) {
  AST *novo = criarAST(ExprValorInteiro);
  novo->valor.expressaoValorInteiro.valor = valor;
  return novo;
}

AST *criarASTExpressaoValorFloat(float valor) {
  AST *novo = criarAST(ExprValorFloat);
  novo->valor.expressaoValorFloat.valor = valor;
  return novo;
}

AST *criarASTExpressaoValorString(char *valor) {
  AST *novo = criarAST(ExprValorString);
  novo->valor.expressaoValorString.valor = valor;
  return novo;
}
