#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cmath>

namespace {

enum class Command {
    kNorth,
    kSouth,
    kEast,
    kWest,
    kLeft,
    kRight,
    kForward,
};

struct Instruction {
    Command command;
    int value;

    bool operator==(const Instruction &other) const {
        return command == other.command && value == other.value;
    }
};

enum class Direction {
    kNorth,
    kSouth,
    kEast,
    kWest,
};

struct Position {
    int x;
    int y;

    bool operator==(const Position &other) const {
        return x == other.x && y == other.y;
    }
};

Command CharToCommand(char c) {
    switch (c) {
    case 'N':
        return Command::kNorth;
    case 'S':
        return Command::kSouth;
    case 'E':
        return Command::kEast;
    case 'W':
        return Command::kWest;
    case 'L':
        return Command::kLeft;
    case 'R':
        return Command::kRight;
    case 'F':
        return Command::kForward;
    default:
        assert(!"never reach here");
        std::exit(1);
    }
}

std::vector<Instruction> ParseInput(const std::vector<std::string> &inputs) {
    std::vector<Instruction> insns;
    for (const auto &input : inputs) {
        char c;
        int value;
        sscanf(input.c_str(), "%c%d", &c, &value);

        insns.emplace_back(Instruction{CharToCommand(c), value});
    }

    return insns;
}

Direction Turn(int degrees, Direction current) {
    assert(degrees % 90 == 0);

    if (degrees >= 0) {
        degrees %= 360;
    } else {
        int tmp = -1 * degrees;
        tmp %= 360;
        degrees = -1 * tmp;
    }

    Direction dirs[] = {Direction::kNorth, Direction::kEast, Direction::kSouth, Direction::kWest};
    int pos = 0;
    for (int i = 0; i < 4; ++i) {
        if (dirs[i] == current) {
            pos = i;
            break;
        }
    }

    pos += (degrees / 90);
    if (pos >= 4) {
        pos %= 4;
    } else if (pos < 0) {
        pos += 4;
    }

    return dirs[pos];
}

Position TurnWayPoint(int degrees, const Position &way_point) {
    assert(degrees % 90 == 0);

    int count;
    if (degrees >= 0) {
        count = (degrees % 360) / 90;
    } else {
        int tmp = -1 * degrees;
        tmp = (tmp % 360) / 90;
        tmp = -1 * tmp;
        count = tmp + 4;
    }

    switch (count) {
    case 0:
        return way_point;
    case 1:
        return Position{way_point.y, -way_point.x};
    case 2:
        return Position{-way_point.x, -way_point.y};
    case 3:
        return Position{-way_point.y, way_point.x};
    default:
        assert(!"never reach here");
        return way_point;
    }
}

int Solve01(const std::vector<Instruction> &insns) {
    Direction dir = Direction::kEast;
    Position pos{0, 0};
    for (const auto &insn : insns) {
        switch (insn.command) {
        case Command::kNorth:
            pos.y += insn.value;
            break;
        case Command::kSouth:
            pos.y -= insn.value;
            break;
        case Command::kEast:
            pos.x += insn.value;
            break;
        case Command::kWest:
            pos.y += insn.value;
            break;
        case Command::kLeft:
            dir = Turn(-1 * insn.value, dir);
            break;
        case Command::kRight:
            dir = Turn(insn.value, dir);
            break;
        case Command::kForward:
            switch (dir) {
            case Direction::kNorth:
                pos.y += insn.value;
                break;
            case Direction::kEast:
                pos.x += insn.value;
                break;
            case Direction::kSouth:
                pos.y -= insn.value;
                break;
            case Direction::kWest:
                pos.x -= insn.value;
                break;
            }
            break;
        }
    }

    return static_cast<int>(std::abs(pos.x) + std::abs(pos.y));
}

int Solve02(const std::vector<Instruction> &insns) {
    Position way_point{10, 1};
    Position pos{0, 0};

    for (const auto &insn : insns) {
        switch (insn.command) {
        case Command::kNorth:
            way_point.y += insn.value;
            break;
        case Command::kSouth:
            way_point.y -= insn.value;
            break;
        case Command::kEast:
            way_point.x += insn.value;
            break;
        case Command::kWest:
            way_point.x -= insn.value;
            break;
        case Command::kLeft:
            way_point = TurnWayPoint(-1 * insn.value, way_point);
            break;
        case Command::kRight:
            way_point = TurnWayPoint(insn.value, way_point);
            break;
        case Command::kForward:
            pos.x += insn.value * way_point.x;
            pos.y += insn.value * way_point.y;
            break;
        }
    }

    return static_cast<int>(std::abs(pos.x) + std::abs(pos.y));
}

void Test01() {
    // clang-format off
    std::vector<std::string> inputs {
        "F10",
        "N3",
        "F7",
        "R90",
        "F11",
    };
    // clang-format on

    auto insns = ParseInput(inputs);
    assert(insns.size() == 5);
    assert((insns[0] == Instruction{Command::kForward, 10}));
    assert((insns[1] == Instruction{Command::kNorth, 3}));
    assert((insns[2] == Instruction{Command::kForward, 7}));
    assert((insns[3] == Instruction{Command::kRight, 90}));
    assert((insns[4] == Instruction{Command::kForward, 11}));

    assert(Turn(90, Direction::kEast) == Direction::kSouth);
    assert(Turn(-90, Direction::kEast) == Direction::kNorth);
    assert(Turn(360, Direction::kEast) == Direction::kEast);
    assert(Turn(-360, Direction::kEast) == Direction::kEast);

    assert(Solve01(insns) == 25);
}

void Test02() {
    // clang-format off
    std::vector<std::string> inputs {
        "F10",
        "N3",
        "F7",
        "R90",
        "F11",
    };
    // clang-format on

    auto insns = ParseInput(inputs);
    assert((TurnWayPoint(90, Position{4, 10}) == Position{10, -4}));
    assert((TurnWayPoint(180, Position{4, 10}) == Position{-4, -10}));
    assert((TurnWayPoint(270, Position{4, 10}) == Position{-10, 4}));
    assert((TurnWayPoint(360, Position{4, 10}) == Position{4, 10}));

    assert(Solve02(insns) == 286);
}

} // namespace

int main() {
    Test01();
    Test02();

    std::vector<std::string> inputs;
    std::string line;
    while (std::getline(std::cin, line)) {
        inputs.push_back(line);
    }

    auto insns = ParseInput(inputs);

    std::cout << "Part01: " << Solve01(insns) << std::endl;
    std::cout << "Part02: " << Solve02(insns) << std::endl;
    return 0;
}
