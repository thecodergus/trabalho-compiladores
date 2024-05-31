#include "ast.h"

// Comandos
struct NoComando *criarComandoIf(struct NoExpressao *condicao, cvector_vector_type(struct NoComando *) ifComandos,
                                 cvector_vector_type(struct NoComando *) elseComandos) {
  struct NoComando *novoComando = (struct NoComando *)malloc(sizeof(struct NoComando));
  novoComando->tipo = ComandoIF;
  novoComando->comando.ifComando.condicao = condicao;
  novoComando->comando.ifComando.ifComandos = ifComandos;
  novoComando->comando.ifComando.elseComandos = elseComandos;
  return novoComando;
}

struct NoComando *criarComandoWhile(struct NoExpressao *condicao, cvector_vector_type(struct NoComando *) comandos) {
  struct NoComando *novoComando = (struct NoComando *)malloc(sizeof(struct NoComando));
  novoComando->tipo = ComandoWhile;
  novoComando->comando.whileComando.condicao = condicao;
  novoComando->comando.whileComando.comandos = comandos;
  return novoComando;
}

struct NoComando *criarComandoAtribuicao(char *id, struct NoExpressao *expressao) {
  struct NoComando *novoComando = (struct NoComando *)malloc(sizeof(struct NoComando));
  novoComando->tipo = ComandoAtribuicao;
  novoComando->comando.atribuicao.id = id;
  novoComando->comando.atribuicao.expressao = expressao;
  return novoComando;
}

struct NoComando *criarComandoDeclaracao(char *id, struct NoExpressao *expressao) {
  struct NoComando *novoComando = (struct NoComando *)malloc(sizeof(struct NoComando));
  novoComando->tipo = ComandoDeclaracao;
  novoComando->comando.declaracao.id = id;
  novoComando->comando.declaracao.expressao = expressao;
  return novoComando;
}

struct NoComando *criarComandoChamadaFuncao(char *id, cvector_vector_type(struct NoExpressao *) argumentos) {
  struct NoComando *novoComando = (struct NoComando *)malloc(sizeof(struct NoComando));
  novoComando->tipo = ComandoChamadaFuncao;
  novoComando->comando.chamadaFuncao.id = id;
  novoComando->comando.chamadaFuncao.argumentos = argumentos;
  return novoComando;
}

struct NoComando *criarComandoRetorno(struct NoExpressao *expressao) {
  struct NoComando *novoComando = (struct NoComando *)malloc(sizeof(struct NoComando));
  novoComando->tipo = ComandoRetorno;
  novoComando->comando.retorno.expressao = expressao;
  return novoComando;
}

struct NoComando *criarComandoPrint(struct NoExpressao *expressao) {
  struct NoComando *novoComando = (struct NoComando *)malloc(sizeof(struct NoComando));
  novoComando->tipo = ComandoPrint;
  novoComando->comando.print.expressao = expressao;
  return novoComando;
}

struct NoComando *criarComandoRead(char *id) {
  struct NoComando *novoComando = (struct NoComando *)malloc(sizeof(struct NoComando));
  novoComando->tipo = ComandoRead;
  novoComando->comando.read.id = id;
  return novoComando;
}

// Expressões
struct NoExpressao *criarExpressaoAritmetica(enum OperadorAr operador, struct NoExpressao *esquerda, struct NoExpressao *direita) {
  struct NoExpressao *novaExpressao = (struct NoExpressao *)malloc(sizeof(struct NoExpressao));
  novaExpressao->tipo = ExpressaoAritmetica;
  novaExpressao->expressao.aritmetica.operador = operador;
  novaExpressao->expressao.aritmetica.esquerda = esquerda;
  novaExpressao->expressao.aritmetica.direita = direita;
  return novaExpressao;
}

struct NoExpressao *criarExpressaoRelacional(enum OperadorRel operador, struct NoExpressao *esquerda, struct NoExpressao *direita) {
  struct NoExpressao *novaExpressao = (struct NoExpressao *)malloc(sizeof(struct NoExpressao));
  novaExpressao->tipo = ExpressaoRelacional;
  novaExpressao->expressao.relacional.operador = operador;
  novaExpressao->expressao.relacional.esquerda = esquerda;
  novaExpressao->expressao.relacional.direita = direita;
  return novaExpressao;
}

