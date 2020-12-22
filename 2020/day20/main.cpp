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
#include <set>

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
    std::set<std::vector<std::string>> candidates;

    Image() = default;

    explicit Image(std::int64_t id, std::vector<std::string> tiles) : id(id), tiles(std::move(tiles)) {
        for (const auto flip_type : {FlipType::kNone, FlipType::kVertical, FlipType::kHorizontal, FlipType::kBoth}) {
            for (const auto rotate_type : {RotateType::kNone, RotateType::k90, RotateType::k180, RotateType::k270}) {
                candidates.insert(Transform(flip_type, rotate_type));
            }
        }
    }

    static std::vector<std::string> Flip1(const std::vector<std::string> &data, bool vertical) {
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

    static std::vector<std::string> Rotate1(const std::vector<std::string> &data, int degree) {
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

struct ImagePieceInfo {
    std::int64_t id;
    std::vector<std::string> data;
};

template <typename T>
std::map<std::int64_t, Image> ParseInput(T &input_stream) {
    std::map<std::int64_t, Image> ret;
    std::string line;

    std::int64_t image_id;
    std::vector<std::string> tiles;
    while (std::getline(input_stream, line)) {
        if (line.find("Tile ") == 0) {
            int id;
            sscanf(line.c_str(), "Tile %d:", &id);
            image_id = id;
        } else if (line.empty()) {
            assert(image_id != -1 && !tiles.empty());
            ret[image_id] = Image(image_id, tiles);
            tiles.clear();
            continue;
        } else {
            tiles.push_back(line);
        }
    }

    if (!tiles.empty()) {
        ret[image_id] = Image(image_id, tiles);
    }

    return ret;
}

bool CanPlace(const std::vector<ImagePieceInfo> &infos, const std::vector<std::string> &data, size_t size, size_t pos) {
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

std::vector<ImagePieceInfo> FindValidImageOrder(const std::map<std::int64_t, Image> &images) {
    size_t size = static_cast<size_t>(std::sqrt(images.size()));
    std::vector<ImagePieceInfo> ret;
    std::function<bool(size_t pos, const std::vector<ImagePieceInfo> &acc)> f;
    f = [&f, size, &images, &ret](size_t pos, const std::vector<ImagePieceInfo> &acc) -> bool {
        if (pos == images.size()) {
            ret = acc;
            return true;
        }

        for (const auto &it : images) {
            auto &image = it.second;
            bool used = false;
            for (const auto &info : acc) {
                if (image.id == info.id) {
                    used = true;
                    break;
                }
            }

            if (used) {
                continue;
            }

            for (const auto &candidate : image.candidates) {
                if (CanPlace(acc, candidate, size, pos)) {
                    auto tmp = acc;
                    tmp.emplace_back(ImagePieceInfo{image.id, candidate});
                    if (f(pos + 1, tmp)) {
                        return true;
                    }
                }
            }
        }

        return false;
    };

    auto found = f(0, std::vector<ImagePieceInfo>{});
    assert(found);

    return ret;
}

std::int64_t Solve01(const std::vector<ImagePieceInfo> &images) {
    size_t size = static_cast<size_t>(std::sqrt(images.size()));
    size_t right_up = size - 1;
    size_t left_down = size * size - size;
    size_t right_down = size * size - 1;
    return images[0].id * images[right_up].id * images[left_down].id * images[right_down].id;
}

int CountHashMark(const std::vector<std::string> &data) {
    int ret = 0;
    for (const auto &str : data) {
        for (const char c : str) {
            if (c == '#') {
                ++ret;
            }
        }
    }

    return ret;
}

std::int64_t Solve02(const std::vector<std::string> &full_image, const std::vector<std::string> &monster) {
    Image image(-1, full_image);

    for (const auto &candidate : image.candidates) {
        size_t x_limit = candidate.size() - monster.size();
        size_t y_limit = candidate[0].size() - monster[0].size();

        int count = 0;
        for (size_t i = 0; i < x_limit; ++i) {
            for (size_t j = 0; j < y_limit; ++j) {
                bool found = true;
                for (size_t m = 0; m < monster.size(); ++m) {
                    for (size_t n = 0; n < monster[m].size(); ++n) {
                        if (monster[m][n] != '#') {
                            continue;
                        }

                        if (candidate[i + m][j + n] != monster[m][n]) {
                            found = false;
                            break;
                        }
                    }
                }

                if (found) {
                    ++count;
                }
            }
        }

        if (count != 0) {
            return CountHashMark(candidate) - (count * CountHashMark(monster));
        }
    }

    assert(!"never reach here");
    return -1;
}

std::vector<std::string> RemoveBorder(const std::vector<ImagePieceInfo> &images) {
    size_t size = static_cast<size_t>(std::sqrt(images.size()));
    std::vector<std::string> ret;
    size_t image_size = images[0].data.size();
    for (size_t z = 0; z < size; ++z) {
        for (size_t i = 1; i < image_size - 1; ++i) {
            std::string line;
            for (size_t j = 0; j < size; ++j) {
                size_t idx = (z * size) + j;
                const ImagePieceInfo &info = images[idx];
                const std::string str = info.data[i].substr(1, image_size - 2);
                line.append(str);
            }

            ret.push_back(line);
        }
    }

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
    assert(images[2311].id == 2311);
    assert(images[1951].id == 1951);
    assert(images[1171].id == 1171);
    assert(images[1427].id == 1427);
    assert(images[1489].id == 1489);
    assert(images[2473].id == 2473);
    assert(images[2971].id == 2971);
    assert(images[2729].id == 2729);
    assert(images[3079].id == 3079);

    for (const auto &it : images) {
        auto &image = it.second;
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

    auto valid_order = FindValidImageOrder(images);
    assert(Solve01(valid_order) == 20899048083289);

    auto full_image = RemoveBorder(valid_order);
    assert(full_image.size() == 24 && full_image[0].size() == 24);

    // clang-format off
    std::vector<std::string> monster {
        "                  # ",
        "#    ##    ##    ###",
        " #  #  #  #  #  #   ",
    };
    // clang-format on

    assert(Solve02(full_image, monster) == 273);
}

} // namespace

int main() {
    Test01();

    // clang-format off
    std::vector<std::string> monster {
        "                  # ",
        "#    ##    ##    ###",
        " #  #  #  #  #  #   ",
    };
    // clang-format on

    auto images = ParseInput(std::cin);
    auto valid_order = FindValidImageOrder(images);
    std::cout << "Part01:" << Solve01(valid_order) << std::endl;

    auto full_image = RemoveBorder(valid_order);
    std::cout << "Part02:" << Solve02(full_image, monster) << std::endl;
    return 0;
}
