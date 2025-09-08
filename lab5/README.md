# Laboratório 5 (Ex. 4) — Sincronização Condicional com Threads (ICP361)

## Autor
Matheus Magalhães Nascimento Silva  
DRE: 123504503  

---

## 🎯 Objetivo
Este programa implementa **comunicação e sincronização entre threads** em C, utilizando:
- Variável compartilhada (`soma`);
- Exclusão mútua com `pthread_mutex_t`;
- Variáveis de condição (`pthread_cond_t`) para coordenação condicional.

O requisito é que uma thread **extra** imprima todos os valores da soma que são **múltiplos de 1000**, e que as threads de soma **pausem a execução** até que a thread extra finalize a impressão.

---

## ⚙️ Descrição do Programa
- São criadas **N threads de soma**, que incrementam a variável global `soma` até um valor limite.  
- A cada múltiplo de 1000 atingido:
  - A thread de soma **sinaliza a thread extra** (`pthread_cond_signal`).
  - Ela **aguarda** (`pthread_cond_wait`) até a thread extra imprimir o valor.  
- A **thread extra**:
  - Recebe o sinal, imprime o valor de `soma` quando for múltiplo de 1000.
  - Libera as threads de soma (`pthread_cond_broadcast`).
  - Encerra sua execução quando `soma` atingir o valor final esperado (`100000 * N`).

---

## 🧵 Estrutura do Código
- `ExecutaTarefa`: função executada pelas threads de soma.
- `extra`: função da thread de log, responsável por imprimir múltiplos de 1000.
- `main`:
  - Lê o número de threads via linha de comando.
  - Inicializa `mutex` e `cond`.
  - Cria `N` threads de soma e 1 thread extra.
  - Aguarda término das threads de soma e, em seguida, da thread extra.
  - Finaliza mutex e variável de condição.

---

## ▶️ Como Compilar e Executar
Compile com **pthread**:

```bash
gcc -o lab5 lab5.c -lpthread
```

Execute informando o número de threads:

```bash
./lab5 2
```