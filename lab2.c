/*
Matheus Magalhães Nascimento Silva | DRE: 123504503
*/

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

// Declaração global do vetor:
int *vetor;

// Estrutura de dados para os argumentos:
typedef struct {
    short int id; // Identificador da thread na aplicação.
    short int nthreads; // Número de threads.
    long int dim; // Dimensão do vetor.
} t_args;

// Função que será aplicada sobre cada elemento do vetor (executada por cada thread):
void* f(void *args){
    t_args *arg = (t_args*) args;
    long int fatia, ini, fim;
    fatia = arg->dim / arg->nthreads; // Fatia de elementos por thread.
    ini = arg->id * fatia;
    fim = ini + fatia;
    
    if(arg->id == arg->nthreads-1) fim = arg->dim;
    
    for(long int i=ini; i<fim; i++){
        *(vetor+i) += 1;
    }
    
    free(args); // Libera a memória alocada na main.
    pthread_exit(NULL);
}

// Função para alocar espaço e inicializar o vetor:
short int inicializa_vetor(int **vet, long int dim){
    *vet = (int *) malloc(sizeof (int) * dim); 
    
    if(*vet == NULL){
        fprintf(stderr, "ERRO de alocaçao de memoria\n");
        return 1;
    }
    
    for(long int i = 0; i<dim; i++){
        *(*vet + i) = (int)i;     
    }
    
    return 0; // Sucesso.
}

// Impressão do vetor:
void imprime_vetor(int *vetor, long int dim){
    for (long int i = 0; i < dim; i++){
        printf("%d ", *(vetor + i));
    }
    printf("\n");
}

// Função principal do programa:
int main(int argc, char* argv[]){
    // Variáveis:
    short int nthreads; // Qtde. de threads.
    long int dim; // Dimensão do vetor.
    
    // Validação de variáveis:
    if(argc < 3){
        fprintf(stderr, "ERRO de entrada, digite: <%s> <dimensao do vetor> <qtde de threads>\n", argv[0]);
        return 1;
    }
    
    // Inicialização das variáveis:
    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    pthread_t tid[nthreads];
    
    // Inicialização do vetor:
    if(inicializa_vetor(&vetor, dim)){
        fprintf(stderr, "ERRO de inicialização do vetor\n");
        return 2;
    }
    
    // Impressão do vetor:
    imprime_vetor(vetor, dim);
    
    // Criação das threads:
    for(short int i = 0; i < nthreads; i++){
        // Carregar os argumentos para thread:
        t_args *args = (t_args *) malloc(sizeof(t_args));
        
        // Verificação da criação de args:
        if (args==NULL){
            fprintf(stderr, "ERRO de criacao da estrutura de argumentos\n");
            return 3;
        }
        
        args -> id = i;
        args -> nthreads = nthreads;
        args -> dim = dim;
        
        if(pthread_create(&tid[i], NULL, f, (void *) args)){
            fprintf(stderr, "ERRO de criacao de thread %hd\n", i);
            return 4;
        }
    }
    
    // Tempo das threads terminarem:
    for(short int i=0; i<nthreads; i++){
        pthread_join(tid[i], NULL);
    }
    
    // Verificação dos resultados:
    imprime_vetor(vetor, dim);
    
    // Encerramento:
    free(vetor);
    return 0;
}