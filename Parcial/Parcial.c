#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>

#define N 600

int main(int argc, char *argv[]) {
    int rank, size;
    int i, j, k;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (N % size != 0) {
        if (rank == 0) {
            printf("El numero de procesos debe dividir N=%d exactamente.\n", N);
        }
        MPI_Finalize();
        return 0;
    }

    int rows_per_process = N / size;

    // Reservar memoria
    int *matrix_A = NULL;
    int *matrix_B = (int *)malloc(N * N * sizeof(int));
    int *local_A = (int *)malloc(rows_per_process * N * sizeof(int));
    int *local_C = (int *)calloc(rows_per_process * N, sizeof(int));
    int *matrix_C = NULL;

    if (rank == 0) {
        matrix_A = (int *)malloc(N * N * sizeof(int));
        matrix_C = (int *)malloc(N * N * sizeof(int));

        // Inicialización de matrices A y B
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                matrix_A[i * N + j] = i + j;
                matrix_B[i * N + j] = i - j;
            }
        }
    }

    // Inicia cronómetro
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    // Distribuir filas de A
    MPI_Scatter(matrix_A, rows_per_process * N, MPI_INT,
                local_A, rows_per_process * N, MPI_INT,
                0, MPI_COMM_WORLD);

    // Difundir B completa
    MPI_Bcast(matrix_B, N * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Cálculo paralelo con OpenMP
    #pragma omp parallel for private(i, j, k) shared(local_A, matrix_B, local_C)
    for (i = 0; i < rows_per_process; i++) {
        for (j = 0; j < N; j++) {
            int sum = 0;
            for (k = 0; k < N; k++) {
                sum += local_A[i * N + k] * matrix_B[k * N + j];
            }
            local_C[i * N + j] = sum;
        }
    }

    // Recolectar resultado en el proceso 0
    MPI_Gather(local_C, rows_per_process * N, MPI_INT,
               matrix_C, rows_per_process * N, MPI_INT,
               0, MPI_COMM_WORLD);

    // Detener cronómetro
    MPI_Barrier(MPI_COMM_WORLD);
    end_time = MPI_Wtime();

    // Mostrar resultados y rendimiento
    if (rank == 0) {
        printf("Multiplicacion completada.\n");
        double elapsed = end_time - start_time;
        printf("Tiempo total: %.4f segundos\n", elapsed);

        // Cálculo de speedup y eficiencia
        double baseline_time = elapsed;  // Usar un tiempo base (por ejemplo, con 1 proceso e hilo)
        double speedup = baseline_time / elapsed;
        double efficiency = speedup / size;

        printf("Speedup: %.2f\n", speedup);
        printf("Eficiencia: %.2f%%\n", efficiency * 100);
    }

    // Liberar memoria
    if (rank == 0) {
        free(matrix_A);
        free(matrix_C);
    }
    free(matrix_B);
    free(local_A);
    free(local_C);

    MPI_Finalize();
    return 0;
}