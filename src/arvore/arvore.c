#include "arvore.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int lerNoArquivo(FILE *arquivo, long posicao, NoArvore *no, int *transferencias) {
    if (fseek(arquivo, posicao * sizeof(NoArvore), SEEK_SET) != 0) {
        perror("Erro ao posicionar o ponteiro do arquivo para leitura");
        return -1;
    }
    if (fread(no, sizeof(NoArvore), 1, arquivo) != 1) {
        perror("Erro ao ler nó do arquivo");
        return -1;
    }
    (*transferencias)++;
    return 0;
}

int escreverNoArquivo(FILE *arquivo, long posicao, NoArvore *no) {
    if (fseek(arquivo, posicao * sizeof(NoArvore), SEEK_SET) != 0) {
        perror("Erro ao posicionar o ponteiro do arquivo para escrita");
        return -1;
    }
    if (fwrite(no, sizeof(NoArvore), 1, arquivo) != 1) {
        perror("Erro ao escrever nó no arquivo");
        return -1;
    }
    return 0;
}

void exibirArvoreInOrder(FILE *arquivo, long posicaoRaiz, int nivel) {
    if (posicaoRaiz == -1) {
        return; // Nó vazio, retorna
    }

    NoArvore no;
    int transferencias = 0;
    if (lerNoArquivo(arquivo, posicaoRaiz, &no, &transferencias) == -1) {
        printf("Erro ao ler nó na posição %ld.\n", posicaoRaiz);
        return;
    }

    // Exibe o filho esquerdo
    exibirArvoreInOrder(arquivo, no.esquerda, nivel + 1);

    // Exibe o nó atual
    for (int i = 0; i < nivel; i++) {
        printf("   "); // Indentação para visualizar a estrutura da árvore
    }
    printf("%d\n", no.chave);

    // Exibe o filho direito
    exibirArvoreInOrder(arquivo, no.direita, nivel + 1);
}

void exibirArvore(FILE *arquivo, long posicaoRaiz) {
    exibirArvoreInOrder(arquivo, posicaoRaiz, 0);
}

void construirArvoreBinaria(FILE *arquivoEntrada, int *transferencias, int *comparacoes, long *posicaoRaiz) {
    if (!arquivoEntrada) {
        perror("Arquivo de entrada inválido");
        return;
    }

    FILE *arquivoArvore = fopen("src/arvore/arvore.bin", "wb+");
    if (!arquivoArvore) {
        perror("Erro ao criar o arquivo da árvore");
        return;
    }

    Registro reg;
    *posicaoRaiz = -1;
    long contadorNos = 0;

    while (lerRegistro(arquivoEntrada, contadorNos, &reg, transferencias)) {
        *posicaoRaiz = inserirNoArvore(arquivoArvore, *posicaoRaiz, reg.chave, contadorNos, transferencias, comparacoes);
        if (*posicaoRaiz == -1) {
            printf("Erro ao inserir chave %d na árvore.\n", reg.chave);
            break;
        }
        contadorNos++;
    }

    fclose(arquivoArvore);
}


long inserirNoArvore(FILE *arquivo, long posicaoRaiz, int chave, long contadorNos, int *transferencias, int *comparacoes) {
    NoArvore no;

    if (posicaoRaiz == -1) {
        no.chave = chave;
        no.posicao = contadorNos;
        no.esquerda = -1;
        no.direita = -1;

        long novaPosicao = contadorNos * sizeof(NoArvore);
        if (escreverNoArquivo(arquivo, novaPosicao, &no) == -1) return -1;
        return novaPosicao;
    }

    if (lerNoArquivo(arquivo, posicaoRaiz, &no, transferencias) == -1) {
        printf("Erro ao ler nó na posição %ld.\n", posicaoRaiz);
        return -1;
    }

    (*comparacoes)++;
    if (chave < no.chave) {
        long novaPosicaoEsquerda = inserirNoArvore(arquivo, no.esquerda, chave, contadorNos, transferencias, comparacoes);
        if (novaPosicaoEsquerda == -1) return -1;
        if (no.esquerda != novaPosicaoEsquerda) {
            no.esquerda = novaPosicaoEsquerda;
            if (escreverNoArquivo(arquivo, posicaoRaiz, &no) == -1) return -1;
        }
    } else if (chave > no.chave) {
        long novaPosicaoDireita = inserirNoArvore(arquivo, no.direita, chave, contadorNos, transferencias, comparacoes);
        if (novaPosicaoDireita == -1) return -1;
        if (no.direita != novaPosicaoDireita) {
            no.direita = novaPosicaoDireita;
            if (escreverNoArquivo(arquivo, posicaoRaiz, &no) == -1) return -1;
        }
    }

    return posicaoRaiz;
}

long buscarNoArvore(FILE *arquivo, long posicaoRaiz, int chave, int *transferencias, int *comparacoes) {
    NoArvore no;

    if (posicaoRaiz == -1) {
        return -1;
    }

    if (lerNoArquivo(arquivo, posicaoRaiz, &no, transferencias) == -1) {
        printf("Erro ao ler nó na posição %ld durante a busca.\n", posicaoRaiz);
        return -1;
    }

    (*comparacoes)++;
    if (chave == no.chave) {
        return no.posicao; 
    } else if (chave < no.chave) {
        return buscarNoArvore(arquivo, no.esquerda, chave, transferencias, comparacoes);
    } else {
        return buscarNoArvore(arquivo, no.direita, chave, transferencias, comparacoes);
    }
}

