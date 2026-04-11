%{
/*
 * Analisador Léxico — Mini C Compiler to Calango
 * Disciplina: FGA0003 – Compiladores 1 (2026/1)
 * Professor: Dr. Sergio Antônio Andrade de Freitas
 *
 * Escopo suportado:
 *   - Tipos: int, float, char, bool
 *   - Controle de fluxo: if, else, while, for, do-while
 *   - I/O: printf, scanf
 *   - Literais: inteiros, reais, char, string, true/false
 *   - Operadores: aritméticos, relacionais, lógicos, atribuição simples
 *
 * Restrições conhecidas (fora do escopo):
 *   - Funções além do main (return, void não suportados)
 *   - Atribuição composta (+=, -=, *=, /=)
 *   - Incremento/decremento (++, --)
 *   - Ponteiros e operadores de endereço
 *   - Operadores bitwise (&, |, ^, ~, <<, >>)
 *   - Operador ternário (?:)
 *
 * Mapeamento Mini C → Calango (referência para geração de código):
 *   int       → inteiro
 *   float     → real
 *   char      → caracter
 *   bool      → logico
 *   if/else   → se ... entao / senao
 *   while     → enquanto ... faca
 *   for       → para ... faca
 *   do-while  → faca ... enquanto
 *   printf    → escreva / escreval
 *   scanf     → leia / leiaCaracter
 *   &&        → e
 *   ||        → ou
 *   !         → nao
 *   %         → mod
 *   true      → VERDADEIRO
 *   false     → FALSO
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Contadores para relatório de erros */
int linha = 1;
int coluna = 1;

/* Atualiza coluna após cada token */
#define ATUALIZA_COLUNA() coluna += yyleng

/* Enumeração de tokens */
typedef enum {
    /* Palavras reservadas */
    KW_INT, KW_FLOAT, KW_CHAR, KW_BOOL,
    KW_IF, KW_ELSE,
    KW_WHILE, KW_FOR, KW_DO,
    KW_PRINTF, KW_SCANF,
    KW_MAIN,

    /* Literais */
    LIT_INT,
    LIT_FLOAT,
    LIT_CHAR,
    LIT_STRING,
    LIT_TRUE,
    LIT_FALSE,

    /* Identificador */
    ID,

    /* Operadores aritméticos */
    PLUS,           /* +  */
    MINUS,          /* -  */
    TIMES,          /* *  */
    DIVIDE,         /* /  */
    MOD,            /* %  → mod no Calango */

    /* Operadores relacionais */
    EQ,             /* == */
    NEQ,            /* != */
    LT,             /* <  */
    GT,             /* >  */
    LEQ,            /* <= */
    GEQ,            /* >= */

    /* Operadores lógicos */
    AND,            /* && → e   no Calango */
    OR,             /* || → ou  no Calango */
    NOT,            /* !  → nao no Calango */

    /* Atribuição */
    ASSIGN,         /* =  */

    /* Delimitadores */
    LPAREN,         /* (  */
    RPAREN,         /* )  */
    LBRACE,         /* {  */
    RBRACE,         /* }  */
    LBRACKET,       /* [  */
    RBRACKET,       /* ]  */
    SEMICOLON,      /* ;  */
    COMMA,          /* ,  */
    DOT,            /* .  */

    /* Controle */
    TOKEN_EOF,
    TOKEN_ERRO
} TokenTipo;

/* Estrutura do token */
typedef struct {
    TokenTipo tipo;
    char      lexema[256];
    int       linha;
    int       coluna;
} Token;

/* Imprime token no formato de tabela */
void imprime_token(TokenTipo tipo, const char *nome, const char *lexema, int lin, int col) {
    printf("%-12d %-12d %-20s %s\n", lin, col, nome, lexema);
}

%}

/* ── Opções do Flex ─────────────────────────────────────────── */
%option noyywrap
%option yylineno

/* ── Definições (macros de regex) ───────────────────────────── */
DIGITO          [0-9]
LETRA           [a-zA-Z_]
ALFANUM         [a-zA-Z0-9_]

/* Literais numéricos */
NUM_INT         {DIGITO}+
NUM_FLOAT       {DIGITO}+\.{DIGITO}+

/* Literal char: caractere simples ou sequência de escape */
LIT_CHAR        '([^'\\]|\\.)'

/* Literal string */
LIT_STRING      \"([^\"\\]|\\.)*\"

/* Identificador */
ID              {LETRA}{ALFANUM}*

/* Espaços e tabulações */
BRANCO          [ \t\r]+

/* Comentário de linha */
COM_LINHA       \/\/[^\n]*

/* Comentário de bloco */
COM_BLOCO       \/\*([^*]|\*+[^*/])*\*+\/

%%

 /* ── Comentários (descartar) ──────────────────────────────── */
{COM_LINHA}     { ATUALIZA_COLUNA(); /* ignora */ }
{COM_BLOCO}     {
                    /* Atualiza linha e coluna para comentários multiline */
                    for (int i = 0; i < yyleng; i++) {
                        if (yytext[i] == '\n') { linha++; coluna = 1; }
                        else coluna++;
                    }
                }

 /* ── Espaços (descartar) ──────────────────────────────────── */
{BRANCO}        { ATUALIZA_COLUNA(); }
\n              { linha++; coluna = 1; }

 /* ── Palavras reservadas ──────────────────────────────────── */
