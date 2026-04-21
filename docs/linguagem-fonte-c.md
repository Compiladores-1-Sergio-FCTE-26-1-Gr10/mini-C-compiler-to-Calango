# Linguagem-Fonte Mini C

## Objetivo

Esta página descreve a **sintaxe** e a **semântica básica** da linguagem-fonte adotada neste projeto, chamada **Mini C**.

A Mini C é um subconjunto da linguagem C definido especificamente para este compilador, cujo objetivo é traduzir programas escritos nessa linguagem para **Calango**.

> Esta página complementa a documentação de [Escopo do Projeto](escopo.md).
> As construções suportadas e não suportadas estão detalhadas em `escopo.md`.
> A gramática detalhada pode ser consultada em [Analisador Sintático](analisador-sintatico.md).

---

## Visão Geral

A Mini C adotada neste projeto possui as seguintes características gerais:

- tipagem estática;
- tipos primitivos `int`, `float`, `char` e `bool`;
- programa composto por uma única função principal `main`;
- declarações de variáveis no início do bloco principal;
- comandos de atribuição, seleção, repetição e entrada/saída;
- expressões aritméticas, relacionais e lógicas.

A estrutura geral de um programa válido é:

```c
int main() {
    /* declarações */
    /* comandos */
}
```

Essa definição **não corresponde à linguagem C completa**, mas a uma versão reduzida delimitada pelo escopo do projeto.

---

## Sintaxe Básica

### 1. Estrutura do programa

Todo programa Mini C deve possuir exatamente uma função principal com a seguinte forma:

```c
int main() {
    /* declarações */
    /* comandos */
}
```

Nesta linguagem, as declarações devem aparecer antes dos comandos.

---

### 2. Elementos léxicos principais

A linguagem utiliza, entre outros, os seguintes grupos de tokens:

- **palavras-chave**: `int`, `float`, `char`, `bool`, `main`, `if`, `else`, `while`, `for`, `do`, `printf`, `scanf`, `true`, `false`;
- **identificadores**: nomes de variáveis;
- **literais**: inteiros, reais, caracteres, strings e booleanos;
- **operadores**: aritméticos, relacionais, lógicos e atribuição simples;
- **delimitadores**: `(`, `)`, `{`, `}`, `;`, `,`.

Exemplos de identificadores válidos:

```c
x
contador
_media
valorFinal2
```

Exemplos de literais válidos:

```c
10
3.14
'a'
"texto"
true
false
```

---

### 3. Declarações

As variáveis podem ser declaradas de forma simples ou com inicialização.

#### Declaração simples

```c
int x;
float media;
char letra;
bool ativo;
```

#### Declaração com inicialização

```c
int x = 10;
float media = 7.5;
char letra = 'a';
bool ativo = true;
```

---

### 4. Comandos básicos

A Mini C aceita os seguintes grupos de comandos.

#### Atribuição

```c
x = 10;
media = media + 1.0;
ativo = false;
```

#### Condicional

```c
if (x > 0) {
    printf("positivo");
}

if (x > 0) {
    printf("positivo");
} else {
    printf("nao positivo");
}
```

#### Repetição

```c
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

#### Entrada e saída

```c
printf("ola");
printf("valor: %d", x);
scanf("%d", x);
```

---

### 5. Expressões

As expressões da Mini C podem ser aritméticas, relacionais ou lógicas.

#### Expressões aritméticas

```c
x + y
a - b
n * 2
total / 3
valor % 2
```

#### Expressões relacionais

```c
x == y
x != y
x < y
x <= y
x > y
x >= y
```

#### Expressões lógicas

```c
a && b
a || b
!a
```

As expressões podem ser combinadas e agrupadas com parênteses.

Exemplo:

```c
(x > 0) && (ativo || aprovado)
```

---

### 6. Forma sintática geral

De forma simplificada, a linguagem pode ser descrita pela seguinte estrutura:

```bnf
<programa> ::= "int" "main" "(" ")" "{" <declaracoes> <comandos> "}"

<declaracoes> ::= <declaracao> <declaracoes> | ε
<comandos> ::= <comando> <comandos> | ε

<declaracao> ::= <tipo> ID ";"
               | <tipo> ID "=" <expressao> ";"

<tipo> ::= "int" | "float" | "char" | "bool"

<comando> ::= <atribuicao>
            | <if>
            | <while>
            | <for>
            | <do_while>
            | <printf>
            | <scanf>

<atribuicao> ::= ID "=" <expressao> ";"
```

Essa representação é apenas uma visão simplificada da linguagem. A gramática formal detalhada está documentada na página do analisador sintático.

---

## Semântica Básica

A semântica básica define as regras de significado da linguagem, isto é, as condições para que um programa seja considerado válido não apenas sintaticamente, mas também semanticamente.

### 1. Regras de declaração e uso

A Mini C adota as seguintes regras:

- toda variável deve ser declarada antes do uso;
- não pode haver redeclaração da mesma variável no mesmo escopo;
- as declarações devem ocorrer no início do bloco principal;
- o programa possui uma única função `main`.

Exemplo válido:

```c
int main() {
    int x;
    x = 10;
}
```

Exemplo inválido:

```c
int main() {
    x = 10;
}
```

No segundo caso, `x` foi usada sem declaração prévia.

---

### 2. Regras de tipos

A linguagem possui quatro tipos primitivos:

- `int`: números inteiros;
- `float`: números reais;
- `char`: caracteres;
- `bool`: valores lógicos.

Como a linguagem é estaticamente tipada, toda variável possui um tipo definido em tempo de compilação, e esse tipo deve ser respeitado nas atribuições e expressões.

---

### 3. Regras para atribuição

Uma atribuição só é válida quando o tipo da expressão do lado direito é compatível com o tipo da variável do lado esquerdo.

Exemplos válidos:

```c
int x = 10;
float y = 3.14;
char c = 'a';
bool ok = true;

