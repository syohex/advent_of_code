#include <cassert>
#include <string>
#include <utility>
#include <iostream>
#include <fstream>

namespace {

int WhatFloor(const std::string &s) {
    int floor = 0;
    for (char c : s) {
        switch (c) {
        case '(':
            ++floor;
            break;
        case ')':
            --floor;
            break;
        default:
            std::unreachable();
            break;
        }
    }

    return floor;
}

int FindFirstBasement(const std::string &s) {
    int floor = 0;
    int i = 1;
    for (char c : s) {
        switch (c) {
        case '(':
            ++floor;
            break;
        case ')':
            --floor;
            break;
        default:
            std::unreachable();
            break;
        }

        if (floor == -1) {
            return i;
        }

        ++i;
    }

    return 0;
}

void Test() {
    // Problem 1
    assert(WhatFloor("(())") == 0);
    assert(WhatFloor("()()") == 0);
    assert(WhatFloor("(((") == 3);
    assert(WhatFloor("(()(()(") == 3);
    assert(WhatFloor("))(((((") == 3);
    assert(WhatFloor("())") == -1);
    assert(WhatFloor("))(") == -1);
    assert(WhatFloor(")))") == -3);
    assert(WhatFloor(")())())") == -3);

    // Problem 2
    assert(FindFirstBasement(")") == 1);
    assert(FindFirstBasement("()())") == 5);
}

} // namespace

int main() {
    Test();

    std::ifstream file("../../input/day01.txt");
    std::string input;
    file >> input;

    int ret1 = WhatFloor(input);
    int ret2 = FindFirstBasement(input);

    std::cout << "Problem1 = " << ret1 << std::endl;
    std::cout << "Problem2 = " << ret2 << std::endl;

    return 0;
}
