# Escopo do Projeto

## O que é o Mini C Compiler to Calango?

Este projeto é um compilador que traduz um subconjunto da linguagem C — chamado aqui de **Mini C** — para a linguagem de pseudocódigo **Calango**, utilizada na disciplina de Algoritmos e Programação de Computadores da UnB/FGA.

O compilador recebe um arquivo `.c` como entrada e produz como saída um programa equivalente em Calango, respeitando as construções suportadas pelo interpretador Calango.

---

## Linguagens Envolvidas

### Mini C (entrada)

Subconjunto da linguagem C com as seguintes características:

- Tipagem estática com quatro tipos primitivos: `int`, `float`, `char`, `bool`
- Estrutura obrigatória de programa: bloco `main` único
- Declarações de variáveis apenas no início do bloco
- Estruturas de controle: `if/else`, `while`, `for`, `do-while`
- Entrada e saída via `printf` e `scanf`
- Operadores aritméticos, relacionais e lógicos

_Extensão:_ `.c`

### Calango (saída)

Interpretador de pseudocódigo em português estruturado, desenvolvido na UnB. As principais correspondências entre Mini C e Calango são:

| Construção Mini C | Equivalente Calango |
|---|---|
| `int` | `inteiro` |
| `float` | `real` |
| `char` | `caracter` |
| `bool` | `logico` |
| `if (...) { }` | `se (...) entao ... fimSe` |
| `if (...) { } else { }` | `se (...) entao ... senao ... fimSe` |
| `while (...) { }` | `enquanto (...) faca ... fimEnquanto` |
| `for (init; cond; passo) { }` | `para (...) faca ... fimPara` |
| `do { } while (...);` | `faca ... enquanto (...);` |
| `printf(...)` | `escreva(...)` / `escreval(...)` |
| `scanf(...)` | `leia(...)` / `leiaCaracter(...)` |
| `&&` | `e` |
| `\|\|` | `ou` |
| `!` | `nao` |
| `%` | `mod` |
| `true` | `VERDADEIRO` |
| `false` | `FALSO` |
| `int main() { }` | `algoritmo nome; principal ... fimPrincipal` |

Obs.: como estamos fazendo uma transpilação entre linguagem C para Calango, o qual é interpretado na JVM, não há um arquivo de saída propriamente dito, mas sim um programa em Calango que pode ser executado no interpretador. Com isso, por votação na equipe, convencionamentos que o código gerado terá uma extensão `.cal`.

_Extensão:_ `.cal`

---

## O que é suportado

### Tipos de dados

| Tipo Mini C | Descrição |
|---|---|
| `int` | Números inteiros |
| `float` | Números reais |
| `char` | Caractere simples |
| `bool` | Valor lógico (`true` / `false`) |

### Declaração de variáveis

```c
int x;
float y = 3.14;
char letra = 'a';
bool flag = true;
```

Declarações devem estar **no início do bloco `main`**, antes de qualquer instrução.

### Estruturas de controle

```c
// Condicional simples
if (x > 0) {
    printf("positivo");
}

// Condicional composta
if (x > 0) {
    printf("positivo");
} else {
    printf("negativo");
}

// Enquanto
while (x < 10) {
    x = x + 1;
}

// Para
for (i = 0; i < 10; i = i + 1) {
    printf("%d", i);
}

// Faca-enquanto
do {
    x = x + 1;
} while (x < 10);
```

### Operadores

| Categoria | Operadores |
|---|---|
| Aritméticos | `+`, `-`, `*`, `/`, `%` |
| Relacionais | `==`, `!=`, `<`, `>`, `<=`, `>=` |
| Lógicos | `&&`, `\|\|`, `!` |
| Atribuição | `=` |

### Entrada e saída

```c
printf("mensagem");
printf("valor: %d", x);
scanf("%d", x);
```

### Literais

| Tipo | Exemplos |
|---|---|
| Inteiro | `0`, `42`, `100` |
| Real | `3.14`, `0.5`, `100.0` |
| Caractere | `'a'`, `'\n'`, `'\\'` |
| String | `"ola mundo"`, `"valor: "` |
| Booleano | `true`, `false` |

### Comentários

```c
// comentário de linha
/* comentário de bloco */
```

---

## O que NÃO é suportado

As construções abaixo estão **fora do escopo** desta versão do compilador. O uso de qualquer uma delas resultará em erro léxico ou sintático com mensagem informativa.

| Construção | Exemplo | Motivo |
|---|---|---|
| Funções além do `main` | `int soma(int a, int b)` | Sem equivalente direto no Calango dentro do prazo |
| `return` | `return x;` | Sem funções definidas pelo usuário |
| `void` | `void foo()` | Sem funções definidas pelo usuário |
| Arrays | `int v[10];` | Complexidade adicional em todas as fases |
| Cast explícito | `(int) x` | Sem equivalente no Calango |
| Atribuição composta | `x += 1;` | Sem equivalente no Calango; use `x = x + 1` |
| Incremento/decremento | `x++;`, `x--` | Sem equivalente no Calango; use `x = x + 1` |
| Operadores bitwise | `x & y`, `x \| y`, `x ^ y` | Sem equivalente no Calango |
| Operador ternário | `x > 0 ? 1 : 0` | Sem equivalente no Calango |
| Ponteiros | `int *p`, `&x` | Calango não possui ponteiros |

---

## Fases do Compilador

O projeto é desenvolvido incrementalmente ao longo do semestre, seguindo as fases clássicas de um compilador:

| Fase | Descrição | Ferramenta | Status |
|---|---|---|---|
| Análise Léxica | Identificação de tokens | Flex | ✅ Concluída |
| Análise Sintática | Validação da estrutura gramatical | Bison | ✅ Concluída |
| Análise Semântica | Verificação de tipos e escopo | C | 🔄 Em desenvolvimento |
| Geração de Código | Tradução para Calango | C | ⬜ Pendente |

---

## Histórico de Versões

| Versão | Descrição | Data | Responsável |
|---|---|---|---|
| 0.1 | Criação da página de escopo do projeto | 11/04/2026 | [@Luiz Faria](https://github.com/luizfaria1989), [@João Pedro](https://github.com/Jadequilin),  [@Rivaldâvio](https://github.com/RivaFilho) |
