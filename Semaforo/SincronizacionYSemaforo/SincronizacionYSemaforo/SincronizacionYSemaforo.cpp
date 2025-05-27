#include <iostream>
#include <windows.h>

#define NUM_HILOS 5
#define ITERACIONES 1000

int contador = 0;
HANDLE semaforo;

DWORD WINAPI incrementar(LPVOID lpParam) {
    for (int i = 0; i < ITERACIONES; ++i) {
        // Esperar acceso al sem�foro
        WaitForSingleObject(semaforo, INFINITE);

        // Secci�n cr�tica
        ++contador;

        // Liberar sem�foro
        ReleaseSemaphore(semaforo, 1, NULL);
    }
    return 0;
}

int main() {
    // Crear sem�foro con valor inicial 1 (semaforo binario)
    semaforo = CreateSemaphore(NULL, 1, 1, NULL);

    if (semaforo == NULL) {
        std::cerr << "Error creando el sem�foro." << std::endl;
        return 1;
    }

    HANDLE hilos[NUM_HILOS];

    // Crear hilos
    for (int i = 0; i < NUM_HILOS; ++i) {
        hilos[i] = CreateThread(NULL, 0, incrementar, NULL, 0, NULL);
    }

    // Esperar a que todos los hilos terminen
    WaitForMultipleObjects(NUM_HILOS, hilos, TRUE, INFINITE);

    // Cerrar handles de hilos
    for (int i = 0; i < NUM_HILOS; ++i) {
        CloseHandle(hilos[i]);
    }

    // Cerrar sem�foro
    CloseHandle(semaforo);

    std::cout << "Valor final del contador: " << contador << std::endl;

    return 0;
}