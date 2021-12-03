#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

namespace {

std::int64_t PowerConsumption(const std::vector<std::string> &values) {
    std::int64_t gamma = 0;
    std::int64_t epsilon = 0;

    int len = values[0].size();
    for (size_t i = 0; i < len; ++i) {
        int ones = 0, zeros = 0;
        for (const auto &value : values) {
            if (value[i] == '1') {
                ++ones;
            } else {
                ++zeros;
            }
        }

        int g = ones > zeros ? 1 : 0;
        int e = g == 1 ? 0 : 1;
        gamma = gamma * 2 + g;
        epsilon = epsilon * 2 + e;
    }

    return gamma * epsilon;
}

void Test1() {
    // clang-format off
    std::vector<std::string> values {
        "00100",
        "11110",
        "10110",
        "10111",
        "10101",
        "01111",
        "00111",
        "11100",
        "10000",
        "11001",
        "00010",
        "01010"
    };
    // clang-format on

    assert(PowerConsumption(values) == 198);
}

} // namespace

int main() {
    Test1();

    std::vector<std::string> values;
    std::string tmp;
    while (std::cin >> tmp) {
        values.push_back(tmp);
    }

    auto part1 = PowerConsumption(values);
    std::cout << "Part1: " << part1 << std::endl;
    return 0;
}