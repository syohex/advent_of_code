#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

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

void Test() {
    std::string input(R"(30373
25512
65332
33549
35390)");
    std::stringstream ss(input);
    auto data = ParseInput(ss);
    int ret1 = Problem1(data);

    assert(ret1 == 21);
}

int main() {
    Test();

    auto data = ParseInput(std::cin);
    int ret1 = Problem1(data);

    std::cout << "Problem1: " << ret1 << std::endl;
    return 0;
}
