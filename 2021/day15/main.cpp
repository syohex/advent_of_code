#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdint>
#include <algorithm>
#include <limits>

namespace {

std::vector<std::vector<int>> ParseInput(std::istream &ss) {
    std::vector<std::vector<int>> ret;
    std::string tmp;

    while (std::getline(ss, tmp)) {
        if (tmp.empty()) {
            continue;
        }

        std::vector<int> v;
        for (char c : tmp) {
            v.push_back(c - '0');
        }

        ret.push_back(v);
    }
    return ret;
}

std::int64_t Part1(const std::vector<std::vector<int>> &data) {
    int rows = data.size();
    int cols = data[0].size();

    std::vector<std::vector<std::int64_t>> dp(rows, std::vector<std::int64_t>(cols, std::numeric_limits<std::int64_t>::max()));
    dp[0][0] = 0;

    std::vector<std::vector<int>> stack;
    stack.push_back({0, 0});

    std::vector<std::vector<int>> steps{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    while (!stack.empty()) {
        auto point = stack.back();
        stack.pop_back();

        int base = dp[point[0]][point[1]];
        for (const auto &step : steps) {
            int row = point[0] + step[0];
            int col = point[1] + step[1];
            if (row >= 0 && row < rows && col >= 0 && col < cols) {
                int value = base + data[row][col];
                if (value < dp[row][col]) {
                    dp[row][col] = value;
                    stack.push_back({row, col});
                }
            }
        }
    }

    return dp[rows - 1][cols - 1];
}

void Test() {
    std::string input(R"(1163751742
1381373672
2136511328
3694931569
7463417111
1319128137
1359912421
3125421639
1293138521
2311944581)");

    std::stringstream ss(input);
    auto data = ParseInput(ss);
    auto part1 = Part1(data);
    assert(part1 == 40);
}

} // namespace

int main() {
    Test();

    auto data = ParseInput(std::cin);
    auto part1 = Part1(data);

    std::cout << "Part1: " << part1 << std::endl;
    return 0;
}
