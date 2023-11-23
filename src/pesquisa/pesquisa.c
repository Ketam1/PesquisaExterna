#include "pesquisa.h"
#include "../registro/registro.h"
#include "../index/index.h"
#include "../arvore/arvore.h"
#include "../arvoreb/arvoreb.h"
#include "../arvorebstar/arvorebstar.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/**
 * Realiza uma pesquisa sequencial indexada em um arquivo binário de registros.
 *
 * Esta função abre um arquivo binário e realiza uma pesquisa sequencial indexada para encontrar
 * um registro com a chave especificada. A função também mede o tempo de execução da pesquisa,
 * além de contar o número de transferências de dados e comparações de chaves realizadas.
 *
 * @param nomeArquivo Caminho para o arquivo binário onde a pesquisa será realizada.
 * @param chave Chave do registro a ser pesquisado.
 * @param exibirChaves Flag para indicar se os detalhes do registro encontrado devem ser exibidos.
 */
void acessoSequencialIndexado(const char *nomeArquivo, int chave, int exibirChaves) {
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    int transferenciasConstrucao = 0, comparacoesConstrucao = 0;
    bool encontrado = false;
    Registro reg;
    Indice *indice = NULL;

    // Criando o índice
    clock_t inicioConstrucao = clock();
    int intervaloIndex = 100; // Definindo o intervalo para o índice
    criarIndice(
        arquivo, 
        &indice,
        &intervaloIndex, 
        100, 
        &transferenciasConstrucao, 
        &comparacoesConstrucao
    );
    clock_t fimConstrucao = clock();
    double tempoExecucaoConstrucao = (double)(fimConstrucao - inicioConstrucao) / CLOCKS_PER_SEC;
    

    int transferenciasPesquisa = 0, comparacoesPesquisa = 0;
    clock_t inicioPesquisa = clock();
    long posicao = 0;
    for (int i = 0; i < intervaloIndex; i++) {
        if (indice[i].chave > chave) {
            break;
        }
        posicao = indice[i].posicao;
    }

    while (lerRegistro(arquivo, posicao, &reg, &transferenciasPesquisa)) {
        comparacoesPesquisa++;
        if (reg.chave == chave) {
            encontrado = true;
            printf("Registro encontrado!\n");
            printf("Chave: %d\nDado1: %ld\nDado2: %.50s...\n", reg.chave, reg.dado1, reg.dado2);
            break;
        }
        posicao++;
    }

    clock_t fimPesquisa = clock();
    double tempoExecucaoPesquisa = (double)(fimPesquisa - inicioPesquisa) / CLOCKS_PER_SEC;

    // Imprimindo resultados da pesquisa
    if (!encontrado) {
        printf("Registro não encontrado no arquivo.\n");
    }

    printf(
        "\nMétricas da Pesquisa:\n - Transferências: %d\n - Comparações: %d\n - Tempo de execução: %.7f segundos\n", 
        transferenciasPesquisa, 
        comparacoesPesquisa, 
        tempoExecucaoPesquisa
    );
    printf(
        "\nMétricas da Construção do Índice:\n - Transferências: %d\n - Comparações: %d\n - Tempo de execução: %.7f segundos\n", 
        transferenciasConstrucao, 
        comparacoesConstrucao, 
        tempoExecucaoConstrucao
    );

    fclose(arquivo);
}


/**
 * Realiza uma pesquisa em uma árvore binária de busca construída a partir de um arquivo de registros.
 *
 * Esta função lê registros de um arquivo binário e constrói uma árvore binária de busca. Em seguida,
 * realiza uma pesquisa na árvore para encontrar um registro com a chave especificada. A função também
 * mede o tempo de execução da pesquisa e conta o número de comparações realizadas.
 *
 * @param nomeArquivo Caminho para o arquivo binário de onde os registros são lidos.
 * @param chave Chave do registro a ser pesquisado.
 * @param exibirChaves Flag para indicar se os detalhes do registro encontrado devem ser exibidos.
 */
