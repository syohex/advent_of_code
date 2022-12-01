#include <cassert>
#include <iostream>
#include <istream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdio>
#include <algorithm>

template <typename T>
std::vector<std::vector<int>> parse_input(T &is) {
    std::vector<std::vector<int>> ret;
    std::vector<int> nums;
    std::string tmp;
    while (std::getline(is, tmp)) {
        if (tmp.empty()) {
            ret.push_back(nums);
            nums.clear();
        } else {
            nums.push_back(std::stoi(tmp));
        }
    }

    if (!nums.empty()) {
        ret.push_back(nums);
    }

    return ret;
}

int problem1(const std::vector<std::vector<int>> &data) {
    int ret = 0;
    for (const auto &v : data) {
        int sum = 0;
        for (int num : v) {
            sum += num;
        }
        ret = std::max(ret, sum);
    }

    return ret;
}

int problem2(const std::vector<std::vector<int>> &data) {
    std::vector<int> sums;
    for (const auto &v : data) {
        int sum = 0;
        for (int num : v) {
            sum += num;
        }
        sums.push_back(sum);
    }

    std::sort(sums.begin(), sums.end(), std::greater<int>{});
    return sums[0] + sums[1] + sums[2];
}

void test() {
    std::string input_str = R"(1000
2000
3000

4000

5000
6000

7000
8000
9000

10000
)";

    std::vector<std::string> input;
    std::stringstream ss(input_str);
    const auto data = parse_input(ss);
    int ret1 = problem1(data);
    assert(ret1 == 24000);

    int ret2 = problem2(data);
    assert(ret2 == 45000);
}

int main() {
    test();

    auto data = parse_input(std::cin);

    int ret1 = problem1(data);
    int ret2 = problem2(data);

    std::cout << "Problem1: " << ret1 << std::endl;
    std::cout << "Problem2: " << ret2 << std::endl;

    return 0;
}
