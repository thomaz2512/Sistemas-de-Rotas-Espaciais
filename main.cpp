#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <chrono>
#include <algorithm>
#include <climits>
#include <ctime>

using namespace std;

struct Conexao {
    string destino;
    int duracao;
};

// Estrutura de dados para representar o grafo das estações
unordered_map<string, vector<Conexao>> rede_estacoes;

// Função para carregar dados das estações a partir de um arquivo .csv (Requisito 1)
void carregar_dados_estacoes(const string& caminho_arquivo) {
    ifstream arquivo(caminho_arquivo);
    string linha;

    cout << "||    SISTEMAS DE ROTAS ESPACIAIS    ||\n";
    cout << "--CARREGANDO DADOS DAS ESTACOES...\n";

    auto inicio = chrono::high_resolution_clock::now(); // Tempo de início (Requisito 6)

    if (arquivo.is_open()) {
        while (getline(arquivo, linha)) {
            stringstream ss(linha);
            string origem, destino, duracao_str;

            getline(ss, origem, ',');

            while (ss.good()) {
                getline(ss, destino, ',');
                if (destino.empty()) continue;

                getline(ss, duracao_str, ',');
                if (duracao_str.empty()) continue;

                int duracao = stoi(duracao_str);
                rede_estacoes[origem].emplace_back(Conexao{destino, duracao});
            }
        }
        arquivo.close();
    }

    auto fim = chrono::high_resolution_clock::now();
    chrono::duration<double> tempo_carregamento = fim - inicio;
    cout << "--TEMPO DE CARREGAMENTO: " << tempo_carregamento.count() << " s\n"; // Tempo de execução (Requisito 6)
}

// Função para adicionar uma estação (Requisito 2)
void adicionar_estacao(const string& estacao) {
    if (rede_estacoes.find(estacao) == rede_estacoes.end()) {
        rede_estacoes[estacao] = vector<Conexao>();
        cout << "Estação " << estacao << " adicionada.\n";
    } else {
        cout << "Estação " << estacao << " já existe.\n";
    }
}

// Função para remover uma estação (Requisito 2)
void remover_estacao(const string& estacao) {
    rede_estacoes.erase(estacao);
    for (auto& par : rede_estacoes) {
        auto& conexoes = par.second;
        conexoes.erase(remove_if(conexoes.begin(), conexoes.end(), [&estacao](const Conexao& c) {
            return c.destino == estacao;
        }), conexoes.end());
    }
    cout << "Estação " << estacao << " removida.\n";
}

// Função para adicionar uma rota (Requisito 2)
void adicionar_rota(const string& origem, const string& destino, int duracao) {
    rede_estacoes[origem].emplace_back(Conexao{destino, duracao});
    cout << "Rota de " << origem << " para " << destino << " com duração " << duracao << " adicionada.\n";
}

// Função para remover uma rota (Requisito 2)
void remover_rota(const string& origem, const string& destino) {
    auto& conexoes_origem = rede_estacoes[origem];
    conexoes_origem.erase(remove_if(conexoes_origem.begin(), conexoes_origem.end(),
        [&destino](const Conexao& c) { return c.destino == destino; }), conexoes_origem.end());
    cout << "Rota de " << origem << " para " << destino << " removida.\n";
}

// Função para remover rotas bloqueadas (Requisito 2)
void bloquear_rotas() {
    cout << "\nx----REMOVENDO ROTAS BLOQUEADAS----x\n";
    vector<pair<string, string>> rotas_para_remover = {
        {"Elysium", "Idris"},
        {"Idris", "Rhetor"},
        {"Rhetor", "Croshaw"},
        {"Croshaw", "Nul"}
    };

    for (const auto& rota : rotas_para_remover) {
        string origem = rota.first;
        string destino = rota.second;

        auto& conexoes_origem = rede_estacoes[origem];
        conexoes_origem.erase(remove_if(conexoes_origem.begin(), conexoes_origem.end(),
            [&destino](const Conexao& c) { return c.destino == destino; }), conexoes_origem.end());

        auto& conexoes_destino = rede_estacoes[destino];
        conexoes_destino.erase(remove_if(conexoes_destino.begin(), conexoes_destino.end(),
            [&origem](const Conexao& c) { return c.destino == origem; }), conexoes_destino.end());

        cout << "| --ROTA REMOVIDA: " << origem << " <-> " << destino << "|\n";
    }
    cout << "x----------------------------------x\n";
}

// Implementação do Algoritmo de Dijkstra (Caminho Mínimo) - Requisito 3
void dijkstra(const string& origem, const string& destino) {
    unordered_map<string, int> dist;
    unordered_map<string, string> anterior;
    set<pair<int, string>> fila_prioridade;

    for (const auto& par : rede_estacoes) {
        dist[par.first] = INT_MAX;
    }
    dist[origem] = 0;
    fila_prioridade.insert({0, origem});

    while (!fila_prioridade.empty()) {
        auto [dist_atual, estacao_atual] = *fila_prioridade.begin();
        fila_prioridade.erase(fila_prioridade.begin());

        if (estacao_atual == destino) {
            break;
        }

        for (const auto& conexao : rede_estacoes[estacao_atual]) {
            int nova_dist = dist_atual + conexao.duracao;
            if (nova_dist < dist[conexao.destino]) {
                fila_prioridade.erase({dist[conexao.destino], conexao.destino});
                dist[conexao.destino] = nova_dist;
                anterior[conexao.destino] = estacao_atual;
                fila_prioridade.insert({nova_dist, conexao.destino});
            }
        }
    }

    // Caminho de volta
    vector<string> caminho;
    for (string at = destino; at != origem; at = anterior[at]) {
        caminho.push_back(at);
    }
    caminho.push_back(origem);
    reverse(caminho.begin(), caminho.end());



    cout << "--CAMINHO MAIS CURTO DE" << origem << " ATE " << destino << ":\n";
    for (const auto& estacao : caminho) {
        cout << estacao << " -> ";
    }
    cout << "\n--CUSTO TOTAL: " << dist[destino] << "\n";
}

