# Planejamento de Sprints

## Visão Geral

Este planejamento organiza o desenvolvimento do compilador **Mini C → Calango** em **11 sprints**, com ciclo semanal e cadência fixa de **quarta-feira a quarta-feira**.

A organização foi feita considerando:

- a recomendação de uso de **Scrum com sprints semanais**;
- a exigência de documentação do planejamento no repositório;
- os marcos formais de avaliação da disciplina:
  - **P1**: formulário até **24/04/2026** e entrevistas em **27/04/2026** e **29/04/2026**;
  - **P2**: formulário até **29/05/2026** e entrevistas em **01/06/2026** e **03/06/2026**;
  - **Entrega final (T)**: entrevistas em **29/06/2026** e **01/07/2026**. 

Neste projeto, a equipe definiu que:

- **até a P1**, o objetivo é concluir **até o analisador sintático**;
- **a partir da etapa seguinte**, com foco na **P2**, o projeto avança para **análise semântica**, **geração de código** e testes ponta a ponta.

---

## Critério de Calendarização

As sprints seguem o seguinte padrão:

- **abertura**: quarta-feira;
- **execução**: de quarta a terça;
- **encerramento**: quarta-feira seguinte;
- a quarta de encerramento também funciona como momento de revisão da sprint anterior e planejamento da próxima.

A **Sprint 1** teve início em **01/04/2026**.

---

## Adoção de TDD no Projeto

Até a **primeira entrega (P1)**, a equipe **ainda não utilizou TDD de forma plena**.

Nesta fase inicial, o foco esteve em:

- estruturar o repositório;
- organizar a documentação;
- definir o escopo da linguagem;
- implementar o **analisador léxico**;
- implementar o **analisador sintático**.

Por esse motivo, os testes foram produzidos **após a implementação**, com o objetivo de validar o comportamento já desenvolvido e reduzir inconsistências antes da apresentação.

Essa decisão foi tomada porque, nesse estágio, a arquitetura do compilador, a gramática e a integração entre lexer e parser ainda estavam sendo consolidadas. Adotar TDD desde o primeiro momento teria elevado o retrabalho, já que várias definições ainda estavam instáveis.

Ainda assim, a equipe procurou preparar a base do projeto para uma adoção mais consistente dessa abordagem a partir da próxima iteração. Assim, **a partir da fase voltada para a P2**, o desenvolvimento passará a adotar **TDD de forma mais estruturada**.

---

## Distribuição por Marco

### Até o P1

Até a primeira entrega, a equipe pretende concluir:

- definição do escopo do projeto;
- estruturação do repositório;
- documentação inicial;
- analisador léxico;
- analisador sintático;
- testes léxicos e sintáticos;
- material de apresentação do P1.

### Até o P2

Até a segunda entrega, a equipe pretende concluir:

- análise semântica;
- validação de identificadores e tipos;
- tratamento de erros semânticos principais;
- início e evolução da geração de código para Calango;
- testes semânticos;
- testes ponta a ponta;
- material de apresentação do P2.

### Até a Entrega Final

Até a entrevista final, a equipe pretende consolidar:

- geração de código;
- estabilização do compilador;
- correção de erros;
- regressão ponta a ponta;
- documentação final;
- preparação técnica da apresentação final.

---

## Cronograma das 11 Sprints

| Sprint | Abertura | Execução | Encerramento | Objetivo principal |
|---|---|---|---|---|
| Sprint 1 | 01/04/2026 | 01/04 a 07/04 | 08/04/2026 | Estruturar o projeto e definir a base técnica |
| Sprint 2 | 08/04/2026 | 08/04 a 14/04 | 15/04/2026 | Consolidar o analisador léxico |
| Sprint 3 | 15/04/2026 | 15/04 a 21/04 | 22/04/2026 | Implementar a base do analisador sintático |
| Sprint 4 | 22/04/2026 | 22/04 a 28/04 | 29/04/2026 | Concluir o sintático e preparar o P1 |
| Sprint 5 | 29/04/2026 | 29/04 a 05/05 | 06/05/2026 | Iniciar a análise semântica |
| Sprint 6 | 06/05/2026 | 06/05 a 12/05 | 13/05/2026 | Evoluir a checagem semântica |
| Sprint 7 | 13/05/2026 | 13/05 a 19/05 | 20/05/2026 | Consolidar a base semântica e preparar a tradução |
| Sprint 8 | 20/05/2026 | 20/05 a 26/05 | 27/05/2026 | Evoluir geração de código para o subconjunto principal |
| Sprint 9 | 27/05/2026 | 27/05 a 02/06 | 03/06/2026 | Consolidar o material e a entrega do P2 |
| Sprint 10 | 03/06/2026 | 03/06 a 09/06 | 10/06/2026 | Ampliar geração de código e testes ponta a ponta |
| Sprint 11 | 10/06/2026 | 10/06 a 16/06 | 17/06/2026 | Estabilização final, documentação e preparação da entrega |

---

## Detalhamento das Sprints

