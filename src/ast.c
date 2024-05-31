#include "ast.h"

// Cria um novo nó de comando
struct ASTNode* criar_no_comando(enum Comando comando) {
  struct ASTNode* no = (struct ASTNode*)malloc(sizeof(struct ASTNode));
  no->tipo = NoComando;
  no->tipoNo.comando = comando;
  no->instrucao.vetor = NULL;
  return no;
}

// Cria um novo nó de expressão
struct ASTNode* criar_no_expressao(enum Expressao tipo, Valor valor) {
  struct ASTNode* no = (struct ASTNode*)malloc(sizeof(struct ASTNode));
  no->tipo = NoExpressao;
  no->tipoNo.expressao.tipoExpressao = tipo;
  no->tipoNo.expressao.valor = valor;
  no->instrucao.arvore.esquerda = NULL;
  no->instrucao.arvore.direita = NULL;
  return no;
}

// Cria um novo nó de operador aritmético
struct ASTNode* criar_no_operador_aritmetico(enum OperadorAr operador, struct ASTNode* esquerda, struct ASTNode* direita) {
  struct ASTNode* no = (struct ASTNode*)malloc(sizeof(struct ASTNode));
  no->tipo = NoExpressao;
  no->tipoNo.expressao.tipoExpressao = ExpressaoAritmetica;
  no->tipoNo.expressao.operador.operadorAr = operador;
  no->instrucao.arvore.esquerda = esquerda;
  no->instrucao.arvore.direita = direita;
  return no;
}

// Cria um novo nó de operador relacional
struct ASTNode* criar_no_operador_relacional(enum OperadorRel operador, struct ASTNode* esquerda, struct ASTNode* direita) {
  struct ASTNode* no = (struct ASTNode*)malloc(sizeof(struct ASTNode));
  no->tipo = NoExpressao;
  no->tipoNo.expressao.tipoExpressao = ExpressaoRelacional;
  no->tipoNo.expressao.operador.operadorRel = operador;
  no->instrucao.arvore.esquerda = esquerda;
  no->instrucao.arvore.direita = direita;
  return no;
}

// Cria um novo nó de operador lógico
struct ASTNode* criar_no_operador_logico(enum OperadorLog operador, struct ASTNode* esquerda, struct ASTNode* direita) {
  struct ASTNode* no = (struct ASTNode*)malloc(sizeof(struct ASTNode));
  no->tipo = NoExpressao;
  no->tipoNo.expressao.tipoExpressao = ExpressaoLogica;
  no->tipoNo.expressao.operador.operadorLog = operador;
  no->instrucao.arvore.esquerda = esquerda;
  no->instrucao.arvore.direita = direita;
  return no;
}

// Adiciona um nó à lista de nós de um comando
void adicionar_no_comando(struct ASTNode* comando, struct ASTNode* no) {
  if (comando->instrucao.vetor == NULL) {
    comando->instrucao.vetor = cvector_create(sizeof(struct ASTNode*));
  }
  cvector_push_back(comando->instrucao.vetor, &no);
}

// Imprime a árvore sintática em pré-ordem
void imprimir_arvore(struct ASTNode* no, int nivel) {
  for (int i = 0; i < nivel; i++) {
    printf("  ");
  }
  if (no->tipo == NoComando) {
    switch (no->tipoNo.comando) {
      case ComandoIF:
        printf("ComandoIF\\n");
        break;
      case ComandoWhile:
        printf("ComandoWhile\\n");
        break;
      case ComandoAtribuicao:
        printf("ComandoAtribuicao\\n");
        break;
      case ComandoDeclaracao:
        printf("ComandoDeclaracao\\n");
        break;
      case ComandoChamadaFuncao:
        printf("ComandoChamadaFuncao\\n");
        break;
      case ComandoRetorno:
        printf("ComandoRetorno\\n");
        break;
      case ComandoPrint:
        printf("ComandoPrint\\n");
        break;
      case ComandoRead:
        printf("ComandoRead\\n");
        break;
    }
    for (int i = 0; i < cvector_size(no->instrucao.vetor); i++) {
      struct ASTNode* filho = *(struct ASTNode**)cvector_at(no->instrucao.vetor, i);
      imprimir_arvore(filho, nivel + 1);
    }
  } else {
    switch (no->tipoNo.expressao.tipoExpressao) {
      case ExpressaoAritmetica:
        printf("ExpressaoAritmetica: ");
        switch (no->tipoNo.expressao.operador.operadorAr) {
          case OpSoma:
            printf("+\\n");
            break;
          case OpSub:
            printf("-\\n");
            break;
          case OpMult:
            printf("*\\n");
            break;
          case OpDiv:
            printf("/\\n");
            break;
          case OpPot:
            printf("^\\n");
            break;
        }
        imprimir_arvore(no->instrucao.arvore.esquerda, nivel + 1);
        imprimir_arvore(no->instrucao.arvore.direita, nivel + 1);
        break;
      case ExpressaoRelacional:
        printf("ExpressaoRelacional: ");
        switch (no->tipoNo.expressao.operador.operadorRel) {
          case OpIgual:
            printf("==\\n");
            break;
          case OpDiferente:
            printf("!=\\n");
            break;
          case OpMaior:
            printf(">\\n");
            break;
          case OpMenor:
            printf("<\\n");
            break;
          case OpMaiorIgual:
            printf(">=\\n");
            break;
          case OpMenorIgual:
            printf("<=\\n");
            break;
        }
        imprimir_arvore(no->instrucao.arvore.esquerda, nivel + 1);
        imprimir_arvore(no->instrucao.arvore.direita, nivel + 1);
        break;
      case ExpressaoLogica:
        printf("ExpressaoLogica: ");
        switch (no->tipoNo.expressao.operador.operadorLog) {
          case OpAnd:
            printf("&&\\n");
            break;
          case OpOr:
            printf("||\\n");
            break;
        }
        imprimir_arvore(no->instrucao.arvore.esquerda, nivel + 1);
        imprimir_arvore(no->instrucao.arvore.direita, nivel + 1);
        break;
    }
  }
}