#ifndef DE9912FB_2382_4387_8C42_BD3C683D6A32
#define DE9912FB_2382_4387_8C42_BD3C683D6A32

#include <stdio.h>
#include <stdlib.h>

#include "conversoes.h"
#include "lambda.h"
#include "libraries/cvector.h"
#include "libraries/str.h"
#include "mensagens.h"

#define vector(T) cvector_vector_type(T)

// Tipos de dados
enum TipoDados { Int, Float, String, Void };

// Operadores lógicos
enum LogicalOperators { E, Ou, Nao };

// Operadores relacionais
enum RelationsOperators { Igual, Diferente, Menor, MenorIgual, Maior, MaiorIgual };

// Operadores aritméticos
enum ArithmeticOperators { Soma, Subtracao, Multiplicacao, Divisao, Potencia };

// Criação dos tipos de dados
typedef struct TipoDado TipoDado;

enum TipoToken {
  Program,
  DeclarationFunction,
  DeclarationParameter,
  DeclarationParameterList,
  ParameterList,
  Block,
  DeclarationVariable,
  DeclarationList,
  Assignment,
  If,
  IfBlock,
  ElseBlock,
  While,
  Return,
  Print,
  Read,
  DeclarationFunctionList,
  FunctionCall,
  ExpressionArithmetic,
  ExpressionRelational,
  ExpressionLogical,
  Variable,
  Type,
  Literal,
  Constant,
  Identifier,
  IdentifierList,
  CommandList,
  TokenDesconhecido
};

// Estrutura de dados para os tokens
struct Funcao {
  str id;
  enum TipoDados tipo;
};

struct Parametro {
  str id;
  enum TipoDados tipo;
};

struct Type {
  enum TipoDados tipo;
};

struct Constante {
  enum TipoDados tipo;
  union {
    int inteiro;
    float flutuante;
    str string;
    void *nada;
  } valor;
};

struct Indentificador {
  str id;
};

struct RelationalExpression {
  enum RelationsOperators operator;
};

struct LogicalExpression {
  enum LogicalOperators operator;
};

struct ArithmeticExpression {
  enum ArithmeticOperators operator;
};

// Criação dos tokens
typedef struct Token Token;

struct Token {
  enum TipoToken tipo;
  union {
    struct Funcao funcao;
    struct Parametro parametro;
    struct Type type;
    void *nada;
    struct Constante constante;
    struct Indentificador idenfier;
    struct RelationalExpression relationalExpression;
    struct LogicalExpression logicalExpression;
    struct ArithmeticExpression arithmeticExpression;
  } u;
};

// Criação do AST
enum TipoNo { Arvore, Vetor, Folha, NoDesconhecido };

typedef struct AST AST;

struct AST {
  enum TipoNo tipo;
  Token token;
  union {
    struct {
      struct AST *left;
      struct AST *right;
    } arvore;
    vector(struct AST *) filhos;
  } u;
};

// Definição das funções de criação de AST
AST *criar_programa(AST *funcoes, AST *blocoPrincipal);

AST *criar_lista_funcoes(AST *funcoes, AST *funcao);

AST *criar_parametro(AST *tipo, AST *id);

AST *criar_declaracao_parametros(AST *declaracaoAnterior, AST *parametro);

AST *criar_funcao(AST *tipo, AST *id, AST *parametros, AST *bloco);

AST *criar_funcao_input_void(AST *tipo, AST *id, AST *bloco);

AST *criar_tipo(enum TipoDados tipo);

AST *criar_constante(const char *input, enum TipoDados tipo);

AST *criar_idenfier(const char *input);

AST *criar_bloco_principal(AST *declaracoes, AST *comandos);

AST *criar_lista_identificadores(AST *identificadores, AST *id);

AST *criar_declaracao(AST *tipo, AST *ids);

AST *criar_declaracoes(AST *declaracoes, AST *declaracao);

AST *criar_lista_comandos(AST *comandos, AST *comando);

