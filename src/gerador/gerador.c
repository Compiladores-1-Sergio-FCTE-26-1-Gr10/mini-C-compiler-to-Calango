/*
 * Gerador de Código — Mini C → Calango
 * Disciplina: FGA0003 – Compiladores 1 (2026/1)
 */

#include "gerador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── Indentação ───────────────────────────────────────────────────── */
#define INDENT_PASSO 3

static void indent(FILE *out, int nivel) {
    for (int i = 0; i < nivel * INDENT_PASSO; i++)
        fputc(' ', out);
}

/* ── Protótipos internos ──────────────────────────────────────────── */
static void gen_no(No *n, FILE *out, int nivel);
static void gen_expr(No *n, FILE *out);
static void gen_lista(No *n, FILE *out, int nivel);
static void gen_printf(No *n, FILE *out, int nivel);
static void gen_tipo(No *n, FILE *out);

/* ── Tipo Mini C → Calango ────────────────────────────────────────── */
static void gen_tipo(No *n, FILE *out) {
    switch (n->tipo) {
        case NO_TIPO_INT:   fprintf(out, "inteiro");   break;
        case NO_TIPO_FLOAT: fprintf(out, "real");      break;
        case NO_TIPO_CHAR:  fprintf(out, "caracter");  break;
        case NO_TIPO_BOOL:  fprintf(out, "logico");    break;
        default: break;
    }
}

/* ── Geração de expressão (inline, sem newline) ───────────────────── */
static void gen_expr(No *n, FILE *out) {
    if (!n) return;

    switch (n->tipo) {
        /* Folhas */
        case NO_ID:         fprintf(out, "%s", n->val.sval); break;
        case NO_LIT_INT:    fprintf(out, "%d", n->val.ival); break;
        case NO_LIT_FLOAT:  fprintf(out, "%g", n->val.fval); break;
        case NO_LIT_CHAR:   fprintf(out, "'%c'", n->val.cval); break;
        case NO_LIT_STRING: {
            /* Remove aspas duplas externas e converte \n → escreval */
            /* A string já vem com aspas — repassa direto */
            fprintf(out, "%s", n->val.sval);
            break;
        }
        case NO_LIT_TRUE:   fprintf(out, "VERDADEIRO"); break;
        case NO_LIT_FALSE:  fprintf(out, "FALSO");      break;

        /* Operadores binários */
        case NO_OP_MAIS:
            fprintf(out, "("); gen_expr(n->filho[0], out);
            fprintf(out, " + "); gen_expr(n->filho[1], out);
            fprintf(out, ")"); break;
        case NO_OP_MENOS:
            fprintf(out, "("); gen_expr(n->filho[0], out);
            fprintf(out, " - "); gen_expr(n->filho[1], out);
            fprintf(out, ")"); break;
        case NO_OP_MULT:
            fprintf(out, "("); gen_expr(n->filho[0], out);
            fprintf(out, " * "); gen_expr(n->filho[1], out);
            fprintf(out, ")"); break;
        case NO_OP_DIV:
            /* Calango: \ para inteiros, / para reais
             * Usamos \ por padrão (divisão inteira é o caso mais comum) */
            fprintf(out, "("); gen_expr(n->filho[0], out);
            fprintf(out, " \\ "); gen_expr(n->filho[1], out);
            fprintf(out, ")"); break;
        case NO_OP_MOD:
            fprintf(out, "("); gen_expr(n->filho[0], out);
            fprintf(out, " mod "); gen_expr(n->filho[1], out);
            fprintf(out, ")"); break;
        case NO_OP_EQ:
            fprintf(out, "("); gen_expr(n->filho[0], out);
            fprintf(out, " = "); gen_expr(n->filho[1], out);
            fprintf(out, ")"); break;
        case NO_OP_NEQ:
            fprintf(out, "("); gen_expr(n->filho[0], out);
            fprintf(out, " <> "); gen_expr(n->filho[1], out);
            fprintf(out, ")"); break;
        case NO_OP_LT:
            fprintf(out, "("); gen_expr(n->filho[0], out);
            fprintf(out, " < "); gen_expr(n->filho[1], out);
            fprintf(out, ")"); break;
        case NO_OP_GT:
            fprintf(out, "("); gen_expr(n->filho[0], out);
            fprintf(out, " > "); gen_expr(n->filho[1], out);
            fprintf(out, ")"); break;
        case NO_OP_LEQ:
            fprintf(out, "("); gen_expr(n->filho[0], out);
            fprintf(out, " <= "); gen_expr(n->filho[1], out);
            fprintf(out, ")"); break;
        case NO_OP_GEQ:
            fprintf(out, "("); gen_expr(n->filho[0], out);
            fprintf(out, " >= "); gen_expr(n->filho[1], out);
            fprintf(out, ")"); break;
        case NO_OP_AND:
            fprintf(out, "("); gen_expr(n->filho[0], out);
            fprintf(out, " e "); gen_expr(n->filho[1], out);
            fprintf(out, ")"); break;
        case NO_OP_OR:
            fprintf(out, "("); gen_expr(n->filho[0], out);
            fprintf(out, " ou "); gen_expr(n->filho[1], out);
            fprintf(out, ")"); break;

        /* Operadores unários */
        case NO_OP_NOT:
            fprintf(out, "nao "); gen_expr(n->filho[0], out); break;
        case NO_OP_NEG:
            fprintf(out, "-"); gen_expr(n->filho[0], out); break;

        default: break;
    }
}

