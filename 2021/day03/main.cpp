#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <set>

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

std::int64_t BinaryToNumber(const std::string &s) {
    std::int64_t ret = 0;
    for (char c : s) {
        ret = ret * 2 + (c - '0');
    }

    return ret;
}

std::int64_t LifeSupportRating(const std::vector<std::string> &values) {
    int len = values[0].size();
    std::set<std::string> oxygen_cand(values.begin(), values.end());
    std::set<std::string> co2_cand(values.begin(), values.end());

    const auto is_more_ones = [](const std::set<std::string> &s, size_t index) -> bool {
        size_t ones = 0, zeros = 0;
        for (const auto &it : s) {
            if (it[index] == '1') {
                ++ones;
            } else {
                ++zeros;
            }
        }

        return ones >= zeros;
    };

    for (size_t i = 0; i < len; ++i) {
        if (oxygen_cand.size() >= 2) {
            if (is_more_ones(oxygen_cand, i)) {
                for (auto it = oxygen_cand.begin(); it != oxygen_cand.end();) {
                    const auto &value = *it;
                    if (value[i] == '0') {
                        it = oxygen_cand.erase(it);
                    } else {
                        ++it;
                    }
                }
            } else {
                for (auto it = oxygen_cand.begin(); it != oxygen_cand.end();) {
                    const auto &value = *it;
                    if (value[i] == '1') {
                        it = oxygen_cand.erase(it);
                    } else {
                        ++it;
                    }
                }
            }
        }
        if (co2_cand.size() >= 2) {
            if (is_more_ones(co2_cand, i)) {
                for (auto it = co2_cand.begin(); it != co2_cand.end();) {
                    const auto &value = *it;
                    if (value[i] == '1') {
                        it = co2_cand.erase(it);
                    } else {
                        ++it;
                    }
                }
            } else {
                for (auto it = co2_cand.begin(); it != co2_cand.end();) {
                    const auto &value = *it;
                    if (value[i] == '0') {
                        it = co2_cand.erase(it);
                    } else {
                        ++it;
                    }
                }
            }
        }
    }

    auto oxygen = *oxygen_cand.begin();
    auto co2 = *co2_cand.begin();
    auto ret = BinaryToNumber(oxygen) * BinaryToNumber(co2);
    return ret;
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

void Test2() {
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

    assert(BinaryToNumber("10111") == 23);
    assert(BinaryToNumber("01010") == 10);
    assert(LifeSupportRating(values) == 230);
}

} // namespace

int main() {
    Test1();
    Test2();

    std::vector<std::string> values;
    std::string tmp;
    while (std::cin >> tmp) {
        values.push_back(tmp);
    }

    auto part1 = PowerConsumption(values);
    auto part2 = LifeSupportRating(values);
    std::cout << "Part1: " << part1 << std::endl;
    std::cout << "Part2: " << part2 << std::endl;
    return 0;
}