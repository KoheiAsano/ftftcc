#include "ftftcc.h"

static Node *stmt(void);
static Node *expr(void);
static Node *assign(void);
Node *equality(void);
Node *relational(void);
Node *add(void);
Node *mul(void);
Node *unary(void);
Node *primary(void);

Node *new_node(NodeKind kind)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    return node;
}

Node *new_unary(NodeKind kind, Node *lhs)
{
    Node *node = new_node(kind);
    node->lhs = lhs;
    return node;
}

Node *new_binary(NodeKind kind, Node *lhs, Node *rhs)
{
    Node *node = new_node(kind);
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_node_num(int val)
{
    Node *node = new_node(ND_NUM);
    node->val = val;
    return node;
}

Node *new_node_var(char name)
{
    Node *node = new_node(ND_VAR);
    node->name = name;
    return node;
}

Node *program(void)
{
    Node head = {};
    Node *cur = &head;

    while (!at_eof())
    {
        cur->next = stmt();
        cur = cur->next;
    }
    return head.next;
}

// stmt = expr ";"
//      | "return" expr ";"
static Node *stmt(void)
{
    Node *node;
    if (consume("return"))
        node = new_unary(ND_RETURN, expr());
    else
        node = new_unary(ND_EXPR_STMT, expr());
    expect(";");
    return node;
}
// expr = assign
static Node *expr()
{
    return assign();
}

// assign = equality ( "=" assign)?
static Node *assign()
{
    Node *node = equality();
    if (consume("="))
        node = new_binary(ND_ASSIGN, node, assign());
    return node;
}

// equality = relational (( "==" | "!=" ) relational )*
Node *equality()
{
    Node *node = relational();
    for (;;)
    {
        if (consume("=="))
            node = new_binary(ND_EQ, node, relational());
        else if (consume("!="))
            node = new_binary(ND_NEQ, node, relational());
        else
            return node;
    }
}

// relational = add ( "<" add | ">" add | "<=" add | ">=" add )*
Node *relational()
{
    Node *node = add();

    for (;;)
    {
        if (consume("<"))
            node = new_binary(ND_LT, node, add());
        else if (consume(">"))
            node = new_binary(ND_LT, add(), node);
        else if (consume("<="))
            node = new_binary(ND_LE, node, add());
        else if (consume(">="))
            node = new_binary(ND_LE, add(), node);
        else
            return node;
    }
}

// add = mul ( "+" mul | "-" mul )*
Node *add()
{
    Node *node = mul();

    for (;;)
    {
        if (consume("+"))
            node = new_binary(ND_ADD, node, mul());
        else if (consume("-"))
            node = new_binary(ND_SUB, node, mul());
        else
            return node;
    }
}

// mul = unary ("*" unary | "/" unary)*
Node *mul()
{
    Node *node = unary();

    for (;;)
    {
        if (consume("*"))
            node = new_binary(ND_MUL, node, unary());
        else if (consume("/"))
            node = new_binary(ND_DIV, node, unary());
        else
            return node;
    }
}

// unary = primary | ("+" | "-")? unary
Node *unary()
{
    if (consume("+"))
        return unary();

    if (consume("-"))
        return new_binary(ND_SUB, new_node_num(0), unary());

    return primary();
}

// primary = num | "(" epxr ")"
Node *primary()
{
    if (consume("("))
    {
        Node *node = expr();
        expect(")");
        return node;
    }
    Token *tok = consume_ident();
    if (tok)
        return new_node_var(*tok->str);
    return new_node_num(expect_number());
}
