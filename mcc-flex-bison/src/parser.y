%define api.prefix {mcc_parser_}

%define api.pure full
%lex-param   {void *scanner}
%parse-param {void *scanner} {struct mcc_parser_result* result}

%define parse.trace
%define parse.error verbose

%code requires {
#include "mcc/parser.h"
}

%{
#include <string.h>

int mcc_parser_lex();
void mcc_parser_error();

#define loc(ast_node, ast_sloc) \
	(ast_node)->node.sloc.start_col = (ast_sloc).first_column; \
    (ast_node)->node.sloc.end_col = (ast_sloc).last_column; \
    (ast_node)->node.sloc.start_line = (ast_sloc).first_line; \
    (ast_node)->node.sloc.end_line = (ast_sloc).last_line; 
%}

%define api.value.type union
%define api.token.prefix {TK_}

%locations

%token END 0 "EOF"

%token <char*>  BOOL_LITERAL "bool literal"
%token <long>   INT_LITERAL   "integer literal"
%token <double> FLOAT_LITERAL "float literal"
%token <char*>  STRING_LITERAL "string literal"


%token <char*> IDENTIFIER "identifier"

%token EQUAL_SIGN "="
%token NOT "!"

%token PLUS  "+"
%token MINUS "-"
%token ASTER "*"
%token SLASH "/"

%token LESS "<"
%token LESS_EQ "<="
%token GREATER ">"
%token GREATER_EQ ">="
%token EQ "=="
%token NOT_EQ "!="

%token AND "&&"
%token OR "||"

%token BOOL "bool"
%token INT "int"
%token FLOAT "float"
%token STRING "string"

%token LPARENTH "("
%token RPARENTH ")"

%token LBRACE "{"
%token RBRACE "}"

%token LBRACKET "["
%token RBRACKET "]"

%token COMMA ","
%token SEMICOLON ";"

%token IF "if"
%token ELSE "else"
%token RETURN "return"
%token WHILE "while"

%token VOID "void"


%left AND OR
%left EQ NOT_EQ
%left LESS LESS_EQ GREATER GREATER_EQ
%left PLUS MINUS
%left ASTER SLASH

%precedence RPARENTH
%precedence ELSE

%precedence UMINUS
%precedence NOT

%type <struct mcc_ast_function_definition *> function_def program
%type <struct mcc_ast_expression *> expression binary_op unary_op arguments
%type <struct mcc_ast_literal *> literal arr_identifier
%type <struct mcc_ast_statement *> statement if_stmt while_stmt ret_stmt compound_stmt
%type <struct mcc_ast_declaration *> declaration parameters
%type <struct mcc_ast_assignment *> assignment
%type <enum mcc_ast_type> type function_type
%type <struct mcc_ast_function_call *> call_expr

%destructor { mcc_ast_delete_function_definition($$); } function_def program
%destructor { mcc_ast_delete_expression($$); } expression binary_op unary_op arguments
%destructor { mcc_ast_delete_literal($$); } literal arr_identifier 
%destructor { mcc_ast_delete_statement($$); } statement if_stmt while_stmt ret_stmt compound_stmt
%destructor { mcc_ast_delete_declaration($$); } declaration parameters
%destructor { mcc_ast_delete_assignment($$); } assignment
%destructor { mcc_ast_delete_function_call($$); } call_expr
%destructor { free($$); } IDENTIFIER STRING_LITERAL

%start toplevel

%%

toplevel : program { result->program = $1; }
         ;

program : function_def { $$ = $1; }
	    | program function_def { $$ = $1; mcc_ast_new_function_def_linked_function_def($1, $2); }
	    ;

function_def : function_type IDENTIFIER LPARENTH RPARENTH LBRACE compound_stmt RBRACE {$$ = mcc_ast_new_function_definition($1, $2, NULL, $6); loc($$, @1);}
			 | function_type IDENTIFIER LPARENTH parameters RPARENTH LBRACE compound_stmt RBRACE {$$ = mcc_ast_new_function_definition($1, $2, $4, $7);   loc($$, @1);}
	;

function_type : BOOL { $$ = MCC_AST_TYPE_BOOL; }
			  | INT { $$ = MCC_AST_TYPE_INT; }
			  | FLOAT { $$ = MCC_AST_TYPE_FLOAT; }
			  | STRING { $$ = MCC_AST_TYPE_STRING; }
			  | VOID { $$ = MCC_AST_TYPE_VOID; }

