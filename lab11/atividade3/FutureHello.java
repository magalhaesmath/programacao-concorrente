/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 11 */
/* Codigo: Exemplo de uso de futures (MODIFICADO) */
/* -------------------------------------------------------------------*/

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.ArrayList;
import java.util.List;

/**
 * ATIVIDADE 2: Novo tipo de tarefa.
 * Esta classe implementa Callable e verifica se um unico numero eh primo.
 * Ela retorna 1L se for primo, e 0L se nao for.
 */
class VerificaPrimoCallable implements Callable<Long> {
    private long numero;

    // Construtor recebe o numero para verificar
    public VerificaPrimoCallable(long n) {
        this.numero = n;
    }

    /**
     * Funcao auxiliar para determinar se um numero eh primo.
     * (Baseada na logica C fornecida na Atividade 1)
     */
    private boolean ehPrimo(long n) {
        if (n <= 1) return false;
        if (n == 2) return true;
        if (n % 2 == 0) return false;
        
        // Loop traduzido de 'for(i=3; i< sqrt(n)+1; i+=2)'
        for (long i = 3; i < Math.sqrt(n) + 1; i += 2) {
            if (n % i == 0) {
                return false;
            }
        }
        return true;
    }

    // Método para execução (retorna 1L se primo, 0L se nao)
    @Override
    public Long call() throws Exception {
        if (ehPrimo(this.numero)) {
            return 1L;
        } else {
            return 0L;
        }
    }
}


//classe do método main
public class FutureHello  {
    
    // N e NTHREADS foram REMOVIDOS daqui

    public static void main(String[] args) {
        
        // --- INICIO DA MODIFICACAO: Analise dos argumentos ---
        
        if (args.length < 2) {
            System.err.println("Erro: Faltam argumentos.");
            System.err.println("Uso: java FutureHello <numero_de_threads> <limite_N>");
            System.exit(1);
        }

        int nThreads = 0;
        long nMax = 0; // 'N' sera lido como long para suportar valores grandes

        try {
            nThreads = Integer.parseInt(args[0]);
            nMax = Long.parseLong(args[1]); // Parse N como long
        } catch (NumberFormatException e) {
            System.err.println("Erro: Argumentos invalidos. Devem ser numeros inteiros.");
            System.err.println("Uso: java FutureHello <numero_de_threads> <limite_N>");
            System.exit(1);
        }

        if (nThreads <= 0 || nMax <= 0) {
             System.err.println("Erro: Os numeros devem ser positivos.");
             System.exit(1);
        }
        // --- FIM DA MODIFICACAO ---


        //cria um pool de threads (usando a variavel lida)
        ExecutorService executor = Executors.newFixedThreadPool(nThreads);
        
        //cria uma lista para armazenar referencias de chamadas assincronas (Futures)
        List<Future<Long>> list = new ArrayList<Future<Long>>();

        System.out.println("Usando " + nThreads + " threads.");
        System.out.println("Submetendo tarefas para verificar primos de 1 a " + nMax + "...");
        
        // ATIVIDADE 3: Submete N tarefas (usando a variavel lida)
        for (long i = 1; i <= nMax; i++) {
            // Cria a tarefa de verificacao para o numero 'i'
            Callable<Long> worker = new VerificaPrimoCallable(i);
            
            // Submete a tarefa ao pool e armazena o "futuro" na lista
            Future<Long> submit = executor.submit(worker);
            list.add(submit);
        }

        System.out.println(list.size() + " tarefas foram submetidas.");
        
        //recupera os resultados e faz o somatório final (contagem de primos)
        long sum = 0;
        System.out.println("Aguardando e somando resultados...");
        
        for (Future<Long> future : list) {
            try {
                // future.get() bloqueia ate a tarefa especifica terminar
                // e retorna o valor (1L ou 0L)
                sum += future.get(); 
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (ExecutionException e) {
                e.printStackTrace();
            }
        }
        
        // ATIVIDADE 3: Imprime a quantidade total de primos encontrados
        System.out.println("Total de numeros primos encontrados: " + sum);
        
        // Encerra o pool de threads
        executor.shutdown();
    }
}