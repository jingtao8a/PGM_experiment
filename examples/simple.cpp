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

int main() {
    // Generate some random data
    std::vector<int> data(10000000);
    std::generate(data.begin(), data.end(), std::rand);
    data.push_back(42);
    std::sort(data.begin(), data.end());

    // Construct the PGM-index
    const int epsilon = 8; // space-time trade-off parameter
    pgm::PGMIndex<int, epsilon> index(data);

    // Query the PGM-index
    auto q = 42;
//    auto range = index.search(q);
    std::cout << index.segment_index_for_key(q) << std::endl;
    q = 13092309;
    std::cout << index.segment_index_for_key(q) << std::endl;
//    auto lo = data.begin() + range.lo;
//    auto hi = data.begin() + range.hi;
//    std::cout << *std::lower_bound(lo, hi, q);
//
//    std::cout << std::endl;

    auto matrix = index.getMatrix();
    for (auto& vec : matrix) {
        std::cout << vec.size() << std::endl;
    }
    std::cout << std::endl;
    return 0;
}