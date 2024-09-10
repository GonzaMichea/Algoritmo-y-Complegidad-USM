#include <bits/stdc++.h>
using namespace std;
#include <filesystem>
#include <chrono>
#define int long long

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

// Función para hacer la partición en Quick Sort usando el primer elemento como pivote
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[low]; // Elegir el primer elemento como pivote
    int left = low + 1;
    int right = high;

    while (left <= right) {
        // Encontrar un elemento mayor que el pivote
        while (left <= right && arr[left] <= pivot) {
            left++;
        }
        // Encontrar un elemento menor que el pivote
        while (left <= right && arr[right] > pivot) {
            right--;
        }
        // Intercambiar elementos si left es menor que right
        if (left < right) {
            swap(arr[left], arr[right]);
        }
    }
    // Colocar el pivote en su posición correcta
    swap(arr[low], arr[right]);
    return right;
}

// Función de ordenamiento Quick Sort
void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high); // Índice de partición

        // Ordenar las subpartes de forma recursiva
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
signed main(){
    std::ofstream results("../CSV_times/quick_times.csv");
    if (!results.is_open()) {
        cerr << "No se pudo abrir el archivo CSV para escribir." << endl;
        return 1;
    }
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
            quickSort(array, 0, array.size() - 1);
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