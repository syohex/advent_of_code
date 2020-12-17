#include <cassert>
#include <vector>
#include <map>
#include <iostream>
#include <string>

namespace {

int CountActiveNeighbor01(const std::map<std::vector<int>, bool> &grid, int x, int y, int z) {
    int count = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            for (int k = -1; k <= 1; ++k) {
                if (i == 0 && j == 0 && k == 0) {
                    continue;
                }

                const std::vector<int> key{x + i, y + j, z + k};
                if (grid.find(key) != grid.end()) {
                    count += grid.at(key) ? 1 : 0;
                }
            }
        }
    }

    return count;
}

int CountActiveNeighbor02(const std::map<std::vector<int>, bool> &grid, int x, int y, int z, int w) {
    int count = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            for (int k = -1; k <= 1; ++k) {
                for (int m = -1; m <= 1; ++m) {
                    if (i == 0 && j == 0 && k == 0 && m == 0) {
                        continue;
                    }

                    const std::vector<int> key{x + i, y + j, z + k, w + m};
                    if (grid.find(key) != grid.end()) {
                        count += grid.at(key) ? 1 : 0;
                    }
                }
            }
        }
    }

    return count;
}

int Solve01(const std::map<std::vector<int>, bool> &input, int cycles) {
    auto grid = input;

    for (int cycle = 0; cycle < cycles; ++cycle) {
        std::map<std::vector<int>, bool> next;

        for (const auto &it : grid) {
            int x = it.first[0];
            int y = it.first[1];
            int z = it.first[2];

            for (int i = x - 1; i <= x + 1; ++i) {
                for (int j = y - 1; j <= y + 1; ++j) {
                    for (int k = z - 1; k <= z + 1; ++k) {
                        const std::vector<int> key{i, j, k};
                        bool active;
                        if (grid.find(key) == grid.end()) {
                            active = false;
                        } else {
                            active = grid.at(key);
                        }

                        int count = CountActiveNeighbor01(grid, i, j, k);
                        if ((active && (count == 2 || count == 3)) || (!active && count == 3)) {
                            next[key] = true;
                        } else {
                            next[key] = false;
                        }
                    }
                }
            }
        }

        grid = next;
    }

    int ret = 0;
    for (const auto &it : grid) {
        ret += it.second ? 1 : 0;
    }

    return ret;
}

int Solve02(const std::map<std::vector<int>, bool> &input, int cycles) {
    auto grid = input;

    for (int cycle = 0; cycle < cycles; ++cycle) {
        std::map<std::vector<int>, bool> next;

        for (const auto &it : grid) {
            int x = it.first[0];
            int y = it.first[1];
            int z = it.first[2];
            int w = it.first[3];

            for (int i = x - 1; i <= x + 1; ++i) {
                for (int j = y - 1; j <= y + 1; ++j) {
                    for (int k = z - 1; k <= z + 1; ++k) {
                        for (int m = w - 1; m <= w + 1; ++m) {
                            const std::vector<int> key{i, j, k, m};
                            bool active;
                            if (grid.find(key) == grid.end()) {
                                active = false;
                            } else {
                                active = grid.at(key);
                            }

                            int count = CountActiveNeighbor02(grid, i, j, k, m);
                            if ((active && (count == 2 || count == 3)) || (!active && count == 3)) {
                                next[key] = true;
                            } else {
                                next[key] = false;
                            }
                        }
                    }
                }
            }
        }

        grid = next;
    }

    int ret = 0;
    for (const auto &it : grid) {
        ret += it.second ? 1 : 0;
    }

    return ret;
}

std::map<std::vector<int>, bool> ParseInput01(const std::vector<std::string> &input) {
    std::map<std::vector<int>, bool> m;
    for (int i = 0; i < static_cast<int>(input.size()); ++i) {
        for (int j = 0; j < static_cast<int>(input[i].size()); ++j) {
            m[std::vector<int>{i, j, 0}] = input[i][j] == '#';
        }
    }

    return m;
}

std::map<std::vector<int>, bool> ParseInput02(const std::vector<std::string> &input) {
    std::map<std::vector<int>, bool> m;
    for (int i = 0; i < static_cast<int>(input.size()); ++i) {
        for (int j = 0; j < static_cast<int>(input[i].size()); ++j) {
            m[std::vector<int>{i, j, 0, 0}] = input[i][j] == '#';
        }
    }

    return m;
}

void Test01() {
    // clang-format off
    std::vector<std::string> input {
        ".#.",
        "..#",
        "###",
    };
    // clang-format on

    auto grid = ParseInput01(input);
    Solve01(grid, 1);
    assert(Solve01(grid, 6) == 112);
}

void Test02() {
    // clang-format off
    std::vector<std::string> input {
        ".#.",
        "..#",
        "###",
    };
    // clang-format on

    auto grid = ParseInput02(input);
    Solve02(grid, 1);
    assert(Solve02(grid, 6) == 848);
}

} // namespace

int main() {
    Test01();
    Test02();

    std::vector<std::string> input;
    std::string line;
    while (std::getline(std::cin, line)) {
        input.push_back(line);
    }

    auto grid01 = ParseInput01(input);
    auto grid02 = ParseInput02(input);
    std::cout << "Part01: " << Solve01(grid01, 6) << std::endl;
    std::cout << "Part02: " << Solve02(grid02, 6) << std::endl;
    return 0;
}
