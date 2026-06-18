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


/* ── Otimizações ────────────────────────────────────────────────── */

typedef struct NomeUsado {
    char nome[256];
    struct NomeUsado *prox;
} NomeUsado;

static int eh_binario(TipoNo tipo) {
    return tipo == NO_OP_MAIS || tipo == NO_OP_MENOS || tipo == NO_OP_MULT ||
           tipo == NO_OP_DIV  || tipo == NO_OP_MOD   || tipo == NO_OP_EQ   ||
           tipo == NO_OP_NEQ  || tipo == NO_OP_LT    || tipo == NO_OP_GT   ||
           tipo == NO_OP_LEQ  || tipo == NO_OP_GEQ   || tipo == NO_OP_AND  ||
           tipo == NO_OP_OR;
}

static int eh_lit_bool(No *n) {
    return n && (n->tipo == NO_LIT_TRUE || n->tipo == NO_LIT_FALSE);
}

static int valor_bool(No *n) {
    return n && n->tipo == NO_LIT_TRUE;
}

static int eh_lit_num(No *n) {
    return n && (n->tipo == NO_LIT_INT || n->tipo == NO_LIT_FLOAT);
}

static double valor_num(No *n) {
    return n->tipo == NO_LIT_FLOAT ? n->val.fval : n->val.ival;
}

static int num_igual(No *n, double v) {
    return eh_lit_num(n) && valor_num(n) == v;
}

static void liberar_filhos(No *n) {
    if (!n) return;
    for (int i = 0; i < n->n_filhos; i++) {
        ast_libera(n->filho[i]);
        n->filho[i] = NULL;
    }
    n->n_filhos = 0;
}

static void substituir_por_int(No *n, int valor) {
    No *prox = n->prox;
    liberar_filhos(n);
    n->tipo = NO_LIT_INT;
    n->val.ival = valor;
    n->prox = prox;
}

static void substituir_por_float(No *n, float valor) {
    No *prox = n->prox;
    liberar_filhos(n);
    n->tipo = NO_LIT_FLOAT;
    n->val.fval = valor;
    n->prox = prox;
}

static void substituir_por_bool(No *n, int valor) {
    No *prox = n->prox;
    liberar_filhos(n);
    n->tipo = valor ? NO_LIT_TRUE : NO_LIT_FALSE;
    n->prox = prox;
}

static void substituir_por_filho(No *n, int indice_filho) {
    if (!n || indice_filho < 0 || indice_filho >= n->n_filhos || !n->filho[indice_filho]) {
        return;
    }

    No *mantido = n->filho[indice_filho];
    No *prox_original = n->prox;

    for (int i = 0; i < n->n_filhos; i++) {
        if (i != indice_filho) {
            ast_libera(n->filho[i]);
        }
    }

    No copia = *mantido;
    free(mantido);
    *n = copia;
    n->prox = prox_original;
}

static int nome_esta_no_conjunto(NomeUsado *set, const char *nome) {
    for (NomeUsado *cur = set; cur; cur = cur->prox) {
        if (strcmp(cur->nome, nome) == 0) {
            return 1;
        }
    }
    return 0;
}

static void adicionar_nome(NomeUsado **set, const char *nome) {
    if (!set || !nome || nome[0] == '\0' || nome_esta_no_conjunto(*set, nome)) {
        return;
    }

    NomeUsado *novo = calloc(1, sizeof(NomeUsado));
    if (!novo) {
        fprintf(stderr, "AST: sem memória para otimização\n");
        exit(1);
    }

    strncpy(novo->nome, nome, sizeof(novo->nome) - 1);
    novo->prox = *set;
    *set = novo;
}

static void liberar_conjunto(NomeUsado *set) {
    while (set) {
        NomeUsado *prox = set->prox;
        free(set);
        set = prox;
    }
}

static void coletar_usos(No *n, NomeUsado **usados) {
    if (!n) return;

    for (No *cur = n; cur; cur = cur->prox) {
        switch (cur->tipo) {
            case NO_ID:
                adicionar_nome(usados, cur->val.sval);
                break;

            case NO_DECL:
                if (cur->n_filhos > 1 && cur->filho[1] && cur->filho[1]->tipo == NO_ATRIB) {
                    coletar_usos(cur->filho[1]->filho[1], usados);
                }
                break;

            case NO_ATRIB:
                if (cur->n_filhos > 1) {
                    coletar_usos(cur->filho[1], usados);
                }
                break;

            case NO_SCANF:
                if (cur->n_filhos > 1 && cur->filho[1] && cur->filho[1]->tipo == NO_ID) {
                    adicionar_nome(usados, cur->filho[1]->val.sval);
                }
                break;

            case NO_PRINTF:
                if (cur->n_filhos > 1) {
                    coletar_usos(cur->filho[1], usados);
                }
                break;

            default:
                for (int i = 0; i < cur->n_filhos; i++) {
                    coletar_usos(cur->filho[i], usados);
                }
                break;
        }
    }
}

