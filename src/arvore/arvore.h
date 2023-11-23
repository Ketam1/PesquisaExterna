#ifndef ARVORE_H
#define ARVORE_H

#include "../registro/registro.h"

typedef struct NoArvore {
    int chave; // Chave do registro
    long posicao; // Posição do registro no armazenamento externo
    long esquerda; // Posição do filho esquerdo no arquivo
    long direita; // Posição do filho direito no arquivo
} NoArvore;

long inserirNoArvore(FILE *arquivo, long posicaoRaiz, int chave, long posicaoRegistro, int *transferencias, int *comparacoes);
NoArvore* buscarNoArvore(NoArvore *raiz, int chave, int *transferencias, int *comparacoes);
void destruirArvore(NoArvore *raiz);

#endif // ARVORE_H
