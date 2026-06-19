#include "ast.h"
#include "gerador.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static No *bin(TipoNo tipo, No *esq, No *dir) {
    No *n = ast_no(tipo, 1);
    ast_filho(n, esq);
    ast_filho(n, dir);
    return n;
}

static No *declaracao(TipoNo tipo, const char *nome, No *inicializador) {
    No *decl = ast_no(NO_DECL, 1);
    ast_filho(decl, ast_no(tipo, 1));

    if (inicializador == NULL) {
        ast_filho(decl, ast_str(NO_ID, nome, 1));
        return decl;
    }

    No *atrib = ast_no(NO_ATRIB, 1);
    ast_filho(atrib, ast_str(NO_ID, nome, 1));
    ast_filho(atrib, inicializador);
    ast_filho(decl, atrib);
    return decl;
}

static No *printf_id(const char *nome) {
    No *saida = ast_no(NO_PRINTF, 1);
    No *args = ast_no(NO_ARGS, 1);

    ast_filho(saida, ast_str(NO_LIT_STRING, "\"%d\\n\"", 1));
    ast_filho(args, ast_str(NO_ID, nome, 1));
    ast_filho(saida, args);

    return saida;
}

static No *programa_para_teste(void) {
    No *programa = ast_no(NO_PROGRAMA, 1);
    No *bloco = ast_no(NO_BLOCO, 1);

    No *decl_morta = declaracao(NO_TIPO_INT, "morta", ast_int(99, 1));
    No *decl_usada = declaracao(
        NO_TIPO_INT,
        "resultado",
        bin(NO_OP_MAIS, ast_int(2, 1), ast_int(3, 1))
    );

    decl_morta->prox = decl_usada;

    ast_filho(bloco, decl_morta);
    ast_filho(bloco, printf_id("resultado"));
    ast_filho(programa, bloco);

    return programa;
}

static char *ler_arquivo(FILE *arquivo) {
    long tamanho;
    char *conteudo;

    assert(fflush(arquivo) == 0);
    assert(fseek(arquivo, 0, SEEK_END) == 0);
    tamanho = ftell(arquivo);
    assert(tamanho >= 0);
    assert(fseek(arquivo, 0, SEEK_SET) == 0);

    conteudo = calloc((size_t)tamanho + 1, sizeof(char));
    assert(conteudo != NULL);

    assert(fread(conteudo, 1, (size_t)tamanho, arquivo) == (size_t)tamanho);
    conteudo[tamanho] = '\0';
    return conteudo;
}

int main(void) {
    FILE *saida = tmpfile();
    No *raiz = programa_para_teste();
    char *codigo;

    assert(saida != NULL);

    gerarASTOtimizada(raiz, saida);
    codigo = ler_arquivo(saida);

    assert(strstr(codigo, "algoritmo MiniC;") != NULL);
    assert(strstr(codigo, "inteiro resultado;") != NULL);
    assert(strstr(codigo, "resultado = 5;") != NULL);
    assert(strstr(codigo, "escreval(resultado);") != NULL);
    assert(strstr(codigo, "morta") == NULL);
    assert(strstr(codigo, "2+3") == NULL);

    free(codigo);
    fclose(saida);
    ast_libera(raiz);

    printf("Teste gerarASTOtimizada: OK\n");
    return 0;
}