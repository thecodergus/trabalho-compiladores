#include "astUtils.h"

AST criar_programa(vector(struct AST) funcoes, vector(struct AST) blocoPrincipal) {
  AST programa;
  programa.tipo = Vetor;
  programa.token.tipo = Program;
  programa.token.u.nada = NULL;
  programa.u.filhos = concat_vectors(funcoes, blocoPrincipal);

  return programa;
}

vector(AST) concat_vectors(vector(AST) a, vector(AST) b) {
  vector(AST) result = NULL;

  if (a) {
    for (AST *it = cvector_begin(a); it != cvector_end(a); ++it) {
      cvector_push_back(result, *it);
    }
  }

  if (b) {
    for (AST *it = cvector_begin(b); it != cvector_end(b); ++it) {
      cvector_push_back(result, *it);
    }
  }

  return result;
}

AST criar_lista_funcoes(AST funcoes, AST funcao) {
  AST listaFuncoes;
  listaFuncoes.tipo = Vetor;
  listaFuncoes.token.tipo = DeclarationFunction;
  listaFuncoes.u.filhos = funcoes.u.filhos;

  cvector_push_back(listaFuncoes.u.filhos, funcoes);

  return listaFuncoes;
}

AST criar_parametro(AST tipo, AST id) {
  AST parametro;
  parametro.tipo = Arvore;
  parametro.token.tipo = Parameter;

  AST *left = malloc(sizeof(AST));
  AST *right = malloc(sizeof(AST));

  *left = tipo;
  *right = id;

  parametro.u.arvore.left = left;
  parametro.u.arvore.right = right;

  return parametro;
}

AST criar_declaracao_parametros(AST declaracaoAnterior, AST parametro) {
  AST declaracaoParametros;
  declaracaoParametros.tipo = Vetor;
  declaracaoParametros.token.tipo = Parameters;
  declaracaoParametros.u.filhos = declaracaoAnterior.u.filhos;

  cvector_push_back(declaracaoParametros.u.filhos, parametro);

  return declaracaoParametros;
}

AST criar_funcao(AST tipo, AST id, AST parametros, AST bloco) {
  AST funcao;
  funcao.tipo = Vetor;
  funcao.token.tipo = Function;
  funcao.u.filhos = NULL;

  cvector_push_back(funcao.u.filhos, tipo);
  cvector_push_back(funcao.u.filhos, id);
  cvector_push_back(funcao.u.filhos, parametros);
  cvector_push_back(funcao.u.filhos, bloco);

  return funcao;
}

AST criar_funcao_input_void(AST tipo, AST id, AST bloco) {
  AST funcao;
  funcao.tipo = Vetor;
  funcao.token.tipo = Function;
  funcao.u.filhos = NULL;

  cvector_push_back(funcao.u.filhos, tipo);
  cvector_push_back(funcao.u.filhos, id);
  cvector_push_back(funcao.u.filhos, bloco);

  return funcao;
}

AST criar_tipo(enum TipoDados tipo) {
  AST tipoAST;
  tipoAST.tipo = Folha;
  tipoAST.token.tipo = Type;
  tipoAST.token.u.type.tipo = tipo;
  tipoAST.u.arvore.left = NULL;
  tipoAST.u.arvore.right = NULL;

  return tipoAST;
}

AST criar_constante_int(const char *input) {
  AST tipoInt;
  tipoInt.tipo = Folha;
  tipoInt.token.tipo = ConstantInt;
  tipoInt.token.u.constInt.valor = atoi(input);
  tipoInt.u.arvore.left = NULL;
  tipoInt.u.arvore.right = NULL;

  return tipoInt;
}

AST criar_constante_float(const char *input) {
  AST tipoFloat;
  tipoFloat.tipo = Folha;
  tipoFloat.token.tipo = ConstantFloat;
  tipoFloat.token.u.constFloat.valor = atof(input);
  tipoFloat.u.arvore.left = NULL;
  tipoFloat.u.arvore.right = NULL;

  return tipoFloat;
}

AST criar_constante_string(const char *input) {
  AST tipoString;
  tipoString.tipo = Folha;
  tipoString.token.tipo = ConstantString;
  tipoString.token.u.constString.valor = str_acquire(input);
  tipoString.u.arvore.left = NULL;
  tipoString.u.arvore.right = NULL;

  return tipoString;
}

AST criar_constante_void() {
  AST tipoVoid;
  tipoVoid.tipo = Folha;
  tipoVoid.token.tipo = Void;
  tipoVoid.token.u.nada = NULL;
  tipoVoid.u.arvore.left = NULL;
  tipoVoid.u.arvore.right = NULL;

  return tipoVoid;
}
