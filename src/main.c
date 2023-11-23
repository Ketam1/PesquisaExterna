#include "pesquisa/pesquisa.h"
#include "registro/registro.h"
#include "util/util.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 5 || argc > 6) {
        fprintf(stderr, "Uso: %s <método> <quantidade> <situação> <chave> [-P]\n", argv[0]);
        return 1;
    }

    int metodo = atoi(argv[1]);
    int quantidade = atoi(argv[2]);
    int situacao = atoi(argv[3]);
    int chave = atoi(argv[4]);
    int exibirChaves = (argc == 6 && argv[5][0] == '-' && argv[5][1] == 'P');

    // Verificar se os argumentos são válidos
    if (metodo < 1 || metodo > 4 || situacao < 1 || situacao > 3) {
        fprintf(stderr, "Argumentos inválidos.\n");
        return 1;
    }

    char nomeArquivo[100];
    char caminhoCompleto[260]; 
    const char *situacaoStr = situacao == 1 ? "asc" : (situacao == 2 ? "desc" : "rand");
    sprintf(nomeArquivo, "teste_%s_%d.bin", situacaoStr, quantidade);
    sprintf(caminhoCompleto, "testes/%s", nomeArquivo);

    if (gerarArquivo(caminhoCompleto, quantidade, situacao) != 1) {
        fprintf(stderr, "Falha ao criar o arquivo.\n");
        return 1; // Encerra o programa em caso de falha
    }

    if (exibirChaves) {
        exibirRegistros(caminhoCompleto, quantidade);
    }

    switch (metodo) {
        case 1:
            acessoSequencialIndexado(caminhoCompleto, chave, exibirChaves);
            break;
        case 2:
            arvoreBinariaPesquisa(caminhoCompleto, chave, exibirChaves);
            break;
        case 3:
            arvoreB(caminhoCompleto, chave, exibirChaves);
            break;
        case 4:
            arvoreBStar(caminhoCompleto, chave, exibirChaves);
            break;
        default:
            fprintf(stderr, "Método de pesquisa inválido.\n");
            return 1;
    }

    return 0;
}
