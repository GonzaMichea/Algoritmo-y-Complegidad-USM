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

// Función para sumar dos matrices
void sumMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

// Función para restar dos matrices
void subtractMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
}

// Multiplicación tradicional para tamaños pequeños
void traditionalMultiplication(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < n; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Implementación del algoritmo de Strassen
void strassen(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C, int n) {
    if (n <= 64) {  // Caso base: usa multiplicación tradicional
        traditionalMultiplication(A, B, C, n);
        return;
    }

    int newSize = n / 2;
    vector<vector<int>> A11(newSize, vector<int>(newSize)), A12(newSize, vector<int>(newSize));
    vector<vector<int>> A21(newSize, vector<int>(newSize)), A22(newSize, vector<int>(newSize));
    vector<vector<int>> B11(newSize, vector<int>(newSize)), B12(newSize, vector<int>(newSize));
    vector<vector<int>> B21(newSize, vector<int>(newSize)), B22(newSize, vector<int>(newSize));

    vector<vector<int>> M1(newSize, vector<int>(newSize)), M2(newSize, vector<int>(newSize));
    vector<vector<int>> M3(newSize, vector<int>(newSize)), M4(newSize, vector<int>(newSize));
    vector<vector<int>> M5(newSize, vector<int>(newSize)), M6(newSize, vector<int>(newSize));
    vector<vector<int>> M7(newSize, vector<int>(newSize));

    // Divide las matrices A y B en submatrices
    for (int i = 0; i < newSize; ++i) {
        for (int j = 0; j < newSize; ++j) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + newSize];
            A21[i][j] = A[i + newSize][j];
            A22[i][j] = A[i + newSize][j + newSize];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + newSize];
            B21[i][j] = B[i + newSize][j];
            B22[i][j] = B[i + newSize][j + newSize];
        }
    }

    // Calculo de los 7 productos intermedios
    vector<vector<int>> AResult(newSize, vector<int>(newSize));
    vector<vector<int>> BResult(newSize, vector<int>(newSize));

    sumMatrices(A11, A22, AResult, newSize);
    sumMatrices(B11, B22, BResult, newSize);
    strassen(AResult, BResult, M1, newSize);

    sumMatrices(A21, A22, AResult, newSize);
    strassen(AResult, B11, M2, newSize);

    subtractMatrices(B12, B22, BResult, newSize);
    strassen(A11, BResult, M3, newSize);

    subtractMatrices(B21, B11, BResult, newSize);
    strassen(A22, BResult, M4, newSize);

    sumMatrices(A11, A12, AResult, newSize);
    strassen(AResult, B22, M5, newSize);

    subtractMatrices(A21, A11, AResult, newSize);
    sumMatrices(B11, B12, BResult, newSize);
    strassen(AResult, BResult, M6, newSize);

    subtractMatrices(A12, A22, AResult, newSize);
    sumMatrices(B21, B22, BResult, newSize);
    strassen(AResult, BResult, M7, newSize);

    // Combina los resultados en la matriz C
    for (int i = 0; i < newSize; ++i) {
        for (int j = 0; j < newSize; ++j) {
            C[i][j] = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j]; // C11
            C[i][j + newSize] = M3[i][j] + M5[i][j]; // C12
            C[i + newSize][j] = M2[i][j] + M4[i][j]; // C21
            C[i + newSize][j + newSize] = M1[i][j] - M2[i][j] + M3[i][j] + M6[i][j]; // C22
        }
    }
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

// Función para obtener el tamaño de una matriz desde su archivo
int getMatrixSize(const string& filepath) {
    ifstream file(filepath, ios::binary);
    if (!file.is_open()) {
        cerr << "No se pudo abrir el archivo para lectura: " << filepath << endl;
        return 0;
    }

    int rows, cols;
    file.read(reinterpret_cast<char*>(&rows), sizeof(rows));
    file.read(reinterpret_cast<char*>(&cols), sizeof(cols));
    file.close();
    return rows; // Asumimos que es una matriz cuadrada
}

signed main() {
    vector<string> squareFiles1;
    vector<string> squareFiles2;

    // Cargar todos los archivos de matrices cuadradas
    for (const auto& entry : fs::directory_iterator("datasets/square_matrices1")) {
        squareFiles1.push_back(entry.path().string());
    }
    for (const auto& entry : fs::directory_iterator("datasets/square_matrices2")) {
        squareFiles2.push_back(entry.path().string());
    }

    // Ordenar archivos de matrices cuadradas por dimensiones
    sort(squareFiles1.begin(), squareFiles1.end(), [](const string& a, const string& b) {
        return getMatrixSize(a) < getMatrixSize(b);
    });
    sort(squareFiles2.begin(), squareFiles2.end(), [](const string& a, const string& b) {
        return getMatrixSize(a) < getMatrixSize(b);
    });

    vector<tuple<int, int, int, int, int>> squareMultiplicationTimes;

    // Multiplicar todas las combinaciones de matrices cuadradas
    for (const auto& fileA : squareFiles1) {
        for (const auto& fileB : squareFiles2) {
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
                vector<vector<int>> resultMatrix(matrixA.size(), vector<int>(matrixB[0].size()));
                strassen(matrixA, matrixB, resultMatrix, matrixA.size());
                auto end = chrono::high_resolution_clock::now();
                int duration = chrono::duration_cast<chrono::microseconds>(end - start).count();

                // Extraer dimensiones de las matrices
                int sizeA = matrixA.size();
                int sizeB = matrixB.size();

                squareMultiplicationTimes.emplace_back(sizeA, sizeB,sizeA, sizeB,duration);
            }
        }
    }

    // Guardar los tiempos en un archivo CSV
    saveTimesToCSV(squareMultiplicationTimes, "../CSV_times_M/strassen_multiplication_times.csv");

    return 0;
}
