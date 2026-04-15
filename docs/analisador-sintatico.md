# Analisador Sintático

## Visão Geral

O analisador sintático (ou *parser*) é a segunda fase do compilador. Ele recebe a sequência de tokens produzida pelo analisador léxico e verifica se essa sequência obedece à **gramática** da linguagem Mini C. Se a estrutura for válida, o parser conclui a análise com sucesso; caso contrário, reporta um erro sintático com a posição (linha e coluna) onde o problema foi encontrado.

O analisador foi implementado com **Bison**, que gera automaticamente um parser **LALR(1)** a partir das regras gramaticais definidas no arquivo `parser.y`.

---

## Gramática Livre de Contexto

A gramática abaixo define formalmente a linguagem Mini C suportada pelo compilador. A notação utilizada é BNF (*Backus-Naur Form*), onde:

- `→` separa o nome da regra de suas produções
- `|` indica alternativas
- `ε` representa a produção vazia (a regra pode não gerar nada)

### Estrutura geral do programa

```
programa        → KW_INT KW_MAIN LPAREN RPAREN LBRACE bloco RBRACE

bloco           → declaracoes instrucoes
```

Todo programa Mini C é composto por um único bloco `main`. Dentro dele, as declarações de variáveis vêm obrigatoriamente antes das instruções.

### Declarações

```
declaracoes     → declaracoes declaracao
                | ε

declaracao      → tipo ID SEMICOLON
                | tipo ID ASSIGN expressao SEMICOLON

tipo            → KW_INT
                | KW_FLOAT
                | KW_CHAR
                | KW_BOOL
```

### Instruções

```
instrucoes      → instrucoes instrucao
                | ε

instrucao       → atribuicao
                | condicional
                | enquanto
                | para
                | faca_enquanto
                | saida
                | entrada
```

### Atribuição

```
atribuicao      → ID ASSIGN expressao SEMICOLON
```

### Condicional

```
condicional     → KW_IF LPAREN expressao RPAREN LBRACE instrucoes RBRACE
                | KW_IF LPAREN expressao RPAREN LBRACE instrucoes RBRACE
                  KW_ELSE LBRACE instrucoes RBRACE
```

### Estruturas de repetição

```
enquanto        → KW_WHILE LPAREN expressao RPAREN LBRACE instrucoes RBRACE

para            → KW_FOR LPAREN atribuicao expressao SEMICOLON atribuicao_for RPAREN
                  LBRACE instrucoes RBRACE

atribuicao_for  → ID ASSIGN expressao

faca_enquanto   → KW_DO LBRACE instrucoes RBRACE
                  KW_WHILE LPAREN expressao RPAREN SEMICOLON
```

### Entrada e saída

```
saida           → KW_PRINTF LPAREN LIT_STRING RPAREN SEMICOLON
                | KW_PRINTF LPAREN LIT_STRING COMMA args_printf RPAREN SEMICOLON

args_printf     → expressao
                | args_printf COMMA expressao

entrada         → KW_SCANF LPAREN LIT_STRING COMMA ID RPAREN SEMICOLON
```

### Expressões

A hierarquia de regras abaixo garante a **precedência de operadores** — operadores com maior precedência estão mais profundos na árvore gramatical, sendo resolvidos primeiro.

```
expressao       → expressao OR     expressao       (menor precedência)
                | expressao AND    expressao
                | expressao EQ     expressao
                | expressao NEQ    expressao
                | expressao LT     expressao
                | expressao GT     expressao
                | expressao LEQ    expressao
                | expressao GEQ    expressao
                | expressao PLUS   expressao
                | expressao MINUS  expressao
                | expressao TIMES  expressao
                | expressao DIVIDE expressao
                | expressao MOD    expressao
                | NOT expressao
                | MINUS expressao                  (menos unário)
                | LPAREN expressao RPAREN
                | ID
                | LIT_INT
                | LIT_FLOAT
                | LIT_CHAR
                | LIT_STRING
                | LIT_TRUE
                | LIT_FALSE                        (maior precedência)
```

---

## Precedência de Operadores

A tabela abaixo resume a ordem de precedência dos operadores, do menor para o maior. Operadores na mesma linha têm a mesma precedência.

| Nível | Operadores | Associatividade |
|---|---|---|
| 1 (menor) | `\|\|` | esquerda |
| 2 | `&&` | esquerda |
| 3 | `==`, `!=` | esquerda |
| 4 | `<`, `>`, `<=`, `>=` | esquerda |
| 5 | `+`, `-` | esquerda |
| 6 | `*`, `/`, `%` | esquerda |
| 7 | `!` | direita |
| 8 (maior) | `-` unário | direita |

> **Exemplo:** em `2 + 3 * 4`, o `*` tem precedência maior que `+`, então `3 * 4` é resolvido primeiro, resultando em `14`. Em `!x || y`, o `!` tem precedência maior que `||`, então `!x` é avaliado primeiro.

---

## Tratamento de Erros

Quando o parser encontra uma sequência de tokens que não corresponde a nenhuma regra da gramática, ele chama a função `yyerror`, que reporta o erro com a posição exata no código-fonte:

```
ERRO SINTÁTICO [linha 5, col 12]: syntax error (token: 'else')
```

O parser utiliza os contadores de linha e coluna mantidos pelo analisador léxico para localizar precisamente o erro.

---

## Exemplo de Análise

Dado o programa Mini C abaixo:

```c
int main() {
    int x = 10;
    int y;
    if (x > 5) {
        y = x + 1;
    } else {
        y = 0;
    }
    while (y < 20) {
        y = y + 1;
    }
    printf("resultado: %d", y);
}
```

O parser reconhece a seguinte estrutura:

```
programa
└── bloco
    ├── declaracoes
    │   ├── declaracao: int x = 10
    │   └── declaracao: int y
    └── instrucoes
        ├── condicional: if (x > 5) { ... } else { ... }
        ├── enquanto: while (y < 20) { ... }
        └── saida: printf("resultado: %d", y)
```

Saída esperada:
```
Análise sintática concluída: programa válido.
```

## Histórico de Versões

| Versão | Descrição | Data | Responsável |
| ------ | --------- | ---- | ----------- |
| `0.1` | Criação da página e início da documentação. | 11/04/2026 | [@Luiz Faria](https://github.com/luizfaria1989), [@João Pedro](https://github.com/Jadequilin),  [@Rivaldâvio](https://github.com/RivaFilho) |