//
// Created by 19210 on 2024/9/27.
//

#include "closure_algorithm.h"
#include "shrinkingcone_algorithm.h"
#include <iostream>
#include <random>
#include <chrono>
#include <unordered_set>
#include <vector>
#include <fstream>

#define DATA_SIZE 1000000
#define keyType long

static keyType searchKey = 20000;

static void data_init(std::vector<keyType>& keys) {
    std::random_device rd;
    std::mt19937 engine(rd());
//    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    std::uniform_int_distribution<keyType> dist(0, DATA_SIZE * 100);
    std::unordered_set<keyType> key_set;

    auto _n = DATA_SIZE;
    while (key_set.size() < _n) {
        keyType key = dist(engine);
        std::cout << key_set.size() << " ";
        key_set.insert(key);
    }
    std::cout << std::endl;
    std::copy(key_set.begin(), key_set.end(), std::back_inserter(keys));

//    shuffle(keys.begin(), keys.end(), std::default_random_engine(seed));
}

template <class T>
bool load_binary_data(T data[], int length, const std::string& file_path) {
    std::ifstream is(file_path.c_str(), std::ios::binary | std::ios::in);
    if (!is.is_open()) {
        return false;
    }
    is.read(reinterpret_cast<char*>(data), std::streamsize(length * sizeof(T)));
    is.close();
    return true;
}
int main() {
    keyType* keys = new keyType[DATA_SIZE];
    load_binary_data(keys, DATA_SIZE, "D:/ALEX/resources/ycsb-200M.bin.data");
    // Generate some random data
    std::vector<long> data(keys, keys + DATA_SIZE);

    //    data_init(data);
    data.push_back(searchKey);
    std::sort(data.begin(), data.end());
    auto iter = std::lower_bound(data.begin(), data.end(), searchKey);
    std::cout << searchKey << " real_position is " << std::distance(data.begin(), iter) << std::endl;

//    dili::ClosureAlgorithm<long> closureAlgorithm(data);
//    auto& segments = closureAlgorithm.getSegments();
    dili::ShrinkingConeAlgorithm<long> shrinkingConeAlgorithm(data);
    auto& segments = shrinkingConeAlgorithm.getSegments();
    std::cout << segments.size() << std::endl;
    for (int i = 0; i < segments.size(); ++i) {
        std::cout << "key " << segments[i].key << " ";
        std::cout << "slope " << segments[i].slope << " ";
        std::cout << "intercept " << segments[i].intercept << " ";
        std::cout << std::endl;
    }
    std::cout << searchKey << " predict_position is " << segments[7](searchKey) << std::endl;
}
