# Analisador lêxico

## Gramática livre de contexto

###  Hierarquia de Chomsky

Para explicar o que é uma gramática livre de contexto deve ser discutido primeiro os conceitos de hierarquia de Chomsky. Proposta por Noam Chomsky em 1959, a hierarquia de Chomsky busca classificar as linguagens em quatro categorias, que vão do tipo 0 ao tipo 3. A Tabela 01 resume os conceitos de cada uma dessas categorias.

**Tabela 01:** Resumo dos tipos de linguagem.

| Tipo de linguagem | Tipo de gramática | Exemplo |
| ----------------- | ----------------- | ------- |
| Regular (Tipo-3) | Gramática regular | - |
| Livre de contexto (Tipo-2) | Gramática livre de contexto | Fortran, Pascal |
| Sensível ao contexto (Tipo-1) | Gramática sensível ao contexto | - |
| Irrestrita (Tipo-0) | Gramática irrestrita | - |

### Sintaxe da gramática livre de contexto

As gramáticas livre de contexto apresentam quatro diferentes conjuntos de termos, são eles:

* **$V$**: Representa um conjunto de símbolos não-terminais.
* **$\Sigma$:** Representa um conjunto de símbolos terminais. Os elementos de $\Sigma$ são utilizados para definir a sintaxe (estrutura) da linguagem (Tremblay; Sorenson, 2008, p.35). A união dos conjuntos de símbolos não terminais com $V$ com os símbolos terminais $\Sigma$ é chamada de vocabulário de uma linguagem (Tremblay; Sorenson, 2008, p.35).
* **$P$**: Conjunto de regras que envolve as relações de $(\Sigma \cup V)*V(\Sigma \cup V)*$ para o conjunto $(\Sigma \cup V)$ (Tremblay; Sorenson, 2008, p.35).
* **$S$**: Símbolo inicial.

## Expressões regulares

As expressões regulares abaixo definem os padrões reconhecidos pelo analisador. A notação utilizada é a do Flex (compatível com POSIX estendido).

### Fragmentos auxiliares

Fragmentos são definições nomeadas usadas para compor expressões maiores.

**Tabela 02:** Resumo dos fragmentos auxiliares.

| Nome | Expressão Regular | Descrição |
|---|---|---|
| `DIGITO` | `[0-9]` | Qualquer dígito decimal |
| `LETRA` | `[a-zA-Z_]` | Letra maiúscula, minúscula ou underscore |
| `ALFANUM` | `[a-zA-Z0-9_]` | Letra, dígito ou underscore |
| `BRANCO` | `[ \t\r]+` | Espaços, tabulações e retorno de carro |
| `COM_LINHA` | `\/\/[^\n]*` | Comentário de linha (do `//` até o fim da linha) |
| `COM_BLOCO` | `\/\*([^*]\|\*+[^*/])*\*+\/` | Comentário de bloco `/* ... */` |

