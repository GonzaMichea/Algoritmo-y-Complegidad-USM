#include <bits/stdc++.h>
using namespace std;
//areglo totalmente random
vector<int> generateRandomArray(int size) {
    vector<int> arr(size);
    for (int i = 0; i < size; ++i) {
        arr[i] = rand() % 1000;
    }
    return arr;
}
//arreglo semi ordenado
vector<int> generateNearlySortedArray(int size) {
    vector<int> arr(size);
    for (int i = 0; i < size; ++i) {
        arr[i] = i;
    }
    // Intercambia algunos elementos para crear "casi ordenación"
    for (int i = 0; i < size / 10; ++i) {
        std::swap(arr[rand() % size], arr[rand() % size]);
    }
    return arr;
}

// matriz de n x n
vector<vector<int>> generateRandomMatrix(int n) {
    vector<vector<int>> matrix(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            matrix[i][j] = rand() % 100;
        }
    }
    return matrix;
}
//matriz de n x m
vector<vector<int>> generateRandomRectangularMatrix(int m, int n) {
    vector<vector<int>> matrix(m, vector<int>(n));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            matrix[i][j] = rand() % 100;
        }
    }
    return matrix;
}

void saveArrayToFile(const vector<int>& arr, const string& filename) {
    ofstream file(filename);
    for (int val : arr) {
        file << val << " ";
    }
    file.close();
}

void saveMatrixToFile(const vector<vector<int>>& matrix, const string& filename) {
    ofstream file(filename);
    for (const auto& row : matrix) {
        for (int val : row) {
            file << val << " ";
        }
        file << "\n";
    }
    file.close();
}
