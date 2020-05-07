#include "ftftcc.h"

char *user_input;

// Report an error and exit.
void error(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

// Report an error location and exit.
void error_at(char *loc, char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    int pos = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, "");
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}
// Attach new token to cur, Return new token
Token *new_token(TokenKind kind, Token *cur, char *str, int len)
{
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->len = len;
    cur->next = tok;
    return tok;
}

// Check whether p start by p
bool startswith(char *p, char *q)
{
    return memcmp(p, q, strlen(q)) == 0;
}
// Check whether c is alphabet or not
bool is_alpha(char c)
{
    return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
}

// Check whether c is alphabet or number or not
bool is_alnum(char c)
{
    return ('0' <= c && c <= '9') || is_alpha(c);
}

// Tokenize p
Token *tokenize()
{
    char *p = user_input;
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p)
    {
        if (isspace(*p))
        {
            p++;
            continue;
        }
        // keywords
        if (startswith(p, "return") && !is_alnum(p[6]))
        {
            cur = new_token(TK_RESERVED, cur, p, 6);
            p += 6;
            continue;
        }
        // multi-character symbols
        if (startswith(p, "==") || startswith(p, "!=") || startswith(p, "<=") || startswith(p, ">="))
        {
            cur = new_token(TK_RESERVED, cur, p, 2);
            p += 2;
            continue;
        }

        // single-character symbols
        if (strchr("+-*/()<>=;", *p))
        {
            cur = new_token(TK_RESERVED, cur, p++, 1);
            continue;
        }

        // Identifier
        if ('a' <= *p && *p <= 'z')
        {
            cur = new_token(TK_IDENT, cur, p++, 1);
            continue;
        }

        if (isdigit(*p))
        {
            cur = new_token(TK_NUM, cur, p, 1);
            char *q = p;
            cur->val = strtol(p, &p, 10);
            cur->len = p - q;
            continue;
        }
        error_at(p, "invalid token");
    }
    new_token(TK_EOF, cur, p, 0);
    return head.next;
}

Token *token;

// Consume the current token if it matches `op`
bool consume(char *op)
{
    if (token->kind != TK_RESERVED || strlen(op) != token->len || memcmp(token->str, op, token->len))
        return false;
    token = token->next;
    return true;
}
// Consume the current token if it is Identifier, return token
Token *consume_ident()
{
    if (token->kind != TK_IDENT)
        return NULL;
    Token *tok = token;
    token = token->next;
    return tok;
}

// Ensure that current token is given `op`
void expect(char *op)
{
    if (token->kind != TK_RESERVED || strlen(op) != token->len || memcmp(token->str, op, token->len))
        error_at(token->str, " is not '%s'", op);
    token = token->next;
}
// Ensure that current token is TK_NUM
int expect_number()
{
    if (token->kind != TK_NUM)
        error_at(token->str, "expected number but ");
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof()
{
    return token->kind == TK_EOF;
}