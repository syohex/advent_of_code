#include <cassert>
#include <iostream>
#include <vector>
#include <map>
#include <deque>

namespace {

struct Info {
    std::int64_t first = -1;
    std::int64_t second = -1;

    void Update(std::int64_t index) {
        if (first == -1) {
            first = index;
        } else if (second == -1) {
            second = index;
        } else {
            first = second;
            second = index;
        }
    }

    std::int64_t Value() const {
        assert(first != -1);

        if (second == -1) {
            return 0;
        }

        return second - first;
    }
};

std::int64_t Solve01(const std::vector<std::int64_t> &inputs, std::int64_t turn) {
    std::map<std::int64_t, Info> m;
    for (std::int64_t i = 0; i < static_cast<std::int64_t>(inputs.size()); ++i) {
        m[inputs[i]].Update(i + 1);
    }

    auto prev = inputs.back();
    auto start = static_cast<std::int64_t>(inputs.size());
    for (std::int64_t i = start; i < turn; ++i) {
        auto value = m[prev].Value();
        m[value].Update(i + 1);
        prev = value;
    }

    return prev;
}

void Test01() {
    std::vector<std::int64_t> inputs{0, 3, 6};

    assert(Solve01(inputs, 4) == 0);
    assert(Solve01(inputs, 5) == 3);
    assert(Solve01(inputs, 6) == 3);
    assert(Solve01(inputs, 7) == 1);
    assert(Solve01(inputs, 8) == 0);
    assert(Solve01(inputs, 9) == 4);
    assert(Solve01(inputs, 10) == 0);

    assert((Solve01(std::vector<std::int64_t>{1, 3, 2}, 2020) == 1));
    assert((Solve01(std::vector<std::int64_t>{2, 1, 3}, 2020) == 10));
    assert((Solve01(std::vector<std::int64_t>{1, 2, 3}, 2020) == 27));
    assert((Solve01(std::vector<std::int64_t>{2, 3, 1}, 2020) == 78));
    assert((Solve01(std::vector<std::int64_t>{3, 2, 1}, 2020) == 438));
    assert((Solve01(std::vector<std::int64_t>{3, 1, 2}, 2020) == 1836));
}

void Test02() {
    
}

} // namespace

int main() {
    Test01();

    std::vector<std::int64_t> inputs{2, 1, 10, 11, 0, 6};
    std::cout << "Part01: " << Solve01(inputs, 2020) << std::endl;
    return 0;
}
