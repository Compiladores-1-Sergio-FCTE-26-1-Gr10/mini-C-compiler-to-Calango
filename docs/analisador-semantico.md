# Analisador Semântico

## Visão Geral

> A análise semântica é a fase do compilador responsável por verificar o significado do programa, garantindo que as construções sintaticamente válidas também façam sentido dentro das regras da linguagem. (Aho et al., 2008, tradução da equipe).

Enquanto o analisador sintático verifica se a *estrutura* do programa está correta, o analisador semântico verifica se o *significado* é válido. Por exemplo, um programa pode ser sintaticamente correto — as palavras estão no lugar certo — mas semanticamente errado, como quando uma variável é usada sem ter sido declarada.

O analisador semântico do Mini C Compiler to Calango foi implementado diretamente no arquivo `parser_AS.y`, integrando as verificações semânticas às ações das regras gramaticais do Bison. Essa abordagem é chamada de **tradução dirigida pela sintaxe**: cada vez que o parser reduz uma regra gramatical, a ação semântica correspondente é executada.

---

## Tabela de Símbolos

O núcleo do analisador semântico é a **tabela de símbolos**, uma estrutura de dados que armazena informações sobre cada variável declarada no programa.

### Estrutura

Cada entrada na tabela contém os seguintes campos:

| Campo | Tipo | Descrição |
|---|---|---|
| `nome` | `char[64]` | Nome da variável (identificador) |
| `tipo` | `TipoDado` | Tipo da variável (`int`, `float`, `char` ou `bool`) |
| `inicializado` | `int` | Indica se a variável já recebeu um valor (`0` = não, `1` = sim) |
| `linha_decl` | `int` | Linha do código-fonte onde a variável foi declarada |

O enumerador `TipoDado` representa os quatro tipos primitivos suportados pelo Mini C:

```c
typedef enum {
    TIPO_INT,
    TIPO_FLOAT,
    TIPO_CHAR,
    TIPO_BOOL,
    TIPO_INVALIDO
} TipoDado;
```

### Operações

A tabela suporta três operações principais:

**Inserção** (`tabela_insere`): chamada ao declarar uma variável. Verifica se o nome já existe na tabela antes de inserir — caso exista, emite erro de redeclaração.

**Busca** (`tabela_busca`): chamada ao usar uma variável em atribuição, expressão ou entrada/saída. Retorna o ponteiro para o símbolo encontrado, ou `NULL` se não existir.

**Impressão** (`tabela_imprime`): chamada ao final da análise, exibe a tabela formatada no terminal com todos os símbolos registrados.

### Saída da tabela ao final da análise

Ao concluir com sucesso, o compilador imprime a tabela de símbolos no terminal:

```
╔═════════════════════════════════════════════════╗
║              TABELA DE SÍMBOLOS                 ║
╠══════════════╦════════════╦═════════════╦═══════╣
║ NOME         ║ TIPO       ║INICIALIZADO ║ LINHA ║
╠══════════════╬════════════╬═════════════╬═══════╣
║ x            ║ int        ║ sim         ║ 2     ║
║ y            ║ int        ║ não         ║ 3     ║
╚══════════════╩════════════╩═════════════╩═══════╝
```

---

## Verificações Implementadas

### Redeclaração de variável

Ao declarar uma variável, o compilador verifica se ela já foi declarada anteriormente no mesmo escopo. Caso positivo, emite um erro semântico indicando a linha da declaração original.

Dado o seguinte trecho de código Mini C:

```c
int main() {
    int x;
    int x; 
}
```

A saída esperada é:

```
ERRO SEMÂNTICO [linha 3]: variável 'x' já foi declarada na linha 2.
```

### Uso de variável não declarada

Em atribuições, expressões, entradas (`scanf`) e na variável de passo do `for`, o compilador verifica se a variável referenciada foi previamente declarada. Caso não tenha sido, emite um erro semântico.

Dado o seguinte trecho de código Mini C:

```c
int main() {
    z = 10; 
}
```

A saída esperada é:

```
ERRO SEMÂNTICO [linha 2]: variável 'z' não declarada.
```

Essa verificação ocorre nos seguintes contextos:

| Contexto | Onde é verificado |
|---|---|
| Atribuição (`ID = expr`) | Regra `atribuicao` |
| Uso em expressão (`ID`) | Regra `expressao` |
| Leitura (`scanf(..., ID)`) | Regra `entrada` |
| Passo do `for` (`ID = expr`) | Regra `atribuicao_for` |

