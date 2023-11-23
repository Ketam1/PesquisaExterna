#ifndef ARVOREBSTAR_H
#define ARVOREBSTAR_H

#include "../registro/registro.h"
#include <stdbool.h>

#define ORDEM_ARVORE_BSTAR 5 // Definindo a ordem da árvore B*

// Estrutura para nós internos
typedef struct NoInternoArvoreBStar {
    int numChaves; // Número de chaves no nó
    int chaves[ORDEM_ARVORE_BSTAR - 1]; // Array de chaves
    struct NoArvoreBStar *filhos[ORDEM_ARVORE_BSTAR]; // Ponteiros para os filhos
} NoInternoArvoreBStar;

// Estrutura para nós folha
typedef struct NoFolhaArvoreBStar {
    int numChaves; // Número de chaves no nó
    int chaves[ORDEM_ARVORE_BSTAR - 1]; // Array de chaves
    Registro registros[ORDEM_ARVORE_BSTAR - 1]; // Array de registros
    struct NoFolhaArvoreBStar *proximo; // Ponteiro para o próximo nó folha
} NoFolhaArvoreBStar;

// Estrutura unificada para a árvore B*
typedef struct NoArvoreBStar {
    bool folha; // Indica se o nó é uma folha
    union {
        NoInternoArvoreBStar interno;
        NoFolhaArvoreBStar folha;
    } tipo;
} NoArvoreBStar;

NoArvoreBStar* criarNoArvoreBStar(bool ehFolha);
NoArvoreBStar* inserirArvoreBStar(NoArvoreBStar *raiz, Registro reg, int *transferencias, int *comparacoes);
Registro* buscarArvoreBStar(NoArvoreBStar *raiz, int chave, int *transferencias, int *comparacoes);
void destruirArvoreBStar(NoArvoreBStar *raiz);

#endif // ARVOREBSTAR_H