parameters : declaration { $$ = $1; }
		   | declaration COMMA parameters {$$ = $1; mcc_ast_new_declaration_linked_parameter($1, $3);}
		   ;

arguments : expression {$$ = $1;}
		  | expression COMMA arguments {$$ = $1; mcc_ast_new_expression_linked_argument($1, $3);}
		  ; 

expression : literal                                  { $$ = mcc_ast_new_expression_literal($1); loc($$, @1); }
		   | binary_op                                { $$ =$1; };
		   | unary_op                                 { $$ =$1; };
           | LPARENTH expression RPARENTH             { $$ = mcc_ast_new_expression_parenth($2); loc($$, @1); }
		   | IDENTIFIER								  { $$ = mcc_ast_new_expression_identifier($1, NULL); loc($$, @1); }
		   | IDENTIFIER LBRACKET expression RBRACKET  { $$ = mcc_ast_new_expression_identifier($1, $3); loc($$, @1); }
		   | call_expr								  { $$ = mcc_ast_new_expression_function_call($1); loc($$, @1); }
           ;

call_expr : IDENTIFIER LPARENTH RPARENTH { $$ = mcc_ast_new_function_call($1, NULL); loc($$, @1); }
		  | IDENTIFIER LPARENTH arguments RPARENTH { $$ = mcc_ast_new_function_call($1,$3); loc($$, @1); }
		  ;


statement : if_stmt     { $$ = $1; }
          | while_stmt  { $$ = $1; }
		  | ret_stmt    { $$ = $1; }
		  | declaration SEMICOLON { $$ = mcc_ast_new_statement_declaration($1); loc($$,@1); }
		  | assignment SEMICOLON { $$ = mcc_ast_new_statement_assignment($1); loc($$,@1); }
		  | expression SEMICOLON { $$ = mcc_ast_new_statement_expression($1); loc($$,@1); }
		  | LBRACE compound_stmt RBRACE { $$ = $2; }
		  | LBRACE RBRACE { $$ = mcc_ast_new_statement_compound(NULL); loc($$,@1); }
		  | SEMICOLON { $$ = mcc_ast_new_statement_compound(NULL); loc($$,@1); }
          ;

declaration : type IDENTIFIER { $$ = mcc_ast_new_declaration($1, NULL, $2); loc($$,@1); }
			| type LBRACKET arr_identifier RBRACKET IDENTIFIER { $$ = mcc_ast_new_declaration($1,$3,$5); loc($$, @1);}
			;

arr_identifier : INT_LITERAL { $$ = mcc_ast_new_literal_int($1); loc($$, @1);}

assignment : IDENTIFIER EQUAL_SIGN expression { $$ = mcc_ast_new_assignment($1,NULL,$3); loc($$,@1); }
		   | IDENTIFIER LBRACKET expression RBRACKET EQUAL_SIGN expression { $$ = mcc_ast_new_assignment($1,$3,$6); loc($$, @1);}
           ;

if_stmt : IF LPARENTH expression RPARENTH statement { $$ = mcc_ast_new_statement_if($3, $5, NULL); loc($$, @1); }
        | IF LPARENTH expression RPARENTH statement ELSE statement { $$ = mcc_ast_new_statement_if($3, $5, $7); loc($$, @1); }
        ;

while_stmt : WHILE LPARENTH expression RPARENTH statement { $$ = mcc_ast_new_statement_while($3, $5); loc($$, @1); }
           ;

ret_stmt : RETURN SEMICOLON { $$ = mcc_ast_new_statement_return(NULL); loc($$, @1);}
         | RETURN expression SEMICOLON  { $$ = mcc_ast_new_statement_return($2); loc($$, @1);}
		 ;

compound_stmt : statement { $$ = mcc_ast_new_statement_compound($1); loc($$, @1); }
			  | compound_stmt statement  {$$ = $1; mcc_ast_new_statement_linked_compound($1, $2); }
			  ;