### Inferência de tipo nas expressões

As expressões retornam o tipo inferido como valor semântico, utilizando o mecanismo `%type` do Bison. Isso permite que verificações futuras — como compatibilidade de tipos em atribuições — possam usar essa informação.

A Tabela 01 resume as regras de inferência implementadas.

**Tabela 01:** Regras de inferência de tipo nas expressões.

| Expressão | Tipo retornado |
|---|---|
| `expr + expr`, `expr - expr`, `expr * expr`, `expr / expr` | `float` se algum operando for `float`; caso contrário, `int` |
| `expr % expr` | `int` |
| `expr == expr`, `expr != expr`, `expr < expr`, etc. | `bool` |
| `expr && expr`, `expr \|\| expr` | `bool` |
| `!expr` | `bool` |
| `-expr` (unário) | mesmo tipo de `expr` |
| `(expr)` | mesmo tipo de `expr` |
| `ID` | tipo da variável conforme a tabela de símbolos |
| `LIT_INT` | `int` |
| `LIT_FLOAT` | `float` |
| `LIT_CHAR` | `char` |
| `LIT_STRING` | `string` |
| `true`, `false` | `bool` |

**Autor(es):** [Luiz Faria](https://github.com/luizfaria1989), [João Pedro](https://github.com/Jadequilin),  [Rivaldâvio](https://github.com/RivaFilho).

---

## Tratamento de Erros

Quando o analisador semântico detecta uma violação das regras de significado da linguagem, ele reporta o erro via `fprintf(stderr, ...)` com a posição exata no código-fonte. O formato adotado é:

```
ERRO SEMÂNTICO [linha N]: <descrição do erro>.
```

A análise **não é interrompida** ao encontrar um erro semântico — o compilador continua processando o restante do programa para reportar todos os erros de uma única vez.

---

## Exemplo de Análise

Dado o programa Mini C abaixo:

```c
int main() {
    int x = 10;
    int y;
    y = x + 1;
    printf("resultado: %d", y);
}
```

O analisador semântico executa as seguintes verificações:

1. Declaração de `x` → inserida na tabela com `inicializado = 1`, tipo `int`, linha 2.
2. Declaração de `y` → inserida na tabela com `inicializado = 0`, tipo `int`, linha 3.
3. Atribuição `y = x + 1` → `y` encontrado na tabela ✓; `x` encontrado na tabela ✓; tipo inferido da expressão: `int`. Marca `y` como `inicializado = 1`.
4. `printf` com `y` → `y` encontrado na tabela ✓.

A saída esperada ao final é:

```
Análise concluída: programa válido.

╔═════════════════════════════════════════════════╗
║              TABELA DE SÍMBOLOS                 ║
╠══════════════╦════════════╦═════════════╦═══════╣
║ NOME         ║ TIPO       ║INICIALIZADO ║ LINHA ║
╠══════════════╬════════════╬═════════════╬═══════╣
║ x            ║ int        ║ sim         ║ 2     ║
║ y            ║ int        ║ sim         ║ 3     ║
╚══════════════╩════════════╩═════════════╩═══════╝
```

---

## O que ainda não está implementado

As verificações abaixo estão previstas para as próximas sprints:

- **Compatibilidade de tipos em atribuição:** verificar se o tipo da expressão é compatível com o tipo da variável (ex: atribuir `float`a um `int`).
- **Uso de variável não inicializada:** avisar quando uma variável é lida antes de receber um valor.
- **Verificação de tipo nas condições de laço:** garantir que a expressão de condição em `if`, `while` e `for` seja do tipo `bool`.

---

## Bibliografia

1. AHO, Alfred V. et al. **Compilers: Principles, Techniques, and Tools**. 2ª edição. Pearson, 2008.
2. TREMBLAY, Jean-Paul; SORENSON, Paul G. **The Theory and Practice of Compiler Writing**. 1ª edição. 2008.

---

## Histórico de Versões

| Versão | Descrição | Data | Responsável |
| ------ | --------- | ---- | ----------- |
| `0.1` | Criação da página e documentação inicial do analisador semântico. | 24/04/2026 | [João Pedro](https://github.com/Jadequilin) |
