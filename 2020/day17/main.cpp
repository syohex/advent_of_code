#include <cassert>
#include <vector>
#include <map>
#include <iostream>
#include <string>

namespace {

int CountActiveNeighbor(const std::map<std::vector<int>, int> &m, int x, int y, int z) {
    int count = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            for (int k = -1; k <= 1; ++k) {
                if (i == 0 && j == 0 && k == 0) {
                    continue;
                }

                const std::vector<int> key{x + i, y + j, z + k};
                if (m.find(key) != m.end()) {
                    count += m.at(key);
                }
            }
        }
    }

    return count;
}

int Solve01(const std::map<std::vector<int>, int> &input, int cycles) {
    auto tmp = input;
    std::map<std::vector<int>, char> m;

    for (int cycle = 0; cycle < cycles; ++cycle) {
        std::map<std::vector<int>, int> next;

        for (const auto &it : tmp) {
            int x = it.first[0];
            int y = it.first[1];
            int z = it.first[2];

            for (int i = x - 1; i <= x + 1; ++i) {
                for (int j = y - 1; j <= y + 1; ++j) {
                    for (int k = z - 1; k <= z + 1; ++k) {
                        const std::vector<int> key{i, j, k};
                        int val;
                        if (tmp.find(key) == tmp.end()) {
                            val = 0;
                        } else {
                            val = tmp.at(key);
                        }

                        int count = CountActiveNeighbor(tmp, i, j, k);
                        if ((val == 1 && (count == 2 || count == 3)) || (val == 0 && count == 3)) {
                            next[key] = 1;
                        } else {
                            next[key] = 0;
                        }
                    }
                }
            }
        }

        tmp = next;
    }

    int ret = 0;
    for (const auto &it : tmp) {
        ret += it.second;
    }

    return ret;
}

std::map<std::vector<int>, int> ParseInput(const std::vector<std::string> &input) {
    std::map<std::vector<int>, int> m;
    for (int i = 0; i < static_cast<int>(input.size()); ++i) {
        for (int j = 0; j < static_cast<int>(input[i].size()); ++j) {
            m[std::vector<int>{i, j, 0}] = input[i][j] == '#' ? 1 : 0;
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

    auto m = ParseInput(input);
    Solve01(m, 1);
    assert(Solve01(m, 6) == 112);
}

} // namespace

int main() {
    Test01();

    std::vector<std::string> input;
    std::string line;
    while (std::getline(std::cin, line)) {
        input.push_back(line);
    }

    auto m = ParseInput(input);
    std::cout << "Part01: " << Solve01(m, 6) << std::endl;
    return 0;
}