"int"           { imprime_token(KW_INT,     "KW_INT",     yytext, linha, coluna); ATUALIZA_COLUNA(); }
"float"         { imprime_token(KW_FLOAT,   "KW_FLOAT",   yytext, linha, coluna); ATUALIZA_COLUNA(); }
"char"          { imprime_token(KW_CHAR,    "KW_CHAR",    yytext, linha, coluna); ATUALIZA_COLUNA(); }
"bool"          { imprime_token(KW_BOOL,    "KW_BOOL",    yytext, linha, coluna); ATUALIZA_COLUNA(); }
"if"            { imprime_token(KW_IF,      "KW_IF",      yytext, linha, coluna); ATUALIZA_COLUNA(); }
"else"          { imprime_token(KW_ELSE,    "KW_ELSE",    yytext, linha, coluna); ATUALIZA_COLUNA(); }
"while"         { imprime_token(KW_WHILE,   "KW_WHILE",   yytext, linha, coluna); ATUALIZA_COLUNA(); }
"for"           { imprime_token(KW_FOR,     "KW_FOR",     yytext, linha, coluna); ATUALIZA_COLUNA(); }
"do"            { imprime_token(KW_DO,      "KW_DO",      yytext, linha, coluna); ATUALIZA_COLUNA(); }
"printf"        { imprime_token(KW_PRINTF,  "KW_PRINTF",  yytext, linha, coluna); ATUALIZA_COLUNA(); }
"scanf"         { imprime_token(KW_SCANF,   "KW_SCANF",   yytext, linha, coluna); ATUALIZA_COLUNA(); }
"main"          { imprime_token(KW_MAIN,    "KW_MAIN",    yytext, linha, coluna); ATUALIZA_COLUNA(); }
"true"          { imprime_token(LIT_TRUE,   "LIT_TRUE",   yytext, linha, coluna); ATUALIZA_COLUNA(); }
"false"         { imprime_token(LIT_FALSE,  "LIT_FALSE",  yytext, linha, coluna); ATUALIZA_COLUNA(); }

 /* ── Literais numéricos ───────────────────────────────────── */
 /* FLOAT antes de INT para evitar que "3.14" seja lido como INT "3" + DOT + INT "14" */
{NUM_FLOAT}     { imprime_token(LIT_FLOAT,  "LIT_FLOAT",  yytext, linha, coluna); ATUALIZA_COLUNA(); }
{NUM_INT}       { imprime_token(LIT_INT,    "LIT_INT",    yytext, linha, coluna); ATUALIZA_COLUNA(); }

 /* ── Literal char ─────────────────────────────────────────── */
{LIT_CHAR}      { imprime_token(LIT_CHAR,   "LIT_CHAR",   yytext, linha, coluna); ATUALIZA_COLUNA(); }

 /* ── Literal string ───────────────────────────────────────── */
{LIT_STRING}    { imprime_token(LIT_STRING, "LIT_STRING", yytext, linha, coluna); ATUALIZA_COLUNA(); }

 /* ── Identificador (após palavras reservadas) ─────────────── */
{ID}            { imprime_token(ID,          "ID",         yytext, linha, coluna); ATUALIZA_COLUNA(); }

 /* ── Operadores aritméticos ───────────────────────────────── */
"+"             { imprime_token(PLUS,    "PLUS",    yytext, linha, coluna); ATUALIZA_COLUNA(); }
"-"             { imprime_token(MINUS,   "MINUS",   yytext, linha, coluna); ATUALIZA_COLUNA(); }
"*"             { imprime_token(TIMES,   "TIMES",   yytext, linha, coluna); ATUALIZA_COLUNA(); }
"/"             { imprime_token(DIVIDE,  "DIVIDE",  yytext, linha, coluna); ATUALIZA_COLUNA(); }
"%"             { imprime_token(MOD,     "MOD",     yytext, linha, coluna); ATUALIZA_COLUNA(); }

 /* ── Operadores relacionais ───────────────────────────────── */
 /* Operadores de dois caracteres ANTES dos de um caractere */
"=="            { imprime_token(EQ,      "EQ",      yytext, linha, coluna); ATUALIZA_COLUNA(); }
"!="            { imprime_token(NEQ,     "NEQ",     yytext, linha, coluna); ATUALIZA_COLUNA(); }
"<="            { imprime_token(LEQ,     "LEQ",     yytext, linha, coluna); ATUALIZA_COLUNA(); }
">="            { imprime_token(GEQ,     "GEQ",     yytext, linha, coluna); ATUALIZA_COLUNA(); }
"<"             { imprime_token(LT,      "LT",      yytext, linha, coluna); ATUALIZA_COLUNA(); }
">"             { imprime_token(GT,      "GT",      yytext, linha, coluna); ATUALIZA_COLUNA(); }

 /* ── Operadores lógicos ───────────────────────────────────── */
