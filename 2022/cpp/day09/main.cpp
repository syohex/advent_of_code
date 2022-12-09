#include <cassert>
#include <vector>
#include <set>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <cmath>

enum class Direction {
    kRight,
    kUp,
    kDown,
    kLeft,
};

struct Step {
    Direction direction;
    int steps;
};

template <typename T>
std::vector<Step> ParseInput(T &it) {
    std::vector<Step> ret;
    std::string tmp;
    while (std::getline(it, tmp)) {
        char c;
        int steps;

        sscanf(tmp.c_str(), "%c %d", &c, &steps);

        Direction dir;
        switch (c) {
        case 'R':
            dir = Direction::kRight;
            break;
        case 'U':
            dir = Direction::kUp;
            break;
        case 'D':
            dir = Direction::kDown;
            break;
        case 'L':
            dir = Direction::kLeft;
            break;
        default:
            assert(!"never reach here");
        }

        ret.push_back({dir, steps});
    }

    return ret;
}

enum class MoveDirection {
    kLeftUp,
    kUp,
    kRightUp,
    kLeft,
    kStay,
    kRight,
    kLeftDown,
    kDown,
    kRightDown,
};

bool isTouching(int h_row, int h_col, int t_row, int t_col) {
    int row_diff = std::abs(h_row - t_row);
    int col_diff = std::abs(h_col - t_col);
    int diff = row_diff * row_diff + col_diff * col_diff;
    return diff <= 2;
}

MoveDirection HowMove(int h_row, int h_col, int t_row, int t_col) {
    if (isTouching(h_row, h_col, t_row, t_col)) {
        return MoveDirection::kStay;
    }

    if (h_row == t_row) {
        if (h_col > t_col) {
            return MoveDirection::kRight;
        }

        return MoveDirection::kLeft;
    }

    if (h_col == t_col) {
        if (h_row > t_row) {
            return MoveDirection::kDown;
        }

        return MoveDirection::kUp;
    }

    if (h_col < t_col) {
        if (h_row < t_row) {
            return MoveDirection::kLeftUp;
        }

        return MoveDirection::kLeftDown;
    }

    if (h_col > t_col) {
        if (h_row < t_row) {
            return MoveDirection::kRightUp;
        }

        return MoveDirection::kRightDown;
    }

    assert(!"never reach here");
}

size_t Problem1(const std::vector<Step> &data) {
    int h_row = 0, h_col = 0;
    int t_row = 0, t_col = 0;

    std::set<std::pair<int, int>> visited;
    visited.insert({0, 0});

    for (const auto &step : data) {
        for (int i = 0; i < step.steps; ++i) {
            switch (step.direction) {
            case Direction::kRight:
                h_col += 1;
                break;
            case Direction::kDown:
                h_row += 1;
                break;
            case Direction::kLeft:
                h_col -= 1;
                break;
            case Direction::kUp:
                h_row -= 1;
                break;
            }

            MoveDirection md = HowMove(h_row, h_col, t_row, t_col);
            switch (md) {
            case MoveDirection::kLeftUp:
                t_row -= 1;
                t_col -= 1;
                break;
            case MoveDirection::kUp:
                t_row -= 1;
                break;
            case MoveDirection::kRightUp:
                t_row -= 1;
                t_col += 1;
                break;
            case MoveDirection::kLeft:
                t_col -= 1;
                break;
            case MoveDirection::kStay:
                break;
            case MoveDirection::kRight:
                t_col += 1;
                break;
            case MoveDirection::kLeftDown:
                t_row += 1;
                t_col -= 1;
                break;
            case MoveDirection::kDown:
                t_row += 1;
                break;
            case MoveDirection::kRightDown:
                t_row += 1;
                t_col += 1;
                break;
            }

            visited.insert({t_row, t_col});
        }
    }

    return visited.size();
}

void Test() {
    std::string input(R"(R 4
U 4
L 3
D 1
R 4
D 1
L 5
R 2)");
    std::stringstream ss(input);
    auto data = ParseInput(ss);
    size_t ret1 = Problem1(data);

    assert(ret1 == 13);
}

int main() {
    Test();

    auto data = ParseInput(std::cin);
    size_t ret1 = Problem1(data);

    std::cout << "Problem1: " << ret1 << std::endl;
    return 0;
}
