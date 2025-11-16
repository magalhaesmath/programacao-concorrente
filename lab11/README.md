# Laboratório 11: Pools de Threads e Futures

Este laboratório explora a implementação e o uso de pools de threads em Java para resolver problemas de processamento concorrente.

* **Atividade 1:** Foca na implementação de um pool de threads "manual" a partir dos mecanismos básicos de `wait()` e `notify()`.
* **Atividade 3:** Utiliza o framework `ExecutorService` e `Future` do pacote `java.util.concurrent` para executar tarefas assíncronas e agregar seus resultados.

---

## 🚀 Atividade 1: Pool de Threads Manual

### Descrição

O objetivo era criar uma classe (`FilaTarefas`) que gerencia um conjunto fixo de threads "trabalhadoras" e uma fila de tarefas (`Runnable`). As threads consomem tarefas da fila assim que elas ficam disponíveis, implementando o padrão produtor-consumidor.

### Implementação

O programa principal (`MyPool`) submete tarefas da classe `Primo` (que verifica se um número é primo) ao pool.

Foram feitas algumas modificações para facilitar os testes:
1.  **Loop Principal:** O loop na classe `MyPool` foi modificado para começar em 1 (em vez de 0), tornando a sequência de verificação (1 a 25) mais intuitiva.
2.  **Instrumentação de Log:** A classe `Primo` foi alterada para que seu método `run()` imprima não apenas o resultado da verificação, mas também o **ID da thread** (`Thread.currentThread().getName()`) responsável pela operação.

### Testes e Validação

Para avaliar a corretude do programa, a estratégia foi:

1.  **Teste com Valor Baixo:** Foi usado um intervalo pequeno (1 a 25), onde os resultados (quais números são primos) podem ser facilmente verificados manualmente.
2.  **Verificação de Concorrência:** Ao variar o número de threads (1, 2, 4 e 8) e observar os logs instrumentados, foi possível confirmar visualmente que o trabalho estava, de fato, sendo distribuído entre as diferentes threads do pool. Isso validou que o mecanismo de concorrência estava funcionando e que as tarefas não estavam sendo executadas sequencialmente ou por uma única thread.

Os resultados detalhados desta verificação estão no arquivo `atividade1/results.txt`.

---

## ⚙️ Atividade 3: ExecutorService e Future

### Descrição

O objetivo era usar a API de alto nível `ExecutorService` para calcular a quantidade total de números primos em um intervalo `[1, N]`, onde `N` é um valor grande.

### Implementação

A classe `VerificaPrimoCallable` foi criada para implementar `Callable<Long>`, retornando `1L` se o número for primo e `0L` caso contrário.

O programa principal (`FutureHello`) submete `N` tarefas ao pool e armazena os `Future`s em uma lista. Ao final, ele itera sobre a lista, somando os resultados obtidos de `future.get()` para obter a contagem total.

Para facilitar os testes, o código foi modificado para que o **Número de Threads** e o **valor de N** fossem passados como **argumentos de linha de comando**.

### Testes e Validação

A avaliação foi focada na robustez e precisão em larga escala:

1.  **Teste de Escala (Variação de `N`):** Foram usados valores de `N` progressivamente maiores: **100.000**, **1.000.000** e **10.000.000**.
2.  **Teste de Regressão (Variação de Threads):** Para *cada* valor de `N` acima, o programa foi executado variando o número de threads (1, 2, 4, 8 e 16).
3.  **Verificação de Corretude:** Os resultados (contagem total de primos) foram comparados com valores de referência obtidos pela internet e com o auxílio de LLMs.

Todos os testes produziram resultados corretos, confirmando que a lógica de agregação estava correta e que o número de threads não interferiu na precisão do resultado final.

---

## 🛠️ Como Compilar e Executar

Este projeto usa um `Makefile` para automatizar a compilação e os testes.

### Pré-requisitos
* Java Development Kit (JDK)
* `make`

### Compilação
Para compilar todas as classes Java do projeto:
```bash
make
```

### Execução da Atividade 1
(Requer compilação prévia)
```bash
java MyPool
```

### Execução da Atividade 3 (Teste Único)
Você pode executar a Atividade 3 com valores personalizados de `THREADS` e `N`:
```bash
# Exemplo: 8 threads e N = 500.000
make run THREADS=8 N=500000
```

### Execução da Bateria de Testes (Atividade 3)
Para executar automaticamente todos os 15 cenários de teste (3 valores de `N` x 5 contagens de `THREADS`), use:
```bash
make test
```

### Limpeza
Para remover todos os arquivos `.class` e o `results.txt`:
```bash
make clean
```