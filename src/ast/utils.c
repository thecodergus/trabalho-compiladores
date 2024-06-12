#include "ast.h"

char *get_substring_before_delimiter(char *str, const char *delimiters) {
  char *substring = NULL;
  char *delimiter;
  size_t len;

  /* find the first occurrence of a delimiter in str */
  delimiter = strpbrk(str, delimiters);

  /* if a delimiter was found */
  if (delimiter != NULL) {
    /* calculate the length of the substring */
    len = delimiter - str;

    /* allocate memory for the substring */
    substring = malloc(len + 1);

    /* copy the substring from str to substring */
    strncpy(substring, str, len);

    /* add a null terminator to the end of the substring */
    substring[len] = '\0';
  }

  return substring;
}

char *get_id_from_ID(AST *id) { return str_ptr(id->token.u.idenfier.id); }

char *strdup(const char *src) {
  char *dst = malloc(strlen(src) + 1);  // Space for length plus nul
  if (dst == NULL) return NULL;         // No memory
  strcpy(dst, src);                     // Copy the characters
  return dst;                           // Return the new string
}

void percorrer_arvore_aplicando_funcao(AST *a, void (*fn)(AST *)) {
  if (a && fn) {
    fn(a);
    switch (a->tipo) {
      case Arvore:
        percorrer_arvore_aplicando_funcao(a->u.arvore.left, fn);
        percorrer_arvore_aplicando_funcao(a->u.arvore.right, fn);
        break;
      case Vetor:
        for (AST **it = cvector_begin(a->u.filhos); it != cvector_end(a->u.filhos); it++) {
          percorrer_arvore_aplicando_funcao(*it, fn);
        }
        break;
      case Folha:
      default:
        break;
    }
  }
}

AST *procurar_funcao(vector(AST *) funcoes, str id) {
  if (funcoes) {
    for (AST **it = cvector_begin(funcoes); it != cvector_end(funcoes); it++) {
      AST *fn = *it;
      if (fn && fn->tipo == Vetor && cvector_size(fn->u.filhos) > 0) {
        AST *id_ = fn->u.filhos[1];
        if (str_eq(id_->token.u.idenfier.id, id)) {
          return fn;
        }
      }
    }
  }

  return NULL;
}

vector(enum TipoDados) procurar_tipagem_dos_parametros_funcao(AST *funcao) {
  vector(enum TipoDados) tipos_encontrados = NULL;

  if (funcao && get_tipo_no(funcao) == Vetor && get_tipo_token(funcao) == DeclarationFunction && cvector_size(funcao->u.filhos) > 0) {
    AST *parametros = funcao->u.filhos[2];

    if (parametros && get_tipo_no(parametros) == Vetor && get_tipo_token(parametros) == DeclarationParameterList) {
      for (AST **it = cvector_begin(parametros->u.filhos); it != cvector_end(parametros->u.filhos); it++) {
        AST *parametro = *it;
        if (parametro && get_tipo_no(parametro) == Arvore && get_tipo_token(parametro) == DeclarationParameter) {
          AST *tipo = parametro->u.arvore.left;
          if (tipo && get_tipo_no(tipo) == Folha && get_tipo_token(tipo) == Type) {
            cvector_push_back(tipos_encontrados, tipo->token.u.type.tipo);
          }
        }
      }
    }
  }

  return tipos_encontrados;
}

enum TipoNo get_tipo_no(AST *no) {
  if (no) {
    return no->tipo;
  }

  return NoDesconhecido;
}

enum TipoToken get_tipo_token(AST *no) {
  if (no) {
    return no->token.tipo;
  }
  return TokenDesconhecido;
}

str get_funcao_id(AST *funcao) {
  if (funcao && get_tipo_no(funcao) == Vetor && get_tipo_token(funcao) == DeclarationFunction && cvector_size(funcao->u.filhos) > 0) {
    return funcao->u.filhos[1]->token.u.idenfier.id;
  }

  return str_null;
}

str get_id_id(AST *id) {
  if (id && get_tipo_no(id) == Folha && get_tipo_token(id) == Identifier) {
    return id->token.u.idenfier.id;
  }
  return str_null;
}

str get_chamada_funcao_id(AST *chamada) {
  if (chamada && get_tipo_no(chamada) == Arvore && get_tipo_token(chamada) == FunctionCall) {
    AST *id = chamada->u.arvore.left;
    return get_id_id(id);
  }

  return str_null;
}

vector(enum TipoDados) get_lista_parametros(AST *parametros) {
  vector(enum TipoDados) lista = NULL;
  if (parametros && get_tipo_no(parametros) == Vetor && get_tipo_token(parametros) == ParameterList) {
    for (AST **it = cvector_begin(parametros->u.filhos); it != cvector_end(parametros->u.filhos); it++) {
      cvector_push_back(lista, (*it)->token.u.constante.tipo);
    }
  }
  return lista;
}

vector(str) get_ids_declaracoes_variaveis(vector(AST *) declaracoes) {
  vector(str) declaracoes_ids = NULL;

  for (size_t i = 0; i < cvector_size(declaracoes); i++) {
    for (size_t j = 0; j < cvector_size(declaracoes[i]->u.arvore.right->u.filhos); j++) {
      cvector_push_back(declaracoes_ids, declaracoes[i]->u.arvore.right->u.filhos[j]->token.u.idenfier.id);
    }
  }

  return declaracoes_ids;
}

vector(str) get_ids_funcoes(vector(AST *) funcoes) {
  vector(str) lista_ids = NULL;

  if (funcoes) {
    for (size_t i = 0; i < cvector_size(funcoes); i++) {
      cvector_push_back(lista_ids, funcoes[i]->u.filhos[1]->token.u.idenfier.id);
    }
  }

  return lista_ids;
}

vector(str) get_ids_parametros_funcao(AST *funcao) {
  vector(str) lista = NULL;

  if (funcao && get_tipo_no(funcao) == Vetor && get_tipo_token(funcao) == DeclarationFunction && cvector_size(funcao->u.filhos) == 4) {
    AST *parametros = funcao->u.filhos[2];
    if (parametros && get_tipo_no(parametros) == Vetor && get_tipo_token(parametros) == DeclarationParameterList) {
      for (size_t i = 0; i < cvector_size(parametros->u.filhos); i++) {
        AST *parametro = parametros->u.filhos[i];

        if (parametro && get_tipo_no(parametro) == Arvore && get_tipo_token(parametro) == DeclarationParameter) {
          AST *id = parametro->u.arvore.right;
          cvector_push_back(lista, get_id_id(id));
        }
      }
    }
  }

  return lista;
}

enum TipoDados get_tipo_declaracao(vector(AST *) declaracoes, str id) {
  for (size_t i = 0; i < cvector_size(declaracoes); i++) {
    for (size_t j = 0; j < cvector_size(declaracoes[i]->u.arvore.right->u.filhos); j++) {
      if (str_eq(declaracoes[i]->u.arvore.right->u.filhos[j]->token.u.idenfier.id, id)) {
        return declaracoes[i]->u.arvore.left->token.u.type.tipo;
      }
    }
  }

  return Void;
}

enum TipoDados get_tipo_funcao(vector(AST *) funcoes, str id) {
  for (size_t i = 0; i < cvector_size(funcoes); i++) {
    if (str_eq(funcoes[i]->u.filhos[1]->token.u.idenfier.id, id)) {
      return funcoes[i]->u.filhos[0]->token.u.type.tipo;
    }
  }

  return Void;
}