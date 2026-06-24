/*
 * Gerador de Código — Mini C → Calango
 * Disciplina: FGA0003 – Compiladores 1 (2026/1)
 *
 * Mapeamento baseado no repositório oficial do Calango:
 *   https://github.com/lucasmoten/calango (CalangoTokenMaker.flex + IndentadorCalango.java)
 *
 * Mapeamento confirmado:
 *
 * ESTRUTURA:
 *   programa     → algoritmo NomeAlgo;\n principal\n ... fimPrincipal
 *   declaração   → tipo nome;         (sem init — Calango não suporta init na decl)
 *   atribuição   → nome = expr;       (usa = com ; no final)
 *
 * CONTROLE:
 *   if           → se(cond)entao\n ... fimSe
 *   if/else      → se(cond)entao\n ... senao\n ... fimSe
 *   while        → enquanto(cond)faca\n ... fimEnquanto
 *   for          → enquanto equivalente (para do Calango é restrito a contador inteiro)
 *   do-while     → faca\n ... enquanto(cond);
 *
 * I/O:
 *   printf(\n)   → escreval(args)
 *   printf()     → escreva(args)
 *   scanf        → leia(var)  /  leiaCaracter(var) para tipo caracter
 *
 * OPERADORES:
 *   &&  → e        ||  → ou       !  → nao
 *   %   → mod      ==  → ==       != → !=
 *   true → verdadeiro   false → falso
 *   /   → /        (Calango aceita / e \ para divisão)
 */

#include "gerador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INDENT_PASSO 3

static void indent(FILE *out, int nivel) {
    for (int i = 0; i < nivel * INDENT_PASSO; i++)
        fputc(' ', out);
}

/* ── Protótipos ───────────────────────────────────────────────────── */
static void gen_no(No *n, FILE *out, int nivel);
static void gen_expr(No *n, FILE *out);
static void gen_lista(No *n, FILE *out, int nivel);
static void gen_printf(No *n, FILE *out, int nivel);

/* ── Tipo → Calango ───────────────────────────────────────────────── */
static void gen_tipo(No *n, FILE *out) {
    switch (n->tipo) {
        case NO_TIPO_INT:   fprintf(out, "inteiro");  break;
        case NO_TIPO_FLOAT: fprintf(out, "real");     break;
        case NO_TIPO_CHAR:  fprintf(out, "caracter"); break;
        case NO_TIPO_BOOL:  fprintf(out, "logico");   break;
        default: break;
    }
}

/* ── Expressão inline ─────────────────────────────────────────────── */
static void gen_expr(No *n, FILE *out) {
    if (!n) return;
    switch (n->tipo) {
        case NO_ID:         fprintf(out, "%s",   n->val.sval); break;
        case NO_LIT_INT:    fprintf(out, "%d",   n->val.ival); break;
        case NO_LIT_FLOAT:  fprintf(out, "%g",   n->val.fval); break;
        case NO_LIT_CHAR:   fprintf(out, "'%c'", n->val.cval); break;
        case NO_LIT_STRING: fprintf(out, "%s",   n->val.sval); break;
        case NO_LIT_TRUE:   fprintf(out, "verdadeiro"); break;
        case NO_LIT_FALSE:  fprintf(out, "falso");      break;

        /* Aritméticos — parentesados para preservar precedência */
        case NO_OP_MAIS:
            fprintf(out,"(");gen_expr(n->filho[0],out);
            fprintf(out,"+");gen_expr(n->filho[1],out);fprintf(out,")"); break;
        case NO_OP_MENOS:
            fprintf(out,"(");gen_expr(n->filho[0],out);
            fprintf(out,"-");gen_expr(n->filho[1],out);fprintf(out,")"); break;
        case NO_OP_MULT:
            fprintf(out,"(");gen_expr(n->filho[0],out);
            fprintf(out,"*");gen_expr(n->filho[1],out);fprintf(out,")"); break;
        case NO_OP_DIV:
            fprintf(out,"(");gen_expr(n->filho[0],out);
            fprintf(out,"/");gen_expr(n->filho[1],out);fprintf(out,")"); break;
        case NO_OP_MOD:
            fprintf(out,"(");gen_expr(n->filho[0],out);
            fprintf(out," mod ");gen_expr(n->filho[1],out);fprintf(out,")"); break;

        /* Comparadores — Calango usa == e != igual ao C */
        case NO_OP_EQ:
            fprintf(out,"(");gen_expr(n->filho[0],out);
            fprintf(out,"==");gen_expr(n->filho[1],out);fprintf(out,")"); break;
        case NO_OP_NEQ:
            fprintf(out,"(");gen_expr(n->filho[0],out);
            fprintf(out,"!=");gen_expr(n->filho[1],out);fprintf(out,")"); break;
        case NO_OP_LT:
            fprintf(out,"(");gen_expr(n->filho[0],out);
            fprintf(out,"<");gen_expr(n->filho[1],out);fprintf(out,")"); break;
        case NO_OP_GT:
            fprintf(out,"(");gen_expr(n->filho[0],out);
            fprintf(out,">");gen_expr(n->filho[1],out);fprintf(out,")"); break;
        case NO_OP_LEQ:
            fprintf(out,"(");gen_expr(n->filho[0],out);
            fprintf(out,"<=");gen_expr(n->filho[1],out);fprintf(out,")"); break;
        case NO_OP_GEQ:
            fprintf(out,"(");gen_expr(n->filho[0],out);
            fprintf(out,">=");gen_expr(n->filho[1],out);fprintf(out,")"); break;

        /* Lógicos */
        case NO_OP_AND:
            fprintf(out,"(");gen_expr(n->filho[0],out);
            fprintf(out," e ");gen_expr(n->filho[1],out);fprintf(out,")"); break;
        case NO_OP_OR:
            fprintf(out,"(");gen_expr(n->filho[0],out);
            fprintf(out," ou ");gen_expr(n->filho[1],out);fprintf(out,")"); break;
        case NO_OP_NOT:
            fprintf(out,"nao ");gen_expr(n->filho[0],out); break;
        case NO_OP_NEG:
            fprintf(out,"-");gen_expr(n->filho[0],out); break;

        default: break;
    }
}

