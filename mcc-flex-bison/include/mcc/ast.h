// Abstract Syntax Tree (AST)
//
// Here we define the AST data structure of the compiler. It mainly consists of
// *tagged unions* for node types and enums for operators.
//
// In addition to the node type specific members, each node features a common
// member `mmc_ast_node` which serves as a *base-class*. It holds data
// independent from the actual node type, like the source location.
//
// Also note that this makes excessive use of C11's *anonymous structs and
// unions* feature.

#ifndef MCC_AST_H
#define MCC_AST_H

// Forward Declarations
struct mcc_ast_expression;
struct mcc_ast_literal;

// ------------------------------------------------------------------- AST Node

struct mcc_ast_source_location {
	int start_line;
	int start_col;
	int end_line;
	int end_col;
};

struct mcc_ast_node {
	struct mcc_ast_source_location sloc;
};
// ------------------------------------------------------------------ Types
enum mcc_ast_type {
	MCC_AST_TYPE_BOOL,
	MCC_AST_TYPE_FLOAT,
	MCC_AST_TYPE_INT,
	MCC_AST_TYPE_STRING,
	MCC_AST_TYPE_VOID,
};

// ------------------------------------------------------------------ Operators

enum mcc_ast_binary_op {
	MCC_AST_BINARY_OP_ADD,
	MCC_AST_BINARY_OP_SUB,
	MCC_AST_BINARY_OP_MUL,
	MCC_AST_BINARY_OP_DIV,
	MCC_AST_BINARY_OP_LESS,
	MCC_AST_BINARY_OP_LESS_EQ,
	MCC_AST_BINARY_OP_GREATER,
	MCC_AST_BINARY_OP_GREATER_EQ,
	MCC_AST_BINARY_OP_EQ,
	MCC_AST_BINARY_OP_NOT_EQ,
	MCC_AST_BINARY_OP_AND,
	MCC_AST_BINARY_OP_OR,

};

enum mcc_ast_unary_op {
	MCC_AST_UNARY_OP_MINUS,
	MCC_AST_UNARY_OP_NOT,
};

// ---------------------------------------------------------------- Expressions

enum mcc_ast_expression_type {
	MCC_AST_EXPRESSION_TYPE_LITERAL,
	MCC_AST_EXPRESSION_TYPE_BINARY_OP,
	MCC_AST_EXPRESSION_TYPE_UNARY_OP,
	MCC_AST_EXPRESSION_TYPE_PARENTH,
	MCC_AST_EXPRESSION_TYPE_IDENTIFIER,
	MCC_AST_EXPRESSION_TYPE_FUNCTION_CALL,
};

struct mcc_ast_expression {
	struct mcc_ast_node node;

	enum mcc_ast_expression_type type;
	union {
		// MCC_AST_EXPRESSION_TYPE_LITERAL
		struct mcc_ast_literal *literal;

		// MCC_AST_EXPRESSION_TYPE_BINARY_OP
		struct {
			enum mcc_ast_binary_op op;
			struct mcc_ast_expression *lhs;
			struct mcc_ast_expression *rhs;
		};

		// MCC_AST_EXPRESSION_TYPE_UNARY_OP
		struct {
			enum mcc_ast_unary_op unary_op;
			struct mcc_ast_expression *unary_expression;
		};

		// MCC_AST_EXPRESSION_TYPE_IDENTIFIER
		struct {
			char *identifier;
			struct mcc_ast_expression *index;
		};

		// MCC_AST_EXPRESSION_TYPE_PARENTH
		struct mcc_ast_expression *expression;

		// MCC_AST_EXPRESSION_TYPE_FUNCTION_CALL
		struct mcc_ast_function_call *function_call;
	};

	struct mcc_ast_expression *next_expression;

	struct mcc_symbol_table *symbol_table;
};

struct mcc_ast_expression *mcc_ast_new_expression_literal(struct mcc_ast_literal *literal);

struct mcc_ast_expression *mcc_ast_new_expression_binary_op(enum mcc_ast_binary_op op,
                                                            struct mcc_ast_expression *lhs,
                                                            struct mcc_ast_expression *rhs);

struct mcc_ast_expression *mcc_ast_new_expression_unary_op(enum mcc_ast_unary_op unary_op,
                                                           struct mcc_ast_expression *unary_expression);

struct mcc_ast_expression *mcc_ast_new_expression_parenth(struct mcc_ast_expression *expression);

struct mcc_ast_expression *mcc_ast_new_expression_identifier(char *identifier, struct mcc_ast_expression *index);

struct mcc_ast_expression *mcc_ast_new_expression_function_call(struct mcc_ast_function_call *function_call);

struct mcc_ast_expression *mcc_ast_new_expression_linked_argument(struct mcc_ast_expression *first_expression,
                                                                  struct mcc_ast_expression *next_expression);

void mcc_ast_delete_expression(struct mcc_ast_expression *expression);

// ------------------------------------------------------------------- Literals

enum mcc_ast_literal_type {
	MCC_AST_LITERAL_TYPE_INT,
	MCC_AST_LITERAL_TYPE_FLOAT,
	MCC_AST_LITERAL_TYPE_STRING,
	MCC_AST_LITERAL_TYPE_BOOL,
};

struct mcc_ast_literal {
	struct mcc_ast_node node;

	enum mcc_ast_literal_type type;
	union {
		// MCC_AST_LITERAL_TYPE_INT
		long i_value;

		// MCC_AST_LITERAL_TYPE_FLOAT
		double f_value;

		// MCC_AST_LITERAL_TYPE_STRING
		char *s_value;

		// MCC_AST_LITERAL_TYPE_BOOL
		char *b_value;
	};
};

struct mcc_ast_literal *mcc_ast_new_literal_int(long value);

