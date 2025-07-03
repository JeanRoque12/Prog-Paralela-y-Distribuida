#include <stdio.h>
#include <cuda_runtime.h>
#include <math.h>

#define N 1048576
#define BLOCK_SIZE 256

__global__ void vector_add(const float *A, const float *B, float *C, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n)
        C[i] = A[i] + B[i];
}

int main() {
    float *A = (float*) malloc(N * sizeof(float));
    float *B = (float*) malloc(N * sizeof(float));
    float *C = (float*) malloc(N * sizeof(float));

    for (int i = 0; i < N; i++) {
        A[i] = i * 0.5f;
        B[i] = i * 2.0f;
    }

    float *d_A, *d_B, *d_C;
    size_t size = N * sizeof(float);

    cudaMalloc(&d_A, size);
    cudaMalloc(&d_B, size);
    cudaMalloc(&d_C, size);

    cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    cudaEventRecord(start);
    int gridSize = (N + BLOCK_SIZE - 1) / BLOCK_SIZE;
    vector_add<<<gridSize, BLOCK_SIZE>>>(d_A, d_B, d_C, N);
    cudaEventRecord(stop);

    cudaEventSynchronize(stop);

    float kernel_time_ms = 0;
    cudaEventElapsedTime(&kernel_time_ms, start, stop);

    cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);

    printf("Tiempo kernel CUDA: %.6f ms\n", kernel_time_ms);

    int errors = 0;
    for (int i = 0; i < N; i++) {
        float expected = A[i] + B[i];
        if (fabs(C[i] - expected) > 1e-5) errors++;
    }

    if (errors == 0) printf("Suma CUDA correcta.\n");
    else printf("Errores en suma CUDA: %d\n", errors);

    cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);
    free(A); free(B); free(C);

    return 0;
}

