#ifndef INDEX_H
#define INDEX_H

#include "../registro/registro.h"
#include <stdio.h>

typedef struct {
    int chave;
    long posicao;
} Indice;

void criarIndice(
    FILE *arquivo,
    Indice **indice,
    int *tamanhoIndice,
    int intervaloIndex,
    int *transferencias,
    int *comparacoes
);

#endif // INDEX_H
