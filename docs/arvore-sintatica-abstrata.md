# Árvore Sintática Abstrata (AST)

## Visão Geral

> (adicionar definição)

A integração do Bison executa ações durante o parsing de baixo para cima (*bottom-up*). Se a geração de código Calango ocorresse diretamente no parser, o código seria emitido na ordem errada, pois o Calango exige todas as declarações antes das instruções dentro do bloco principal, enquanto o parser processa declarações e instruções intercaladas conforme aparecem no código-fonte.

A AST resolve esse problema: durante o parsing, cada regra da gramática constrói e retorna um nó. Ao final do parsing, obtém-se a árvore completa do programa, permitindo que o gerador de código percorra essa árvore na ordem correta exigida pelo Calango.

---

## Tipos de Nó

A estrutura de nós da árvore é definida por uma enumeração (`enum TipoNo`), que categoriza as diferentes construções do programa. A Tabela 01 apresenta os tipos de nós suportados.

**Tabela 01:** Categorias de nós da AST.

| Categoria | Tipos de nó |
|---|---|
| **Estrutura do programa** | `NO_PROGRAMA`, `NO_BLOCO`, `NO_LISTA_DECL`, `NO_LISTA_INSTR` |
| **Declarações** | `NO_DECL`, `NO_TIPO_INT`, `NO_TIPO_FLOAT`, `NO_TIPO_CHAR`, `NO_TIPO_BOOL` |
| **Instruções** | `NO_ATRIB`, `NO_IF`, `NO_IF_ELSE`, `NO_WHILE`, `NO_FOR`, `NO_DO_WHILE`, `NO_PRINTF`, `NO_SCANF` |
| **Operadores binários** | `NO_OP_MAIS`, `NO_OP_MENOS`, `NO_OP_MULT`, `NO_OP_DIV`, `NO_OP_MOD`, `NO_OP_EQ`, `NO_OP_NEQ`, `NO_OP_LT`, `NO_OP_GT`, `NO_OP_LEQ`, `NO_OP_GEQ`, `NO_OP_AND`, `NO_OP_OR` |
| **Operadores unários** | `NO_OP_NOT`, `NO_OP_NEG` (menos unário) |
| **Folhas (literais)** | `NO_ID`, `NO_LIT_INT`, `NO_LIT_FLOAT`, `NO_LIT_CHAR`, `NO_LIT_STRING`, `NO_LIT_TRUE`, `NO_LIT_FALSE` |
| **Auxiliar** | `NO_ARGS` (lista de argumentos do printf) |

**Autor(es):** [João Pedro](https://github.com/Jadequilin).

---

## Estrutura do Nó

A estrutura `struct No` no arquivo `ast.h` padroniza como os nós são representados na memória.

## Histórico de Versões

| Versão | Descrição | Data | Responsável |
| ------ | --------- | ---- | ----------- |
| `0.1` | Criação da página e documentação inicial da árvore sintática abstrata. | 06/06/2026 | [Luiz Faria](https://github.com/luizfaria1989) |