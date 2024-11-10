# Projeto de Matemática Discreta

Desenvolver um progama RSA

Grupo 4

## Windows
**Opção 1**
Baixar o executável em [releases](https://github.com/Felipe-Aragao/RSA-MD/releases)

**Opção 2**
1 - Baixar [msys2](https://www.msys2.org/)

2 - Pelo terminal do msys2:
`pacman -S mingw-w64-ucrt-x86_64-gcc`
`pacman -S mingw-w64-ucrt-x86_64-gtk3`

3 - Ir para o local de main.c e no terminal do msys2:
``gcc `pkg-config --cflags gtk+-3.0` -o main main.c `pkg-config --libs gtk+-3.0` ``

## Linux

1 - Baixar gtk:
``sudo apt install libgtk-3-dev``

2 - Abri local de main.c:
``gcc `pkg-config --cflags gtk+-3.0` -o main main.c `pkg-config --libs gtk+-3.0` -lm``
