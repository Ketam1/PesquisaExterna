#include "registro.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Lê um registro de um arquivo binário em uma posição específica e atualiza a contagem de transferências.
 *
 * Esta função posiciona o ponteiro do arquivo na posição especificada e lê um registro.
 * Em caso de erro na busca ou leitura, exibe uma mensagem apropriada. A função também
 * incrementa a contagem de transferências de dados do armazenamento externo para a memória interna.
 *
 * @param arquivo Ponteiro para o arquivo binário de onde o registro será lido.
 * @param posicao Posição do registro no arquivo, baseada no índice do registro.
 * @param reg Ponteiro para o registro onde os dados lidos serão armazenados.
 * @param transferencias Ponteiro para a contagem de transferências de dados.
 */
bool lerRegistro(FILE *arquivo, long posicao, Registro *reg, int *transferencias) {
    if (fseek(arquivo, posicao * sizeof(Registro), SEEK_SET) != 0) {
        perror("Erro ao buscar posição no arquivo");
        return false;
    }

    if (fread(reg, sizeof(Registro), 1, arquivo) == 1) {
        (*transferencias)++;
        return true;
    } else {
        if (feof(arquivo)) {
            printf("Fim do arquivo alcançado.\n");
        } else {
            perror("Erro ao ler registro do arquivo");
        }
        return false;
    }
}


/**
 * Escreve um registro em um arquivo binário em uma posição específica.
 *
 * Esta função posiciona o ponteiro do arquivo na posição especificada e escreve um registro.
 * Em caso de erro na busca ou escrita, exibe uma mensagem apropriada.
 *
 * @param arquivo Ponteiro para o arquivo binário onde o registro será escrito.
 * @param posicao Posição no arquivo onde o registro será escrito, baseada no índice do registro.
 * @param reg Ponteiro para o registro que contém os dados a serem escritos.
 */
void escreverRegistro(FILE *arquivo, long posicao, const Registro *reg) {
    if (fseek(arquivo, posicao * sizeof(Registro), SEEK_SET) != 0) {
        perror("Erro ao buscar posição no arquivo");
        return;
    }

    if (fwrite(reg, sizeof(Registro), 1, arquivo) != 1) {
        perror("Erro ao escrever registro no arquivo");
    }
}

/**
 * Exibe todos os registros de um arquivo binário.
 *
 * Esta função abre um arquivo binário para leitura e exibe todos os registros contidos nele.
 * Cada registro é exibido com seu índice, chave e dados associados.
 *
 * @param nomeArquivo Caminho para o arquivo binário de onde os registros serão lidos e exibidos.
 */
void exibirRegistros(const char *nomeArquivo, int quantidade) {
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    // Calcular o intervalo de salto
    int salto = (quantidade > 20) ? quantidade / 20 : 1;
    int registrosExibidos = 0;

    Registro reg;
    int i = 0;

    while (registrosExibidos < 20 && i < quantidade) {
        fseek(arquivo, i * sizeof(Registro), SEEK_SET);
        if (!fread(&reg, sizeof(Registro), 1, arquivo)) {
            break; // Sai do loop se não conseguir ler mais registros
        }

        printf("Registro %d\nChave: %d\n\n", i, reg.chave);
        i += salto;
        registrosExibidos++;
    }

    fclose(arquivo);
}


