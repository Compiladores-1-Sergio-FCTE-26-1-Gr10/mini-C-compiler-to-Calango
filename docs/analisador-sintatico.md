# Analisador Sintático

## Visão Geral

> Um compilador realiza uma análise sintática do programa de origem para discernir sua estrutura e para verificar se o programa é válido em uma determinada linguagem de programação L. Qualquer desvio das regras de L é denominado erro sintático.(Tremblay; Sorenson, 2008, p.186, tradução da equipe).

Em _Theory and Practice of Compiler Writing_, os autores explicam que os erros sintáticos "não são erros no significado do programa; são erros que tornam o programa sem sentido, simplesmente porque qualquer programa sintaticamente inválido é sem sentido" (Tremblay; Sorenson, 2008, p.186, tradução da equipe). Dessa forma, ao construirmos a segunda etapa do compilador, o analisador sintático, ele será capaz de avaliar se a estrutura do programa em Mini C está correta sintaticamente e poderá passar para a próxima etapa de compilação.

Caso o _parser_ encontre um erro sintático ao analisar o código, ele deverá retornar um erro sintático com a posição (linha e coluna) onde o problema foi encontrado.

O analisador foi implementado com Bison, que gera automaticamente um parser **LALR(1)** a partir das regras gramaticais definidas no arquivo `parser.y`.

---

## Gramática Livre de Contexto

A gramática abaixo define formalmente a linguagem Mini C suportada pelo compilador. Utilizamos a notação Backus Naur Form (BNF) para representar as relações da gramática. Tremblay e Sorenson (2008, p.36) explicam que ao usar esta terminologia, o símbolo <sentença> é um elemento dos símbolos não terminais (denotados por **$V$** no documento do [analisador léxico](analisador-lexico.md)) e o símbolo "sentença" é um elemento do conjunto dos símbolos terminais (denotados por **$\Sigma$** no documento do [analisador léxico](analisador-lexico.md)).

- `<nome>` representa um símbolo não terminal (variável sintática);
- `::=` separa o símbolo não terminal das suas produções;
- `|` indica alternativas para a produção do mesmo símbolo terminal.

> **Nota:** Utilizamos nomes em letras maiúsculas (ex: `KW_INT`, `KW_MAIN`) para representar os símbolos terminais, ou seja, os tokens gerados pelo [analisador léxico](analisador-lexico.md).

### Estrutura geral do programa

Definimos a estrutura geral de um programa escrito em Mini C como sendo um conjunto de símbolos que obedecem a seguinte regra sintática

```
<programa>      ::= KW_INT KW_MAIN LPAREN RPAREN LBRACE <bloco> RBRACE

<bloco>         ::= <declaracoes> <instrucoes>
```

> **Nota:** Todo programa Mini C é composto por um único bloco `main`. Dentro dele, as declarações de variáveis vêm obrigatoriamente antes das instruções.

### Declarações

Definimos a estrutura das declarações de um programa escrito em Mini C como sendo um conjunto de símbolos que obedecem a seguinte regra sintática

```
<declaracoes>   ::= <declaracoes> <declaracao> 
                  | /* vazio */

<declaracao>    ::= <tipo> ID SEMICOLON 
                  | <tipo> ID ASSIGN <expressao> SEMICOLON

<tipo>          ::= KW_INT 
                  | KW_FLOAT 
                  | KW_CHAR 
                  | KW_BOOL
```

### Instruções

Definimos a estrutura das instruções de um programa escrito em Mini C como sendo um conjunto de símbolos que obedecem a seguinte regra sintática

```
<instrucoes>    ::= <instrucoes> <instrucao> 
                  | /* vazio */

<instrucao>     ::= <atribuicao> 
                  | <condicional> 
                  | <enquanto> 
                  | <para> 
                  | <faca_enquanto> 
                  | <saida> 
                  | <entrada>
```

### Atribuição

Definimos a estrutura da instrução de atribuição de um programa escrito em Mini C como sendo um conjunto de símbolos que obedecem a seguinte regra sintática

```
<atribuicao>    ::= ID ASSIGN <expressao> SEMICOLON
```

### Condicional

Definimos a estrutura condicional de um programa escrito em Mini C como sendo um conjunto de símbolos que obedecem a seguinte regra sintática:

