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