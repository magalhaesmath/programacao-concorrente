/*
Matheus Magalhães Nascimento Silva | DRE: 123504503
*/

/*
Programa sequencial para ler dois vetores de floats e suas dimensões de um arquivo binário,
calcular o produto interno entre eles e gravar o resultado no mesmo arquivo, além de exibir
o tempo de execução no terminal, ao fim da execução.
*/

#include <stdio.h>
#include <stdlib.h>
#include "timer.h" // Biblioteca p/ contagem do tempo de execução usada pela Profa. Silvana.

int main(int argc, char* argv[]) {
    // Variáveis:
    long int n; // Dimensão dos vetores.
    float *vetor1, *vetor2; // Ponteiros pros vetores.
    double produtoInterno = 0.0; // Variável para armazenar o prod. interno.
    FILE *arquivo; // Ponteiro pro arquivo de entrada.
    size_t ret;

    // Variáveis para medir o tempo com timer.h (biblioteca que a Profa. Silvana usou em sala):
    double start, finish, tempo_execucao;
    GET_TIME(start);

    // Verifica se o nome do arquivo foi passado:
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo entrada>\n", argv[0]);
        return 1;
    }

    // Abre o arquivo binário para leitura e escrita:
    arquivo = fopen(argv[1], "rb+");

    // Verifica se houve erro ao abrir o arquivo:
    if (!arquivo) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", argv[1]);
        return 2;
    }

    // Lê a dimensão dos vetores:
    ret = fread(&n, sizeof(long int), 1, arquivo);

    // Verifica se houve erro ao ler a dimensão:
    if (ret != 1) {
        fprintf(stderr, "ERRO ao ler dimensão do arquivo\n");
        fclose(arquivo);
        return 3;
    }

    // Aloca memória para os vetores:
    vetor1 = (float*) malloc(sizeof(float) * n);
    vetor2 = (float*) malloc(sizeof(float) * n);

    // Verifica se houve erro na alocação de memória de algum dos vetores:
    if (!vetor1 || !vetor2) {
        fprintf(stderr, "Erro de alocação de memória\n");
        fclose(arquivo);
        return 4;
    }

    // Lê os vetores:
    ret = fread(vetor1, sizeof(float), n, arquivo);
    ret = fread(vetor2, sizeof(float), n, arquivo);

    // Calcula o produto interno dos 2 vetores:
    for (long int i = 0; i < n; i++) {
        produtoInterno += vetor1[i] * vetor2[i];
    }

    // Exibe no terminal o resultado obtido:
    printf("\n------- Produto Interno Sequencial -------\n");
    printf("\nDimensão: %ld\n", n);
    printf("Produto interno calculado: %.6f\n", produtoInterno);

    // Posiciona o ponteiro no fim do arquivo para gravar o produto interno calculado:
    fseek(arquivo, 0, SEEK_END);
    ret = fwrite(&produtoInterno, sizeof(double), 1, arquivo);

    // Verifica se houve erro na escrita e, não havendo, indica sucesso. Caso contrário, falha:
    if (ret != 1) {
        fprintf(stderr, "ERRO ao escrever o produto interno no arquivo\n");
        return 5;
    } else {
        printf("\nProduto interno gravado no arquivo com sucesso!\n");
    }

    // Finaliza a contagem de tempo de execução:
    GET_TIME(finish);

    // Calcula o tempo corrido de execução:
    tempo_execucao = finish - start;

    printf("\nTempo de execução: %.6f segundos\n", tempo_execucao);
    printf("\n---------------------- FIM ----------------------\n");

    // Libera memória e fecha o arquivo:
    free(vetor1);
    free(vetor2);
    fclose(arquivo);

    return 0;
}