static const char *id_de_declaracao(No *n) {
    if (!n || n->tipo != NO_DECL || n->n_filhos < 2 || !n->filho[1]) {
        return NULL;
    }

    if (n->filho[1]->tipo == NO_ID) {
        return n->filho[1]->val.sval;
    }

    if (n->filho[1]->tipo == NO_ATRIB && n->filho[1]->n_filhos > 0 && n->filho[1]->filho[0]) {
        return n->filho[1]->filho[0]->val.sval;
    }

    return NULL;
}

static const char *id_de_atribuicao(No *n) {
    if (!n || n->tipo != NO_ATRIB || n->n_filhos < 1 || !n->filho[0] || n->filho[0]->tipo != NO_ID) {
        return NULL;
    }
    return n->filho[0]->val.sval;
}

static void remover_mortos_em_subarvores(No *n, NomeUsado *usados);

static void remover_mortos_em_lista(No **head, NomeUsado *usados) {
    if (!head) return;

    No *cur = *head;
    No *ant = NULL;

    while (cur) {
        No *prox = cur->prox;
        int remover = 0;
        const char *nome = NULL;

        remover_mortos_em_subarvores(cur, usados);

        if (cur->tipo == NO_DECL) {
            nome = id_de_declaracao(cur);
            remover = nome && !nome_esta_no_conjunto(usados, nome);
        } else if (cur->tipo == NO_ATRIB) {
            nome = id_de_atribuicao(cur);
            remover = nome && !nome_esta_no_conjunto(usados, nome);
        }

        if (remover) {
            cur->prox = NULL;
            ast_libera(cur);
            if (ant) {
                ant->prox = prox;
            } else {
                *head = prox;
            }
        } else {
            ant = cur;
        }

        cur = prox;
    }
}

static void remover_mortos_em_subarvores(No *n, NomeUsado *usados) {
    if (!n) return;

    switch (n->tipo) {
        case NO_PROGRAMA:
            if (n->n_filhos > 0) remover_mortos_em_subarvores(n->filho[0], usados);
            break;

        case NO_BLOCO:
            if (n->n_filhos > 0) remover_mortos_em_lista(&n->filho[0], usados);
            if (n->n_filhos > 1) remover_mortos_em_lista(&n->filho[1], usados);
            break;

        case NO_IF:
            if (n->n_filhos > 1) remover_mortos_em_lista(&n->filho[1], usados);
            break;

        case NO_IF_ELSE:
            if (n->n_filhos > 1) remover_mortos_em_lista(&n->filho[1], usados);
            if (n->n_filhos > 2) remover_mortos_em_lista(&n->filho[2], usados);
            break;

        case NO_WHILE:
            if (n->n_filhos > 1) remover_mortos_em_lista(&n->filho[1], usados);
            break;

        case NO_FOR:
            if (n->n_filhos > 3) remover_mortos_em_lista(&n->filho[3], usados);
            break;

        case NO_DO_WHILE:
            if (n->n_filhos > 0) remover_mortos_em_lista(&n->filho[0], usados);
            break;

        default:
            break;
    }
}

