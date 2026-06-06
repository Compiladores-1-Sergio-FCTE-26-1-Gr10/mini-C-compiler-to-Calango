# Otimizações Aplicadas

## Visão Geral

No desenvolvimento do Mini C Compiler to Calango, optou-se por não implementar uma fase de otimização explícita sobre código intermediário (como TAC — *Three-Address Code* ou SSA — *Static Single Assignment*). 

Em vez disso, foram aplicadas otimizações estruturais de alto nível. Essas otimizações atuam diretamente sobre a estrutura do programa, no design das estruturas de dados internas (como a Tabela de Símbolos e a Árvore Sintática Abstrata) e nas decisões durante a emissão do código final, garantindo máxima eficiência dentro do escopo do projeto.

---

## Otimizações na Tabela de Símbolos

A Tabela de Símbolos foi projetada para ter uma manutenção simplificada e desempenho adequado para o tamanho de escopo esperado no Mini C. A Tabela 01 apresenta as decisões de otimização adotadas.

**Tabela 01:** Otimizações aplicadas na Tabela de Símbolos.

| Otimização | Impacto |
|---|---|
| **Busca com escopo progressivo (interno → externo)** | Implementa o mecanismo de *shadowing* (sombreamento de variáveis) corretamente sem adicionar custos computacionais extras de resolução de escopo. |
| **Remoção imediata ao sair do escopo (`sairEscopo()`)** | Garante que o uso de memória seja mínimo em todos os momentos, limpando da tabela todas as variáveis assim que o bloco que as contém é encerrado. |
| **Campo `usado` (variáveis mortas)** | Deixa a estrutura pronta para futuras etapas de eliminação de declarações desnecessárias (código morto) na geração do Calango. |
| **Uso de `copiarStringSegura` (`strncpy` + null-term)** | Previne ativamente falhas de segurança e corrupção de memória (como *buffer overflow*) ao lidar com nomes de variáveis excessivamente longos. |
| **Implementação via Lista Encadeada** | Considerando as restrições da linguagem Mini C, a complexidade O(n) com n pequeno é performática o suficiente. Mantém a base de código muito mais simples, limpa e rastreável do que se fosse implementada uma *Hash Table*. |

**Autor(es):** [João Pedro](https://github.com/Jadequilin).

---

## Otimizações na AST (Árvore Sintática Abstrata)

O gerenciamento de memória e a travessia na AST formam o núcleo de performance do compilador durante as análises finais. A Tabela 02 resume as otimizações voltadas para a árvore.

**Tabela 02:** Otimizações aplicadas na estrutura da AST.

| Otimização | Impacto |
|---|---|
| **Lista encadeada direta via `->prox`** | A eliminação de nós intermediários "de lista" reduz a quantidade de alocações necessárias. Permite travessias mais rápidas e simplificadas através de loops comuns (`for(cur=n; cur; cur=cur->prox)`). |
| **Array fixo `filho[4]` com alocação única** | A exigência de apenas uma alocação por nó melhora exponencialmente a localidade de cache. Os quatro filhos são inicializados nativamente como NULL. |
| **Adoção restrita do `calloc` em vez do `malloc`** | Zera automaticamente os blocos de memória obtidos do sistema, eliminando uma grande classe de defeitos acidentais causados por contadores de filhos (`n_filhos`) não inicializados ou ponteiros com lixo de memória. |
| **Desalocação via `ast_libera` recursiva** | Libera os nós filhos e os nós encadeados em `prox` em profundidade antes de acionar o `free` do nó pai, garantindo *zero vazamento de memória* (memory leak) independentemente da profundidade ou largura da árvore sintática gerada. |

**Autor(es):** [João Pedro](https://github.com/Jadequilin).

---

## Otimizações na Geração de Código

O gerador recebe a AST completamente validada e a converte eficientemente. A Tabela 03 lista as otimizações implementadas para o empacotamento do código de saída.

**Tabela 03:** Otimizações aplicadas no pipeline de geração Calango.

| Otimização | Impacto |
|---|---|
| **Parentesamento explícito em `gen_expr`** | Garante que toda a avaliação de precedência matemática capturada pela gramática LALR(1) seja preservada de forma imutável no Calango, prevenindo divergências de interpretação da linguagem alvo. |
| **Escolha automática entre `escreva` vs `escreval`** | Ao analisar strings em tempo de compilação procurando quebras de linha explícitas, preserva a equivalência semântica de exibição sem a necessidade de comandos em múltiplas linhas. |
| **Conversão universal `for` → `enquanto`** | Ao traduzir todos os laços de repetição do tipo `for` para uma estrutura genérica `enquanto`, garante-se o sucesso da execução para qualquer condição ou inicialização declarada, visto a restrição do laço `para` do interpretador alvo. |
| **Bloqueio de pipeline (Fail-Fast)** | A inibição da geração de código para `erros_sem > 0` bloqueia desperdícios de processamento I/O e evita a geração de artefatos `.cal` incorretos a partir de lógicas furadas. |
| **Separação estrutural de funções `gen_expr` e `gen_no`** | Expressões tornam-se completamente enxutas (inline) e desatreladas do controle de espaço em branco ou *newlines* (`\n`), prevenindo a quebra inadequada de linhas de instrução ao resolver sentenças complexas. |
| **Indentação padronizada por nível** | O avanço em múltiplos de 3 espaços (`nivel * 3`) assegura a compatibilidade estrita do arquivo de saída com o `IndentadorCalango.java` nativo, promovendo legibilidade máxima do código compilado. |

**Autor(es):** [João Pedro](https://github.com/Jadequilin).

---

## Histórico de Versões

| Versão | Descrição | Data | Responsável |
| ------ | --------- | ---- | ----------- |
| `0.1` | Criação da página para detalhamento das otimizações estruturais do compilador. | 06/06/2026 | [Luiz Faria](https://github.com/luizfaria1989), [João Pedro](https://github.com/Jadequilin) |