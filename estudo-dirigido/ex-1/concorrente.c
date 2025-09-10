/*
Exercício 1 - Fórmula de Bailey–Borwein–Plouffe (BBP) - Concorrente
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

// Variável global de pi:
long double pi = 0;

// Estrutura de dados p/ args da thread:
typedef struct {
    int ini, fim, id;
} t_args;

// Função de thread:
void *bailey(void* arg){
    // Variáveis:
    t_args* args = (t_args *) arg; // Type Cast: void * -> t_args *.

    printf("-- Thread %d iniciando...\n", args->id);

    // Cálculo da fórmula de Bailey:
    for(int i = args->ini; i<args->fim; i++){
        pi += (4.0 / (8.0 * i + 1.0)
             - 2.0 / (8.0 * i + 4.0)
             - 1.0 / (8.0 * i + 5.0)
             - 1.0 / (8.0 * i + 6.0))
             / pow(16.0, i);
    }

    printf("-- Thread %d acabou.\n", args->id);

    // Encerra a thread atual.
    pthread_exit(NULL);
}

// Função main():
int main(int argc, char* argv[]){
    // Verificando se os args foram passados de forma correta:
    if (argc<3) {
        fprintf(stderr, "-- ERRO na passagem de argumentos. Digite: %s <# de iteracoes> <# de threads>\n", argv[0]);
        return 1;
    }

    // Variáveis:
    int n = atoi(argv[1]);
    int n_threads = atoi(argv[2]);
    int ini = 0;
    int fatia = n/n_threads;
    pthread_t tids[n_threads];
    t_args* args;

    // Criação das threads:
    for(int i = 1; i<= n_threads; i++){

        // Alocação de memória p/ args:
        args = malloc(sizeof(t_args));
        
        // Preenchendo args:
        args->id = i;
        args->ini = ini;
        if(args->ini + fatia > n){
            int excesso = (args->ini + fatia)%n;
            args->fim = (args->ini + fatia) - excesso;
        } else {
            args->fim = args->ini + fatia;
        }

        // Criando thread:
        if(pthread_create(&tids[i-1], NULL, bailey, (void *) args)){
            fprintf(stderr, "-- ERRO na criacao da thread %d.\n", i);
            return 2;
        }
    }

    // Aguardando o fim de todas as threads:
    for(int i = 1; i<=n_threads; i++){
        if(pthread_join(tids[i-1], NULL)){
            fprintf(stderr, "-- ERRO na finalizacao da thread %d.\n", i);
            return 3;
        }
    }
    
    printf("Pi = %Lf", pi);
    printf("-- FIM\n");

    return 0;
}