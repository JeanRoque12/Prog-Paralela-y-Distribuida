#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MAX_ITER 20
#define CHECKPOINT_ITER 5
#define FAIL_AT_ITER 10

typedef struct {
    int iter;
    int suma;
} Estado;

int main(int argc, char** argv) {
    int rank, size;
    Estado estado = {0, 0};
    char filename[100];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 3) {
        if (rank == 0)
            printf("Este programa requiere al menos 3 procesos.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    sprintf(filename, "checkpoint_rank%d.dat", rank);

    // Intentar recuperar el estado desde archivo
    FILE* f = fopen(filename, "rb");
    if (f) {
        fread(&estado, sizeof(Estado), 1, f);
        fclose(f);
        printf("[Rank %d] Checkpoint encontrado. Recuperando estado: iter=%d, suma=%d\n", rank, estado.iter, estado.suma);
    } else {
        printf("[Rank %d] Sin checkpoint previo. Iniciando desde cero.\n", rank);
    }

    // Sincronizacion antes de comenzar a trabajar
    MPI_Barrier(MPI_COMM_WORLD);

    for (int i = estado.iter + 1; i <= MAX_ITER; ++i) {
        estado.iter = i;
        estado.suma += 1;

        printf("[Rank %d] Iteracion %d, suma=%d\n", rank, estado.iter, estado.suma);

        if (estado.iter == CHECKPOINT_ITER) {
            FILE* fout = fopen(filename, "wb");
            fwrite(&estado, sizeof(Estado), 1, fout);
            fclose(fout);
            printf("[Rank %d] Checkpoint guardado en iteracion %d\n", rank, estado.iter);
        }

        // Simulacion de fallo intencional
        if (rank == 1 && estado.iter == FAIL_AT_ITER) {
            printf("[Rank %d] Simulando fallo intencional en iteracion %d\n", rank, estado.iter);
            exit(1);  // Finaliza abruptamente para simular error
        }
    }

    printf("[Rank %d] Proceso finalizado correctamente.\n", rank);
    MPI_Finalize();
    return 0;
}
