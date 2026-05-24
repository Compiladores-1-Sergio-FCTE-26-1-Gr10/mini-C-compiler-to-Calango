#include "tabela.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Simbolo *tabela = NULL;
static int escopoAtual = 0;
static int proximaOrdemDeclaracao = 0;

static void copiarStringSegura(char *destino, const char *origem, size_t tamanho) {
    if (destino == NULL || origem == NULL || tamanho == 0) {
        return;
    }

    strncpy(destino, origem, tamanho - 1);
    destino[tamanho - 1] = '\0';
}

void inicializarTabela() {
    tabela = NULL;
    escopoAtual = 0;
    proximaOrdemDeclaracao = 0;
}

void liberarTabela() {
    Simbolo *atual = tabela;
    Simbolo *prox;

    while (atual != NULL) {
        prox = atual->prox;
        free(atual);
        atual = prox;
    }

    tabela = NULL;
    escopoAtual = 0;
    proximaOrdemDeclaracao = 0;
}

void entrarEscopo() {
    escopoAtual++;
}

int sairEscopo() {
    Simbolo *atual = tabela;
    Simbolo *anterior = NULL;
    Simbolo *temp;

    if (escopoAtual == 0) {
        return 0;
    }

    while (atual != NULL) {
        if (atual->escopo == escopoAtual) {
            temp = atual;

            if (anterior == NULL) {
                tabela = atual->prox;
                atual = tabela;
            } else {
                anterior->prox = atual->prox;
                atual = atual->prox;
            }

            free(temp);
        } else {
            anterior = atual;
            atual = atual->prox;
        }
    }

    escopoAtual--;
    return 1;
}

Simbolo *buscarSimboloEscopoAtual(const char *nome) {
    Simbolo *s = tabela;

    while (s != NULL) {
        if (strcmp(s->nome, nome) == 0 && s->escopo == escopoAtual) {
            return s;
        }
        s = s->prox;
    }

    return NULL;
}

Simbolo *buscarSimbolo(const char *nome) {
    Simbolo *s = tabela;
    Simbolo *encontrado = NULL;

    while (s != NULL) {
        if (strcmp(s->nome, nome) == 0 && s->escopo <= escopoAtual) {
            if (encontrado == NULL || s->escopo >= encontrado->escopo) {
                encontrado = s;
            }
        }
        s = s->prox;
    }

    return encontrado;
}

int inserirSimbolo(const char *nome, const char *tipo, int linhaDeclaracao) {
    Simbolo *novo;
    Simbolo *ultimo;

    if (nome == NULL || tipo == NULL) {
        return 0;
    }

    if (buscarSimboloEscopoAtual(nome) != NULL) {
        return 0;
    }

    novo = (Simbolo *) malloc(sizeof(Simbolo));
    if (novo == NULL) {
        return 0;
    }

    copiarStringSegura(novo->nome, nome, sizeof(novo->nome));
    copiarStringSegura(novo->tipo, tipo, sizeof(novo->tipo));
    novo->linhaDeclaracao = linhaDeclaracao;
    novo->inicializado = 0;
    novo->usado = 0;
    novo->ordemDeclaracao = proximaOrdemDeclaracao++;
    novo->escopo = escopoAtual;
    novo->prox = NULL;

    if (tabela == NULL) {
        tabela = novo;
    } else {
        ultimo = tabela;
        while (ultimo->prox != NULL) {
            ultimo = ultimo->prox;
        }
        ultimo->prox = novo;
    }

    return 1;
}

int marcarInicializado(const char *nome) {
    Simbolo *s = buscarSimbolo(nome);

    if (s == NULL) {
        return 0;
    }

    s->inicializado = 1;
    return 1;
}

int marcarUsado(const char *nome) {
    Simbolo *s = buscarSimbolo(nome);

    if (s == NULL) {
        return 0;
    }

    s->usado = 1;
    return 1;
}

void imprimirTabela() {
    Simbolo *s = tabela;

    printf("\nTabela de Simbolos:\n");
    while (s != NULL) {
        printf(
            "Nome: %s | Tipo: %s | Linha: %d | Inicializado: %d | Usado: %d | Ordem: %d | Escopo: %d\n",
            s->nome,
            s->tipo,
            s->linhaDeclaracao,
            s->inicializado,
            s->usado,
            s->ordemDeclaracao,
            s->escopo
        );
        s = s->prox;
    }
}

int tipoEhNumerico(const char *tipo) {
    return strcmp(tipo, "int") == 0 || strcmp(tipo, "float") == 0;
}

int tipoEhBooleano(const char *tipo) {
    return strcmp(tipo, "bool") == 0;
}

int tiposCompativeisAtribuicao(const char *tipoDestino, const char *tipoOrigem) {
    if (tipoDestino == NULL || tipoOrigem == NULL) {
        return 0;
    }

    if (strcmp(tipoDestino, tipoOrigem) == 0) {
        return 1;
    }

    /* promocao permitida: int -> float */
    if (strcmp(tipoDestino, "float") == 0 && strcmp(tipoOrigem, "int") == 0) {
        return 1;
    }

    return 0;
}

const char *resultadoOperacaoAritmetica(const char *tipoEsq, const char *tipoDir, char op) {
    if (tipoEsq == NULL || tipoDir == NULL) {
        return "invalid";
    }

    if (op == '%') {
        if (strcmp(tipoEsq, "int") == 0 && strcmp(tipoDir, "int") == 0) {
            return "int";
        }
        return "invalid";
    }

    if (!tipoEhNumerico(tipoEsq) || !tipoEhNumerico(tipoDir)) {
        return "invalid";
    }

    if (strcmp(tipoEsq, "float") == 0 || strcmp(tipoDir, "float") == 0) {
        return "float";
    }

    return "int";
}

const char *resultadoOperacaoRelacional(const char *tipoEsq, const char *tipoDir) {
    if (tipoEsq == NULL || tipoDir == NULL) {
        return "invalid";
    }

    if (tipoEhNumerico(tipoEsq) && tipoEhNumerico(tipoDir)) {
        return "bool";
    }

    if (strcmp(tipoEsq, tipoDir) == 0) {
        return "bool";
    }

    return "invalid";
}

const char *resultadoOperacaoLogica(const char *tipoEsq, const char *tipoDir) {
    if (tipoEsq == NULL || tipoDir == NULL) {
        return "invalid";
    }

    if (strcmp(tipoEsq, "bool") == 0 && strcmp(tipoDir, "bool") == 0) {
        return "bool";
    }

    return "invalid";
}

const char *resultadoMenosUnario(const char *tipo) {
    if (tipo == NULL) {
        return "invalid";
    }

    if (tipoEhNumerico(tipo)) {
        return tipo;
    }

    return "invalid";
}

const char *resultadoNot(const char *tipo) {
    if (tipo == NULL) {
        return "invalid";
    }

    if (strcmp(tipo, "bool") == 0) {
        return "bool";
    }

    return "invalid";
}