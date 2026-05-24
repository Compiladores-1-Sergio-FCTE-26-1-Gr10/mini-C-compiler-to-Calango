/* TESTE GERADOR 05 — scanf e expressoes logicas */
int main() {
    int n;
    bool ok;
    scanf("%d", n);
    ok = (n > 0) && (n < 100);
    if (ok) {
        printf("valido\n");
    }
}
