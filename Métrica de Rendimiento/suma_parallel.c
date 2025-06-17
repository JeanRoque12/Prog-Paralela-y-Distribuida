#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define SIZE 100000000  // 100 millones

int main() {
    int *arr = malloc(SIZE * sizeof(int));
    if (arr == NULL) {
        printf("Error al asignar memoria.\n");
        return 1;
    }

    // Inicializar arreglo con valores simples (1)
    for (int i = 0; i < SIZE; i++) {
        arr[i] = 1;
    }

    int num_threads;
    long long sum = 0;

    // Pedir número de hilos al usuario
    printf("Ingrese el numero de hilos: ");
    scanf("%d", &num_threads);

    double start_time = omp_get_wtime();

    // Establecer número de hilos
    omp_set_num_threads(num_threads);

    // Sumar los elementos usando reducción paralela
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < SIZE; i++) {
        sum += arr[i];
    }

    double end_time = omp_get_wtime();

    printf("Suma total: %lld\n", sum);
    printf("Tiempo de ejecucion con %d hilo(s): %f segundos\n", num_threads, end_time - start_time);

    free(arr);
    return 0;
}
