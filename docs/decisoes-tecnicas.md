# Decisões Técnicas

## Visão Geral

Esta página registra as principais decisões técnicas tomadas pelo grupo ao longo do desenvolvimento do compilador Mini C → Calango. Cada decisão é acompanhada de sua justificativa, garantindo rastreabilidade e transparência no processo de desenvolvimento.

---

## Ferramentas Escolhidas

| Ferramenta | Função | Justificativa |
|---|---|---|
| **Flex** | Geração do analisador léxico | Ferramenta padrão da disciplina; gera AFD automaticamente a partir de expressões regulares |
| **Bison** | Geração do analisador sintático | Complementar ao Flex; gera parser LALR(1) a partir de gramáticas livres de contexto |
| **C** | Linguagem de implementação | Integração nativa com Flex e Bison; controle fino sobre estruturas de dados |
| **VS Code** | Editor de código-fonte (IDE) | Ambiente de desenvolvimento com suporte nativo/plugins para C, Flex, Bison e visualização de documentação |
| **Markdown** | Linguagem de marcação | Sintaxe limpa, legível e padronizada, ideal para focar no conteúdo técnico sem se preocupar com complexidades de formatação visual |
| **MkDocs** | Gerador de site estático | Compila rapidamente os arquivos Markdown em um site de documentação navegável, estruturado e com aspecto profissional |
| **Git** | Controle de versão de código | Permite o rastreamento seguro do histórico de alterações e viabiliza o desenvolvimento paralelo do código entre os membros da equipe |
| **GitHub** | Hospedagem do repositório remoto | Centraliza o código-fonte na nuvem, facilitando o trabalho colaborativo, o backup do projeto e a revisão de código |
| **GitHub Pages** | Hospedagem da documentação online | Integração nativa com o GitHub que permite publicar o site gerado pelo MkDocs de forma automatizada e gratuita |
| **Microsoft Teams** | Comunicação e gestão da equipe | Plataforma institucional que centraliza reuniões por vídeos |

---

## Arquitetura do Projeto

O compilador é organizado em dois arquivos separados que se comunicam entre si:

- `lexer.l` — analisador léxico (Flex): lê o código-fonte e retorna tokens ao parser
- `parser.y` — analisador sintático (Bison): consome os tokens e valida a estrutura do programa

O arquivo `parser.tab.h`, gerado automaticamente pelo Bison, é incluído no `lexer.l` para que ambos compartilhem a definição dos tokens. Dessa forma, os arquivos permanecem separados mas integrados.

**Fluxo de compilação:**

```
código Mini C (.c)
       ↓
   lexer.l (Flex)
       ↓ tokens
   parser.y (Bison)
       ↓
 "programa válido" / erro
```

---

## Restrições do Escopo

Por decisão do grupo, as construções abaixo **não são suportadas** nesta versão do compilador. Todas as restrições foram autorizadas pelo professor e estão justificadas a seguir.

### Funções além do `main`

**Decisão:** não suportado.

**Justificativa:** O Calango utiliza subprogramas com sintaxe e semântica distintas das funções em C. A tradução correta exigiria gerenciamento de escopo separado por função em todas as fases do compilador (léxico, sintático, semântico e geração de código), adicionando complexidade desproporcional ao prazo disponível.

### `return` e `void`

**Decisão:** não suportado.

**Justificativa:** Consequência direta da restrição de funções. Sem funções definidas pelo usuário, `return` e `void` não se aplicam ao escopo do projeto.

### Atribuição composta (`+=`, `-=`, `*=`, `/=`)

**Decisão:** não suportado.

**Justificativa:** Não existe equivalente direto no Calango. A alternativa correta é a atribuição simples expandida: `x = x + 1`. O lexer reporta erro informativo ao encontrar esses operadores.

### Incremento e decremento (`++`, `--`)

**Decisão:** não suportado.

**Justificativa:** Mesmo motivo da atribuição composta. Substituir por `x = x + 1` é equivalente e compatível com o Calango. O lexer reporta erro informativo com sugestão de correção.