AST *criar_comando_if(AST *condicao, AST *blocoIf, AST *blocoElse);

AST *criar_comando_while(AST *condicao, AST *bloco);

AST *criar_comando_atribuicao(AST *id, AST *expressao);

AST *criar_comando_read(AST *id);

AST *criar_comando_print(AST *expressao);

AST *criar_comando_chamada_funcao(AST *id, AST *parametros);

AST *criar_lista_parametros(AST *parametros, AST *parametro);

AST *criar_expressao_logica(enum LogicalOperators operator, AST * left, AST *right);

AST *criar_expressao_aritmetica(enum ArithmeticOperators operator, AST * left, AST *right);

AST *criar_expressao_relacional(enum RelationsOperators operator, AST * left, AST *right);

AST *criar_retorno_funcao(AST *expressao);

void analise_semantica_variaveis(vector(AST *) declaracoes_variaveis, AST *bloco);

void analise_semantica_verificar_variavel(str id, enum TipoDados tipo, AST *arvore);

void printar_tipo_no(AST *no);

void analise_semantica_funcoes(vector(AST *) declaracoes_funcoes);

void analise_semantica_verificar_retorno_funcao(enum TipoDados tipo, AST *bloco);

void converter_constant_para(AST *constante, enum TipoDados tipo);

void imprimir_arvore_estilo_arvore(AST *arvore, int nivel);

char *tipo_dado_str(enum TipoDados tipo);

void printar_folhas(AST *arvore);

void imprimir_token(Token token, int nivel);

void imprimir_traco(int nivel);

char *operador_logico_str(enum LogicalOperators op);

char *operador_aritmetico_str(enum ArithmeticOperators op);

char *operador_relacional_str(enum RelationsOperators op);

char *operador_logico_str_original(enum LogicalOperators op);

char *operador_aritmetico_str_original(enum ArithmeticOperators op);

char *operador_relacional_str_original(enum RelationsOperators op);

char *get_substring_before_delimiter(char *str, const char *delimiters);

void imprimir_codigo(AST *arvore);

char *get_id_from_ID(AST *id);

char *tipo_dado_str_original(enum TipoDados tipo);

void analise_semantica_expressao(AST *arvore);

char *strdup(const char *src);

void percorrer_arvore_aplicando_funcao(AST *a, void (*fn)(AST *));

void analise_semantica_chamada_funcao_existe(vector(AST *) funcoes, AST *nodo);

AST *procurar_funcao(vector(AST *) funcoes, str id);

vector(enum TipoDados) procurar_tipagem_dos_parametros_funcao(AST *funcao);

vector(enum TipoDados) get_lista_parametros(AST *parametros);

enum TipoNo get_tipo_no(AST *no);

enum TipoToken get_tipo_token(AST *no);

str get_funcao_id(AST *funcao);

str get_id_id(AST *id);

str get_chamada_funcao_id(AST *chamada);

void analise_semantica_chamada_funcao_numero_parametros(vector(AST *) funcoes, AST *nodo);

void analise_semantica_funcoes_multiplamente_declaradas(vector(AST *) funcoes);

void analise_semantica_variaveis_multiplamente_declaradas(vector(AST *) declaracoes);

vector(str) get_ids_declaracoes_variaveis(vector(AST *) declaracoes);

void analise_semantica_uso_variavel_nao_declarada(vector(AST *) funcoes, vector(AST *) declaracoes, AST *bloco);
vector(str) get_ids_funcoes(vector(AST *) funcoes);

vector(str) get_ids_parametros_funcao(AST *funcao);

void analise_semantica_chamada_funcao_tipos_entrada(vector(AST *) funcoes, AST *nodo);

void analise_semantica_atribuicao(vector(AST *) funcoes, vector(AST *) parametros, vector(AST *) declaracoes, AST *nodo);

enum TipoDados get_tipo_declaracao(vector(AST *) declaracoes, str id);

enum TipoDados get_tipo_funcao(vector(AST *) funcoes, str id);

#endif