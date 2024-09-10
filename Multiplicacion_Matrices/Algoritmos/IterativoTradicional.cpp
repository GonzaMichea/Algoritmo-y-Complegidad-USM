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
    // Leer las dimensiones de la matriz
    file.read(reinterpret_cast<char*>(&rows), sizeof(rows));
    file.read(reinterpret_cast<char*>(&cols), sizeof(cols));

    vector<vector<int>> matrix(rows, vector<int>(cols));
    // Leer los elementos de la matriz
    for (int i = 0; i < rows; ++i) {
        file.read(reinterpret_cast<char*>(matrix[i].data()), cols * sizeof(int));
    }

    file.close();
    return matrix;
}

// Multiplicar dos matrices
vector<vector<int>> multiplyMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int n = A.size();      // Filas de A
    int m = B.size();      // Filas de B (columnas de A)
    int p = B[0].size();   // Columnas de B
    vector<vector<int>> result(n, vector<int>(p, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < p; ++j) {
            for (int k = 0; k < m; ++k) {
                result[i][j] += A[i][k] * B[k][j];
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

    vector<tuple<int, int, int, int, int>> squareMultiplicationTimes;
    vector<tuple<int, int, int, int, int>> rectangularMultiplicationTimes;

    // Multiplicar todas las combinaciones de matrices cuadradas
    for (const auto& fileA : squareFiles1) {
        for (const auto& fileB : squareFiles2) {
            auto matrixA = readMatrixFromBinaryFile(fileA);
            auto matrixB = readMatrixFromBinaryFile(fileB);
            if (matrixA.empty() || matrixB.empty()) {
                cerr << "Error al leer matrices: " << fileA << " o " << fileB << endl;
                continue;
            }
            // Verificar que las dimensiones son compatibles para la multiplicación
            if (matrixA[0].size() != matrixB.size()) {
                continue;
            }

            // Medir el tiempo de multiplicación de matrices
            auto start = chrono::high_resolution_clock::now();
            vector<vector<int>> resultMatrix = multiplyMatrices(matrixA, matrixB);
            auto end = chrono::high_resolution_clock::now();
            int duration = chrono::duration_cast<chrono::microseconds>(end - start).count();

            // Extraer dimensiones de las matrices
            int rowsA = matrixA.size();
            int colsA = matrixA[0].size();
            int rowsB = matrixB.size();
            int colsB = matrixB[0].size();

            squareMultiplicationTimes.emplace_back(rowsA, colsA, rowsB, colsB, duration);
        }
    }

    // Multiplicar todas las combinaciones de matrices rectangulares
    for (const auto& fileA : rectangularFiles) {
        for (const auto& fileB : rectangularFiles) {
            if (fileA != fileB) {  // Evitar multiplicar una matriz por sí misma
                auto matrixA = readMatrixFromBinaryFile(fileA);
                auto matrixB = readMatrixFromBinaryFile(fileB);

                if (matrixA.empty() || matrixB.empty()) {
                    cerr << "Error al leer matrices: " << fileA << " o " << fileB << endl;
                    continue;
                }

                // Verificar que las dimensiones son compatibles para la multiplicación
                if (matrixA[0].size() != matrixB.size()) {
                    continue;
                }

                // Medir el tiempo de multiplicación de matrices
                auto start = chrono::high_resolution_clock::now();
                vector<vector<int>> resultMatrix = multiplyMatrices(matrixA, matrixB);
                auto end = chrono::high_resolution_clock::now();
                int duration = chrono::duration_cast<chrono::microseconds>(end - start).count();

                // Extraer dimensiones de las matrices
                int rowsA = matrixA.size();
                int colsA = matrixA[0].size();
                int rowsB = matrixB.size();
                int colsB = matrixB[0].size();

                rectangularMultiplicationTimes.emplace_back(rowsA, colsA, rowsB, colsB, duration);
            }
        }
    }

    // Guardar los tiempos en archivos CSV separados
    saveTimesToCSV(squareMultiplicationTimes, "../CSV_times_M/square_multiplication_times.csv");
    saveTimesToCSV(rectangularMultiplicationTimes, "../CSV_times_M/rectangular_multiplication_times.csv");

    return 0;
}


