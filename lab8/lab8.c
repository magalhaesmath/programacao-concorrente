/*
Autor: Matheus Magalhães Nascimento Silva.
Laboratório 8 — Produtor/Consumidor com lotes completos
Compilar: gcc lab8.c -o lab8 -pthread
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

static int N = 0; /* total de números */
static int M = 0; /* tamanho do buffer */
static int T = 0; /* número de threads consumidoras */

int *buffer = NULL;
int buf_in = 0;
int buf_out = 0;
int buf_count = 0;

sem_t sem_empty;
sem_t sem_full;
sem_t sem_mutex;
sem_t sem_print;

int *primos_por_thread = NULL;

int ehPrimo(int n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int i = 3; i * i <= n; i += 2)
        if (n % i == 0) return 0;
    return 1;
}

void *produtor(void *arg) {
    (void)arg;
    int next = 1;
    int lote = 0;

    while (next <= N) {
        /* aguarda buffer ficar vazio */
        while (1) {
            sem_wait(&sem_mutex);
            int vazio = (buf_count == 0);
            sem_post(&sem_mutex);
            if (vazio) break;
            usleep(1000);
        }

        lote++;
        int restante = N - next + 1;
        int k = (restante < M) ? restante : M;

        sem_wait(&sem_print);
        printf("\n[PRODUTOR] Buffer vazio detectado — iniciando lote %d\n", lote);
        printf("[PRODUTOR] Inserindo %d números (%d até %d)\n", k, next, next + k - 1);
        sem_post(&sem_print);

        /* reserva k slots */
        for (int i = 0; i < k; i++) sem_wait(&sem_empty);

        /* insere lote */
        sem_wait(&sem_mutex);
        for (int i = 0; i < k; i++) {
            buffer[buf_in] = next;
            buf_in = (buf_in + 1) % M;
            buf_count++;
            next++;
        }
        sem_post(&sem_mutex);

        /* libera k itens */
        for (int i = 0; i < k; i++) sem_post(&sem_full);

        sem_wait(&sem_print);
        printf("[PRODUTOR] Lote %d finalizado — buffer contém %d elementos\n", lote, buf_count);
        sem_post(&sem_print);
    }

    /* --- CORREÇÃO AQUI: aguarda buffer esvaziar totalmente antes de inserir sentinelas --- */
    sem_wait(&sem_print);
    printf("\n[PRODUTOR] Aguardando buffer esvaziar completamente antes de inserir sentinelas...\n");
    sem_post(&sem_print);

    int vazios = 0;
    while (1) {
        if (sem_getvalue(&sem_empty, &vazios) != 0) {
            /* sem_getvalue falhou — apenas espere e tente de novo */
            usleep(1000);
            continue;
        }
        if (vazios == M) break; /* buffer totalmente vazio */
        usleep(1000);
    }

    sem_wait(&sem_print);
    printf("[PRODUTOR] Inserindo %d sentinelas (0) para encerrar consumidores...\n", T);
    sem_post(&sem_print);

    /* agora é seguro escrever as sentinelas sem travar em sem_wait(&sem_empty) */
    sem_wait(&sem_mutex);
    for (int i = 0; i < T; i++) {
        buffer[buf_in] = 0;
        buf_in = (buf_in + 1) % M;
        buf_count++;
    }
    sem_post(&sem_mutex);

    for (int i = 0; i < T; i++) sem_post(&sem_full);

    sem_wait(&sem_print);
    printf("[PRODUTOR] Inserção completa. Encerrando thread produtora.\n\n");
    sem_post(&sem_print);

    return NULL;
}

void *consumidor(void *arg) {
    int id = *((int *) arg);
    free(arg);
    int cont_local = 0;

    while (1) {
        sem_wait(&sem_full);

        sem_wait(&sem_mutex);
        int val = buffer[buf_out];
        buf_out = (buf_out + 1) % M;
        buf_count--;
        sem_post(&sem_mutex);

        sem_post(&sem_empty);

        if (val == 0) break;

        if (ehPrimo(val)) {
            cont_local++;
            sem_wait(&sem_print);
            printf("Thread %d: encontrou o primo %d (total até agora: %d)\n",
                   id, val, cont_local);
            sem_post(&sem_print);
        }
    }

    primos_por_thread[id] = cont_local;

    sem_wait(&sem_print);
    printf("Thread %d finalizou — total de primos encontrados: %d\n", id, cont_local);
    sem_post(&sem_print);

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Uso: %s <N> <M> <T>\n", argv[0]);
        return EXIT_FAILURE;
    }

    N = atoi(argv[1]);
    M = atoi(argv[2]);
    T = atoi(argv[3]);

    if (N <= 0 || M <= 0 || T <= 0) {
        fprintf(stderr, "Parâmetros inválidos.\n");
        return EXIT_FAILURE;
    }

    buffer = malloc(sizeof(int) * M);
    primos_por_thread = calloc(T, sizeof(int));

    sem_init(&sem_empty, 0, M);
    sem_init(&sem_full, 0, 0);
    sem_init(&sem_mutex, 0, 1);
    sem_init(&sem_print, 0, 1);

    pthread_t prod;
    pthread_t *cons = malloc(sizeof(pthread_t) * T);

    pthread_create(&prod, NULL, produtor, NULL);

    for (int i = 0; i < T; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&cons[i], NULL, consumidor, id);
    }

    pthread_join(prod, NULL);
    for (int i = 0; i < T; i++) pthread_join(cons[i], NULL);

    int total = 0, maior = -1;
    int vencedor = 0;
    for (int i = 0; i < T; i++) {
        total += primos_por_thread[i];
        if (primos_por_thread[i] > maior) {
            maior = primos_por_thread[i];
            vencedor = i;
        }
    }

    printf("\n=== RESULTADO FINAL ===\n");
    printf("N = %d, M = %d, T = %d\n", N, M, T);
    printf("Total de primos encontrados: %d\n", total);
    printf("Thread vencedora: %d com %d primos\n", vencedor, primos_por_thread[vencedor]);

    sem_destroy(&sem_empty);
    sem_destroy(&sem_full);
    sem_destroy(&sem_mutex);
    sem_destroy(&sem_print);
    free(buffer);
    free(primos_por_thread);
    free(cons);

    return EXIT_SUCCESS;
}