/* ── Auxiliar: verifica se string tem \n no final ─────────────────── */
static int tem_newline(const char *s) {
    /* s vem com aspas: "texto\n" */
    size_t len = strlen(s);
    return len >= 4 && s[len-3] == '\\' && s[len-2] == 'n';
}

/* ── Auxiliar: extrai conteúdo da string sem aspas e sem \n ────────── */
static void string_sem_newline(const char *s, char *dest, size_t max) {
    /* copia sem a primeira e última aspas, e remove \n do final */
    size_t len = strlen(s);
    if (len < 2) { dest[0] = '\0'; return; }

    strncpy(dest, s + 1, max - 1);         /* pula aspas de abertura */
    dest[max - 1] = '\0';

    size_t d = strlen(dest);
    if (d > 0) dest[d - 1] = '\0';         /* remove aspas de fechamento */

    /* remove \n do final se tiver */
    d = strlen(dest);
    if (d >= 2 && dest[d-2] == '\\' && dest[d-1] == 'n')
        dest[d-2] = '\0';
}

/* ── Geração de printf → escreva/escreval ─────────────────────────── */
static void gen_printf(No *n, FILE *out, int nivel) {
    /* filho[0] = LIT_STRING (formato)
     * filho[1] = NO_ARGS (opcional) */

    No *fmt  = n->filho[0];
    No *args = (n->n_filhos > 1) ? n->filho[1] : NULL;

    int usa_escreval = tem_newline(fmt->val.sval);
    const char *cmd  = usa_escreval ? "escreval" : "escreva";

    indent(out, nivel);

    if (!args) {
        /* printf("texto\n") → escreval("texto") */
        char buf[256];
        string_sem_newline(fmt->val.sval, buf, sizeof(buf));
        fprintf(out, "%s(\"%s\")\n", cmd, buf);
        return;
    }

    /* printf("fmt", arg1, arg2, ...) →
     * Calango não tem printf com formato, então geramos um escreva por argumento.
     * Estratégia: escreva(arg1, arg2, ...) — Calango aceita múltiplos separados por vírgula */
    fprintf(out, "%s(", cmd);

    /* Emite argumentos separados por vírgula */
    for (int i = 0; i < args->n_filhos; i++) {
        if (i > 0) fprintf(out, ", ");
        gen_expr(args->filho[i], out);
    }
    fprintf(out, ")\n");
}

