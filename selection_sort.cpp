#include <bits/stdc++.h>
#include "datasets.h" // Archivo de encabezado para declarar funciones de datasets
#include <chrono> 
using namespace std;

void selectionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        swap(arr[i], arr[minIndex]);
    }
}

int main() {
    vector<int> randomArray = generateRandomArray(100);

    // Guardar el dataset generado en un archivo antes de ordenar
    saveArrayToFile(randomArray, "random_array_before_sorting.txt");

    // Aplicar Selection Sort al dataset
    cout << "Arreglo sin ordenar:" << endl;
    for (int i : randomArray) {
        cout << i << " ";
    }
    cout << endl;
// Capturar el tiempo antes de comenzar el ordenamiento
    auto start = chrono::high_resolution_clock::now();

    // Aplicar Selection Sort al dataset
    selectionSort(randomArray);

    // Capturar el tiempo después de completar el ordenamiento
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "Arreglo Ordenado Por Selection Sort:" << endl;
    for (int i : randomArray) {
        cout << i << " ";
    }
    cout << endl;
    // Mostrar el tiempo de ejecución
    cout << "Tiempo de ejecucion: " << duration.count() << " microsegundos" << endl;
    return 0;
}
