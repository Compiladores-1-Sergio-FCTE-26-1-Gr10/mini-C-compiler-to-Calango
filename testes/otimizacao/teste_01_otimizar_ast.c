#include "ast.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static No *bin(TipoNo tipo, No *esq, No *dir) {
    No *n = ast_no(tipo, 1);
    ast_filho(n, esq);
    ast_filho(n, dir);
    return n;
}

static void testa_constant_folding_aritmetico(void) {
    No *expr = bin(
        NO_OP_MULT,
        bin(NO_OP_MAIS, ast_int(2, 1), ast_int(3, 1)),
        ast_int(4, 1)
    );

    otimizarAST(expr);

    assert(expr->tipo == NO_LIT_INT);
    assert(expr->val.ival == 20);
    assert(expr->n_filhos == 0);

    ast_libera(expr);
}

static void testa_constant_folding_relacional(void) {
    No *expr = bin(NO_OP_LEQ, ast_int(10, 1), ast_int(20, 1));

    otimizarAST(expr);

    assert(expr->tipo == NO_LIT_TRUE);
    assert(expr->n_filhos == 0);

    ast_libera(expr);
}

static void testa_simplificacao_algebrica(void) {
    No *expr = bin(NO_OP_MAIS, ast_str(NO_ID, "total", 1), ast_int(0, 1));

    otimizarAST(expr);

    assert(expr->tipo == NO_ID);
    assert(strcmp(expr->val.sval, "total") == 0);
    assert(expr->n_filhos == 0);

    ast_libera(expr);
}

static void testa_simplificacao_logica(void) {
    No *expr = bin(NO_OP_AND, ast_no(NO_LIT_TRUE, 1), ast_str(NO_ID, "ativo", 1));

    otimizarAST(expr);

    assert(expr->tipo == NO_ID);
    assert(strcmp(expr->val.sval, "ativo") == 0);
    assert(expr->n_filhos == 0);

    ast_libera(expr);
}

int main(void) {
    testa_constant_folding_aritmetico();
    testa_constant_folding_relacional();
    testa_simplificacao_algebrica();
    testa_simplificacao_logica();

    printf("Teste otimizarAST: OK\n");
    return 0;
}