# 💻 Laboratório 8 — Produtor/Consumidor com Lotes Completos (Threads e Semáforos)
# Autor: Matheus Magalhães Nascimento Silva 
# Disciplina: Programação Concorrente (ICP361)
Este laboratório implementa o **problema Produtor/Consumidor** com as seguintes particularidades:

- A **thread produtora** só insere elementos **quando o buffer estiver completamente vazio**.  
- Cada inserção (ou *lote*) **preenche o buffer inteiro** (com `M` elementos), exceto o último lote, que pode ter menos.  
- As **threads consumidoras** retiram elementos **um por vez** e testam se cada número é primo.  
- A comunicação e sincronização são feitas usando **semáforos POSIX (`sem_t`)** e **threads (`pthread`)**.  
- Logs detalhados indicam o progresso de produção e consumo.

---

## ⚙️ Estrutura do Projeto

| Arquivo | Descrição |
|----------|------------|
| `lab8.c` | Código-fonte principal em C (produtor/consumidor) |
| `README.md` | Este arquivo de documentação |
| `testes.txt` | Arquivo que contém os testes realizados e seus respectivos resultados |
---

## 🧠 Conceitos Utilizados

- **Threads (`pthread`)** — execução concorrente de produtor e consumidores  
- **Semáforos (`sem_t`)** — controle de acesso ao buffer:
  - `sem_empty`: quantos espaços vazios restam  
  - `sem_full`: quantos itens estão disponíveis  
  - `sem_mutex`: exclusão mútua no acesso ao buffer  
  - `sem_print`: evita sobreposição de mensagens no terminal  
- **Buffer circular** — armazenamento compartilhado de tamanho `M`  
- **Primalidade** — verificação simples via divisão até √n

---

## 🧩 Compilação

Para compilar o programa, faça:

```c
gcc lab8.c -o lab8 -lm
```

## ▶️ Execução

O programa recebe três parâmetros de linha de comando:

```
./lab8 <N> <M> <T>
```

| Parâmetro | Significado                                      |
| --------- | ------------------------------------------------ |
| `N`       | Total de números a produzir (de 1 até N)         |
| `M`       | Tamanho do buffer (número de elementos por lote) |
| `T`       | Número de threads consumidoras                   |


## 🧾 Exemplo de Execução

```
./lab8 10 3 2
```
## 🔍 Logs Importantes

### 🏭 Produtor
- Indica o **início e fim de cada lote**.  
- Mostra o **intervalo de números inseridos** (ex: `Inserindo 3 números (1 até 3)`).
- **Aguarda o buffer ficar vazio** antes de iniciar uma nova inserção.  
- Insere **sentinelas (`0`) ao final** para sinalizar o término dos consumidores.  
- Exemplo:
```
[PRODUTOR] Buffer vazio detectado — iniciando lote 1
[PRODUTOR] Inserindo 3 números (1 até 3)
[PRODUTOR] Lote 1 finalizado — buffer contém 3 elementos
...
[PRODUTOR] Inserindo 2 sentinelas (0) para encerrar consumidores...
[PRODUTOR] Inserção completa. Encerrando thread produtora.
```

### 👷 Consumidor
- Indica quando **encontra um número primo** e quantos já encontrou até o momento.  
- **Finaliza** ao consumir uma **sentinela (`0`)**.  
- Mostra o **total de primos encontrados** por cada thread.  
- Exemplo:
```
Thread 0: encontrou o primo 2 (total até agora: 1)
Thread 1: encontrou o primo 3 (total até agora: 1)
...
Thread 0 finalizou — total de primos encontrados: 2
```

## 🏁 Critérios de Corretude

O programa é considerado **correto** se todas as condições abaixo forem satisfeitas:

### ✅ (1) Processamento Completo
- Todos os números no intervalo **1 até N** são produzidos e consumidos **exatamente uma vez**.
- Nenhum número é perdido ou duplicado durante a execução.

### ✅ (2) Encerramento Controlado
- Cada **thread consumidora** encerra corretamente após ler uma **sentinela (`0`)**.
- O número de sentinelas inseridas é **igual ao número de consumidores (T)**.

### ✅ (3) Validação Matemática
- O **total de primos encontrados** corresponde ao valor esperado matematicamente
  (em testes pequenos, foi checado manualmente; em testes maiores, foi checado com o auxílio de LLM's).

### ✅ (4) Integridade do Buffer
- O **buffer nunca excede** o limite máximo de **M elementos**.
- O contador de elementos **nunca assume valor negativo**.

### ✅ (5) Ausência de Deadlocks
- Nenhuma thread fica bloqueada indefinidamente.
- Todas as threads (produtora e consumidoras) **finalizam corretamente**.
