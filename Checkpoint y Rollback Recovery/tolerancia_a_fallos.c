#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CHECKPOINT_FILE_PREFIX "checkpoint_rank_"
#define MAX_ITER 20
#define CHECKPOINT_INTERVAL 5
#define FAILURE_ITER 10 // Simula fallo en esta iteración

typedef struct {
    int iter;
    int suma_parcial;
} Estado;

// Función para generar nombre de archivo del checkpoint
void get_checkpoint_filename(int rank, char *filename) {
    sprintf(filename, "%s%d.dat", CHECKPOINT_FILE_PREFIX, rank);
}

// Guarda el estado en un archivo local
void guardar_checkpoint(Estado *estado, int rank) {
    char filename[100];
    get_checkpoint_filename(rank, filename);
    FILE *f = fopen(filename, "wb");
    if (f == NULL) {
        perror("Error guardando checkpoint");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    fwrite(estado, sizeof(Estado), 1, f);
    fclose(f);
    printf("[Rank %d] Checkpoint guardado en iteracion %d\n", rank, estado->iter);
}

// Carga el estado desde archivo si existe
int cargar_checkpoint(Estado *estado, int rank) {
    char filename[100];
    get_checkpoint_filename(rank, filename);
    FILE *f = fopen(filename, "rb");
    if (f == NULL) {
        return 0; // No hay checkpoint
    }
    fread(estado, sizeof(Estado), 1, f);
    fclose(f);
    printf("[Rank %d] Checkpoint cargado desde iteracion %d\n", rank, estado->iter);
    return 1;
}

// Simula cómputo simple: suma de índice local
void computo(Estado *estado, int rank) {
    estado->suma_parcial += rank; // Simulación simple
}

// Simula fallo en cierta iteración
void simular_fallo(int iter, int rank) {
    if (iter == FAILURE_ITER && rank == 1) { // Solo proceso 1 falla
        printf("[Rank %d] Simulando fallo intencional en iteracion %d\n", rank, iter);
        exit(1);
    }
}

int main(int argc, char **argv) {
    Estado estado = {0, 0};
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 3) {
        if (rank == 0)
            fprintf(stderr, "Se requieren al menos 3 procesos.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Cargar checkpoint si existe
    int recuperado = cargar_checkpoint(&estado, rank);
    if (!recuperado) {
        estado.iter = 0;
        estado.suma_parcial = 0;
    }

    for (int i = estado.iter; i < MAX_ITER; i++) {
        estado.iter = i;

        computo(&estado, rank);
        printf("[Rank %d] Iteracion %d, suma_parcial = %d\n", rank, i, estado.suma_parcial);

        simular_fallo(i, rank);

        if (i % CHECKPOINT_INTERVAL == 0 && i != 0) {
            MPI_Barrier(MPI_COMM_WORLD); // Sincronización global
            guardar_checkpoint(&estado, rank);
        }

        sleep(1); // Para observar el progreso lentamente
    }

    printf("[Rank %d] Finalizando normalmente. Suma final = %d\n", rank, estado.suma_parcial);

    MPI_Finalize();
    return 0;
}
