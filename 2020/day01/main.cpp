#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>
#include <set>
#include <map>

namespace {

std::int64_t Solve01(const std::vector<std::int64_t> inputs) {
    std::set<std::int64_t> s;
    for (auto input : inputs) {
        s.insert(input);
    }

    for (auto input : inputs) {
        auto diff = 2020 - input;
        if (s.find(diff) != s.end()) {
            return input * diff;
        }
    }

    assert(!"never reach here");
    return 0;
}

std::int64_t Solve02(const std::vector<std::int64_t> inputs) {
    std::map<std::int64_t, std::vector<std::int64_t>> m;
    for (size_t i = 0; i < inputs.size() - 1; ++i) {
        for (size_t j = i + 1; j < inputs.size(); ++j) {
            auto sum = inputs[i] + inputs[j];
            m[sum] = std::vector<std::int64_t>{inputs[i], inputs[j]};
        }
    }

    for (auto input : inputs) {
        auto diff = 2020 - input;
        if (m.find(diff) != m.end()) {
            return input * m[diff][0] * m[diff][1];
        }
    }

    assert(!"never reach here");
    return 0;
}

void Test01() {
    std::vector<std::int64_t> inputs{
        1721, 979, 366, 299, 675, 1456,
    };

    assert(Solve01(inputs) == 514579);
}

void Test02() {
    std::vector<std::int64_t> inputs{
        1721, 979, 366, 299, 675, 1456,
    };

    assert(Solve02(inputs) == 241861950);
}

} // namespace

int main() {
    Test01();
    Test02();

    std::vector<std::int64_t> inputs;
    std::int64_t val;

    while (std::cin >> val) {
        inputs.push_back(val);
    }

    auto answer01 = Solve01(inputs);
    assert(answer01 == 157059);
    std::cout << "Part01: " << answer01 << std::endl;

    auto answer02 = Solve02(inputs);
    assert(answer02 == 165080960);
    std::cout << "Part02: " << answer02 << std::endl;
    return 0;
}
