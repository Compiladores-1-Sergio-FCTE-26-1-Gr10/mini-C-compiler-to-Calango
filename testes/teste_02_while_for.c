/* TESTE 02 — while e for válidos */
int main() {
    int i;
    int soma;

    soma = 0;
    i = 0;

    while (i < 10) {
        soma = soma + i;
        i = i + 1;
    }

    for (i = 0; i < 5; i = i + 1) {
        soma = soma + 1;
    }

    printf("soma: %d\n", soma);
}
