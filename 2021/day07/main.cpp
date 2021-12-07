#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>
#include <map>
#include <cmath>
#include <climits>

namespace {

std::int64_t Part1(const std::vector<int> &data) {
    std::map<int, int> m;
    int min = INT_MAX;
    int max = INT_MIN;
    for (int d : data) {
        ++m[d];
        min = std::min(min, d);
        max = std::max(max, d);
    }

    std::int64_t ret = INT_MAX;
    for (int i = min; i <= max; ++i) {
        std::int64_t sum = 0;
        for (const auto &it : m) {
            sum += std::abs(i - it.first) * it.second;
        }

        ret = std::min(ret, sum);
    }

    return ret;
}

std::int64_t Part2(const std::vector<int> &data) {
    std::map<int, int> m;
    int min = INT_MAX;
    int max = INT_MIN;
    for (int d : data) {
        ++m[d];
        min = std::min(min, d);
        max = std::max(max, d);
    }

    std::int64_t ret = INT_MAX;
    for (int i = min; i <= max; ++i) {
        std::int64_t sum = 0;
        for (const auto &it : m) {
            std::int64_t diff = std::abs(i - it.first);
            if (diff == 0) {
                continue;
            }

            std::int64_t fuel = (1 + diff) * diff / 2;
            sum += fuel * it.second;
        }

        ret = std::min(ret, sum);
    }

    return ret;
}

void Test() {
    std::vector<int> data{16, 1, 2, 0, 4, 2, 7, 1, 2, 14};
    assert(Part1(data) == 37);
    assert(Part2(data) == 168);
}

} // namespace

int main() {
    Test();

    std::vector<int> data;
    std::string tmp;
    while (std::getline(std::cin, tmp, ',')) {
        if (tmp.empty()) {
            continue;
        }

        data.push_back(std::stoi(tmp));
    }

    auto part1 = Part1(data);
    auto part2 = Part2(data);

    std::cout << "Part1: " << part1 << std::endl;
    std::cout << "Part2: " << part2 << std::endl;

    return 0;
}