#include <cassert>
#include <iostream>
#include <vector>
#include <string>

namespace {

std::int64_t countTreeEncountered(const std::vector<std::string> &map, int x_step, int y_step) {
    int x = 0, y = 0;

    int width = static_cast<int>(map[0].size());
    int bottom = static_cast<int>(map.size());

    int forests = 0;
    while (y < bottom) {
        if (map[y][x] == '#') {
            ++forests;
        }

        x += x_step;
        if (x >= width) {
            x %= width;
        }
        y += y_step;
    }

    return forests;
}

void test() {
    std::vector<std::string> sample_map{
        // clang-format off
        "..##.......",
        "#...#...#..",
        ".#....#..#.",
        "..#.#...#.#",
        ".#...##..#.",
        "..#.##.....",
        ".#.#.#....#",
        ".#........#",
        "#.##...#...",
        "#...##....#",
        ".#..#...#.#",
        // clang-format on
    };

    // part 1
    assert(countTreeEncountered(sample_map, 3, 1));

    // part 2
    int ret = 1;
    ret *= countTreeEncountered(sample_map, 1, 1);
    ret *= countTreeEncountered(sample_map, 3, 1);
    ret *= countTreeEncountered(sample_map, 5, 1);
    ret *= countTreeEncountered(sample_map, 7, 1);
    ret *= countTreeEncountered(sample_map, 1, 2);
    assert(ret == 336);
}

} // namespace

int main() {
    test();

    std::vector<std::string> v;
    std::string line;
    while (std::getline(std::cin, line)) {
        v.push_back(line);
    }

    std::cout << "Part1: " << countTreeEncountered(v, 3, 1) << std::endl;

    std::int64_t ret = 1;
    ret *= countTreeEncountered(v, 1, 1);
    ret *= countTreeEncountered(v, 3, 1);
    ret *= countTreeEncountered(v, 5, 1);
    ret *= countTreeEncountered(v, 7, 1);
    ret *= countTreeEncountered(v, 1, 2);
    std::cout << "Part2: " << ret << std::endl;
    return 0;
}