**Autor:** [Luiz Faria](https://github.com/luizfaria1989), [João Pedro](https://github.com/Jadequilin),  [Rivaldâvio](https://github.com/RivaFilho).

### Literais e identificadores

Tremblay e Sorenson (2008, p. 144) explicam que os identificadores começam com uma letra e consistem em todos os caracteres alfanuméricos que se seguem; já os literais começam e terminam com o uso de apóstrofos ('). Seguindo essa definição, a Tabela 03, apresenta uma versão resumida dos literais e identificadores presentes na construção deste compilador.

**Tabela 03:** Resumo dos literais e identificadores.

| Token | Expressão Regular | Descrição | Exemplo |
|---|---|---|---|
| `ID` | `[a-zA-Z_][a-zA-Z0-9_]*` | Identificador: começa com letra ou `_`, seguido de letras, dígitos ou `_` | `x`, `minhaVar`, `_aux1` |
| `LIT_INT` | `[0-9]+` | Sequência de um ou mais dígitos | `0`, `42`, `1000` |
| `LIT_FLOAT` | `[0-9]+\.[0-9]+` | Parte inteira, ponto obrigatório, parte decimal | `3.14`, `0.5`, `100.0` |
| `LIT_CHAR` | `'([^'\\]\|\\.)' ` | Caractere entre aspas simples, com suporte a escapes | `'a'`, `'\n'`, `'\\'` |
| `LIT_STRING` | `\"([^\"\\]\|\\.)*\"` | Sequência de caracteres entre aspas duplas, com suporte a escapes | `"ola"`, `"x\n"` |
| `LIT_TRUE` | `true` | Literal booleano verdadeiro → `VERDADEIRO` no Calango | `true` |
| `LIT_FALSE` | `false` | Literal booleano falso → `FALSO` no Calango | `false` |

**Autor:** [Luiz Faria](https://github.com/luizfaria1989), [João Pedro](https://github.com/Jadequilin),  [Rivaldâvio](https://github.com/RivaFilho).

> **Nota:** A regra de `LIT_FLOAT` é posicionada **antes** de `LIT_INT` no arquivo `.l`. Isso garante que `3.14` seja reconhecido como um único token flutuante, e não como `3` (inteiro) + `.` (ponto) + `14` (inteiro).

> **Nota:** As palavras reservadas (`int`, `if`, `while`, etc.) são posicionadas **antes** da regra de `ID`. O Flex aplica a primeira regra que corresponder ao texto, portanto `if` nunca será classificado como identificador.

## Tokens 

### Palavras reservadas

Tremblay e Sorenson (2008, p. 90) explicam que as palavras reservadas (_Keywords_) não devem ser utilizadas como identificadores, devem ser pronunciáveis (para fácil recordação), e devem ser geralmente devem ser escolhidas para que seja improvável que dupliquem identificadores definidos pelo usuário. Seguindo a definição dos autores, a Tabela 04, apresenta uma versão resumida das palavras reservadas presentes na construção deste compilador.

**Tabela 04:** Resumo das palavras reservadas.

| Token | Lexema | Equivalente no Calango |
|---|---|---|
| `KW_INT` | `int` | `inteiro` |
| `KW_FLOAT` | `float` | `real` |
| `KW_CHAR` | `char` | `caracter` |
| `KW_BOOL` | `bool` | `logico` |
| `KW_IF` | `if` | `se ... entao` |
| `KW_ELSE` | `else` | `senao` |
| `KW_WHILE` | `while` | `enquanto ... faca` |
| `KW_FOR` | `for` | `para ... faca` |
| `KW_DO` | `do` | `faca` (parte do `faca...enquanto`) |
| `KW_PRINTF` | `printf` | `escreva` / `escreval` |
| `KW_SCANF` | `scanf` | `leia` / `leiaCaracter` |
| `KW_MAIN` | `main` | bloco `principal` |

**Autor:** [Luiz Faria](https://github.com/luizfaria1989), [João Pedro](https://github.com/Jadequilin),  [Rivaldâvio](https://github.com/RivaFilho).

### Operadores aritméticos

Tremblay e Sorenson (2008, p. 774) explicam os operadores binários e unários são parte da notação algorítmica, eles recebem a ordem matemática padrão de precedência ao serem utilizados. Seguindo a definição dos autores, a Tabela 05, apresenta uma versão resumida dos operações aritméticos presentes na construção deste compilador.

**Tabela 05:** Resumo dos tokens de operadores aritméticos.

| Token | Lexema | Equivalente no Calango |
|---|---|---|
| `PLUS` | `+` | `+` |
| `MINUS` | `-` | `-` |
| `TIMES` | `*` | `*` |
| `DIVIDE` | `/` | `/` (real) ou `\` (inteiro, decidido na geração de código) |
| `MOD` | `%` | `mod` |

**Autor:** [Luiz Faria](https://github.com/luizfaria1989), [João Pedro](https://github.com/Jadequilin),  [Rivaldâvio](https://github.com/RivaFilho).

> **Nota sobre divisão:** Em C, `/` entre dois inteiros realiza divisão inteira. No Calango, `/` é sempre divisão real e `\` é divisão inteira. A distinção é **semântica** (depende dos tipos dos operandos) e será resolvida na fase de geração de código, não no léxico.

### Operadores relacionais

Tremblay e Sorenson (2008, p. 588) explicam os operadores relacionais podem ser implementados com a criação de uma rotina capaz de gerar uma ramificação condicional, que busque avaliar qual tipo de operador está sendo utilizado na expressão analisada. A Tabela 06, apresenta uma versão resumida dos operações relacionais presentes na construção deste compilador.

**Tabela 06:** Resumo dos operadores relacionais.

| Token | Lexema | Equivalente no Calango |
|---|---|---|
| `EQ` | `==` | `==` |
| `NEQ` | `!=` | `!=` |
| `LT` | `<` | `<` |
| `GT` | `>` | `>` |
| `LEQ` | `<=` | `<=` |
| `GEQ` | `>=` | `>=` |

**Autor:** [Luiz Faria](https://github.com/luizfaria1989), [João Pedro](https://github.com/Jadequilin),  [Rivaldâvio](https://github.com/RivaFilho).

### Operadores lógicos

Tremblay e Sorenson (2008, p.776) explicam que os operadores binários são: negação lógica, e lógico, ou lógico; eles devem seguir a ordem de prioridade em que o operador NÃO recebe a maior ordem de prioridade e o operador OU a menor ordem. Seguindo a definição dos autores, a Tabela 07 apresenta os três operadores lógicos e sua representação na documentação do compilador.

**Tabela 07:** Resumo dos operadores lógicos.

| Token | Lexema | Equivalente no Calango |
|---|---|---|
| `AND` | `&&` | `e` |
| `OR` | `\|\|` | `ou` |
| `NOT` | `!` | `nao` |

**Autor:** [Luiz Faria](https://github.com/luizfaria1989), [João Pedro](https://github.com/Jadequilin),  [Rivaldâvio](https://github.com/RivaFilho).

### Atribuição

Tremblay e Sorenson (2008, p.591) explicam que o aspecto único sobre a instrução de atribuição (que denotamos por `=`), é que do ponto de vista da análise semântica é necessário uma referência à uma localização de uma variável, em vez de seu valor. Ou seja, o compilador precisa identificar primeiro o endereço físico de memória onde o novo dado será gravado, independente do valor que a variável está guardando naquele momento. A Tabela 08 apresenta o resumo do operador de atribuição utilizado na construção deste compilador.

**Tabela 08:** Resumo dos tokens de atribuição.

| Token | Lexema | Observação |
|---|---|---|
| `ASSIGN` | `=` | Única forma de atribuição suportada |

**Autor:** [Luiz Faria](https://github.com/luizfaria1989), [João Pedro](https://github.com/Jadequilin),  [Rivaldâvio](https://github.com/RivaFilho).

### Delimitadores

**Tabela 09:** Resumo dos tokens de delimitação.

| Token | Lexema |
|---|---|
| `LPAREN` | `(` |
| `RPAREN` | `)` |
| `LBRACE` | `{` |
| `RBRACE` | `}` |
| `LBRACKET` | `[` |
| `RBRACKET` | `]` |
| `SEMICOLON` | `;` |
| `COMMA` | `,` |
| `DOT` | `.` |

**Autor:** [Luiz Faria](https://github.com/luizfaria1989), [João Pedro](https://github.com/Jadequilin),  [Rivaldâvio](https://github.com/RivaFilho).

## Tratamento de erros (símbolos inválidos)

O analisador distingue dois tipos de erro léxico:

**Símbolos fora do escopo** — operadores que existem em C mas não têm equivalente no Calango. O lexer os reconhece explicitamente e emite uma mensagem informativa com sugestão de correção quando possível:

```
ERRO LÉXICO [linha 5, col 3]: '++' fora do escopo. Use '= x + 1'.
ERRO LÉXICO [linha 7, col 8]: '+=' fora do escopo.
ERRO LÉXICO [linha 9, col 2]: '&' (endereço/bitwise) fora do escopo.
```

**Símbolos desconhecidos** — qualquer caractere que não se enquadra em nenhuma regra definida:

```
ERRO LÉXICO [linha 3, col 5]: símbolo desconhecido '@'.
```

Em ambos os casos, o analisador **não interrompe** a execução — continua processando o restante do arquivo para reportar o máximo de erros possível em uma única passagem.

## Conveções adicionais

- **Case-sensitive:** Mini C é case-sensitive, assim como C. `Int` e `int` são tokens diferentes (o primeiro seria um `ID`, o segundo `KW_INT`).
- **Rastreamento de posição:** Linha e coluna são mantidos manualmente para cada token, permitindo mensagens de erro precisas nas fases subsequentes do compilador.
- **Comentários são descartados:** Tanto comentários de linha (`//`) quanto de bloco (`/* */`) são consumidos pelo lexer e não geram tokens.
- **Espaços são descartados:** Espaços, tabulações e quebras de linha são usados apenas para atualizar os contadores de posição.

## Bibliografia 

1. TREMBLAY, Jean-Paul; SORENSON, Paul G. **The Theory and Practice of Compiler Writing**. 1ª edição. 2008.

## Histórico de Versões

| Versão | Descrição | Data | Responsável |
| ------ | --------- | ---- | ----------- |
| `0.1` | Criação da página e início da documentação. | 08/04/2026 | [Luiz](https://github.com/luizfaria1989) |
| `0.2` | Melhorias na documentação do analisador léxico. | 08/04/2026 | [Luiz Faria](https://github.com/luizfaria1989), [João Pedro](https://github.com/Jadequilin),  [Rivaldâvio](https://github.com/RivaFilho)  |
| `0.3` | Adiciona referências bibliográficas dos tipos de token e autoria das tabelas. | 20/04/2026 | [Luiz Faria](https://github.com/luizfaria1989)  |