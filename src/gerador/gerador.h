#ifndef GERADOR_H
#define GERADOR_H

/*
 * Gerador de Código — Mini C → Calango
 * Disciplina: FGA0003 – Compiladores 1 (2026/1)
 *
 * Percorre a AST e emite código Calango para um FILE*.
 *
 * Mapeamento principal:
 *   int        → inteiro        float    → real
 *   char       → caracter       bool     → logico
 *   if/else    → se/entao/senao/fimse
 *   while      → enquanto/faca/fimenquanto
 *   for        → para/faca/fimpara
 *   do-while   → faca/enquanto
 *   printf     → escreva / escreval
 *   scanf      → leia / leiaCaracter
 *   &&         → e               ||  → ou       ! → nao
 *   %          → mod             true → VERDADEIRO  false → FALSO
 *   / (int)    → \               / (real) → /
 */

#include "ast.h"
#include <stdio.h>

/* Gera código Calango a partir da AST, escrevendo em `saida` */
void gerar(No *raiz, FILE *saida);

/* Otimiza a AST antes da geração e emite o código Calango final. */
void gerarASTOtimizada(No *raiz, FILE *saida);

#endif /* GERADOR_H */