### Sprint 1
**Abertura:** 01/04/2026  
**Execução:** 01/04/2026 a 07/04/2026  
**Encerramento:** 08/04/2026  

**Objetivo:** estruturar o projeto e definir a base técnica e documental.

**Tarefas:**
- organizar o repositório da equipe;
- definir a stack do projeto;
- configurar o ambiente com `flex`, `bison`, compilador C e `make`;
- configurar a documentação com MkDocs;
- configurar a publicação no GitHub Pages;
- criar as páginas iniciais da documentação;
- definir o escopo preliminar da Mini C;
- definir a linguagem-alvo Calango;
- registrar as decisões técnicas iniciais;
- alinhar o fluxo de trabalho da equipe.

**Entregas esperadas:**
- repositório estruturado;
- documentação inicial publicada;
- escopo preliminar definido;
- base técnica inicial organizada.

---

### Sprint 2
**Abertura:** 08/04/2026  
**Execução:** 08/04/2026 a 14/04/2026  
**Encerramento:** 15/04/2026  

**Objetivo:** consolidar o analisador léxico.

**Tarefas:**
- definir tokens da linguagem;
- formalizar palavras-chave, identificadores, literais, operadores e delimitadores;
- definir o tratamento de comentários e espaços em branco;
- implementar as regras principais do `lexer.l`;
- validar entradas léxicas válidas;
- validar entradas léxicas inválidas;
- documentar a etapa léxica.

**Entregas esperadas:**
- primeira versão funcional do analisador léxico;
- testes léxicos iniciais;
- documentação léxica consistente com a implementação.

---

### Sprint 3
**Abertura:** 15/04/2026  
**Execução:** 15/04/2026 a 21/04/2026  
**Encerramento:** 22/04/2026  

**Objetivo:** implementar a base do analisador sintático.

**Tarefas:**
- criar ou evoluir o `parser.y`;
- implementar a estrutura do programa principal;
- implementar regras de declaração;
- implementar atribuição;
- implementar expressões básicas;
- configurar precedência e associatividade;
- integrar lexer e parser;
- criar testes sintáticos iniciais;
- atualizar a documentação da gramática.

**Entregas esperadas:**
- parser aceitando programas básicos válidos;
- rejeição de entradas sintaticamente inválidas;
- gramática inicial documentada.

---

### Sprint 4
**Abertura:** 22/04/2026  
**Execução:** 22/04/2026 a 28/04/2026  
**Encerramento:** 29/04/2026  

**Objetivo:** concluir o analisador sintático e preparar a apresentação do P1.

**Tarefas:**
- implementar estruturas sintáticas restantes;
- consolidar `if/else`, `while`, `for` e `do-while`, se ainda faltarem;
- consolidar `printf` e `scanf` no nível sintático;
- revisar conflitos e ambiguidades da gramática;
- revisar documentação de escopo, decisões técnicas, léxico e sintático;
- preparar o formulário do P1;
- preparar a demonstração e a fala da equipe.

**Entregas esperadas:**
- etapa sintática consolidada;
- documentação coerente com o estado real do projeto;
- equipe preparada para a P1.

**Observação:**  
Nesta entrega, o foco da equipe é **até o analisador sintático**. A análise semântica ficará para a etapa seguinte.

---

### Sprint 5
**Abertura:** 29/04/2026  
**Execução:** 29/04/2026 a 05/05/2026  
**Encerramento:** 06/05/2026  

**Objetivo:** iniciar a análise semântica.

**Tarefas:**
- projetar a tabela de símbolos;
- implementar inserção e busca de identificadores;
- validar declaração antes do uso;
- validar redeclaração no mesmo escopo;
- definir o formato das mensagens de erro semântico;
- iniciar a estrutura de testes para a nova etapa.

**Entregas esperadas:**
- infraestrutura inicial da análise semântica;
- validações básicas de identificadores;
- base preparada para adoção mais consistente de TDD.

---

### Sprint 6
**Abertura:** 06/05/2026  
**Execução:** 06/05/2026 a 12/05/2026  
**Encerramento:** 13/05/2026  

**Objetivo:** evoluir a checagem semântica.

**Tarefas:**
- validar tipos em atribuições;
- validar promoção `int -> float`, se mantida no escopo;
- rejeitar conversões implícitas não suportadas;
- validar expressões aritméticas;
- validar expressões relacionais;
- validar expressões lógicas;
- validar condições em estruturas de controle;
- ampliar a cobertura dos testes semânticos.

**Entregas esperadas:**
- checagem de tipos inicial consolidada;
- erros semânticos mais precisos;
- início prático da adoção de TDD para a fase semântica.

---

### Sprint 7
**Abertura:** 13/05/2026  
**Execução:** 13/05/2026 a 19/05/2026  
**Encerramento:** 20/05/2026  

**Objetivo:** consolidar a base semântica e preparar a tradução.

**Tarefas:**
- finalizar regras semânticas prioritárias;
- revisar coerência entre escopo e implementação;
- definir a estratégia de geração de código;
- mapear tipos Mini C → Calango;
- mapear operadores e comandos principais;
- criar testes para saídas esperadas em Calango;
- documentar as decisões da tradução.

