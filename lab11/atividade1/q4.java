import java.util.LinkedList;

//-------------------------------------------------------------------------------
// CLASSE FILATAREFAS (Pool de Threads)
// Gerencia uma fila de tarefas e um conjunto de threads trabalhadoras.
//-------------------------------------------------------------------------------
class FilaTarefas {
	private final int nThreads;                 // Numero de threads no pool.
	private final MyPoolThreads[] threads;      // Array para armazenar as threads trabalhadoras.
	private final LinkedList<Runnable> queue;   // Fila de tarefas (recurso compartilhado).
	private boolean shutdown;                  // Flag para indicar o encerramento do pool.

	/**
		* Construtor: inicializa o pool com o numero especificado de threads.
		* @param nThreads Numero de threads trabalhadoras a serem criadas.
	*/
	public FilaTarefas(int nThreads) {
		this.shutdown = false;
		this.nThreads = nThreads;
		queue = new LinkedList<Runnable>();     // Inicializa a fila de tarefas.
		threads = new MyPoolThreads[nThreads];  // Inicializa o array de threads.

		// Cria e inicia todas as threads trabalhadoras.
		for (int i=0; i<nThreads; i++) {
			threads[i] = new MyPoolThreads();
			threads[i].start(); // Inicia a thread (ela comeca a executar o run()).
			} 
	}

	/**
		* Metodo publico para submeter uma nova tarefa (Runnable) ao pool.
		* @param r A tarefa a ser executada.
	*/
	public void execute(Runnable r) {
		// Obtem o lock da fila (exclusao mutua) para adicionar a tarefa.
		synchronized(queue) {
			if (this.shutdown) return; // Se o pool esta sendo desligado, rejeita a tarefa.
			queue.addLast(r);          // Adiciona a tarefa ao final da fila.
			queue.notify();           // Acorda UMA thread que esteja esperando (wait) na fila vazia.
			}
	}
    
	/**
		* Metodo para iniciar o desligamento (shutdown) ordenado do pool.
		* Nao aceita novas tarefas e espera as existentes terminarem.
	*/
	public void shutdown() {
		// Obtem o lock para modificar a flag shutdown e notificar as threads.
		synchronized(queue) {
			this.shutdown=true;     // Sinaliza para as threads que devem parar.
			queue.notifyAll();      // Acorda TODAS as threads em wait() para que verifiquem a flag shutdown.
		}
        
		// Espera que todas as threads trabalhadoras terminem sua execucao.
		for (int i=0; i<nThreads; i++) {
			try { threads[i].join(); } catch (InterruptedException e) { return; }
		}
	}

	/**
		* Classe interna que define a "thread trabalhadora" do pool.
		* Estende Thread e seu metodo run() contem a logica de execucao.
		*/
	private class MyPoolThreads extends Thread {
		public void run() {
			Runnable r;
        
			// Loop principal da thread trabalhadora: continua executando enquanto o pool estiver ativo.
			while (true) {
				// Obtem o lock da fila para verificar se ha tarefas.
				synchronized(queue) {
					// Condicao de espera: se a fila esta vazia E o pool nao foi desligado,
					// a thread libera o lock e espera (wait).
					while (queue.isEmpty() && (!shutdown)) {
						try { 
							queue.wait(); 
						}
						catch (InterruptedException ignored){} // Ignora a interrupcao
					}
            
					// Condicao de termino: se a fila esta vazia E o pool foi desligado (shutdown=true),
					// a thread sai do metodo run() e termina.
					if (queue.isEmpty()) {
						return;
					}
            
					// Se ha tarefas, remove a primeira da fila.
					r = (Runnable) queue.removeFirst();
				} // Fim do bloco synchronized (lock da fila eh liberado).
          
				try { 
					// Executa a tarefa FORA do bloco synchronized.
					// Isso permite que outras threads acessem a fila enquanto esta executa.
					r.run(); 
				}
				catch (RuntimeException e) {
					// Captura excecoes da tarefa para que a thread trabalhadora nao morra.
				}
			} // Fim do while(true), volta para tentar pegar outra tarefa.
		} 
	} 
}
//-------------------------------------------------------------------------------

//--PASSO 1: cria uma classe que implementa a interface Runnable 
class Hello implements Runnable {
	String msg;
	public Hello(String m) { msg = m; }

	//--metodo executado pela thread
	public void run() {
		System.out.println(msg);
	}
}

//-------------------------------------------------------------------------------
// NOVA CLASSE PRIMO (IMPLEMENTADA)
//-------------------------------------------------------------------------------
class Primo implements Runnable {
	// Usando 'long' para ser compativel com a logica C 'long unsigned int'
	private long numero;

	public Primo(long n) {
		this.numero = n;
	}

	/**
		* Funcao auxiliar para determinar se um numero eh primo.
		* Esta eh a traducao direta da logica C fornecida.
		*/
	private boolean ehPrimo(long n) {
		if (n <= 1) return false; // 0 (C) -> false (Java)
		if (n == 2) return true;  // 1 (C) -> true (Java)
		if (n % 2 == 0) return false;

		// Loop traduzido de 'for(i=3; i< sqrt(n)+1; i+=2)'
		for (long i = 3; i < Math.sqrt(n) + 1; i += 2) {
				if (n % i == 0) {
						return false;
				}
		}
		return true; // 1 (C) -> true (Java)
	}

	//--metodo executado pela thread
	@Override
	public void run() {
    // Pega o nome da thread atual
    String threadName = Thread.currentThread().getName(); 
    
    if (ehPrimo(this.numero)) {
        System.out.println(
            "[" + threadName + "] O numero " + this.numero + " EH primo."
        );
    } else {
        System.out.println(
            "[" + threadName + "] O numero " + this.numero + " NAO EH primo."
        );
    }
}
}
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// CLASSE DA APLICACAO (MAIN) - MODIFICADA
//-------------------------------------------------------------------------------
class MyPool {
	private static final int NTHREADS = 8;

	public static void main (String[] args) {
		//--PASSO 2: cria o pool de threads
		FilaTarefas pool = new FilaTarefas(NTHREADS); 
     
		//--PASSO 3: dispara a execução dos objetos runnable usando o pool de threads
		for (int i = 1; i <= 25; i++) {
			// Linhas anteriores (92-93) COMENTADAS:
			// final String m = "Hello da tarefa " + i;
			// Runnable hello = new Hello(m);
			// pool.execute(hello);

			// Linhas 94 e 95 DESCOMENTADAS e implementadas:
			// (Note que 'i' eh int, mas o construtor de Primo aceita long)
			Runnable primo = new Primo(i);
			pool.execute(primo);
		}

		//--PASSO 4: esperar pelo termino das threads
		pool.shutdown();
		System.out.println("Terminou");
	}
}