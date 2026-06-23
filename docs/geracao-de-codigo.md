# Geração de Código

## Visão Geral

A etapa de geração de código transforma a AST construída durante a análise sintática/semântica em um programa equivalente na linguagem Calango.

No projeto Mini C Compiler to Calango, o gerador (`gerador.c`) percorre a AST e emite texto Calango no formato esperado pelo interpretador. A geração só ocorre quando a análise semântica não registra erros impeditivos.

---

## Mapeamento Mini C para Calango

A linguagem Calango possui estruturas semelhantes às de C, mas com palavras-chave e algumas restrições próprias. Por isso, parte da geração é tradução direta e parte exige adaptação.

**Tabela 01:** Mapeamento das estruturas do Mini C para o Calango.

| Mini C | Calango | Observação |
|---|---|---|
| `int`, `float`, `char`, `bool` | `inteiro`, `real`, `caracter`, `logico` | Mapeamento de tipos primitivos. |
| `true`, `false` | `verdadeiro`, `falso` | Mapeamento de literais booleanos. |
| `if (c) { ... }` | `se(c)entao ... fimSe` | Condicional simples. |
| `if (c) { ... } else { ... }` | `se(c)entao ... senao ... fimSe` | Condicional composta. |
| `while (c) { ... }` | `enquanto(c)faca ... fimEnquanto` | Laço de repetição. |
| `for (init; cond; passo) { ... }` | `init; enquanto(cond)faca ... passo; fimEnquanto` | O `for` é reescrito como `enquanto`. |
| `do { ... } while (c);` | `faca ... enquanto(c);` | Preserva a execução mínima do bloco. |
| `printf("texto")` | `escreva("texto")` ou `escreval("texto")` | `escreval` é usado quando há `
` ao final. |
| `printf("%d", x)` | `escreva(x)` | Os especificadores de formato não são emitidos. |
| `scanf("%d", x)` | `leia(x)` | O formato é descartado e a variável é usada como alvo de leitura. |
| `&&`, `\|\|`, `!`, `%` | `e`, `ou`, `nao`, `mod` | Mapeamento de operadores. |

**Autor(es):** [João Pedro](https://github.com/Jadequilin), [Pedro Silva](https://github.com/314dro).

---

## Funções do Gerador

**Tabela 02:** Funções principais do gerador de código.

| Função | Responsabilidade |
|---|---|
| `gerar(raiz, saida)` | Ponto de entrada da geração sem otimização explícita. Percorre a AST e escreve a saída. |
| `gerarASTOtimizada(raiz, saida)` | Aplica `otimizarAST` antes de chamar a geração final. |
| `gen_tipo(n, out)` | Converte nós de tipo da AST para palavras-chave do Calango. |
| `gen_expr(n, out)` | Emite expressões aritméticas, relacionais e lógicas de forma recursiva. |
| `gen_no(n, out, nivel)` | Emite declarações, comandos, blocos e estruturas de controle. |
| `gen_lista(n, out, nivel)` | Percorre listas encadeadas de nós usando o campo `prox`. |
| `gen_printf(n, out, nivel)` | Traduz comandos `printf` para `escreva` ou `escreval`. |

---

## Decisões de Tradução

**Tabela 03:** Decisões tomadas na geração de código.

| Decisão | Justificativa |
|---|---|
| **Declaração e inicialização separadas** | O Calango não utiliza declaração com inicialização no mesmo formato de C. Assim, `int x = 5;` é emitido como `inteiro x;` seguido de `x = 5;`. |
| **Conversão de `for` para `enquanto`** | O `for` do Mini C aceita inicialização, condição e passo em formato C. A forma mais segura de representar isso no Calango é usando `enquanto`. |
| **Parênteses em expressões** | O gerador preserva a precedência definida pela AST envolvendo operações binárias com parênteses. |
| **Geração bloqueada em erro semântico** | Se a análise semântica encontra erro, o gerador não emite código final. |
| **Otimização antes da geração** | A chamada `gerarASTOtimizada` reduz a AST antes da saída em Calango. |

---

## Exemplo de Saída

Entrada Mini C:

```c
int main() {
    int x = 5;
    if (x > 0) {
        printf("%d
", x);
    }
}
```

Saída Calango esperada:

```text
algoritmo MiniC;
principal
   inteiro x;
      x = 5;
   se((x>0))entao
      escreval(x);
   fimSe
fimPrincipal
```

---

## Histórico de Versões

| Versão | Descrição | Data | Responsável |
|---|---|---|---|
| `0.1` | Criação da página e documentação da etapa de geração de código. | 06/06/2026 | [Luiz Faria](https://github.com/luizfaria1989), [João Pedro](https://github.com/Jadequilin) |
| `0.2` | Atualização conforme a geração otimizada da AST e a tradução final para Calango. | 19/06/2026 | [João Pedro](https://github.com/Jadequilin), [Pedro Silva](https://github.com/314dro) |
