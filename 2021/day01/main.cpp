#include <iostream>
#include <cassert>
#include <vector>

namespace {

int CountIncreased(const std::vector<int> &nums) {
    int ret = 0;
    for (size_t i = 1; i < nums.size(); ++i) {
        if (nums[i] > nums[i - 1]) {
            ++ret;
        }
    }

    return ret;
}

int CountIncreasedThreePair(const std::vector<int> &nums) {
    int ret = 0;
    int prev = nums[0] + nums[1] + nums[2];
    for (size_t i = 3; i < nums.size(); ++i) {
        int cur = nums[i] + nums[i - 1] + nums[i - 2];
        if (cur > prev) {
            ++ret;
        }

        prev = cur;
    }

    return ret;
}

void Test1() {
    std::vector<int> nums{199, 200, 208, 210, 200, 207, 240, 269, 260, 263};
    assert(CountIncreased(nums) == 7);
}

void Test2() {
    std::vector<int> nums{199, 200, 208, 210, 200, 207, 240, 269, 260, 263};
    assert(CountIncreasedThreePair(nums) == 5);
}

} // namespace

int main() {
    Test1();
    Test2();

    std::vector<int> nums;
    int num;
    while (std::cin >> num) {
        nums.push_back(num);
    }

    std::cout << "Part1: " << CountIncreased(nums) << std::endl;
    std::cout << "Part2: " << CountIncreasedThreePair(nums) << std::endl;
    return 0;
}