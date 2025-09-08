/*
Este código foi baseado no da Profa. Silvana, modificado por mim.
Autor (modificações): Matheus Magalhães Nascimento Silva | DRE: 123504503.
*/
/* Codigo: Comunicação entre threads usando variável compartilhada,
* exclusao mutua com bloqueio e sincronização condicional
*/

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

long int soma = 0;               // variavel compartilhada
pthread_mutex_t mutex;           // lock
pthread_cond_t cond;             // variavel de condição
int pronto = 0;                  // sinalizador

// funcao executada pelas threads de soma
void *ExecutaTarefa (void *arg) {
  long int id = (long int) arg;
  printf("Thread : %ld esta executando...\n", id);

  for (int i=0; i<100000; i++) {
     pthread_mutex_lock(&mutex);

     // <<< MUDANÇA MINIMA: se ja existe um multiplo pendente, aguarda
     while (pronto)
         pthread_cond_wait(&cond, &mutex);

     soma++; 

     if (soma % 1000 == 0) {
        pronto = 1;
        pthread_cond_signal(&cond);    // acorda a extra
        // espera a extra imprimir
        while (pronto)
            pthread_cond_wait(&cond, &mutex);
     }

     pthread_mutex_unlock(&mutex);
  }

  printf("Thread : %ld terminou!\n", id);
  pthread_exit(NULL);
}

// funcao executada pela thread extra
void *extra (void *args) {
  long int nthreads = (long int) args;
  long int limite = 100000 * nthreads;  // Valor final esperado.

  printf("Extra : esta executando...\n");

  while (1) {
     pthread_mutex_lock(&mutex);

     while (!pronto && soma < limite)
         pthread_cond_wait(&cond, &mutex);

     if (pronto) {
        printf("soma = %ld \n", soma);
        pronto = 0;
        pthread_cond_broadcast(&cond); // Libera as threads de soma.
     }

     if (soma >= limite) { // Critério de parada.
        pthread_mutex_unlock(&mutex);
        break;
     }

     pthread_mutex_unlock(&mutex);
  }

  printf("Extra : terminou!\n");
  pthread_exit(NULL);
}

// Fluxo principal:
int main(int argc, char *argv[]) {
   pthread_t *tid; 
   int nthreads; 

   if(argc<2) {
      printf("Digite: %s <numero de threads>\n", argv[0]);
      return 1;
   }
   nthreads = atoi(argv[1]);

   tid = (pthread_t*) malloc(sizeof(pthread_t)*(nthreads+1));
   if(tid==NULL) {puts("ERRO--malloc"); return 2;}

   pthread_mutex_init(&mutex, NULL);
   pthread_cond_init(&cond, NULL);

   for(long int t=0; t<nthreads; t++) {
     if (pthread_create(&tid[t], NULL, ExecutaTarefa, (void *)t)) {
       printf("--ERRO: pthread_create()\n"); exit(-1);
     }
   }

   // cria thread de log:
   if (pthread_create(&tid[nthreads], NULL, extra, (void*)(long)nthreads)) {
      printf("--ERRO: pthread_create() extra\n"); exit(-1);
   }

   // <<< MUDANÇA MINIMA: primeiro espera apenas os trabalhadores
   for (int t=0; t<nthreads; t++) {
     if (pthread_join(tid[t], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
     } 
   }

   // Acorda a `extra` caso ela esteja esperando (evita deadlock):
   pthread_mutex_lock(&mutex);
   pthread_cond_broadcast(&cond);
   pthread_mutex_unlock(&mutex);

   // Agora espera a thread extra terminar:
   if (pthread_join(tid[nthreads], NULL)) {
       printf("--ERRO: pthread_join() extra\n"); exit(-1);
   }

   pthread_mutex_destroy(&mutex);
   pthread_cond_destroy(&cond);

   printf("Valor final de 'soma' = %ld\n", soma);
   return 0;
}