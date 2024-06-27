#include "ast.h"

AST *criar_ast(const enum TipoToken tipo) {
  AST *aux = (AST *)malloc(sizeof(AST));
  aux->tipo = tipo;

  return aux;
}

AST *criar_int(const int i) {
  AST *aux = criar_ast(ConsanteInt);
  aux->inteiro = i;

  return aux;
}

AST *criar_float(const float f) {
  AST *aux = criar_ast(ConsanteFloat);
  aux->flutuante = f;

  return aux;
}

AST *criar_string(const char *string) {
  AST *aux = criar_ast(ConsanteString);
  aux->string = string;

  return aux;
}

AST *criar_id(const char *id) {
  AST *aux = criar_ast(Id);
  aux->id = id;

  return aux;
}

AST *transicao(AST *a, AST *b) {
  if (a && a->tipo == SituacaoTransicao) {
    cvector_push_back(a->transicao, b);

    return a;
  } else if (!a) {
    a = criar_ast(SituacaoTransicao);
    a->tipo = SituacaoTransicao;
    cvector_push_back(a->transicao, b);

    return a;
  } else {
    exibir_erro("Funcao 'transicao' sendo usada de forma errada");
  }

  return NULL;
}

AST *criar_operacao_aritmetica(AST *a, AST *b, const char *simbolo) {
  AST *aux = criar_ast(ExpressaoAritmetica);

  aux->aritmetica.simbolo = simbolo;
  aux->aritmetica.esquerda = a;
  aux->aritmetica.direita = b;

  return aux;
}
AST *criar_operacao_relacional(AST *a, AST *b, const char *simbolo) {
  AST *aux = criar_ast(ExpressaoAritmetica);

  aux->aritmetica.simbolo = simbolo;
  aux->aritmetica.esquerda = a;
  aux->aritmetica.direita = b;

  return aux;
}

AST *criar_operacao_logica(AST *a, AST *b, const char *simbolo) {
  AST *aux = criar_ast(ExpressaoLogica);

  aux->aritmetica.simbolo = simbolo;
  aux->aritmetica.esquerda = a;
  aux->aritmetica.direita = b;

  return aux;
}

AST *criar_chamada_funcao(AST *id, AST *parametros) {
  AST *aux = criar_ast(ChamadaFuncao);

  aux->chamada_funcao.id = id->id;
  aux->chamada_funcao.parametros = parametros->transicao;

  return aux;
}

AST *criar_read(AST *id) {
  AST *aux = criar_ast(Read);
  aux->read.id = id->id;
  return aux;
}

AST *criar_print(AST *instancia) {
  AST *aux = criar_ast(Print);
  aux->print.parametro = instancia;

  return aux;
}

AST *criar_atribuicao(AST *id, AST *expr) {
  AST *aux = criar_ast(Atribuicao);
  aux->atribuicao.id = id->id;
  aux->atribuicao.expressao = expr;

  return aux;
}

AST *criar_enquanto(AST *expr, AST *bloco) {
  AST *aux = criar_ast(While);

  aux->while_.codicao = expr->logica;
  aux->while_.bloco = bloco->transicao;

  return aux;
}

AST *criar_if(AST *expr, AST *blocoIf, AST *blocoElse) {
  AST *aux = criar_ast(If);

  aux->if_.codicao = expr->logica;
  aux->if_.blocoIf = blocoIf->transicao;
  aux->if_.blocoElse = blocoElse ? blocoElse->transicao : NULL;

  return aux;
}

AST *criar_retorno(AST *ret, enum TipoDado tipo) {
  AST *retorno = criar_ast(Retorno);

  retorno->retorno.ret = ret;
  retorno->retorno.tipo = tipo;

  return retorno;
}

AST *criar_tipo(enum TipoDado tipo) {
  AST *aux = criar_ast(Tipo);

  aux->declaracao_tipo = tipo;

  return aux;
}

AST *criar_variavel(AST *tipo, AST *lista_ids) {
  AST *aux = criar_ast(Variavel);

  aux->variavel.tipo = tipo->declaracao_tipo;
  aux->variavel.ids = lista_ids->transicao;

  return aux;
}

AST *criar_bloco(AST *declaracoes, AST *comandos) {
  AST *aux = criar_ast(Bloco);

  aux->bloco.declaracoes = declaracoes->transicao;
  aux->bloco.comandos = comandos->transicao;

  return aux;
}

AST *criar_parametro(AST *tipo, AST *id) {
  AST *aux = criar_ast(Parametro);

  aux->parametro.tipo = tipo->declaracao_tipo;
  aux->parametro.id = id->id;

  return aux;
}

AST *criar_funcao(AST *retorno, AST *id, AST *parametros, AST *bloco) {
  AST *aux = criar_ast(Funcao);

  aux->funcao.retorno = retorno->declaracao_tipo;
  aux->funcao.id = id->id;
  aux->funcao.parametros = parametros->transicao;
  aux->funcao.bloco = bloco;

  return aux;
}

AST *criar_programa(AST *funcoes, AST *main) {
  AST *aux = criar_ast(Programa);

  aux->programa.funcoes = funcoes ? funcoes->transicao : NULL;
  aux->programa.bloco = main;

  return aux;
}