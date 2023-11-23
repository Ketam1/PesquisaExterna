#include "arvoreb.h"
#include <stdlib.h>
#include <stdbool.h>

/**
 * Cria um novo nó para uma Árvore B.
 *
 * Esta função aloca memória para um novo nó de Árvore B, inicializa o número de chaves como 0,
 * define o nó como uma folha e inicializa todos os ponteiros de filhos como NULL.
 * Retorna o ponteiro para o novo nó criado.
 *
 * @return Ponteiro para o novo nó de Árvore B.
 */
NoArvoreB* criarNoArvoreB() {
    NoArvoreB *novoNo = (NoArvoreB*)malloc(sizeof(NoArvoreB));
    novoNo->numChaves = 0;
    novoNo->folha = true;
    for (int i = 0; i < ORDEM_ARVORE_B; i++) {
        novoNo->filhos[i] = NULL;
    }
    return novoNo;
}

/**
 * Divide um nó da Árvore B que atingiu sua capacidade máxima.
 *
 * Esta função é chamada quando um nó da Árvore B atinge sua capacidade máxima de chaves.
 * Ela divide o nó em dois, promovendo a chave do meio para o nó pai e ajustando os ponteiros
 * dos filhos adequadamente. A função também lida com a inserção do novo nó criado na árvore.
 *
 * @param i Índice no nó pai onde o novo nó será inserido.
 * @param no Ponteiro para o nó pai onde a divisão ocorrerá.
 * @param noFilho Ponteiro para o nó que será dividido.
 */
void dividirNo(int i, NoArvoreB *no, NoArvoreB *noFilho) {
    NoArvoreB *novoNo = criarNoArvoreB();
    novoNo->folha = noFilho->folha;
    const int pontoMedio = ORDEM_ARVORE_B / 2;

    // Correção: O novo nó terá um número de chaves igual a pontoMedio - 1
    novoNo->numChaves = pontoMedio - 1;

    // Mover chaves para o novo nó
    for (int j = 0; j < pontoMedio - 1; j++) {
        novoNo->entradas[j] = noFilho->entradas[j + pontoMedio];
    }

    // Mover filhos para o novo nó, se não for uma folha
    if (!noFilho->folha) {
        for (int j = 0; j < pontoMedio; j++) {
            novoNo->filhos[j] = noFilho->filhos[j + pontoMedio];
        }
    }

    // Ajustar o número de chaves do nó filho original
    noFilho->numChaves = pontoMedio - 1;

    // Mover filhos do nó pai para abrir espaço para o novo filho
    for (int j = no->numChaves; j >= i + 1; j--) {
        no->filhos[j + 1] = no->filhos[j];
    }

    // Inserir o novo filho no nó pai
    no->filhos[i + 1] = novoNo;

    // Mover chaves do nó pai para abrir espaço para a chave promovida
    for (int j = no->numChaves - 1; j >= i; j--) {
        no->entradas[j + 1] = no->entradas[j];
    }

    // Promover a chave do meio para o nó pai
    no->entradas[i] = noFilho->entradas[pontoMedio - 1];

    // Incrementar o número de chaves no nó pai
    no->numChaves++;
}

/**
 * Insere uma chave em um nó da Árvore B que não está cheio.
 *
 * Esta função insere uma nova chave em um nó da Árvore B que ainda tem espaço.
 * Se o nó for uma folha, a chave é inserida diretamente. Se não for uma folha,
 * a função encontra o filho correto para prosseguir com a inserção e, se necessário,
 * divide o filho cheio antes de prosseguir.
 *
 * @param no Ponteiro para o nó onde a chave será inserida.
 * @param chave Chave a ser inserida.
 * @param posicao Posição do registro no armazenamento externo.
 * @param transferencias Ponteiro para contagem de transferências na árvore.
 * @param comparacoes Ponteiro para contagem de comparações realizadas.
 */
void inserirNoNaoCheio(NoArvoreB *no, int chave, long posicao, int *transferencias, int *comparacoes) {
    int i = no->numChaves - 1;

    // Verifica se o nó é uma folha
    if (no->folha) {
        // Enquanto houver chaves maiores e estamos dentro dos limites do nó atual
        while (i >= 0 && chave < no->entradas[i].chave) {
            // Desloca as chaves maiores para a direita para abrir espaço
            no->entradas[i + 1] = no->entradas[i];
            i--;
            (*comparacoes)++;  // Incrementa a contagem de comparações
        }

        // Insere a chave na posição correta
        no->entradas[i + 1].chave = chave;
        no->entradas[i + 1].posicao = posicao;
        no->numChaves++;  // Incrementa o número de chaves no nó
        (*transferencias)++;  // Incrementa a contagem de transferências de dados
    } else {
        // Se o nó não for uma folha, determina o filho apropriado para inserção
        while (i >= 0 && chave < no->entradas[i].chave) {
            i--;
            (*comparacoes)++;  // Incrementa a contagem de comparações
        }
        i++;

        // Verifica se o filho onde a chave será inserida está cheio
        if (no->filhos[i]->numChaves == ORDEM_ARVORE_B - 1) {
            // Se o filho estiver cheio, divide o filho
            dividirNo(i, no, no->filhos[i]);

            // Após a divisão, a chave deve ser inserida no filho certo
            if (chave > no->entradas[i].chave) {
                i++;
            }
        }

        // Chama recursivamente a função para inserir a chave no filho apropriado
        inserirNoNaoCheio(no->filhos[i], chave, posicao, transferencias, comparacoes);
    }
}

