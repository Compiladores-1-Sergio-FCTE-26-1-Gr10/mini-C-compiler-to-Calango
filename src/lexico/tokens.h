/*
 * tokens.h — Definição dos tokens para teste isolado do léxico
 * Mini C Compiler to Calango
 */
#ifndef TOKENS_H
#define TOKENS_H

/* Palavras reservadas */
#define KW_INT     256
#define KW_FLOAT   257
#define KW_CHAR    258
#define KW_BOOL    259
#define KW_IF      260
#define KW_ELSE    261
#define KW_WHILE   262
#define KW_FOR     263
#define KW_DO      264
#define KW_PRINTF  265
#define KW_SCANF   266
#define KW_MAIN    267

/* Literais */
#define LIT_INT    268
#define LIT_FLOAT  269
#define LIT_CHAR   270
#define LIT_STRING 271
#define LIT_TRUE   272
#define LIT_FALSE  273

/* Identificador */
#define ID         274

/* Operadores aritméticos */
#define PLUS       275
#define MINUS      276
#define TIMES      277
#define DIVIDE     278
#define MOD        279

/* Operadores relacionais */
#define EQ         280
#define NEQ        281
#define LT         282
#define GT         283
#define LEQ        284
#define GEQ        285

/* Operadores lógicos */
#define AND        286
#define OR         287
#define NOT        288

/* Atribuição */
#define ASSIGN     289

/* Delimitadores */
#define LPAREN     290
#define RPAREN     291
#define LBRACE     292
#define RBRACE     293
#define LBRACKET   294
#define RBRACKET   295
#define SEMICOLON  296
#define COMMA      297
#define DOT        298

/* Nomes legíveis para impressão */
static const char *nome_token(int tok) {
    switch (tok) {
        case KW_INT:     return "KW_INT";
        case KW_FLOAT:   return "KW_FLOAT";
        case KW_CHAR:    return "KW_CHAR";
        case KW_BOOL:    return "KW_BOOL";
        case KW_IF:      return "KW_IF";
        case KW_ELSE:    return "KW_ELSE";
        case KW_WHILE:   return "KW_WHILE";
        case KW_FOR:     return "KW_FOR";
        case KW_DO:      return "KW_DO";
        case KW_PRINTF:  return "KW_PRINTF";
        case KW_SCANF:   return "KW_SCANF";
        case KW_MAIN:    return "KW_MAIN";
        case LIT_INT:    return "LIT_INT";
        case LIT_FLOAT:  return "LIT_FLOAT";
        case LIT_CHAR:   return "LIT_CHAR";
        case LIT_STRING: return "LIT_STRING";
        case LIT_TRUE:   return "LIT_TRUE";
        case LIT_FALSE:  return "LIT_FALSE";
        case ID:         return "ID";
        case PLUS:       return "PLUS";
        case MINUS:      return "MINUS";
        case TIMES:      return "TIMES";
        case DIVIDE:     return "DIVIDE";
        case MOD:        return "MOD";
        case EQ:         return "EQ";
        case NEQ:        return "NEQ";
        case LT:         return "LT";
        case GT:         return "GT";
        case LEQ:        return "LEQ";
        case GEQ:        return "GEQ";
        case AND:        return "AND";
        case OR:         return "OR";
        case NOT:        return "NOT";
        case ASSIGN:     return "ASSIGN";
        case LPAREN:     return "LPAREN";
        case RPAREN:     return "RPAREN";
        case LBRACE:     return "LBRACE";
        case RBRACE:     return "RBRACE";
        case LBRACKET:   return "LBRACKET";
        case RBRACKET:   return "RBRACKET";
        case SEMICOLON:  return "SEMICOLON";
        case COMMA:      return "COMMA";
        case DOT:        return "DOT";
        default:         return "DESCONHECIDO";
    }
}

#endif /* TOKENS_H */
