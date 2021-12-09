#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace {

std::vector<std::vector<int>> ParseInput(const std::vector<std::string> &data) {
    std::vector<std::vector<int>> ret;
    for (const auto &d : data) {
        std::vector<int> v;
        for (char c : d) {
            v.push_back(c - '0');
        }

        ret.push_back(v);
    }
    return ret;
}

int Part1(const std::vector<std::vector<int>> &data) {
    int rows = data.size();
    int cols = data[0].size();

    int ret = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (i >= 1 && data[i - 1][j] <= data[i][j]) {
                continue;
            }
            if (i < rows - 1 && data[i + 1][j] <= data[i][j]) {
                continue;
            }
            if (j >= 1 && data[i][j - 1] <= data[i][j]) {
                continue;
            }
            if (j < cols - 1 && data[i][j + 1] <= data[i][j]) {
                continue;
            }

            ret += data[i][j] + 1;
        }
    }

    return ret;
}

void Test() {
    std::vector<std::string> input{
        "2199943210", "3987894921", "9856789892", "8767896789", "9899965678",
    };

    auto data = ParseInput(input);
    auto part1 = Part1(data);

    assert(part1 == 15);
}

} // namespace

int main() {
    Test();

    std::vector<std::string> input;
    std::string tmp;
    while (std::cin >> tmp) {
        if (tmp.empty()) {
            continue;
        }

        input.push_back(tmp);
    }

    auto data = ParseInput(input);
    auto part1 = Part1(data);

    std::cout << "Part1: " << part1 << std::endl;
    return 0;
}
