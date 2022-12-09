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

void MoveKnot(MoveDirection direction, int &row, int &col) {
    switch (direction) {
    case MoveDirection::kLeftUp:
        row -= 1;
        col -= 1;
        break;
    case MoveDirection::kUp:
        row -= 1;
        break;
    case MoveDirection::kRightUp:
        row -= 1;
        col += 1;
        break;
    case MoveDirection::kLeft:
        col -= 1;
        break;
    case MoveDirection::kStay:
        break;
    case MoveDirection::kRight:
        col += 1;
        break;
    case MoveDirection::kLeftDown:
        row += 1;
        col -= 1;
        break;
    case MoveDirection::kDown:
        row += 1;
        break;
    case MoveDirection::kRightDown:
        row += 1;
        col += 1;
        break;
    }
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

size_t Problem2(const std::vector<Step> &data) {
    std::vector<std::pair<int, int>> ropes(10, {0, 0});

    std::set<std::pair<int, int>> visited;
    visited.insert({ropes[9].first, ropes[9].second});

    for (const auto &step : data) {
        for (int i = 0; i < step.steps; ++i) {
            switch (step.direction) {
            case Direction::kRight:
                ropes[0].second += 1;
                break;
            case Direction::kDown:
                ropes[0].first += 1;
                break;
            case Direction::kLeft:
                ropes[0].second -= 1;
                break;
            case Direction::kUp:
                ropes[0].first -= 1;
                break;
            }

            for (size_t i = 1; i < 10; ++i) {
                MoveDirection dir = HowMove(ropes[i - 1].first, ropes[i - 1].second, ropes[i].first, ropes[i].second);
                MoveKnot(dir, ropes[i].first, ropes[i].second);
            }

            visited.insert({ropes[9].first, ropes[9].second});
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
    size_t ret2 = Problem2(data);

    assert(ret1 == 13);
    assert(ret2 == 1);

    std::string input2(R"(R 5
U 8
L 8
D 3
R 17
D 10
L 25
U 20)");

    std::stringstream ss2(input2);
    auto data2 = ParseInput(ss2);
    size_t ret2_2 = Problem2(data2);
    assert(ret2_2 == 36);
}

int main() {
    Test();

    auto data = ParseInput(std::cin);
    size_t ret1 = Problem1(data);
    size_t ret2 = Problem2(data);

    std::cout << "Problem1: " << ret1 << std::endl;
    std::cout << "Problem2: " << ret2 << std::endl;
    return 0;
}
