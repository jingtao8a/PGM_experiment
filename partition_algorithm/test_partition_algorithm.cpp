//
// Created by 19210 on 2024/9/27.
//

#include "closure_algorithm.h"
#include <iostream>

int main() {
    // Generate some random data
    std::vector<long> data(1000000);
    std::generate(data.begin(), data.end(), std::rand);
    data.push_back(42);
    std::sort(data.begin(), data.end());
    auto iter = std::lower_bound(data.begin(), data.end(), 42);
    std::cout << std::distance(data.begin(), iter) << std::endl;
    dili::ClosureAlgorithm<long> closureAlgorithm(data);
    auto& segments = closureAlgorithm.getSegments();
    for (int i = 0; i < segments.size(); ++i) {
        std::cout << segments[i].key << " ";
    }
    std::cout << std::endl;
    std::cout << segments[0](42) << std::endl;
}
