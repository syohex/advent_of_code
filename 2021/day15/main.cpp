#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdint>
#include <algorithm>
#include <limits>
#include <set>
#include <list>

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

    std::list<std::vector<int>> points;
    points.push_back({0, 0});

    std::vector<std::vector<int>> steps{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    while (!points.empty()) {
        auto point = points.front();
        points.pop_front();

        int base = dp[point[0]][point[1]];
        for (const auto &step : steps) {
            int row = point[0] + step[0];
            int col = point[1] + step[1];
            if (row >= 0 && row < rows && col >= 0 && col < cols) {
                int value = base + data[row][col];
                if (value < dp[row][col]) {
                    dp[row][col] = value;
                    if (step[0] > 0 || step[1] > 0) {
                        points.push_front({row, col});
                    } else {
                        points.push_back({row, col});
                    }
                }
            }
        }
    }

    return dp[rows - 1][cols - 1];
}

std::int64_t Part2(const std::vector<std::vector<int>> &input) {
    int base_rows = input.size();
    int base_cols = input[0].size();
    std::vector<std::vector<int>> data(base_rows * 5, std::vector<int>(base_cols * 5, 0));
    int rows = data.size();
    int cols = data[0].size();

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            int base_row = base_rows * i;
            for (int m = 0; m < base_rows; ++m) {
                int base_col = base_cols * j;
                for (int n = 0; n < base_cols; ++n) {
                    int row = base_row + m;
                    int col = base_col + n;

                    if (i == 0 && j == 0) {
                        data[row][col] = input[m][n];
                    } else {
                        if (i == 0) {
                            int value = data[row][col - base_cols] + 1;
                            data[row][col] = value == 10 ? 1 : value;
                        } else {
                            int value = data[row - base_rows][col] + 1;
                            data[row][col] = value == 10 ? 1 : value;
                        }
                    }
                }
            }
        }
    }

    std::vector<std::vector<int>> steps{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    std::vector<std::vector<std::int64_t>> dp(rows, std::vector<std::int64_t>(cols, std::numeric_limits<std::int64_t>::max()));
    dp[0][0] = 0;

    std::list<std::vector<int>> points;
    points.push_back({0, 0});

    while (!points.empty()) {
        auto point = points.front();
        points.pop_front();

        int base = dp[point[0]][point[1]];
        for (const auto &step : steps) {
            int row = point[0] + step[0];
            int col = point[1] + step[1];
            if (row >= 0 && row < rows && col >= 0 && col < cols) {
                int value = base + data[row][col];
                if (value < dp[row][col]) {
                    dp[row][col] = value;
                    if (step[0] > 0 || step[1] > 0) {
                        points.push_front({row, col});
                    } else {
                        points.push_back({row, col});
                    }
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
    auto part2 = Part2(data);
    assert(part1 == 40);
    assert(part2 == 315);
}

} // namespace

int main() {
    Test();

    auto data = ParseInput(std::cin);
    auto part1 = Part1(data);
    auto part2 = Part2(data);

    std::cout << "Part1: " << part1 << std::endl;
    std::cout << "Part2: " << part2 << std::endl;
    return 0;
}
