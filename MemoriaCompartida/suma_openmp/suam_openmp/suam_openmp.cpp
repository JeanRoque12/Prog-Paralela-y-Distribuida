#include <iostream>
#include <omp.h>

int main() {
    const int N = 1000;
    int suma = 0;

#pragma omp parallel for reduction(+:suma)
    for (int i = 0; i < N; ++i) {
        suma += i;
    }

    std::cout << "La suma de 0 a " << N - 1 << " es: " << suma << std::endl;
    return 0;
}