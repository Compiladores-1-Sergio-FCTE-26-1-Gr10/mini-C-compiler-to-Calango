/* TESTE SINTÁTICO 06 — ERRO: parêntese não fechado no if */
int main() {
    int x;
    x = 1;
    if (x > 0 {
        printf("falta paren\n");
    }
}
