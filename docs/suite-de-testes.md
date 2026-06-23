# Suíte de Testes

## Visão Geral

A suíte de testes valida o comportamento do compilador por fase. Em vez de testar apenas funções internas, os testes executam programas Mini C de entrada e conferem se cada etapa aceita casos válidos, rejeita casos inválidos ou gera o código Calango esperado.

A automação fica concentrada no `Makefile`, que permite executar a suíte completa ou apenas uma etapa específica.

---

## Organização dos Testes

A estrutura esperada da pasta `testes` é:

```text
testes/
├── lexico/
├── sintatico/
├── semantico/
└── gerador/
```

**Tabela 01:** Resumo dos grupos de teste.

| Grupo | Comando | O que valida |
|---|---|---|
| Léxico | `make test-lex` | Reconhecimento de tokens e rejeição de símbolos fora do escopo. |
| Sintático | `make test-sint` | Estruturas gramaticais válidas e erros de sintaxe. |
| Semântico | `make test-sem` | Declaração, redeclaração, uso de variáveis e avisos semânticos. |
| Gerador | `make test-ger` | Tradução básica de Mini C para Calango. |
| Otimizações | `make test-opt` | Redução da AST antes da geração do código final. |

---

## Testes Léxicos

Os testes léxicos verificam se o Flex reconhece corretamente palavras reservadas, identificadores, literais, operadores e delimitadores. Também cobrem símbolos fora do escopo do projeto, como incremento/decremento, atribuição composta e operadores bitwise.

| Arquivo | Finalidade |
|---|---|
| `teste_01_tokens.c` | Valida o reconhecimento dos principais tokens da linguagem. |
| `teste_02_erro_operador.c` | Valida mensagens para operadores fora do escopo. |

---

## Testes Sintáticos

Os testes sintáticos validam a gramática implementada em Bison.

| Arquivo | Finalidade |
|---|---|
| `teste_01_valido.c` | Confirma aceitação de um programa Mini C válido. |
| `teste_02_erro.c` | Confirma rejeição de estrutura sintaticamente inválida. |

---

## Testes Semânticos

A fase semântica concentra validações sobre declaração e uso de variáveis.

| Arquivo | Finalidade |
|---|---|
| `teste_01_valido.c` | Programa semanticamente válido. |
| `teste_02_while_for.c` | Uso de variáveis em laços `while` e `for`. |
| `teste_03_dowhile.c` | Uso de variáveis em `do-while`. |
| `teste_04_redeclaracao.c` | Erro de redeclaração no mesmo escopo. |
| `teste_05_nao_declarada.c` | Erro por uso de variável não declarada. |
| `teste_06_sem_init.c` | Aviso por uso de variável antes da inicialização. |
| `teste_07_tipos.c` | Cobertura de diferentes tipos primitivos aceitos pela linguagem. |
| `teste_08_expr_logica.c` | Expressões relacionais e lógicas. |

---

## Testes do Gerador de Código

Os testes do gerador verificam se o programa Mini C é traduzido para uma saída Calango esperada. Arquivos `.esperado` são usados como referência para comparar trechos da saída.

| Arquivo | O que verifica |
|---|---|
| `teste_01_basico.c` | Estrutura mínima `algoritmo MiniC`, `principal` e `fimPrincipal`. |
| `teste_02_if_else.c` | Tradução de `if/else` para `se...senao...fimSe`. |
| `teste_03_while.c` | Tradução de `while` para `enquanto...fimEnquanto`. |
| `teste_04_for.c` | Conversão de `for` para inicialização + `enquanto` + passo no final do bloco. |
| `teste_05_scanf.c` | Tradução de `scanf` para `leia` e conversão de tipos/literais básicos. |

---

## Testes de Otimização

A PR de código final acrescenta testes específicos para a otimização aplicada à AST antes da emissão do código Calango.

| Arquivo | O que verifica |
|---|---|
| `teste_06_otimizacao_constant_folding.c` | Redução de expressões constantes. |
| `teste_07_otimizacao_simplificacao_algebrica.c` | Simplificação de operações redundantes. |
| `teste_08_otimizacao_logica.c` | Simplificação de expressões booleanas. |
| `teste_09_otimizacao_variavel_morta.c` | Remoção de declarações/atribuições sem uso posterior. |

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