### Arrays

**Decisão:** não suportado.

**Justificativa:** Arrays adicionam complexidade em todas as fases do compilador: declaração com tamanho, acesso indexado, verificação de bounds na semântica e tradução para vetores do Calango. A restrição permite entregar um compilador funcional e completo para variáveis simples dentro do prazo.

### Cast explícito

**Decisão:** não suportado.

**Justificativa:** O Calango não possui operador de cast explícito. A tradução não seria direta e a construção é pouco comum em programas simples dentro do escopo suportado.

### Operadores bitwise (`&`, `|`, `^`, `~`, `<<`, `>>`)

**Decisão:** não suportado.

**Justificativa:** O Calango não possui operadores bitwise. Não há tradução possível sem alterar a semântica do programa.

### Operador ternário (`?:`)

**Decisão:** não suportado.

**Justificativa:** O Calango não possui operador ternário. O equivalente é um `se...entao...senao`, que pertence à análise sintática e não ao léxico.

### Declaração de variáveis fora do início do bloco

**Decisão:** apenas no início do bloco `main` (estilo C89).

**Justificativa:** Simplifica o parser, que não precisa distinguir declarações de instruções em posições arbitrárias. O Calango também exige declarações no início do bloco, tornando a restrição consistente com a linguagem alvo.

---

## Decisões de Implementação

### Precedência de operadores via diretivas do Bison

Em vez de criar regras gramaticais separadas para cada nível de precedência (`termo_add`, `termo_mul`, etc.), utilizamos as diretivas `%left` e `%right` do Bison. O resultado semântico é idêntico, mas o arquivo `.y` fica significativamente mais simples e legível.

### `UMINUS` — menos unário

O token fictício `UMINUS` foi criado exclusivamente para atribuir ao menos unário (`-x`) precedência maior do que a subtração (`a - b`), evitando ambiguidade na gramática sem necessidade de regra extra.

### `LIT_FLOAT` antes de `LIT_INT` no lexer

A regra de `LIT_FLOAT` é posicionada antes de `LIT_INT` no `lexer.l`. O Flex aplica a primeira regra que corresponder — sem essa ordem, `3.14` seria tokenizado como `LIT_INT(3)` + `DOT` + `LIT_INT(14)`.

### Palavras reservadas antes de `ID`

Todas as palavras reservadas (`int`, `if`, `while`, etc.) são declaradas antes da regra de `ID` no lexer. Isso garante que `if` seja sempre `KW_IF` e nunca classificado como identificador.

### Operadores de dois caracteres antes de um caractere

Os operadores `==`, `!=`, `<=`, `>=`, `&&`, `||` são declarados antes de `=`, `<`, `>`, `&`, `|`. Sem essa ordem, `==` seria tokenizado como dois tokens `ASSIGN` separados.

### Divisão: decisão semântica, não léxica

Em C, `/` entre inteiros é divisão inteira; entre reais, é divisão real. No Calango, `/` é sempre divisão real e `\` é divisão inteira. O lexer reconhece apenas `DIVIDE` para o token `/` — a distinção entre divisão inteira e real será resolvida na fase de geração de código, com base nos tipos dos operandos.

### `%` mapeado para `mod`

O operador de resto `%` do Mini C é mapeado para `mod` do Calango. A tradução é semanticamente equivalente para inteiros positivos, que é o caso de uso predominante no escopo do projeto.

## Histórico de Versões

| Versão | Descrição | Data | Responsável |
| ------ | --------- | ---- | ----------- |
| `0.1` | Criação da página. | 08/04/2026 | [@Luiz Faria](https://github.com/luizfaria1989) |
| `0.2` | Início da documentação das decisões técnicas do projeto. | 11/04/2026 | [@Luiz Faria](https://github.com/luizfaria1989), [@João Pedro](https://github.com/Jadequilin),  [@Rivaldâvio](https://github.com/RivaFilho) |
| `0.3` | Incrementa tabela de ferramentas escolhidas. | 21/04/2026 | [@Luiz Faria](https://github.com/luizfaria1989) |