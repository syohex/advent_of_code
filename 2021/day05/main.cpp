#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <sstream>
#include <string>
#include <cmath>

namespace {

struct LineData {
    int x1;
    int y1;
    int x2;
    int y2;

    bool IsHorizontal() const {
        return y1 == y2;
    }

    bool IsVertical() const {
        return x1 == x2;
    }

    bool IsDiagonal() const {
        return std::abs(x1 - x2) == std::abs(y1 - y2);
    }
};

std::vector<LineData> ParseInput(std::istream &ss) {
    std::vector<LineData> ret;
    std::string tmp;
    while (std::getline(ss, tmp)) {
        if (tmp.empty()) {
            continue;
        }

        int x1, y1, x2, y2;
        sscanf(tmp.c_str(), "%d,%d -> %d,%d", &x1, &y1, &x2, &y2);
        ret.push_back({x1, y1, x2, y2});
    }

    return ret;
}

int Part1(const std::vector<LineData> &data) {
    int max_x = -1;
    int max_y = -1;

    for (const auto &d : data) {
        if (d.IsHorizontal() || d.IsVertical()) {
            max_x = std::max({max_x, d.x1, d.x2});
            max_y = std::max({max_y, d.y1, d.y2});
        }
    }

    std::vector<std::vector<int>> diagram(max_y + 1, std::vector<int>(max_x + 1, 0));

    for (const auto &d : data) {
        if (d.IsHorizontal()) {
            int start = std::min(d.x1, d.x2);
            int end = std::max(d.x1, d.x2);
            for (int i = start; i <= end; ++i) {
                diagram[d.y1][i] += 1;
            }
        } else if (d.IsVertical()) {
            int start = std::min(d.y1, d.y2);
            int end = std::max(d.y1, d.y2);
            for (int i = start; i <= end; ++i) {
                diagram[i][d.x1] += 1;
            }
        }
    }

    int ret = 0;
    for (int i = 0; i <= max_y; ++i) {
        for (int j = 0; j <= max_x; ++j) {
            if (diagram[i][j] >= 2) {
                ++ret;
            }
        }
    }

    return ret;
}

int Part2(const std::vector<LineData> &data) {
    int max_x = -1;
    int max_y = -1;

    for (const auto &d : data) {
        max_x = std::max({max_x, d.x1, d.x2});
        max_y = std::max({max_y, d.y1, d.y2});
    }

    std::vector<std::vector<int>> diagram(max_y + 1, std::vector<int>(max_x + 1, 0));

    for (const auto &d : data) {
        if (d.IsHorizontal()) {
            int start = std::min(d.x1, d.x2);
            int end = std::max(d.x1, d.x2);
            for (int i = start; i <= end; ++i) {
                diagram[d.y1][i] += 1;
            }
        } else if (d.IsVertical()) {
            int start = std::min(d.y1, d.y2);
            int end = std::max(d.y1, d.y2);
            for (int i = start; i <= end; ++i) {
                diagram[i][d.x1] += 1;
            }
        } else if (d.IsDiagonal()) {
            int diff = std::abs(d.x1 - d.x2);
            if (d.x1 <= d.x2) {
                if (d.y1 <= d.y2) {
                    for (int i = 0; i <= diff; ++i) {
                        diagram[d.y1 + i][d.x1 + i] += 1;
                    }
                } else {
                    for (int i = 0; i <= diff; ++i) {
                        diagram[d.y1 - i][d.x1 + i] += 1;
                    }
                }
            } else {
                if (d.y2 <= d.y1) {
                    for (int i = 0; i <= diff; ++i) {
                        diagram[d.y2 + i][d.x2 + i] += 1;
                    }
                } else {
                    for (int i = 0; i <= diff; ++i) {
                        diagram[d.y2 - i][d.x2 + i] += 1;
                    }
                }
            }
        }
    }

    int ret = 0;
    for (int i = 0; i <= max_y; ++i) {
        for (int j = 0; j <= max_x; ++j) {
            if (diagram[i][j] >= 2) {
                ++ret;
            }
        }
    }

    return ret;
}

void Test1() {
    std::string input(R"(0,9 -> 5,9
8,0 -> 0,8
9,4 -> 3,4
2,2 -> 2,1
7,0 -> 7,4
6,4 -> 2,0
0,9 -> 2,9
3,4 -> 1,4
0,0 -> 8,8
5,5 -> 8,2)");

    std::istringstream ss(input);
    auto data = ParseInput(ss);
    assert(Part1(data) == 5);
    assert(Part2(data) == 12);
}

} // namespace

int main() {
    Test1();

    auto line_data = ParseInput(std::cin);
    auto part1 = Part1(line_data);
    auto part2 = Part2(line_data);

    std::cout << "Part1: " << part1 << std::endl;
    std::cout << "Part2: " << part2 << std::endl;
    return 0;
}