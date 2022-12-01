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

void test1() {
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
    int ret = problem1(data);
    assert(ret == 24000);
}

int main() {
    test1();

    auto data = parse_input(std::cin);

    int ret = problem1(data);

    std::cout << "Problem1: " << ret << std::endl;

    return 0;
}
