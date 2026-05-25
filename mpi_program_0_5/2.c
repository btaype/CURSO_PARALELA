#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rango, procesos;
    long long lanzamientos_totales;
    long long lanzamientos_locales;
    long long dentro_local = 0;
    long long dentro_global = 0;
    long long i;

    double x, y;
    double pi;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rango);
    MPI_Comm_size(MPI_COMM_WORLD, &procesos);

    if (rango == 0) {
        printf("Ingrese el numero total de lanzamientos: ");
        scanf("%lld", &lanzamientos_totales);
    }


    MPI_Bcast(&lanzamientos_totales, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    
    lanzamientos_locales = lanzamientos_totales / procesos;

    /* Semilla distinta para cada proceso */
    srand((unsigned) time(NULL) + rango);

    
    for (i = 0; i < lanzamientos_locales; i++) {
        x = -1.0 + 2.0 * rand() / (double) RAND_MAX;
        y = -1.0 + 2.0 * rand() / (double) RAND_MAX;

        if (x * x + y * y <= 1.0) {
            dentro_local++;
        }
    }

    
    MPI_Reduce(&dentro_local, &dentro_global, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rango == 0) {
        pi = 4.0 * dentro_global / (double)(lanzamientos_locales * procesos);
        printf("Estimacion de pi = %.10f\n", pi);
    }

    MPI_Finalize();
    return 0;
}