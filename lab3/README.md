# Matheus Magalhães Nascimento Silva | DRE: 123504503.

# Projeto: Produto Interno Sequencial e Concorrente em C

Este projeto tem como objetivo implementar e comparar o cálculo do **produto interno entre dois vetores** de forma **sequencial** e **concorrente**, utilizando **threads**. Além disso, são gerados vetores de entrada em arquivos binários para teste, e os tempos de execução são medidos para avaliar a performance.

---

## Estrutura do Projeto

### 1. **Gerador de Vetores**
Um programa em C que gera vetores de números aleatórios e os salva em arquivos binários.  
Cada arquivo contém:
- A dimensão `N` do vetor.
- Dois vetores de entrada de tamanho `N`.

Foram gerados arquivos de teste com dimensões:  
- **R2, R5, R10, R20 e R100**.

---

### 2. **Produto Interno Sequencial**
Um programa em C que:
- Lê o valor de `N` e os dois vetores de um arquivo binário.
- Calcula o produto interno de forma sequencial (um único fluxo de execução).
- Mede o tempo de execução usando o arquivo auxiliar `timer.h`.

---

### 3. **Produto Interno Concorrente**
Um programa em C que:
- Lê o valor de `N` e os dois vetores de um arquivo binário.
- Recebe como entrada o número de threads `T`.
- Divide o cálculo do produto interno entre as `T` threads de forma balanceada.
- Mede o tempo de execução usando o arquivo auxiliar `timer.h`.

---

### 4. **Medição de Tempo**
O tempo de execução de cada programa foi obtido com auxílio do arquivo **`timer.h`**, que utiliza a função `gettimeofday` para calcular o tempo em segundos com precisão de microssegundos.

### 5. **Conteúdo Disponível no Repositório**

#### Códigos-fonte:

Gerador de vetores.

Produto interno sequencial.

Produto interno concorrente.

Arquivo timer.h para medição do tempo.

Arquivos binários de teste: R2, R5, R10, R20, R100.

#### Documento de resultados (.txt):

Contém todos os testes realizados.

Apresenta os tempos de execução obtidos para cada versão (sequencial e concorrente).