#ifndef ARVOREB_H
#define ARVOREB_H

#include "../registro/registro.h"
#include <stdbool.h>

#define ORDEM_ARVORE_B 4 // Definindo a ordem da árvore B

typedef struct Entrada {
    int chave; // Chave do registro
    long posicao; // Posição do registro no armazenamento externo
} Entrada;

typedef struct NoArvoreB {
    int numChaves; // Número de chaves no nó
    Entrada entradas[ORDEM_ARVORE_B - 1]; // Array de entradas (chaves e posições)
    struct NoArvoreB *filhos[ORDEM_ARVORE_B]; // Ponteiros para os filhos
    bool folha; // Indica se o nó é uma folha
} NoArvoreB;

NoArvoreB* criarNoArvoreB();
NoArvoreB* inserirNoArvoreB(NoArvoreB *raiz, int chave, long referencia, int *transferencias, int *comparacoes);
Entrada* buscarNoArvoreB(NoArvoreB *raiz, int chave, int *transferencias, int *comparacoes);
void destruirArvoreB(NoArvoreB *raiz);

#endif // ARVOREB_H
