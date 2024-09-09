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

// Función para sumar dos matrices
vector<vector<int>> addMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int n = A.size();
    vector<vector<int>> C(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}

// Función para restar dos matrices
vector<vector<int>> subtractMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int n = A.size();
    vector<vector<int>> C(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return C;
}

// Función para dividir la matriz en submatrices
void splitMatrix(const vector<vector<int>>& P, vector<vector<int>>& C11, vector<vector<int>>& C12,
                 vector<vector<int>>& C21, vector<vector<int>>& C22) {
    int n = P.size() / 2;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            C11[i][j] = P[i][j];
            C12[i][j] = P[i][j + n];
            C21[i][j] = P[i + n][j];
            C22[i][j] = P[i + n][j + n];
        }
    }
}

// Función para combinar las submatrices en una sola matriz
void joinMatrices(vector<vector<int>>& P, const vector<vector<int>>& C11, const vector<vector<int>>& C12,
                  const vector<vector<int>>& C21, const vector<vector<int>>& C22) {
    int n = C11.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            P[i][j] = C11[i][j];
            P[i][j + n] = C12[i][j];
            P[i + n][j] = C21[i][j];
            P[i + n][j + n] = C22[i][j];
        }
    }
}

// Algoritmo de Strassen
vector<vector<int>> strassenMultiply(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int n = A.size();
    if (n == 1) {
        vector<vector<int>> C(1, vector<int>(1));
        C[0][0] = A[0][0] * B[0][0];
        return C;
    }

    int halfSize = n / 2;
    vector<vector<int>> A11(halfSize, vector<int>(halfSize));
    vector<vector<int>> A12(halfSize, vector<int>(halfSize));
    vector<vector<int>> A21(halfSize, vector<int>(halfSize));
    vector<vector<int>> A22(halfSize, vector<int>(halfSize));
    vector<vector<int>> B11(halfSize, vector<int>(halfSize));
    vector<vector<int>> B12(halfSize, vector<int>(halfSize));
    vector<vector<int>> B21(halfSize, vector<int>(halfSize));
    vector<vector<int>> B22(halfSize, vector<int>(halfSize));

    splitMatrix(A, A11, A12, A21, A22);
    splitMatrix(B, B11, B12, B21, B22);

    vector<vector<int>> M1 = strassenMultiply(addMatrices(A11, A22), addMatrices(B11, B22));
    vector<vector<int>> M2 = strassenMultiply(addMatrices(A21, A22), B11);
    vector<vector<int>> M3 = strassenMultiply(A11, subtractMatrices(B12, B22));
    vector<vector<int>> M4 = strassenMultiply(A22, subtractMatrices(B21, B11));
    vector<vector<int>> M5 = strassenMultiply(addMatrices(A11, A12), B22);
    vector<vector<int>> M6 = strassenMultiply(subtractMatrices(A21, A11), addMatrices(B11, B12));
    vector<vector<int>> M7 = strassenMultiply(subtractMatrices(A12, A22), addMatrices(B21, B22));

    vector<vector<int>> C11 = addMatrices(subtractMatrices(addMatrices(M1, M4), M5), M7);
    vector<vector<int>> C12 = addMatrices(M3, M5);
    vector<vector<int>> C21 = addMatrices(M2, M4);
    vector<vector<int>> C22 = addMatrices(subtractMatrices(addMatrices(M1, M3), M2), M6);

    vector<vector<int>> C(n, vector<int>(n));
    joinMatrices(C, C11, C12, C21, C22);

    return C;
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
            cout << "Matriz A: " << matrixA.size() << "x" << matrixA[0].size()
     << " con Matriz B: " << matrixB.size() << "x" << matrixB[0].size() << endl;
            // Verificar que las dimensiones son compatibles para la multiplicación
            if (matrixA[0].size() != matrixB.size()) {
                continue;
            }

            // Medir el tiempo de multiplicación de matrices usando Strassen
            auto start = chrono::high_resolution_clock::now();
            cout<<"multi"<<endl;
            vector<vector<int>> resultMatrix = strassenMultiply(matrixA, matrixB);
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

    // Mu;ltiplicar todas las combinaciones de matrices rectangulares
    /*
    for (const auto& fileA : rectangularFiles) {
        for (const auto& fileB : rectangularFiles) {
            if (fileA != fileB) {  // Evitar multiplicar una matriz por sí misma
                auto matrixA = readMatrixFromBinaryFile(fileA);
                auto matrixB = readMatrixFromBinaryFile(fileB);

                if (matrixA.empty() || matrixB.empty()) {
                    cerr << "Error al leer matrices: " << fileA << " o " << fileB << endl;
                    continue;
                }
                cout << "Matriz A: " << matrixA.size() << "x" << matrixA[0].size()
     << " con Matriz B: " << matrixB.size() << "x" << matrixB[0].size() << endl;
                // Verificar que las dimensiones son compatibles para la multiplicación
                if (matrixA[0].size() != matrixB.size()) {
                    continue;
                }

                // Medir el tiempo de multiplicación de matrices usando Strassen
                auto start = chrono::high_resolution_clock::now();
                vector<vector<int>> resultMatrix = strassenMultiply(matrixA, matrixB);
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
    */
    // Guardar los tiempos en archivos CSV separados
    saveTimesToCSV(squareMultiplicationTimes, "square_multiplication_strassen_times.csv");
    //saveTimesToCSV(rectangularMultiplicationTimes, "rectangular_multiplication_strassen_times.csv");

    return 0;
}
