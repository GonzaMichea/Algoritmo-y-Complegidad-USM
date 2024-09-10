#include <bits/stdc++.h>
#include <filesystem>
#include <chrono>
using namespace std;
namespace fs = std::filesystem;
#define int long long

// Leer una matriz desde un archivo binario
vector<vector<int>> readMatrixFromBinaryFile(const string& filepath) {
    ifstream file(filepath, ios::binary);
    if (!file.is_open()) {
        cerr << "No se pudo abrir el archivo para lectura: " << filepath << endl;
        return {};
    }

    int rows, cols;
    file.read(reinterpret_cast<char*>(&rows), sizeof(rows));
    file.read(reinterpret_cast<char*>(&cols), sizeof(cols));

    vector<vector<int>> matrix(rows, vector<int>(cols));
    for (int i = 0; i < rows; ++i) {
        file.read(reinterpret_cast<char*>(matrix[i].data()), cols * sizeof(int));
    }

    file.close();
    return matrix;
}

// Multiplicar dos matrices
vector<vector<int>> multiplyMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int n = A.size();      // Filas de A
    int p = B.size();  
    vector<vector<int>> result(n, vector<int>(p, 0));  // Resultado de dimensiones n x p
    
    // Multiplicación fila por fila
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < p; ++j) {
            for (size_t k = 0; k < A[0].size(); ++k) {  // Cambiado a size_t
                result[i][j] += A[i][k] * B[j][k];  // Fila por fila
            }
        }
    }
    
    return result;
}

// Guardar los tiempos de multiplicación en un archivo CSV
void saveTimesToCSV(const vector<tuple<int, int, int, int, int>>& times, const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        file << "Matrix A Rows,Matrix A Cols,Matrix B Rows,Matrix B Cols,Time (micro)\n";
        for (const auto& [rowsA, colsA, rowsB, colsB, time] : times) {
            file << rowsA << "," << colsA << "," << rowsB << "," << colsB << "," << time << "\n";
        }
        file.close();
    } else {
        cerr << "No se pudo abrir el archivo para escritura: " << filename << endl;
    }
}

// Función para obtener las dimensiones de una matriz desde su archivo
tuple<int, int> getMatrixDimensions(const string& filepath) {
    ifstream file(filepath, ios::binary);
    if (!file.is_open()) {
        cerr << "No se pudo abrir el archivo para lectura: " << filepath << endl;
        return {0, 0};
    }

    int rows, cols;
    file.read(reinterpret_cast<char*>(&rows), sizeof(rows));
    file.read(reinterpret_cast<char*>(&cols), sizeof(cols));
    file.close();

    return {rows, cols};
}

// Transponer una matriz
vector<vector<int>> transposeMatrix(const vector<vector<int>>& A) {
    int rows = A.size();
    int cols = A[0].size();
    vector<vector<int>> transposed(cols, vector<int>(rows));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            transposed[j][i] = A[i][j];
        }
    }

    return transposed;
}

