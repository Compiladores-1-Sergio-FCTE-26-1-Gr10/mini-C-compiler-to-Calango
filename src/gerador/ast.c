/*
 * AST — Árvore Sintática Abstrata
 * Mini C Compiler to Calango
 * Disciplina: FGA0003 – Compiladores 1 (2026/1)
 */

#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── Rótulos para impressão ───────────────────────────────────── */
static const char *rotulo(TipoNo t) {
    switch (t) {
        case NO_PROGRAMA:    return "PROGRAMA";
        case NO_BLOCO:       return "BLOCO";
        case NO_LISTA_DECL:  return "DECLARACOES";
        case NO_LISTA_INSTR: return "INSTRUCOES";
        case NO_DECL:        return "DECL";
        case NO_TIPO_INT:    return "TIPO:int";
        case NO_TIPO_FLOAT:  return "TIPO:float";
        case NO_TIPO_CHAR:   return "TIPO:char";
        case NO_TIPO_BOOL:   return "TIPO:bool";
        case NO_ATRIB:       return "ATRIB(=)";
        case NO_IF:          return "IF";
        case NO_IF_ELSE:     return "IF_ELSE";
        case NO_WHILE:       return "WHILE";
        case NO_FOR:         return "FOR";
        case NO_DO_WHILE:    return "DO_WHILE";
        case NO_PRINTF:      return "PRINTF";
        case NO_SCANF:       return "SCANF";
        case NO_OP_MAIS:     return "OP(+)";
        case NO_OP_MENOS:    return "OP(-)";
        case NO_OP_MULT:     return "OP(*)";
        case NO_OP_DIV:      return "OP(/)";
        case NO_OP_MOD:      return "OP(mod)";
        case NO_OP_EQ:       return "OP(==)";
        case NO_OP_NEQ:      return "OP(!=)";
        case NO_OP_LT:       return "OP(<)";
        case NO_OP_GT:       return "OP(>)";
        case NO_OP_LEQ:      return "OP(<=)";
        case NO_OP_GEQ:      return "OP(>=)";
        case NO_OP_AND:      return "OP(&&)";
        case NO_OP_OR:       return "OP(||)";
        case NO_OP_NOT:      return "OP(!)";
        case NO_OP_NEG:      return "OP(neg)";
        case NO_LIT_TRUE:    return "LIT(true)";
        case NO_LIT_FALSE:   return "LIT(false)";
        case NO_ARGS:        return "ARGS";
        default:             return "?";
    }
}

/* ── Criação de nós ───────────────────────────────────────────── */

No *ast_no(TipoNo tipo, int linha) {
    No *n = calloc(1, sizeof(No));
    if (!n) { fprintf(stderr, "AST: sem memória\n"); exit(1); }
    n->tipo  = tipo;
    n->linha = linha;
    return n;
}

void ast_filho(No *pai, No *filho) {
    if (!pai || !filho) return;
    if (pai->n_filhos >= 4) {
        fprintf(stderr, "AST: nó com mais de 4 filhos\n");
        return;
    }
    pai->filho[pai->n_filhos++] = filho;
}

No *ast_int(int v, int linha) {
    No *n = ast_no(NO_LIT_INT, linha);
    n->val.ival = v;
    return n;
}

No *ast_float(float v, int linha) {
    No *n = ast_no(NO_LIT_FLOAT, linha);
    n->val.fval = v;
    return n;
}

No *ast_char(char v, int linha) {
    No *n = ast_no(NO_LIT_CHAR, linha);
    n->val.cval = v;
    return n;
}

No *ast_str(TipoNo tipo, const char *s, int linha) {
    No *n = ast_no(tipo, linha);
    strncpy(n->val.sval, s, sizeof(n->val.sval) - 1);
    return n;
}

/* ── Impressão da árvore ──────────────────────────────────────── */

static void prefixo(int nivel, int ultimo) {
    for (int i = 0; i < nivel - 1; i++) printf("│   ");
    if (nivel > 0) printf(ultimo ? "└── " : "├── ");
}

void ast_imprime(No *n, int nivel) {
    if (!n) return;

    /* Listas encadeadas via ->prox são impressas como irmãos */
    No *cur = n;
    while (cur) {
        int tem_prox = (cur->prox != NULL);
        int ultimo   = !tem_prox;  /* dentro do nível atual */

        prefixo(nivel, ultimo);

        switch (cur->tipo) {
            case NO_LIT_INT:
                printf("LIT_INT(%d)  [linha %d]\n", cur->val.ival, cur->linha);
                break;
            case NO_LIT_FLOAT:
                printf("LIT_FLOAT(%.6g)  [linha %d]\n", cur->val.fval, cur->linha);
                break;
            case NO_LIT_CHAR:
                printf("LIT_CHAR('%c')  [linha %d]\n", cur->val.cval, cur->linha);
                break;
            case NO_LIT_STRING:
                printf("LIT_STRING(%s)  [linha %d]\n", cur->val.sval, cur->linha);
                break;
            case NO_ID:
                printf("ID(%s)  [linha %d]\n", cur->val.sval, cur->linha);
                break;
            default:
                printf("%s  [linha %d]\n", rotulo(cur->tipo), cur->linha);
                break;
        }

        /* Imprime filhos */
        for (int i = 0; i < cur->n_filhos; i++)
            ast_imprime(cur->filho[i], nivel + 1);

        cur = cur->prox;
    }
}

/* ── Liberação ────────────────────────────────────────────────── */

void ast_libera(No *n) {
    if (!n) return;
    for (int i = 0; i < n->n_filhos; i++) ast_libera(n->filho[i]);
    ast_libera(n->prox);
    free(n);
}
