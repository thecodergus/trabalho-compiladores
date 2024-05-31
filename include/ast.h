#ifndef DE9912FB_2382_4387_8C42_BD3C683D6A32
#define DE9912FB_2382_4387_8C42_BD3C683D6A32

#include <stdio.h>
#include <stdlib.h>
#include <utils/cvector.h>
#include <utils/str.h>

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
  str string;
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

// Cria um novo nó de comando vazio
struct ASTNode *criar_no_comando(enum Comando comando);

// Cria um novo nó de expressão vazio
struct ASTNode *criar_no_expressao(enum Expressao tipo_expressao);

// Cria um novo nó de operador aritmético
struct ASTNode *criar_no_operador_aritmetico(enum OperadorAr operador, struct ASTNode *esquerda, struct ASTNode *direita);

// Cria um novo nó de operador relacional
struct ASTNode *criar_no_operador_relacional(enum OperadorRel operador, struct ASTNode *esquerda, struct ASTNode *direita);

// Cria um novo nó de operador lógico
struct ASTNode *criar_no_operador_logico(enum OperadorLog operador, struct ASTNode *esquerda, struct ASTNode *direita);

// Cria um novo nó de operador de igualdade
struct ASTNode *criar_no_operador_igualdade(enum OperadorIgualdade operador, struct ASTNode *esquerda, struct ASTNode *direita);

// Adiciona um filho à lista de filhos de um nó
void adicionar_filho(struct ASTNode *pai, struct ASTNode *filho);

// Imprime a árvore sintática abstrata em forma de texto
void imprimir_ast(struct ASTNode *no);

#endif