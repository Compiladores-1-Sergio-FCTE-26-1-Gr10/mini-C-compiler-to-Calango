# ══════════════════════════════════════════════════════════════════════
# Makefile — Mini C Compiler to Calango
# Disciplina: FGA0003 – Compiladores 1 (2026/1)
#
# Alvos:
#   make            → compila todas as fases
#   make lexico     → compila só o analisador léxico
#   make sintatico  → compila só o analisador sintático
#   make semantico  → compila só o analisador semântico + AST
#   make gerador    → compila o gerador de código Calango
#   make test       → roda todos os testes
#   make test-lex   → testes do léxico
#   make test-sint  → testes do sintático
#   make test-sem   → testes do semântico
#   make test-ger   → testes do gerador
#   make clean      → remove arquivos gerados
# ══════════════════════════════════════════════════════════════════════

CC    = gcc
FLEX  = flex
BISON = bison
CFLAGS = -Wall -Wno-unused-function -g

LEX_DIR  = src/lexico
SINT_DIR = src/sintatico
SEM_DIR  = src/semantico
GER_DIR  = src/gerador

TEST_LEX  = testes/lexico
TEST_SINT = testes/sintatico
TEST_SEM  = testes/semantico
TEST_GER  = testes/gerador

# ── Alvos principais ──────────────────────────────────────────────────
all: lexico sintatico semantico gerador

# ─────────────────────────────────────────────────────────────────────
# LÉXICO
# ─────────────────────────────────────────────────────────────────────
lexico: minic_lexico

minic_lexico: $(LEX_DIR)/lex.yy.c
	$(CC) $(CFLAGS) -DSTANDALONE $(LEX_DIR)/lex.yy.c -o minic_lexico -lfl

$(LEX_DIR)/lex.yy.c: $(LEX_DIR)/lexer.l
	$(FLEX) -o $(LEX_DIR)/lex.yy.c $(LEX_DIR)/lexer.l

# ─────────────────────────────────────────────────────────────────────
# SINTÁTICO
# ─────────────────────────────────────────────────────────────────────
sintatico: minic_sintatico

minic_sintatico: $(SINT_DIR)/parser.tab.c $(SINT_DIR)/lex.yy.c
	$(CC) $(CFLAGS) \
	    $(SINT_DIR)/parser.tab.c \
	    $(SINT_DIR)/lex.yy.c \
	    -o minic_sintatico

$(SINT_DIR)/parser.tab.c $(SINT_DIR)/parser.tab.h: $(SINT_DIR)/parser.y
	$(BISON) -d -o $(SINT_DIR)/parser.tab.c $(SINT_DIR)/parser.y

$(SINT_DIR)/lex.yy.c: $(SINT_DIR)/lexer.l $(SINT_DIR)/parser.tab.h
	$(FLEX) -o $(SINT_DIR)/lex.yy.c $(SINT_DIR)/lexer.l

# ─────────────────────────────────────────────────────────────────────
# SEMÂNTICO
# ─────────────────────────────────────────────────────────────────────
semantico: minic_semantico

minic_semantico: $(SEM_DIR)/parser.tab.c $(SEM_DIR)/lex.yy.c \
                 $(SEM_DIR)/tabela.c $(SEM_DIR)/ast.c
	$(CC) $(CFLAGS) \
	    $(SEM_DIR)/parser.tab.c \
	    $(SEM_DIR)/lex.yy.c \
	    $(SEM_DIR)/tabela.c \
	    $(SEM_DIR)/ast.c \
	    -o minic_semantico

$(SEM_DIR)/parser.tab.c $(SEM_DIR)/parser.tab.h: $(SEM_DIR)/parser.y
	$(BISON) -d -o $(SEM_DIR)/parser.tab.c $(SEM_DIR)/parser.y

$(SEM_DIR)/lex.yy.c: $(SEM_DIR)/lexer.l $(SEM_DIR)/parser.tab.h
	$(FLEX) -o $(SEM_DIR)/lex.yy.c $(SEM_DIR)/lexer.l

