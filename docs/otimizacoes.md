# Otimizações Aplicadas

## Visão Geral

No desenvolvimento do Mini C Compiler to Calango, o grupo optou por não implementar uma fase de otimização sobre código intermediário, como TAC (*Three-Address Code*) ou SSA (*Static Single Assignment*).

A versão final do gerador, entretanto, inclui otimizações aplicadas diretamente sobre a **Árvore Sintática Abstrata (AST)** antes da emissão do código Calango. Essa decisão mantém o projeto dentro do escopo da disciplina, mas ainda permite reduzir expressões constantes, simplificar operações redundantes e remover declarações ou atribuições sem uso.

---

## Otimizações na Tabela de Símbolos

A Tabela de Símbolos foi projetada para manter uma estrutura simples e suficiente para o tamanho esperado dos programas Mini C.

**Tabela 01:** Decisões de otimização na Tabela de Símbolos.

| Otimização | Impacto |
|---|---|
| **Busca por escopo** | Permite consultar símbolos respeitando o escopo ativo e evita conflitos de nomes no mesmo bloco. |
| **Remoção ao sair do escopo** | Libera símbolos associados ao escopo encerrado, reduzindo o uso de memória durante a compilação. |
| **Campo `inicializado`** | Permite emitir aviso quando uma variável é usada antes de receber valor. |
| **Campo `usado`** | Serve de apoio à identificação de variáveis que não influenciam o resultado final. |
| **Cópia segura de strings** | Reduz risco de corrupção de memória ao registrar nomes de identificadores. |
| **Lista encadeada** | Mantém a implementação simples e adequada ao volume de símbolos esperado. |

**Autor(es):** [João Pedro](https://github.com/Jadequilin), [Pedro Silva](https://github.com/314dro).

---

## Otimizações na AST

A AST representa a estrutura do programa depois da análise sintática. Como o gerador percorre essa árvore para produzir Calango, aplicar otimizações nessa estrutura permite limpar o programa antes da emissão final.

**Tabela 02:** Otimizações aplicadas na AST.

| Otimização | Exemplo | Resultado |
|---|---|---|
| **Constant folding** | `(2 + 3) * 4` | `20` |
| **Simplificação algébrica** | `x + 0`, `x - 0`, `x * 1`, `x / 1` | `x` |
| **Multiplicação por zero** | `x * 0` ou `0 * x` | `0` |
| **Simplificação lógica** | `true && x` | `x` |
| **Simplificação lógica** | `false || x` | `x` |
| **Remoção de variáveis mortas** | declaração/atribuição sem uso posterior | nó removido da saída |

Essas otimizações são aplicadas de forma recursiva por `otimizarAST`, antes da chamada final de geração de código.

**Autor(es):** [João Pedro](https://github.com/Jadequilin), [Pedro Silva](https://github.com/314dro).

---

## Otimizações na Geração de Código

O gerador recebe a AST validada e otimizada, percorrendo seus nós para emitir código Calango.

**Tabela 03:** Decisões de geração com impacto na saída.

| Decisão | Impacto |
|---|---|
| **Geração a partir de AST otimizada** | Garante que reduções e remoções sejam aplicadas antes da emissão do código final. |
| **Parentesamento explícito de expressões** | Preserva a precedência definida pela gramática e evita ambiguidades na linguagem alvo. |
| **Conversão de `for` para `enquanto`** | Traduz o `for` do Mini C para uma forma compatível com o Calango. |
| **Separação entre declaração e inicialização** | Converte `int x = 5;` para `inteiro x;` seguido de `x = 5;`, respeitando a sintaxe do Calango. |
| **Escolha entre `escreva` e `escreval`** | Usa `escreval` quando o formato contém `
` ao final; caso contrário, usa `escreva`. |
| **Bloqueio em caso de erro semântico** | Evita gerar Calango quando a análise semântica já identificou erro. |
| **Indentação padronizada** | Mantém a estrutura do código gerado legível e consistente. |

---

## Testes de Otimização

A PR de código final acrescenta testes específicos para a etapa de otimização no diretório `testes/gerador`.

| Teste | Finalidade |
|---|---|
| `teste_06_otimizacao_constant_folding.c` | Valida redução de expressões constantes. |
| `teste_07_otimizacao_simplificacao_algebrica.c` | Valida remoção de operações aritméticas redundantes. |
| `teste_08_otimizacao_logica.c` | Valida simplificação de expressões booleanas constantes. |
| `teste_09_otimizacao_variavel_morta.c` | Valida remoção de variáveis declaradas ou atribuídas sem uso posterior. |

Para executar apenas essa parte da suíte:

```bash
make test-opt
```

---

## Histórico de Versões

| Versão | Descrição | Data | Responsável |
|---|---|---|---|
| `0.1` | Criação da página para detalhamento das otimizações estruturais do compilador. | 06/06/2026 | [Luiz Faria](https://github.com/luizfaria1989), [João Pedro](https://github.com/Jadequilin) |
| `0.2` | Inclusão das otimizações sobre a AST, dos testes de otimização e da integração com o fluxo do gerador. | 19/06/2026 | [Pedro Silva](https://github.com/314dro), [João Pedro](https://github.com/Jadequilin) |