// Verificação da conectividade da rede usando DFS (Requisito 4)
void dfs(const string& estacao, set<string>& visitados) {
    visitados.insert(estacao);
    for (const auto& conexao : rede_estacoes[estacao]) {
        if (visitados.find(conexao.destino) == visitados.end()) {
            dfs(conexao.destino, visitados);
        }
    }
}

bool verificar_conectividade() {
    set<string> visitados;
    if (!rede_estacoes.empty()) {
        dfs(rede_estacoes.begin()->first, visitados);
    }
    return visitados.size() == rede_estacoes.size();
}

// Função para buscar rotas alternativas ou redundantes (Requisito 5)
void buscar_rotas(const string& atual, const string& alvo,
                 vector<string>& caminho, vector<int>& custos,
                 vector<vector<string>>& caminhos_possiveis,
                 vector<vector<int>>& custos_possiveis,
                 set<string>& visitados,
                 int limite_profundidade = 10) {

    if (caminho.size() > limite_profundidade) {
        return;
    }

    caminho.push_back(atual);

    if (atual == alvo) {
        caminhos_possiveis.push_back(caminho);
        custos_possiveis.push_back(custos);
    } else {
        visitados.insert(atual);

        for (const auto& conexao : rede_estacoes[atual]) {
            if (visitados.find(conexao.destino) == visitados.end()) {
                custos.push_back(conexao.duracao);
                buscar_rotas(conexao.destino, alvo, caminho, custos,
                            caminhos_possiveis, custos_possiveis, visitados, limite_profundidade);
                custos.pop_back();
            }
        }

        visitados.erase(atual);
    }

    caminho.pop_back();
}

int main() {
    string arquivo_csv = "estacoeserotas.csv";

    // Carregar dados das estações
    cout << "--CARREGANDO DADOS DAS ESTACOES...\n";
    auto inicio = chrono::high_resolution_clock::now();
    carregar_dados_estacoes(arquivo_csv); // Requisito 1: Criar grafo
    auto duracao = chrono::high_resolution_clock::now() - inicio;
    cout << "--TEMPO DE EXECUCAO: " << duracao.count() << "'s\n\n";

    // Remover rotas bloqueadas
    cout << "--REMOVENDO ROTAS BLOQUEADAS...\n";
    inicio = chrono::high_resolution_clock::now();
    bloquear_rotas(); // Requisito 2: Bloquear rotas
    duracao = chrono::high_resolution_clock::now() - inicio;
    cout << "--TEMPO DE BLOQUEIO: " << duracao.count() << "'s\n\n";

    // Verificar conectividade da rede
    cout << "--VERIFICANDO CONECTIVIDADE COM A REDE...\n";
    inicio = chrono::high_resolution_clock::now();
    bool conectada = verificar_conectividade(); // Requisito 4: Verificar rede
    duracao = chrono::high_resolution_clock::now() - inicio;
    cout << "--TEMPO DE EXECUCAO: " << duracao.count() << " s\n";
    cout << (conectada ? "--REDE CONECTADA!\n\n" : "--REDE NAO CONECTADA!\n\n");

    // Encontrar o caminho mais curto entre Terra e Centauri
    cout << "--CALCULANDO O CAMINHO MAIS CURTO(Dijkstra)...\n\n";
    inicio = chrono::high_resolution_clock::now();
    dijkstra("Terra", "Centauri"); // Requisito 3: Caminho mínimo
    duracao = chrono::high_resolution_clock::now() - inicio;
    cout << "--TEMPO DE CALCULO: " << duracao.count() << " s\n\n";

    // Buscar rotas alternativas/redundantes
    cout << "--BUSCANDO ROTAS ALTERNATIVAS...\n";
    vector<string> caminho;
    vector<int> custos;
    vector<vector<string>> caminhos_possiveis;
    vector<vector<int>> custos_possiveis;
    set<string> visitados;

    inicio = chrono::high_resolution_clock::now();
    buscar_rotas("Terra", "Centauri", caminho, custos, caminhos_possiveis, custos_possiveis, visitados);
    duracao = chrono::high_resolution_clock::now() - inicio;
    cout << "--TEMPO DE BUSCA: " << duracao.count() << "'s\n\n";

    // Exibir rotas alternativas/redundantes
    cout << "--ROTAS ALTERNATIVAS DE TERRA PARA CENTAURI:\n";
    for (const auto& rotas : caminhos_possiveis) {
        for (const auto& estacao : rotas) {
            cout << estacao << " -> ";
        }
        cout << "\n";
    }

    return 0;
}

