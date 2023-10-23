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
        // std::cout << "Veiculo: " << veiculoAtual << std::endl;

        int jMaisPerto = 0;
        int distanciaJMaisPerto = 99999;
        bool distanciaAtualizada = false;

        if (pesosPorVeiculos[veiculoAtual] < data.Q) {
            for(int j = 1; j <= data.n; j++) {

                auto it = std::find(posicoesPercorridas.begin(), posicoesPercorridas.end(), j);
                
                // std::cout << "Positions traversed: ";
                for(int pos : posicoesPercorridas) {
                    std::cout << pos << " ";
                }

                std::cout << std::endl;
                // std::cout << "procurando: " << j << std::endl;

                // j atual nao esta em posicoesPercorridas
                if (it == posicoesPercorridas.end()) {
                    // std::cout << "nao achou: " << j << std::endl;
                    if (data.c[posicoesVeiculos[veiculoAtual]][j] < distanciaJMaisPerto && data.c[posicoesVeiculos[veiculoAtual]][j] != 0 && ((pesosPorVeiculos[veiculoAtual] + data.d[j - 1]) <= data.Q)) {
                        distanciaJMaisPerto = data.c[posicoesVeiculos[veiculoAtual]][j];
                        // std::cout << "distancia: " << distanciaJMaisPerto << std::endl;
                        distanciaAtualizada = true;
                        // std::cout << "j mais perto: " << j << std::endl;
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
            // std::cout << "terceirizando o item: " << jMaisPerto << " pois data.p[jMaisPerto] = " << data.p[jMaisPerto - 1] << " e distanciaJMaisPerto = " << distanciaJMaisPerto << std::endl;
        } else {
            if (distanciaAtualizada == false) {
                distanciaJMaisPerto = data.c[posicoesVeiculos[veiculoAtual]][0];
            }

            if (!solution.rotaPorVeiculo[veiculoAtual].empty()) {
                if (solution.rotaPorVeiculo[veiculoAtual].back() != 0) {
                    // std::cout << "Posicao mais perto: " << jMaisPerto << std::endl;

                    solution.rotaPorVeiculo[veiculoAtual].push_back(jMaisPerto);
                    
                    if (jMaisPerto > 0) {
                        pesosPorVeiculos[veiculoAtual] += data.d[jMaisPerto-1];
                    }

                    solution.custoRoteamento += distanciaJMaisPerto;

                    posicoesPercorridas.push_back(jMaisPerto);

                    // std::cout << "adicionando a rota: " << jMaisPerto << std::endl;

                    // std::cout << "nova rota: " << std::endl;
                    for(int x = 0; x < solution.rotaPorVeiculo[veiculoAtual].size(); x++) {
                        std::cout << solution.rotaPorVeiculo[veiculoAtual][x] << " ";
                    }
                    std::cout << std::endl;

                    posicoesVeiculos[veiculoAtual] = jMaisPerto;
                    // std::cout << "posicoesVeiculos[veiculoAtual] (adicionando): " << posicoesVeiculos[veiculoAtual] << std::endl;

                    // std::cout << "novo peso: " << pesosPorVeiculos[veiculoAtual] << std::endl;

                    // std::cout << "adicionando " << distanciaJMaisPerto << std::endl;
                    solution.custoTotal += distanciaJMaisPerto;
                }
            } else {
                // std::cout << "Posicao mais perto: " << jMaisPerto << std::endl;

                solution.rotaPorVeiculo[veiculoAtual].push_back(jMaisPerto);

                if (jMaisPerto > 0) {
                    pesosPorVeiculos[veiculoAtual] += data.d[jMaisPerto-1];
                }

                solution.custoRoteamento += distanciaJMaisPerto;
                solution.custoUtilizacaoVeiculos += data.r;
                solution.custoTotal += data.r;
                solution.numeroDeRotas += 1;

                posicoesPercorridas.push_back(jMaisPerto);

                // std::cout << "adicionando a rota: " << jMaisPerto << std::endl;

                // std::cout << "nova rota: " << std::endl;
                for(int x = 0; x < solution.rotaPorVeiculo[veiculoAtual].size(); x++) {
                    std::cout << solution.rotaPorVeiculo[veiculoAtual][x] << " ";
                }
                std::cout << std::endl;

                posicoesVeiculos[veiculoAtual] = jMaisPerto;
                // std::cout << "posicoesVeiculos[veiculoAtual] (adicionando): " << posicoesVeiculos[veiculoAtual] << std::endl;

                // std::cout << "novo peso: " << pesosPorVeiculos[veiculoAtual] << std::endl;

                // std::cout << "adicionando " << distanciaJMaisPerto << std::endl;
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
                    // std::cout << "terceirizando o item não visitado: " << j << " pois data.p[j] = " << data.p[j - 1] << std::endl;
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

    bool improvement_found = true; // flag para continuar buscando melhorias

    // Função auxiliar para calcular o custo da rota
    auto calculate_route_cost = [&data](const std::vector<int>& route) {
        int cost = 0;

        for (size_t i = 0; i < route.size() - 1; ++i) {
            cost += data.c[route[i]][route[i+1]];
        }

        return cost;
    };

    while (improvement_found) {
        improvement_found = false; // reseta a flag para cada iteração

        for (size_t v = 0; v < best_solution.rotaPorVeiculo.size(); ++v) {
            std::vector<int>& current_route = best_solution.rotaPorVeiculo[v];
            int current_route_cost = calculate_route_cost(current_route);

            // std::cout << "Verificando rota do veículo " << v << " com custo inicial: " << current_route_cost << std::endl;

            for (size_t i = 0; i < current_route.size() - 1; ++i) { 
                for (size_t j = i+1; j < current_route.size() - 1; ++j) {
                    // Troca temporariamente as posições dos itens i e j
                    std::swap(current_route[i], current_route[j]);

                    int new_route_cost = calculate_route_cost(current_route);

                    // std::cout << "Trocando posições " << i << " e " << j << ". Novo custo: " << new_route_cost << std::endl;

                    // Se a nova rota é melhor (tem um custo menor), atualiza a solução
                    if (new_route_cost < current_route_cost) {
                        // std::cout << "Nova rota é melhor! Atualizando custo." << std::endl;
                        current_route_cost = new_route_cost;
                        improvement_found = true; // indica que foi encontrada uma melhoria
                    } else {
                        // Desfaz a troca
                        std::swap(current_route[i], current_route[j]);
                    }
                }
            }
        }
    }

    best_solution.custoTotal = best_solution.custoRoteamento + best_solution.custoTerceirizacao + best_solution.custoUtilizacaoVeiculos;

    // std::cout << "Custo da solução final após aplicar a estrutura de vizinhança: " << best_solution.custoTotal << std::endl;

    return best_solution;
}

Solution multi_route_swap_neighborhood(const Data& data, const Solution& initial_solution) {
    Solution best_solution = initial_solution;

    bool improvement_found = true; // flag para continuar buscando melhorias

    // Função auxiliar para calcular o custo da rota
    auto calculate_route_cost = [&data](const std::vector<int>& route) {
        int cost = 0;

        for (size_t i = 0; i < route.size() - 1; ++i) {
            cost += data.c[route[i]][route[i+1]];
        }

        return cost;
    };

    while (improvement_found) {
        improvement_found = false; // reseta a flag para cada iteração

        for (size_t v1 = 0; v1 < best_solution.rotaPorVeiculo.size(); ++v1) {
            for (size_t v2 = 0; v2 < best_solution.rotaPorVeiculo.size(); ++v2) {
                if (v1 != v2) { // Evita a comparação da mesma rota
                    for (size_t i = 0; i < best_solution.rotaPorVeiculo[v1].size(); ++i) {
                        for (size_t j = 0; j < best_solution.rotaPorVeiculo[v2].size(); ++j) {
                            std::vector<int> current_route1 = best_solution.rotaPorVeiculo[v1];
                            std::vector<int> current_route2 = best_solution.rotaPorVeiculo[v2];

                            int prev_route1_cost = calculate_route_cost(current_route1);
                            int prev_route2_cost = calculate_route_cost(current_route2);

                            // std::cout << "Verificando troca entre veículo " << v1 << " na posição " << i << " e veículo " << v2 << " na posição " << j << std::endl;
                            // std::cout << "Custos anteriores: Rota1: " << prev_route1_cost << ", Rota2: " << prev_route2_cost << std::endl;

                            // Troca temporariamente os itens entre as rotas
                            if (current_route1[i] != 0 && current_route2[j] != 0) {
                                std::swap(current_route1[i], current_route2[j]);

                                int new_route1_cost = calculate_route_cost(current_route1);
                                int new_route2_cost = calculate_route_cost(current_route2);

                                // std::cout << "Novos custos após a troca: Rota1: " << new_route1_cost << ", Rota2: " << new_route2_cost << std::endl;

                                // Se a troca resulta em um custo menor, atualiza a solução
                                if (new_route1_cost + new_route2_cost < prev_route1_cost + prev_route2_cost) {
                                    // std::cout << "A troca melhorou o custo! Atualizando a solução." << std::endl;
                                    best_solution.rotaPorVeiculo[v1] = current_route1;
                                    best_solution.rotaPorVeiculo[v2] = current_route2;
                                    best_solution.custoRoteamento += new_route1_cost + new_route2_cost - prev_route1_cost - prev_route2_cost;
                                    best_solution.custoTotal += new_route1_cost + new_route2_cost - prev_route1_cost - prev_route2_cost;
                                    improvement_found = true; // indica que foi encontrada uma melhoria
                                } else {
                                    // std::cout << "A troca não melhorou o custo. Desfazendo." << std::endl;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // std::cout << "Custo da solução final após aplicar multi-route swap: " << best_solution.custoTotal << std::endl;

    return best_solution;
}

Solution outsourced_neighborhood(const Data& data, const Solution& initial_solution) {
    Solution best_solution = initial_solution;

    // Lista de entregas incorporadas à rota
    std::vector<int> incorporated_deliveries;

    // Função auxiliar para calcular o custo da rota
    auto calculate_route_cost = [&data](const std::vector<int>& route) {
        int cost = 0;

        for (size_t i = 0; i < route.size() - 1; ++i) {
            cost += data.c[route[i]][route[i+1]];
        }
        
        return cost;
    };

    // Tentar terceirizar as entregas
    for (size_t v = 0; v < best_solution.rotaPorVeiculo.size(); ++v) {
        for (size_t i = 0; i < best_solution.rotaPorVeiculo[v].size(); ++i) {
            int current_location = best_solution.rotaPorVeiculo[v][i];

            // Se o nó é '0', continue para o próximo nó
            if (current_location == 0) {
                continue;
            }

            int route_cost_with_current = calculate_route_cost(best_solution.rotaPorVeiculo[v]);
            
            best_solution.rotaPorVeiculo[v].erase(best_solution.rotaPorVeiculo[v].begin() + i);
            int route_cost_without_current = calculate_route_cost(best_solution.rotaPorVeiculo[v]);
            
            // Se a terceirização é mais barata
            if (route_cost_without_current + data.p[current_location - 1] < route_cost_with_current) {
                best_solution.terceirizados.push_back(current_location);
                best_solution.custoTerceirizacao += data.p[current_location - 1];
                best_solution.custoRoteamento -= route_cost_with_current - route_cost_without_current;
            } else {
                // Desfaz a remoção do local
                best_solution.rotaPorVeiculo[v].insert(best_solution.rotaPorVeiculo[v].begin() + i, current_location);
            }
        }
    }

    // Verificar se as entregas terceirizadas podem ser incorporadas de volta na rota
    for (int terc : best_solution.terceirizados) {
        int current_cost_with_terceirized = data.p[terc - 1];
        
        int best_route_cost = 999999;
        size_t best_v = 0;

        for (size_t v = 0; v < best_solution.rotaPorVeiculo.size(); ++v) {
            std::vector<int> tmp_route = best_solution.rotaPorVeiculo[v];
            tmp_route.push_back(terc);
            
            int route_cost = calculate_route_cost(tmp_route);
            if (route_cost < best_route_cost) {
                best_route_cost = route_cost;
                best_v = v;
            }
        }

        // Se incorporar a entrega na rota é mais barato
        if (best_route_cost < current_cost_with_terceirized) {
            best_solution.rotaPorVeiculo[best_v].push_back(terc);
            best_solution.custoTerceirizacao -= data.p[terc - 1];
            best_solution.custoRoteamento += best_route_cost;

            // Adiciona à lista de entregas incorporadas
            incorporated_deliveries.push_back(terc);
        }
    }

    // Remove as entregas incorporadas da lista de terceirizados
    for (int delivery : incorporated_deliveries) {
        best_solution.terceirizados.erase(std::remove(best_solution.terceirizados.begin(), best_solution.terceirizados.end(), delivery), best_solution.terceirizados.end());
    }

    best_solution.custoTotal = best_solution.custoRoteamento + best_solution.custoTerceirizacao + best_solution.custoUtilizacaoVeiculos;

    return best_solution;
}

Solution vnd(const Data& data, Solution initial_solution) {
    Solution best_solution = initial_solution;
    Solution current_solution = initial_solution;
    
    int k = 0;
    while (k < 3) {
        std::cout << "Current neighborhood index: " << k << std::endl;
        switch(k) {
            case 0:
                std::cout << "Applying swap_neighborhood..." << std::endl;
                current_solution = swap_neighborhood(data, current_solution);
                std::cout << "New total cost after swap_neighborhood: " << current_solution.custoTotal << std::endl;
                break;
            case 1:
                std::cout << "Applying multi_route_swap_neighborhood..." << std::endl;
                current_solution = multi_route_swap_neighborhood(data, current_solution);
                std::cout << "New total cost after multi_route_swap_neighborhood: " << current_solution.custoTotal << std::endl;
                break;
            case 2:
                std::cout << "Applying outsourced_neighborhood..." << std::endl;
                current_solution = outsourced_neighborhood(data, current_solution);
                std::cout << "New total cost after outsourced_neighborhood: " << current_solution.custoTotal << std::endl;
                break;
        }

        if (current_solution.custoTotal < best_solution.custoTotal) {
            std::cout << "Found a better solution! Updating best_solution and resetting neighborhood index." << std::endl;
            best_solution = current_solution;
            k = 0; // reinicia o índice da vizinhança se houver melhora
        } else {
            std::cout << "No improvement found. Moving to the next neighborhood." << std::endl;
            ++k; // vai para a próxima vizinhança
        }
    }

    std::cout << "VND finished. Best solution cost: " << best_solution.custoTotal << std::endl;
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
    Data data = readFile("input2.txt");

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

    // Integrate the multi_route_swap_neighborhood
    std::cout << "\n\n\n\n" << std::endl;
    Solution multi_route_improved_solution = multi_route_swap_neighborhood(data, solution);
    std::cout << "\n\n\n\nSolução após vizinhança multi-route swap:" << std::endl;
    printSolution(multi_route_improved_solution);

    // Integrate the outsourced_neighborhood
    std::cout << "\n\n\n\n" << std::endl;
    Solution outsourced_improved_solution = outsourced_neighborhood(data, solution);
    std::cout << "\n\n\n\nSolução após vizinhança por terceirização:" << std::endl;
    printSolution(outsourced_improved_solution);

    Solution vnd_solution = vnd(data, solution);
    std::cout << "\n\n\n\nSolução após VND:" << std::endl;
    printSolution(vnd_solution);

    return 0;
}