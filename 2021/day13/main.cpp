#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cstdint>

namespace {

struct Instruction {
    char orientation;
    int position;
};

struct Data {
    std::vector<std::vector<char>> grid;
    std::vector<Instruction> instructions;
};

Data ParseInput(std::istream &ss) {
    std::string tmp;
    std::vector<std::vector<int>> coordinates;

    int max_x = -1;
    int max_y = -1;
    while (std::getline(ss, tmp)) {
        if (tmp.empty()) {
            break;
        }

        size_t pos = tmp.find(',');
        int x = std::stoi(tmp.substr(0, pos));
        int y = std::stoi(tmp.substr(pos + 1));
        coordinates.push_back({x, y});

        max_x = std::max(max_x, x);
        max_y = std::max(max_y, y);
    }

    Data ret;
    while (std::getline(ss, tmp)) {
        size_t pos = tmp.find('=');
        char orientation = tmp.substr(pos - 1, 1)[0];
        int position = std::stoi(tmp.substr(pos + 1));
        ret.instructions.push_back({orientation, position});
    }

    std::vector<std::vector<char>> grid(max_y + 1, std::vector<char>(max_x + 1, '.'));
    for (const auto &c : coordinates) {
        grid[c[1]][c[0]] = '#';
    }
    ret.grid = std::move(grid);

    return ret;
}

std::int64_t Part1(const Data &data, int count) {
    auto grid = data.grid;
    int rows = grid.size() - 1;
    int cols = grid[0].size() - 1;
    for (int i = 0; i < count; ++i) {
        const auto &inst = data.instructions[i];
        if (inst.orientation == 'x') {
            for (int row = 0; row <= rows; ++row) {
                for (int col = inst.position + 1, offset = 1; col <= cols; ++col, ++offset) {
                    if (grid[row][col] == '.') {
                        continue;
                    }

                    grid[row][inst.position - offset] = '#';
                }
            }

            cols = inst.position - 1;
        } else {
            for (int row = inst.position + 1, offset = 1; row <= rows; ++row, ++offset) {
                for (int col = 0; col <= cols; ++col) {
                    if (grid[row][col] == '.') {
                        continue;
                    }

                    grid[inst.position - offset][col] = '#';
                }
            }

            rows = inst.position - 1;
        }
    }

    std::int64_t ret = 0;
    for (int i = 0; i <= rows; ++i) {
        for (int j = 0; j <= cols; ++j) {
            if (grid[i][j] == '#') {
                ++ret;
            }
        }
    }

    return ret;
}

void Part2(const Data &data) {
    auto grid = data.grid;
    int rows = grid.size() - 1;
    int cols = grid[0].size() - 1;
    for (const auto &inst : data.instructions) {
        if (inst.orientation == 'x') {
            for (int row = 0; row <= rows; ++row) {
                for (int col = inst.position + 1, offset = 1; col <= cols; ++col, ++offset) {
                    if (grid[row][col] == '.') {
                        continue;
                    }

                    grid[row][inst.position - offset] = '#';
                }
            }

            cols = inst.position - 1;
        } else {
            for (int row = inst.position + 1, offset = 1; row <= rows; ++row, ++offset) {
                for (int col = 0; col <= cols; ++col) {
                    if (grid[row][col] == '.') {
                        continue;
                    }

                    grid[inst.position - offset][col] = '#';
                }
            }

            rows = inst.position - 1;
        }
    }

    for (int i = 0; i <= rows; ++i) {
        for (int j = 0; j <= cols; ++j) {
            if (grid[i][j] == '#') {
                printf("%c", grid[i][j]);
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void Test() {
    std::string input(R"(6,10
0,14
9,10
0,3
10,4
4,11
6,0
6,12
4,1
0,13
10,12
3,4
3,0
8,4
1,10
2,14
8,10
9,0

fold along y=7
fold along x=5)");

    std::stringstream ss(input);
    auto data = ParseInput(ss);
    auto part1 = Part1(data, 1);
    assert(part1 == 17);
    Part2(data);
}

} // namespace

int main() {
    Test();

    auto data = ParseInput(std::cin);
    auto part1 = Part1(data, 1);
    std::cout << "Part1: " << part1 << std::endl;

    Part2(data);
    return 0;
}
