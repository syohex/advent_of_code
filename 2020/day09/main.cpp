#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>
#include <deque>
#include <set>
#include <string>
#include <algorithm>

namespace {

bool HasAddPair(const std::deque<std::int64_t> &q, std::int64_t val) {
    std::set<int> s;
    for (const auto i : q) {
        s.insert(i);
    }

    for (const auto i : q) {
        int diff = val - i;
        if (diff == i) {
            continue;
        }

        if (s.find(diff) != s.end()) {
            return true;
        }
    }

    return false;
}

std::int64_t Solve01(const std::vector<std::int64_t> &input, std::int64_t preamble) {
    assert(input.size() >= preamble);

    std::deque<std::int64_t> q;
    size_t i = 0;
    for (; i < preamble; ++i) {
        q.push_back(input[i]);
    }

    for (; i < input.size(); ++i) {
        if (!HasAddPair(q, input[i])) {
            return input[i];
        }

        q.pop_front();
        q.push_back(input[i]);
    }

    assert(!"never reach here");
    return -1;
}

std::int64_t AddMinAndMax(const std::vector<std::int64_t> &input, size_t min_index, size_t max_index) {
    std::vector<std::int64_t> v;
    for (size_t i = min_index; i <= max_index; ++i) {
        v.push_back(input[i]);
    }

    std::sort(v.begin(), v.end());
    return v.front() + v.back();
}

std::int64_t Solve02(const std::vector<std::int64_t> &input, std::int64_t val) {
    std::vector<std::int64_t> sums(input.size());
    sums[0] = input[0];
    for (size_t i = 1; i < input.size(); ++i) {
        sums[i] = sums[i - 1] + input[i];
    }

    for (size_t i = 0; i < sums.size(); ++i) {
        for (size_t j = 0; j < i; ++j) {
            if (sums[i] - sums[j] == val) {
                return AddMinAndMax(input, j + 1, i);
            }
        }
    }

    assert(!"never reach here");
    return -1;
}

void Test01() {
    std::vector<std::int64_t> input{35, 20, 15, 25, 47, 40, 62, 55, 65, 95, 102, 117, 150, 182, 127, 219, 299, 277, 309, 576};
    assert(Solve01(input, 5) == 127LL);
}

void Test02() {
    std::vector<std::int64_t> input{35, 20, 15, 25, 47, 40, 62, 55, 65, 95, 102, 117, 150, 182, 127, 219, 299, 277, 309, 576};
    assert(Solve02(input, 127LL) == 62);
}

} // namespace

int main() {
    Test01();
    Test02();

    std::vector<std::int64_t> input;
    std::string line;
    while (std::getline(std::cin, line)) {
        input.push_back(static_cast<std::int64_t>(std::stoll(line)));
    }

    std::int64_t invalid_val = Solve01(input, 25);
    std::cout << "Part01: " << invalid_val << std::endl;
    std::cout << "Part02: " << Solve02(input, invalid_val) << std::endl;
    return 0;
}
