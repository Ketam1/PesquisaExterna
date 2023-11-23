#include <sys/stat.h>
#include "../registro/registro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * Verifica se um arquivo especificado existe no sistema de arquivos.
 *
 * Esta função utiliza a chamada de sistema 'stat' para obter informações sobre o arquivo
 * especificado pelo caminho. Se 'stat' retornar 0, significa que o arquivo existe.
 *
 * @param caminho Uma string constante que representa o caminho do arquivo a ser verificado.
 * @return Retorna 1 (verdadeiro) se o arquivo existe, caso contrário, retorna 0 (falso).
 */
int arquivoExiste(const char *caminho) {
    struct stat buffer;
    return (stat(caminho, &buffer) == 0);
}

/**
 * Gera dados aleatórios para um registro.
 *
 * Esta função preenche a estrutura de um registro com dados aleatórios. A chave do registro
 * é definida para o valor especificado, 'dado1' recebe um número aleatório e 'dado2' é
 * preenchido com uma string de caracteres aleatórios.
 *
 * @param reg Um ponteiro para a estrutura de Registro a ser preenchida.
 * @param chave Um inteiro que será usado como chave do registro.
 */
void gerarDadosAleatorios(Registro *reg, int chave) {
    reg->chave = chave;
    reg->dado1 = rand();
    for (int i = 0; i < TAMANHO_DADO - 1; i++) {
        reg->dado2[i] = 'A' + (rand() % 26);
    }
    reg->dado2[TAMANHO_DADO - 1] = '\0';
}

/**
 * Gera um arquivo de registros com dados aleatórios.
 *
 * Esta função cria um arquivo no caminho especificado e preenche-o com uma quantidade
 * especificada de registros. Os registros são gerados com chaves em ordem ascendente,
 * descendente ou aleatória, dependendo do modo fornecido. Se o arquivo já existir, a
 * função retorna sem criar um novo arquivo. A função aloca memória para armazenar os
 * registros temporariamente antes de escrevê-los no arquivo.
 *
 * @param caminhoCompleto String constante com o caminho completo do arquivo a ser criado.
 * @param quantidade Número de registros a serem gerados no arquivo.
 * @param modo Modo de geração das chaves dos registros (ascendente, descendente ou aleatório).
 * @return Retorna 1 para sucesso ou -1 para falha na criação do arquivo.
 */
int gerarArquivo(const char *caminhoCompleto, int quantidade, int modo) {
    // Verifica se o arquivo já existe
    if (arquivoExiste(caminhoCompleto)) {
        printf("Arquivo já existe: %s\n\n", caminhoCompleto);
        return 1; // Retorna 1 para indicar que o arquivo já existe
    }

    // Abre o arquivo para escrita
    FILE *arquivo = fopen(caminhoCompleto, "wb");
    if (!arquivo) {
        perror("Não foi possível abrir o arquivo");
        return -1; // Retorna -1 se não conseguir abrir o arquivo
    }

    Registro reg;
    int chaveAtual = (modo == 2) ? (quantidade + 1) : 0; // Define a chave inicial com base no modo

    // Gera e escreve cada registro individualmente no arquivo
    for (int i = 0; i < quantidade; i++) {
        // Gera a chave do registro de acordo com o modo especificado
        if (modo == 1) { // Modo ascendente
            chaveAtual += 1;
        } else if (modo == 2) { // Modo descendente
            chaveAtual -= 1;
        } else if (modo == 3) { // Modo aleatório
            chaveAtual = rand() % (quantidade * 1000) + 1;
        }

        gerarDadosAleatorios(&reg, chaveAtual); // Gera dados aleatórios para o registro

        // Utiliza a função escreverRegistro para escrever o registro no arquivo
        escreverRegistro(arquivo, i, &reg);
    }

    fclose(arquivo); // Fecha o arquivo

    printf("Arquivo criado: %s\n", caminhoCompleto);
    return 1; // Retorna 1 para indicar sucesso na criação do arquivo
}

