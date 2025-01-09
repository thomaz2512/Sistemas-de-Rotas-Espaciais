
## Sistemas de Rotas Espaciais

Este projeto foi desenvolvido como parte da disciplina Teoria dos Grafos no curso de Ciência da Computação na Universidade Tuiuti do Paraná. Ele simula um sistema de gerenciamento de rotas espaciais, representado por um grafo, para resolver problemas relacionados à conectividade e navegação em uma rede de estações orbitais.


## Descrição do Projeto

No contexto de uma missão de resgate espacial, o programa ajuda a:

1. Encontrar o caminho mais curto entre duas estações utilizando o algoritmo de Dijkstra.
2. Verificar a conectividade da rede espacial.
3. Identificar rotas redundantes ou alternativas em caso de falhas.
4. Adicionar e remover estações e rotas.
5. Bloquear rotas que estão indisponíveis.

## Requisitos Atendidos

- Representação da rede de estações orbitais como um grafo usando listas de adjacências.
- Algoritmo para encontrar o caminho mais curto.
- Verificação de conectividade da rede.
- Cálculo de rotas alternativas.
- Medição do tempo de execução para funções críticas.
## Arquivos

- main.cpp: Código-fonte do programa.
- estacoeserotas.csv: Arquivo de entrada contendo os dados das estações e rotas.
## Funcionalidade Principais

1. Carregamento de Dados:

- Leitura de estações e rotas a partir de um arquivo .csv.

2. Gerenciamento do Grafo:

- Adicionar ou remover estações e rotas.
- Bloquear rotas específicas.

3. Busca pelo Caminho Mais Curto:

- Implementação do algoritmo de Dijkstra.

4. Verificação de Conectividade:

- Verifica se todas as estações estão conectadas.

5. Rotas Alternativas:

- Busca por caminhos redundantes ou alternativos entre duas estações.

6. Medição de Desempenho:

- Tempo de execução medido para funções principais.
## Como Executar

### Pré-requisitos

- Compilador C++ (como g++).
- Arquivo de entrada no formato .csv.

### Compilação
Para compilar o programa, use o comando:

    g++ main.cpp -o main.cpp

### Execução
Para executar, use:

    ./rotas_espaciais

### Entradas Necessárias
Certifique-se de que o arquivo estacoeserotas.csv esteja no mesmo diretório que o executável. O programa usará este arquivo para carregar as informações da rede.
## Exemplos de uso

1. Caminho Mais Curto:
- Encontra o caminho mais curto entre a Terra e Centauri.
2. Remoção de Rotas Bloqueadas:
- Remove rotas específicas como "Elysium - Idris".
4. Análise de Conectividade:
- Verifica se a rede espacial é conectada após falhas.
## Observações

- O tempo de execução é calculado para funções principais usando a biblioteca <chrono>.
- As rotas e estações podem ser alteradas diretamente no arquivo de entrada .csv.