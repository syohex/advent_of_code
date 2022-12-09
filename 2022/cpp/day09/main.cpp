#include <cassert>
#include <vector>
#include <set>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <cmath>

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

struct Step {
    MoveDirection direction;
    int steps;
};

struct Position {
    int row;
    int col;

    bool operator<(const Position &other) const {
        if (row == other.row) {
            return col < other.col;
        }

        return row < other.row;
    }
};

template <typename T>
std::vector<Step> ParseInput(T &it) {
    std::vector<Step> ret;
    std::string tmp;
    while (std::getline(it, tmp)) {
        char c;
        int steps;

        sscanf(tmp.c_str(), "%c %d", &c, &steps);

        MoveDirection dir;
        switch (c) {
        case 'R':
            dir = MoveDirection::kRight;
            break;
        case 'U':
            dir = MoveDirection::kUp;
            break;
        case 'D':
            dir = MoveDirection::kDown;
            break;
        case 'L':
            dir = MoveDirection::kLeft;
            break;
        default:
            assert(!"never reach here");
        }

        ret.push_back({dir, steps});
    }

    return ret;
}

bool isTouching(const Position &a, const Position &b) {
    int row_diff = std::abs(a.row - b.row);
    int col_diff = std::abs(a.col - b.col);
    int diff = row_diff * row_diff + col_diff * col_diff;
    return diff <= 2;
}

MoveDirection HowMove(const Position &a, const Position &b) {
    if (isTouching(a, b)) {
        return MoveDirection::kStay;
    }

    if (a.row == b.row) {
        if (a.col > b.col) {
            return MoveDirection::kRight;
        }

        return MoveDirection::kLeft;
    }

    if (a.col == b.col) {
        if (a.row > b.row) {
            return MoveDirection::kDown;
        }

        return MoveDirection::kUp;
    }

    if (a.col < b.col) {
        if (a.row < b.row) {
            return MoveDirection::kLeftUp;
        }

        return MoveDirection::kLeftDown;
    }

    if (a.col > b.col) {
        if (a.row < b.row) {
            return MoveDirection::kRightUp;
        }

        return MoveDirection::kRightDown;
    }

    assert(!"never reach here");
}

void MoveKnot(MoveDirection direction, Position &p) {
    switch (direction) {
    case MoveDirection::kLeftUp:
        p.row -= 1;
        p.col -= 1;
        break;
    case MoveDirection::kUp:
        p.row -= 1;
        break;
    case MoveDirection::kRightUp:
        p.row -= 1;
        p.col += 1;
        break;
    case MoveDirection::kLeft:
        p.col -= 1;
        break;
    case MoveDirection::kStay:
        break;
    case MoveDirection::kRight:
        p.col += 1;
        break;
    case MoveDirection::kLeftDown:
        p.row += 1;
        p.col -= 1;
        break;
    case MoveDirection::kDown:
        p.row += 1;
        break;
    case MoveDirection::kRightDown:
        p.row += 1;
        p.col += 1;
        break;
    }
}

size_t Problem1(const std::vector<Step> &data) {
    Position head{0, 0};
    Position tail{0, 0};

    std::set<Position> visited;
    visited.insert(tail);

    for (const auto &step : data) {
        for (int i = 0; i < step.steps; ++i) {
            MoveKnot(step.direction, head);

            MoveDirection direction = HowMove(head, tail);
            MoveKnot(direction, tail);

            visited.insert(tail);
        }
    }

    return visited.size();
}

size_t Problem2(const std::vector<Step> &data) {
    std::vector<Position> ropes(10, {0, 0});

    std::set<Position> visited;
    visited.insert(ropes[9]);

    for (const auto &step : data) {
        for (int i = 0; i < step.steps; ++i) {
            MoveKnot(step.direction, ropes[0]);

            for (size_t i = 1; i < 10; ++i) {
                MoveDirection dir = HowMove(ropes[i - 1], ropes[i]);
                MoveKnot(dir, ropes[i]);
            }

            visited.insert(ropes[9]);
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
