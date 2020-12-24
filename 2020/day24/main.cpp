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
    auto directions = ParseInput(ss);
    assert(directions.size() == 20);
}

} // namespace

int main() {
    Test01();
    return 0;
}
