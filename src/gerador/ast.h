#ifndef AST_H
#define AST_H

/*
 * AST — Árvore Sintática Abstrata
 * Mini C Compiler to Calango
 * Disciplina: FGA0003 – Compiladores 1 (2026/1)
 */

/* ── Tipos de nó ──────────────────────────────────────────────── */
typedef enum {
    /* Programa e bloco */
    NO_PROGRAMA,
    NO_BLOCO,
    NO_LISTA_DECL,
    NO_LISTA_INSTR,

    /* Declarações */
    NO_DECL,          /* tipo + id (+ expr opcional) */

    /* Tipos */
    NO_TIPO_INT,
    NO_TIPO_FLOAT,
    NO_TIPO_CHAR,
    NO_TIPO_BOOL,

    /* Instruções */
    NO_ATRIB,         /* id = expr */
    NO_IF,            /* if (cond) bloco_then */
    NO_IF_ELSE,       /* if (cond) bloco_then else bloco_else */
    NO_WHILE,
    NO_FOR,
    NO_DO_WHILE,
    NO_PRINTF,
    NO_SCANF,

    /* Expressões binárias */
    NO_OP_MAIS,
    NO_OP_MENOS,
    NO_OP_MULT,
    NO_OP_DIV,
    NO_OP_MOD,
    NO_OP_EQ,
    NO_OP_NEQ,
    NO_OP_LT,
    NO_OP_GT,
    NO_OP_LEQ,
    NO_OP_GEQ,
    NO_OP_AND,
    NO_OP_OR,

    /* Expressões unárias */
    NO_OP_NOT,
    NO_OP_NEG,        /* menos unário */

    /* Folhas */
    NO_ID,
    NO_LIT_INT,
    NO_LIT_FLOAT,
    NO_LIT_CHAR,
    NO_LIT_STRING,
    NO_LIT_TRUE,
    NO_LIT_FALSE,

    /* Auxiliares */
    NO_ARGS           /* lista de argumentos printf */
} TipoNo;

/* ── Estrutura do nó ──────────────────────────────────────────── */
typedef struct No {
    TipoNo tipo;

    /* Valor (folhas) */
    union {
        int    ival;
        float  fval;
        char   cval;
        char   sval[256];
    } val;

    /* Filhos (até 4 para cobrir for: init, cond, passo, corpo) */
    struct No *filho[4];
    int        n_filhos;

    /* Para listas encadeadas (lista de instruções/declarações) */
    struct No *prox;

    /* Linha original (depuração) */
    int linha;
} No;

/* ── API ──────────────────────────────────────────────────────── */

/* Cria nó folha sem filhos */
No *ast_no(TipoNo tipo, int linha);

/* Adiciona filho a um nó (até 4) */
void ast_filho(No *pai, No *filho);

/* Cria nó folha inteiro */
No *ast_int(int v, int linha);

/* Cria nó folha float */
No *ast_float(float v, int linha);

/* Cria nó folha char */
No *ast_char(char v, int linha);

/* Cria nó folha string/id */
No *ast_str(TipoNo tipo, const char *s, int linha);

/* Imprime a árvore no stdout (indentada) */
void ast_imprime(No *n, int nivel);

/* Libera toda a árvore */
void ast_libera(No *n);

/* Otimiza a AST in-place: constant folding, simplificações algébricas e remoção de variáveis mortas em listas de declarações/instruções. */
void otimizarAST(No *n);

#endif /* AST_H */