# Suíte de Testes

## Visão Geral

A suíte de testes valida o comportamento do compilador por fase. Em vez de testar apenas funções internas, os testes executam programas Mini C de entrada e conferem se cada etapa aceita casos válidos, rejeita casos inválidos ou gera o código Calango esperado.

A automação fica concentrada no `Makefile`, que permite executar a suíte completa ou apenas uma etapa específica. Atualmente a suíte conta com **60 testes** distribuídos entre as cinco etapas.

---

## Como os Testes Funcionam

Cada teste é um arquivo `.c` de entrada. O resultado é comparado a arquivos de referência opcionais que ficam ao lado do teste:

| Arquivo | Função |
|---|---|
| `nome.esperado` | Trecho que **deve** aparecer na saída (stdout/stderr) para o teste passar. |
| `nome.nao_esperado` | Trecho que **não pode** aparecer na saída (usado, por exemplo, para confirmar a remoção de variáveis mortas). |

Quando um teste não possui `.esperado`, ele é considerado aprovado se o executável terminar com código de saída `0`.

---

## Organização dos Testes

A estrutura da pasta `testes` é:

```text
testes/
├── lexico/
├── sintatico/
├── semantico/
└── gerador/
```

Os testes do gerador são separados por faixa de numeração: os arquivos `teste_0X_*` cobrem a geração básica e os arquivos `teste_1X_otim_*` cobrem as otimizações.

**Tabela 01:** Resumo dos grupos de teste.

| Grupo | Comando | Qtd. | O que valida |
|---|---|---|---|
| Léxico | `make test-lex` | 12 | Reconhecimento de tokens e rejeição de símbolos fora do escopo. |
| Sintático | `make test-sint` | 13 | Estruturas gramaticais válidas e erros de sintaxe. |
| Semântico | `make test-sem` | 16 | Declaração, redeclaração, uso de variáveis e avisos semânticos. |
| Gerador | `make test-ger` | 9 | Tradução de Mini C para Calango (`teste_0X`). |
| Otimizações | `make test-opt` | 10 | Redução da AST antes da geração do código final (`teste_1X`). |

---

## Testes Léxicos

Os testes léxicos verificam se o Flex reconhece corretamente palavras reservadas, identificadores, literais, operadores e delimitadores. Também cobrem símbolos fora do escopo do projeto, como incremento/decremento, atribuição composta e operadores bitwise.

| Arquivo | Finalidade |
|---|---|
| `teste_01_tokens_basicos.c` | Reconhecimento dos tokens básicos (tipos, identificadores, literais e atribuição). |
| `teste_02_operadores_rel_log.c` | Operadores relacionais e lógicos. |
| `teste_03_literais.c` | Literais `char`, string e booleanos. |
| `teste_04_erro_operador.c` | ERRO: incremento e atribuição composta fora do escopo. |
| `teste_05_comentarios.c` | Comentários de linha e de bloco são descartados. |
| `teste_06_aritmeticos.c` | Operadores aritméticos (`+`, `-`, `*`, `/`, `%`). |
| `teste_07_delimitadores.c` | Delimitadores: vírgula e colchetes. |
| `teste_08_erro_bitwise.c` | ERRO: operadores bitwise (`&`, `\|`, `^`) fora do escopo. |
| `teste_09_erro_desconhecido.c` | ERRO: símbolo desconhecido (`@`). |
| `teste_10_erro_decremento.c` | ERRO: decremento (`--`) fora do escopo. |
| `teste_11_palavras_controle.c` | Palavras-chave de controle de fluxo (`if`, `else`, `while`, `for`, `do`). |
| `teste_12_negacao_e_logicos.c` | Negação lógica (`!`) e operadores `&&` / `\|\|`. |

---

## Testes Sintáticos

Os testes sintáticos validam a gramática implementada em Bison, cobrindo programas válidos e diferentes classes de erro de sintaxe.

| Arquivo | Finalidade |
|---|---|
| `teste_01_valido_basico.c` | Aceitação de programa Mini C válido com `if`. |
| `teste_02_lacos.c` | `while`, `for` e `do-while` válidos. |
| `teste_03_if_else_aninhado.c` | `if/else` aninhado e expressões compostas. |
| `teste_04_io.c` | `scanf` e `printf` com argumentos. |
| `teste_05_erro_pontovirgula.c` | ERRO: falta de ponto e vírgula. |
| `teste_06_erro_parenteses.c` | ERRO: parêntese não fechado no `if`. |
| `teste_07_decl_init.c` | Declarações com inicialização para todos os tipos. |
| `teste_08_expr_aritmetica.c` | Expressões aritméticas com precedência de operadores. |
| `teste_09_unarios.c` | Operadores unários (`-` e `!`). |
| `teste_10_printf_multiargs.c` | `printf` com múltiplos argumentos. |
| `teste_11_erro_chave.c` | ERRO: chave de bloco não fechada. |
| `teste_12_erro_decl_apos_instr.c` | ERRO: declaração após instrução (ordem inválida). |
| `teste_13_erro_falta_expr.c` | ERRO: atribuição sem expressão à direita. |

