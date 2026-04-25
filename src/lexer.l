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
 *   - Declarações apenas no início do bloco main
 *
 * Restrições conhecidas (fora do escopo):
 *   - Funções além do main
 *   - Arrays
 *   - Cast explícito
 *   - Atribuição composta (+=, -=, *=, /=)
 *   - Incremento/decremento (++, --)
 *   - Ponteiros, bitwise, ternário
 *
 * Mapeamento Mini C → Calango (referência para geração de código):
 *   int       → inteiro         float  → real
 *   char      → caracter        bool   → logico
 *   if/else   → se...entao/senao
 *   while     → enquanto...faca
 *   for       → para...faca
 *   do-while  → faca...enquanto
 *   printf    → escreva/escreval
 *   scanf     → leia/leiaCaracter
 *   &&  → e      ||  → ou      !  → nao
 *   %   → mod    true → VERDADEIRO   false → FALSO
 */

#include "parser_proj.tab.h"  /* tokens gerados pelo Bison */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int linha  = 1;
int coluna = 1;

#define ATUALIZA_COLUNA() coluna += yyleng
%}

%option noyywrap

/* ── Definições (macros de regex) ───────────────────────────── */
DIGITO      [0-9]
LETRA       [a-zA-Z_]
ALFANUM     [a-zA-Z0-9_]

NUM_INT     {DIGITO}+
NUM_FLOAT   {DIGITO}+\.{DIGITO}+

LIT_CHAR    '([^'\\]|\\.)'
LIT_STRING  \"([^\"\\]|\\.)*\"

ID          {LETRA}{ALFANUM}*

BRANCO      [ \t\r]+
COM_LINHA   \/\/[^\n]*
COM_BLOCO   \/\*([^*]|\*+[^*/])*\*+\/

%%

 /* ── Comentários (descartar) ──────────────────────────────── */
{COM_LINHA}     { ATUALIZA_COLUNA(); }
{COM_BLOCO}     {
                    for (int i = 0; i < yyleng; i++) {
                        if (yytext[i] == '\n') { linha++; coluna = 1; }
                        else coluna++;
                    }
                }

 /* ── Espaços (descartar) ──────────────────────────────────── */
{BRANCO}        { ATUALIZA_COLUNA(); }
\n              { linha++; coluna = 1; }

 /* ── Palavras reservadas ──────────────────────────────────── */
"int"           { ATUALIZA_COLUNA(); return KW_INT;    }
"float"         { ATUALIZA_COLUNA(); return KW_FLOAT;  }
"char"          { ATUALIZA_COLUNA(); return KW_CHAR;   }
"bool"          { ATUALIZA_COLUNA(); return KW_BOOL;   }
"if"            { ATUALIZA_COLUNA(); return KW_IF;     }
"else"          { ATUALIZA_COLUNA(); return KW_ELSE;   }
"while"         { ATUALIZA_COLUNA(); return KW_WHILE;  }
"for"           { ATUALIZA_COLUNA(); return KW_FOR;    }
"do"            { ATUALIZA_COLUNA(); return KW_DO;     }
"printf"        { ATUALIZA_COLUNA(); return KW_PRINTF; }
"scanf"         { ATUALIZA_COLUNA(); return KW_SCANF;  }
"main"          { ATUALIZA_COLUNA(); return KW_MAIN;   }
"true"          { ATUALIZA_COLUNA(); return LIT_TRUE;  }
"false"         { ATUALIZA_COLUNA(); return LIT_FALSE; }

 /* ── Literais numéricos (FLOAT antes de INT) ──────────────── */
{NUM_FLOAT}     { ATUALIZA_COLUNA(); return LIT_FLOAT; }
{NUM_INT}       { ATUALIZA_COLUNA(); return LIT_INT;   }

 /* ── Literal char ─────────────────────────────────────────── */
{LIT_CHAR}      { ATUALIZA_COLUNA(); return LIT_CHAR;   }

 /* ── Literal string ───────────────────────────────────────── */
{LIT_STRING}    { ATUALIZA_COLUNA(); return LIT_STRING; }

 /* ── Identificador ────────────────────────────────────────── */
{ID}            { ATUALIZA_COLUNA(); return ID; }

 /* ── Operadores aritméticos ───────────────────────────────── */
"+"             { ATUALIZA_COLUNA(); return PLUS;   }
"-"             { ATUALIZA_COLUNA(); return MINUS;  }
"*"             { ATUALIZA_COLUNA(); return TIMES;  }
"/"             { ATUALIZA_COLUNA(); return DIVIDE; }
"%"             { ATUALIZA_COLUNA(); return MOD;    }

 /* ── Operadores relacionais (2 chars antes de 1 char) ─────── */
