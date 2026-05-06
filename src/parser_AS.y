%{
/*
 * Analisador Sintático + Semântico — Mini C Compiler to Calango
 * Disciplina: FGA0003 – Compiladores 1 (2026/1)
 * Professor: Dr. Sergio Antônio Andrade de Freitas
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int   yylex();
extern char *yytext;
extern int   linha;
extern int   coluna;
extern FILE *yyin;

void yyerror(const char *msg) {
    fprintf(stderr, "ERRO SINTÁTICO [linha %d, col %d]: %s (token: '%s')\n",
            linha, coluna, msg, yytext);
}

/* ══════════════════════════════════════════════════════════════
   TABELA DE SÍMBOLOS
   ══════════════════════════════════════════════════════════════ */

#define MAX_SIMBOLOS 256

typedef enum { TIPO_INT, TIPO_FLOAT, TIPO_CHAR, TIPO_BOOL, TIPO_INVALIDO } TipoDado;

typedef struct {
    char     nome[64];
    TipoDado tipo;
    int      inicializado;
    int      linha_decl;
} Simbolo;

static Simbolo tabela[MAX_SIMBOLOS];
static int     qtd = 0;

static const char *tipo_str(TipoDado t) {
    switch(t) {
        case TIPO_INT:   return "int";
        case TIPO_FLOAT: return "float";
        case TIPO_CHAR:  return "char";
        case TIPO_BOOL:  return "bool";
        default:         return "invalido";
    }
}

static int tabela_insere(const char *nome, TipoDado tipo, int init, int lin) {
    for (int i = 0; i < qtd; i++) {
        if (strcmp(tabela[i].nome, nome) == 0) {
            fprintf(stderr,
                "ERRO SEMÂNTICO [linha %d]: variável '%s' já foi declarada na linha %d.\n",
                lin, nome, tabela[i].linha_decl);
            return -1;
        }
    }
    strncpy(tabela[qtd].nome, nome, 63);
    tabela[qtd].tipo         = tipo;
    tabela[qtd].inicializado = init;
    tabela[qtd].linha_decl   = lin;
    qtd++;
    return 0;
}

static Simbolo *tabela_busca(const char *nome) {
    for (int i = 0; i < qtd; i++)
        if (strcmp(tabela[i].nome, nome) == 0)
            return &tabela[i];
    return NULL;
}

static void tabela_imprime() {
    printf("\n╔══════════════════════════════════════════════════╗\n");
    printf("║              TABELA DE SÍMBOLOS                 ║\n");
    printf("╠══════════════╦════════════╦═════════════╦═══════╣\n");
    printf("║ %-12s ║ %-10s ║ %-11s ║ %-5s ║\n",
           "NOME", "TIPO", "INICIALIZADO", "LINHA");
    printf("╠══════════════╬════════════╬═════════════╬═══════╣\n");
    for (int i = 0; i < qtd; i++) {
        printf("║ %-12s ║ %-10s ║ %-11s ║ %-5d ║\n",
               tabela[i].nome,
               tipo_str(tabela[i].tipo),
               tabela[i].inicializado ? "sim" : "não",
               tabela[i].linha_decl);
    }
    printf("╚══════════════╩════════════╩═════════════╩═══════╝\n");
}

/* Tipo sendo declarado no momento (passado entre regras) */
static TipoDado tipo_atual = TIPO_INVALIDO;
%}

/* ── Union: valores semânticos dos tokens ───────────────────── */
%union {
    int   intValue;
    float floatValue;
    char  charValue;
    char  strValue[128];
}

/* ── Tokens com valor semântico ─────────────────────────────── */
%token <intValue>   LIT_INT LIT_TRUE LIT_FALSE
%token <floatValue> LIT_FLOAT
%token <charValue>  LIT_CHAR
%token <strValue>   LIT_STRING ID

/* ── Tokens sem valor semântico ─────────────────────────────── */
%token KW_INT KW_FLOAT KW_CHAR KW_BOOL
%token KW_IF KW_ELSE KW_WHILE KW_FOR KW_DO
%token KW_PRINTF KW_SCANF KW_MAIN
%token PLUS MINUS TIMES DIVIDE MOD
%token EQ NEQ LT GT LEQ GEQ
%token AND OR NOT ASSIGN
%token LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET
%token SEMICOLON COMMA DOT

/* ── Tipos das regras com valor semântico ───────────────────── */
%type <strValue> expressao

/* ── Precedência ────────────────────────────────────────────── */
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
   ESTRUTURA GERAL
   ══════════════════════════════════════════════════════════════ */

programa
    : KW_INT KW_MAIN LPAREN RPAREN LBRACE bloco RBRACE
        {
            printf("\nAnálise concluída: programa válido.\n");
            tabela_imprime();
        }
    ;

bloco
    : declaracoes instrucoes
    ;

/* ══════════════════════════════════════════════════════════════
   DECLARAÇÕES
   ══════════════════════════════════════════════════════════════ */

declaracoes
    : declaracoes declaracao
    | /* vazio */
    ;

