#include <iostream>
#include <thread>
#include <cmath>
#include <cstdint>
#include <ctime>

using namespace std;

const uint64_t Nt = 4;
const uint64_t kc = 100;

double Riemann_Zeta(double s, uint64_t k) {

    double result = 0.0;

    for (uint64_t i = 1; i < k; i++) {

        for (uint64_t j = 1; j < k; j++) {

            result += (2 * (i & 1) - 1) / pow(i + j, s);

        }
    }

    return result * pow(2, s);
}




void calcular_zeta(double x[], uint64_t k) {

    x[k] = Riemann_Zeta(2.0, k);

}


void calcular_bloques(double x[], uint64_t id) {

    uint64_t bloque = (kc + Nt - 1) / Nt;

    uint64_t inicio = id * bloque;

    uint64_t fin = inicio + bloque;

    if (fin > kc) {

        fin = kc;

    }

    for (uint64_t k = inicio; k < fin; k++) {

        x[k] = Riemann_Zeta(2.0, k);

    }

}




void calcular_saltos(double x[], uint64_t id) {

    for (uint64_t k = id; k < kc; k = k + Nt) {

        x[k] = Riemann_Zeta(2.0, k);

    }

}


int main() {

    double x_original[kc];
    double x_bloques[kc];
    double x_saltos[kc];

    thread threads[Nt];

    clock_t inicio;
    clock_t fin;


   
    inicio = clock();

    for (uint64_t k = 0; k < kc; k = k + Nt) {

        for (uint64_t t = 0; t < Nt; t++) {

            if (k + t < kc) {

                threads[t] = thread(calcular_zeta, x_original, k + t);

            }
        }

        for (uint64_t t = 0; t < Nt; t++) {

            if (k + t < kc) {

                threads[t].join();

            }
        }
    }

    fin = clock();

    cout << "Tiempo por saltos pero creando varios threads: "<< double(fin - inicio) / CLOCKS_PER_SEC
         << " segundos" << endl;



   

    inicio = clock();

    for (uint64_t t = 0; t < Nt; t++) {

        threads[t] = thread(calcular_bloques, x_bloques, t);

    }

    for (uint64_t t = 0; t < Nt; t++) {

        threads[t].join();

    }

    fin = clock();

    cout << "Tiempo por bloques: "
         << double(fin - inicio) / CLOCKS_PER_SEC
         << " segundos" << endl;



   

    inicio = clock();

    for (uint64_t t = 0; t < Nt; t++) {

        threads[t] = thread(calcular_saltos, x_saltos, t);

    }

    for (uint64_t t = 0; t < Nt; t++) {

        threads[t].join();

    }

    fin = clock();

    cout << "Tiempo saltos: " << double(fin - inicio) / CLOCKS_PER_SEC
         << " segundos" << endl;



   

    return 0;
}