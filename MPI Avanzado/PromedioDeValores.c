#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int N;
    int *local_data = NULL;
    int local_sum = 0, total_sum = 0;
    float average;

    // Inicialización MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Obtener el identificador del proceso
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Obtener el número total de procesos

    // Proceso raíz solicita el tamaño N
    if (rank == 0) {
        printf("Ingrese la cantidad de valores por proceso (N): ");
        fflush(stdout);
        scanf("%d", &N);
    }

    // Broadcast de N a todos los procesos
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Semilla diferente por proceso
    srand(time(NULL) + rank);

    // Asignar memoria y generar datos aleatorios
    local_data = (int *)malloc(N * sizeof(int));
    for (int i = 0; i < N; i++) {
        local_data[i] = rand() % 100;  // Valores aleatorios entre 0 y 99
        local_sum += local_data[i];
    }

    // Reducir todas las sumas locales en el proceso raíz
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Proceso raíz calcula el promedio total
    if (rank == 0) {
        average = (float)total_sum / (N * size);
    }

    // Broadcast del promedio a todos los procesos
    MPI_Bcast(&average, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Cada proceso imprime el promedio recibido
    printf("Proceso %d: Promedio total = %.2f\n", rank, average);

    // Liberar recursos y finalizar
    free(local_data);
    MPI_Finalize();
    return 0;
}
