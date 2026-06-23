# Analisador Semântico

## Visão Geral

A análise semântica verifica se um programa sintaticamente válido também respeita as regras de significado da linguagem. No projeto Mini C Compiler to Calango, essa fase é integrada às ações do Bison e utiliza uma Tabela de Símbolos para registrar informações sobre variáveis.

Na versão final do projeto, a análise semântica atua principalmente sobre declaração, uso, inicialização e disponibilidade dos identificadores. A etapa também constrói a AST, que será usada posteriormente pela otimização e pela geração de código.

---

## Integração com o Parser

O analisador semântico é implementado junto ao arquivo `parser.y` da etapa semântica. A cada redução de regra gramatical, o Bison executa ações em C responsáveis por:

- inserir variáveis declaradas na Tabela de Símbolos;
- verificar redeclaração no escopo atual;
- verificar uso de variável não declarada;
- marcar variáveis como inicializadas;
- emitir aviso quando uma variável é lida antes de receber valor;
- construir nós da AST correspondentes às declarações, expressões e instruções.

Essa abordagem é conhecida como tradução dirigida pela sintaxe, pois as ações semânticas são associadas diretamente às regras da gramática.

---

## Tabela de Símbolos

A Tabela de Símbolos armazena os identificadores declarados no programa e seus metadados.

**Tabela 01:** Campos principais da estrutura de símbolo.

| Campo | Descrição |
|---|---|
| `nome` | Nome do identificador. |
| `tipo` | Tipo declarado: `int`, `float`, `char` ou `bool`. |
| `linhaDeclaracao` | Linha em que a variável foi declarada. |
| `inicializado` | Indica se a variável já recebeu valor. |
| `usado` | Indica se a variável foi referenciada em alguma expressão ou comando. |
| `ordemDeclaracao` | Ordem de inserção na tabela. |
| `escopo` | Identificador do escopo em que o símbolo foi inserido. |
| `prox` | Ponteiro para o próximo símbolo da lista encadeada. |

**Tabela 02:** Operações principais.

| Operação | Responsabilidade |
|---|---|
| `inicializarTabela` | Prepara a tabela antes da análise. |
| `inserirSimbolo` | Registra uma nova variável declarada. |
| `buscarSimboloEscopoAtual` | Verifica se já existe variável com mesmo nome no escopo atual. |
| `buscarSimbolo` | Procura uma variável declarada para uso em expressão, atribuição ou entrada/saída. |
| `marcarInicializado` | Atualiza o estado de uma variável que recebeu valor. |
| `marcarUsado` | Registra que uma variável foi utilizada. |
| `imprimirTabela` | Exibe o conteúdo da tabela ao final da análise semântica. |

---

## Verificações Implementadas

### Redeclaração de variável

Ao declarar uma variável, o compilador verifica se já existe símbolo com o mesmo nome no escopo atual.

```c
int main() {
    int x;
    int x;
}
```

Saída esperada:

```text
ERRO SEMÂNTICO [linha 3]: variável 'x' já declarada neste escopo.
```

### Uso de variável não declarada

O analisador verifica se o identificador existe antes de usá-lo em atribuições, expressões, `scanf` e passo de `for`.

```c
int main() {
    z = 10;
}
```

Saída esperada:

```text
ERRO SEMÂNTICO [linha 2]: variável 'z' não declarada.
```

### Uso antes da inicialização

Quando uma variável declarada é lida em uma expressão antes de receber valor, o analisador emite um aviso semântico.

```c
int main() {
    int x;
    int y;
    y = x + 1;
}
```

Saída esperada:

```text
AVISO SEMÂNTICO [linha 4]: 'x' usada sem inicialização.
```

### Marcação de variáveis utilizadas

A fase semântica marca identificadores como usados ao encontrá-los em expressões, atribuições, comandos de entrada e comandos de saída. Essa informação também serve de apoio para a etapa de otimização da AST no gerador.

---

## Relação com a AST

Além das verificações, o parser semântico constrói a AST. Cada declaração, atribuição, comando de controle, entrada/saída e expressão gera um nó ou subárvore. Ao final da análise, a AST pode ser impressa para depuração e reutilizada pelo fluxo de geração de código.

---

## Limitações da Verificação Semântica

A Tabela de Símbolos registra o tipo declarado de cada variável e o código possui funções auxiliares para tratar compatibilidade de tipos. Porém, na implementação atual, as ações do parser concentram a validação efetiva em declaração, uso e inicialização. Por isso, a documentação não deve afirmar que todas as incompatibilidades de tipos são bloqueadas em tempo de compilação.

---

## Bibliografia

1. AHO, Alfred V. et al. **Compilers: Principles, Techniques, and Tools**. 2ª edição. Pearson, 2008.
2. TREMBLAY, Jean-Paul; SORENSON, Paul G. **The Theory and Practice of Compiler Writing**. 1ª edição. 2008.

---

## Histórico de Versões

| Versão | Descrição | Data | Responsável |
|---|---|---|---|
| `0.1` | Criação da página e documentação inicial do analisador semântico. | 24/04/2026 | [João Pedro](https://github.com/Jadequilin) |
| `0.2` | Atualização da documentação conforme a implementação final da Tabela de Símbolos, avisos de inicialização e AST. | 19/06/2026 | [João Pedro](https://github.com/Jadequilin), [Pedro Silva](https://github.com/314dro) |
