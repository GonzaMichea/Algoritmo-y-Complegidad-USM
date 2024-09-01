#ifndef DATASETS_H
#define DATASETS_H

#include <vector>
#include <string>

// Genera un arreglo completamente aleatorio
std::vector<int> generateRandomArray(int size);

// Genera un arreglo casi ordenado
std::vector<int> generateNearlySortedArray(int size);

// Genera una matriz cuadrada aleatoria de tamaño n x n
std::vector<std::vector<int>> generateRandomMatrix(int n);

// Genera una matriz rectangular aleatoria de tamaño m x n
std::vector<std::vector<int>> generateRandomRectangularMatrix(int m, int n);

// Guarda un arreglo en un archivo
void saveArrayToFile(const std::vector<int>& arr, const std::string& filename);

// Guarda una matriz en un archivo
void saveMatrixToFile(const std::vector<std::vector<int>>& matrix, const std::string& filename);

#endif
