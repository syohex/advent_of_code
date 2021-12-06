#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <map>

namespace {

size_t Part1(const std::vector<int> &data, int days) {
    std::vector<int> values(data);

    for (int i = 0; i < days; ++i) {
        int create_count = 0;
        for (size_t j = 0; j < values.size(); ++j) {
            if (values[j] == 0) {
                values[j] = 6;
                ++create_count;
            } else {
                --values[j];
            }
        }

        for (int j = 0; j < create_count; ++j) {
            values.push_back(8);
        }
    }

    return values.size();
}

size_t Part2(const std::vector<int> &data, int days) {
    std::map<int, std::int64_t> m;
    for (int d : data) {
        ++m[d];
    }

    for (int i = 0; i < days; ++i) {
        std::map<int, std::int64_t> tmp;
        tmp[8] = m[0];
        for (int i = 1; i <= 8; ++i) {
            tmp[i - 1] = m[i];
        }
        tmp[6] += m[0];
        m = tmp;
    }

    std::int64_t ret = 0;
    for (const auto &it : m) {
        ret += it.second;
    }

    return ret;
}

void Test() {
    std::vector<int> data{3, 4, 3, 1, 2};
    assert(Part1(data, 18) == 26);
    assert(Part1(data, 80) == 5934);
    assert(Part2(data, 18) == 26);
    assert(Part2(data, 80) == 5934);
    assert(Part2(data, 256) == 26984457539ULL);
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

    auto part1 = Part1(data, 80);
    auto part2 = Part2(data, 256);

    std::cout << "Part1: " << part1 << std::endl;
    std::cout << "Part2: " << part2 << std::endl;
    return 0;
}