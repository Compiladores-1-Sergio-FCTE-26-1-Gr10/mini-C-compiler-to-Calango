# Mini C Compiler to Calango

![Status](https://img.shields.io/badge/Status-Em%20Desenvolvimento-orange)
![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)
![Markdown](https://img.shields.io/badge/markdown-%23000000.svg?style=for-the-badge&logo=markdown&logoColor=white)

Este é o repositório do grupo 10 da disciplina de Compiladores sob a orientação do Prof. Sérgio Freitas (UnB - Gama). Neste repositório você encontrará o desenvolvimento de um compilador de mini c - uma variante mais restrita da linguagem de programação C - para Calango - a qual é utilizada na disciplina de Algoritmos e Programação de Computadores. O projeto conta com os códigos produzidos ao longo da disciplina pelos membros do grupo e também a documentação das etapas de produção do compilador, ela pode ser acessada pelo Git Pages do repositório.

Acesse a documentação completa do projeto [aqui](https://compiladores-1-sergio-fcte-26-1-gr10.github.io/mini-C-compiler-to-Calango/)

## Grupo 10 - Compiladores 1 - 2026.1

<center>
<table style="margin-left: auto; margin-right: auto;">
    <tr>
        <td align="center">
            <a href="https://github.com/Jadequilin">
                <img style="border-radius: 50%;" src="https://github.com/Jadequilin.png" width="150px;"/>
                <h5 class="text-center">João Pedro Araujo de Freitas Lyra</h5>
            </a>
        </td>
        <td align="center">
            <a href="https://github.com/luizfaria1989">
                <img style="border-radius: 50%;" src="https://github.com/luizfaria1989.png" width="150px;"/>
                <h5 class="text-center">Luiz Guilherme Morais da Costa Faria</h5>
            </a>
        </td>
        <td align="center">
            <a href="https://github.com/MVConsorte">
                <img style="border-radius: 50%;" src="https://github.com/MVConsorte.png" width="150px;"/>
                <h5 class="text-center">Mateus Villela Consorte</h5>
            </a>
        </td>
    </tr>
    <tr>
        <td align="center">
            <a href="https://github.com/314dro">
                <img style="border-radius: 50%;" src="https://github.com/314dro.png" width="150px;"/>
                <h5 class="text-center">Pedro Henrique Martins Silva</h5>
            </a>
        </td>
        <td align="center">
            <a href="https://github.com/RivaFilho">
                <img style="border-radius: 50%;" src="https://github.com/RivaFilho.png" width="150px;"/>
                <h5 class="text-center">Rivaldavio Joaquim da Silva Filho</h5>
            </a>
        </td>
        <td></td>
    </tr>
</table>
</center>

---

## Histórico de Versões

| Versão       | Descrição | Data                                           | Autor                                       |
| --------- | ----- | --------------------------------------------------- | -------------------------------------------- |
| 1.0 |  Criação do readme inicial.   |  12/04/2026 | [@Luiz](https://github.com/luizfaria1989) |
| 1.1 |  Adição de guia do compilador.   |  19/06/2026 | [@Pedro](https://github.com/314dro) |


# Guia Mini C Compiler to Calango

Guia rápido para compilar, executar e testar o compilador Mini C para Calango.

## Requisitos

Instale as dependências necessárias:

```bash
sudo apt-get update
sudo apt-get install -y build-essential flex bison
```

## Compilação

Para compilar todas as etapas do compilador:

```bash
make all
```

Também é possível compilar cada etapa separadamente:

```bash
make lexico
make sintatico
make semantico
make gerador
```

Executáveis gerados:

```text
./minic_lexico
./minic_sintatico
./minic_semantico
./minic_gerador
```

## Limpeza

Para remover executáveis e arquivos intermediários gerados pelo `flex` e pelo `bison`:

```bash
make clean
```

## Execução manual

Para gerar código Calango a partir de um arquivo Mini C:

```bash
make gerador
./minic_gerador caminho/do/arquivo.c
```

A saída será impressa no terminal em formato Calango.

## Exemplo de testes manual

### Exemplo 1 — constant folding e variável morta

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

O teste valida que `(2 + 3) * 4` foi reduzido para `20` e que a variável `morta` foi removida da saída.


## Testes automatizados

Para rodar toda a suíte de testes:

```bash
make test
```

Esse comando executa os testes:

```text
léxicos
sintáticos
semânticos
gerador
otimizações do gerador
```

Para rodar uma etapa específica:

```bash
make test-lex
make test-sint
make test-sem
make test-ger
make test-ger
```