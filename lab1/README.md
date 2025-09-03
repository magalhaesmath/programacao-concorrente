# Laboratório 1 — Meu Primeiro Programa Concorrente  
## Disciplina: Programação Concorrente (ICP361) — UFRJ 2025.2  
**Autor:** Matheus Magalhães Nascimento Silva  
**DRE:** 123504503  

---

## 📌 Objetivo
Implementar o **primeiro programa concorrente** em C, utilizando **4 threads** para incrementar de 1 cada elemento de um vetor de `4*N` inteiros.  

Cada thread deve receber **uma fatia igual do vetor**, garantindo balanceamento da carga de trabalho. O valor de `N` é passado como argumento na execução do programa.

---

## 📝 Roteiro do Laboratório
1. Definir como dividir a tarefa entre as 4 threads.  
2. Implementar funções para inicializar e verificar o vetor.  
3. Criar as threads na função `main`, passar os argumentos adequados e aguardar o término com `pthread_join`.  
4. Validar os resultados e garantir que o programa funciona corretamente.  

---

## 📂 Estrutura do Código
- `vetor`: declarado de forma **global** e alocado dinamicamente.  
- `t_args`: estrutura usada para passar os argumentos (índice inicial, índice final e id da thread).  
- `incrementaVetor(void* arg)`: função executada pelas threads, responsável por incrementar os elementos do vetor.  
- `main()`: cria as threads, divide o trabalho e aguarda todas finalizarem.  

---

## ▶️ Como Compilar e Executar
No terminal, compile com o `gcc` e a biblioteca `pthread`:  

```bash
gcc -o lab1 lab1.c -lpthread
```

Execute informando o valor de N (cada thread processará N elementos totalizando 4*N elementos):

```bash
./lab1 5
```
