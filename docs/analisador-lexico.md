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
* **$S$**: Simbolo inicial.

## Tokens 

| Token | Classificação | Padrão (REGEX) | Exemplo / Valor |
| ----- | ------------- | -------------- | --------------- |
| `PLUS`| Operador aritmético | + | + |
| `TIMES`| Operador aritmético | * | * |
| `MINUS` | Operador aritmético | - | - |
| `TIMES` | Operador aritmético | \ | \ |
| `AND` | Operador booleano | && | && |
| `OR` | Operador booleano | | |
| `LPAREN` | Delimitador aritmético | ( | ( |
| `RPAREN` | Delimitador aritmético | ) | ) |
| `KW_IF` | Palavra reservada | if | if |
| `KW_ELSE` | Palavra reservada | else | else |
| `KW_FOR` | Palavra reservada | for | for |
| `KW_WHILE` | Palavra reservada | while | while |
| `NUM_INT` | Número inteiro | [0-9]+ | 3 |

## Tratamento de erros

### Símbolos inválidos

## Conveções adicionais

## Bibliografia 

1. TREMBLAY, Jean-Paul; SORENSON, Paul G. **The Theory and Practice of Compiler Writing**. 1ª edição. 2008.

## Histórico de Versões

| Versão | Descrição | Data | Responsável |
| ------ | --------- | ---- | ----------- |
| `0.1` | Criação da página e início da documentação. | 08/04/2026 | [@Luiz]() |