binary_op : expression PLUS  expression  { $$ = mcc_ast_new_expression_binary_op(MCC_AST_BINARY_OP_ADD, $1, $3); loc($$, @1); }
		  | expression MINUS expression  { $$ = mcc_ast_new_expression_binary_op(MCC_AST_BINARY_OP_SUB, $1, $3); loc($$, @1); }
		  | expression ASTER expression  { $$ = mcc_ast_new_expression_binary_op(MCC_AST_BINARY_OP_MUL, $1, $3); loc($$, @1); }
		  | expression SLASH expression  { $$ = mcc_ast_new_expression_binary_op(MCC_AST_BINARY_OP_DIV, $1, $3); loc($$, @1); }
		  | expression LESS expression  { $$ = mcc_ast_new_expression_binary_op(MCC_AST_BINARY_OP_LESS, $1, $3); loc($$, @1); }
		  | expression LESS_EQ expression  { $$ = mcc_ast_new_expression_binary_op(MCC_AST_BINARY_OP_LESS_EQ, $1, $3); loc($$, @1); }
		  | expression GREATER expression  { $$ = mcc_ast_new_expression_binary_op(MCC_AST_BINARY_OP_GREATER, $1, $3); loc($$, @1); }
		  | expression GREATER_EQ expression  { $$ = mcc_ast_new_expression_binary_op(MCC_AST_BINARY_OP_GREATER_EQ, $1, $3); loc($$, @1); }
		  | expression EQ expression  { $$ = mcc_ast_new_expression_binary_op(MCC_AST_BINARY_OP_EQ, $1, $3); loc($$, @1); }
		  | expression NOT_EQ expression  { $$ = mcc_ast_new_expression_binary_op(MCC_AST_BINARY_OP_NOT_EQ, $1, $3); loc($$, @1); }
		  | expression AND expression  { $$ = mcc_ast_new_expression_binary_op(MCC_AST_BINARY_OP_AND, $1, $3); loc($$, @1); }
		  | expression OR expression  { $$ = mcc_ast_new_expression_binary_op(MCC_AST_BINARY_OP_OR, $1, $3); loc($$, @1); }
		  ;

unary_op : MINUS expression %prec UMINUS { $$ = mcc_ast_new_expression_unary_op(MCC_AST_UNARY_OP_MINUS, $2); loc($$, @1);}
		 | NOT expression %prec NOT { $$ = mcc_ast_new_expression_unary_op(MCC_AST_UNARY_OP_NOT, $2); loc($$, @1);}
		 ;

literal : INT_LITERAL   { $$ = mcc_ast_new_literal_int($1);   loc($$, @1); }
        | FLOAT_LITERAL { $$ = mcc_ast_new_literal_float($1); loc($$, @1); }
		| BOOL_LITERAL { $$ = mcc_ast_new_literal_bool($1); loc($$,@1); }
		| STRING_LITERAL { $$ = mcc_ast_new_literal_string($1); loc($$,@1); }
        ;

type : BOOL { $$ = MCC_AST_TYPE_BOOL; }
	|FLOAT { $$ = MCC_AST_TYPE_FLOAT; }
	|INT { $$ = MCC_AST_TYPE_INT; }
	|STRING { $$ = MCC_AST_TYPE_STRING; }
	;

%%

#include <assert.h>

#include "scanner.h"
#include "utils/unused.h"

void mcc_parser_error(struct MCC_PARSER_LTYPE *yylloc, yyscan_t *scanner, struct mcc_parser_result *result, const char *msg)
{
	
	char *error_message = (char*)malloc(256 * sizeof(char));
	
	if(!error_message){
		printf("Failed to malloc error message\n");
		return;
	}

	sprintf(error_message,"%d:%d: %s\n", yylloc->last_line,yylloc->first_column,msg);
	
	result->error_message = error_message;

	if(result->error_message != NULL){
		result->status = MCC_PARSER_STATUS_UNKNOWN_ERROR;
	}

	UNUSED(yylloc);
	UNUSED(scanner);
	UNUSED(msg);

}

struct mcc_parser_result mcc_parse_string(const char *input)
{
	assert(input);

	FILE *in = fmemopen((void *)input, strlen(input), "r");
	if (!in) {
		return (struct mcc_parser_result){
		    .status = MCC_PARSER_STATUS_UNABLE_TO_OPEN_STREAM,
		};
	}

	struct mcc_parser_result result = mcc_parse_file(in);

	fclose(in);

	return result;
}

struct mcc_parser_result mcc_parse_file(FILE *input)
{
	assert(input);

	yyscan_t scanner;
	mcc_parser_lex_init(&scanner);
	mcc_parser_set_in(input, scanner);

	struct mcc_parser_result result = {
	    .status = MCC_PARSER_STATUS_OK,
	};

	if (yyparse(scanner, &result) != 0) {
		result.status = MCC_PARSER_STATUS_UNKNOWN_ERROR;
	}

	mcc_parser_lex_destroy(scanner);

	return result;
}


int calculate_column(int last_column, char *text){
	for (unsigned int i=0; i<strlen(text);i++){
		last_column++;
		if (text[i]=='\n') {
			last_column=0;
		}
	}
	return last_column;
}

