#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip>

using namespace std;
using namespace std::chrono;

int main() {
    const int N = 300;

    vector<vector<double>> A(N, vector<double>(N));
    vector<vector<double>> B(N, vector<double>(N));
    vector<vector<double>> C(N, vector<double>(N, 0.0));

    mt19937 gen(42);
    uniform_real_distribution<double> dist(0.0, 1.0);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = dist(gen);
            B[i][j] = dist(gen);
        }
    }

    auto t1 = high_resolution_clock::now();

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0.0; 
            for (int k = 0; k < N; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }

    auto t2 = high_resolution_clock::now();

    cout << "Tiempo version dependiente: "
         << duration_cast<milliseconds>(t2 - t1).count()
         << " ms\n";

    cout << "Valor de control C[0][0]: " << fixed << setprecision(6) << C[0][0] << "\n";

    return 0;
}