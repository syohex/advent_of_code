#include <cassert>
#include <cstdint>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

namespace {

struct Image {
    std::int64_t id = -1;
    std::vector<std::string> tiles;

    std::vector<std::string> Flip() const {
        std::vector<std::string> ret;
        for (const auto &tile : tiles) {
            std::string line = tile;
            std::reverse(line.begin(), line.end());
            ret.push_back(line);
        }

        return ret;
    }

    std::vector<std::string> Rotate() const {
        std::vector<std::string> ret;
        for (auto it = tiles.rbegin(); it != tiles.rend(); ++it) {
            std::string line = *it;
            std::reverse(line.begin(), line.end());
            ret.push_back(line);
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
            "#.#",
        };
        std::vector<std::string> flipped {
            "..#",
            ".##",
            "#..",
            "#.#",
        };
        std::vector<std::string> rotated {
            "#.#",
            "#..",
            ".##",
            "..#",
        };
        // clang-format on
        Image image{1, data};
        assert(image.Flip() == flipped);
        assert(image.Rotate() == rotated);
    }
}

} // namespace

int main() {
    Test01();
    return 0;
}
