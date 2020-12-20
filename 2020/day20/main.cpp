#include <cassert>
#include <cstdint>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <functional>
#include <cmath>

namespace {

enum class Type {
    kNormal,
    kVerticalFlipped,
    kHorizonalFlipped,
    kRotated,
};

struct Image {
    std::int64_t id = -1;
    std::vector<std::string> tiles;

    std::vector<std::string> Flip(Type type) const {
        assert(type == Type::kVerticalFlipped || type == Type::kHorizonalFlipped);

        std::vector<std::string> ret;
        if (type == Type::kVerticalFlipped) {
            for (const auto &tile : tiles) {
                std::string line = tile;
                std::reverse(line.begin(), line.end());
                ret.push_back(line);
            }
        } else {
            for (auto it = tiles.rbegin(); it != tiles.rend(); ++it) {
                std::string line = *it;
                ret.push_back(line);
            }
        }

        return ret;
    }

    std::vector<std::string> Rotate(int degree) const {
        assert(degree % 90 == 0 && degree < 360);

        std::vector<std::string> ret;
        int count = degree / 90;
        switch (count) {
        case 1: {
            ret = tiles;
            for (size_t i = 0; i < tiles.size(); ++i) {
                size_t row_limit = tiles.size();
                for (size_t j = 0; j < row_limit; ++j) {
                    ret[i][j] = tiles[row_limit - 1 - j][i];
                }
            }
            break;
        }
        case 2: {
            for (auto it = tiles.rbegin(); it != tiles.rend(); ++it) {
                std::string line = *it;
                std::reverse(line.begin(), line.end());
                ret.push_back(line);
            }
            break;
        }
        case 3: {
            ret = tiles;
            for (size_t i = 0; i < tiles.size(); ++i) {
                size_t col_limit = tiles[i].size();
                for (size_t j = 0; j < col_limit; ++j) {
                    ret[i][j] = tiles[j][col_limit - 1 - i];
                }
            }
            break;
        }
        }

        return ret;
    }
};

template <typename T>
std::vector<Image> ParseInput(T &input_stream) {
    std::vector<Image> ret;
    std::string line;

    Image image;
    while (std::getline(input_stream, line)) {
        if (line.find("Tile ") == 0) {
            int id;
            sscanf(line.c_str(), "Tile %d:", &id);
            image.id = id;
        } else if (line.empty()) {
            assert(image.id != -1 && !image.tiles.empty());
            ret.push_back(image);
            continue;
        } else {
            image.tiles.push_back(line);
        }
    }

    ret.push_back(image);
    return ret;
}

std::int64_t Solve01(const std::vector<Image> &images) {
    return 0;
}

void Test01() {
    std::string input(R"(Tile 2311:
..##.#..#.
##..#.....
#...##..#.
####.#...#
##.##.###.
##...#.###
.#.#.#..##
..#....#..
###...#.#.
..###..###

Tile 1951:
#.##...##.
#.####...#
.....#..##
#...######
.##.#....#
.###.#####
###.##.##.
.###....#.
..#.#..#.#
#...##.#..

Tile 1171:
####...##.
#..##.#..#
##.#..#.#.
.###.####.
..###.####
.##....##.
.#...####.
#.##.####.
####..#...
.....##...

Tile 1427:
###.##.#..
.#..#.##..
.#.##.#..#
#.#.#.##.#
....#...##
...##..##.
...#.#####
.#.####.#.
..#..###.#
..##.#..#.

Tile 1489:
##.#.#....
..##...#..
.##..##...
..#...#...
#####...#.
#..#.#.#.#
...#.#.#..
##.#...##.
..##.##.##
###.##.#..

Tile 2473:
#....####.
#..#.##...
#.##..#...
######.#.#
.#...#.#.#
.#########
.###.#..#.
########.#
##...##.#.
..###.#.#.

Tile 2971:
..#.#....#
#...###...
#.#.###...
##.##..#..
.#####..##
.#..####.#
#..#.#..#.
..####.###
..#.#.###.
...#.#.#.#

Tile 2729:
...#.#.#.#
####.#....
..#.#.....
....#..#.#
.##..##.#.
.#.####...
####.#.#..
##.####...
##..#.##..
#.##...##.

Tile 3079:
#.#.#####.
.#..######
..#.......
######....
####.#..#.
.#...#.##.
#.#####.##
..#.###...
..#.......
..#.###...)");

    std::istringstream ss(input);
    auto images = ParseInput(ss);
    assert(images.size() == 9);
    assert(images[0].id == 2311);
    assert(images[1].id == 1951);
    assert(images[2].id == 1171);
    assert(images[3].id == 1427);
    assert(images[4].id == 1489);
    assert(images[5].id == 2473);
    assert(images[6].id == 2971);
    assert(images[7].id == 2729);
    assert(images[8].id == 3079);

    {
        // clang-format off
        std::vector<std::string> data {
            "#..",
            "##.",
            "..#",
        };
        std::vector<std::string> vertical_flipped {
            "..#",
            ".##",
            "#..",
        };
        std::vector<std::string> horizontal_flipped {
            "..#",
            "##.",
            "#..",
        };
        std::vector<std::string> rotated90 {
            ".##",
            ".#.",
            "#..",
        };
        std::vector<std::string> rotated180 {
            "#..",
            ".##",
            "..#",
        };
        std::vector<std::string> rotated270 {
            "..#",
            ".#.",
            "##.",
        };
        // clang-format on

        Image image{1, data};
        assert(image.Flip(Type::kVerticalFlipped) == vertical_flipped);
        assert(image.Flip(Type::kHorizonalFlipped) == horizontal_flipped);

        assert(image.Rotate(90) == rotated90);
        assert(image.Rotate(180) == rotated180);
        assert(image.Rotate(270) == rotated270);
    }
}

} // namespace

int main() {
    Test01();
    return 0;
}
