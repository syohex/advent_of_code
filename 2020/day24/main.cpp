#include <cassert>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

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
    for (const auto &dir : instructions) {
        int x = 0;
        int y = 0;
        for (auto d : dir) {
            switch (d) {
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
}

} // namespace

int main() {
    Test01();

    auto instructions = ParseInput(std::cin);
    std::cout << "Part01: " << Solve01(instructions) << std::endl;
    return 0;
}
