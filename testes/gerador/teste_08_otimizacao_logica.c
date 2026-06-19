/* TESTE GERADOR 08 — otimização: expressões lógicas constantes */
int main() {
    bool ativo;
    ativo = true && (false || true);
    if (ativo || false) {
        printf("ok\n");
    }
}