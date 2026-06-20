# Otimizações Aplicadas

## Visão Geral

No desenvolvimento do Mini C Compiler to Calango, optou-se por não implementar uma fase de otimização sobre código intermediário, como TAC — *Three-Address Code* — ou SSA — *Static Single Assignment*.

Em vez disso, foram aplicadas otimizações estruturais de alto nível, atuando diretamente sobre a Tabela de Símbolos, a Árvore Sintática Abstrata — AST — e o processo de geração do código final em Calango.

Com as atualizações mais recentes, o compilador passou a contar também com uma etapa explícita de otimização sobre a AST antes da geração do código Calango. Essa etapa reduz expressões constantes, simplifica operações redundantes e remove declarações ou atribuições que não impactam o resultado final do programa.

---

## Otimizações na Tabela de Símbolos

A Tabela de Símbolos foi projetada para manter uma estrutura simples e adequada ao escopo esperado da linguagem Mini C.

**Tabela 01:** Otimizações aplicadas na Tabela de Símbolos.

| Otimização                                                 | Impacto                                                                                                                          |
| ---------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------- |
| **Busca com escopo progressivo — interno → externo**       | Permite resolver corretamente o sombreamento de variáveis sem aumentar desnecessariamente a complexidade da resolução de escopo. |
| **Remoção imediata ao sair do escopo (`sairEscopo`)**      | Remove da tabela os símbolos pertencentes ao bloco encerrado, reduzindo o uso de memória durante a compilação.                   |
| **Campo `usado` para rastreamento de variáveis**           | Permite identificar quais símbolos foram efetivamente utilizados, servindo como base para remoção de variáveis mortas.           |
| **Remoção de variáveis mortas (`removerVariaveisMortas`)** | Remove variáveis declaradas que não são utilizadas, evitando declarações desnecessárias no código Calango gerado.                |
| **Cópia segura de strings**                                | Evita problemas de corrupção de memória ao lidar com nomes de identificadores.                                                   |
| **Implementação via lista encadeada**                      | Mantém a estrutura simples e suficiente para o tamanho esperado dos programas Mini C.                                            |

