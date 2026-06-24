/* TESTE SEMÂNTICO 02 — while e for válidos */
int main() {
    int i;
    int soma;
    soma = 0;
    for (i = 0; i < 5; i = i + 1) {
        soma = soma + i;
    }
    while (soma > 0) {
        soma = soma - 1;
    }
}
