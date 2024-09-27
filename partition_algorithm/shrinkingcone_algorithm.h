//
// Created by 19210 on 2024/9/27.
//

#ifndef PIECEWISEGEOMETRICMODELINDEX_SHRINKINGCONE_ALGORITHM_H
#define PIECEWISEGEOMETRICMODELINDEX_SHRINKINGCONE_ALGORITHM_H
#include <cstdint>
#include <type_traits>
#include <vector>

namespace dili {

    template<typename K, size_t Epsilon = 64, typename Floating=double>
    class ShrinkingConeAlgorithm {
    private:
        static_assert(Epsilon > 0);
        struct Segment {
            K key;              ///< The first key that the segment indexes.
            Floating slope;     ///< The slope of the segment.
            uint32_t intercept; ///< The intercept of the segment.
            Segment() = default;
            Segment(K key_, Floating slope_, uint32_t intercept_): key(key_), slope(slope_), intercept(intercept_) {}
            operator K() { return key;}
            size_t operator()(const K &k) const {
                size_t pos;
                if constexpr (std::is_same_v<K, int64_t> || std::is_same_v<K, int32_t>)
                    pos = size_t(slope * double(std::make_unsigned_t<K>(k) - key));
                else
                    pos = size_t(slope * double(k - key));
                return pos + intercept;
            }
        };
        std::vector<Segment> segments;
    public:
        ShrinkingConeAlgorithm(const std::vector<K>& data): ShrinkingConeAlgorithm(data.begin(), data.end()) {}

        template<typename RandomIt>
        ShrinkingConeAlgorithm(RandomIt first, RandomIt last) {
            build(first, last);
        }

        std::vector<Segment>& getSegments() { return segments; }
    private:
        static double min_double(double d1, double d2) {
            if (d1 - d2 < 0) {
                return d1;
            }
            return d2;
        }

        static double max_double(double d1, double d2) {
            if (d1 - d2 < 0) {
                return d2;
            }
            return d1;
        }

        template<typename RandomIt>
        void build(RandomIt first, RandomIt last) {
            auto n = (size_t) std::distance(first, last);
            if (n == 0) {//data数量为0
                return;
            }
            double sl_high = 1e7; // infinite
            double sl_low = 0;
            int origin_loc = 0;
            std::vector<Segment> s_segs;
            std::vector<K> data;
            data.push_back(first[0]);
            for (int i = 1; i < n; ++i) {

                double k_up = i + Epsilon;
                double k_low = i - Epsilon;
                double max_bound = sl_high * (first[i] - first[origin_loc]) + origin_loc;
                double min_bound = sl_low * (first[i] - first[origin_loc]) + origin_loc;
//                double max_bound = sl_high * first[i];
//                double min_bound = sl_low * first[i];
                if (k_up >= min_bound || k_low <= max_bound) {//[k_low, k_up]区间有部分在圆锥体内
                    // (k_up - origin_loc) /  (first[i] - first[origin_loc])  <=> new_sl_high
                    // (k_low - origin_loc) / (first[i] - first[origin_loc]) <=> new_sl_low
                    if (k_up >= min_bound) {
                        sl_high = min_double((k_up - origin_loc) / (first[i] - first[origin_loc]), sl_high);//更新sl_high
                    }
                    if (k_low <= max_bound) {
                        sl_low = max_double((k_low - origin_loc) / (first[i] - first[origin_loc]), sl_low);//更新sl_low
                    }
                    data.push_back(first[i]);
                    if (i == n - 1) {//最后一个key
                        double slope = (i - origin_loc) / (first[i] - first[origin_loc]);
                        s_segs.emplace_back(Segment(first[origin_loc], slope, origin_loc));
                        data.resize(0);
                    }
                }
                else {// 在圆锥体外
                    double slope = 1.0 * ((i - 1) - origin_loc) / (first[i - 1] - first[origin_loc]);
                    s_segs.emplace_back(Segment(first[origin_loc], slope, origin_loc));
                    origin_loc = i;
                    sl_high = 1e9;
                    sl_low = 0;
                    data.clear();
                    data.resize(0);
                    data.push_back(first[i]);
                }
            }
            if (!data.empty()) {//最后一个key是单独一个segment
                s_segs.emplace_back(Segment(first[origin_loc], 0, origin_loc));
                data.resize(0);
            }
            segments = std::move(s_segs);
        }
    };
}


#endif //PIECEWISEGEOMETRICMODELINDEX_SHRINKINGCONE_ALGORITHM_H