declaracao
    : tipo ID SEMICOLON
        {
            /* Insere na tabela sem valor inicial */
            tabela_insere($2, tipo_atual, 0, linha);
        }
    | tipo ID ASSIGN expressao SEMICOLON
        {
            /* Insere na tabela com valor inicial */
            tabela_insere($2, tipo_atual, 1, linha);
        }
    ;

tipo
    : KW_INT   { tipo_atual = TIPO_INT;   }
    | KW_FLOAT { tipo_atual = TIPO_FLOAT; }
    | KW_CHAR  { tipo_atual = TIPO_CHAR;  }
    | KW_BOOL  { tipo_atual = TIPO_BOOL;  }
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

atribuicao
    : ID ASSIGN expressao SEMICOLON
        {
            /* Verifica se variável foi declarada */
            Simbolo *s = tabela_busca($1);
            if (!s) {
                fprintf(stderr,
                    "ERRO SEMÂNTICO [linha %d]: variável '%s' não declarada.\n",
                    linha, $1);
            } else {
                s->inicializado = 1;
            }
        }
    ;

condicional
    : KW_IF LPAREN expressao RPAREN LBRACE instrucoes RBRACE
    | KW_IF LPAREN expressao RPAREN LBRACE instrucoes RBRACE
      KW_ELSE LBRACE instrucoes RBRACE
    ;

enquanto
    : KW_WHILE LPAREN expressao RPAREN LBRACE instrucoes RBRACE
    ;

para
    : KW_FOR LPAREN atribuicao expressao SEMICOLON atribuicao_for RPAREN
      LBRACE instrucoes RBRACE
    ;

atribuicao_for
    : ID ASSIGN expressao
        {
            Simbolo *s = tabela_busca($1);
            if (!s)
                fprintf(stderr,
                    "ERRO SEMÂNTICO [linha %d]: variável '%s' não declarada.\n",
                    linha, $1);
        }
    ;

faca_enquanto
    : KW_DO LBRACE instrucoes RBRACE
      KW_WHILE LPAREN expressao RPAREN SEMICOLON
    ;

saida
    : KW_PRINTF LPAREN LIT_STRING RPAREN SEMICOLON
    | KW_PRINTF LPAREN LIT_STRING COMMA args_printf RPAREN SEMICOLON
    ;

args_printf
    : expressao
    | args_printf COMMA expressao
    ;

entrada
    : KW_SCANF LPAREN LIT_STRING COMMA ID RPAREN SEMICOLON
        {
            Simbolo *s = tabela_busca($5);
            if (!s)
                fprintf(stderr,
                    "ERRO SEMÂNTICO [linha %d]: variável '%s' não declarada.\n",
                    linha, $5);
            else
                s->inicializado = 1;
        }
    ;

/* ══════════════════════════════════════════════════════════════
   EXPRESSÕES
   (retornam string com o tipo inferido para avisos semânticos)
   ══════════════════════════════════════════════════════════════ */

expressao
    : expressao OR     expressao  { strcpy($$, "bool"); }
    | expressao AND    expressao  { strcpy($$, "bool"); }
    | expressao EQ     expressao  { strcpy($$, "bool"); }
    | expressao NEQ    expressao  { strcpy($$, "bool"); }
    | expressao LT     expressao  { strcpy($$, "bool"); }
    | expressao GT     expressao  { strcpy($$, "bool"); }
    | expressao LEQ    expressao  { strcpy($$, "bool"); }
    | expressao GEQ    expressao  { strcpy($$, "bool"); }
    | expressao PLUS   expressao  { strcpy($$, strcmp($1,"float")==0||strcmp($3,"float")==0 ? "float" : "int"); }
    | expressao MINUS  expressao  { strcpy($$, strcmp($1,"float")==0||strcmp($3,"float")==0 ? "float" : "int"); }
    | expressao TIMES  expressao  { strcpy($$, strcmp($1,"float")==0||strcmp($3,"float")==0 ? "float" : "int"); }
    | expressao DIVIDE expressao  { strcpy($$, strcmp($1,"float")==0||strcmp($3,"float")==0 ? "float" : "int"); }
    | expressao MOD    expressao  { strcpy($$, "int"); }
    | NOT expressao               { strcpy($$, "bool"); }
    | MINUS expressao %prec UMINUS { strcpy($$, $2); }
    | LPAREN expressao RPAREN     { strcpy($$, $2); }
    | ID
        {
            /* Verifica se variável foi declarada */
            Simbolo *s = tabela_busca($1);
            if (!s) {
                fprintf(stderr,
                    "ERRO SEMÂNTICO [linha %d]: variável '%s' não declarada.\n",
                    linha, $1);
                strcpy($$, "invalido");
            } else {
                strcpy($$, tipo_str(s->tipo));
            }
        }
    | LIT_INT    { strcpy($$, "int");   }
    | LIT_FLOAT  { strcpy($$, "float"); }
    | LIT_CHAR   { strcpy($$, "char");  }
    | LIT_STRING { strcpy($$, "string");}
    | LIT_TRUE   { strcpy($$, "bool");  }
    | LIT_FALSE  { strcpy($$, "bool");  }
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