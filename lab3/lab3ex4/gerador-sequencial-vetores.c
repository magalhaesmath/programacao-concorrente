/*
Matheus Magalhães Nascimento Silva | DRE: 123504503
*/

/*
Programa auxiliar para gerar DOIS vetores de floats
e salvar em arquivo binário.
*/

#include <stdio.h>
#include <stdlib.h> 
#include <time.h> // Biblioteca para gerar números aleatórios com base no tempo.
#include "timer.h" // Biblioteca p/ contagem do tempo de execução usada pela Profa. Silvana.

// Tamanho máximo do vetor:
#define MAX 1000

// LOG para que os vetores sejam impressos no terminal ao fim da execução:
#define LOG

// Função principal do programa:
int main(int argc, char* argv[]) {
    // Variáveis:
    float *vetor1, *vetor2; // Os vetores de saída.
    long int n; // Dimensão dos vetores.
    float elem; // Elemento aleatório a ser inserido no vetor.
    FILE *descritorArquivo; // Descritor do arquivo de saída.
    size_t ret; 

    // Variáveis para medir o tempo com timer.h (biblioteca que a Profa. Silvana usou em sala):
    double start, finish, tempo_execucao;
    GET_TIME(start);

    // Verifica se a entrada no terminal foi errada:
    if (argc < 3) {
        fprintf(stderr, "Digite: %s <dimensao> <nome arquivo saida>\n", argv[0]);
        return 1;
    }
    n = atol(argv[1]); // Pega a dimensão dos vetores.

    // Aloca memória pros vetores.
    vetor1 = (float*) malloc(sizeof(float) * n);
    vetor2 = (float*) malloc(sizeof(float) * n);
    
    // Verifica se houve erro de alocação em algum dos vetores:
    if (!vetor1 || !vetor2) {
        fprintf(stderr, "Erro de alocacao da memoria dos vetores\n");
        return 2;
    }

    srand(time(NULL));

    // Preenche os dois vetores com floats aleatórios:
    for (long int i = 0; i < n; i++) {
        elem = (rand() % MAX) / 3.0f;
        vetor1[i] = elem;

        elem = (rand() % MAX) / 3.0f;
        vetor2[i] = elem;
    }
    
    // Verifica se o LOG está definido:
    #ifdef LOG  

    // Identifica o programa no terminal"
    printf("\n------- Gerador sequencial de vetores -------\n");
    
    // Imprime o vetor 1:
    fprintf(stdout, "%ld\n", n);
    fprintf(stdout, "\nVetor 1:\n");
    for (long int i = 0; i < n; i++) fprintf(stdout, "%f ", vetor1[i]);

    // Imprime o vetor 2:
    fprintf(stdout, "\n\nVetor 2:\n");
    for (long int i = 0; i < n; i++) fprintf(stdout, "%f ", vetor2[i]);
    #endif

    // Escreve no arquivo binário:
    descritorArquivo = fopen(argv[2], "wb");
    if (!descritorArquivo) {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 3;
    }

    ret = fwrite(&n, sizeof(long int), 1, descritorArquivo);
    ret = fwrite(vetor1, sizeof(float), n, descritorArquivo);
    ret = fwrite(vetor2, sizeof(float), n, descritorArquivo);

    // Fecha o descritor do arquivo:
    fclose(descritorArquivo);

    // Desaloca a memória alocada pros vetores:
    free(vetor1);
    free(vetor2);

    // Finaliza a contagem de tempo de execução:
    GET_TIME(finish);

    // Calcula o tempo corrido de execução:
    tempo_execucao = finish - start;

    printf("\nTempo de execução: %.6f segundos\n", tempo_execucao);

    printf("\n-------------------- FIM --------------------\n\n");
    return 0;
}