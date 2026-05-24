/* TESTE 08 — Expressões lógicas e relacionais compostas */
int main() {
    int x;
    int y;
    bool r;

    x = 3;
    y = 7;
    r = (x < y) && (y > 0);

    if (r || (x == 3)) {
        printf("ok\n");
    }
}
