# Abordagem de Desenvolvimento: TDD Adaptada para Dev. de Compiladores

## Introdução

O Desenvolvimento Guiado por Testes (do inglês, *Test-Driven Development* - TDD) é uma prática de engenharia de software que inverte a lógica tradicional de programação. Em vez de escrever o código da aplicação e depois testá-lo, o desenvolvedor escreve o teste automatizado antes mesmo da funcionalidade existir. Segundo Beck (2002, p. 9), o TDD baseia-se em uma premissa simples, onde "o código de teste é escrito antes do código de produção", garantindo que todo novo comportamento seja verificado desde a sua concepção.

O TDD tradicional é regido pelo ciclo de feedback ultra-curto conhecido como **Red-Green-Refactor**:

1. **Red (Vermelho):** Escrever um teste que falha (pois a funcionalidade ainda não existe).
2. **Green (Verde):** Escrever o código mínimo e mais simples possível para fazer o teste passar.
3. **Refactor (Azul):** Melhorar a qualidade do código (limpeza, padrões, otimização) com a segurança de que o teste continuará passando.

### Objetivo e Motivação da Abordagem TDD Adaptada

Construir um compilador (ou, neste caso, um transpilador de Mini-C para Calango) apresenta desafios arquiteturais únicos. A geração de analisadores léxicos e sintáticos através de ferramentas como Flex e Bison produz código C gerado automaticamente. Aplicar o TDD clássico (testes unitários em funções internas) a códigos gerados por máquinas é ineficiente e frágil. 

Além disso, gramáticas livres de contexto são altamente acopladas. Como alertam Aho et al. (2007, p. 201), "a adição de uma nova regra de produção em um analisador sintático pode introduzir ambiguidades não intencionais", resultando nos temidos conflitos de *Shift/Reduce*. 

Portanto, a motivação para a **Abordagem TDD Adaptada** é criar uma rede de segurança contra regressões sistêmicas durante a evolução da gramática da linguagem Mini-C, focando no comportamento externo (Caixa Preta) da tradução, e não na estrutura interna das funções em C, garantindo que o mapeamento semântico para a linguagem Calango ocorra de forma determinística e incremental.

# Metodologia

A aplicação do TDD no contexto deste projeto exigiu uma reavaliação dos princípios originais para se adequar ao pipeline do Flex/Bison.

**Princípios Mantidos:**

* **Test-First:** Nenhum token ou regra gramatical é adicionado ao `.l` ou `.y` sem que exista um caso de uso documentado em um teste falho.
* **Prevenção de Regressão:** A suíte de testes atua como a documentação viva e o contrato de funcionamento da gramática.
* **Ciclo Incremental:** A linguagem será construída estrutura por estrutura (ex: primeiro variáveis, depois operações aritméticas, depois laços de repetição).

**Princípios Ajustados:**

* **Mudança de Granularidade (De Unitário para Integração):** Em vez de testar funções C isoladas (`yylex()` ou `yyparse()`), os testes são de integração fundamentados em arquivos físicos. Avalia-se a entrada de um arquivo `.c` válido e a saída gerada em um arquivo `.cal`.
* **Caixa Preta do Analisador:** A estrutura interna da Árvore Sintática Abstrata (AST) não é acoplada aos testes, permitindo refatorações estruturais profundas no compilador sem quebrar a suíte de validação, desde que o código final em Calango permaneça correto.

### O Novo Ciclo (Passo a Passo)

O fluxo de trabalho durante o desenvolvimento do compilador seguirá a seguinte esteira:

1. **Especificação (Red):** Cria-se um arquivo de entrada em Mini-C (ex: `teste_soma.c`) com uma construção de linguagem específica, e um arquivo de gabarito com a saída esperada (ex: `esperado_soma.cal`).
2. **Execução Inicial:** Roda-se o *script* de automação de testes. O teste falhará apontando erro léxico/sintático, pois o compilador ainda não reconhece a construção.
3. **Implementação (Código de Produção):**

    * Adição de expressões regulares no analisador léxico (`.l`).
    * Definição de regras de produção e construção da AST no analisador sintático (`.y`).
    * Atualização do módulo de percurso da árvore (*Tree Walker*) para emitir o código em Calango.
4. **Validação (Green):** Executa-se a suíte de testes novamente. O *script* compilará o código de entrada e fará um *diff* rigoroso entre a saída gerada e o arquivo `.cal` esperado. Se o código de saída for `0` (sem diferenças), o teste passa.
5. **Refatoração (Blue):** Limpeza do código C desenvolvido para gerenciar a Tabela de Símbolos, eliminação de duplicações nas ações semânticas do Bison e otimização de memória.
6. **Commit:** O ciclo é finalizado e o código versionado.

### Requisitos e Ferramentas Necessárias

Para o adequado funcionamento da metodologia e do ambiente de desenvolvimento, são necessários:

* **Ambiente Linux:** Preferencialmente WSL (Windows Subsystem for Linux) rodando distribuição Ubuntu.
* **Ferramentas de Compilação:** `gcc` ou `clang`, e `make` (via pacote `build-essential`).
* **Geradores de Analisadores:** `flex` e `bison` devidamente instalados e no *PATH* do sistema.
* **Automação de Testes:** Um *script* local (escrito em *Bash*, *Python* ou outro) capaz de iterar sobre um diretório `tests/`, passar os arquivos pelo binário do compilador e comparar as saídas utilizando utilitários como `diff`.

### Visão CI/CD: Validação Contínua no GitHub

A segurança do TDD adaptado será potencializada por um pipeline de Integração Contínua (CI). O fluxo de *merge* para a ramificação principal (`main`) será protegido por automatizações do **GitHub Actions**.

**Pipeline de Validação:**

1. **Gatilho:** Abertura ou atualização de um *Pull Request* direcionado à `main`.
2. **Provisionamento:** O GitHub Actions inicializa um *runner* Ubuntu.
3. **Configuração de Ambiente:** Instalação automática das dependências (`sudo apt-get install flex bison build-essential`).
4. **Build:** Execução do comando `make` para compilar os arquivos `.l` e `.y` e gerar o executável do transpilador.
5. **Testes Automatizados:** Execução do comando `make test`, que aciona o *script* validador rodando toda a suíte de `.c` para `.cal`.
6. **Critério de Aceite:** O *merge* da ramificação só será liberado (*Merge Block*) se o *runner* reportar que 100% dos testes passaram, garantindo que o código novo não quebrou gramáticas antigas.

# Referências

AHO, Alfred V.; LAM, Monica S.; SETHI, Ravi; ULLMAN, Jeffrey D. *Compiladores: princípios, técnicas e ferramentas*. 2. ed. São Paulo: Pearson Addison-Wesley, 2007.

BECK, Kent. *Test-Driven Development: By Example*. 1. ed. Boston: Addison-Wesley Professional, 2002.

[thmasq/mini-c-compiler](https://github.com/thmasq/mini-c-compiler)

---

## Histórico de Versões

| Versão | Descrição | Data | Autor |
|--------|-----------|------|-------|
| 1.0 | Primeira versão do documento de metodologia TDD Adaptada | 15/04/2026 | [@Mateus](https://github.com/MVConsorte) |
| 1.1 | Ajuste do render | 15/04/2026 | [@Mateus](https://github.com/MVConsorte) |