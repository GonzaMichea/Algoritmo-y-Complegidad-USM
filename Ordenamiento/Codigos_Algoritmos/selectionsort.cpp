#include <bits/stdc++.h>
#include <chrono>
#include <filesystem>
#define int long long
using namespace std;

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

signed main() {
    ofstream results("../CSV_times/sort_times.csv");
    results << "Dataset,Size,Time(microseconds)\n";

    // Tipos de arreglos
    vector<string> types = {"RandomArray", "NearlySortedArray", "ReversedArray"};

    // Tamaños de los arreglos
    vector<int> sizes = {100, 1000, 10000, 20000, 40000, 60000, 80000, 100000};

    // Recorrer tipos de arreglos
    for (const string& type : types) {
        // Ruta base del .bin
        string basePath = "data_sets/" + type + "/";

        // Recorrer los archivos binarios de diferentes tamaños
        for (int size : sizes) {
            string filepath = basePath + to_string(size) + "/array_" + to_string(size) + ".bin";
            
            // Comprobar si el archivo existe
            if (!filesystem::exists(filepath)) {
                cerr << "No se encontró el archivo: " << filepath << endl;
                continue;
            }

            // Cargar el arreglo desde el archivo binario
            vector<int> array = loadArrayFromBinaryFile(filepath);

            // Medir el tiempo de ordenamiento
            auto start = chrono::high_resolution_clock::now();
            selectionSort(array);
            auto end = chrono::high_resolution_clock::now();

            auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
            cout << "Tiempo de ejecucion para " << type << " de tamaño " << size << ": " << duration.count() << " microsegundos" << endl;

            // Escribir los resultados en un CSV
            results << type << "," << size << "," << duration.count() << "\n";
        }
    }

    results.close();
    return 0;
}

