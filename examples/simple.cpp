/*
 * This example shows how to index and query a vector of random integers with the PGM-index.
 * Compile with:
 *   g++ simple.cpp -std=c++17 -I../include -o simple
 * Run with:
 *   ./simple
 */

#include <vector>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "pgm/pgm_index.hpp"

template<typename T>
T GCD(T a, T b) {
    if(b) while((a %= b) && (b %= a));
    return a + b;
}

const int epsilon = 8; // space-time trade-off parameter

void test_matrix0(pgm::PGMIndex<long, epsilon>& index) {
    auto matrix = index.getMatrix();
    for (auto &vec: matrix) {
        std::cout << vec.size() << std::endl;
    }
    std::cout << std::endl;
    std::vector<long> matrix0(matrix[0]);
//    for (int i = 0; i < matrix0.size(); ++i) {
//        std::cout << matrix0[i] << std::endl;
//    }

    std::vector<long> matrix0_diff(matrix0.size() - 1);
    for (int i = 0; i < matrix0_diff.size(); ++i) {
        matrix0_diff[i] = matrix0[i + 1] - matrix0[i];
    }
    std::sort(matrix0_diff.begin(), matrix0_diff.end());
    for (int i = 0; i < matrix0_diff.size(); ++i) {
        std::cout << matrix0_diff[i] << std::endl;
    }
}

void test_matrix_minus_1(pgm::PGMIndex<long, epsilon>& index) {
    auto matrix = index.getMatrix();
    for (auto &vec: matrix) {
        std::cout << vec.size() << std::endl;
    }
    std::cout << std::endl;
    std::vector<long> matrix_minus_1 = matrix[matrix.size() - 2];
//    for (long i : matrix_minus_1) {
//        std::cout << i << std::endl;
//    }

    std::vector<long> matrix_minus_1_diff(matrix_minus_1.size() - 1);
    for (int i = 0; i < matrix_minus_1_diff.size(); ++i) {
        matrix_minus_1_diff[i] = matrix_minus_1[i + 1] - matrix_minus_1[i];
    }
    std::sort(matrix_minus_1_diff.begin(), matrix_minus_1_diff.end());
    for (int i = 0; i < matrix_minus_1_diff.size(); ++i) {
        matrix_minus_1_diff[i] = matrix_minus_1_diff[i] / 1e6;
        std::cout << matrix_minus_1_diff[i] << std::endl;
    }
}

int main() {
    // Generate some random data
    std::vector<long> data(1000000);
    std::generate(data.begin(), data.end(), std::rand);
    data.push_back(42);
    std::sort(data.begin(), data.end());

    // Construct the PGM-index

    pgm::PGMIndex<long, epsilon> index(data);

    // Query the PGM-index
//    auto q = 42;
//    auto range = index.search(q);
//    std::cout << index.segment_index_for_key(q) << std::endl;
//    q = 13092309;
//    std::cout << index.segment_index_for_key(q) << std::endl;
//    auto lo = data.begin() + range.lo;
//    auto hi = data.begin() + range.hi;
//    std::cout << *std::lower_bound(lo, hi, q);
//
//    std::cout << std::endl;

    auto matrix = index.getMatrix();
    for (auto &vec: matrix) {
//        std::cout << vec.back() << std::endl;
        for (long i : vec) {
            std::cout << i << " ";
        }
        std::cout << "\n" << std::endl;
    }
    return 0;
}