struct NoExpressao *criarExpressaoLogica(enum OperadorLog operador, struct NoExpressao *esquerda, struct NoExpressao *direita) {
  struct NoExpressao *novaExpressao = (struct NoExpressao *)malloc(sizeof(struct NoExpressao));
  novaExpressao->tipo = ExpressaoLogica;
  novaExpressao->expressao.logica.operador = operador;
  novaExpressao->expressao.logica.esquerda = esquerda;
  novaExpressao->expressao.logica.direita = direita;
  return novaExpressao;
}

struct NoExpressao *criarExpressaoIgualdade(enum OperadorIgualdade operador, struct NoExpressao *esquerda, struct NoExpressao *direita) {
  struct NoExpressao *novaExpressao = (struct NoExpressao *)malloc(sizeof(struct NoExpressao));
  novaExpressao->tipo = ExpressaoIgualdade;
  novaExpressao->expressao.igualdade.operador = operador;
  novaExpressao->expressao.igualdade.esquerda = esquerda;
  novaExpressao->expressao.igualdade.direita = direita;
  return novaExpressao;
}

struct NoExpressao *criarValorInt(int valor) {
  struct NoExpressao *novaExpressao = (struct NoExpressao *)malloc(sizeof(struct NoExpressao));
  novaExpressao->expressao.valor.tipo = Int;
  novaExpressao->expressao.valor.valor.valorInt = valor;
  return novaExpressao;
}

struct NoExpressao *criarValorFloat(float valor) {
  struct NoExpressao *novaExpressao = (struct NoExpressao *)malloc(sizeof(struct NoExpressao));
  novaExpressao->expressao.valor.tipo = Float;
  novaExpressao->expressao.valor.valor.valorFloat = valor;
  return novaExpressao;
}

struct NoExpressao *criarValorString(char *valor) {
  struct NoExpressao *novaExpressao = (struct NoExpressao *)malloc(sizeof(struct NoExpressao));
  novaExpressao->expressao.valor.tipo = String;
  novaExpressao->expressao.valor.valor.valorString = valor;
  return novaExpressao;
}

// Funções para imprimir a AST

void imprimirNoComando(struct NoComando *no, int nivel) {
  for (int i = 0; i < nivel; i++) {
    printf("  ");
  }
  switch (no->tipo) {
    case ComandoIF:
      printf("ComandoIF\n");
      imprimirNoExpressao(no->comando.ifComando.condicao, nivel + 1);
      for (int i = 0; i < cvector_size(no->comando.ifComando.ifComandos); i++) {
        imprimirNoComando(no->comando.ifComando.ifComandos[i], nivel + 1);
      }
      for (int i = 0; i < cvector_size(no->comando.ifComando.elseComandos); i++) {
        imprimirNoComando(no->comando.ifComando.elseComandos[i], nivel + 1);
      }
      break;
    case ComandoWhile:
      printf("ComandoWhile\n");
      imprimirNoExpressao(no->comando.whileComando.condicao, nivel + 1);
      for (int i = 0; i < cvector_size(no->comando.whileComando.comandos); i++) {
        imprimirNoComando(no->comando.whileComando.comandos[i], nivel + 1);
      }
      break;
    case ComandoAtribuicao:
      printf("ComandoAtribuicao\n");
      for (int i = 0; i < nivel + 1; i++) {
        printf("  ");
      }
      printf("%s\n", no->comando.atribuicao.id);
      imprimirNoExpressao(no->comando.atribuicao.expressao, nivel + 1);
      break;
    case ComandoDeclaracao:
      printf("ComandoDeclaracao\n");
      for (int i = 0; i < nivel + 1; i++) {
        printf("  ");
      }
      printf("%s\n", no->comando.declaracao.id);
      imprimirNoExpressao(no->comando.declaracao.expressao, nivel + 1);
      break;
    case ComandoChamadaFuncao:
      printf("ComandoChamadaFuncao\n");
      for (int i = 0; i < nivel + 1; i++) {
        printf("  ");
      }
      printf("%s\n", no->comando.chamadaFuncao.id);
  }
}

void imprimirNoExpressao(struct NoExpressao *no, int nivel) {}

void imprimirAST(struct Genesis *raiz) {
  for (int i = 0; i < cvector_size(raiz->comandos); i++) {
    imprimirNoComando(raiz->comandos[i], 0);
  }
}