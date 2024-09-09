#include <bits/stdc++.h>
#include <filesystem>
using namespace std;
#define int long long
namespace fs = std::filesystem;

void save_matrix_to_bin(const vector<vector<int>>& matrix, const string& filename) {
    ofstream file(filename, ios::binary);
    if (file.is_open()) {
        int rows = matrix.size();
        int cols = matrix[0].size();
        file.write(reinterpret_cast<const char*>(&rows), sizeof(int));
        file.write(reinterpret_cast<const char*>(&cols), sizeof(int));
        for (int i = 0; i < rows; ++i) {
            file.write(reinterpret_cast<const char*>(matrix[i].data()), cols * sizeof(int));
        }
        file.close();
    } else {
        cerr << "Error al abrir el archivo " << filename << endl;
    }
}

vector<vector<int>> generate_random_matrix(int rows, int cols) {
    vector<vector<int>> matrix(rows, vector<int>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = rand() % 100; // Genera números aleatorios entre 0 y 99
        }
    }
    return matrix;
}

signed main() {
    srand(time(0)); // Inicializa la semilla para generar números aleatorios

    // Directorios para guardar las matrices
    fs::create_directories("datasets/square_matrices1");
    fs::create_directories("datasets/rectangular_matrices");
    fs::create_directories("datasets/square_matrices2");
    

    // Generar matrices cuadradas y guardarlas
    int k = 1;
    while(k <=2){
        for (int size = 2; size <= 512; size *= 2) { // Tamaños de 2x2 hasta 512x512
            auto matrix = generate_random_matrix(size, size);
            string filename = "datasets/square_matrices"+to_string(k)+"/matrix_" + to_string(size) + "x" + to_string(size) + ".bin";
            save_matrix_to_bin(matrix, filename);
        }
        k += 1;
    }

    // Generar matrices rectangulares y guardarlas
    for (int rows = 64; rows <= 512; rows *= 2) { // Variación de filas
        for (int cols = 32; cols <= 256; cols *= 2) { // Variación de columnas
            if(cols != rows){
                auto matrix = generate_random_matrix(rows, cols);
                string filename = "datasets/rectangular_matrices/matrix_" + to_string(rows) + "x" + to_string(cols) + ".bin";
                save_matrix_to_bin(matrix, filename);
            }
        }
    }

    cout << "Matrices generadas y guardadas en archivos binarios." << endl;
    return 0;
}
