#include <cassert>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <set>

namespace {

enum class Direction {
    kEast,
    kSouthEast,
    kSouthWest,
    kWest,
    kNorthWest,
    kNorthEast,
};

template <typename T>
std::vector<std::vector<Direction>> ParseInput(T &stream) {
    // clang-format off
    std::map<std::string, Direction> table {
        {"e", Direction::kEast},
        {"se", Direction::kSouthEast},
        {"sw", Direction::kSouthWest},
        {"w", Direction::kWest},
        {"nw", Direction::kNorthWest},
        {"ne", Direction::kNorthEast},
    };
    // clang-format on

    std::vector<std::vector<Direction>> ret;
    std::string line;
    while (std::getline(stream, line)) {
        std::vector<Direction> dirs;
        size_t pos = 0;

        while (pos < line.size()) {
            for (const auto &it : table) {
                const auto &key = it.first;
                if (strncmp(&line[pos], key.c_str(), key.size()) == 0) {
                    dirs.push_back(it.second);
                    pos += key.size();
                    break;
                }
            }
        }

        ret.push_back(dirs);
    }

    return ret;
}

int Solve01(const std::vector<std::vector<Direction>> &instructions) {
    std::map<std::vector<int>, bool> m;
    for (const auto &directions : instructions) {
        int x = 0;
        int y = 0;
        for (auto dir : directions) {
            switch (dir) {
            case Direction::kEast:
                x += 2;
                break;
            case Direction::kSouthEast:
                x += 1;
                y -= 1;
                break;
            case Direction::kSouthWest:
                x -= 1;
                y -= 1;
                break;
            case Direction::kWest:
                x -= 2;
                break;
            case Direction::kNorthWest:
                x -= 1;
                y += 1;
                break;
            case Direction::kNorthEast:
                x += 1;
                y += 1;
                break;
            }
        }

        std::vector<int> key{x, y};
        if (m.find(key) == m.end()) {
            m[key] = true; // black
        } else {
            m[key] = !m[key]; // flip
        }
    }

    int ret = 0;
    for (const auto &it : m) {
        if (it.second) {
            ++ret;
        }
    }

    return ret;
}

const std::vector<std::vector<int>> &Steps() {
    static const std::vector<std::vector<int>> steps{
        {2, 0},   // east
        {1, -1},  // south east
        {-1, -1}, // south west
        {-2, 0},  // west
        {-1, 1},  // north west
        {1, 1},   // north east
    };

    return steps;
}

int MatchRule(const std::map<std::vector<int>, bool> &map, int x, int y) {
    int blacks = 0;

    for (const auto &step : Steps()) {
        std::vector<int> key{x + step[0], y + step[1]};
        if (map.find(key) != map.end() && map.at(key)) {
            ++blacks;
        }
    }

    if (map.at(std::vector<int>{x, y})) {
        return blacks == 0 || blacks > 2;
    } else {
        return blacks == 2;
    }
}

int Solve02(const std::vector<std::vector<Direction>> &instructions, int days) {
    std::map<std::vector<int>, bool> m;

    for (const auto &dirs : instructions) {
        int x = 0;
        int y = 0;
        for (auto dir : dirs) {
            switch (dir) {
            case Direction::kEast:
                x += 2;
                break;
            case Direction::kSouthEast:
                x += 1;
                y -= 1;
                break;
            case Direction::kSouthWest:
                x -= 1;
                y -= 1;
                break;
            case Direction::kWest:
                x -= 2;
                break;
            case Direction::kNorthWest:
                x -= 1;
                y += 1;
                break;
            case Direction::kNorthEast:
                x += 1;
                y += 1;
                break;
            }
        }

        std::vector<int> key{x, y};
        if (m.find(key) == m.end()) {
            m[key] = true; // black
        } else {
            m[key] = !m[key]; // flip
        }
    }

    for (int i = 0; i < days; ++i) {
        auto orig = m;
        for (const auto &it : orig) {
            for (const auto &step : Steps()) {
                std::vector<int> key{it.first[0] + step[0], it.first[1] + step[1]};
                if (orig.find(key) == orig.end()) {
                    m[key] = false;
                }
            }
        }

        orig = m;
        for (const auto &it : orig) {
            if (MatchRule(orig, it.first[0], it.first[1])) {
                m[it.first] = !m[it.first];
            }
        }
    }

    int ret = 0;
    for (const auto &it : m) {
        if (it.second) {
            ++ret;
        }
    }

    return ret;
}

void Test01() {
    std::string input(R"(sesenwnenenewseeswwswswwnenewsewsw
neeenesenwnwwswnenewnwwsewnenwseswesw
seswneswswsenwwnwse
nwnwneseeswswnenewneswwnewseswneseene
swweswneswnenwsewnwneneseenw
eesenwseswswnenwswnwnwsewwnwsene
sewnenenenesenwsewnenwwwse
wenwwweseeeweswwwnwwe
wsweesenenewnwwnwsenewsenwwsesesenwne
neeswseenwwswnwswswnw
nenwswwsewswnenenewsenwsenwnesesenew
enewnwewneswsewnwswenweswnenwsenwsw
sweneswneswneneenwnewenewwneswswnese
swwesenesewenwneswnwwneseswwne
enesenwswwswneneswsenwnewswseenwsese
wnwnesenesenenwwnenwsewesewsesesew
nenewswnwewswnenesenwnesewesw
eneswnwswnwsenenwnwnwwseeswneewsenese
neswnwewnwnwseenwseesewsenwsweewe
wseweeenwnesenwwwswnew)");

    std::istringstream ss(input);
    auto instructions = ParseInput(ss);
    assert(instructions.size() == 20);
    assert(instructions[0].size() == 20);

    assert(Solve01(instructions) == 10);

    assert(Solve02(instructions, 1) == 15);
    assert(Solve02(instructions, 10) == 37);
}

} // namespace

int main() {
    Test01();

    auto instructions = ParseInput(std::cin);
    std::cout << "Part01: " << Solve01(instructions) << std::endl;
    std::cout << "Part02: " << Solve02(instructions, 100) << std::endl;
    return 0;
}
