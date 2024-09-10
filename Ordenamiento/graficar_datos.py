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
            time = int(row['Time(microseconds)']) / 1000  # Convertir a milisegundos
            
            if dataset in data:
                data[dataset]['sizes'].append(size)
                data[dataset]['times'].append(time)
                
    return data

# Leer datos de los CSV's
sort_data = read_data('CSV_times/sort_times.csv')
merge_data = read_data('CSV_times/merge_times.csv')
quick_data = read_data('CSV_times/quick_times.csv')
function_data = read_data('CSV_times/function_times.csv')

# Función para graficar en una celda de la subfigura
def plot_by_array_type(ax, array_type, sort_data, merge_data, quick_data, function_data):
    # Graficar tiempos de 'Sort'
    if array_type in sort_data:
        ax.plot(sort_data[array_type]['sizes'], sort_data[array_type]['times'], marker='>', linestyle='-', linewidth=2, label=f'Sort - {array_type}')
    
    # Graficar tiempos de 'Merge'
    if array_type in merge_data:
        ax.plot(merge_data[array_type]['sizes'], merge_data[array_type]['times'], marker='x', linestyle='--', linewidth=2, label=f'Merge - {array_type}')
    
    # Graficar tiempos de 'Quick'
    if array_type in quick_data:
        ax.plot(quick_data[array_type]['sizes'], quick_data[array_type]['times'], marker='^', linestyle='-.', linewidth=2, label=f'Quick - {array_type}')
    
    # Graficar tiempos de 'Function'
    if array_type in function_data:
        ax.plot(function_data[array_type]['sizes'], function_data[array_type]['times'], marker='o', linestyle='solid', linewidth=2, label=f'Function - {array_type}')
    
    ax.set_title(f'Tiempos de {array_type}')
    ax.set_xlabel('Tamaño del Arreglo')
    ax.set_ylabel('Tiempo (milisegundos)')
    ax.grid(True)
    ax.legend()
    
    # Ajustar los límites del eje Y (zoom en el eje Y hasta 5000 milisegundos)
    ax.set_ylim(0, 500)

# Crear un subplot de 2 filas y 2 columnas
fig, axs = plt.subplots(2, 2, figsize=(12, 10))  # Ajustar el tamaño según se necesite

# Graficar cada tipo de arreglo en una celda de los subplots
plot_by_array_type(axs[0, 0], 'RandomArray', sort_data, merge_data, quick_data, function_data)
plot_by_array_type(axs[0, 1], 'NearlySortedArray', sort_data, merge_data, quick_data, function_data)
plot_by_array_type(axs[1, 0], 'ReversedArray', sort_data, merge_data, quick_data, function_data)

# Eliminar el subplot vacío si tienes solo tres tipos de arreglo
fig.delaxes(axs[1, 1])

# Ajustar el espacio entre los subplots
plt.tight_layout()

# Mostrar la imagen con todos los gráficos
plt.show()
