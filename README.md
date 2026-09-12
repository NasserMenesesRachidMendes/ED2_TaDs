# ED2_TaDs

Trabalho de estrutura de dados 2.

Implementações próprias de pilha, fila e lista encadeada em C, com alocação
dinâmica. O `main.c` só orquestra: chama as operações dos módulos em
`src/biblioteca/estruturas/`.

```
src/
  main.c                      ponto de entrada
  biblioteca/estruturas/
    pilha.c   pilha.h         LIFO
    fila.c    fila.h          FIFO
    lista.c   lista.h         lista encadeada
    grafo.c   grafo.h         esqueleto, ainda sem implementação
```

## Como compilar e rodar

Com o ambiente pronto (veja abaixo), na raiz do projeto:

```
mingw32-make        # Windows
make                # Linux e macOS
```

Isso gera `build/programa.exe`. Para compilar e executar de uma vez:

```
mingw32-make run
```

No VS Code, `Ctrl+Shift+B` faz o mesmo, e `F5` roda com o depurador.

Um detalhe que costuma pegar quem está começando: o projeto tem vários `.c`, e
todos precisam ser passados ao compilador de uma vez. Compilar só o `main.c`
resulta em dezenas de erros de *undefined reference* / *símbolo externo não
resolvido* — o `#include` traz só as declarações dos `.h`, enquanto o corpo das
funções está nos outros `.c`, e é o linker que junta tudo. O Makefile já cuida
disso; não compile arquivo por arquivo.

## Preparando o ambiente no Windows

São quatro passos. O terceiro é o que mais dá problema, então leia com calma.

### 1. Instalar o MSYS2

Baixe e instale de <https://www.msys2.org>. Aceite o caminho padrão,
`C:\msys64`.

### 2. Instalar compilador, make e depurador

Abra o menu Iniciar e procure por **MSYS2 UCRT64** — é importante ser essa, não
a "MSYS" nem a "MINGW64". Na janela que abrir:

```bash
pacman -Syu
```

Essa primeira atualização pode fechar a janela sozinha no meio; se fechar, é
esperado — abra de novo e rode o mesmo comando mais uma vez. Depois:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc \
                   mingw-w64-ucrt-x86_64-make \
                   mingw-w64-ucrt-x86_64-gdb
```

O prefixo `mingw-w64-ucrt-x86_64-` não é decoração. O MSYS2 mantém ambientes
paralelos (UCRT64, MINGW64, CLANG64, MSYS), cada um com sua própria cópia dos
pacotes em pastas diferentes. Esse prefixo instala em `C:\msys64\ucrt64\bin`,
que é a pasta do passo 3. Se instalar `pacman -S make gdb` sem prefixo, vem a
versão MSYS, que vai parar em `C:\msys64\usr\bin` e não vai ser encontrada.

### 3. Colocar no PATH

Sem isso, o Windows não acha o `gcc` mesmo estando instalado.

1. Tecla Windows, digite `variáveis de ambiente`, abra **"Editar as variáveis de
   ambiente para a sua conta"**.
2. No quadro **de cima** ("Variáveis de usuário"), selecione `Path` →
   **Editar**.
3. **Novo** → cole `C:\msys64\ucrt64\bin`.
4. **OK** em todas as janelas abertas, uma por uma. Isso importa: é o OK que faz
   o Windows avisar os programas já abertos sobre a mudança. Se fechar com
   Cancelar ou pelo X, a alteração fica gravada mas ninguém fica sabendo dela.
5. Feche **todos** os Prompts de Comando e janelas do VS Code, e abra de novo.

O passo 5 não é opcional. No Windows, um programa recebe uma cópia do PATH
quando é iniciado e nunca mais atualiza. Um Prompt aberto antes da alteração vai
continuar dizendo que `gcc` não existe — e um aberto *depois* também, se tiver
sido aberto pelo Explorer, que ainda está com a cópia antiga. Se depois dos OKs
ainda não funcionar, reinicie o Explorer (Ctrl+Shift+Esc → aba Processos →
Explorador de Arquivos → Reiniciar tarefa) ou reinicie o Windows.

### 4. Conferir

Em um Prompt de Comando **novo**:

```
gcc --version
mingw32-make --version
gdb --version
```

Os três têm que responder. Repare que é `--version`, com dois traços — com um
traço só, o gcc entende como opção de compilação e reclama de "no input files".

### 5. VS Code

Instale a extensão **C/C++** (`ms-vscode.cpptools`) — é ela que dá o `F5`. Abra
a pasta do projeto e use `Ctrl+Shift+B`.

As configurações em `.vscode/` já vêm prontas e não têm caminho de máquina
nenhuma: elas chamam `gcc`, `make` e `gdb` pelo PATH. Não é preciso editar nada.

## Preparando o ambiente no Linux ou macOS

```bash
# Debian / Ubuntu
sudo apt install build-essential gdb

# Fedora
sudo dnf install gcc make gdb

# macOS
xcode-select --install
```

Depois, `make` na raiz. No macOS o `F5` do VS Code precisa de ajuste: o
`.vscode/launch.json` está configurado para `gdb`, e o macOS usa `lldb` —
troque `"MIMode": "gdb"` e `"miDebuggerPath": "gdb"` por `"lldb"`. Compilar e
rodar pelo terminal funcionam sem mexer em nada.
