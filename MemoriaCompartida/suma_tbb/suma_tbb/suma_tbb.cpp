#include <iostream>
#include <vector>
#include <tbb/parallel_reduce.h>
#include <tbb/blocked_range.h>
#include <tbb/global_control.h>

int main() {
    const int N = 1000;
    std::vector<int> datos(N);
    for (int i = 0; i < N; ++i)
        datos[i] = i;

    int suma = tbb::parallel_reduce(
        tbb::blocked_range<size_t>(0, datos.size()),
        0,
        [&](const tbb::blocked_range<size_t>& r, int local_sum) {
            for (size_t i = r.begin(); i < r.end(); ++i)
                local_sum += datos[i];
            return local_sum;
        },
        std::plus<>()
    );

    std::cout << "La suma total es: " << suma << std::endl;
    return 0;
}