void arvoreBinariaPesquisa(const char *nomeArquivo, int chave, int exibirChaves) {
    // Abrindo o arquivo para leitura
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    NoArvore *raiz = NULL;
    Registro reg;
    int transferenciasConstrucao = 0; // Inicialize a contagem de transferências
    int comparacoesConstrucao = 0;    // Inicialize a contagem de comparações

    // Início da construção da árvore
    clock_t inicioConstrucao = clock();
    long posicao = 0;
    while (lerRegistro(arquivo, posicao, &reg, &transferenciasConstrucao)) {
        raiz = inserirNoArvore(raiz, reg.chave, posicao, &transferenciasConstrucao, &comparacoesConstrucao);
        posicao++;
    }
    clock_t fimConstrucao = clock();
    double tempoExecucaoConstrucao = (double)(fimConstrucao - inicioConstrucao) / CLOCKS_PER_SEC;

    // Início da pesquisa
    int transferenciasPesquisa = 0; 
    int comparacoesPesquisa = 0;    
    clock_t inicioPesquisa = clock();

    Registro resultado;
    NoArvore *noEncontrado = buscarNoArvore(raiz, chave, &transferenciasPesquisa, &comparacoesPesquisa);
    bool registroEncontrado = false;
    
    if(noEncontrado != NULL){
        registroEncontrado = lerRegistro(arquivo, noEncontrado->posicao, &resultado, &transferenciasPesquisa);
    } 
    
    clock_t fimPesquisa = clock();
    double tempoExecucaoPesquisa = (double)(fimPesquisa - inicioPesquisa) / CLOCKS_PER_SEC;

    // Imprimindo o resultado da pesquisa
    if (registroEncontrado) {
        printf("Registro encontrado!\n");
        printf("Chave: %d\nDado1: %ld\nDado2: %.50s...\n", resultado.chave, resultado.dado1, resultado.dado2);
    } else {
        printf("Registro não encontrado no arquivo.\n");
    }

    printf(
        "\nMétricas da Pesquisa:\n - Transferências: %d\n - Comparações: %d\n - Tempo de execução: %.7f segundos\n", 
        transferenciasPesquisa, 
        comparacoesPesquisa, 
        tempoExecucaoPesquisa
    );
    printf(
        "\nMétricas da Construção do Índice:\n - Transferências: %d\n - Comparações: %d\n - Tempo de execução: %.7f segundos\n", 
        transferenciasConstrucao, 
        comparacoesConstrucao, 
        tempoExecucaoConstrucao
    );

    fclose(arquivo);
    destruirArvore(raiz);
}

/**
 * Realiza uma pesquisa em uma árvore B construída a partir de um arquivo de registros.
 *
 * Esta função lê registros de um arquivo binário e constrói uma árvore B. Em seguida,
 * realiza uma pesquisa na árvore para encontrar um registro com a chave especificada. A função também
 * mede o tempo de execução da pesquisa e conta o número de comparações realizadas.
 *
 * @param nomeArquivo Caminho para o arquivo binário de onde os registros são lidos.
 * @param chave Chave do registro a ser pesquisado.
 * @param exibirChaves Flag para indicar se os detalhes do registro encontrado devem ser exibidos.
 */
