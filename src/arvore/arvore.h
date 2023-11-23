#ifndef ARVORE_H
#define ARVORE_H

#include "../registro/registro.h"
#include <stdio.h>

typedef struct NoArvore {
    int chave; // Chave do registro
    long posicao; // Posição do registro no armazenamento externo
    long esquerda; // Posição do filho esquerdo no arquivo
    long direita; // Posição do filho direito no arquivo
} NoArvore;

long inserirNoArvore(FILE *arquivo, long posicaoRaiz, int chave, long contadorNos, int *transferencias, int *comparacoes);
long buscarNoArvore(FILE *arquivo, long posicaoRaiz, int chave, int *transferencias, int *comparacoes);
void construirArvoreBinaria(FILE *arquivoEntrada, int *transferencias, int *comparacoes, long *posicaoRaiz);
void exibirArvore(FILE *arquivo, long posicaoRaiz);

#endif // ARVORE_H
