#include <cassert>
#include <string>
#include <iostream>
#include <cstdint>
#include <algorithm>
#include <vector>
#include <map>

namespace {

std::int64_t Solve01(std::vector<std::int64_t> &inputs) {
    std::sort(inputs.begin(), inputs.end());

    inputs.push_back(inputs.back() + 3);

    std::map<std::int64_t, int> m;
    std::int64_t prev = 0;
    for (auto input : inputs) {
        auto diff = input - prev;
        assert(diff >= 1 && diff <= 3);
        ++m[diff];
        prev = input;
    }

    return m[1] * m[3];
}

std::int64_t Solve02(std::vector<std::int64_t> &inputs) {
    inputs.push_back(0);
    std::sort(inputs.begin(), inputs.end());
    inputs.push_back(inputs.back() + 3);

    std::vector<std::int64_t> acc(inputs.size(), 0);
    acc[0] = 1;
    for (size_t i = 0; i < inputs.size(); ++i) {
        for (size_t j = 0; j < i; ++j) {
            if (inputs[i] - inputs[j] <= 3) {
                acc[i] += acc[j];
            }
        }
    }

    return acc.back();
}

void Test01() {
    std::vector<std::int64_t> inputs01{
        16, 10, 15, 5, 1, 11, 7, 19, 6, 12, 4,
    };
    std::vector<std::int64_t> inputs02{
        28, 33, 18, 42, 31, 14, 46, 20, 48, 47, 24, 23, 49, 45, 19, 38, 39, 11, 1, 32, 25, 35, 8, 17, 7, 9, 4, 2, 34, 10, 3,
    };

    assert(Solve01(inputs01) == 35);
    assert(Solve01(inputs02) == 220);

    assert(Solve02(inputs01) == 8);
    assert(Solve02(inputs02) == 19208);
}

} // namespace

int main() {
    Test01();

    std::vector<std::int64_t> inputs;
    std::string line;
    while (std::getline(std::cin, line)) {
        inputs.push_back(std::stoll(line));
    }

    std::cout << "Part01: " << Solve01(inputs) << std::endl;
    std::cout << "Part02: " << Solve02(inputs) << std::endl;
    return 0;
}
