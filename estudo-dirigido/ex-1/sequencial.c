/*
Exercício 1 - Fórmula de Bailey–Borwein–Plouffe (BBP) - Sequencial
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]) {
    // Verificando se os args foram passados corretamente
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <n termos>\n", argv[0]);
        return 1;
    }

    // Variáveis
    long int n = atol(argv[1]); // usar atol para segurança com números grandes
    double pi = 0.0;

    // Cálculo da fórmula BBP
    for (long int i = 0; i < n; i++) {
        pi += (4.0 / (8.0 * i + 1.0)
             - 2.0 / (8.0 * i + 4.0)
             - 1.0 / (8.0 * i + 5.0)
             - 1.0 / (8.0 * i + 6.0))
             / pow(16.0, i);
    }

    // Printa o resultado
    printf("Pi (%ld termos) = %.15f\n", n, pi);

    return 0;
}
