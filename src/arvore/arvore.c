#include "arvore.h"
#include <stdlib.h>
#include "../registro/registro.h"
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * Insere um novo nó na árvore binária de busca.
 *
 * Esta função insere um novo nó com uma chave e posição especificadas em uma árvore binária de busca.
 * A inserção é feita de forma recursiva, mantendo as propriedades da árvore binária de busca.
 * Se a árvore estiver vazia (raiz == NULL), um novo nó é criado e retornado como a nova raiz.
 * Se a chave já existir na árvore, a função não faz nada.
 *
 * @param raiz Ponteiro para a raiz da árvore ou subárvore onde o nó será inserido.
 * @param chave Chave do novo nó a ser inserido.
 * @param posicao Posição do registro no armazenamento externo.
 * @param transferencias Ponteiro para contagem de transferências na árvore.
 * @param comparacoes Ponteiro para contagem de comparações realizadas.
 * @return Ponteiro para a raiz atualizada da árvore ou subárvore.
 */
long inserirNoArvore(FILE *arquivo, long posicaoRaiz, int chave, long posicaoRegistro, int *transferencias, int *comparacoes) {
    NoArvore no;

    // Se a posição da raiz for -1, crie um novo nó
    if (posicaoRaiz == -1) {
        no.chave = chave;
        no.posicao = posicaoRegistro;
        no.esquerda = -1;
        no.direita = -1;

        // Escreva o novo nó no final do arquivo
        fseek(arquivo, 0, SEEK_END);
        long novaPosicao = ftell(arquivo);
        fwrite(&no, sizeof(NoArvore), 1, arquivo);
        return novaPosicao;
    }

    // Ler o nó atual do arquivo
    fseek(arquivo, posicaoRaiz * sizeof(NoArvore), SEEK_SET);
    fread(&no, sizeof(NoArvore), 1, arquivo);

    (*comparacoes)++;
    if (chave < no.chave) {
        long novaPosicaoEsquerda = inserirNoArvore(arquivo, no.esquerda, chave, posicaoRegistro, transferencias, comparacoes);
        if (no.esquerda != novaPosicaoEsquerda) {
            no.esquerda = novaPosicaoEsquerda;
            fseek(arquivo, posicaoRaiz * sizeof(NoArvore), SEEK_SET);
            fwrite(&no, sizeof(NoArvore), 1, arquivo);
        }
    } else if (chave > no.chave) {
        long novaPosicaoDireita = inserirNoArvore(arquivo, no.direita, chave, posicaoRegistro, transferencias, comparacoes);
        if (no.direita != novaPosicaoDireita) {
            no.direita = novaPosicaoDireita;
            fseek(arquivo, posicaoRaiz * sizeof(NoArvore), SEEK_SET);
            fwrite(&no, sizeof(NoArvore), 1, arquivo);
        }
    }

    return posicaoRaiz;
}

/**
 * Busca um nó na árvore binária de busca com base em uma chave especificada.
 *
 * Esta função procura recursivamente um nó na árvore binária de busca cuja chave
 * corresponda à chave fornecida. A busca começa a partir do nó raiz fornecido.
 * Se a chave for encontrada, a função retorna o ponteiro para o nó correspondente.
 * Se a chave não for encontrada ou a árvore estiver vazia (raiz == NULL), retorna NULL.
 *
 * @param raiz Ponteiro para a raiz da árvore ou subárvore onde a busca será realizada.
 * @param chave Chave do nó a ser buscado.
 * @param transferencias Ponteiro para contagem de transferências na árvore.
 * @param comparacoes Ponteiro para contagem de comparações realizadas.
 * @return Ponteiro para o nó encontrado ou NULL se a chave não for encontrada.
 */
NoArvore* buscarNoArvore(NoArvore *raiz, int chave, int *transferencias, int *comparacoes) {
    // Verifica se a árvore está vazia (raiz == NULL)
    if (raiz == NULL) {
        return NULL;
    }

    (*comparacoes)++;  // Incrementa a contagem de comparações

    // Compara a chave desejada com a chave do nó atual
    if (chave == raiz->chave) {
        return raiz;  // Retorna um ponteiro para o nó com a chave correspondente
    } else if (chave < raiz->chave) {
        // Se a chave desejada for menor, continua a busca no ramo esquerdo da árvore
        return buscarNoArvore(raiz->esquerda, chave, transferencias, comparacoes);
    } else {
        // Se a chave desejada for maior, continua a busca no ramo direito da árvore
        return buscarNoArvore(raiz->direita, chave, transferencias, comparacoes);
    }
}

/**
 * Libera a memória alocada para uma árvore binária de busca.
 *
 * Esta função percorre recursivamente toda a árvore binária de busca, começando
 * pela raiz fornecida, e libera a memória de cada nó. A função primeiro libera
 * as subárvores esquerda e direita de um nó e, em seguida, libera o próprio nó.
 * Se a árvore estiver vazia (raiz == NULL), a função não executa nenhuma ação.
 *
 * @param raiz Ponteiro para a raiz da árvore a ser destruída.
 */
void destruirArvore(NoArvore *raiz) {
    if (raiz != NULL) {
        destruirArvore(raiz->esquerda);
        destruirArvore(raiz->direita);
        free(raiz);
    }
}
