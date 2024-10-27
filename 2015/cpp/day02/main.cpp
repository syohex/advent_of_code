#include <cassert>
#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>

namespace {

struct Data {
    int64_t length;
    int64_t width;
    int64_t height;
};

Data Parse(const std::string &s) {
    Data d;
    int64_t tmp = 0;
    size_t i = 0;

    while (s[i] != 'x') {
        tmp = tmp * 10 + s[i] - '0';
        ++i;
    }

    d.length = tmp;
    tmp = 0;
    ++i;

    while (s[i] != 'x') {
        tmp = tmp * 10 + s[i] - '0';
        ++i;
    }
    d.width = tmp;
    tmp = 0;
    ++i;

    while (i < s.size()) {
        tmp = tmp * 10 + s[i] - '0';
        ++i;
    }
    d.height = tmp;

    return d;
}

int64_t SquareFeet(const Data &d) {
    int64_t area1 = d.length * d.width;
    int64_t area2 = d.width * d.height;
    int64_t area3 = d.height * d.length;
    return 2 * area1 + 2 * area2 + 2 * area3 + std::min({area1, area2, area3});
}

int64_t Problem1(const std::vector<Data> &input) {
    int64_t ret = 0;
    for (const auto &d : input) {
        ret += SquareFeet(d);
    }

    return ret;
}

int64_t Problem2(const std::vector<Data> &input) {
    int64_t ret = 0;
    for (const auto &d : input) {
        std::vector<int64_t> v{d.length, d.width, d.height};
        std::sort(v.begin(), v.end());

        ret += 2 * v[0] + 2 * v[1] + d.length * d.width * d.height;
    }

    return ret;
}

void Test() {
    {
        auto d = Parse("2x3x4");
        assert(d.length == 2);
        assert(d.width == 3);
        assert(d.height == 4);
    }
    {
        std::vector<Data> input;
        input.push_back(Parse("2x3x4"));

        auto ret1 = Problem1(input);
        assert(ret1 == 58);

        auto ret2 = Problem2(input);
        assert(ret2 == 34);
    }
    {
        std::vector<Data> input;
        input.push_back(Parse("1x1x10"));

        auto ret1 = Problem1(input);
        assert(ret1 == 43);

        auto ret2 = Problem2(input);
        assert(ret2 == 14);
    }
}

} // namespace

int main() {
    Test();

    std::ifstream file("../../input/day02.txt");
    std::vector<Data> input;

    std::string tmp;
    while (std::getline(file, tmp)) {
        if (tmp.empty()) {
            continue;
        }

        input.push_back(Parse(tmp));
    }

    auto ret1 = Problem1(input);
    auto ret2 = Problem2(input);

    std::cout << "Problem1: " << ret1 << std::endl;
    std::cout << "Problem2: " << ret2 << std::endl;

    return 0;
}
