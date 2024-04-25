/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_BISON_TAB_H_INCLUDED
# define YY_YY_BISON_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    COMANDO_PRINT = 258,           /* COMANDO_PRINT  */
    COMANDO_READ = 259,            /* COMANDO_READ  */
    COMANDO_IF = 260,              /* COMANDO_IF  */
    COMANDO_ELSE = 261,            /* COMANDO_ELSE  */
    COMANDO_WHILE = 262,           /* COMANDO_WHILE  */
    COMANDO_RETURN = 263,          /* COMANDO_RETURN  */
    ID = 264,                      /* ID  */
    LITERAL = 265,                 /* LITERAL  */
    TIPO_VOID = 266,               /* TIPO_VOID  */
    TIPO_INT = 267,                /* TIPO_INT  */
    TIPO_FLOAT = 268,              /* TIPO_FLOAT  */
    TIPO_STRING = 269,             /* TIPO_STRING  */
    OPERADOR_MULTIPLICACAO = 270,  /* OPERADOR_MULTIPLICACAO  */
    OPERADOR_DIVISAO = 271,        /* OPERADOR_DIVISAO  */
    OPERADOR_SOMA = 272,           /* OPERADOR_SOMA  */
    OPERADOR_SUBTRACAO = 273,      /* OPERADOR_SUBTRACAO  */
    OPERADOR_POTENCIA = 274,       /* OPERADOR_POTENCIA  */
    SIMBOLO_ATRIBUICAO = 275,      /* SIMBOLO_ATRIBUICAO  */
    SIMBOLO_ABRE_PARENTESES = 276, /* SIMBOLO_ABRE_PARENTESES  */
    SIMBOLO_FECHA_PARENTESES = 277, /* SIMBOLO_FECHA_PARENTESES  */
    SIMBOLO_ABRE_CHAVES = 278,     /* SIMBOLO_ABRE_CHAVES  */
    SIMBOLO_FECHA_CHAVES = 279,    /* SIMBOLO_FECHA_CHAVES  */
    SIMBOLO_VIRGULA = 280,         /* SIMBOLO_VIRGULA  */
    SIMBOLO_PONTO_VIRGULA = 281,   /* SIMBOLO_PONTO_VIRGULA  */
    FIM = 282,                     /* FIM  */
    LOGICA_OR = 283,               /* LOGICA_OR  */
    LOGICA_AND = 284,              /* LOGICA_AND  */
    LOGICA_EQ = 285,               /* LOGICA_EQ  */
    LOGICA_NE = 286,               /* LOGICA_NE  */
    LOGICA_LT = 287,               /* LOGICA_LT  */
    LOGICA_LE = 288,               /* LOGICA_LE  */
    LOGICA_GT = 289,               /* LOGICA_GT  */
    LOGICA_GE = 290,               /* LOGICA_GE  */
    LOGICA_NOT = 291               /* LOGICA_NOT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_BISON_TAB_H_INCLUDED  */
