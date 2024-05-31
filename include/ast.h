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
    union {
      int valorInt;
      float valorFloat;
      char *valorString;
    } valor;
  } expressao;
};

struct NoComando {
  enum ComandoTipo tipo;
  union {
    struct {
      struct NoExpressao *condicao;
      cvector_vector_type(struct NoComando *) comandos;
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

struct No {
  enum NoTipo tipo;
  union {
    struct NoComando *comando;
    struct NoExpressao *expressao;
    union {
      int valorInt;
      float valorFloat;
      char *valorString;
    } valor;
  } no;
};

#endif