"&&"            { imprime_token(AND,     "AND",     yytext, linha, coluna); ATUALIZA_COLUNA(); }
"||"            { imprime_token(OR,      "OR",      yytext, linha, coluna); ATUALIZA_COLUNA(); }
"!"             { imprime_token(NOT,     "NOT",     yytext, linha, coluna); ATUALIZA_COLUNA(); }

 /* ── Atribuição ───────────────────────────────────────────── */
"="             { imprime_token(ASSIGN,  "ASSIGN",  yytext, linha, coluna); ATUALIZA_COLUNA(); }

 /* ── Delimitadores ────────────────────────────────────────── */
"("             { imprime_token(LPAREN,   "LPAREN",   yytext, linha, coluna); ATUALIZA_COLUNA(); }
")"             { imprime_token(RPAREN,   "RPAREN",   yytext, linha, coluna); ATUALIZA_COLUNA(); }
"{"             { imprime_token(LBRACE,   "LBRACE",   yytext, linha, coluna); ATUALIZA_COLUNA(); }
"}"             { imprime_token(RBRACE,   "RBRACE",   yytext, linha, coluna); ATUALIZA_COLUNA(); }
"["             { imprime_token(LBRACKET, "LBRACKET", yytext, linha, coluna); ATUALIZA_COLUNA(); }
"]"             { imprime_token(RBRACKET, "RBRACKET", yytext, linha, coluna); ATUALIZA_COLUNA(); }
";"             { imprime_token(SEMICOLON,"SEMICOLON",yytext, linha, coluna); ATUALIZA_COLUNA(); }
","             { imprime_token(COMMA,    "COMMA",    yytext, linha, coluna); ATUALIZA_COLUNA(); }
"."             { imprime_token(DOT,      "DOT",      yytext, linha, coluna); ATUALIZA_COLUNA(); }

 /* ── Símbolos fora do escopo (erro léxico informativo) ────── */
"++"            { fprintf(stderr, "ERRO LÉXICO [linha %d, col %d]: '++' fora do escopo. Use '= x + 1'.\n", linha, coluna); ATUALIZA_COLUNA(); }
"--"            { fprintf(stderr, "ERRO LÉXICO [linha %d, col %d]: '--' fora do escopo. Use '= x - 1'.\n", linha, coluna); ATUALIZA_COLUNA(); }
"+="            { fprintf(stderr, "ERRO LÉXICO [linha %d, col %d]: '+=' fora do escopo.\n", linha, coluna); ATUALIZA_COLUNA(); }
"-="            { fprintf(stderr, "ERRO LÉXICO [linha %d, col %d]: '-=' fora do escopo.\n", linha, coluna); ATUALIZA_COLUNA(); }
"*="            { fprintf(stderr, "ERRO LÉXICO [linha %d, col %d]: '*=' fora do escopo.\n", linha, coluna); ATUALIZA_COLUNA(); }
"/="            { fprintf(stderr, "ERRO LÉXICO [linha %d, col %d]: '/=' fora do escopo.\n", linha, coluna); ATUALIZA_COLUNA(); }
"&"             { fprintf(stderr, "ERRO LÉXICO [linha %d, col %d]: '&' (endereço/bitwise) fora do escopo.\n", linha, coluna); ATUALIZA_COLUNA(); }
"|"             { fprintf(stderr, "ERRO LÉXICO [linha %d, col %d]: '|' (bitwise) fora do escopo.\n", linha, coluna); ATUALIZA_COLUNA(); }
"^"             { fprintf(stderr, "ERRO LÉXICO [linha %d, col %d]: '^' (bitwise) fora do escopo.\n", linha, coluna); ATUALIZA_COLUNA(); }
"~"             { fprintf(stderr, "ERRO LÉXICO [linha %d, col %d]: '~' (bitwise) fora do escopo.\n", linha, coluna); ATUALIZA_COLUNA(); }
"<<"            { fprintf(stderr, "ERRO LÉXICO [linha %d, col %d]: '<<' (bitwise) fora do escopo.\n", linha, coluna); ATUALIZA_COLUNA(); }
">>"            { fprintf(stderr, "ERRO LÉXICO [linha %d, col %d]: '>>' (bitwise) fora do escopo.\n", linha, coluna); ATUALIZA_COLUNA(); }
"?"             { fprintf(stderr, "ERRO LÉXICO [linha %d, col %d]: '?' (ternário) fora do escopo.\n", linha, coluna); ATUALIZA_COLUNA(); }

 /* ── Símbolo desconhecido (erro léxico genérico) ──────────── */
.               { fprintf(stderr, "ERRO LÉXICO [linha %d, col %d]: símbolo desconhecido '%s'.\n", linha, coluna, yytext); ATUALIZA_COLUNA(); }

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

    printf("%-12s %-12s %-20s %s\n", "LINHA", "COLUNA", "TOKEN", "LEXEMA");
    printf("%s\n", "------------------------------------------------------------");

    yylex();

    if (argc > 1) fclose(yyin);
    return 0;
}
