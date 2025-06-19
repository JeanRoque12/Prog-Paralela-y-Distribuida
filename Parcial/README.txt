Descripción
Este proyecto implementa la multiplicación de dos matrices grandes utilizando un enfoque híbrido de paralelismo:

MPI (Message Passing Interface): para distribuir la carga de trabajo entre múltiples procesos que pueden correr en diferentes nodos o núcleos.

OpenMP: para paralelizar el cálculo dentro de cada proceso usando múltiples hilos.

El programa divide la matriz A en bloques de filas que se distribuyen entre los procesos MPI. La matriz B se envía completa a todos los procesos. Cada proceso multiplica su bloque asignado utilizando OpenMP para paralelizar el cálculo entre hilos. Finalmente, los resultados se recopilan en el proceso raíz.

Requisitos
Compilador compatible con MPI y OpenMP (por ejemplo, mpicc con soporte OpenMP).

Entorno MPI instalado (por ejemplo, MPICH o Microsoft MPI).

Soporte para OpenMP en el compilador.

Sistema operativo Linux, macOS o Windows con entorno MPI configurado.

Archivos
matrix_mpi_openmp.c : Código fuente en C que implementa la multiplicación paralela.

Cómo compilar
Ejemplo usando mpicc:

bash
Copiar
Editar
mpicc -fopenmp matrix_mpi_openmp.c -o matrix_mpi_openmp
-fopenmp habilita OpenMP.

mpicc es el compilador MPI (puede variar según la instalación).

Cómo ejecutar
Para ejecutar con MPI y OpenMP, usa mpiexec o mpirun y define el número de procesos e hilos:

bash
Copiar
Editar
export OMP_NUM_THREADS=4   # Número de hilos por proceso OpenMP
mpiexec -n 2 ./matrix_mpi_openmp
-n 2 indica que se usan 2 procesos MPI.

Cada proceso usará 4 hilos OpenMP en este ejemplo.

Parámetros y restricciones
La matriz es cuadrada de tamaño N x N, donde N está definido en el código (#define N 600).

El número de procesos MPI debe dividir exactamente a N (por ejemplo, si N=600, los procesos pueden ser 1, 2, 3, 4, 5, 6, 10, 12, 15, 20, 30, 60, 100, 150, 300, o 600).

Ajusta el número de hilos OpenMP según la cantidad de núcleos disponibles en tu CPU.

Resultados
El programa imprime la duración total de la multiplicación.

Calcula el speedup y la eficiencia basada en un tiempo base secuencial (debes definir ese valor manualmente en el código).

El rendimiento puede variar según el hardware, el número de procesos e hilos, y el tamaño de la matriz.

Notas y recomendaciones
Para ver beneficios reales, usa matrices grandes y balancea bien el número de procesos y hilos según tus recursos.

Siempre sincroniza los procesos antes y después de la medición del tiempo con MPI_Barrier.

Verifica que tu compilador y entorno soporten OpenMP y MPI.

Si el tiempo no mejora con más procesos/hilos, revisa configuración y tamaño del problema.

Autor
Jean Roque