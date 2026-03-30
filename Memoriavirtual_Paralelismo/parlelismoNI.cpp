#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main() {
    const long long N = 500000000; 

    volatile long long x = 0;
    volatile long long a = 0, b = 0, c = 0, d = 0;

   
    auto t1 = high_resolution_clock::now();

    for (long long i = 0; i < N; i++) {
        x = x + 1;
    }

    auto t2 = high_resolution_clock::now();

    
    auto t3 = high_resolution_clock::now();

    for (long long i = 0; i < N; i++) {
        a = a + 1;
        b = b + 1;
        c = c + 1;
        d = d + 1;
    }

    auto t4 = high_resolution_clock::now();

    cout << "Tiempo dependencia: "
         << duration_cast<milliseconds>(t2 - t1).count()
         << " ms\n";

    cout << "Tiempo independiente: "
         << duration_cast<milliseconds>(t4 - t3).count()
         << " ms\n";

    cout << x << " " << a << " " << b << " " << c << " " << d << endl;

    return 0;
}