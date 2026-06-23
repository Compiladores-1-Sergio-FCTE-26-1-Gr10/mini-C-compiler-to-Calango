# Árvore Sintática Abstrata (AST)

## Visão Geral

A Árvore Sintática Abstrata, ou AST (*Abstract Syntax Tree*), é uma representação hierárquica do programa após a análise sintática. Diferente da árvore sintática completa, a AST remove detalhes puramente gramaticais e mantém apenas as construções necessárias para as próximas fases do compilador.

No Mini C Compiler to Calango, a AST é usada para organizar declarações, instruções e expressões antes da geração de código. Essa estrutura também permite aplicar otimizações antes da emissão final em Calango.

---

## Papel da AST no Pipeline

A integração com o Bison executa ações durante o parsing. Se o código Calango fosse emitido diretamente no parser, a saída poderia ficar difícil de reorganizar e otimizar. Com a AST, o compilador primeiro constrói a representação completa do programa e depois percorre essa árvore na ordem adequada.

O fluxo adotado é:

```text
Código Mini C
   ↓
Tokens do analisador léxico
   ↓
Regras do parser Bison
   ↓
AST
   ↓
Análise/otimização da árvore
   ↓
Código Calango
```

---

## Tipos de Nó

A estrutura de nós é definida por uma enumeração `TipoNo`, que categoriza as construções da linguagem.

**Tabela 01:** Categorias de nós da AST.

| Categoria | Tipos de nó |
|---|---|
| Estrutura do programa | `NO_PROGRAMA`, `NO_BLOCO`, `NO_LISTA_DECL`, `NO_LISTA_INSTR` |
| Declarações | `NO_DECL`, `NO_TIPO_INT`, `NO_TIPO_FLOAT`, `NO_TIPO_CHAR`, `NO_TIPO_BOOL` |
| Instruções | `NO_ATRIB`, `NO_IF`, `NO_IF_ELSE`, `NO_WHILE`, `NO_FOR`, `NO_DO_WHILE`, `NO_PRINTF`, `NO_SCANF` |
| Operadores binários | `NO_OP_MAIS`, `NO_OP_MENOS`, `NO_OP_MULT`, `NO_OP_DIV`, `NO_OP_MOD`, `NO_OP_EQ`, `NO_OP_NEQ`, `NO_OP_LT`, `NO_OP_GT`, `NO_OP_LEQ`, `NO_OP_GEQ`, `NO_OP_AND`, `NO_OP_OR` |
| Operadores unários | `NO_OP_NOT`, `NO_OP_NEG` |
| Folhas | `NO_ID`, `NO_LIT_INT`, `NO_LIT_FLOAT`, `NO_LIT_CHAR`, `NO_LIT_STRING`, `NO_LIT_TRUE`, `NO_LIT_FALSE` |
| Auxiliar | `NO_ARGS`, usado para lista de argumentos do `printf` |

---

## Estrutura do Nó

Cada nó da AST armazena seu tipo, linha de origem, valor opcional e ponteiros para filhos.

**Tabela 02:** Campos principais da estrutura `No`.

| Campo | Função |
|---|---|
| `tipo` | Indica a categoria do nó. |
| `linha` | Registra a linha do código-fonte associada ao nó. |
| `val` | Armazena valor de literal ou identificador, quando aplicável. |
| `filho[4]` | Vetor fixo de filhos usado para representar subestruturas. |
| `n_filhos` | Quantidade de filhos ocupados. |
| `prox` | Encadeia nós que pertencem à mesma lista de declarações ou instruções. |

O uso de `prox` evita criar nós intermediários apenas para listas, simplificando a travessia do gerador.

---

## Funções Principais

| Função | Responsabilidade |
|---|---|
| `ast_no` | Cria um nó genérico. |
| `ast_filho` | Adiciona um filho ao nó. |
| `ast_int`, `ast_float`, `ast_char`, `ast_str` | Criam nós de literais e identificadores. |
| `ast_imprime` | Exibe a árvore para depuração. |
| `ast_libera` | Libera recursivamente a memória da árvore. |
| `otimizarAST` | Aplica simplificações e remoções antes da geração de código. |

---

## Histórico de Versões

| Versão | Descrição | Data | Responsável |
|---|---|---|---|
| `0.1` | Criação da página e documentação inicial da árvore sintática abstrata. | 06/06/2026 | [Luiz Faria](https://github.com/luizfaria1989) |
| `0.2` | Inclusão da estrutura de nós, funções principais e relação com otimizações. | 19/06/2026 | [João Pedro](https://github.com/Jadequilin), [Pedro Silva](https://github.com/314dro) |
