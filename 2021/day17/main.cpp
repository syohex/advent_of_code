#include <cassert>
#include <iostream>
#include <string>
#include <cstdio>
#include <algorithm>
#include <cmath>
#include <limits>
#include <tuple>

namespace {

struct Data {
    int min_x;
    int max_x;
    int min_y;
    int max_y;

    bool IsTargetArea(int x, int y) const noexcept {
        return x >= min_x && x <= max_x && y >= min_y && y <= max_y;
    }

    bool CanContinue(int y) const noexcept {
        return y >= min_y;
    }
};

Data ParseInput(const std::string &input) {
    Data ret;
#if _WIN32
    sscanf_s(input.c_str(), "target area: x=%d..%d, y=%d..%d", &ret.min_x, &ret.max_x, &ret.min_y, &ret.max_y);
#else
    sscanf(input.c_str(), "target area: x=%d..%d, y=%d..%d", &ret.min_x, &ret.max_x, &ret.min_y, &ret.max_y);
#endif
    return ret;
}

std::tuple<bool, int> FireProbe(const Data &d, int x_velocity, int y_velocity) {
    int x = 0;
    int y = 0;

    int max_y = 0;
    int valid = false;
    while (true) {
        if (d.IsTargetArea(x, y)) {
            valid = true;
        }
        if (!d.CanContinue(y)) {
            break;
        }

        x += x_velocity;
        y += y_velocity;

        max_y = std::max(max_y, y);

        if (x_velocity > 0) {
            x_velocity -= 1;
        } else if (x_velocity < 0) {
            x_velocity += 1;
        }
        y_velocity -= 1;
    }

    return {valid, max_y};
}

int Part1(const Data &d) {
    int end_y = std::max(std::abs(d.min_y), std::abs(d.max_y));
    int ret = 0;

    for (int i = 1; i <= d.max_x; ++i) {
        for (int j = d.min_y; j <= end_y; ++j) {
            auto [ok, max_y] = FireProbe(d, i, j);
            if (ok) {
                ret = std::max(ret, max_y);
            }
        }
    }

    return ret;
}

int Part2(const Data &d) {
    int end_y = std::max(std::abs(d.min_y), std::abs(d.max_y));
    int ret = 0;

    for (int i = 1; i <= d.max_x; ++i) {
        for (int j = d.min_y; j <= end_y; ++j) {
            bool ok;
            std::tie(ok, std::ignore) = FireProbe(d, i, j);
            if (ok) {
                ++ret;
            }
        }
    }

    return ret;
}

void Test() {
    std::string input("target area: x=20..30, y=-10..-5");
    Data data = ParseInput(input);
    auto part1 = Part1(data);
    auto part2 = Part2(data);

    assert(part1 == 45);
    assert(part2 == 112);
}

} // namespace

int main() {
    Test();

    std::string input;
    std::getline(std::cin, input);
    auto data = ParseInput(input);
    auto part1 = Part1(data);
    auto part2 = Part2(data);

    std::cout << "Part1: " << part1 << std::endl;
    std::cout << "Part2: " << part2 << std::endl;
    return 0;
}