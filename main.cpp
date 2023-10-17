#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <numeric>

bool areAllZeroes(const std::vector<int>& v) {
    return std::all_of(v.begin(), v.end(), [](int i) {
        return i == 0;
    });
}

struct Data {
    int n, k, Q, L, r;
    std::vector<int> d, p;
    std::vector<std::vector<int>> c;
};

struct Solution {
    int custoTotal;
    int custoRoteamento;
    int custoUtilizacaoVeiculos;
    int custoTerceirizacao;

    std::vector<int> terceirizados;

    int numeroDeRotas;
    std::vector<std::vector<int>> rotaPorVeiculo;
};

Solution greedy_solution(Data& data) {
    Solution solution;
    solution.custoTotal = 0;

    int veiculoAtual = 0;
    std::vector<int> posicoesVeiculos(data.k, 0);
    std::vector<int> pesosPorVeiculos(data.k, 0);
    std::vector<int> posicoesPercorridas;

    for(int i = 0; i < data.k; ++i) {
        solution.rotaPorVeiculo.push_back(std::vector<int>());
    }

    while (true) {
        std::cout << "Veiculo: " << veiculoAtual << std::endl;

        int jMaisPerto = 0;
        int distanciaJMaisPerto = 99999;
        bool distanciaAtualizada = false;

        if (pesosPorVeiculos[veiculoAtual] < data.Q) {
            for(int j = 1; j <= data.n; j++) {

                auto it = std::find(posicoesPercorridas.begin(), posicoesPercorridas.end(), j);
                
                std::cout << "Positions traversed: ";
                for(int pos : posicoesPercorridas) {
                    std::cout << pos << " ";
                }

                std::cout << std::endl;
                std::cout << "procurando: " << j << std::endl;

                // j atual nao esta em posicoesPercorridas
                if (it == posicoesPercorridas.end()) {
                    std::cout << "nao achou: " << j << std::endl;
                    if (data.c[posicoesVeiculos[veiculoAtual]][j] < distanciaJMaisPerto && data.c[posicoesVeiculos[veiculoAtual]][j] != 0 && ((pesosPorVeiculos[veiculoAtual] + data.d[j - 1]) <= data.Q)) {
                        distanciaJMaisPerto = data.c[posicoesVeiculos[veiculoAtual]][j];
                        std::cout << "distancia: " << distanciaJMaisPerto << std::endl;
                        distanciaAtualizada = true;
                        std::cout << "j mais perto: " << j << std::endl;
                        jMaisPerto = j;
                    }
                }
            }
        }

        if (distanciaAtualizada && (distanciaJMaisPerto >= data.p[jMaisPerto - 1])) {
            solution.terceirizados.push_back(jMaisPerto);
            solution.custoTerceirizacao += data.p[jMaisPerto - 1];
            solution.custoTotal += data.p[jMaisPerto - 1];
            posicoesPercorridas.push_back(jMaisPerto);
            std::cout << "terceirizando o item: " << jMaisPerto << " pois data.p[jMaisPerto] = " << data.p[jMaisPerto - 1] << " e distanciaJMaisPerto = " << distanciaJMaisPerto << std::endl;
        } else {
            if (distanciaAtualizada == false) {
                distanciaJMaisPerto = data.c[posicoesVeiculos[veiculoAtual]][0];
            }

            if (!solution.rotaPorVeiculo[veiculoAtual].empty()) {
                if (solution.rotaPorVeiculo[veiculoAtual].back() != 0) {
                    std::cout << "Posicao mais perto: " << jMaisPerto << std::endl;

                    solution.rotaPorVeiculo[veiculoAtual].push_back(jMaisPerto);
                    
                    if (jMaisPerto > 0) {
                        pesosPorVeiculos[veiculoAtual] += data.d[jMaisPerto-1];
                    }

                    solution.custoRoteamento += distanciaJMaisPerto;

                    posicoesPercorridas.push_back(jMaisPerto);

                    std::cout << "adicionando a rota: " << jMaisPerto << std::endl;

                    std::cout << "nova rota: " << std::endl;
                    for(int x = 0; x < solution.rotaPorVeiculo[veiculoAtual].size(); x++) {
                        std::cout << solution.rotaPorVeiculo[veiculoAtual][x] << " ";
                    }
                    std::cout << std::endl;

                    posicoesVeiculos[veiculoAtual] = jMaisPerto;
                    std::cout << "posicoesVeiculos[veiculoAtual] (adicionando): " << posicoesVeiculos[veiculoAtual] << std::endl;

                    std::cout << "novo peso: " << pesosPorVeiculos[veiculoAtual] << std::endl;

                    std::cout << "adicionando " << distanciaJMaisPerto << std::endl;
                    solution.custoTotal += distanciaJMaisPerto;
                }
            } else {
                std::cout << "Posicao mais perto: " << jMaisPerto << std::endl;

                solution.rotaPorVeiculo[veiculoAtual].push_back(jMaisPerto);

                if (jMaisPerto > 0) {
                    pesosPorVeiculos[veiculoAtual] += data.d[jMaisPerto-1];
                }

                solution.custoRoteamento += distanciaJMaisPerto;
                solution.custoUtilizacaoVeiculos += data.r;
                solution.custoTotal += data.r;
                solution.numeroDeRotas += 1;

                posicoesPercorridas.push_back(jMaisPerto);

                std::cout << "adicionando a rota: " << jMaisPerto << std::endl;

                std::cout << "nova rota: " << std::endl;
                for(int x = 0; x < solution.rotaPorVeiculo[veiculoAtual].size(); x++) {
                    std::cout << solution.rotaPorVeiculo[veiculoAtual][x] << " ";
                }
                std::cout << std::endl;

                posicoesVeiculos[veiculoAtual] = jMaisPerto;
                std::cout << "posicoesVeiculos[veiculoAtual] (adicionando): " << posicoesVeiculos[veiculoAtual] << std::endl;

                std::cout << "novo peso: " << pesosPorVeiculos[veiculoAtual] << std::endl;

                std::cout << "adicionando " << distanciaJMaisPerto << std::endl;
                solution.custoTotal += distanciaJMaisPerto;
            }
        }

        // int temp = 0;
        // std::cin >> temp;

        std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;

        if(areAllZeroes(posicoesVeiculos)) {
            for(auto &rota : solution.rotaPorVeiculo) {
                if (!rota.empty() && rota.front() != 0) {
                    rota.insert(rota.begin(), 0);
                }
            }

            // terceirizando todas posicoes nao percorridas
            for (int j = 1; j <= data.n; j++) {
                auto it = std::find(posicoesPercorridas.begin(), posicoesPercorridas.end(), j);
                if (it == posicoesPercorridas.end()) {
                    solution.terceirizados.push_back(j);
                    solution.custoTerceirizacao += data.p[j - 1];
                    solution.custoTotal += data.p[j - 1];
                    std::cout << "terceirizando o item não visitado: " << j << " pois data.p[j] = " << data.p[j - 1] << std::endl;
                }
            }
            return solution;
        }

        if (veiculoAtual < data.k - 1) {
            veiculoAtual++;
        } else {
            veiculoAtual = 0;
        }
    }
}