# ─────────────────────────────────────────────────────────────────────
# GERADOR DE CÓDIGO CALANGO
# ─────────────────────────────────────────────────────────────────────
gerador: minic_gerador

minic_gerador: $(GER_DIR)/parser.tab.c $(GER_DIR)/lex.yy.c \
               $(GER_DIR)/tabela.c $(GER_DIR)/ast.c $(GER_DIR)/gerador.c
	$(CC) $(CFLAGS) \
	    $(GER_DIR)/parser.tab.c \
	    $(GER_DIR)/lex.yy.c \
	    $(GER_DIR)/tabela.c \
	    $(GER_DIR)/ast.c \
	    $(GER_DIR)/gerador.c \
	    -o minic_gerador

$(GER_DIR)/parser.tab.c $(GER_DIR)/parser.tab.h: $(GER_DIR)/parser.y
	$(BISON) -d -o $(GER_DIR)/parser.tab.c $(GER_DIR)/parser.y

$(GER_DIR)/lex.yy.c: $(GER_DIR)/lexer.l $(GER_DIR)/parser.tab.h
	$(FLEX) -o $(GER_DIR)/lex.yy.c $(GER_DIR)/lexer.l

# ─────────────────────────────────────────────────────────────────────
# TESTES
# ─────────────────────────────────────────────────────────────────────
test: test-lex test-sint test-sem test-ger

test-lex: minic_lexico
	@$(call rodar_testes,$(TEST_LEX),./minic_lexico,"Léxico")

test-sint: minic_sintatico
	@$(call rodar_testes,$(TEST_SINT),./minic_sintatico,"Sintático")

test-sem: minic_semantico
	@$(call rodar_testes,$(TEST_SEM),./minic_semantico,"Semântico")

test-ger: minic_gerador
	@$(call rodar_testes,$(TEST_GER),./minic_gerador,"Gerador")

define rodar_testes
	echo ""; \
	echo "══════════════════════════════════════════"; \
	echo "  Testes $(3)"; \
	echo "══════════════════════════════════════════"; \
	PASS=0; FAIL=0; \
	for t in $(1)/*.c; do \
	    [ -f "$$t" ] || continue; \
	    base=$$(basename $$t .c); \
	    esperado=$(1)/$$base.esperado; \
	    echo ""; \
	    echo "  ── $$t"; \
	    if [ -f "$$esperado" ]; then \
	        saida=$$($(2) $$t 2>&1); \
	        if echo "$$saida" | grep -qF "$$(cat $$esperado)"; then \
	            echo "    PASSOU"; PASS=$$((PASS+1)); \
	        else \
	            echo "    FALHOU"; \
	            echo "    Esperado: $$(cat $$esperado)"; \
	            echo "    Obtido  : $$(echo "$$saida" | head -3)"; \
	            FAIL=$$((FAIL+1)); \
	        fi; \
	    else \
	        $(2) $$t > /dev/null 2>&1; \
	        if [ $$? -eq 0 ]; then echo "    PASSOU"; PASS=$$((PASS+1)); \
	        else echo "    FALHOU"; FAIL=$$((FAIL+1)); fi; \
	    fi; \
	done; \
	echo ""; \
	echo "  Resultado: $$PASS passou(aram), $$FAIL falhou(aram)"; \
	echo "══════════════════════════════════════════"
endef

# ─────────────────────────────────────────────────────────────────────
clean:
	rm -f minic_lexico minic_sintatico minic_semantico minic_gerador
	rm -f $(LEX_DIR)/lex.yy.c
	rm -f $(SINT_DIR)/lex.yy.c $(SINT_DIR)/parser.tab.c $(SINT_DIR)/parser.tab.h
	rm -f $(SEM_DIR)/lex.yy.c  $(SEM_DIR)/parser.tab.c  $(SEM_DIR)/parser.tab.h
	rm -f $(GER_DIR)/lex.yy.c  $(GER_DIR)/parser.tab.c  $(GER_DIR)/parser.tab.h

.PHONY: all lexico sintatico semantico gerador test test-lex test-sint test-sem test-ger clean
