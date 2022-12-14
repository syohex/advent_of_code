#include <cassert>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <cstring>
#include <set>
#include <limits>

struct Pos {
    int row;
    int col;
    bool is_tile;

    bool operator<(const Pos &other) const {
        if (row == other.row) {
            return col < other.col;
        }

        return row < other.row;
    }
};

template <typename T>
std::set<Pos> ParseInput(T &it) {
    std::string line;
    std::set<Pos> ret;

    while (std::getline(it, line)) {
        std::string tmp;
        for (char c : line) {
            if (c != ' ' && c != '-') {
                tmp.push_back(c);
            }
        }

        std::stringstream ss(tmp);
        int prev_col, prev_row;
        int i = 0;
        while (std::getline(ss, tmp, '>')) {
            int col, row;
            sscanf(tmp.c_str(), "%d,%d", &col, &row);

            if (i > 0) {
                if (prev_col == col) {
                    int start_row = std::min(prev_row, row);
                    int end_row = std::max(prev_row, row);
                    for (int i = start_row; i <= end_row; ++i) {
                        ret.insert({i, col, true});
                    }
                } else {
                    int start_col = std::min(prev_col, col);
                    int end_col = std::max(prev_col, col);
                    for (int i = start_col; i <= end_col; ++i) {
                        ret.insert({row, i, true});
                    }
                }
            }

            prev_col = col;
            prev_row = row;
            ++i;
        }
    }

    return ret;
}

enum class NextAction {
    kDown,
    kLeftDown,
    kRightDown,
    kStop,
};

void PrintBoard(const std::set<Pos> &blocks) {
    int min_col = std::numeric_limits<int>::max();
    int max_row = std::numeric_limits<int>::min();
    int max_col = std::numeric_limits<int>::min();

    for (const auto &p : blocks) {
        min_col = std::min(min_col, p.col);
        max_row = std::max(max_row, p.row);
        max_col = std::max(max_col, p.col);
    }

    for (int row = 0; row <= max_row; ++row) {
        for (int col = min_col; col <= max_col; ++col) {
            const auto it = blocks.find({row, col});
            if (it != blocks.end()) {
                printf("%c", it->is_tile ? '#' : 'o');
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
}

NextAction CheckCollision(int row, int col, const std::set<Pos> &blocks) {
    if (blocks.find({row + 1, col}) == blocks.end()) {
        return NextAction::kDown;
    }

    if (blocks.find({row + 1, col - 1}) == blocks.end()) {
        return NextAction::kLeftDown;
    }

    if (blocks.find({row + 1, col + 1}) == blocks.end()) {
        return NextAction::kRightDown;
    }

    return NextAction::kStop;
}

int Problem1(const std::set<Pos> &tiles) {
    std::set<Pos> blocks = tiles;
    int max_depth = -1;
    for (const Pos &p : blocks) {
        max_depth = std::max(max_depth, p.row);
    }

    int row = 0;
    int col = 500;
    int sands = 1;
    while (true) {
        NextAction action = CheckCollision(row, col, blocks);
        // printf("## %d, %d => %d\n", row, col, static_cast<int>(action));
        switch (action) {
        case NextAction::kDown:
            row += 1;
            break;
        case NextAction::kLeftDown:
            row += 1;
            col -= 1;
            break;
        case NextAction::kRightDown:
            row += 1;
            col += 1;
            break;
        case NextAction::kStop:
            assert(blocks.find({row, col, false}) == blocks.end());

            ++sands;
            blocks.insert({row, col, false});

            row = 0;
            col = 500;
            break;
        }

        if (row > max_depth) {
            break;
        }
    }

    return sands - 1;
}

void Test() {
    std::string input(R"(498,4 -> 498,6 -> 496,6
503,4 -> 502,4 -> 502,9 -> 494,9)");
    std::stringstream ss(input);

    auto data = ParseInput(ss);
    int ret1 = Problem1(data);
    assert(ret1 == 24);
}

int main() {
    Test();

    auto data = ParseInput(std::cin);

    int ret1 = Problem1(data);

    std::cout << "Problem1: " << ret1 << std::endl;
    return 0;
}