Solution swap_neighborhood(const Data& data, const Solution& initial_solution) {
    Solution best_solution = initial_solution;

    // Função auxiliar para calcular o custo da rota
    auto calculate_route_cost = [&data](const std::vector<int>& route) {
        int cost = 0;
        std::cout << "Calculando custo para a rota: ";
        for (const int& point : route) {
            std::cout << point << " -> ";
        }
        std::cout << std::endl;

        for (size_t i = 0; i < route.size() - 1; ++i) {
            int segment_cost = data.c[route[i]][route[i+1]];
            std::cout << "Custo do segmento " << route[i] << " -> " << route[i+1] << ": " << segment_cost << std::endl;
            cost += segment_cost;
        }

        std::cout << "Custo total da rota: " << cost << std::endl;
        return cost;
    };


    for (size_t v = 0; v < best_solution.rotaPorVeiculo.size(); ++v) {
        std::vector<int>& current_route = best_solution.rotaPorVeiculo[v];
        int current_route_cost = calculate_route_cost(current_route);

        std::cout << "Verificando rota do veículo " << v << " com custo inicial: " << current_route_cost << std::endl;

        for (size_t i = 0; i < current_route.size() - 1; ++i) { 
            for (size_t j = i+1; j < current_route.size() - 1; ++j) {
                // Troca temporariamente as posições dos itens i e j
                std::swap(current_route[i], current_route[j]);

                int new_route_cost = calculate_route_cost(current_route);

                std::cout << "Trocando posições " << i << " e " << j << ". Novo custo: " << new_route_cost << std::endl;

                // Se a nova rota é melhor (tem um custo menor), atualiza a solução
                if (new_route_cost < current_route_cost) {
                    std::cout << "Nova rota é melhor! Atualizando custo." << std::endl;

                    current_route_cost = new_route_cost;
                    best_solution.custoRoteamento -= current_route_cost;  // Revisar lógica
                    best_solution.custoTotal -= current_route_cost;       // Revisar lógica

                } else {
                    // Desfaz a troca
                    std::swap(current_route[i], current_route[j]);
                }
            }
        }
    }

    std::cout << "Custo da solução final após aplicar a estrutura de vizinhança: " << best_solution.custoTotal << std::endl;
    return best_solution;
}

