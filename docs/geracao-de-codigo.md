# Geração de Código

## Visão Geral

> (definicao)

No projeto Mini C Compiler to Calango, o gerador de código (`gerador.c`) atua diretamente sobre a Árvore Sintática Abstrata (AST) construída e validada na fase semântica. O código Calango (`.cal`) equivalente só é emitido se a análise semântica relatar zero erros (`erros_sem == 0`).

A saída desta fase foi testada e validada contra os analisadores oficiais do Calango (`CalangoTokenMaker.flex` e `IndentadorCalango.java`), garantindo que o código gerado possua a sintaxe, a indentação padronizada (3 espaços por nível) e as palavras-chave estritamente compatíveis.

---

## Mapeamento Mini C para Calango

A linguagem Calango possui regras sintáticas, comandos e estruturas mais restritas e específicas do que a linguagem C. A Tabela 01 apresenta as principais conversões estruturais e lexicais implementadas pelo gerador.

**Tabela 01:** Mapeamento das estruturas do Mini C para o Calango.

| Mini C | Calango | Observação |
|---|---|---|
| `int`, `float`, `char`, `bool` | `inteiro`, `real`, `caracter`, `logico` | Mapeamento direto de tipos primitivos. |
| `true`, `false` | `verdadeiro`, `falso` | Mapeamento de literais booleanos. |
| `if(c){}` | `se(c)entao ... fimSe` | Sem espaço entre `se` e `(`. |
| `if(c){} else{}` | `se(c)entao ... senao ... fimSe` | Estrutura condicional completa. |
| `while(c){}` | `enquanto(c)faca ... fimEnquanto` | Laço de repetição. |
| `for(init; cond; passo){}` | `init` + `enquanto(cond)faca ... passo` + `fimEnquanto` | O laço `para` do Calango é muito restrito. Todo `for` é traduzido utilizando a estrutura genérica `enquanto`. |
| `do{} while(c);` | `faca ... enquanto(c);` | O sinal de `;` é obrigatório após o `enquanto` final. |
| `printf("...
", x)` | `escreval(x)` | O analisador detecta o escape `\n` no final da string para a quebra. |
| `printf("...", x)` | `escreva(x)` | Comando de saída padrão sem quebra de linha. |
| `scanf("%d", &n)` | `leia(n)` | O `&` (endereço) e o especificador de formato (ex: `%d`) são descartados na saída. |
| `&&`, `\|\|`, `!`, `%` | `e`, `ou`, `nao`, `mod` | Mapeamento de operadores lógicos e aritméticos. |

**Autor(es):** [João Pedro](https://github.com/Jadequilin).

---

## Funções do Gerador

A arquitetura do processo de geração é dividida em funções modulares (`gerador.c`), cada uma responsável por transcrever nós ou subárvores específicas da AST para texto puro. A Tabela 02 resume as principais funções que compõem essa API.

**Tabela 02:** Funções que compõem o gerador de código.

| Função | Responsabilidade |
|---|---|
| `gerar(raiz, saida)` | Ponto de entrada público do gerador. Prepara o esqueleto do arquivo (ex: `algoritmo MiniC`, `principal`) e inicia a recursão no bloco global. |
| `gen_tipo(n, out)` | Converte nós do tipo de dado da AST para a respectiva palavra-chave em Calango (ex: `NO_TIPO_INT` → `inteiro`). |
| `gen_expr(n, out)` | Gera as expressões matemáticas e lógicas *inline*, de forma totalmente recursiva e sem emitir quebras de linha (`\n`). |
| `gen_no(n, out, nivel)` | Responsável por gerar a instrução ou declaração COMPLETA (incluindo o `\n`). Utiliza a função utilitária `indent(out, nivel)` para a formatação do código. |
| `gen_lista(n, out, nivel)` | Percorre as listas encadeadas (por meio do atributo `->prox`), chamando `gen_no` iterativamente para cada declaração ou instrução do bloco. |
| `gen_printf(n, out, nivel)` | Possui lógica especializada para analisar a string alvo do nó `printf`, definindo se usará `escreva` ou `escreval`, emitindo os argumentos sem os marcadores do C. |

**Autor(es):** [João Pedro](https://github.com/Jadequilin).

---

## Decisões de Projeto e Otimizações Estruturais

Como algumas construções válidas do Mini C não possuem contrapartida exata no Calango, foram necessárias adaptações na lógica do gerador para garantir que os arquivos `.cal` finais compilem sem erros semânticos. A Tabela 03 aborda os tratamentos especiais implementados.

**Tabela 03:** Decisões de tradução e tratamentos do gerador.

| Decisão | Justificativa e Impacto |
|---|---|
| **Tradução Universal do `for`** | A instrução `para` do Calango aceita apenas contadores inteiros operando de forma estritamente sequencial. Como o Mini C permite expressões de passo e condições arbitrárias no `for`, optou-se por convertê-lo invariavelmente para um bloco `enquanto`. A inicialização precede o laço e a instrução de passo ocorre no fechamento do bloco. |
| **Desmembramento de Atribuições na Declaração** | O interpretador Calango não permite declarar e inicializar variáveis na mesma linha (ex: `inteiro x = 5`). O gerador detecta este comportamento (um `NO_DECL` que carrega um `NO_ATRIB` como filho) e imprime duas linhas separadas: a declaração do tipo e a atribuição isolada em seguida. |
| **Parentesamento Explícito de Expressões** | As regras de precedência entre C e Calango poderiam causar discrepâncias no momento da interpretação. Como o *Bison* já construiu a AST respeitando a precedência correta, a função `gen_expr` envolve todos os operadores binários em seus próprios parênteses explícitos `(a + (b * c))`. |
| **Separação de Contexto Lógico (`gen_expr` vs `gen_no`)** | A rigorosa separação de expressões *inline* da lógica de impressão de nós evita a injeção acidental de quebras de linha durante resoluções matemáticas profundas ou cadeias lógicas dentro de condicionais `se`. |

**Autor(es):** [João Pedro](https://github.com/Jadequilin).

---

## Bibliografia

1

---

## Histórico de Versões

| Versão | Descrição | Data | Responsável |
| ------ | --------- | ---- | ----------- |
| `0.1` | Criação da página e documentação da etapa de Geração de Código do compilador. | 06/06/2026 | [Luiz Faria](https://github.com/luizfaria1989), [João Pedro](https://github.com/Jadequilin) |