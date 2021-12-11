#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <cstdio>

namespace {

std::vector<std::vector<int>> ParseInput(const std::vector<std::string> &input) {
    std::vector<std::vector<int>> ret;
    for (const auto &s : input) {
        std::vector<int> v;
        for (char c : s) {
            v.push_back(c - '0');
        }
        ret.push_back(v);
    }
    return ret;
}

int Part1(const std::vector<std::vector<int>> &input, int steps) {
    auto data = input;
    int rows = input.size();
    int cols = input[0].size();

    std::vector<std::vector<int>> moves{
        {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1},
    };
    std::function<void(int r, int c, std::vector<std::vector<bool>> &flashed)> f;
    f = [&](int r, int c, std::vector<std::vector<bool>> &flashed) {
        flashed[r][c] = true;

        for (const auto &move : moves) {
            int new_r = r + move[0];
            int new_c = c + move[1];
            if (new_r >= 0 && new_r < rows && new_c >= 0 && new_c < cols) {
                ++data[new_r][new_c];
                if (!flashed[new_r][new_c] && data[new_r][new_c] >= 10) {
                    f(new_r, new_c, flashed);
                }
            }
        }
    };

    int ret = 0;
    for (int step = 0; step < steps; ++step) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                ++data[i][j];
            }
        }

        std::vector<std::vector<bool>> flashed(rows, std::vector<bool>(cols, false));
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (data[i][j] < 10) {
                    continue;
                }

                if (!flashed[i][j]) {
                    f(i, j, flashed);
                }
            }
        }

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (data[i][j] >= 10) {
                    ++ret;
                    data[i][j] = 0;
                }
            }
        }
    }

    return ret;
}

int Part2(const std::vector<std::vector<int>> &input) {
    auto data = input;
    int rows = input.size();
    int cols = input[0].size();

    std::vector<std::vector<int>> moves{
        {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1},
    };
    std::function<void(int r, int c, std::vector<std::vector<bool>> &flashed)> f;
    f = [&](int r, int c, std::vector<std::vector<bool>> &flashed) {
        flashed[r][c] = true;

        for (const auto &move : moves) {
            int new_r = r + move[0];
            int new_c = c + move[1];
            if (new_r >= 0 && new_r < rows && new_c >= 0 && new_c < cols) {
                ++data[new_r][new_c];
                if (!flashed[new_r][new_c] && data[new_r][new_c] >= 10) {
                    f(new_r, new_c, flashed);
                }
            }
        }
    };

    int ret = 1;
    int all = rows * cols;
    while (true) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                ++data[i][j];
            }
        }

        std::vector<std::vector<bool>> flashed(rows, std::vector<bool>(cols, false));
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (data[i][j] < 10) {
                    continue;
                }

                if (!flashed[i][j]) {
                    f(i, j, flashed);
                }
            }
        }

        int count = 0;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (data[i][j] >= 10) {
                    data[i][j] = 0;
                    ++count;
                }
            }
        }

        if (count == all) {
            return ret;
        }

        ++ret;
    }

    // never reach here
    return -1;
}

void TestSimple() {
    std::vector<std::string> input{
        "11111", "19991", "19191", "19991", "11111",
    };

    auto data = ParseInput(input);
    assert(Part1(data, 1) == 9);
    assert(Part1(data, 2) == 9);
}

void Test() {
    std::vector<std::string> input{
        "5483143223", "2745854711", "5264556173", "6141336146", "6357385478",
        "4167524645", "2176841721", "6882881134", "4846848554", "5283751526",
    };

    auto data = ParseInput(input);
    assert(Part1(data, 10) == 204);
    assert(Part1(data, 100) == 1656);

    assert(Part2(data) == 195);
}

} // namespace

int main() {
    TestSimple();
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
    auto part1 = Part1(data, 100);
    auto part2 = Part2(data);

    std::cout << "Part1: " << part1 << std::endl;
    std::cout << "Part2: " << part2 << std::endl;

    return 0;
}
