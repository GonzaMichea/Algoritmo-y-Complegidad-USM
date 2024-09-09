#include <bits/stdc++.h>
#include <chrono> 
#include <filesystem>
using namespace std;

// Función para leer un arreglo desde un archivo binario
vector<int> loadArrayFromBinaryFile(const string& filepath) {
    ifstream file(filepath, ios::binary);
    vector<int> array;
    if (file.is_open()) {
        size_t size;
        file.read(reinterpret_cast<char*>(&size), sizeof(size));
        array.resize(size);
        file.read(reinterpret_cast<char*>(array.data()), size * sizeof(int));
        file.close();
    } else {
        cerr << "No se pudo abrir el archivo: " << filepath << endl;
    }
    return array;
}

// Función de ordenamiento Selection Sort
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
    // Aquí almacenaremos los resultados
    ofstream results("sort_times.csv");
    results << "Dataset,Size,Time(microseconds)\n";

    // Ruta donde están guardados los archivos .bin
    string basePath = "data_sets/RandomArray/";

    // Recorre los archivos binarios de diferentes tamaños
    for (int size : {100, 1000, 10000, 100000}) {
        string filepath = basePath + to_string(size) + "/random_array_" + to_string(size) + ".bin";
        
        // Cargar el arreglo desde el archivo binario
        vector<int> array = loadArrayFromBinaryFile(filepath);

        // Medir el tiempo de ordenamiento
        auto start = chrono::high_resolution_clock::now();
        selectionSort(array);
        auto end = chrono::high_resolution_clock::now();

        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "Tiempo de ejecucion para tamaño " << size << ": " << duration.count() << " microsegundos" << endl;

        // Escribir los resultados en el archivo CSV
        results << "RandomArray," << size << "," << duration.count() << "\n";
    }

    results.close();
    return 0;
}