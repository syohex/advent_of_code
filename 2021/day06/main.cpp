#include <cassert>
#include <iostream>
#include <vector>
#include <string>

namespace {

int Part1(const std::vector<int> &data, int days) {
    std::vector<int> values(data);

    for (int i = 0; i < days; ++i) {
        int create_count = 0;
        for (size_t j = 0; j < values.size(); ++j) {
            if (values[j] == 0) {
                values[j] = 6;
                ++create_count;
            } else {
                --values[j];
            }
        }

        for (int j = 0; j < create_count; ++j) {
            values.push_back(8);
        }
    }

    return static_cast<int>(values.size());
}

void Test() {
    std::vector<int> data{3, 4, 3, 1, 2};
    assert(Part1(data, 18) == 26);
    assert(Part1(data, 80) == 5934);
}

} // namespace

int main() {
    Test();

    std::vector<int> data;
    std::string tmp;
    while (std::getline(std::cin, tmp, ',')) {
        if (tmp.empty()) {
            continue;
        }

        data.push_back(std::stoi(tmp));
    }

    auto part1 = Part1(data, 80);

    std::cout << "Part1: " << part1 << std::endl;
    return 0;
}