#ifndef DE9912FB_2382_4387_8C42_BD3C683D6A32
#define DE9912FB_2382_4387_8C42_BD3C683D6A32

#include <stdio.h>
#include <stdlib.h>
#include <utils/cvector.h>

enum NoTipo { NoComando, NoExpressao };

enum Comando {
  ComandoIF,
  ComandoWhile,
  ComandoAtribuicao,
  ComandoDeclaracao,
  ComandoChamadaFuncao,
  ComandoRetorno,
  ComandoPrint,
  ComandoRead
};

enum Expressao { ExpressaoAritmetica, ExpressaoRelacional, ExpressaoLogica };

enum OperadorAr { OpSoma, OpSub, OpMult, OpDiv, OpPot };

enum OperadorRel { OpIgual, OpDiferente, OpMaior, OpMenor, OpMaiorIgual, OpMenorIgual };

enum OperadorLog { OpAnd, OpOr };

enum OperadorIgualdade { OpEqual, OpNot };

typedef union {
  int inteiro;
  float flutuante;
  char *string;
} Valor;

struct ASTNode {
  enum NoTipo tipo;
  union {
    enum Comando comando;
    struct {
      enum Expressao tipoExpressao;
      union {
        enum OperadorAr operadorAr;
        enum OperadorRel operadorRel;
        enum OperadorLog operadorLog;
        enum OperadorIgualdade operadorIgualdade;
      } operador;
      Valor valor;
    } expressao;
  } tipoNo;

  union {
    struct {
      struct ASTNode *esquerda;
      struct ASTNode *direita;
    } arvore;
    cvector_vector_type(struct ASTNode *) vetor;
  } instrucao;
};

#endif