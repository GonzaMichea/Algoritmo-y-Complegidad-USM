#include <bits/stdc++.h>
#include <filesystem>
#include <chrono>
using namespace std;
namespace fs = std::filesystem;
#define int long long

vector<vector<int>> readMatrixFromBinaryFile(const string& filepath) {
    ifstream file(filepath, ios::binary);
    if (!file) {
        cerr << "No se pudo abrir el archivo para lectura: " << filepath << endl;
        return {};
    }

    int rows, cols;
    file.read(reinterpret_cast<char*>(&rows), sizeof(rows));
    file.read(reinterpret_cast<char*>(&cols), sizeof(cols));

    vector<vector<int>> matrix(rows, vector<int>(cols));
    file.read(reinterpret_cast<char*>(matrix.data()->data()), rows * cols * sizeof(int));
    return matrix;
}

// Función para sumar dos matrices
void sumar(vector<vector<int>>& result, const vector<vector<int>>& X, const vector<vector<int>>& Y) {
    int n = X.size();
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            result[i][j] = X[i][j] + Y[i][j];
}

void restar(vector<vector<int>>& result, const vector<vector<int>>& X, const vector<vector<int>>& Y) {
    int n = X.size();
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            result[i][j] = X[i][j] - Y[i][j];
}

vector<vector<int>> strassen(vector<vector<int>> &X, vector<vector<int>> &Y) {
    cout << "Strassen" << endl;
    int n = X.size();
    cout << "n: " << n << endl;
    if (n <= 64) {
        cout<<"case base"<<endl; // Base case size, adjust based on profiling
        vector<vector<int>> Z(n, vector<int>(n, 0));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                for (int k = 0; k < n; k++)
                    Z[i][j] += X[i][k] * Y[k][j];
                    cout<<"multi"<<endl;
        return Z;
    }

    int k = n / 2;
    vector<vector<int>> A(k, vector<int>(k)), B(k, vector<int>(k)), C(k, vector<int>(k)), D(k, vector<int>(k)),
                        E(k, vector<int>(k)), F(k, vector<int>(k)), G(k, vector<int>(k)), H(k, vector<int>(k));

    vector<vector<int>> Z11(k, vector<int>(k)), Z12(k, vector<int>(k)), Z21(k, vector<int>(k)), Z22(k, vector<int>(k)),
                        temp1(k, vector<int>(k)), temp2(k, vector<int>(k));

    for (int i = 0; i < k; i++)
        for (int j = 0; j < k; j++) {
            int index_i_k = i + k;
            int index_j_k = j + k;
            A[i][j] = X[i][j];
            B[i][j] = X[i][index_j_k];
            C[i][j] = X[index_i_k][j];
            D[i][j] = X[index_i_k][index_j_k];

            E[i][j] = Y[i][j];
            F[i][j] = Y[i][index_j_k];
            G[i][j] = Y[index_i_k][j];
            H[i][j] = Y[index_i_k][index_j_k];
        }

    restar(temp1, F, H); // F-H
    vector<vector<int>> P1 = strassen(A, temp1); // P1 = A(F-H)

    sumar(temp1, A, B); // A+B
    vector<vector<int>> P2 = strassen(temp1, H); // P2 = (A+B)H

    sumar(temp1, C, D); // C+D
    vector<vector<int>> P3 = strassen(temp1, E); // P3 = (C+D)E

    restar(temp1, G, E); // G-E
    vector<vector<int>> P4 = strassen(D, temp1); // P4 = D(G-E)

    sumar(temp1, A, D); // A+D
    sumar(temp2, E, H); // E+H
    vector<vector<int>> P5 = strassen(temp1, temp2); // P5 = (A+D)(E+H)

    restar(temp1, B, D); // B-D
    sumar(temp2, G, H);  // G+H
    vector<vector<int>> P6 = strassen(temp1, temp2); // P6 = (B-D)(G+H)

    restar(temp1, A, C); // A-C
    sumar(temp2, E, F);  // E+F
    vector<vector<int>> P7 = strassen(temp1, temp2); // P7 = (A-C)(E+F)

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            Z11[i][j] = P5[i][j] + P4[i][j] - P2[i][j] + P6[i][j];
            Z12[i][j] = P1[i][j] + P2[i][j];
            Z21[i][j] = P3[i][j] + P4[i][j];
            Z22[i][j] = P1[i][j] + P5[i][j] - P3[i][j] - P7[i][j];
        }
    }

    vector<vector<int>> Z(n, vector<int>(n));
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            Z[i][j] = Z11[i][j];
            Z[i][j + k] = Z12[i][j];
            Z[i + k][j] = Z21[i][j];
            Z[i + k][j + k] = Z22[i][j];
        }
    }

    return Z;
}
void saveTimesToCSV(const vector<tuple<int, int, int, int, int>>& times, const string& filename) {
    ofstream file(filename);
    if (file) {
        file << "Matrix A Rows,Matrix A Cols,Matrix B Rows,Matrix B Cols,Time (micro)\n";
        for (const auto& [rowsA, colsA, rowsB, colsB, time] : times) {
            file << rowsA << "," << colsA << "," << rowsB << "," << colsB << "," << time << "\n";
        }
    } else {
        cerr << "No se pudo abrir el archivo para escritura: " << filename << endl;
    }
}

tuple<int, int> getMatrixDimensions(const string& filepath) {
    ifstream file(filepath, ios::binary);
    if (!file) {
        cerr << "No se pudo abrir el archivo para lectura: " << filepath << endl;
        return {0, 0};
    }

    int rows, cols;
    file.read(reinterpret_cast<char*>(&rows), sizeof(rows));
    file.read(reinterpret_cast<char*>(&cols), sizeof(cols));
    return {rows, cols};
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
        auto [rowsA, colsA] = getMatrixDimensions(a);
        auto [rowsB, colsB] = getMatrixDimensions(b);
        return rowsA < rowsB || (rowsA == rowsB && colsA < colsB);
    });

    sort(squareFiles2.begin(), squareFiles2.end(), [](const string& a, const string& b) {
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

            // Medir el tiempo de multiplicación de matrices
            auto start = chrono::high_resolution_clock::now();
            vector<vector<int>> resultMatrix = strassen(matrixA, matrixB);
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

    saveTimesToCSV(squareMultiplicationTimes, "multiplication_times_square.csv");

    return 0;
}
