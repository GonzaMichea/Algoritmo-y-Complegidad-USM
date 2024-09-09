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
// Función para combinar dos subarrays
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);

    // Copiar datos a los subarrays temporales
    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int i = 0; i < n2; ++i)
        R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;

    // Mezclar los subarrays de nuevo en el array original
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copiar los elementos restantes de L[], si hay alguno
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copiar los elementos restantes de R[], si hay alguno
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Función de ordenamiento Merge Sort
void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Ordenar la primera y segunda mitad
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        // Combinar las dos mitades ordenadas
        merge(arr, left, mid, right);
    }
}


signed main(){
    ofstream results("merge_times.csv");
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
            mergeSort(array, 0, array.size() - 1);
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