void arvoreB(const char *nomeArquivo, int chave, int exibirChaves) {
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    NoArvoreB *raiz = NULL;
    int transferenciasConstrucao = 0, comparacoesConstrucao = 0;
    long posicao = 0;

    // Início da construção da árvore
    Registro reg;
    clock_t inicioConstrucao = clock();
    while (lerRegistro(arquivo, posicao, &reg, &transferenciasConstrucao)) {
        raiz = inserirNoArvoreB(raiz, reg.chave, posicao, &transferenciasConstrucao, &comparacoesConstrucao);
        posicao++;
    }
    clock_t fimConstrucao = clock();
    double tempoExecucaoConstrucao = (double)(fimConstrucao - inicioConstrucao) / CLOCKS_PER_SEC;

    // Início da pesquisa
    int transferenciasPesquisa = 0; 
    int comparacoesPesquisa = 0;    
    clock_t inicioPesquisa = clock();

    Registro resultado;
    Entrada *entradaEncontrada = buscarNoArvoreB(raiz, chave, &transferenciasPesquisa, &comparacoesPesquisa);
    bool registroEncontrado = false;
    
    if(entradaEncontrada != NULL){
        registroEncontrado = lerRegistro(arquivo, entradaEncontrada->posicao, &resultado, &transferenciasPesquisa);
    } 
    
    clock_t fimPesquisa = clock();
    double tempoExecucaoPesquisa = (double)(fimPesquisa - inicioPesquisa) / CLOCKS_PER_SEC;

    // Imprimindo o resultado da pesquisa
    if (registroEncontrado) {
        printf("Registro encontrado!\n");
        printf("Chave: %d\nDado1: %ld\nDado2: %.50s...\n", resultado.chave, resultado.dado1, resultado.dado2);
    } else {
        printf("Registro não encontrado no arquivo.\n");
    }

    printf(
        "\nMétricas da Pesquisa:\n - Transferências: %d\n - Comparações: %d\n - Tempo de execução: %.7f segundos\n", 
        transferenciasPesquisa, 
        comparacoesPesquisa, 
        tempoExecucaoPesquisa
    );
    printf(
        "\nMétricas da Construção do Índice:\n - Transferências: %d\n - Comparações: %d\n - Tempo de execução: %.7f segundos\n", 
        transferenciasConstrucao, 
        comparacoesConstrucao, 
        tempoExecucaoConstrucao
    );

    fclose(arquivo);
    destruirArvoreB(raiz);
}

/**
 * Realiza uma pesquisa em uma árvore B* construída a partir de um arquivo de registros.
 *
 * Esta função lê registros de um arquivo binário e constrói uma árvore B*. Em seguida,
 * realiza uma pesquisa na árvore para encontrar um registro com a chave especificada. A função também
 * mede o tempo de execução da pesquisa e conta o número de comparações realizadas.
 *
 * @param nomeArquivo Caminho para o arquivo binário de onde os registros são lidos.
 * @param chave Chave do registro a ser pesquisado.
 * @param exibirChaves Flag para indicar se os detalhes do registro encontrado devem ser exibidos.
 */
void arvoreBStar(const char *nomeArquivo, int chave, int exibirChaves) {
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    NoArvoreBStar *raiz = NULL;
    Registro reg;
    int transferenciasConstrucao = 0, comparacoesConstrucao = 0;

    clock_t inicioConstrucao = clock();
    while (fread(&reg, sizeof(Registro), 1, arquivo)) {
        raiz = inserirArvoreBStar(raiz, reg, &transferenciasConstrucao, &comparacoesConstrucao);
        transferenciasConstrucao++;
    }
    clock_t fimConstrucao = clock();
    double tempoExecucaoConstrucao = (double)(fimConstrucao - inicioConstrucao) / CLOCKS_PER_SEC;

    fclose(arquivo);

    int transferenciasPesquisa = 0, comparacoesPesquisa = 0;
    clock_t inicioPesquisa = clock();
    Registro *resultado = buscarArvoreBStar(raiz, chave, &transferenciasPesquisa, &comparacoesPesquisa);
    clock_t fimPesquisa = clock();
    double tempoExecucaoPesquisa = (double)(fimPesquisa - inicioPesquisa) / CLOCKS_PER_SEC;

    // Imprimindo o resultado da pesquisa
    if (resultado != NULL) {
        printf("Registro de chave %d encontrado!\n", resultado->chave);
        printf("Chave: %d\nDado1: %ld\nDado2: %.50s...\n", resultado->chave, resultado->dado1, resultado->dado2);
    } else {
        printf("Registro não encontrado no arquivo.\n");
    }

    printf(
        "\nMétricas da Pesquisa:\n - Transferências: %d\n - Comparações: %d\n - Tempo de execução: %.7f segundos\n", 
        transferenciasPesquisa, 
        comparacoesPesquisa, 
        tempoExecucaoPesquisa
    );
    printf(
        "\nMétricas da Construção do Índice:\n - Transferências: %d\n - Comparações: %d\n - Tempo de execução: %.7f segundos\n", 
        transferenciasConstrucao, 
        comparacoesConstrucao, 
        tempoExecucaoConstrucao
    );

    destruirArvoreBStar(raiz);
}