x = 20;
y = x;
```

Exemplos inválidos:

```c
int x = 3.14;
char c = 10;
bool ok = 1;
```

Neste projeto, adota-se a seguinte política semântica:

- atribuições entre elementos do mesmo tipo são válidas;
- `int` pode ser promovido para `float`;
- `float` não deve ser implicitamente convertido para `int`;
- `char` deve receber caractere;
- `bool` deve receber valor booleano.

---

### 4. Regras para expressões

#### Expressões aritméticas

As operações aritméticas exigem operandos numéricos.

```c
+
-
*
/
%
```

Regras básicas:

- `+`, `-`, `*` e `/` podem ser usados com `int` e `float`;
- `%` deve ser usado apenas com operandos inteiros;
- valores booleanos não devem participar de operações aritméticas.

#### Expressões relacionais

Operadores relacionais produzem resultado lógico.

```c
==
!=
<
>
<=
>=
```

Regras básicas:

- comparações resultam em valor booleano;
- comparações relacionais exigem operandos compatíveis;
- `==` e `!=` também exigem compatibilidade entre os tipos comparados.

#### Expressões lógicas

Operadores lógicos atuam sobre valores booleanos.

```c
&&
||
!
```

Regras básicas:

- `&&` e `||` exigem operandos booleanos;
- `!` exige operando booleano;
- o resultado é sempre booleano.

---

### 5. Regras para condições

As expressões usadas em estruturas de controle devem produzir valor booleano.

Exemplo válido:

```c
bool ativo = true;

if (ativo) {
    printf("ok");
}
```

Exemplo válido:

```c
int x = 10;

if (x > 0) {
    printf("positivo");
}
```

Exemplo inválido:

```c
int x = 10;

if (x + 1) {
    printf("erro");
}
```

Nesse caso, a condição não é booleana.

---

### 6. Regras para entrada e saída

As operações de entrada e saída devem respeitar os tipos envolvidos.
	
#### `printf`

O primeiro argumento deve ser uma string literal. Os argumentos seguintes devem ser compatíveis com os especificadores de formato usados na string.

Exemplo:

```c
printf("x = %d", x);
printf("media = %f", media);
printf("letra = %c", letra);
```

#### `scanf`

O `scanf` recebe uma string de formato e um identificador previamente declarado.

Exemplo:

```c
scanf("%d", x);
scanf("%f", media);
scanf("%c", letra);
```

---

### 7. Erros semânticos esperados

Entre os principais erros semânticos tratados pelo compilador, destacam-se:

- uso de variável não declarada;
- redeclaração de variável;
- atribuição entre tipos incompatíveis;
- operação com tipos incompatíveis;
- uso de expressão não booleana em condição;
- uso de construção fora do escopo definido para a linguagem.

---

## Exemplos

### Exemplo semanticamente válido

```c
int main() {
    int x = 10;
    float media = 7.5;
    bool aprovado = true;

    if ((x > 0) && aprovado) {
        printf("x = %d", x);
    }

    while (x < 15) {
        x = x + 1;
    }
}
```

### Exemplo semanticamente inválido

```c
int main() {
    int x;
    x = 3.14;

    if (x + 1) {
        printf("erro");
    }
}
```

Esse programa é inválido porque:

- `x` é do tipo `int`, mas recebe uma expressão do tipo `float`;
- a condição do `if` não produz valor booleano.

---

## Relação com TDD

Como o desenvolvimento do compilador é orientado por **TDD (Test-Driven Development)**, as regras sintáticas e semânticas descritas nesta página servem também como base para a definição dos testes do projeto.

Para cada construção aceita ou rejeitada pela linguagem, devem ser criados testes que validem o comportamento esperado do compilador, incluindo:

- reconhecimento de programas válidos;
- rejeição de programas sintaticamente inválidos;
- rejeição de programas semanticamente inválidos;
- verificação das mensagens de erro produzidas;
- validação da tradução correta para Calango, quando aplicável.

Dessa forma, a especificação da linguagem não é apenas descritiva, mas também orienta os critérios de aceitação implementados na suíte de testes.

---

## Considerações Finais

A definição da linguagem-fonte estabelece a base para as demais fases do compilador.

A partir dela, tornam-se possíveis:

- a especificação dos tokens da linguagem;
- a construção da gramática do analisador sintático;
- a implementação das verificações semânticas;
- a tradução das construções aceitas para Calango.

Esta página deve ser lida em conjunto com:

- `escopo.md`, para o escopo funcional da linguagem;
- `analisador-lexico.md`, para os tokens e padrões léxicos;
- `analisador-sintatico.md`, para a gramática detalhada;
- `decisoes-tecnicas.md`, para as decisões de implementação.

---

## Histórico de Versões

| Versão | Descrição | Data | Responsável |
|---|---|---|---|
| 0.1 | Criação da página de linguagem-fonte Mini C | 21/04/2026 | @314dro |
