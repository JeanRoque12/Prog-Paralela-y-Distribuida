#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1048576

int main() {
    float *A = (float*) malloc(N * sizeof(float));
    float *B = (float*) malloc(N * sizeof(float));
    float *C = (float*) malloc(N * sizeof(float));

    for (int i = 0; i < N; i++) {
        A[i] = i * 0.5f;
        B[i] = i * 2.0f;
    }

    double start = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        C[i] = A[i] + B[i];
    }

    double end = omp_get_wtime();
    printf("Tiempo CPU (OpenMP): %.6f segundos\n", end - start);

    double cuda_time;
    printf("Ingrese el tiempo de ejecución CUDA (en segundos) para calcular speedup: ");
    scanf("%lf", &cuda_time);

    if (cuda_time > 0) {
        printf("Speedup CPU vs CUDA: %.2f\n", (end - start) / cuda_time);
    }

    free(A); free(B); free(C);
    return 0;
}