static void aplicar_constant_folding(No *n) {
    if (!n) return;

    for (No *cur = n; cur; cur = cur->prox) {
        for (int i = 0; i < cur->n_filhos; i++) {
            aplicar_constant_folding(cur->filho[i]);
        }

        if (eh_binario(cur->tipo) && cur->n_filhos == 2) {
            No *esq = cur->filho[0];
            No *dir = cur->filho[1];

            if (eh_lit_num(esq) && eh_lit_num(dir)) {
                double a = valor_num(esq);
                double b = valor_num(dir);
                int resultado_inteiro = esq->tipo == NO_LIT_INT && dir->tipo == NO_LIT_INT;

                switch (cur->tipo) {
                    case NO_OP_MAIS:
                        if (resultado_inteiro) substituir_por_int(cur, (int)a + (int)b);
                        else substituir_por_float(cur, (float)(a + b));
                        break;
                    case NO_OP_MENOS:
                        if (resultado_inteiro) substituir_por_int(cur, (int)a - (int)b);
                        else substituir_por_float(cur, (float)(a - b));
                        break;
                    case NO_OP_MULT:
                        if (resultado_inteiro) substituir_por_int(cur, (int)a * (int)b);
                        else substituir_por_float(cur, (float)(a * b));
                        break;
                    case NO_OP_DIV:
                        if (b != 0.0) {
                            if (resultado_inteiro) substituir_por_int(cur, (int)a / (int)b);
                            else substituir_por_float(cur, (float)(a / b));
                        }
                        break;
                    case NO_OP_MOD:
                        if (resultado_inteiro && (int)b != 0) substituir_por_int(cur, (int)a % (int)b);
                        break;
                    case NO_OP_EQ:  substituir_por_bool(cur, a == b); break;
                    case NO_OP_NEQ: substituir_por_bool(cur, a != b); break;
                    case NO_OP_LT:  substituir_por_bool(cur, a <  b); break;
                    case NO_OP_GT:  substituir_por_bool(cur, a >  b); break;
                    case NO_OP_LEQ: substituir_por_bool(cur, a <= b); break;
                    case NO_OP_GEQ: substituir_por_bool(cur, a >= b); break;
                    default: break;
                }
                continue;
            }

            if (eh_lit_bool(esq) && eh_lit_bool(dir)) {
                int a = valor_bool(esq);
                int b = valor_bool(dir);

                switch (cur->tipo) {
                    case NO_OP_AND: substituir_por_bool(cur, a && b); break;
                    case NO_OP_OR:  substituir_por_bool(cur, a || b); break;
                    case NO_OP_EQ:  substituir_por_bool(cur, a == b); break;
                    case NO_OP_NEQ: substituir_por_bool(cur, a != b); break;
                    default: break;
                }
                continue;
            }

            if (esq && dir && esq->tipo == NO_LIT_CHAR && dir->tipo == NO_LIT_CHAR) {
                if (cur->tipo == NO_OP_EQ) {
                    substituir_por_bool(cur, esq->val.cval == dir->val.cval);
                    continue;
                }
                if (cur->tipo == NO_OP_NEQ) {
                    substituir_por_bool(cur, esq->val.cval != dir->val.cval);
                    continue;
                }
            }

            switch (cur->tipo) {
                case NO_OP_MAIS:
                    if (num_igual(esq, 0.0)) substituir_por_filho(cur, 1);
                    else if (num_igual(dir, 0.0)) substituir_por_filho(cur, 0);
                    break;
                case NO_OP_MENOS:
                    if (num_igual(dir, 0.0)) substituir_por_filho(cur, 0);
                    break;
                case NO_OP_MULT:
                    if (num_igual(esq, 1.0)) substituir_por_filho(cur, 1);
                    else if (num_igual(dir, 1.0)) substituir_por_filho(cur, 0);
                    else if (num_igual(esq, 0.0)) substituir_por_filho(cur, 0);
                    else if (num_igual(dir, 0.0)) substituir_por_filho(cur, 1);
                    break;
                case NO_OP_DIV:
                    if (num_igual(dir, 1.0)) substituir_por_filho(cur, 0);
                    break;
                case NO_OP_AND:
                    if (eh_lit_bool(esq)) {
                        if (valor_bool(esq)) substituir_por_filho(cur, 1);
                        else substituir_por_bool(cur, 0);
                    } else if (eh_lit_bool(dir)) {
                        if (valor_bool(dir)) substituir_por_filho(cur, 0);
                        else substituir_por_bool(cur, 0);
                    }
                    break;
                case NO_OP_OR:
                    if (eh_lit_bool(esq)) {
                        if (valor_bool(esq)) substituir_por_bool(cur, 1);
                        else substituir_por_filho(cur, 1);
                    } else if (eh_lit_bool(dir)) {
                        if (valor_bool(dir)) substituir_por_bool(cur, 1);
                        else substituir_por_filho(cur, 0);
                    }
                    break;
                default:
                    break;
            }
        } else if (cur->tipo == NO_OP_NEG && cur->n_filhos == 1 && cur->filho[0]) {
            No *f = cur->filho[0];
            if (f->tipo == NO_LIT_INT) substituir_por_int(cur, -f->val.ival);
            else if (f->tipo == NO_LIT_FLOAT) substituir_por_float(cur, -f->val.fval);
        } else if (cur->tipo == NO_OP_NOT && cur->n_filhos == 1 && eh_lit_bool(cur->filho[0])) {
            substituir_por_bool(cur, !valor_bool(cur->filho[0]));
        }
    }
}

void otimizarAST(No *n) {
    NomeUsado *usados = NULL;

    if (!n) return;

    aplicar_constant_folding(n);
    coletar_usos(n, &usados);
    remover_mortos_em_subarvores(n, usados);
    liberar_conjunto(usados);
}