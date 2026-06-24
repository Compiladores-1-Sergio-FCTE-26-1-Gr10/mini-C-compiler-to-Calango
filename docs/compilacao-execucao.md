# Compilação, Execução Manual e Suíte de Testes

## Visão Geral

Este documento apresenta os comandos necessários para compilar, executar manualmente e testar o Mini C Compiler to Calango.

O compilador traduz um subconjunto da linguagem Mini C para código equivalente em Calango, passando pelas etapas de análise léxica, análise sintática, análise semântica e geração de código.

---

## Requisitos

Antes de compilar o projeto, instale as dependências:

```bash
sudo apt-get update
sudo apt-get install -y build-essential flex bison libfl-dev
```

| Dependência | Finalidade                                                         |
| ----------- | ------------------------------------------------------------------ |
| `gcc`       | Compilar os arquivos C do projeto.                                 |
| `make`      | Automatizar a compilação e execução dos testes.                    |
| `flex`      | Gerar o analisador léxico.                                         |
| `bison`     | Gerar o analisador sintático.                                      |
| `libfl-dev` | Disponibilizar a biblioteca `libfl`, usada na linkagem com `-lfl`. |

---

## Compilação do Compilador

Compilar todas as etapas:

```bash
make all
```

Executáveis gerados:

```text
./minic_lexico
./minic_sintatico
./minic_semantico
./minic_gerador
```

Compilar cada etapa separadamente:

```bash
make lexico
make sintatico
make semantico
make gerador
```

---

## Limpeza dos Arquivos Gerados

```bash
make clean
```

Fluxo recomendado para recompilar do zero:

```bash
make clean
make all
```

---

## Execução Manual do Compilador

Compilar o gerador:

```bash
make gerador
```

Executar o compilador com um arquivo Mini C:

```bash
./minic_gerador caminho/do/arquivo.c
```

A saída será exibida diretamente no terminal em formato Calango.

---

## Exemplo de Execução Manual

Arquivo `manual_otimizacao.c`:

```c
int main() {
    int resultado;
    int morta;

    resultado = (2 + 3) * 4;
    morta = 99;

    printf("%d\n", resultado);
}
```

Execução:

```bash
./minic_gerador manual_otimizacao.c
```

Saída esperada:

```text
algoritmo MiniC;
principal
   inteiro resultado;
   resultado = 20;
   escreval(resultado);
fimPrincipal
```

Esse exemplo valida:

| Comportamento                     | Resultado esperado                             |
| --------------------------------- | ---------------------------------------------- |
| Otimização de expressão constante | `(2 + 3) * 4` é reduzido para `20`.            |
| Remoção de variável morta         | A variável `morta` não aparece na saída final. |

---

## Suíte de Testes Automatizados

A suíte de testes fica organizada na pasta:

```text
testes/
```

Estrutura esperada:

```text
testes/
├── lexico/
├── sintatico/
├── semantico/
└── gerador/
```

---

## Executar Todos os Testes

```bash
make test
```

Esse comando executa:

| Etapa       | Descrição                                                      |
| ----------- | -------------------------------------------------------------- |
| Léxico      | Valida reconhecimento de tokens e erros léxicos.               |
| Sintático   | Valida estruturas gramaticais válidas e inválidas.             |
| Semântico   | Valida escopo, tipos, inicialização e uso de variáveis.        |
| Gerador     | Valida a geração de código Calango.                            |
| Otimizações | Valida otimizações aplicadas antes da emissão do código final. |

---

## Executar Testes por Etapa

```bash
make test-lex
make test-sint
make test-sem
make test-ger
make test-opt
```

| Comando          | O que executa                        |
| ---------------- | ------------------------------------ |
| `make test-lex`  | Testes do analisador léxico.         |
| `make test-sint` | Testes do analisador sintático.      |
| `make test-sem`  | Testes do analisador semântico.      |
| `make test-ger`  | Testes básicos do gerador de código. |
| `make test-opt`  | Testes de otimização do gerador.     |

O alvo `make test-opt` existe para facilitar a validação isolada das otimizações, mas esses testes também são executados pelo comando principal:

```bash
make test
```

---

## Histórico de Versões

| Versão | Descrição                                                                                             | Data       | Responsável                              |
| ------ | ----------------------------------------------------------------------------------------------------- | ---------- | ---------------------------------------- |
| `0.1`  | Criação da documentação de compilação, execução manual e suíte de testes para publicação no GitPages. | 19/06/2026 | [Pedro Silva](https://github.com/314dro) |
