#include "tabela.h"
#include <assert.h>
#include <stdio.h>

static void testa_remocao_de_simbolos_nao_usados(void) {
    inicializarTabela();

    assert(inserirSimbolo("usada", "int", 1) == 1);
    assert(inserirSimbolo("morta", "float", 2) == 1);
    assert(inserirSimbolo("tambem_usada", "bool", 3) == 1);

    assert(marcarUsado("usada") == 1);
    assert(marcarUsado("tambem_usada") == 1);

    removerVariaveisNaoUsadas();

    assert(buscarSimbolo("usada") != NULL);
    assert(buscarSimbolo("tambem_usada") != NULL);
    assert(buscarSimbolo("morta") == NULL);

    liberarTabela();
}

static void testa_preservacao_de_escopo_ativo(void) {
    inicializarTabela();
    entrarEscopo();

    assert(inserirSimbolo("global_usada", "int", 10) == 1);
    assert(inserirSimbolo("global_morta", "int", 11) == 1);
    assert(marcarUsado("global_usada") == 1);

    removerVariaveisNaoUsadas();

    assert(buscarSimbolo("global_usada") != NULL);
    assert(buscarSimbolo("global_morta") == NULL);

    sairEscopo();
    liberarTabela();
}

int main(void) {
    testa_remocao_de_simbolos_nao_usados();
    testa_preservacao_de_escopo_ativo();

    printf("Teste removerVariaveisNaoUsadas: OK\n");
    return 0;
}