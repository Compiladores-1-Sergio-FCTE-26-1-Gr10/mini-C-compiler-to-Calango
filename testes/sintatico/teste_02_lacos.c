/* TESTE SINTÁTICO 02 — while, for e do-while válidos */
int main() {
    int i;
    int soma;
    soma = 0;
    for (i = 0; i < 10; i = i + 1) {
        soma = soma + i;
    }
    while (soma > 0) {
        soma = soma - 1;
    }
    do {
        i = i - 1;
    } while (i > 0);
}
