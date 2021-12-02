#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

namespace {

enum class Way {
    kForward,
    kDown,
    kUp,
};

struct MoveData {
    Way way;
    int size;
};

std::int64_t Problem1(const std::vector<MoveData> &moves) {
    std::int64_t horizontal_pos = 0;
    std::int64_t depth = 0;

    for (const auto &move : moves) {
        switch (move.way) {
        case Way::kForward:
            horizontal_pos += move.size;
            break;
        case Way::kDown:
            depth += move.size;
            break;
        case Way::kUp:
            depth -= move.size;
            break;
        }
    }

    return horizontal_pos * depth;
}

void Test1() {
    // clang-format off
    std::vector<MoveData> moves {
        {Way::kForward, 5},
        {Way::kDown, 5},
        {Way::kForward, 8},
        {Way::kUp, 3},
        {Way::kDown, 8},
        {Way::kForward, 2},
    };
    // clang-format on

    assert(Problem1(moves) == 150);
}

} // namespace

int main() {
    Test1();

    std::string way_str;
    int size;

    std::vector<MoveData> moves;
    while (std::cin >> way_str) {
        std::cin >> size;

        Way w;
        if (way_str == "forward") {
            w = Way::kForward;
        } else if (way_str == "down") {
            w = Way::kDown;
        } else {
            w = Way::kUp;
        }

        moves.push_back({w, size});
    }

    std::cout << "Problem1: " << Problem1(moves) << std::endl;

    return 0;
}