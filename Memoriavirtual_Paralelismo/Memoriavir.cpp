#include <iostream>
#include <vector>
#include <chrono>
#include <sys/resource.h>
#include <iomanip>

using namespace std;
using namespace std::chrono;

struct PFInfo {
    long minflt;
    long majflt;
};

PFInfo obtenerPF() {
    rusage uso;
    getrusage(RUSAGE_SELF, &uso);
    return {uso.ru_minflt, uso.ru_majflt};
}

int main() {
    const size_t PAGE = 4096;
    const size_t BLOCK_SIZE = 256ULL * 1024 * 1024; 
    const size_t TOTAL_SIZE = 4ULL * 1024 * 1024 * 1024;
    const size_t NUM_BLOCKS = TOTAL_SIZE / BLOCK_SIZE;

    vector<char> datos;
    datos.resize(TOTAL_SIZE);

    cout << left << setw(10) << "Bloque"
         << setw(15) << "Tiempo(ms)"
         << setw(20) << "MinorPF nuevos"
         << setw(20) << "MajorPF nuevos"
         << "\n";

    cout << string(65, '-') << "\n";

    for (size_t b = 0; b < NUM_BLOCKS; b++) {
        size_t inicioBloque = b * BLOCK_SIZE;
        size_t finBloque = inicioBloque + BLOCK_SIZE;

        PFInfo antes = obtenerPF();
        auto t1 = high_resolution_clock::now();

        for (size_t i = inicioBloque; i < finBloque; i += PAGE) {
            datos[i] = 1; // tocar una vez por pagina
        }

        auto t2 = high_resolution_clock::now();
        PFInfo despues = obtenerPF();

        auto tiempo = duration_cast<milliseconds>(t2 - t1).count();

        cout << left << setw(10) << b
             << setw(15) << tiempo
             << setw(20) << (despues.minflt - antes.minflt)
             << setw(20) << (despues.majflt - antes.majflt)
             << "\n";
    }

    return 0;
}