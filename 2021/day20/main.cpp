#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

namespace {

struct Data {
    std::vector<int> enhancement;
    std::vector<std::vector<int>> image;
};

Data ParseInput(std::istream &ss) {
    Data d;
    std::string tmp;

    std::getline(ss, tmp);
    for (char c : tmp) {
        d.enhancement.push_back(c == '#' ? 1 : 0);
    }

    std::getline(ss, tmp); // skip empty line

    while (std::getline(ss, tmp)) {
        if (tmp.empty()) {
            continue;
        }

        std::vector<int> v;
        for (char c : tmp) {
            v.push_back(c == '#' ? 1 : 0);
        }
        d.image.push_back(std::move(v));
    }

    return d;
}

int Part1(const Data &data, int count) {
    int rows = data.image.size();
    int cols = data.image[0].size();
    std::vector<std::vector<int>> output(rows + 2, std::vector<int>(cols + 2, 0));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            output[i + 1][j + 1] = data.image[i][j];
        }
    }

    for (int n = 1; n <= count; ++n) {
        rows = output.size();
        cols = output[0].size();

        std::vector<std::vector<int>> tmp(rows, std::vector<int>(cols, 0));
        int outside_value;
        if (data.enhancement[0] == 0) {
            outside_value = 0;
        } else {
            if (n % 2 == 0) {
                outside_value = 1;
            } else {
                outside_value = 0;
            }
        }

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int value = 0;
                for (int x = -1; x <= 1; ++x) {
                    for (int y = -1; y <= 1; ++y) {
                        int row = i + x;
                        int col = j + y;
                        if (row >= 0 && row < rows && col >= 0 && col < cols) {
                            value = 2 * value + output[row][col];
                        } else {
                            value = 2 * value + outside_value;
                        }
                    }
                }

                tmp[i][j] = data.enhancement[value];
            }
        }

        int new_outside_value;
        if (data.enhancement[0] == 0) {
            new_outside_value = 0;
        } else {
            if (n % 2 == 1) {
                new_outside_value = 1;
            } else {
                new_outside_value = 0;
            }
        }

        output = std::vector<std::vector<int>>(rows + 2, std::vector<int>(cols + 2, new_outside_value));
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                output[i + 1][j + 1] = tmp[i][j];
            }
        }
    }

    int ret = 0;
    for (const auto &v : output) {
        for (int n : v) {
            ret += n;
        }
    }

    return ret;
}

void Test() {
    std::ifstream iss;
    iss.open("test.txt");
    auto data = ParseInput(iss);
    iss.close();

    auto part1 = Part1(data, 2);
    auto part2 = Part1(data, 50);
    assert(part1 == 35);
    assert(part2 == 3351);
}

} // namespace

int main() {
    Test();

    auto data = ParseInput(std::cin);
    auto part1 = Part1(data, 2);
    auto part2 = Part1(data, 50);

    std::cout << "Part1: " << part1 << std::endl;
    std::cout << "Part2: " << part2 << std::endl;

    assert(part1 == 5065);
    assert(part2 == 14790);
    return 0;
}