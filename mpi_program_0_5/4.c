#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rango, procesos;
    int valor_local, suma, recibido;
    int mascara;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rango);
    MPI_Comm_size(MPI_COMM_WORLD, &procesos);

    valor_local = rango + 1;
    suma = valor_local;

    mascara = 1;
    while (mascara < procesos) {
        int companero = rango ^ mascara;

        MPI_Sendrecv(&suma, 1, MPI_INT, companero, 0,
                     &recibido, 1, MPI_INT, companero, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        suma += recibido;
        mascara = mascara * 2;
    }

    printf("Proceso %d: suma global = %d\n", rango, suma);

    MPI_Finalize();
    return 0;
}