signed main() {
    vector<string> squareFiles1;
    vector<string> squareFiles2;
    vector<string> rectangularFiles;

    // Cargar todos los archivos de matrices cuadradas
    for (const auto& entry : fs::directory_iterator("datasets/square_matrices1")) {
        squareFiles1.push_back(entry.path().string());
    }
    for (const auto& entry : fs::directory_iterator("datasets/square_matrices2")) {
        squareFiles2.push_back(entry.path().string());
    }

    // Cargar todos los archivos de matrices rectangulares
    for (const auto& entry : fs::directory_iterator("datasets/rectangular_matrices")) {
        rectangularFiles.push_back(entry.path().string());
    }

    // Ordenar archivos de matrices cuadradas por dimensiones
    sort(squareFiles1.begin(), squareFiles1.end(), [](const string& a, const string& b) {
        auto [rowsA, colsA] = getMatrixDimensions(a);
        auto [rowsB, colsB] = getMatrixDimensions(b);
        return rowsA < rowsB || (rowsA == rowsB && colsA < colsB);
    });

    sort(squareFiles2.begin(), squareFiles2.end(), [](const string& a, const string& b) {
        auto [rowsA, colsA] = getMatrixDimensions(a);
        auto [rowsB, colsB] = getMatrixDimensions(b);
        return rowsA < rowsB || (rowsA == rowsB && colsA < colsB);
    });

    // Ordenar archivos de matrices rectangulares por dimensiones
    sort(rectangularFiles.begin(), rectangularFiles.end(), [](const string& a, const string& b) {
        auto [rowsA, colsA] = getMatrixDimensions(a);
        auto [rowsB, colsB] = getMatrixDimensions(b);
        return rowsA < rowsB || (rowsA == rowsB && colsA < colsB);
    });

    vector<tuple<int, int, int, int, int>> squareMultiplicationTimes_t;
    vector<tuple<int, int, int, int, int>> rectangularMultiplicationTimes_t;

    // Multiplicar todas las combinaciones de matrices cuadradas
    for (const auto& fileA : squareFiles1) {
        for (const auto& fileB : squareFiles2) {
            auto matrixA = readMatrixFromBinaryFile(fileA);
            auto matrixB = readMatrixFromBinaryFile(fileB);
            auto matrixB_t = transposeMatrix(matrixB);
            if (matrixA.empty() || matrixB.empty()) {
                continue;
            }

            // Verificar que las dimensiones son compatibles para la multiplicación
            if (matrixA[0].size() != matrixB_t.size()) {
                continue;
            }

            // Medir el tiempo de multiplicación de matrices
            auto start = chrono::high_resolution_clock::now();
            vector<vector<int>> resultMatrix = multiplyMatrices(matrixA, matrixB_t);
            auto end = chrono::high_resolution_clock::now();
            int duration = chrono::duration_cast<chrono::microseconds>(end - start).count();

            // Extraer dimensiones de las matrices
            int rowsA = matrixA.size();
            int colsA = matrixA[0].size();
            int rowsB = matrixB.size();
            int colsB = matrixB[0].size();

            squareMultiplicationTimes_t.emplace_back(rowsA, colsA, rowsB, colsB, duration);
        }
    }
    
    // Multiplicar todas las combinaciones de matrices rectangulares
    for (const auto& fileA : rectangularFiles) {
        for (const auto& fileB : rectangularFiles) {
            if (fileA != fileB) {  // Evitar multiplicar una matriz por sí misma
                auto matrixA = readMatrixFromBinaryFile(fileA);
                auto matrixB = readMatrixFromBinaryFile(fileB);
                auto matrixB_t = transposeMatrix(matrixB);
                if (matrixA.empty() || matrixB.empty()) {
                    continue;
                }

                // Verificar que las dimensiones son compatibles para la multiplicación
                if (matrixA.size() == matrixB_t.size() && matrixA[0].size() == matrixB_t[0].size()) {
                    // Medir el tiempo de multiplicación de matrices
                    auto start = chrono::high_resolution_clock::now();
                    vector<vector<int>> resultMatrix = multiplyMatrices(matrixA, matrixB_t);
                    auto end = chrono::high_resolution_clock::now();
                    int duration = chrono::duration_cast<chrono::microseconds>(end - start).count();

                    // Extraer dimensiones de las matrices
                    int rowsA = matrixA.size();
                    int colsA = matrixA[0].size();
                    int rowsB = matrixB.size();
                    int colsB = matrixB[0].size();

                    rectangularMultiplicationTimes_t.emplace_back(rowsA, colsA, rowsB, colsB, duration);
                }
            }
        }
    }

    // Guardar los tiempos de multiplicación en archivos CSV
    saveTimesToCSV(squareMultiplicationTimes_t, "../CSV_times_M/square_multiplication_t_times.csv");
    saveTimesToCSV(rectangularMultiplicationTimes_t, "../CSV_times_M/rectangular_multiplication_t_times.csv");

    return 0;
}