struct mcc_ast_literal *mcc_ast_new_literal_float(double value);

struct mcc_ast_literal *mcc_ast_new_literal_string(char *value);

struct mcc_ast_literal *mcc_ast_new_literal_bool(char *value);

void mcc_ast_delete_literal(struct mcc_ast_literal *literal);
// ------------------------------------------------------------------- Declaration, Assignment
struct mcc_ast_declaration {
	struct mcc_ast_node node;
	enum mcc_ast_type type;
	struct mcc_ast_literal *literal;
	char *identifier;
	struct mcc_ast_declaration *next_parameter;
	struct mcc_symbol_table *symbol_table;
};

struct mcc_ast_assignment {
	struct mcc_ast_node node;
	char *identifier;
	struct mcc_ast_expression *value;
	struct mcc_ast_expression *index;
	struct mcc_symbol_table *symbol_table;
};

struct mcc_ast_declaration *
mcc_ast_new_declaration(enum mcc_ast_type type, struct mcc_ast_literal *literal, char *identifier);

struct mcc_ast_declaration *mcc_ast_new_declaration_linked_parameter(struct mcc_ast_declaration *first_parameter,
                                                                     struct mcc_ast_declaration *next_parameter);

void mcc_ast_delete_declaration(struct mcc_ast_declaration *declaration);

struct mcc_ast_assignment *
mcc_ast_new_assignment(char *identifier, struct mcc_ast_expression *index, struct mcc_ast_expression *value);

void mcc_ast_delete_assignment(struct mcc_ast_assignment *assignment);

// ------------------------------------------------------------------- Statements

enum mcc_ast_statement_type {
	MCC_AST_STATEMENT_TYPE_IF,
	MCC_AST_STATEMENT_TYPE_WHILE,
	MCC_AST_STATEMENT_TYPE_DECLARATION,
	MCC_AST_STATEMENT_TYPE_ASSIGNMENT,
	MCC_AST_STATEMENT_TYPE_EXPRESSION,
	MCC_AST_STATEMENT_TYPE_RETURN,
	MCC_AST_STATEMENT_TYPE_COMPOUND
};

struct mcc_ast_statement {
	struct mcc_ast_node node;

	enum mcc_ast_statement_type type;
	union {
		// MCC_AST_STATEMENT_TYPE_IF
		struct {
			struct mcc_ast_expression *if_expression;
			struct mcc_ast_statement *if_statement;
			struct mcc_ast_statement *else_statement;
		};

		// MCC_AST_STATMENT_TYPE_WHILE
		struct {
			struct mcc_ast_expression *while_expression;
			struct mcc_ast_statement *while_statement;
		};

		// MCC_AST_STATEMENT_TYPE_DECLARATION
		struct mcc_ast_declaration *declaration;

		// MCC_AST_STATEMENT_TYPE_ASSIGNMENT
		struct mcc_ast_assignment *assignment;

		// MCC_AST_STATEMENT_TYPE_EXPRESSION
		struct mcc_ast_expression *expression;

		// MCC_AST_STATMENT_TYPE_RETURN
		struct mcc_ast_expression *return_expression;

		// MCC_AST_STATEMENT_TYPE_COMPOUND
		struct mcc_ast_statement *compound_statement;
	};

	struct mcc_ast_statement *next_statement;

	struct mcc_symbol_table *symbol_table;
};

struct mcc_ast_statement *mcc_ast_new_statement_if(struct mcc_ast_expression *if_expression,
                                                   struct mcc_ast_statement *if_statement,
                                                   struct mcc_ast_statement *else_statement);

struct mcc_ast_statement *mcc_ast_new_statement_while(struct mcc_ast_expression *while_expression,
                                                      struct mcc_ast_statement *while_statment);

struct mcc_ast_statement *mcc_ast_new_statement_declaration(struct mcc_ast_declaration *declaration);

struct mcc_ast_statement *mcc_ast_new_statement_assignment(struct mcc_ast_assignment *assignment);

struct mcc_ast_statement *mcc_ast_new_statement_expression(struct mcc_ast_expression *expression);

struct mcc_ast_statement *mcc_ast_new_statement_return(struct mcc_ast_expression *return_expression);

struct mcc_ast_statement *mcc_ast_new_statement_compound(struct mcc_ast_statement *compound_statement);

struct mcc_ast_statement *mcc_ast_new_statement_linked_compound(struct mcc_ast_statement *frist_statement,
                                                                struct mcc_ast_statement *next_statement);

void mcc_ast_delete_statement(struct mcc_ast_statement *statement);

// ------------------------------------------------------------------- Function

struct mcc_ast_function_definition {
	struct mcc_ast_node node;
	enum mcc_ast_type return_type;
	char *identifier;
	struct mcc_ast_declaration *parameters;
	struct mcc_ast_statement *statements;
	struct mcc_ast_function_definition *next_definition;
};

struct mcc_ast_function_call {
	struct mcc_ast_node node;
	char *identifier;
	struct mcc_ast_expression *arguments;
};

struct mcc_ast_function_definition *mcc_ast_new_function_definition(enum mcc_ast_type return_type,
                                                                    char *identifier,
                                                                    struct mcc_ast_declaration *parameters,
                                                                    struct mcc_ast_statement *statements);
void mcc_ast_delete_function_definition(struct mcc_ast_function_definition *function_definition);

struct mcc_ast_function_call *mcc_ast_new_function_call(char *identifier, struct mcc_ast_expression *arguments);
void mcc_ast_delete_function_call(struct mcc_ast_function_call *function_call);

struct mcc_ast_function_definition *
mcc_ast_new_function_def_linked_function_def(struct mcc_ast_function_definition *first_definition,
                                             struct mcc_ast_function_definition *next_definition);

#endif // MCC_AST_H
