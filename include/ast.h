#ifndef DE9912FB_2382_4387_8C42_BD3C683D6A32
#define DE9912FB_2382_4387_8C42_BD3C683D6A32

#include <stdio.h>
#include <stdlib.h>
#include <utils/cvector.h>

enum NoTipo { NoComando, NoExpressao, NoValor };

enum ComandoTipo {
  ComandoIF,
  ComandoWhile,
  ComandoAtribuicao,
  ComandoDeclaracao,
  ComandoChamadaFuncao,
  ComandoRetorno,
  ComandoPrint,
  ComandoRead
};

enum Expressao { ExpressaoAritmetica, ExpressaoRelacional, ExpressaoLogica, ValorPuro, ExpressaoIgualdade };

enum OperadorAr { OpSoma, OpSub, OpMult, OpDiv, OpPot };

enum OperadorRel { OpIgual, OpDiferente, OpMaior, OpMenor, OpMaiorIgual, OpMenorIgual };

enum OperadorLog { OpAnd, OpOr };

enum OperadorIgualdade { OpEqual, OpNot };

enum Valor { Int, Float, String };

struct NoExpressao {
  enum Expressao tipo;
  union {
    struct {
      enum OperadorAr operador;
      struct NoExpressao *esquerda;
      struct NoExpressao *direita;
    } aritmetica;
    struct {
      enum OperadorRel operador;
      struct NoExpressao *esquerda;
      struct NoExpressao *direita;
    } relacional;
    struct {
      enum OperadorLog operador;
      struct NoExpressao *esquerda;
      struct NoExpressao *direita;
    } logica;
    struct {
      enum OperadorIgualdade operador;
      struct NoExpressao *esquerda;
      struct NoExpressao *direita;
    } igualdade;
    struct {
      enum Valor tipo;
      union {
        int valorInt;
        float valorFloat;
        char *valorString;
      } valor;
    } valor;
  } expressao;
};

struct NoComando {
  enum ComandoTipo tipo;
  union {
    struct {
      struct NoExpressao *condicao;
      cvector_vector_type(struct NoComando *) ifComandos;
      cvector_vector_type(struct NoComando *) elseComandos;
    } ifComando;
    struct {
      struct NoExpressao *condicao;
      cvector_vector_type(struct NoComando *) comandos;
    } whileComando;
    struct {
      char *id;
      struct NoExpressao *expressao;
    } atribuicao;
    struct {
      char *id;
      struct NoExpressao *expressao;
    } declaracao;
    struct {
      char *id;
      cvector_vector_type(struct NoExpressao *) argumentos;
    } chamadaFuncao;
    struct {
      struct NoExpressao *expressao;
    } retorno;
    struct {
      struct NoExpressao *expressao;
    } print;
    struct {
      char *id;
    } read;
  } comando;
};

struct Genesis {
  cvector_vector_type(struct NoComando *) comandos;
};

// Funções para criar os nós da AST

// Comandos
struct NoComando *criarComandoIf(struct NoExpressao *condicao, cvector_vector_type(struct NoComando *) ifComandos,
                                 cvector_vector_type(struct NoComando *) elseComandos);
struct NoComando *criarComandoWhile(struct NoExpressao *condicao, cvector_vector_type(struct NoComando *) comandos);
struct NoComando *criarComandoAtribuicao(char *id, struct NoExpressao *expressao);
struct NoComando *criarComandoDeclaracao(char *id, struct NoExpressao *expressao);
struct NoComando *criarComandoChamadaFuncao(char *id, cvector_vector_type(struct NoExpressao *) argumentos);
struct NoComando *criarComandoRetorno(struct NoExpressao *expressao);
struct NoComando *criarComandoPrint(struct NoExpressao *expressao);
struct NoComando *criarComandoRead(char *id);

// Expressões
struct NoExpressao *criarExpressaoAritmetica(enum OperadorAr operador, struct NoExpressao *esquerda, struct NoExpressao *direita);
struct NoExpressao *criarExpressaoRelacional(enum OperadorRel operador, struct NoExpressao *esquerda, struct NoExpressao *direita);
struct NoExpressao *criarExpressaoLogica(enum OperadorLog operador, struct NoExpressao *esquerda, struct NoExpressao *direita);
struct NoExpressao *criarExpressaoIgualdade(enum OperadorIgualdade operador, struct NoExpressao *esquerda, struct NoExpressao *direita);
struct NoExpressao *criarValorInt(int valor);
struct NoExpressao *criarValorFloat(float valor);
struct NoExpressao *criarValorString(char *valor);

// Funções para imprimir a AST
void imprimirNoComando(struct NoComando *no, int nivel);
void imprimirNoExpressao(struct NoExpressao *no, int nivel);
void imprimirAST(struct Genesis *raiz);

#endif