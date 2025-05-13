import concurrent.futures
import time

def tarea(numero):
    print(f"Iniciando tarea {numero}")
    if (numero % 2 == 0):
        time.sleep(8)  # Simula una operación que tarda 8 segundos si se realiza una condicion
    else:
        time.sleep(5) #Si la condicion no es realizada, se espera 5 segundos en vez
    
    print(f"Tarea {numero} completada")
    return f"Resultado de {numero}"

def main():
    numeros = [1, 2, 3, 4]
    
    # Ejecutar tareas en paralelo usando ThreadPoolExecutor
    with concurrent.futures.ThreadPoolExecutor() as executor:
        resultados = executor.map(tarea, numeros)

    print("\n--- Resultados ---")
    for resultado in resultados:
        print(resultado)

if __name__ == "__main__":
    inicio = time.time()
    main()
    fin = time.time()
    print(f"\nTiempo total: {fin - inicio:.2f} segundos")