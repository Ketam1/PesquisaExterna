#include "arvorebstar.h"
#include <stdlib.h>

/**
 * Cria um novo nó para a árvore B*.
 *
 * Esta função aloca memória para um novo nó da árvore B*, inicializa suas propriedades,
 * e define todos os seus filhos como NULL.
 *
 * @return Ponteiro para o novo nó criado.
 */
NoArvoreBStar* criarNoArvoreBStar(bool ehFolha) {
    NoArvoreBStar *novoNo = (NoArvoreBStar*)malloc(sizeof(NoArvoreBStar));
    novoNo->folha = ehFolha;
    novoNo->tipo.folha.numChaves = 0;

    if (ehFolha) {
        for (int i = 0; i < ORDEM_ARVORE_BSTAR; i++) {
            novoNo->tipo.folha.proximo = NULL;
        }
    } else {
        for (int i = 0; i < ORDEM_ARVORE_BSTAR; i++) {
            novoNo->tipo.interno.filhos[i] = NULL;
        }
    }
    return novoNo;
}

/**
 * Encontra a posição correta para inserir uma nova chave em um nó.
 *
 * Esta função percorre as chaves de um nó para encontrar a posição correta onde
 * uma nova chave deve ser inserida. A posição é determinada de forma que as chaves
 * do nó permaneçam ordenadas.
 *
 * @param chaves Array de chaves do nó.
 * @param numChaves Número atual de chaves no nó.
 * @param chave Chave a ser inserida.
 * @return Índice no array de chaves onde a nova chave deve ser inserida.
 */
int encontrarPosicaoInsercao(int chaves[], int numChaves, int chave) {
    int pos = 0;
    while (pos < numChaves && chaves[pos] < chave) {
        pos++;
    }
    return pos;
}


bool inserirRegistroNoNóFolha(NoFolhaArvoreBStar *no, Registro reg, int *transferencias, int *comparacoes) {
    if (no == NULL || no->numChaves >= ORDEM_ARVORE_BSTAR - 1) {
        // Nó é nulo ou já está cheio
        return false;
    }

    int posicaoInsercao = 0;
    while (posicaoInsercao < no->numChaves && no->chaves[posicaoInsercao] < reg.chave) {
        (*comparacoes)++;
        posicaoInsercao++;
    }

    // Se a chave já existe, não insere
    if (posicaoInsercao < no->numChaves && no->chaves[posicaoInsercao] == reg.chave) {
        (*comparacoes)++;
        return false;
    }

    // Desloca as chaves e registros para abrir espaço para o novo registro
    for (int i = no->numChaves; i > posicaoInsercao; i--) {
        no->chaves[i] = no->chaves[i - 1];
        no->registros[i] = no->registros[i - 1];
        (*transferencias) += 2;
    }

    // Insere o novo registro
    no->chaves[posicaoInsercao] = reg.chave;
    no->registros[posicaoInsercao] = reg;
    no->numChaves++;
    (*transferencias) += 2;

    return true;
}

void dividirNó(NoArvoreBStar *pai, int index, NoArvoreBStar *nó, int *transferencias, int *comparacoes) {

}

NoArvoreBStar* inserirArvoreBStar(NoArvoreBStar *raiz, Registro reg, int *transferencias, int *comparacoes) {
    // Implementação específica para inserção na árvore B*
    // ...
}

Registro* buscarArvoreBStar(NoArvoreBStar *raiz, int chave, int *transferencias, int *comparacoes) {
    // Implementação específica para busca na árvore B*
    // ...
}

void destruirArvoreBStar(NoArvoreBStar *raiz) {
    // Implementação específica para destruir a árvore B*
    // ...
}