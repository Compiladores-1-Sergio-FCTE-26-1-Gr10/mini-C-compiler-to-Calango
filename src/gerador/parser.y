%{
/*
 * Gerador de Código — Mini C Compiler to Calango
 * Disciplina: FGA0003 – Compiladores 1 (2026/1)
 *
 * Faz análise semântica + constrói AST + gera código Calango.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tabela.h"
#include "ast.h"
#include "gerador.h"

extern int   yylex(void);
extern char *yytext;
extern int   linha;
extern int   coluna;
extern FILE *yyin;

void yyerror(const char *msg) {
    fprintf(stderr, "ERRO SINTÁTICO [linha %d, col %d]: %s (token: '%s')\n",
            linha, coluna, msg, yytext);
}

static char  tipo_atual[16];
static int   erros_sem = 0;
static No   *raiz_ast  = NULL;

static void erro_sem(int lin, const char *fmt, const char *nome) {
    fprintf(stderr, "ERRO SEMÂNTICO [linha %d]: ", lin);
    fprintf(stderr, fmt, nome);
    fprintf(stderr, "\n");
    erros_sem++;
}
%}

%union {
    int    intValue;
    float  floatValue;
    char   charValue;
    char   strValue[256];
    struct No *no;
}

%token <intValue>   LIT_INT LIT_TRUE LIT_FALSE
%token <floatValue> LIT_FLOAT
%token <charValue>  LIT_CHAR
%token <strValue>   LIT_STRING ID

%token KW_INT KW_FLOAT KW_CHAR KW_BOOL
%token KW_IF KW_ELSE KW_WHILE KW_FOR KW_DO
%token KW_PRINTF KW_SCANF KW_MAIN
%token PLUS MINUS TIMES DIVIDE MOD
%token EQ NEQ LT GT LEQ GEQ
%token AND OR NOT ASSIGN
%token LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET
%token SEMICOLON COMMA DOT

%type <no>       programa bloco declaracoes declaracao instrucoes instrucao
%type <no>       atribuicao condicional enquanto para faca_enquanto
%type <no>       saida entrada args_printf atribuicao_for expressao
%type <strValue> tipo

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

programa
    : KW_INT KW_MAIN LPAREN RPAREN LBRACE bloco RBRACE
        {
            No *n = ast_no(NO_PROGRAMA, linha);
            ast_filho(n, $6);
            raiz_ast = n;
            $$ = n;

            if (erros_sem > 0) {
                fprintf(stderr, "\n%d erro(s) semântico(s). Geração cancelada.\n", erros_sem);
            } else {
                gerarASTOtimizada(raiz_ast, stdout);
            }
        }
    ;

bloco
    : declaracoes instrucoes
        {
            No *n = ast_no(NO_BLOCO, linha);
            if ($1) ast_filho(n, $1);
            if ($2) ast_filho(n, $2);
            $$ = n;
        }
    ;

declaracoes
    : declaracoes declaracao
        {
            if ($1 == NULL) { $$ = $2; }
            else {
                No *u = $1; while (u->prox) u = u->prox; u->prox = $2;
                $$ = $1;
            }
        }
    | /* vazio */ { $$ = NULL; }
    ;

declaracao
    : tipo ID SEMICOLON
        {
            if (buscarSimboloEscopoAtual($2) != NULL)
                erro_sem(linha, "variável '%s' já declarada neste escopo.", $2);
            else
                inserirSimbolo($2, $1, linha);

            No *n = ast_no(NO_DECL, linha);
            No *t = ast_no(
                strcmp($1,"int")==0   ? NO_TIPO_INT   :
                strcmp($1,"float")==0 ? NO_TIPO_FLOAT :
                strcmp($1,"char")==0  ? NO_TIPO_CHAR  : NO_TIPO_BOOL, linha);
            ast_filho(n, t);
            ast_filho(n, ast_str(NO_ID, $2, linha));
            $$ = n;
        }
    | tipo ID ASSIGN expressao SEMICOLON
        {
            if (buscarSimboloEscopoAtual($2) != NULL)
                erro_sem(linha, "variável '%s' já declarada neste escopo.", $2);
            else {
                inserirSimbolo($2, $1, linha);
                marcarInicializado($2);
            }

            No *n = ast_no(NO_DECL, linha);
            No *t = ast_no(
                strcmp($1,"int")==0   ? NO_TIPO_INT   :
                strcmp($1,"float")==0 ? NO_TIPO_FLOAT :
                strcmp($1,"char")==0  ? NO_TIPO_CHAR  : NO_TIPO_BOOL, linha);
            No *a = ast_no(NO_ATRIB, linha);
            ast_filho(a, ast_str(NO_ID, $2, linha));
            ast_filho(a, $4);
            ast_filho(n, t);
            ast_filho(n, a);
            $$ = n;
        }
    ;

tipo
    : KW_INT   { strcpy(tipo_atual,"int");   strcpy($$,"int");   }
    | KW_FLOAT { strcpy(tipo_atual,"float"); strcpy($$,"float"); }
    | KW_CHAR  { strcpy(tipo_atual,"char");  strcpy($$,"char");  }
    | KW_BOOL  { strcpy(tipo_atual,"bool");  strcpy($$,"bool");  }
    ;

instrucoes
    : instrucoes instrucao
        {
            if ($1 == NULL) { $$ = $2; }
            else {
                No *u = $1; while (u->prox) u = u->prox; u->prox = $2;
                $$ = $1;
            }
        }
    | /* vazio */ { $$ = NULL; }
    ;

instrucao
    : atribuicao    { $$ = $1; }
    | condicional   { $$ = $1; }
    | enquanto      { $$ = $1; }
    | para          { $$ = $1; }
    | faca_enquanto { $$ = $1; }
    | saida         { $$ = $1; }
    | entrada       { $$ = $1; }
    ;

atribuicao
    : ID ASSIGN expressao SEMICOLON
        {
            Simbolo *s = buscarSimbolo($1);
            if (!s)
                erro_sem(linha, "variável '%s' não declarada.", $1);
            else { marcarInicializado($1); marcarUsado($1); }

            No *n = ast_no(NO_ATRIB, linha);
            ast_filho(n, ast_str(NO_ID, $1, linha));
            ast_filho(n, $3);
            $$ = n;
        }
    ;

condicional
    : KW_IF LPAREN expressao RPAREN LBRACE instrucoes RBRACE
        {
            No *n = ast_no(NO_IF, linha);
            ast_filho(n, $3); ast_filho(n, $6);
            $$ = n;
        }
    | KW_IF LPAREN expressao RPAREN LBRACE instrucoes RBRACE
      KW_ELSE LBRACE instrucoes RBRACE
        {
            No *n = ast_no(NO_IF_ELSE, linha);
            ast_filho(n, $3); ast_filho(n, $6); ast_filho(n, $10);
            $$ = n;
        }
    ;

enquanto
    : KW_WHILE LPAREN expressao RPAREN LBRACE instrucoes RBRACE
        {
            No *n = ast_no(NO_WHILE, linha);
            ast_filho(n, $3); ast_filho(n, $6);
            $$ = n;
        }
    ;

para
    : KW_FOR LPAREN atribuicao expressao SEMICOLON atribuicao_for RPAREN
      LBRACE instrucoes RBRACE
        {
            No *n = ast_no(NO_FOR, linha);
            ast_filho(n, $3); ast_filho(n, $4);
            ast_filho(n, $6); ast_filho(n, $9);
            $$ = n;
        }
    ;

atribuicao_for
    : ID ASSIGN expressao
        {
            Simbolo *s = buscarSimbolo($1);
            if (!s)
                erro_sem(linha, "variável '%s' não declarada.", $1);
            else { marcarInicializado($1); marcarUsado($1); }

            No *n = ast_no(NO_ATRIB, linha);
            ast_filho(n, ast_str(NO_ID, $1, linha));
            ast_filho(n, $3);
            $$ = n;
        }
    ;

faca_enquanto
    : KW_DO LBRACE instrucoes RBRACE
      KW_WHILE LPAREN expressao RPAREN SEMICOLON
        {
            No *n = ast_no(NO_DO_WHILE, linha);
            ast_filho(n, $3); ast_filho(n, $7);
            $$ = n;
        }
    ;

saida
    : KW_PRINTF LPAREN LIT_STRING RPAREN SEMICOLON
        {
            No *n = ast_no(NO_PRINTF, linha);
            ast_filho(n, ast_str(NO_LIT_STRING, $3, linha));
            $$ = n;
        }
    | KW_PRINTF LPAREN LIT_STRING COMMA args_printf RPAREN SEMICOLON
        {
            No *n = ast_no(NO_PRINTF, linha);
            ast_filho(n, ast_str(NO_LIT_STRING, $3, linha));
            ast_filho(n, $5);
            $$ = n;
        }
    ;

args_printf
    : expressao
        {
            No *n = ast_no(NO_ARGS, linha);
            ast_filho(n, $1);
            $$ = n;
        }
    | args_printf COMMA expressao
        {
            ast_filho($1, $3);
            $$ = $1;
        }
    ;

entrada
    : KW_SCANF LPAREN LIT_STRING COMMA ID RPAREN SEMICOLON
        {
            Simbolo *s = buscarSimbolo($5);
            if (!s)
                erro_sem(linha, "variável '%s' não declarada.", $5);
            else { marcarInicializado($5); marcarUsado($5); }

            No *n = ast_no(NO_SCANF, linha);
            ast_filho(n, ast_str(NO_LIT_STRING, $3, linha));
            ast_filho(n, ast_str(NO_ID, $5, linha));
            $$ = n;
        }
    ;

expressao
    : expressao OR     expressao { No*n=ast_no(NO_OP_OR,   linha);ast_filho(n,$1);ast_filho(n,$3);$$=n; }
    | expressao AND    expressao { No*n=ast_no(NO_OP_AND,  linha);ast_filho(n,$1);ast_filho(n,$3);$$=n; }
    | expressao EQ     expressao { No*n=ast_no(NO_OP_EQ,   linha);ast_filho(n,$1);ast_filho(n,$3);$$=n; }
    | expressao NEQ    expressao { No*n=ast_no(NO_OP_NEQ,  linha);ast_filho(n,$1);ast_filho(n,$3);$$=n; }
    | expressao LT     expressao { No*n=ast_no(NO_OP_LT,   linha);ast_filho(n,$1);ast_filho(n,$3);$$=n; }
    | expressao GT     expressao { No*n=ast_no(NO_OP_GT,   linha);ast_filho(n,$1);ast_filho(n,$3);$$=n; }
    | expressao LEQ    expressao { No*n=ast_no(NO_OP_LEQ,  linha);ast_filho(n,$1);ast_filho(n,$3);$$=n; }
    | expressao GEQ    expressao { No*n=ast_no(NO_OP_GEQ,  linha);ast_filho(n,$1);ast_filho(n,$3);$$=n; }
    | expressao PLUS   expressao { No*n=ast_no(NO_OP_MAIS, linha);ast_filho(n,$1);ast_filho(n,$3);$$=n; }
    | expressao MINUS  expressao { No*n=ast_no(NO_OP_MENOS,linha);ast_filho(n,$1);ast_filho(n,$3);$$=n; }
    | expressao TIMES  expressao { No*n=ast_no(NO_OP_MULT, linha);ast_filho(n,$1);ast_filho(n,$3);$$=n; }
    | expressao DIVIDE expressao { No*n=ast_no(NO_OP_DIV,  linha);ast_filho(n,$1);ast_filho(n,$3);$$=n; }
    | expressao MOD    expressao { No*n=ast_no(NO_OP_MOD,  linha);ast_filho(n,$1);ast_filho(n,$3);$$=n; }
    | NOT expressao              { No*n=ast_no(NO_OP_NOT,  linha);ast_filho(n,$2);$$=n; }
    | MINUS expressao %prec UMINUS { No*n=ast_no(NO_OP_NEG,linha);ast_filho(n,$2);$$=n; }
    | LPAREN expressao RPAREN    { $$=$2; }
    | ID
        {
            Simbolo *s = buscarSimbolo($1);
            if (!s)
                erro_sem(linha, "variável '%s' não declarada.", $1);
            else {
                if (!s->inicializado)
                    fprintf(stderr,
                        "AVISO SEMÂNTICO [linha %d]: '%s' usada sem inicialização.\n",
                        linha, $1);
                marcarUsado($1);
            }
            $$ = ast_str(NO_ID, $1, linha);
        }
    | LIT_INT    { $$ = ast_int($1,  linha); }
    | LIT_FLOAT  { $$ = ast_float($1,linha); }
    | LIT_CHAR   { $$ = ast_char($1, linha); }
    | LIT_STRING { $$ = ast_str(NO_LIT_STRING,$1,linha); }
    | LIT_TRUE   { $$ = ast_no(NO_LIT_TRUE,  linha); }
    | LIT_FALSE  { $$ = ast_no(NO_LIT_FALSE, linha); }
    ;

%%

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo.c>\n", argv[0]);
        return 1;
    }
    FILE *f = fopen(argv[1], "r");
    if (!f) { fprintf(stderr, "Erro: não foi possível abrir '%s'\n", argv[1]); return 1; }
    yyin = f;

    inicializarTabela();
    entrarEscopo();

    int res = yyparse();

    sairEscopo();
    liberarTabela();
    ast_libera(raiz_ast);
    fclose(f);
    return (res != 0 || erros_sem > 0) ? 1 : 0;
}