/* ── Printf → escreva/escreval ────────────────────────────────────── */
static int tem_newline(const char *s) {
    size_t len = strlen(s);
    return len >= 4 && s[len-3] == '\\' && s[len-2] == 'n';
}

static void string_limpa(const char *s, char *dest, size_t max) {
    size_t len = strlen(s);
    if (len < 2) { dest[0] = '\0'; return; }
    strncpy(dest, s + 1, max - 1);
    dest[max - 1] = '\0';
    size_t d = strlen(dest);
    if (d > 0) dest[d-1] = '\0';
    d = strlen(dest);
    if (d >= 2 && dest[d-2] == '\\' && dest[d-1] == 'n')
        dest[d-2] = '\0';
}

static void gen_printf(No *n, FILE *out, int nivel) {
    No *fmt  = n->filho[0];
    No *args = (n->n_filhos > 1) ? n->filho[1] : NULL;
    const char *cmd = tem_newline(fmt->val.sval) ? "escreval" : "escreva";

    indent(out, nivel);
    if (!args) {
        /* printf("texto\n") sem variáveis — emite string literal limpa */
        char buf[256];
        string_limpa(fmt->val.sval, buf, sizeof(buf));
        fprintf(out, "%s(\"%s\");\n", cmd, buf);
        return;
    }

    /* printf("texto: %d\n", var) com variáveis:
     * Preserva o texto fixo que precede o primeiro especificador de formato.
     * Ex.: "soma: %d\n" → escreval("soma: ", soma)
     */
    {
        char buf[256];
        string_limpa(fmt->val.sval, buf, sizeof(buf));

        /* Encontra o primeiro especificador %d/%f/%s/%c */
        char *spec = strpbrk(buf, "%");
        char prefixo[256] = "";
        if (spec && spec > buf) {
            size_t plen = (size_t)(spec - buf);
            if (plen >= sizeof(prefixo)) plen = sizeof(prefixo) - 1;
            strncpy(prefixo, buf, plen);
            prefixo[plen] = '\0';
        }

        fprintf(out, "%s(", cmd);
        int primeiro = 1;
        if (prefixo[0] != '\0') {
            fprintf(out, "\"%s\"", prefixo);
            primeiro = 0;
        }
        for (int i = 0; i < args->n_filhos; i++) {
            if (!primeiro) fprintf(out, ",");
            gen_expr(args->filho[i], out);
            primeiro = 0;
        }
        fprintf(out, ");\n");
    }
}

