#include "ast.h"

AST *criar_programa(AST *funcoes, AST *blocoPrincipal) {
  AST *programa = (AST *)malloc(sizeof(AST));
  programa->tipo = Arvore;
  programa->token.tipo = Program;
  programa->u.arvore.left = funcoes;
  programa->u.arvore.right = blocoPrincipal;
  return programa;
}

AST *criar_lista_funcoes(AST *funcoes, AST *funcao) {
  if (funcoes == NULL) {
    funcoes = (AST *)malloc(sizeof(AST));
    funcoes->tipo = Vetor;
    funcoes->token.tipo = DeclarationFunctionList;
    funcoes->u.filhos = NULL;
  }
  cvector_push_back(funcoes->u.filhos, funcao);
  return funcoes;
}

AST *criar_parametro(AST *tipo, AST *id) {
  AST *parametro = (AST *)malloc(sizeof(AST));
  parametro->tipo = Arvore;
  parametro->token.tipo = DeclarationParameter;
  parametro->u.arvore.left = tipo;
  parametro->u.arvore.right = id;
  return parametro;
}

AST *criar_declaracao_parametros(AST *declaracaoAnterior, AST *parametro) {
  if (declaracaoAnterior == NULL) {
    declaracaoAnterior = (AST *)malloc(sizeof(AST));
    declaracaoAnterior->tipo = Vetor;
    declaracaoAnterior->token.tipo = DeclarationParameterList;
    declaracaoAnterior->u.filhos = NULL;
  }
  cvector_push_back(declaracaoAnterior->u.filhos, parametro);
  return declaracaoAnterior;
}

AST *criar_funcao(AST *tipo, AST *id, AST *parametros, AST *bloco) {
  AST *funcao = (AST *)malloc(sizeof(AST));
  funcao->tipo = Vetor;
  funcao->token.tipo = DeclarationFunction;
  funcao->u.filhos = NULL;
  cvector_push_back(funcao->u.filhos, tipo);
  cvector_push_back(funcao->u.filhos, id);
  cvector_push_back(funcao->u.filhos, parametros);
  cvector_push_back(funcao->u.filhos, bloco);
  return funcao;
}

AST *criar_funcao_input_void(AST *tipo, AST *id, AST *bloco) {
  AST *funcao = (AST *)malloc(sizeof(AST));
  funcao->tipo = Vetor;
  funcao->token.tipo = DeclarationFunction;
  funcao->u.filhos = NULL;
  cvector_push_back(funcao->u.filhos, tipo);
  cvector_push_back(funcao->u.filhos, id);
  cvector_push_back(funcao->u.filhos, bloco);
  return funcao;
}

AST *criar_tipo(enum TipoDados tipo) {
  AST *tipoAST = (AST *)malloc(sizeof(AST));
  tipoAST->tipo = Folha;
  tipoAST->token.tipo = Type;
  tipoAST->token.u.type.tipo = tipo;
  return tipoAST;
}

AST *criar_constante(const char *input, enum TipoDados tipo) {
  AST *constante = (AST *)malloc(sizeof(AST));
  constante->tipo = Folha;
  constante->token.tipo = Constant;

  switch (tipo) {
    case Int: {
      constante->token.u.constante.tipo = Int;
      constante->token.u.constante.valor.inteiro = atoi(input);
    } break;
    case Float: {
      constante->token.u.constante.tipo = Float;
      constante->token.u.constante.valor.flutuante = atof(input);
    } break;
    case String: {
      constante->token.u.constante.tipo = String;
      str_assign(&constante->token.u.constante.valor.string,
                 str_acquire(input));
    } break;
    case Void: {
      constante->token.u.constante.tipo = Void;
      constante->token.u.constante.valor.nada = NULL;
    } break;

    default:
      break;
  }

  return constante;
}

AST *criar_idenfier(const char *input) {
  AST *idenfier = (AST *)malloc(sizeof(AST));
  idenfier->tipo = Folha;
  idenfier->token.tipo = Identifier;
  str_assign(&idenfier->token.u.idenfier.id, str_acquire(input));

  return idenfier;
}

AST *criar_bloco_principal(AST *declaracoes, AST *comandos) {
  AST *blocoPrincipal = (AST *)malloc(sizeof(AST));
  blocoPrincipal->token.tipo = Block;
  blocoPrincipal->tipo = Arvore;
  blocoPrincipal->u.arvore.left = declaracoes;
  blocoPrincipal->u.arvore.right = comandos;
  return blocoPrincipal;
}

AST *criar_lista_identificadores(AST *identificadores, AST *id) {
  if (identificadores == NULL) {
    identificadores = (AST *)malloc(sizeof(AST));
    identificadores->tipo = Vetor;
    identificadores->token.tipo = IdentifierList;
    identificadores->u.filhos = NULL;
  }
  cvector_push_back(identificadores->u.filhos, id);
  return identificadores;
}

AST *criar_declaracao(AST *tipo, AST *ids) {
  AST *declaracao = (AST *)malloc(sizeof(AST));
  declaracao->tipo = Arvore;
  declaracao->token.tipo = DeclarationVariable;
  declaracao->u.arvore.left = tipo;
  declaracao->u.arvore.right = ids;
  return declaracao;
}

AST *criar_declaracoes(AST *declaracoes, AST *declaracao) {
  if (declaracoes == NULL) {
    declaracoes = (AST *)malloc(sizeof(AST));
    declaracoes->tipo = Vetor;
    declaracoes->token.tipo = DeclarationList;
    declaracoes->u.filhos = NULL;
  }
  cvector_push_back(declaracoes->u.filhos, declaracao);
  return declaracoes;
}

