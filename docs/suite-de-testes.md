# Suíte de Testes

## Visão Geral

Para garantir a confiabilidade do **Mini C Compiler to Calango**, a equipe desenvolveu uma suíte abrangente de testes automatizados. A filosofia de testes adotada baseia-se no **particionamento de equivalência**, onde cada caso de teste representa uma categoria de comportamento distinto ou uma regra de negócio específica da linguagem.

No total, o repositório conta com **17 casos de teste** rigorosos que validam desde a identificação de tokens (Análise Léxica) até a transcrição do código final (Gerador de Código). A validação dos resultados esperados nos testes finais é feita via verificação de *strings* garantidas na saída (usando `grep -qF` com arquivos `.esperado`).

---

## Cobertura Geral do Pipeline

A bateria de testes está dividida entre as quatro fases principais do pipeline do compilador, assegurando que o código rejeite construções inválidas (testes negativos) e processe adequadamente os códigos corretos (testes positivos). A Tabela 01 resume os testes executados.

**Tabela 01:** Resumo de testes completos em todas as fases.

| Fase | Total de Casos | Casos Positivos | Casos Negativos |
|---|---|---|---|
| **Léxico** | 2 | 1 (Todos os tokens válidos reconhecidos) | 1 (`++` e `+=` geram ERRO LÉXICO) |
| **Sintático** | 2 | 1 (Estrutura sintática completa e válida) | 1 (Falta de `;` gera ERRO SINTÁTICO) |
| **Semântico** | 8 | 4 (Programas totalmente válidos) | 3 (Erros: redeclaração, não declarada) + 1 (Aviso: sem init) |
| **Gerador** | 5 | 5 (Verifica a integridade da sintaxe Calango na saída) | N/A (Gerador bloqueado se houver erros prévios) |

**Autor(es):** [João Pedro](https://github.com/Jadequilin).

---

## Testes do Analisador Semântico

A fase semântica possui a maior carga de verificações de regras da linguagem. Foram construídos 8 casos de uso específicos para testar os alertas e interrupções gerados pela manipulação da Tabela de Símbolos. A Tabela 02 detalha esses arquivos.

**Tabela 02:** Detalhamento dos testes da Análise Semântica.

| Arquivo | O que verifica |
|---|---|
| `teste_01_valido.c` | Programa válido com os 4 tipos fundamentais, blocos `if/else`, e `printf`. (Zero erros esperados). |
| `teste_02_while_for.c` | Loops `while` e `for`: valida o controle correto da variável de passo e validação da condição. |
| `teste_03_dowhile.c` | Comando `do-while` aliado ao `scanf`: verifica se a variável é marcada com sucesso como "inicializada" logo após o `leia`. |
| `teste_04_redeclaracao.c` | **ERRO SEMÂNTICO:** Tenta redeclarar propositalmente uma variável no mesmo escopo. |
| `teste_05_nao_declarada.c` | **ERRO SEMÂNTICO:** Força o uso de uma variável fantasma/não declarada em uma expressão. |
| `teste_06_sem_init.c` | **AVISO SEMÂNTICO:** Variável declarada e usada imediatamente antes de receber um primeiro valor. |
| `teste_07_tipos.c` | Testa todos os 4 tipos primitivos e seus respectivos literais (`int`, `float`, `char`, `bool`). |
| `teste_08_expr_logica.c` | Testa a avaliação de expressões compostas e aninhadas envolvendo `&&`, `||` e operadores relacionais. |

**Autor(es):** [João Pedro](https://github.com/Jadequilin).

---

## Testes do Gerador de Código

Os testes do gerador não focam mais em erros no C, mas sim na precisão da tradução da AST para o pseudocódigo Calango válido. Arquivos anexos do tipo `.esperado` contêm a string de validação exigida. A Tabela 03 mapeia os 5 cenários testados.

**Tabela 03:** Detalhamento dos testes de Geração de Código.

| Arquivo | O que verifica (Saída Calango) |
|---|---|
| `teste_01_basico.c` | Verifica a montagem da estrutura mínima obrigatória: `algoritmo MiniC;`, `principal` e `fimPrincipal`. |
| `teste_02_if_else.c` | Geração correta e indentada do bloco de decisão `se(cond)entao ... senao ... fimSe`. |
| `teste_03_while.c` | Geração precisa da repetição `enquanto(cond)faca ... fimEnquanto`. |
| `teste_04_for.c` | Confirma a robustez da tradução do `for` em `enquanto` (inicialização *antes* do laço e passo no *interior* do bloco). |
| `teste_05_scanf.c` | Mapeamento do `scanf` para o comando `leia(var)`, adequação dos tipos `logico` e palavras reservadas `verdadeiro`/`falso`, `e`/`ou`. |

**Autor(es):** [João Pedro](https://github.com/Jadequilin).

---

## Automação com Makefile

Para garantir produtividade e execução reprodutível em ambientes de integração contínua (CI) e máquinas locais, a execução da suíte foi unificada no comando de automação via `Makefile`.

* **`make test`**: Executa ativamente **todos os 17 casos no total**, passando por todas as fases e validando cada arquivo contra a regra estabelecida.

Também é possível isolar os testes na fase sob desenvolvimento ativo usando subcomandos:
* **`make test-lex`**: Executa apenas as validações da fase léxica.
* **`make test-sint`**: Executa apenas as validações da fase sintática.
* **`make test-sem`**: Roda exclusivamente os 8 cenários do interpretador de regras semânticas.
* **`make test-ger`**: Roda os testes de string sobre os arquivos gerados em Calango.

---

## Histórico de Versões

| Versão | Descrição | Data | Responsável |
| ------ | --------- | ---- | ----------- |
| `0.1` | Documentação inicial da suíte de testes (17 casos) e automação via Makefile. | 06/06/2026 | [Luiz Faria](https://github.com/luizfaria1989), [João Pedro](https://github.com/Jadequilin) |