**Entregas esperadas:**
- análise semântica cobrindo o núcleo do subconjunto da linguagem;
- estratégia de geração de código definida;
- testes ponta a ponta iniciais.

---

### Sprint 8
**Abertura:** 20/05/2026  
**Execução:** 20/05/2026 a 26/05/2026  
**Encerramento:** 27/05/2026  

**Objetivo:** evoluir a geração de código para o subconjunto principal.

**Tarefas:**
- gerar código para declarações;
- gerar código para atribuições;
- gerar código para `printf` e `scanf`;
- gerar código para estruturas de seleção e repetição prioritárias;
- tratar diferenças relevantes entre Mini C e Calango;
- ampliar os testes ponta a ponta;
- revisar backlog antes do P2.

**Entregas esperadas:**
- tradução funcional das construções centrais da linguagem;
- maior estabilidade do fluxo Mini C → Calango;
- base técnica para a apresentação do P2.

---

### Sprint 9
**Abertura:** 27/05/2026  
**Execução:** 27/05/2026 a 02/06/2026  
**Encerramento:** 03/06/2026  

**Objetivo:** consolidar o material e a apresentação do P2.

**Tarefas:**
- revisar o que foi prometido desde o P1;
- consolidar as melhorias realizadas;
- ajustar o planejamento com base no progresso real;
- revisar documentação da etapa semântica e de geração;
- preparar o formulário do P2;
- preparar demonstração e explicação da equipe;
- estabilizar build e testes antes da entrevista.

**Entregas esperadas:**
- material do P2 pronto;
- progresso semântico e de geração demonstrável;
- backlog reajustado para a fase final.

---

### Sprint 10
**Abertura:** 03/06/2026  
**Execução:** 03/06/2026 a 09/06/2026  
**Encerramento:** 10/06/2026  

**Objetivo:** ampliar geração de código e testes ponta a ponta.

**Tarefas:**
- completar traduções ainda pendentes;
- revisar geração de operadores lógicos e relacionais;
- revisar casos especiais de tradução;
- ampliar casos de teste `.c -> .cal`;
- fortalecer `make test`;
- integrar execução automatizada de testes;
- corrigir regressões identificadas.

**Entregas esperadas:**
- geração de código mais completa;
- pipeline de testes mais sólido;
- compilador mais estável.

---

### Sprint 11
**Abertura:** 10/06/2026  
**Execução:** 10/06/2026 a 16/06/2026  
**Encerramento:** 17/06/2026  

**Objetivo:** estabilização final, documentação e preparação da entrega final.

**Tarefas:**
- corrigir bugs remanescentes;
- revisar mensagens de erro;
- revisar README e páginas técnicas;
- revisar páginas de escopo, decisões técnicas, léxico, sintático, semântico e sprints;
- registrar problemas encontrados e soluções adotadas;
- rodar suíte final de regressão;
- preparar o roteiro da entrevista final;
- alinhar a equipe para a explicação técnica do projeto.

**Entregas esperadas:**
- compilador estabilizado;
- documentação final consistente;
- equipe preparada para a entrevista final.

---

## Relação com os Marcos da Disciplina

| Marco | Data acadêmica | Sprint associada |
|---|---|---|
| P1 | formulário até 24/04/2026 e entrevistas em 27/04/2026 e 29/04/2026 | Sprint 4 |
| P2 | formulário até 29/05/2026 e entrevistas em 01/06/2026 e 03/06/2026 | Sprint 9 |
| Entrega final (T) | entrevistas em 29/06/2026 e 01/07/2026 | Pós-Sprint 11 |

---

## Definição de Pronto

Uma sprint será considerada concluída quando:

- as tarefas prioritárias da sprint tiverem sido implementadas;
- os testes correspondentes tiverem sido executados;
- a documentação estiver coerente com o estado real do projeto;
- a equipe conseguir demonstrar o incremento entregue.

---

## Considerações Finais

Este planejamento busca manter coerência entre:

- o cronograma acadêmico da disciplina;
- os marcos de avaliação;
- o estado atual do projeto;
- a capacidade real de evolução da equipe.

A estratégia adotada prioriza:

1. **P1 até o sintático**;
2. **P2 com análise semântica e avanço da geração de código**;
3. **fase final voltada à estabilização, testes e entrega**.

Além disso, o plano registra explicitamente a transição metodológica do projeto:

- **na P1**, os testes foram feitos **após a implementação**;
- **a partir da fase seguinte**, a equipe pretende aplicar **TDD de forma mais consistente**.

---

## Histórico de Versões

| Versão | Descrição | Data | Responsável |
| ------ | --------- | ---- | ----------- |
| `0.1` | Criação da página. | 08/04/2026 | [@Luiz]() |
| `0.2` | Criação do planejamento inicial de sprints | 22/04/2026 | Grupo |
| `0.3` | Criação definitiva do planejamento das sprints | 04/04/2026 | [@314dro]() |