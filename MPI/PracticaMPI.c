#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    int rank, size;
    int data;

    // Inicializa el entorno MPI
    MPI_Init(&argc, &argv);

    // Obtiene el identificador del proceso actual
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Determina el número total de procesos
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Comunicación punto a punto
    if (rank == 0) {
        data = 100;
        MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Proceso %d envio el valor %d al proceso 1\n", rank, data);
    } else if (rank == 1) {
        MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Proceso %d recibio el valor %d del proceso 0\n", rank, data);
    }

    // Finaliza el entorno MPI
    MPI_Finalize();

    return 0;
}
