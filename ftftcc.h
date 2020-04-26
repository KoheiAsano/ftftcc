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
void error_at(char *loc, char *fmt, ...);
bool startswith(char *p, char *q);
Token *tokenize();

// parse.c
bool consume(char *op);
void expect(char *op);
int expect_number();
bool at_eof();

typedef enum
{
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_EQ,
    ND_NEQ,
    ND_LT,
    ND_LE,
    ND_NUM,
} NodeKind;

typedef struct Node Node;
struct Node
{
    NodeKind kind;
    Node *lhs;
    Node *rhs;
    int val;
};

Node *new_node(NodeKind kind);
Node *new_binary(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);
Node *expr();

// codegen.c

void gen(Node *node);