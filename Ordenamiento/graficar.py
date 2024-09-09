import matplotlib.pyplot as plt
import csv

def read_data(filename):
    data = {
        'RandomArray': {'sizes': [], 'times': []},
        'NearlySortedArray': {'sizes': [], 'times': []},
        'ReversedArray': {'sizes': [], 'times': []}
    }
    
    with open(filename, 'r') as file:
        reader = csv.DictReader(file)
        for row in reader:
            dataset = row['Dataset']
            size = int(row['Size'])
            time = int(row['Time(microseconds)']) / 1000  # a milisegundos
            
            if dataset in data:
                data[dataset]['sizes'].append(size)
                data[dataset]['times'].append(time)
                
    return data

# Leer datos de los CSV's
sort_data = read_data('sort_times.csv')
merge_data = read_data('merge_times.csv')
quick_data = read_data('quick_times.csv')
function_data = read_data('function_times.csv')

# Graficar los datos de sort_times en su propia ventana
plt.figure(figsize=(8, 6))
for dataset, values in sort_data.items():
    plt.plot(values['sizes'], values['times'], marker='>', linestyle='-', label=f'{dataset} - Sort')
plt.title('Sort Times')
plt.xlabel('Tamaño del Arreglo')
plt.ylabel('Tiempo (milisegundos)')
plt.grid(True)
plt.legend()
plt.show()

# Graficar los datos de merge_times en su propia ventana
plt.figure(figsize=(8, 6))
for dataset, values in merge_data.items():
    plt.plot(values['sizes'], values['times'], marker='x', linestyle='--', label=f'{dataset} - Merge')
plt.title('Merge Times')
plt.xlabel('Tamaño del Arreglo')
plt.ylabel('Tiempo (milisegundos)')
plt.grid(True)
plt.legend()
plt.show()

# Graficar los datos de quick_times en su propia ventana
plt.figure(figsize=(8, 6))
for dataset, values in quick_data.items():
    plt.plot(values['sizes'], values['times'], marker='^', linestyle='-.', label=f'{dataset} - Quick')
plt.title('Quick Times')
plt.xlabel('Tamaño del Arreglo')
plt.ylabel('Tiempo (milisegundos)')
plt.grid(True)
plt.legend()
plt.show()

# Graficar los datos de function_times en su propia ventana
plt.figure(figsize=(8, 6))
for dataset, values in function_data.items():
    plt.plot(values['sizes'], values['times'], marker='o', linestyle='solid', label=f'{dataset} - Function')
plt.title('Function Times')
plt.xlabel('Tamaño del Arreglo')
plt.ylabel('Tiempo (milisegundos)')
plt.grid(True)
plt.legend()
plt.show()
