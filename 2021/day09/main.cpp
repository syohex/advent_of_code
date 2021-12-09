#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <functional>
#include <queue>

namespace {

std::vector<std::vector<int>> ParseInput(const std::vector<std::string> &data) {
    std::vector<std::vector<int>> ret;
    for (const auto &d : data) {
        std::vector<int> v;
        for (char c : d) {
            v.push_back(c - '0');
        }

        ret.push_back(v);
    }
    return ret;
}

int Part1(const std::vector<std::vector<int>> &data) {
    int rows = data.size();
    int cols = data[0].size();

    int ret = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (i >= 1 && data[i - 1][j] <= data[i][j]) {
                continue;
            }
            if (i < rows - 1 && data[i + 1][j] <= data[i][j]) {
                continue;
            }
            if (j >= 1 && data[i][j - 1] <= data[i][j]) {
                continue;
            }
            if (j < cols - 1 && data[i][j + 1] <= data[i][j]) {
                continue;
            }

            ret += data[i][j] + 1;
        }
    }

    return ret;
}

std::int64_t Part2(const std::vector<std::vector<int>> &data) {
    int rows = data.size();
    int cols = data[0].size();

    std::function<int(int r, int c, std::vector<std::vector<bool>> &checked)> f;
    f = [&](int r, int c, std::vector<std::vector<bool>> &checked) -> int {
        if (checked[r][c] || data[r][c] == 9) {
            return 0;
        }

        checked[r][c] = true;
        int ret = 1;
        if (r >= 1 && data[r - 1][c] > data[r][c]) {
            ret += f(r - 1, c, checked);
        }
        if (r < rows - 1 && data[r + 1][c] > data[r][c]) {
            ret += f(r + 1, c, checked);
        }
        if (c >= 1 && data[r][c - 1] > data[r][c]) {
            ret += f(r, c - 1, checked);
        }
        if (c < cols - 1 && data[r][c + 1] > data[r][c]) {
            ret += f(r, c + 1, checked);
        }

        return ret;
    };

    std::priority_queue<int> q;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (i >= 1 && data[i - 1][j] <= data[i][j]) {
                continue;
            }
            if (i < rows - 1 && data[i + 1][j] <= data[i][j]) {
                continue;
            }
            if (j >= 1 && data[i][j - 1] <= data[i][j]) {
                continue;
            }
            if (j < cols - 1 && data[i][j + 1] <= data[i][j]) {
                continue;
            }

            std::vector<std::vector<bool>> checked(rows, std::vector<bool>(cols, false));
            int basin_size = f(i, j, checked);
            q.push(basin_size);
        }
    }

    std::int64_t ret = 1;
    for (int i = 0; i < 3; ++i) {
        auto n = q.top();
        q.pop();
        ret *= n;
    }

    return ret;
}

void Test() {
    std::vector<std::string> input{
        "2199943210", "3987894921", "9856789892", "8767896789", "9899965678",
    };

    auto data = ParseInput(input);
    auto part1 = Part1(data);
    auto part2 = Part2(data);

    assert(part1 == 15);
    assert(part2 == 1134);
}

} // namespace

int main() {
    Test();

    std::vector<std::string> input;
    std::string tmp;
    while (std::cin >> tmp) {
        if (tmp.empty()) {
            continue;
        }

        input.push_back(tmp);
    }

    auto data = ParseInput(input);
    auto part1 = Part1(data);
    auto part2 = Part2(data);

    std::cout << "Part1: " << part1 << std::endl;
    std::cout << "Part2: " << part2 << std::endl;
    return 0;
}
