import pandas as pd
import matplotlib.pyplot as plt

def plot_csv_data(csv_file):
    # Leer el archivo CSV con pandas
    df = pd.read_csv(csv_file)
    
    # Mostrar las primeras filas del DataFrame para verificar que se ha leído correctamente
    print(df.head())
    
    # Crear una figura y ejes
    fig, ax = plt.subplots(figsize=(10, 6))
    
    # Graficar los tiempos de multiplicación
    for label in df.columns[4:]:
        ax.plot(df.index, df[label], label=label)
    
    # Configurar el gráfico
    ax.set_xlabel("Índice")
    ax.set_ylabel("Tiempo (microsegundos)")
    ax.set_title("Tiempos de Multiplicación de Matrices")
    ax.legend()
    ax.grid(True)

    # Guardar el gráfico en un archivo
    plt.savefig('plot.png')
    plt.show()

# Ruta al archivo CSV
csv_file = 'CSV_times_M/multiplication_times.csv'

# Llamar a la función para graficar los datos
plot_csv_data(csv_file)
