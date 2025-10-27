#include <cassert>
#include <vector>
#include <iostream>
#include <print>

auto get_fuel(int n) -> int {
    return (n / 3) - 2;
}

auto problem1(const std::vector<int> &nums) -> int {
    int ret = 0;
    for (int num : nums) {
        ret += get_fuel(num);
    }

    return ret;
}

auto get_fuel2(int n) -> int {
    int ret = 0;
    while (n > 0) {
        int m = (n / 3) - 2;
        if (m <= 0) {
            break;
        }

        ret += m;
        n = m;
    }

    return ret;
}

auto problem2(const std::vector<int> &nums) -> int {
    int ret = 0;
    for (int num : nums) {
        ret += get_fuel2(num);
    }

    return ret;
}

void test() {
    assert(get_fuel(12) == 2);
    assert(get_fuel(14) == 2);
    assert(get_fuel(1969) == 654);
    assert(get_fuel(100756) == 33583);

    assert(get_fuel2(14) == 2);
    assert(get_fuel2(1969) == 966);
    assert(get_fuel2(100756) == 50346);
}

auto main() -> int {
    test();

    std::vector<int> input;
    int n;
    while (std::cin >> n) {
        input.push_back(n);
    }

    int ret1 = problem1(input);
    std::println("problem1={}", ret1);

    int ret2 = problem2(input);
    std::println("problem2={}", ret2);
    return 0;
}
