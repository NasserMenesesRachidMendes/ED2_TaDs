CC     := gcc
CFLAGS := -Wall -Wextra -std=c11 -g -Isrc
FONTES := $(wildcard src/*.c src/biblioteca/estruturas/*.c)
ALVO   := build/programa.exe

# No Windows forcamos o cmd.exe como shell do make. Sem isso o comportamento
# muda conforme o terminal usado: chamado do Git Bash o make acha 'rm' e
# 'mkdir -p'; chamado do PowerShell, nao acha, e o build quebra.
ifeq ($(OS),Windows_NT)

SHELL       := cmd.exe
.SHELLFLAGS := /c

all: $(ALVO)

$(ALVO): $(FONTES)
	-@mkdir build 2>nul
	$(CC) $(CFLAGS) $(FONTES) -o $(ALVO)

run: $(ALVO)
	@build\programa.exe

clean:
	-@rmdir /s /q build 2>nul

else

all: $(ALVO)

$(ALVO): $(FONTES)
	@mkdir -p build
	$(CC) $(CFLAGS) $(FONTES) -o $(ALVO)

run: $(ALVO)
	@./$(ALVO)

clean:
	rm -rf build

endif

.PHONY: all run clean
