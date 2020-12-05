#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>

namespace {

int ToSeatID(const std::string &str) {
    int row = 0;
    for (size_t i = 0; i < 7; ++i) {
        if (str[i] == 'F') {
            row = 2 * row;
        } else {
            row = 2 * row + 1;
        }
    }

    int column = 0;
    for (size_t i = 7; i < str.size(); ++i) {
        if (str[i] == 'L') {
            column = 2 * column;
        } else {
            column = 2 * column + 1;
        }
    }

    return row * 8 + column;
}

int Solve01(const std::vector<std::string> &strs) {
    int max = 0;
    for (const auto &str : strs) {
        max = std::max(max, ToSeatID(str));
    }
    return max;
}

int Solve02(const std::vector<std::string> &strs) {
    std::set<int> s;
    for (const auto &str : strs) {
        s.insert(ToSeatID(str));
    }

    for (int id : s) {
        if (s.find(id + 1) == s.end() && s.find(id + 2) != s.end()) {
            return id + 1;
        }
    }

    return -1;
}

void test01() {
    assert(ToSeatID("FBFBBFFRLR") == 357);

    std::vector<std::string> v{
        "BFFFBBFRRR",
        "FFFBBBFRRR",
        "BBFFBBFRLL",
    };
    assert(ToSeatID(v[0]) == 567);
    assert(ToSeatID(v[1]) == 119);
    assert(ToSeatID(v[2]) == 820);

    assert(Solve01(v) == 820);
}

} // namespace

int main() {
    test01();

    std::vector<std::string> input;
    std::string line;
    while (std::getline(std::cin, line)) {
        input.push_back(line);
    }

    std::cout << "Part01: " << Solve01(input) << std::endl;
    std::cout << "Part02: " << Solve02(input) << std::endl;

    return 0;
}