/* ── Geração de um nó ─────────────────────────────────────────────── */
static void gen_no(No *n, FILE *out, int nivel) {
    if (!n) return;

    switch (n->tipo) {

        /* ── Programa ──────────────────────────────────────────────── */
        case NO_PROGRAMA:
            fprintf(out, "algoritmo\n");
            fprintf(out, "var\n");
            /* bloco é filho[0] */
            if (n->filho[0]) gen_no(n->filho[0], out, nivel);
            fprintf(out, "fimalgoritmo\n");
            break;

        /* ── Bloco ─────────────────────────────────────────────────── */
        case NO_BLOCO: {
            /* filho[0] = declarações, filho[1] = instruções */
            No *decls  = (n->n_filhos > 0) ? n->filho[0] : NULL;
            No *instrs = (n->n_filhos > 1) ? n->filho[1] : NULL;

            /* Emite declarações (dentro de var) */
            if (decls) gen_lista(decls, out, nivel);

            /* Separa var de inicio */
            fprintf(out, "inicio\n");

            /* Emite instruções */
            if (instrs) gen_lista(instrs, out, nivel + 1);
            break;
        }

        /* ── Declaração ────────────────────────────────────────────── */
        case NO_DECL: {
            /* filho[0] = tipo
             * filho[1] = ID (sem init) ou ATRIB (com init)
             * Calango: nome : tipo */
            indent(out, nivel + 1);

            No *tipo_no = n->filho[0];
            No *segundo = n->filho[1];

            if (segundo->tipo == NO_ID) {
                /* sem inicialização */
                fprintf(out, "%s : ", segundo->val.sval);
                gen_tipo(tipo_no, out);
                fprintf(out, "\n");
            } else {
                /* com inicialização — NO_ATRIB */
                No *id   = segundo->filho[0];
                No *expr = segundo->filho[1];
                fprintf(out, "%s : ", id->val.sval);
                gen_tipo(tipo_no, out);
                fprintf(out, "\n");
                /* A atribuição vai para o inicio */
                /* Guardamos para emitir depois — por ora emite em inicio */
                (void)expr; /* será emitida quando a instrução aparecer */
            }
            break;
        }

        /* ── Atribuição ────────────────────────────────────────────── */
        case NO_ATRIB:
            indent(out, nivel);
            fprintf(out, "%s <- ", n->filho[0]->val.sval);
            gen_expr(n->filho[1], out);
            fprintf(out, "\n");
            break;

        /* ── if sem else ───────────────────────────────────────────── */
        case NO_IF:
            indent(out, nivel);
            fprintf(out, "se ");
            gen_expr(n->filho[0], out);
            fprintf(out, " entao\n");
            if (n->filho[1]) gen_lista(n->filho[1], out, nivel + 1);
            indent(out, nivel);
            fprintf(out, "fimse\n");
            break;

        /* ── if com else ───────────────────────────────────────────── */
        case NO_IF_ELSE:
            indent(out, nivel);
            fprintf(out, "se ");
            gen_expr(n->filho[0], out);
            fprintf(out, " entao\n");
            if (n->filho[1]) gen_lista(n->filho[1], out, nivel + 1);
            indent(out, nivel);
            fprintf(out, "senao\n");
            if (n->filho[2]) gen_lista(n->filho[2], out, nivel + 1);
            indent(out, nivel);
            fprintf(out, "fimse\n");
            break;

        /* ── while ─────────────────────────────────────────────────── */
        case NO_WHILE:
            indent(out, nivel);
            fprintf(out, "enquanto ");
            gen_expr(n->filho[0], out);
            fprintf(out, " faca\n");
            if (n->filho[1]) gen_lista(n->filho[1], out, nivel + 1);
            indent(out, nivel);
            fprintf(out, "fimenquanto\n");
            break;

        /* ── for ───────────────────────────────────────────────────── */
        case NO_FOR: {
            /* filho[0]=init  filho[1]=cond  filho[2]=passo  filho[3]=corpo
             * Calango: para var de inicio ate fim faca ... fimpara
             * Como Mini C tem forma livre, usamos enquanto equivalente */
            No *init  = n->filho[0];
            No *cond  = n->filho[1];
            No *passo = n->filho[2];
            No *corpo = n->filho[3];

            /* emite init antes do loop */
            gen_no(init, out, nivel);

            indent(out, nivel);
            fprintf(out, "enquanto ");
            gen_expr(cond, out);
            fprintf(out, " faca\n");

            if (corpo) gen_lista(corpo, out, nivel + 1);

            /* emite passo no final do corpo */
            gen_no(passo, out, nivel + 1);

            indent(out, nivel);
            fprintf(out, "fimenquanto\n");
            break;
        }

        /* ── do-while ──────────────────────────────────────────────── */
        case NO_DO_WHILE:
            indent(out, nivel);
            fprintf(out, "faca\n");
            if (n->filho[0]) gen_lista(n->filho[0], out, nivel + 1);
            indent(out, nivel);
            fprintf(out, "enquanto ");
            gen_expr(n->filho[1], out);
            fprintf(out, "\n");
            break;

        /* ── printf ────────────────────────────────────────────────── */
        case NO_PRINTF:
            gen_printf(n, out, nivel);
            break;

        /* ── scanf ─────────────────────────────────────────────────── */
        case NO_SCANF: {
            /* filho[0]=formato  filho[1]=ID
             * Calango: leia(var) ou leiaCaracter(var) */
            No *id = n->filho[1];
            indent(out, nivel);
            /* Se o tipo for char usa leiaCaracter, senão leia */
            /* Por simplicidade usamos leia (o tipo está na tabela,
             * mas o gerador não tem acesso direto — safe default) */
            fprintf(out, "leia(%s)\n", id->val.sval);
            break;
        }

        default: break;
    }
}

/* ── Gera lista encadeada via ->prox ──────────────────────────────── */
static void gen_lista(No *n, FILE *out, int nivel) {
    No *cur = n;
    while (cur) {
        gen_no(cur, out, nivel);
        cur = cur->prox;
    }
}

/* ── Ponto de entrada público ─────────────────────────────────────── */
void gerar(No *raiz, FILE *saida) {
    if (!raiz) return;
    gen_no(raiz, saida, 0);
}
