#ifndef TABELA_H
#define TABELA_H

typedef struct simbolo {
    char nome[32];
    char tipo[16];
    int linhaDeclaracao;
    int inicializado;
    int usado;
    int ordemDeclaracao;
    int escopo;
    struct simbolo *prox;
} Simbolo;

/* Controle da tabela */
void inicializarTabela();
void liberarTabela();
void imprimirTabela();

/* Controle de escopo */
void entrarEscopo();
int sairEscopo();

/* Operações principais */
int inserirSimbolo(const char *nome, const char *tipo, int linhaDeclaracao);
Simbolo *buscarSimbolo(const char *nome);
Simbolo *buscarSimboloEscopoAtual(const char *nome);

/* Atualização de estado */
int marcarInicializado(const char *nome);
int marcarUsado(const char *nome);

/* Utilitários de tipo */
int tipoEhNumerico(const char *tipo);
int tipoEhBooleano(const char *tipo);
int tiposCompativeisAtribuicao(const char *tipoDestino, const char *tipoOrigem);

const char *resultadoOperacaoAritmetica(const char *tipoEsq, const char *tipoDir, char op);
const char *resultadoOperacaoRelacional(const char *tipoEsq, const char *tipoDir);
const char *resultadoOperacaoLogica(const char *tipoEsq, const char *tipoDir);
const char *resultadoMenosUnario(const char *tipo);
const char *resultadoNot(const char *tipo);

#endif