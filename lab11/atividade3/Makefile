# --- Variáveis de Compilação ---
JCC = javac
JVM = java
MAIN = FutureHello
SOURCES = $(MAIN).java
CLASSES = $(MAIN).class VerificaPrimoCallable.class

# --- Variáveis de Teste ---
# Listas para o 'make test' (bateria de testes)
THREADS_LIST = 1 2 4 8 16
N_LIST = 100000 1000000 10000000
OUTPUT_FILE = results.txt

# Valores padrão para 'make run' (teste unico)
THREADS = 10
N = 100000


# --- Regras ---

# Regra padrao (compila)
all: $(CLASSES)

# Regra de compilacao
$(CLASSES): $(SOURCES)
	$(JCC) $(SOURCES)

#-------------------------------------------------------------------------------
# REGRA PRINCIPAL DE TESTES
# 'make test'
#-------------------------------------------------------------------------------
test: all
	# Limpa o arquivo de resultados anterior e adiciona um cabecalho
	@echo "Iniciando bateria de testes..."
	@echo "Resultados serao salvos em: $(OUTPUT_FILE)"
	@echo "--- Início da Bateria de Testes ---" > $(OUTPUT_FILE)
	
	# Loop aninhado (usa sintaxe de shell) - TUDO EM UMA LINHA para evitar erros de quebra de linha
	@for n in $(N_LIST); do for t in $(THREADS_LIST); do echo "Executando: N=$$n, Threads=$$t ..."; echo "--- Teste com N=$$n e Threads=$$t ---" >> $(OUTPUT_FILE); $(JVM) $(MAIN) $$t $$n >> $(OUTPUT_FILE) 2>&1; echo "" >> $(OUTPUT_FILE); done; done
	
	@echo "--- Fim da Bateria de Testes ---" >> $(OUTPUT_FILE)
	@echo "Testes concluidos!"

#-------------------------------------------------------------------------------
# Regra para executar um teste unico (para depuracao)
# 'make run THREADS=... N=...'
#-------------------------------------------------------------------------------
run: all
	$(JVM) $(MAIN) $(THREADS) $(N)

#-------------------------------------------------------------------------------
# Regra para limpar os arquivos .class E o arquivo de resultados
# 'make clean'
#-------------------------------------------------------------------------------
clean:
	rm -f *.class $(OUTPUT_FILE)

# Informa ao 'make' que estas sao regras e nao arquivos
.PHONY: all test run clean