AST *criar_lista_comandos(AST *comandos, AST *comando) {
  if (comandos == NULL) {
    comandos = (AST *)malloc(sizeof(AST));
    comandos->tipo = Vetor;
    comandos->token.tipo = CommandList;
    comandos->u.filhos = NULL;
  }
  cvector_push_back(comandos->u.filhos, comando);
  return comandos;
}

AST *criar_comando_if(AST *condicao, AST *blocoIf, AST *blocoElse) {
  AST *comandoIf = (AST *)malloc(sizeof(AST));
  comandoIf->tipo = Arvore;
  comandoIf->token.tipo = If;
  comandoIf->u.arvore.left = condicao;
  comandoIf->u.arvore.right = (AST *)malloc(sizeof(AST));
  comandoIf->u.arvore.right->tipo = Arvore;
  comandoIf->u.arvore.right->token.tipo = If;
  comandoIf->u.arvore.right->u.arvore.left = blocoIf;
  comandoIf->u.arvore.right->u.arvore.right = blocoElse;

  // Dando nomes aos bois
  if (comandoIf->u.arvore.right->u.arvore.left) {
    comandoIf->u.arvore.right->u.arvore.left->token.tipo = IfBlock;
  }

  if (comandoIf->u.arvore.right->u.arvore.right) {
    comandoIf->u.arvore.right->u.arvore.right->token.tipo = ElseBlock;
  }
  return comandoIf;
}

AST *criar_comando_while(AST *condicao, AST *bloco) {
  AST *comandoWhile = (AST *)malloc(sizeof(AST));
  comandoWhile->tipo = Arvore;
  comandoWhile->token.tipo = While;
  comandoWhile->u.arvore.left = condicao;
  comandoWhile->u.arvore.right = bloco;

  return comandoWhile;
}

AST *criar_comando_atribuicao(AST *id, AST *expressao) {
  AST *comandoAtribuicao = (AST *)malloc(sizeof(AST));
  comandoAtribuicao->tipo = Arvore;
  comandoAtribuicao->token.tipo = Assignment;
  comandoAtribuicao->u.arvore.left = id;
  comandoAtribuicao->u.arvore.right = expressao;

  return comandoAtribuicao;
}

AST *criar_comando_read(AST *id) {
  AST *comandoRead = (AST *)malloc(sizeof(AST));
  comandoRead->tipo = Arvore;
  comandoRead->token.tipo = Read;
  comandoRead->u.arvore.left = id;
  comandoRead->u.arvore.right = NULL;

  return comandoRead;
}

AST *criar_comando_print(AST *expressao) {
  AST *comandoPrint = (AST *)malloc(sizeof(AST));
  comandoPrint->tipo = Arvore;
  comandoPrint->token.tipo = Print;
  comandoPrint->u.arvore.left = expressao;
  comandoPrint->u.arvore.right = NULL;

  return comandoPrint;
}

AST *criar_comando_chamada_funcao(AST *id, AST *parametros) {
  AST *comandoChamadaFuncao = (AST *)malloc(sizeof(AST));
  comandoChamadaFuncao->tipo = Arvore;
  comandoChamadaFuncao->token.tipo = FunctionCall;
  comandoChamadaFuncao->u.arvore.left = id;
  comandoChamadaFuncao->u.arvore.right = parametros;

  return comandoChamadaFuncao;
}

AST *criar_lista_parametros(AST *parametros, AST *parametro) {
  if (parametros == NULL) {
    parametros = (AST *)malloc(sizeof(AST));
    parametros->tipo = Vetor;
    parametros->token.tipo = ParameterList;
    parametros->u.filhos = NULL;
  }
  cvector_push_back(parametros->u.filhos, parametro);
  return parametros;
}

AST *criar_expressao_logica(enum LogicalOperators operator, AST * left,
                            AST *right) {
  AST *expressaoLogica = (AST *)malloc(sizeof(AST));
  expressaoLogica->tipo = Arvore;
  expressaoLogica->token.tipo = ExpressionLogical;
  expressaoLogica->u.arvore.left = left;
  expressaoLogica->u.arvore.right = right;
  expressaoLogica->token.u.logicalExpression.operator= operator;
  return expressaoLogica;
}

AST *criar_expressao_aritmetica(enum ArithmeticOperators operator, AST * left,
                                AST *right) {
  AST *expressaoAritmetica = (AST *)malloc(sizeof(AST));
  expressaoAritmetica->tipo = Arvore;
  expressaoAritmetica->token.tipo = ExpressionArithmetic;
  expressaoAritmetica->u.arvore.left = left;
  expressaoAritmetica->u.arvore.right = right;
  expressaoAritmetica->token.u.arithmeticExpression.operator= operator;
  return expressaoAritmetica;
}

AST *criar_expressao_relacional(enum RelationsOperators operator, AST * left,
                                AST *right) {
  AST *expressaoRelacional = (AST *)malloc(sizeof(AST));
  expressaoRelacional->tipo = Arvore;
  expressaoRelacional->token.tipo = ExpressionRelational;
  expressaoRelacional->u.arvore.left = left;
  expressaoRelacional->u.arvore.right = right;
  expressaoRelacional->token.u.relationalExpression.operator= operator;
  return expressaoRelacional;
}

AST *criar_retorno_funcao(AST *expressao) {
  AST *retorno = (AST *)malloc(sizeof(AST));
  retorno->tipo = Arvore;
  retorno->token.tipo = Return;
  retorno->u.arvore.left = expressao ? expressao : criar_constante(NULL, Void);
  retorno->u.arvore.right = NULL;
  return retorno;
}