**Autor(es):** [João Pedro](https://github.com/Jadequilin), [Pedro Silva](https://github.com/314dro).

---

## Otimizações na AST

A AST representa a estrutura sintática do programa e é utilizada nas etapas finais do compilador. As otimizações nessa estrutura reduzem ruídos antes da geração do código Calango.

**Tabela 02:** Otimizações aplicadas na AST.

| Otimização                                      | Impacto                                                                                                 |   |       |   |         |
| ----------------------------------------------- | ------------------------------------------------------------------------------------------------------- | - | ----- | - | ------- |
| **Lista encadeada direta via `prox`**           | Reduz a necessidade de nós intermediários de lista e simplifica a travessia sequencial da árvore.       |   |       |   |         |
| **Array fixo de filhos**                        | Mantém os filhos de cada nó em uma estrutura simples e previsível.                                      |   |       |   |         |
| **Uso de `calloc` na criação de nós**           | Inicializa os campos com zero, reduzindo riscos de ponteiros inválidos ou contadores não inicializados. |   |       |   |         |
| **Liberação recursiva com `ast_libera`**        | Libera filhos e nós encadeados, reduzindo risco de vazamento de memória.                                |   |       |   |         |
| **Otimização recursiva com `otimizarAST`**      | Percorre a árvore antes da geração de código, aplicando otimizações diretamente nos nós da AST.         |   |       |   |         |
| **Constant Folding**                            | Resolve expressões constantes em tempo de compilação. Exemplo: `(2 + 3) * 4` passa a ser `20`.          |   |       |   |         |
| **Simplificação algébrica**                     | Remove operações redundantes, como `x + 0`, `x - 0`, `x * 1`, `x / 1` e `x * 0`.                        |   |       |   |         |
| **Simplificação lógica**                        | Reduz expressões booleanas redundantes, como `true && x`, `false                                        |   | x`e`x |   | false`. |
| **Remoção de declarações e atribuições mortas** | Remove nós que não influenciam o resultado final do programa.                                           |   |       |   |         |

**Autor(es):** [João Pedro](https://github.com/Jadequilin), [Pedro Silva](https://github.com/314dro).

---

## Otimizações na Geração de Código

O gerador recebe a AST validada e otimizada, convertendo-a para código Calango.

**Tabela 03:** Otimizações aplicadas no pipeline de geração Calango.

| Otimização                                                  | Impacto                                                                                                              |
| ----------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------- |
| **Geração a partir de AST otimizada (`gerarASTOtimizada`)** | Garante que o código Calango seja emitido após a redução e limpeza da AST.                                           |
| **Integração com o `parser.y` do gerador**                  | O fluxo principal passa a chamar `gerarASTOtimizada`, evitando que a otimização fique isolada ou sem efeito prático. |
| **Parentesamento explícito em expressões**                  | Preserva a precedência das operações no código Calango gerado.                                                       |
| **Escolha entre `escreva` e `escreval`**                    | Mantém a equivalência da saída de acordo com o uso de quebra de linha.                                               |
| **Conversão de `for` para `enquanto`**                      | Traduz estruturas `for` para uma forma compatível com o Calango.                                                     |
| **Bloqueio da geração em caso de erro semântico**           | Evita a emissão de código inválido quando há erros detectados anteriormente.                                         |
| **Indentação padronizada**                                  | Mantém a legibilidade e a estrutura esperada no código gerado.                                                       |

**Autor(es):** [João Pedro](https://github.com/Jadequilin), [Pedro Silva](https://github.com/314dro).

---

## Exemplos de Otimizações Aplicadas

**Tabela 04:** Exemplos de transformações realizadas antes da geração do código Calango.

| Código Mini C                              | Resultado esperado         | Otimização aplicada       |                       |                      |
| ------------------------------------------ | -------------------------- | ------------------------- | --------------------- | -------------------- |
| `resultado = (2 + 3) * 4;`                 | `resultado = 20;`          | Constant Folding          |                       |                      |
| `y = (x + 0) * 1;`                         | `y = x;`                   | Simplificação algébrica   |                       |                      |
| `ativo = true && (false                    |                            | true);`                   | `ativo = verdadeiro;` | Simplificação lógica |
| `int morta; morta = 99;` sem uso posterior | variável removida da saída | Remoção de variável morta |                       |                      |

---

## Testes de Otimização

As otimizações foram validadas seguindo o padrão do repositório, mantendo os testes dentro da pasta `testes/gerador`.

| Teste                                           | Finalidade                                                                |
| ----------------------------------------------- | ------------------------------------------------------------------------- |
| `teste_06_otimizacao_constant_folding.c`        | Valida a redução de expressões constantes antes da geração do código.     |
| `teste_07_otimizacao_simplificacao_algebrica.c` | Valida a remoção de operações aritméticas redundantes.                    |
| `teste_08_otimizacao_logica.c`                  | Valida a simplificação de expressões booleanas constantes.                |
| `teste_09_otimizacao_variavel_morta.c`          | Valida a remoção de variáveis declaradas ou atribuídas sem uso posterior. |

Executar apenas testes de otimização:

```bash
make test-opt
```

Executar toda a suíte:

```bash
make test
```

---

## Histórico de Versões

| Versão | Descrição                                                                                                                                                                                                          | Data       | Responsável                                                                                 |
| ------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ---------- | ------------------------------------------------------------------------------------------- |
| `0.1`  | Criação da página para detalhamento das otimizações estruturais do compilador.                                                                                                                                     | 06/06/2026 | [Luiz Faria](https://github.com/luizfaria1989), [João Pedro](https://github.com/Jadequilin) |
| `0.2`  | Melhoria da etapa de otimização sobre a AST, incluindo constant folding, simplificação algébrica, simplificação lógica, remoção de variáveis mortas e integração dos testes de otimização ao fluxo do repositório. | 19/06/2026 | [Pedro Silva](https://github.com/314dro)                                                    |
