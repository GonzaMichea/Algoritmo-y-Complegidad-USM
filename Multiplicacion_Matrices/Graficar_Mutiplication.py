import matplotlib.pyplot as plt
import csv
import os

def read_data(filename, matrix_type):
    data = {
        'sizes': [],
        'times': []
    }
    
    if not os.path.isfile(filename):
        print(f"Error: El archivo {filename} no se encuentra.")
        return data
    
    with open(filename, 'r') as file:
        reader = csv.DictReader(file)
        for row in reader:
            try:
                a_rows = int(row['Matrix A Rows'])
                a_cols = int(row['Matrix A Cols'])
                b_rows = int(row['Matrix B Rows'])
                b_cols = int(row['Matrix B Cols'])
                size = max(a_rows * a_cols, b_rows * b_cols)  # Tamaño basado en la cantidad de elementos
                time = int(row['Time (micro)']) / 1000  # Convertir a milisegundos
                
                data['sizes'].append(size)
                data['times'].append(time)
            except KeyError as e:
                print(f"Error: Columna no encontrada en el archivo CSV. {e}")
                
    return data

# Leer datos de los CSV's
rectangular_data_iterativo = read_data('CSV_times_M/rectangular_multiplication_times.csv', 'Rectangular Iterativo')
rectangular_data_traspuesta = read_data('CSV_times_M/rectangular_multiplication_t_times.csv', 'Rectangular Transpuesta')
square_data_iterativo = read_data('CSV_times_M/square_multiplication_times.csv', 'Cuadrada Iterativo')
square_data_traspuesta = read_data('CSV_times_M/square_multiplication_t_times.csv', 'Cuadrada Transpuesta')
strassen_data = read_data('CSV_times_M/strassen_multiplication_times.csv', 'Cuadrada Strassen')

# Función para graficar en una celda de la subfigura
def plot_matrix_times(ax, data, title, linestyle='-', marker='o', color='blue'):
    sizes = data['sizes']
    times = data['times']
    
    if sizes and times:
        ax.plot(sizes, times, marker=marker, linestyle=linestyle, color=color, linewidth=2, label=title)
    
    ax.set_xlabel('Número de Elementos')
    ax.set_ylabel('Tiempo (milisegundos)')
    ax.grid(True, which='both', linestyle='--', linewidth=0.5)
    ax.legend()
    
    # Ajustar los límites del eje Y si es necesario
    if times:
        ax.set_ylim(0, max(times) * 1.1)

# Crear un subplot de 2 filas y 2 columnas
fig, axs = plt.subplots(2, 2, figsize=(16, 12))  # Ajustar el tamaño según se necesite

# Graficar los datos de matrices rectangulares en un solo gráfico
axs[0, 0].plot(rectangular_data_iterativo['sizes'], rectangular_data_iterativo['times'], marker='o', linestyle='', color='blue', label='Rectangular Iterativo')
axs[0, 0].plot(rectangular_data_traspuesta['sizes'], rectangular_data_traspuesta['times'], marker='s', linestyle='', color='red', label='Rectangular Transpuesta')
axs[0, 0].set_title('Matrices Rectangulares')
axs[0, 0].set_xlabel('Número de Elementos')
axs[0, 0].set_ylabel('Tiempo (milisegundos)')
axs[0, 0].grid(True, which='both', linestyle='--', linewidth=0.5)
axs[0, 0].legend()

# Graficar todos los datos de matrices cuadradas en un solo gráfico
axs[0, 1].plot(square_data_iterativo['sizes'], square_data_iterativo['times'], marker='o', linestyle='-', color='blue', linewidth=2, label='Cuadrada Iterativo')
axs[0, 1].plot(square_data_traspuesta['sizes'], square_data_traspuesta['times'], marker='s', linestyle='--', color='red', linewidth=2, label='Cuadrada Transpuesta')
axs[0, 1].plot(strassen_data['sizes'], strassen_data['times'], marker='^', linestyle='-.', color='green', linewidth=2, label='Cuadrada Strassen')
axs[0, 1].set_title('Matrices Cuadradas')
axs[0, 1].set_xlabel('Dimensiones de la Matriz')
axs[0, 1].set_ylabel('Tiempo (milisegundos)')
axs[0, 1].grid(True, which='both', linestyle='--', linewidth=0.5)
axs[0, 1].legend()

# Eliminar el subplot vacío si no es necesario
fig.delaxes(axs[1, 0])
fig.delaxes(axs[1, 1])

# Ajustar el espacio entre los subplots
plt.tight_layout()

# Mostrar la imagen con todos los gráficos
plt.show()
