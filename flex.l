%{
#include "y.tab.h"
%}

%%

"int"    { return TIPO_INT; }
"float"  { return TIPO_FLOAT; }
"string" { return TIPO_STRING; }
"void"   { return TIPO_VOID; }
"if"     { return COMANDO_IF; }
"else"   { return COMANDO_ELSE; }
"while"  { return COMANDO_WHILE; }
"return" { return COMANDO_RETURN; }
"print"  { return COMANDO_PRINT; }
"read"   { return COMANDO_READ; }

[0-9]+    { yylval = atoi(yytext); return CONSTANTE_INTEIRA; }
[0-9]+\\.[0-9]*|[0-9]*\\.[0-9]+ { yylval = atof(yytext); return CONSTANTE_FLOAT; }
"[^"]*"   { yylval = strdup(yytext+1); yylval_len = strlen(yylval); return LITERAL; }
[a-zA-Z][a-zA-Z0-9]* { yylval = strdup(yytext); return ID; }

[ \t\n]   { /* ignorar espaços em branco */ }
"+"|"-"|"*"|"/"|"<"|">"|"="|"!"|"&&"|"||" { return *yytext; }

.         { printf("Caractere desconhecido: %s\n", yytext); }

%%
