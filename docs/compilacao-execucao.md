# Compilação, Execução e Testes

## Visão Geral

Esta página reúne os comandos necessários para compilar, executar manualmente e testar o **Mini C Compiler to Calango**.

O projeto está organizado por fase do compilador. A execução completa passa por análise léxica, análise sintática, análise semântica, construção/otimização da AST e geração de código Calango.

---

## Requisitos

Antes de compilar o projeto em um ambiente Linux ou WSL/Ubuntu, instale as dependências:

```bash
sudo apt-get update
sudo apt-get install -y build-essential flex bison libfl-dev
```

| Dependência | Finalidade |
|---|---|
| `gcc` | Compilar os arquivos C do projeto. |
| `make` | Automatizar a compilação e a execução dos testes. |
| `flex` | Gerar os analisadores léxicos a partir dos arquivos `.l`. |
| `bison` | Gerar os analisadores sintáticos a partir dos arquivos `.y`. |
| `libfl-dev` | Disponibilizar a biblioteca `libfl`, usada na linkagem do analisador léxico. |

---

## Compilação

Para compilar todas as fases:

```bash
make all
```

Também é possível compilar cada fase separadamente:

```bash
make lexico
make sintatico
make semantico
make gerador
```

Os executáveis esperados são:

```text
./minic_lexico
./minic_sintatico
./minic_semantico
./minic_gerador
```

Para limpar executáveis e arquivos intermediários gerados pelo Flex/Bison:

```bash
make clean
```

Fluxo recomendado para recompilar do zero:

```bash
make clean
make all
```

---

## Execução Manual

Para gerar código Calango a partir de um arquivo Mini C:

```bash
make gerador
./minic_gerador caminho/do/arquivo.c
```

A saída é impressa no terminal em formato Calango.

### Exemplo

Arquivo `manual_otimizacao.c`:

```c
int main() {
    int resultado;
    int morta;

    resultado = (2 + 3) * 4;
    morta = 99;

    printf("%d
", resultado);
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

Esse exemplo cobre duas etapas do pipeline final: a expressão constante `(2 + 3) * 4` é reduzida para `20`, e a variável `morta` é removida por não influenciar a saída do programa.

---

## Suíte de Testes

Para executar toda a suíte:

```bash
make test
```

O comando principal chama os testes por etapa:

| Comando | Etapa testada |
|---|---|
| `make test-lex` | Analisador léxico. |
| `make test-sint` | Analisador sintático. |
| `make test-sem` | Analisador semântico. |
| `make test-ger` | Geração básica de código Calango. |
| `make test-opt` | Otimizações aplicadas antes da geração final. |

Para validar somente as otimizações do gerador:

```bash
make test-opt
```

---

## Histórico de Versões

| Versão | Descrição | Data | Responsável |
|---|---|---|---|
| `0.1` | Criação da página de compilação, execução manual e testes. | 19/06/2026 | [Pedro Silva](https://github.com/314dro), [João Pedro](https://github.com/Jadequilin) |
