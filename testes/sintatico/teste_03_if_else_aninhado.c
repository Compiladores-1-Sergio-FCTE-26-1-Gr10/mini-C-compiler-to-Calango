/* TESTE SINTÁTICO 03 — if/else aninhado e expressões compostas */
int main() {
    int x;
    x = 7;
    if (x > 10) {
        printf("grande\n");
    } else {
        if (x > 5) {
            printf("medio\n");
        } else {
            printf("pequeno\n");
        }
    }
}
