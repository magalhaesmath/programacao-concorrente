/*
Autor: Matheus Magalhães Nascimento Silva | DRE: 123504503
Meu 1o programa concorrente, do Lab. 1.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Ponteiro p/ vetor declarado de forma global:
int *vetor;

// Estrutura de dados de argumento p/ função de thread:
typedef struct {
    long int ini, fim, id_thread;
} t_args;

// Função de thread:
void *incrementaVetor(void* arg) {
    // Variáveis:
    t_args *args = (t_args *) arg; // Type Cast: void -> t_args.

    for(long int i = args->ini; i<args->fim; i++){
        printf("%lda thread executando...\n", args->id_thread);
        printf("Elemento %ld = %d (antes do incremento).\n", i, *(vetor+i));
        *(vetor+i) += 1; // Incrementa o elemento atual em 1.
        printf("Elemento %ld = %d (depois do incremento).\n", i, *(vetor+i));
    }

    pthread_exit(NULL); // Finaliza a thread chamadora (atual).
}

// Função main():
int main(int argc, char* argv[]){
    // Verificando se os argumentos foram passados de forma correta:
    if (argc < 2){
        fprintf(stderr, "ERRO na passagem de argumentos. Digite: ./%s <n>\n", argv[0]);
    }
    
    // Variáveis:
    t_args* args;
    long int n = atoi(argv[1]);
    pthread_t tid_aplicacao[4]; // Vetor de TID's.
    int inicio = 0;

    // Alocando memória p/ vetor:
    vetor = malloc(4*n*sizeof(int));

    for(int i = 1; i<=4; i++){
        // Iniciando o processo de criação da thread:
        printf("CRIANDO a thread %d\n", i);

        // Alocando memória p/ args:
        args = malloc(sizeof(t_args));

        // Configura os argumentos da função de thread, atribuindo a fatia da thread:
        args->ini = inicio;
        args->fim = inicio + n; 
        args->id_thread = i;
        
        // Atualiza a variável 'inicio':
        inicio+=n;

        // Cria a thread:
        if(pthread_create(&tid_aplicacao[i-1], NULL, incrementaVetor, (void *) args)){
            fprintf(stderr, "ERRO ao criar a thread %d.\n", i);
            return 2;
        }
    }

    // Espera todas as threads terminarem:
    for (int i = 0; i<4; i++){
        if(pthread_join(tid_aplicacao[i], NULL)){
            fprintf(stderr, "ERRO na finalização da thread %d.\n", i);
        }
    }

    printf("FIM da aplicação.\n");

    return 0;
}