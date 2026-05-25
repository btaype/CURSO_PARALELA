#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rango, procesos;
    int valor_local, suma, recibido;
    int paso;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rango);
    MPI_Comm_size(MPI_COMM_WORLD, &procesos);

    valor_local = rango + 1;
    suma = valor_local;

    paso = 1;
    while (paso < procesos) {

        if (rango % (2 * paso) == 0) {
            if (rango + paso < procesos) {
                MPI_Recv(&recibido, 1, MPI_INT, rango + paso, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                suma += recibido;
            }
        }
        else if (rango % (2 * paso) == paso) {
            MPI_Send(&suma, 1, MPI_INT, rango - paso, 0, MPI_COMM_WORLD);
            break;
        }

        paso = paso * 2;
    }

    if (rango == 0) {
        printf("Suma global = %d\n", suma);
    }

    MPI_Finalize();
    return 0;
}