#include <iostream>
#include "src/LongNumber.cpp"
#include <chrono>


int main() {
    std::ios::sync_with_stdio(false);
    std::cout.tie(0);
    std::cout << "Enter number of fractional part: ";
    int n;
    std::cin >> n;
    ACCURACY = n * 4;
    auto start = std::chrono::high_resolution_clock::now();
    calc_PI(n);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time = end - start;
    std::cout << "Time: " << time.count() << std::endl;
    return 0;
}

