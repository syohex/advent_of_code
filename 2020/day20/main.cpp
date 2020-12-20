#include <cassert>
#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <functional>
#include <cmath>
#include <map>

namespace {

enum class RotateType {
    kNone,
    k90,
    k180,
    k270,
};

enum class FlipType {
    kNone,
    kVertical,
    kHorizontal,
    kBoth,
};

struct Image {
    std::int64_t id = -1;
    std::vector<std::string> tiles;

    static std::vector<std::string> Flip1(const std::vector<std::string>& data, bool vertical) {
        std::vector<std::string> ret;
        if (vertical) {
            for (const auto &tile : data) {
                std::string line = tile;
                std::reverse(line.begin(), line.end());
                ret.push_back(line);
            }
        } else {
            for (auto it = data.rbegin(); it != data.rend(); ++it) {
                std::string line = *it;
                ret.push_back(line);
            }
        }

        return ret;
    }

    std::vector<std::string> Flip(bool vertical) const {
        return Flip1(tiles, vertical);
    }

    static std::vector<std::string> Rotate1(const std::vector<std::string>& data, int degree) {
        std::vector<std::string> ret;
        int count = degree / 90;
        switch (count) {
        case 1: {
            ret = data;
            for (size_t i = 0; i < data.size(); ++i) {
                size_t row_limit = data.size();
                for (size_t j = 0; j < row_limit; ++j) {
                    ret[i][j] = data[row_limit - 1 - j][i];
                }
            }
            break;
        }
        case 2: {
            for (auto it = data.rbegin(); it != data.rend(); ++it) {
                std::string line = *it;
                std::reverse(line.begin(), line.end());
                ret.push_back(line);
            }
            break;
        }
        case 3: {
            ret = data;
            for (size_t i = 0; i < data.size(); ++i) {
                size_t col_limit = data[i].size();
                for (size_t j = 0; j < col_limit; ++j) {
                    ret[i][j] = data[j][col_limit - 1 - i];
                }
            }
            break;
        }
        default:
            assert(!"never reach here");
            break;
        }

        return ret;

    }

    std::vector<std::string> Rotate(int degree) const {
        assert(degree % 90 == 0 && degree < 360);
        return Rotate1(tiles, degree);
    }

    std::vector<std::string> Transform(FlipType flip_type, RotateType rotate_type) const {
        auto data = tiles;
        switch (flip_type) {
        case FlipType::kNone:
            break;
        case FlipType::kVertical:
            data = Flip1(data, true);
            break;
        case FlipType::kHorizontal:
            data = Flip1(data, false);
        case FlipType::kBoth:
            data = Flip1(Flip1(data, false), true);
            break;
        }

        switch (rotate_type) {
        case RotateType::kNone:
            return data;
        case RotateType::k90:
            return Rotate1(data, 90);
        case RotateType::k180:
            return Rotate1(data, 180);
        case RotateType::k270:
            return Rotate1(data, 270);
        }

        assert(!"never reach here");
        return std::vector<std::string>{};
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
            image.tiles.clear();
            continue;
        } else {
            image.tiles.push_back(line);
        }
    }

    if (!image.tiles.empty()) {
        ret.push_back(image);
    }

    return ret;
}

struct Info {
    std::int64_t id;
    std::vector<std::string> data;
};

bool CanPut(const std::vector<Info> &infos, const std::vector<std::string> &data, size_t size, size_t pos) {
    int row = pos / size;
    int col = pos % size;

    if (row - 1 >= 0) {
        auto &info = infos[pos - size];
        if (info.data.back() != data.front()) {
            return false;
        }
    }

    if (col - 1 >= 0) {
        auto &info = infos[pos - 1];
        for (size_t i = 0; i < data.size(); ++i) {
            if (info.data[i].back() != data[i].front()) {
                return false;
            }
        }
    }

    return true;
}

std::int64_t Solve01(const std::vector<Image> &images) {
    size_t limit = images.size();
    size_t row_size = static_cast<size_t>(std::sqrt(limit));

    std::vector<Info> answer;
    std::function<bool(size_t pos, const std::vector<Info> &acc)> f;
    f = [&f, row_size, &images, &answer](size_t pos, const std::vector<Info> &acc) -> bool {
        printf("@@ acc=%zd\n", acc.size());
        if (pos == images.size()) {
            answer = acc;
            return true;
        }

        for (const auto &image : images) {
            bool already_used = false;
            for (const auto &info : acc) {
                if (image.id == info.id) {
                    already_used = true;
                    break;
                }
            }

            if (already_used) {
                continue;
            }

            for (const auto flip_type : {FlipType::kNone, FlipType::kVertical, FlipType::kHorizontal, FlipType::kBoth}) {
                for (const auto rotate_type : {RotateType::kNone, RotateType::k90, RotateType::k180, RotateType::k270}) {
                    auto transformed_data = image.Transform(flip_type, rotate_type);
                    if (CanPut(acc, transformed_data, row_size, pos)) {
                        auto tmp = acc;
                        tmp.emplace_back(Info{image.id, transformed_data});
                        if (f(pos + 1, tmp)) {
                            return true;
                        }
                    }
                }
            }
        }

        return false;
    };

    auto found = f(0, std::vector<Info>{});
    assert(found);

    size_t right_up = row_size - 1;
    size_t left_down = row_size * row_size - row_size;
    size_t right_down = row_size * row_size - 1;
    return answer[0].id * answer[right_up].id * answer[left_down].id * answer[right_down].id;
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

    for (const auto &image : images) {
        assert(image.tiles.size() == 10);
        for (size_t i = 0; i < image.tiles.size(); ++i) {
            assert(image.tiles[i].size() == 10);
        }
    }

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
        assert(image.Flip(true) == vertical_flipped);
        assert(image.Flip(false) == horizontal_flipped);

        assert(image.Rotate(90) == rotated90);
        assert(image.Rotate(180) == rotated180);
        assert(image.Rotate(270) == rotated270);
    }

    assert(Solve01(images) == 20899048083289);
}

} // namespace

int main() {
    Test01();

    auto images = ParseInput(std::cin);
    std::cout << Solve01(images) << std::endl;
    return 0;
}
