%{
/*
 * Analisador Sintático — Mini C Compiler to Calango
 * Disciplina: FGA0003 – Compiladores 1 (2026/1)
 * Professor: Dr. Sergio Antônio Andrade de Freitas
 *
 * Escopo suportado:
 *   - Tipos: int, float, char, bool
 *   - Controle de fluxo: if, else, while, for, do-while
 *   - I/O: printf, scanf
 *   - Literais: inteiros, reais, char, string, true/false
 *   - Operadores: aritméticos, relacionais, lógicos, atribuição simples
 *   - Declarações apenas no início do bloco main
 *
 * Restrições conhecidas (fora do escopo):
 *   - Funções além do main
 *   - Arrays
 *   - Cast explícito
 *   - Atribuição composta (+=, -=, *=, /=)
 *   - Incremento/decremento (++, --)
 *   - Ponteiros, bitwise, ternário
 */

#include <stdio.h>
#include <stdlib.h>

/* Declarações do lexer */
extern int   yylex();
extern int   yylineno;
extern char *yytext;
extern int   linha;
extern int   coluna;
extern FILE *yyin;

void yyerror(const char *msg) {
    fprintf(stderr, "ERRO SINTÁTICO [linha %d, col %d]: %s (token: '%s')\n",
            linha, coluna, msg, yytext);
}
%}

/* ── Declaração dos tokens ──────────────────────────────────── */
%token KW_INT KW_FLOAT KW_CHAR KW_BOOL
%token KW_IF KW_ELSE
%token KW_WHILE KW_FOR KW_DO
%token KW_PRINTF KW_SCANF
%token KW_MAIN

%token ID
%token LIT_INT LIT_FLOAT LIT_CHAR LIT_STRING
%token LIT_TRUE LIT_FALSE

%token PLUS MINUS TIMES DIVIDE MOD
%token EQ NEQ LT GT LEQ GEQ
%token AND OR NOT
%token ASSIGN
%token LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET
%token SEMICOLON COMMA DOT

/* ── Precedência e associatividade (menor → maior) ──────────── */
%left OR
%left AND
%left EQ NEQ
%left LT GT LEQ GEQ
%left PLUS MINUS
%left TIMES DIVIDE MOD
%right NOT
%right UMINUS

%start programa

%%

/* ══════════════════════════════════════════════════════════════
   ESTRUTURA GERAL DO PROGRAMA
   ══════════════════════════════════════════════════════════════ */

programa
    : KW_INT KW_MAIN LPAREN RPAREN LBRACE bloco RBRACE
        { printf("Análise sintática concluída: programa válido.\n"); }
    ;

bloco
    : declaracoes instrucoes
    ;

/* ══════════════════════════════════════════════════════════════
   DECLARAÇÕES (apenas no início do bloco)
   ══════════════════════════════════════════════════════════════ */

declaracoes
    : declaracoes declaracao
    | /* vazio */
    ;

declaracao
    : tipo ID SEMICOLON
    | tipo ID ASSIGN expressao SEMICOLON
    ;

tipo
    : KW_INT
    | KW_FLOAT
    | KW_CHAR
    | KW_BOOL
    ;

/* ══════════════════════════════════════════════════════════════
   INSTRUÇÕES
   ══════════════════════════════════════════════════════════════ */

instrucoes
    : instrucoes instrucao
    | /* vazio */
    ;

instrucao
    : atribuicao
    | condicional
    | enquanto
    | para
    | faca_enquanto
    | saida
    | entrada
    ;

/* ── Atribuição ─────────────────────────────────────────────── */
atribuicao
    : ID ASSIGN expressao SEMICOLON
    ;

/* ── Condicional ────────────────────────────────────────────── */
condicional
    : KW_IF LPAREN expressao RPAREN LBRACE instrucoes RBRACE
    | KW_IF LPAREN expressao RPAREN LBRACE instrucoes RBRACE
      KW_ELSE LBRACE instrucoes RBRACE
    ;

/* ── Enquanto ───────────────────────────────────────────────── */
enquanto
    : KW_WHILE LPAREN expressao RPAREN LBRACE instrucoes RBRACE
    ;

/* ── Para ───────────────────────────────────────────────────── */
para
    : KW_FOR LPAREN atribuicao expressao SEMICOLON atribuicao_for RPAREN
      LBRACE instrucoes RBRACE
    ;

atribuicao_for
    : ID ASSIGN expressao
    ;

/* ── Faca-enquanto ──────────────────────────────────────────── */
faca_enquanto
    : KW_DO LBRACE instrucoes RBRACE
      KW_WHILE LPAREN expressao RPAREN SEMICOLON
    ;

/* ── Saída (printf) ─────────────────────────────────────────── */
saida
    : KW_PRINTF LPAREN LIT_STRING RPAREN SEMICOLON
    | KW_PRINTF LPAREN LIT_STRING COMMA args_printf RPAREN SEMICOLON
    ;

args_printf
    : expressao
    | args_printf COMMA expressao
    ;

/* ── Entrada (scanf) ────────────────────────────────────────── */
entrada
    : KW_SCANF LPAREN LIT_STRING COMMA ID RPAREN SEMICOLON
    ;

/* ══════════════════════════════════════════════════════════════
   EXPRESSÕES
   ══════════════════════════════════════════════════════════════ */

expressao
    : expressao OR     expressao
    | expressao AND    expressao
    | expressao EQ     expressao
    | expressao NEQ    expressao
    | expressao LT     expressao
    | expressao GT     expressao
    | expressao LEQ    expressao
    | expressao GEQ    expressao
    | expressao PLUS   expressao
    | expressao MINUS  expressao
    | expressao TIMES  expressao
    | expressao DIVIDE expressao
    | expressao MOD    expressao
    | NOT expressao
    | MINUS expressao   %prec UMINUS
    | LPAREN expressao RPAREN
    | ID
    | LIT_INT
    | LIT_FLOAT
    | LIT_CHAR
    | LIT_STRING
    | LIT_TRUE
    | LIT_FALSE
    ;

%%

/* ── Programa principal ─────────────────────────────────────── */
int main(int argc, char *argv[]) {
    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        if (!f) {
            fprintf(stderr, "Erro: não foi possível abrir '%s'\n", argv[1]);
            return 1;
        }
        yyin = f;
    }

    int resultado = yyparse();

    if (argc > 1) fclose(yyin);
    return resultado;
}