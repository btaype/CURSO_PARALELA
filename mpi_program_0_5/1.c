#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define DATOS 20
#define BINS 5

int main(int argc, char *argv[]) {

    int rango, procesos;
    int i, n_local, bin;
    double min = 0.0, max = 5.0;
    double ancho;

    double datos[DATOS] = {
        1.3, 2.9, 0.4, 0.3, 1.3,

        4.4, 1.7, 0.4,  3.2, 0.3,
        4.9,  2.4, 3.1, 4.4, 3.9,
        0.4, 4.2, 4.5,  4.9, 0.9
    };

    double *datos_locales;

    int hist_local[BINS];
    int hist_global[BINS];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rango);
    MPI_Comm_size(MPI_COMM_WORLD, &procesos);

    
    if (DATOS % procesos != 0) {
        if (rango == 0) {
            printf("Error: DATOS debe ser divisible entre procesos\n");
        }
        MPI_Finalize();
        return 0;
    }

    n_local = DATOS / procesos;
    ancho = (max - min) / BINS;

    datos_locales = (double *) malloc(n_local * sizeof(double));

    for (i = 0; i < BINS; i++) {
        hist_local[i] = 0;
        hist_global[i] = 0;
    }

   
    MPI_Scatter(datos, n_local, MPI_DOUBLE,
                datos_locales, n_local, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

   
    for (i = 0; i < n_local; i++) {

        if (datos_locales[i] == max)
            bin = BINS - 1;
        else
            bin = (int)((datos_locales[i] - min) / ancho);

        if (bin >= 0 && bin < BINS)
            hist_local[bin]++;
    }

    
    MPI_Reduce(hist_local, hist_global, BINS, MPI_INT, MPI_SUM,
               0, MPI_COMM_WORLD);

    
    if (rango == 0) {
        printf("Histograma final:\n");
        for (i = 0; i < BINS; i++) {
            printf("Bin %d: %d\n", i, hist_global[i]);
        }
    }

    free(datos_locales);
    MPI_Finalize();
    return 0;
}