---

## Testes Semânticos

A fase semântica concentra validações sobre declaração, uso e inicialização de variáveis.

| Arquivo | Finalidade |
|---|---|
| `teste_01_valido.c` | Programa semanticamente válido. |
| `teste_02_lacos.c` | Uso de variáveis em laços `while` e `for`. |
| `teste_03_dowhile.c` | Uso de variáveis em `do-while`. |
| `teste_04_redeclaracao.c` | ERRO: redeclaração no mesmo escopo. |
| `teste_05_nao_declarada.c` | ERRO: uso de variável não declarada. |
| `teste_06_sem_init.c` | AVISO: uso de variável antes da inicialização. |
| `teste_07_tipos.c` | Cobertura de todos os tipos primitivos aceitos. |
| `teste_08_expr_logica.c` | Expressões relacionais e lógicas. |
| `teste_09_init_na_decl.c` | Inicialização na declaração suprime o aviso de uso sem inicialização. |
| `teste_10_nao_declarada_expr.c` | ERRO: variável não declarada usada em expressão. |
| `teste_11_redeclaracao_tipos.c` | ERRO: redeclaração da variável com tipo diferente. |
| `teste_12_scanf_nao_declarada.c` | ERRO: `scanf` sobre variável não declarada. |
| `teste_13_aviso_relacional.c` | AVISO: uso sem inicialização dentro de uma condição. |
| `teste_14_multiplas_decl.c` | Múltiplas declarações e usos válidos. |
| `teste_15_for_passo_nao_declarada.c` | ERRO: variável não declarada no passo do `for`. |
| `teste_16_controle_aninhado.c` | Estruturas de controle aninhadas válidas. |

---

## Testes do Gerador de Código

Os testes do gerador verificam se o programa Mini C é traduzido para a saída Calango esperada. Os arquivos `.esperado` servem de referência para comparar trechos da saída gerada.

| Arquivo | O que verifica |
|---|---|
| `teste_01_basico.c` | Estrutura mínima `algoritmo MiniC`, `principal` e `fimPrincipal`. |
| `teste_02_if_else.c` | Tradução de `if/else` para `se...senao...fimSe`. |
| `teste_03_while.c` | Tradução de `while` para `enquanto...fimEnquanto`. |
| `teste_04_for.c` | Conversão de `for` para inicialização + `enquanto` + passo no fim do bloco. |
| `teste_05_scanf.c` | Tradução de `scanf` para `leia`. |
| `teste_06_dowhile.c` | Tradução de `do-while` para `faca...enquanto`. |
| `teste_07_tipos.c` | Tradução de tipos `char`/`float` para `caracter`/`real`. |
| `teste_08_aritmetica.c` | Expressão aritmética com precedência e parêntese explícito. |
| `teste_09_logico_relacional.c` | Expressão lógica e relacional (`&&` → `e`). |

---

## Testes de Otimização

Os testes de otimização validam as transformações aplicadas à AST antes da emissão do código Calango.

| Arquivo | O que verifica |
|---|---|
| `teste_10_otim_constant_folding.c` | Dobra de expressão constante aritmética. |
| `teste_11_otim_simplificacao_algebrica.c` | Simplificação de `(x + 0) * 1` para `x`. |
| `teste_12_otim_logica.c` | Simplificação de expressões booleanas constantes. |
| `teste_13_otim_variavel_morta.c` | Remoção de declaração/atribuição sem uso posterior. |
| `teste_14_otim_subtracao_zero.c` | Simplificação algébrica `x - 0` para `x`. |
| `teste_15_otim_divisao_um.c` | Simplificação algébrica `x / 1` para `x`. |
| `teste_16_otim_mult_zero.c` | Simplificação algébrica `x * 0` para `0`. |
| `teste_17_otim_fold_relacional.c` | Dobra de constante relacional (`2 < 3` → `verdadeiro`). |
| `teste_18_otim_fold_subtracao.c` | Dobra de constante aritmética (`10 - 4` → `6`). |
| `teste_19_otim_not_constante.c` | Dobra de negação constante (`!false` → `verdadeiro`). |

---

## Execução

Para rodar todos os testes:

```bash
make test
```

Para rodar apenas uma etapa:

```bash
make test-lex
make test-sint
make test-sem
make test-ger
make test-opt
```

---

## Histórico de Versões

| Versão | Descrição | Data | Responsável |
|---|---|---|---|
| `0.1` | Documentação inicial da suíte de testes. | 06/06/2026 | [Luiz Faria](https://github.com/luizfaria1989), [João Pedro](https://github.com/Jadequilin) |
| `0.2` | Inclusão dos testes do gerador e dos testes de otimização. | 19/06/2026 | [Pedro Silva](https://github.com/314dro), [João Pedro](https://github.com/Jadequilin) |
| `0.3` | Página convertida em documento próprio da suíte; ampliação da suíte para 60 testes (léxico, sintático, semântico, gerador e otimizações). | 23/06/2026 | [Luiz Faria](https://github.com/luizfaria1989) |
</content>
