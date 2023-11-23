#ifndef REGISTRO_H
#define REGISTRO_H

#define TAMANHO_DADO 50

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    int chave;
    long dado1;
    char dado2[TAMANHO_DADO];
} Registro;

// Protótipos para manipulação de registros
bool lerRegistro(FILE *arquivo, long posicao, Registro *reg, int *transferencias);
void escreverRegistro(FILE *arquivo, long posicao, const Registro *reg);
void exibirRegistros(const char *nomeArquivo, int quantidade);

#endif
