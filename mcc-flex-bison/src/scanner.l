%option prefix="mcc_parser_"

%option batch
%option bison-bridge
%option bison-locations
%option noinput
%option nounput
%option noyywrap
%option reentrant
%option yylineno

%{
#include "parser.tab.h"

#define YYSTYPE MCC_PARSER_STYPE
#define YYLTYPE MCC_PARSER_LTYPE

#define YY_USER_ACTION \
	yylloc->first_column = yylloc->last_column; \
	yylloc->last_column = calculate_column(yylloc->last_column,yytext);\
	yylloc->first_line = num_lines;\
	yylloc->last_line = yylineno;

	int num_lines=1;
	int comment_caller;
%}

%x comment foo

alpha [a-zA-Z_]
alpha_num [a-zA-Z0-9_]
digit [0-9]
identifier {alpha}{alpha_num}*

bool_literal true|false
int_literal   [0-9]+
float_literal [0-9]+\.[0-9]+
string_literal \"[^"]*\"

line_break \n
comment [/][*][^*]*[*]+([^*/][^*]*[*]+)*[/]

%%

"="               { return TK_EQUAL_SIGN; }
"!"               { return TK_NOT; }

"+"               { return TK_PLUS; }
"-"               { return TK_MINUS; }
"*"               { return TK_ASTER; }
"/"               { return TK_SLASH; }

"<"               { return TK_LESS; }
"<="              { return TK_LESS_EQ; }
">"               { return TK_GREATER; }
">="              { return TK_GREATER_EQ; }
"=="              { return TK_EQ; }
"!="              { return TK_NOT_EQ; }

"&&"              { return TK_AND; }
"||"              { return TK_OR; }

"void"			  { return TK_VOID; }
"bool"            { return TK_BOOL; }
"int"             { return TK_INT; }
"float"           { return TK_FLOAT; }
"string"          { return TK_STRING; }

"("               { return TK_LPARENTH; }
")"               { return TK_RPARENTH; }

"{"		          { return TK_LBRACE; }
"}" 		      { return TK_RBRACE; }

"["		          { return TK_LBRACKET; }
"]"		          { return TK_RBRACKET; }

","		          { return TK_COMMA; }
";"		          { return TK_SEMICOLON; }

"if"		      { return TK_IF; }
"else"		      { return TK_ELSE; }
"return"	      { return TK_RETURN; }
"while"		      { return TK_WHILE; }

{line_break}	  { num_lines++; }

{bool_literal}    { yylval->TK_BOOL_LITERAL = strdup(yytext); return TK_BOOL_LITERAL; }
{int_literal}     { yylval->TK_INT_LITERAL = atol(yytext); return TK_INT_LITERAL; }
{float_literal}   { yylval->TK_FLOAT_LITERAL = atof(yytext); return TK_FLOAT_LITERAL; }
{string_literal}  { yylval->TK_STRING_LITERAL = strdup(yytext); return TK_STRING_LITERAL; }

{identifier}	  { yylval->TK_IDENTIFIER = strdup(yytext); return TK_IDENTIFIER; }


"/*"         			{ comment_caller = INITIAL; BEGIN(comment); }
<foo>"/*" 				{ comment_caller = foo; BEGIN(comment); }
<comment>[^*\n]*
<comment>"*"+[^*/\n]*
<comment>\n             ++num_lines;
<comment>"*"+"/"        BEGIN(comment_caller);

[ \t\r]+        { /* ignore */ }

<<EOF>>           { return TK_END; }

.                 { fprintf(stderr, "invalid character '%c'\n", yytext[0]); }
