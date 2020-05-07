#include "ftftcc.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "引数の個数が正しくありません\n");
        return 1;
    }
    user_input = argv[1];
    token = tokenize();
    Node *node = program();
    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");
    for (Node *n = node; n; n = n->next)
        gen(n);

    printf("    ret\n");
    return 0;
}