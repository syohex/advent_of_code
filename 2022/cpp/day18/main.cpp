#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

struct Position {
    int x;
    int y;
    int z;
};

template <typename T>
std::vector<Position> ParseInput(T &it) {
    std::vector<Position> ret;
    std::string tmp;

    while (std::getline(it, tmp)) {
        std::stringstream ss(tmp);

        std::string tmp2;
        std::vector<int> nums;
        while (std::getline(ss, tmp2, ',')) {
            nums.push_back(std::stoi(tmp2));
        }

        assert(nums.size() == 3);

        ret.push_back({nums[0], nums[1], nums[2]});
    }

    return ret;
}

bool IsAdjacent(const Position &a, const Position &b) {
    return (std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z)) == 1;
}

int Problem1(const std::vector<Position> &positions) {
    int len = positions.size();
    std::vector<int> surface_count(len, 6);

    for (int i = 0; i < len - 1; ++i) {
        for (int j = i + 1; j < len; ++j) {
            if (IsAdjacent(positions[i], positions[j])) {
                --surface_count[i];
                --surface_count[j];
            }
        }
    }

    int sum = 0;
    for (int n : surface_count) {
        sum += n;
    }

    return sum;
}

void Test() {
    std::string input(R"(2,2,2
1,2,2
3,2,2
2,1,2
2,3,2
2,2,1
2,2,3
2,2,4
2,2,6
1,2,5
3,2,5
2,1,5
2,3,5)");
    std::stringstream ss(input);

    auto data = ParseInput(ss);

    int ret0 = Problem1({{1, 1, 1}, {2, 1, 1}});
    assert(ret0 == 10);

    int ret1 = Problem1(data);
    assert(ret1 == 64);
}

int main() {
    Test();

    auto data = ParseInput(std::cin);
    int ret1 = Problem1(data);

    std::cout << "Problem1: " << ret1 << std::endl;
    return 0;
}