"=="            { ATUALIZA_COLUNA(); return EQ;  }
"!="            { ATUALIZA_COLUNA(); return NEQ; }
"<="            { ATUALIZA_COLUNA(); return LEQ; }
">="            { ATUALIZA_COLUNA(); return GEQ; }
"<"             { ATUALIZA_COLUNA(); return LT;  }
">"             { ATUALIZA_COLUNA(); return GT;  }

 /* ── Operadores lógicos ───────────────────────────────────── */
"&&"            { ATUALIZA_COLUNA(); return AND; }
"||"            { ATUALIZA_COLUNA(); return OR;  }
"!"             { ATUALIZA_COLUNA(); return NOT; }

 /* ── Atribuição ───────────────────────────────────────────── */
"="             { ATUALIZA_COLUNA(); return ASSIGN; }

 /* ── Delimitadores ────────────────────────────────────────── */
"("             { ATUALIZA_COLUNA(); return LPAREN;    }
")"             { ATUALIZA_COLUNA(); return RPAREN;    }
"{"             { ATUALIZA_COLUNA(); return LBRACE;    }
"}"             { ATUALIZA_COLUNA(); return RBRACE;    }
"["             { ATUALIZA_COLUNA(); return LBRACKET;  }
"]"             { ATUALIZA_COLUNA(); return RBRACKET;  }
";"             { ATUALIZA_COLUNA(); return SEMICOLON; }
","             { ATUALIZA_COLUNA(); return COMMA;     }
"."             { ATUALIZA_COLUNA(); return DOT;       }

 /* ── Símbolos fora do escopo (erro léxico informativo) ────── */
"++"            { fprintf(stderr, "ERRO LÉXICO [linha %d, col %d]: '++' fora do escopo. Use 'x = x + 1'.\n", linha, coluna); ATUALIZA_COLUNA(); }
"--"            { fprintf(stderr, "ERRO LÉXICO [linha %d, col %d]: '--' fora do escopo. Use 'x = x - 1'.\n", linha, coluna); ATUALIZA_COLUNA(); }
"+="            { fprintf(stderr, "ERRO LÉXICO [linha %d, col %d]: '+=' fora do escopo.\n", linha, coluna); ATUALIZA_COLUNA(); }
"-="            { fprintf(stderr, "ERRO LÉXICO [linha %d, col %d]: '-=' fora do escopo.\n", linha, coluna); ATUALIZA_COLUNA(); }
"*="            { fprintf(stderr, "ERRO LÉXICO [linha %d, col %d]: '*=' fora do escopo.\n", linha, coluna); ATUALIZA_COLUNA(); }
"/="            { fprintf(stderr, "ERRO LÉXICO [linha %d, col %d]: '/=' fora do escopo.\n", linha, coluna); ATUALIZA_COLUNA(); }
"&"             { fprintf(stderr, "ERRO LÉXICO [linha %d, col %d]: '&' fora do escopo.\n", linha, coluna); ATUALIZA_COLUNA(); }
"|"             { fprintf(stderr, "ERRO LÉXICO [linha %d, col %d]: '|' fora do escopo.\n", linha, coluna); ATUALIZA_COLUNA(); }
"^"             { fprintf(stderr, "ERRO LÉXICO [linha %d, col %d]: '^' fora do escopo.\n", linha, coluna); ATUALIZA_COLUNA(); }
"~"             { fprintf(stderr, "ERRO LÉXICO [linha %d, col %d]: '~' fora do escopo.\n", linha, coluna); ATUALIZA_COLUNA(); }
"<<"            { fprintf(stderr, "ERRO LÉXICO [linha %d, col %d]: '<<' fora do escopo.\n", linha, coluna); ATUALIZA_COLUNA(); }
">>"            { fprintf(stderr, "ERRO LÉXICO [linha %d, col %d]: '>>' fora do escopo.\n", linha, coluna); ATUALIZA_COLUNA(); }
"?"             { fprintf(stderr, "ERRO LÉXICO [linha %d, col %d]: '?' fora do escopo.\n", linha, coluna); ATUALIZA_COLUNA(); }

 /* ── Símbolo desconhecido ─────────────────────────────────── */
.               { fprintf(stderr, "ERRO LÉXICO [linha %d, col %d]: símbolo desconhecido '%s'.\n", linha, coluna, yytext); ATUALIZA_COLUNA(); }

%%