#include "index.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * Cria um índice a partir de registros armazenados em um arquivo.
 *
 * Esta função lê registros de um arquivo fornecido e cria um índice para esses registros.
 * O índice é construído a partir de chaves de registros em intervalos especificados.
 * Durante o processo, a função contabiliza transferências (leituras do arquivo) e
 * comparações (embora neste caso não pareça haver comparações entre chaves).
 *
 * @param arquivo Ponteiro para o arquivo de onde os registros são lidos.
 * @param indice Ponteiro para um ponteiro do índice a ser criado.
 * @param tamanhoIndice Ponteiro para armazenar o tamanho do índice criado.
 * @param intervaloIndex Intervalo entre registros para indexar.
 * @param transferencias Ponteiro para contagem de transferências realizadas.
 * @param comparacoes Ponteiro para contagem de comparações realizadas.
 */
void criarIndice(
    FILE *arquivo,
    Indice **indice,
    int *tamanhoIndice,
    int intervaloIndex,
    int *transferencias,
    int *comparacoes
) {
    long posicao = 0; 
    Registro reg;
    *tamanhoIndice = 0; 
    *indice = NULL; 
    *transferencias = 0; 
    *comparacoes = 0; 
    
    while (lerRegistro(arquivo, posicao, &reg, transferencias)) {
        // Adiciona uma entrada no índice a cada intervaloIndex registros
        if (posicao % intervaloIndex == 0) {
            // Realoca espaço para o índice
            *indice = realloc(*indice, (*tamanhoIndice + 1) * sizeof(Indice));
            // Adiciona a chave e a posição do registro no índice
            (*indice)[*tamanhoIndice].chave = reg.chave;
            (*indice)[*tamanhoIndice].posicao = posicao;
            (*tamanhoIndice)++; // Incrementa o tamanho do índice
        }
        posicao++; // Avança para a próxima posição no arquivo
    }
}