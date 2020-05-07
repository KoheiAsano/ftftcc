#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// tokenize.c
typedef enum
{
    TK_RESERVED,
    TK_NUM,
    TK_IDENT,
    TK_EOF,
} TokenKind;
typedef struct Token Token;
struct Token
{
    TokenKind kind;
    Token *next;
    int val;
    char *str;
    int len;
};

extern char *user_input;
extern Token *token;
Token *new_token(TokenKind kind, Token *cur, char *str, int len);
void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
bool startswith(char *p, char *q);
Token *tokenize();

bool consume(char *op);
Token *consume_ident();
void expect(char *op);
int expect_number();
bool at_eof();

// parse.c
typedef enum
{
    ND_ADD,       // +
    ND_SUB,       // -
    ND_MUL,       // *
    ND_DIV,       // /
    ND_ASSIGN,    // =
    ND_EQ,        // ==
    ND_NEQ,       // !=
    ND_LT,        // <
    ND_LE,        // <=
    ND_NUM,       // Integer
    ND_VAR,       // Identifier
    ND_EXPR_STMT, // Expression Statement
    ND_RETURN,    // "return"
} NodeKind;

typedef struct Node Node;
struct Node
{
    NodeKind kind;
    Node *next; // Next node
    Node *lhs;
    Node *rhs;
    char name; // Used if kind == ND_IDENT
    int val;   // Used if kind == ND_NUM
};

Node *new_node(NodeKind kind);
Node *new_unary(NodeKind kind, Node *lhs);
Node *new_binary(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);
Node *program(void);

// codegen.c

void codegen(Node *node);