/* TESTE SEMÂNTICO 16 — estruturas de controle aninhadas, válido */
int main() {
    int x;
    int total;
    total = 0;
    x = 5;
    while (x > 0) {
        if (x > 2) {
            total = total + x;
        }
        x = x - 1;
    }
}
