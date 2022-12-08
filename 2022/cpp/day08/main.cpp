#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdint>
#include <algorithm>

template <typename T>
std::vector<std::vector<int>> ParseInput(T &it) {
    std::vector<std::vector<int>> ret;
    std::string tmp;
    while (std::getline(it, tmp)) {
        if (tmp.empty()) {
            break;
        }

        std::vector<int> row;
        for (char c : tmp) {
            row.push_back(c - '0');
        }

        ret.push_back(std::move(row));
    }

    return ret;
}

int Problem1(const std::vector<std::vector<int>> &data) {
    int rows = data.size();
    int cols = data[0].size();
    int ret = 0;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (i == 0 || i == rows - 1 || j == 0 || j == cols - 1) {
                ++ret;
                continue;
            }

            bool up_ok = true;
            for (int k = i - 1; k >= 0; --k) {
                if (data[k][j] >= data[i][j]) {
                    up_ok = false;
                    break;
                }
            }

            if (up_ok) {
                ++ret;
                continue;
            }

            bool right_ok = true;
            for (int k = j + 1; k < cols; ++k) {
                if (data[i][k] >= data[i][j]) {
                    right_ok = false;
                    break;
                }
            }

            if (right_ok) {
                ++ret;
                continue;
            }

            bool down_ok = true;
            for (int k = i + 1; k < rows; ++k) {
                if (data[k][j] >= data[i][j]) {
                    down_ok = false;
                    break;
                }
            }

            if (down_ok) {
                ++ret;
                continue;
            }

            bool left_ok = true;
            for (int k = j - 1; k >= 0; --k) {
                if (data[i][k] >= data[i][j]) {
                    left_ok = false;
                    break;
                }
            }

            if (left_ok) {
                ++ret;
                continue;
            }
        }
    }

    return ret;
}

int Problem2(const std::vector<std::vector<int>> &data) {
    int rows = data.size();
    int cols = data[0].size();
    int64_t ret = 0;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (i == 0 || i == rows - 1 || j == 0 || j == cols - 1) {
                continue;
            }

            int64_t up_trees = 1;
            for (int k = i - 1; k >= 0; --k) {
                if (data[k][j] >= data[i][j]) {
                    break;
                }

                if (k != 0) {
                    ++up_trees;
                }
            }

            int64_t right_trees = 1;
            for (int k = j + 1; k < cols; ++k) {
                if (data[i][k] >= data[i][j]) {
                    break;
                }

                if (k != cols - 1) {
                    ++right_trees;
                }
            }

            int64_t down_trees = 1;
            for (int k = i + 1; k < rows; ++k) {
                if (data[k][j] >= data[i][j]) {
                    break;
                }

                if (k != rows - 1) {
                    ++down_trees;
                }
            }

            int64_t left_trees = 1;
            for (int k = j - 1; k >= 0; --k) {
                if (data[i][k] >= data[i][j]) {
                    break;
                }
                if (k != 0) {
                    ++left_trees;
                }
            }

            int64_t mul = up_trees * right_trees * down_trees * left_trees;
            ret = std::max(ret, mul);
        }
    }

    return ret;
}

void Test() {
    std::string input(R"(30373
25512
65332
33549
35390)");
    std::stringstream ss(input);
    auto data = ParseInput(ss);
    int ret1 = Problem1(data);
    int ret2 = Problem2(data);

    assert(ret1 == 21);
    assert(ret2 == 8);
}

int main() {
    Test();

    auto data = ParseInput(std::cin);
    int ret1 = Problem1(data);
    int ret2 = Problem2(data);

    std::cout << "Problem1: " << ret1 << std::endl;
    std::cout << "Problem2: " << ret2 << std::endl;
    return 0;
}
