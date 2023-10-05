#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

struct Data {
    int n, k, Q, L, r;
    std::vector<int> d, p;
    std::vector<std::vector<int>> c;
};

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

    data.c.resize(data.n, std::vector<int>(data.n));
    for(int i = 0; i < data.n; ++i) {
        for(int j = 0; j < data.n; ++j) {
            file >> data.c[i][j];
        }
    }

    return data;
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
    for(const auto &row : data.c) {
        for(int elem : row) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
