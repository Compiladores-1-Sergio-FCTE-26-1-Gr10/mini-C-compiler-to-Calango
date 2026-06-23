# Escopo do Projeto

## O que é o Mini C Compiler to Calango?

Este projeto é um compilador/transpilador que traduz um subconjunto da linguagem C, chamado neste trabalho de **Mini C**, para a linguagem de pseudocódigo **Calango**, utilizada na disciplina de Algoritmos e Programação de Computadores da UnB/FGA.

O compilador recebe um arquivo `.c` como entrada e emite um programa equivalente em Calango, respeitando as construções suportadas pelo escopo definido pela equipe.

---

## Linguagens Envolvidas

### Mini C (entrada)

Subconjunto da linguagem C com as seguintes características:

- tipagem estática com quatro tipos primitivos: `int`, `float`, `char` e `bool`;
- estrutura obrigatória com uma única função `main`;
- declarações de variáveis antes das instruções;
- estruturas de controle `if/else`, `while`, `for` e `do-while`;
- entrada e saída via `printf` e `scanf`;
- operadores aritméticos, relacionais e lógicos.

_Extensão:_ `.c`

### Calango (saída)

A saída é um programa em Calango emitido no terminal pelo gerador. Por convenção do projeto, esse conteúdo pode ser salvo com extensão `.cal`.

| Construção Mini C | Equivalente Calango |
|---|---|
| `int` | `inteiro` |
| `float` | `real` |
| `char` | `caracter` |
| `bool` | `logico` |
| `if (...) { }` | `se(...)entao ... fimSe` |
| `if (...) { } else { }` | `se(...)entao ... senao ... fimSe` |
| `while (...) { }` | `enquanto(...)faca ... fimEnquanto` |
| `for (init; cond; passo) { }` | `init; enquanto(cond)faca ... passo; fimEnquanto` |
| `do { } while (...);` | `faca ... enquanto(...);` |
| `printf(...)` | `escreva(...)` / `escreval(...)` |
| `scanf(...)` | `leia(...)` |
| `&&` | `e` |
| `\|\|` | `ou` |
| `!` | `nao` |
| `%` | `mod` |
| `true` | `verdadeiro` |
| `false` | `falso` |
| `int main() { }` | `algoritmo MiniC; principal ... fimPrincipal` |

---

## O que é suportado

### Tipos de dados

| Tipo Mini C | Descrição |
|---|---|
| `int` | Números inteiros. |
| `float` | Números reais. |
| `char` | Caractere simples. |
| `bool` | Valor lógico (`true` / `false`). |

### Declaração de variáveis

```c
int x;
float y = 3.14;
char letra = 'a';
bool flag = true;
```

Declarações devem estar no início do bloco `main`, antes das instruções.

### Estruturas de controle

```c
if (x > 0) {
    printf("positivo");
} else {
    printf("nao positivo");
}

while (x < 10) {
    x = x + 1;
}

for (i = 0; i < 10; i = i + 1) {
    printf("%d", i);
}

do {
    x = x - 1;
} while (x > 0);
```

### Entrada e saída

```c
printf("mensagem");
printf("%d", x);
scanf("%d", x);
```

### Operadores

| Categoria | Operadores |
|---|---|
| Aritméticos | `+`, `-`, `*`, `/`, `%` |
| Relacionais | `==`, `!=`, `<`, `>`, `<=`, `>=` |
| Lógicos | `&&`, `\|\|`, `!` |
| Atribuição | `=` |

---

## O que não é suportado

As construções abaixo estão fora do escopo desta versão do compilador.

| Construção | Exemplo | Motivo |
|---|---|---|
| Funções além do `main` | `int soma(int a, int b)` | Exigiria chamada, retorno e escopos de função. |
| `return` | `return x;` | Consequência da restrição de funções. |
| `void` | `void f()` | Consequência da restrição de funções. |
| Arrays | `int v[10];` | Exigiria acesso indexado e tradução para vetores. |
| Ponteiros e endereço | `int *p`, `&x` | Não há correspondência direta no Calango usado pelo projeto. |
| Cast explícito | `(int) x` | Fora do escopo da gramática. |
| Atribuição composta | `x += 1;` | Deve ser escrita como `x = x + 1;`. |
| Incremento/decremento | `x++`, `x--` | Deve ser escrito como `x = x + 1;` ou `x = x - 1;`. |
| Operadores bitwise | `x & y`, `x \| y`, `x ^ y` | Não há correspondência direta na linguagem alvo. |
| Operador ternário | `x > 0 ? 1 : 0` | Deve ser representado com `if/else`. |

---

## Fases do Compilador

| Fase | Descrição | Ferramenta/estrutura | Status |
|---|---|---|---|
| Análise Léxica | Identificação de tokens e erros léxicos. | Flex | Concluída. |
| Análise Sintática | Validação da gramática Mini C. | Bison | Concluída. |
| AST | Representação intermediária do programa. | Estruturas em C | Concluída. |
| Análise Semântica | Tabela de símbolos, declaração, uso e inicialização. | Bison + C | Concluída no escopo definido. |
| Otimização | Simplificações aplicadas sobre a AST. | C | Concluída no gerador. |
| Geração de Código | Tradução para Calango. | C | Concluída. |

---

## Histórico de Versões

| Versão | Descrição | Data | Responsável |
|---|---|---|---|
| `0.1` | Criação da página de escopo do projeto. | 11/04/2026 | [Luiz Faria](https://github.com/luizfaria1989), [João Pedro](https://github.com/Jadequilin), [Rivaldâvio](https://github.com/RivaFilho) |
| `0.2` | Atualização do escopo conforme a versão final do gerador e das otimizações. | 19/06/2026 | [João Pedro](https://github.com/Jadequilin), [Pedro Silva](https://github.com/314dro) |
