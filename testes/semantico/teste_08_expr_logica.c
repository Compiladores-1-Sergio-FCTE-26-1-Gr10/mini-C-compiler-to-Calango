/* TESTE SEMÂNTICO 08 — expressões lógicas e relacionais válidas */
int main() {
    int x;
    bool r;
    x = 3;
    r = (x > 0) && (x < 10);
    if (r || (x == 3)) {
        printf("ok\n");
    }
}