Data readFile(const std::string& filename) {
    std::ifstream file(filename); 
    if(!file) {
        std::cerr << "Could not open the file!" << std::endl;
        exit(EXIT_FAILURE); // Exiting because we can't return any meaningful data
    }

    Data data;

    file >> data.n >> data.k >> data.Q >> data.L >> data.r;

    data.d.resize(data.n);
    for(int i = 0; i < data.n; ++i) {
        file >> data.d[i];
    }

    data.p.resize(data.n);
    for(int i = 0; i < data.n; ++i) {
        file >> data.p[i];
    }

    data.c.resize(data.n + 1, std::vector<int>(data.n + 1));
    // std::cout << "printing n: " << data.n << std::endl;
    for(int i = 0; i < (data.n + 1); ++i) {
        // std::cout << "printing i: " << i << std::endl;
        for(int j = 0; j < (data.n + 1); ++j) {
            // std::cout << "printing j: " << j << std::endl;
            file >> data.c[i][j];
            // std::cout << "data.c[" << i << "][" << j << "]: " <<  data.c[i][j] << std::endl;
        }
    }

    return data;
}

void printSolution(const Solution& sol) {
    std::cout << "Solução: \n";
    std::cout << "Custo Total: " << sol.custoTotal << "\n";
    std::cout << "Custo de Roteamento: " << sol.custoRoteamento << "\n";
    std::cout << "Custo de Utilização de Veículos: " << sol.custoUtilizacaoVeiculos << "\n";
    std::cout << "Custo de Terceirização: " << sol.custoTerceirizacao << "\n";

    std::cout << "Número de Rotas: " << sol.numeroDeRotas << "\n";

    std::cout << "Rotas por Veículo: \n";
    for(size_t i = 0; i < sol.rotaPorVeiculo.size(); ++i) {
        std::cout << "Veículo " << i << ": ";
        for(int pos : sol.rotaPorVeiculo[i]) {
            std::cout << pos << " ";
        }
        std::cout << "\n";
    }

    std::cout << "Terceirizados: ";
    for(int t : sol.terceirizados) {
        std::cout << t << " ";
    }
    std::cout << "\n";
}

int main() {
    Data data = readFile("input.txt");

    // Output the variables and arrays/matrices to verify they were read correctly
    std::cout << "n: " << data.n << "\nk: " << data.k << "\nQ: " << data.Q 
              << "\nL: " << data.L << "\nr: " << data.r << std::endl;

    std::cout << "\nd: ";
    for(int i : data.d) {
        std::cout << i << " ";
    }

    std::cout << "\np: ";
    for(int i : data.p) {
        std::cout << i << " ";
    }

    std::cout << std::endl << "\nc: " << std::endl;
    for(int i = 0; i < (data.n + 1); i++) {
        for(int j = 0; j < (data.n + 1); j++) {
            std::cout << data.c[i][j] << " ";
        }
        std::cout << std::endl;
    }

    Solution solution = greedy_solution(data);

    printSolution(solution);

    std::cout << "\n\n\n\n" << std::endl;
    Solution improved_solution = swap_neighborhood(data, solution);
    std::cout << "\n\n\n\nSolução após vizinhança por troca:" << std::endl;
    printSolution(improved_solution);

    return 0;
}