#include <bits/stdc++.h>
#include <filesystem>
using namespace std;
#define int long long
void createDirectory(const string& path){
    std::filesystem::create_directories(path);
}

void saveArrayToBinaryFile(const vector<int>& array, const string& path, const string& filename) {
    createDirectory(path);
    
    // Abre el archivo en modo binario
    ofstream file(path + "/" + filename, ios::binary);
    if (file.is_open()) {
        size_t size = array.size();
        
        // Escribe el tamaño del array y luego el contenido del array
        file.write(reinterpret_cast<const char*>(&size), sizeof(size));
        file.write(reinterpret_cast<const char*>(array.data()), size * sizeof(int));
        file.close();
    } else {
        cerr << "No se pudo abrir el archivo para escritura: " << filename << endl;
    }
}

// Genera un arreglo totalmente aleatorio
vector<int> generateRandomArray(int size) {
    vector<int> arr(size);
    for (int i = 0; i < size; ++i) {
        arr[i] = rand() % 1000;
    }
    return arr;
}

// Genera un arreglo casi ordenado
vector<int> generateNearlySortedArray(int size) {
    vector<int> arr(size);
    for (int i = 0; i < size; ++i) {
        arr[i] = i;
    }
    for (int i = 0; i < size / 10; ++i) {
        swap(arr[rand() % size], arr[rand() % size]);
    }
    return arr;
}
//Genera un arreglo ordenado desentemente
vector<int> generateReversedArray(int size) {
    vector<int> arr(size);
    for (int i = 0; i < size; ++i) {
        arr[i] = size - i;
    }
    return arr;
}
signed main() {
    vector<int> sizes = {100, 1000, 10000, 20000, 40000, 60000, 80000, 100000};

    for (int size : sizes) {
        // Generar y guardar arreglos aleatorios
        vector<int> randomArray = generateRandomArray(size);
        saveArrayToBinaryFile(randomArray, "data_sets/RandomArray/" + to_string(size), "array_" + to_string(size) + ".bin");

        // Generar y guardar arreglos casi ordenados
        vector<int> nearlySortedArray = generateNearlySortedArray(size);
        saveArrayToBinaryFile(nearlySortedArray, "data_sets/NearlySortedArray/" + to_string(size), "array_" + to_string(size) + ".bin");
        //Generar y guardar arreglos ordenado desendentemente
        vector<int> reverseArray = generateReversedArray(size);
        saveArrayToBinaryFile(reverseArray, "data_sets/ReversedArray/" + to_string(size), "array_" + to_string(size) + ".bin");
    }

    return 0;
}