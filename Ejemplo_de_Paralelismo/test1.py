import concurrent.futures
import time

def tarea(nombre):
    print(f"Iniciando tarea {nombre}")
    time.sleep(5)  # Simula una operación que tarda 5 segundos
    print(f"Tarea {nombre} completada")
    return f"Resultado de {nombre}"

def main():
    nombres = ["A", "B", "C", "D"]
    # Ejecutar tareas en paralelo usando ThreadPoolExecutor
    with concurrent.futures.ThreadPoolExecutor() as executor:
        resultados = executor.map(tarea, nombres)

    print("\n--- Resultados ---")
    for resultado in resultados:
        print(resultado)

if __name__ == "__main__":
    inicio = time.time()
    main()
    fin = time.time()
    print(f"\nTiempo total: {fin - inicio:.2f} segundos")