/* ── Nó completo ──────────────────────────────────────────────────── */
static void gen_no(No *n, FILE *out, int nivel) {
    if (!n) return;
    switch (n->tipo) {

        case NO_PROGRAMA:
            /* algoritmo NomeAlgo;  (nome do algoritmo com ; conforme exemplos reais) */
            fprintf(out, "algoritmo MiniC;\n");
            if (n->filho[0]) gen_no(n->filho[0], out, nivel);
            break;

        case NO_BLOCO: {
            No *decls  = n->n_filhos > 0 ? n->filho[0] : NULL;
            No *instrs = n->n_filhos > 1 ? n->filho[1] : NULL;
            fprintf(out, "principal\n");
            if (decls)  gen_lista(decls,  out, nivel);
            if (instrs) gen_lista(instrs, out, nivel + 1);
            fprintf(out, "fimPrincipal\n");
            break;
        }

        case NO_DECL: {
            /* Declaração: tipo nome;
             * Se havia init (NO_ATRIB como segundo filho), emite decl + atrib separados.
             * O Calango não suporta inicialização na declaração. */
            No *tipo_no = n->filho[0];
            No *segundo = n->filho[1];

            indent(out, nivel + 1);

            if (segundo->tipo == NO_ID) {
                gen_tipo(tipo_no, out);
                fprintf(out, " %s;\n", segundo->val.sval);
            } else {
                /* com init: emite declaração e atribuição separadas */
                No *id   = segundo->filho[0];
                No *expr = segundo->filho[1];
                gen_tipo(tipo_no, out);
                fprintf(out, " %s;\n", id->val.sval);
                indent(out, nivel + 2);
                fprintf(out, "%s = ", id->val.sval);
                gen_expr(expr, out);
                fprintf(out, ";\n");
            }
            break;
        }

        case NO_ATRIB:
            /* atribuição: nome = expr;  (com ; conforme exemplos reais) */
            indent(out, nivel);
            fprintf(out, "%s = ", n->filho[0]->val.sval);
            gen_expr(n->filho[1], out);
            fprintf(out, ";\n");
            break;

        case NO_IF:
            indent(out, nivel);
            fprintf(out, "se("); gen_expr(n->filho[0], out); fprintf(out, ")entao\n");
            if (n->filho[1]) gen_lista(n->filho[1], out, nivel + 1);
            indent(out, nivel);
            fprintf(out, "fimSe\n");
            break;

        case NO_IF_ELSE:
            indent(out, nivel);
            fprintf(out, "se("); gen_expr(n->filho[0], out); fprintf(out, ")entao\n");
            if (n->filho[1]) gen_lista(n->filho[1], out, nivel + 1);
            indent(out, nivel);
            fprintf(out, "senao\n");
            if (n->filho[2]) gen_lista(n->filho[2], out, nivel + 1);
            indent(out, nivel);
            fprintf(out, "fimSe\n");
            break;

        case NO_WHILE:
            indent(out, nivel);
            fprintf(out, "enquanto("); gen_expr(n->filho[0], out); fprintf(out, ")faca\n");
            if (n->filho[1]) gen_lista(n->filho[1], out, nivel + 1);
            indent(out, nivel);
            fprintf(out, "fimEnquanto\n");
            break;

        case NO_FOR: {
            /* for → enquanto: init antes do loop, passo no final do corpo */
            No *init  = n->filho[0];
            No *cond  = n->filho[1];
            No *passo = n->filho[2];
            No *corpo = n->filho[3];
            gen_no(init, out, nivel);
            indent(out, nivel);
            fprintf(out, "enquanto("); gen_expr(cond, out); fprintf(out, ")faca\n");
            if (corpo) gen_lista(corpo, out, nivel + 1);
            gen_no(passo, out, nivel + 1);
            indent(out, nivel);
            fprintf(out, "fimEnquanto\n");
            break;
        }

        case NO_DO_WHILE:
            /* faca ... enquanto(cond);  — o ; no enquanto é obrigatório */
            indent(out, nivel);
            fprintf(out, "faca\n");
            if (n->filho[0]) gen_lista(n->filho[0], out, nivel + 1);
            indent(out, nivel);
            fprintf(out, "enquanto("); gen_expr(n->filho[1], out); fprintf(out, ");\n");
            break;

        case NO_PRINTF:
            gen_printf(n, out, nivel);
            break;

        case NO_SCANF: {
            No *id = n->filho[1];
            indent(out, nivel);
            fprintf(out, "leia(%s);\n", id->val.sval);
            break;
        }

        default: break;
    }
}

static void gen_lista(No *n, FILE *out, int nivel) {
    for (No *cur = n; cur; cur = cur->prox)
        gen_no(cur, out, nivel);
}

void gerar(No *raiz, FILE *saida) {
    if (raiz) gen_no(raiz, saida, 0);
}


void gerarASTOtimizada(No *raiz, FILE *saida) {
    if (!raiz || !saida) {
        return;
    }

    otimizarAST(raiz);
    gerar(raiz, saida);
}