/**
 * Insere uma chave na Árvore B.
 *
 * Esta função insere uma nova chave na Árvore B. Se a raiz estiver cheia, a árvore cresce em altura.
 * A função cria uma nova raiz, divide a raiz antiga e insere a chave no local apropriado.
 * Se a raiz não estiver cheia, a função chama inserirNoNaoCheio para lidar com a inserção.
 *
 * @param raiz Ponteiro para a raiz da Árvore B.
 * @param chave Chave a ser inserida.
 * @param posicao Posição do registro no armazenamento externo.
 * @param transferencias Ponteiro para contagem de transferências na árvore.
 * @param comparacoes Ponteiro para contagem de comparações realizadas.
 * @return Ponteiro para a raiz atualizada da Árvore B.
 */
NoArvoreB* inserirNoArvoreB(NoArvoreB *raiz, int chave, long posicao, int *transferencias, int *comparacoes) {
    // Verifica se a raiz é nula (árvore vazia)
    if (raiz == NULL) {
        // Cria uma nova raiz e insere a chave nela
        raiz = criarNoArvoreB();
        raiz->entradas[0].chave = chave;
        raiz->entradas[0].posicao = posicao;
        raiz->numChaves = 1;
    } else {
        // Verifica se a raiz está cheia
        if (raiz->numChaves == ORDEM_ARVORE_B - 1) {
            // Cria uma nova raiz e a torna não folha
            NoArvoreB *novaRaiz = criarNoArvoreB();
            novaRaiz->folha = false;
            novaRaiz->filhos[0] = raiz;

            // Divide a raiz antiga e atualiza a raiz global
            dividirNo(0, novaRaiz, raiz);
            (*comparacoes)++;  // Incrementa a contagem de comparações

            // Determina em qual filho inserir a chave
            if (chave > novaRaiz->entradas[0].chave) {
                inserirNoNaoCheio(novaRaiz->filhos[1], chave, posicao, transferencias, comparacoes);
            } else {
                inserirNoNaoCheio(novaRaiz->filhos[0], chave, posicao, transferencias, comparacoes);
            }

            raiz = novaRaiz;  // Atualize a raiz global para a nova raiz
        } else {
            // Se a raiz não estiver cheia, chama a função para inserir no nó não cheio
            inserirNoNaoCheio(raiz, chave, posicao, transferencias, comparacoes);
        }
    }

    return raiz;  // Retorna a raiz atualizada da Árvore B
}

/**
 * Busca uma chave na Árvore B.
 *
 * Esta função busca uma chave na Árvore B. A busca começa a partir da raiz fornecida
 * e segue os ponteiros apropriados até encontrar a chave ou alcançar uma folha.
 * Se a chave for encontrada, a função retorna um ponteiro para a entrada correspondente.
 * Se a chave não for encontrada ou a árvore estiver vazia (raiz == NULL), retorna NULL.
 *
 * @param raiz Ponteiro para a raiz da Árvore B onde a busca será realizada.
 * @param chave Chave a ser buscada.
 * @param transferencias Ponteiro para contagem de transferências na árvore.
 * @param comparacoes Ponteiro para contagem de comparações realizadas.
 * @return Ponteiro para a entrada encontrada ou NULL se a chave não for encontrada.
 */
Entrada* buscarNoArvoreB(NoArvoreB *raiz, int chave, int *transferencias, int *comparacoes) {
    // Verifica se a árvore está vazia (raiz == NULL)
    if (raiz == NULL) {
        return NULL;
    }

    int i = 0;
    // Encontra a posição onde a chave deve estar no nó
    while (i < raiz->numChaves && chave > raiz->entradas[i].chave) {
        i++;
        (*comparacoes)++;  // Incrementa a contagem de comparações
    }

    // Verifica se a chave foi encontrada no nó atual
    if (i < raiz->numChaves && chave == raiz->entradas[i].chave) {
        (*comparacoes)++;  // Incrementa a contagem de comparações
        return &raiz->entradas[i];  // Retorna um ponteiro para a entrada encontrada
    }

    // Se não for uma folha, continua a busca nos filhos apropriados
    if (raiz->folha) {
        return NULL;
    }

    return buscarNoArvoreB(raiz->filhos[i], chave, transferencias, comparacoes);  // Recursivamente busca nos filhos
}

/**
 * Função para destruir uma árvore B e liberar a memória alocada.
 *
 * @param raiz O ponteiro para a raiz da árvore B a ser destruída.
 */
void destruirArvoreB(NoArvoreB *raiz) {
    if (raiz == NULL) {
        // Se a raiz é NULL, não há nada a ser liberado
        return;
    }

    // Se o nó não é uma folha, libera todos os seus filhos primeiro
    if (!raiz->folha) {
        for (int i = 0; i <= raiz->numChaves; i++) {
            destruirArvoreB(raiz->filhos[i]);
        }
    }

    // Libera a memória alocada para o nó atual
    free(raiz);
}