```
<condicional>   ::= KW_IF LPAREN <expressao> RPAREN LBRACE <instrucoes> RBRACE
                  | KW_IF LPAREN <expressao> RPAREN LBRACE <instrucoes> RBRACE KW_ELSE LBRACE <instrucoes> RBRACE
```

### Estruturas de repetição

Definimos as estruturas de repetição de um programa escrito em Mini C como sendo um conjunto se símbolos que obedecem a seguinte regra sintática:

```
<enquanto>      ::= KW_WHILE LPAREN <expressao> RPAREN LBRACE <instrucoes> RBRACE

<para>          ::= KW_FOR LPAREN <atribuicao> <expressao> SEMICOLON <atribuicao_for> RPAREN LBRACE <instrucoes> RBRACE

<atribuicao_for>::= ID ASSIGN <expressao>

<faca_enquanto> ::= KW_DO LBRACE <instrucoes> RBRACE KW_WHILE LPAREN <expressao> RPAREN SEMICOLON
```

### Entrada e saída

Definimos as estruturas de entrada e saída de um programa escrito em Mini C como sendo um conjunto de símbolos que obedecem a seguinte regra sintática:


```
<saida>         ::= KW_PRINTF LPAREN LIT_STRING RPAREN SEMICOLON
                  | KW_PRINTF LPAREN LIT_STRING COMMA <args_printf> RPAREN SEMICOLON

<args_printf>   ::= <expressao> 
                  | <args_printf> COMMA <expressao>

<entrada>       ::= KW_SCANF LPAREN LIT_STRING COMMA ID RPAREN SEMICOLON
```

### Expressões

A hierarquia de regras abaixo garante a precedência de operadores. Operadores com maior precedência estão mais profundos na árvore gramatical, sendo resolvidos primeiro.

> **Nota:** Na BNF estrita, para garantir precedência matemática sem ambiguidade, é necessário criar regras aninhadas (ex: <expressao> ::= <expressao> + <termo>, <termo> ::= <termo> * <fator>). Porém, o Bison utiliza declarações de precedência de tokens (%left, %right) para resolver ambiguidades, a gramática abaixo é apresentada de forma "plana" para refletir exatamente a implementação do código-fonte.

```
<expressao>     ::= <expressao> OR <expressao>
                  | <expressao> AND <expressao>
                  | <expressao> EQ <expressao>
                  | <expressao> NEQ <expressao>
                  | <expressao> LT <expressao>
                  | <expressao> GT <expressao>
                  | <expressao> LEQ <expressao>
                  | <expressao> GEQ <expressao>
                  | <expressao> PLUS <expressao>
                  | <expressao> MINUS <expressao>
                  | <expressao> TIMES <expressao>
                  | <expressao> DIVIDE <expressao>
                  | <expressao> MOD <expressao>
                  | NOT <expressao>
                  | MINUS <expressao>
                  | LPAREN <expressao> RPAREN
                  | ID
                  | LIT_INT
                  | LIT_FLOAT
                  | LIT_CHAR
                  | LIT_STRING
                  | LIT_TRUE
                  | LIT_FALSE
```

---

## Precedência de Operadores

A Tabela 01 resume a ordem de precedência dos operadores, do menor para o maior. Operadores na mesma linha possuem a mesma precedência.

**Tabela 01:** Ordem de precedência dos operadores.

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

**Autor:** [Luiz Faria](https://github.com/luizfaria1989), [João Pedro](https://github.com/Jadequilin),  [Rivaldâvio](https://github.com/RivaFilho).

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

---

## Bibliografia 

1. TREMBLAY, Jean-Paul; SORENSON, Paul G. **The Theory and Practice of Compiler Writing**. 1ª edição. 2008.

---

## Histórico de Versões

| Versão | Descrição | Data | Responsável |
| ------ | --------- | ---- | ----------- |
| `0.1` | Criação da página e início da documentação. | 11/04/2026 | [@Luiz Faria](https://github.com/luizfaria1989), [@João Pedro](https://github.com/Jadequilin),  [@Rivaldâvio](https://github.com/RivaFilho) |
| `0.2` | Adiciona referências bibliográficas para os tópicos do analisador sintático; corrige notação BNF. | 21/04/2026 | [Luiz Faria](https://github.com/luizfaria1989)  |