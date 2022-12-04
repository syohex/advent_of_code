#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>

struct Data {
    int start1;
    int end1;
    int start2;
    int end2;
};

template <typename T>
std::vector<Data> ParseInput(T &it) {
    std::vector<Data> ret;
    std::string tmp;
    while (std::getline(it, tmp)) {
        if (tmp.empty()) {
            break;
        }

        Data d;
        sscanf(tmp.c_str(), "%d-%d,%d-%d", &d.start1, &d.end1, &d.start2, &d.end2);
        ret.push_back(d);
    }

    return ret;
}

int Problem1(const std::vector<Data> &data) {
    int ret = 0;
    for (const Data &d : data) {
        if ((d.start1 <= d.start2 && d.end2 <= d.end1) || (d.start2 <= d.start1 && d.end1 <= d.end2)) {
            ++ret;
        }
    }

    return ret;
}

int Problem2(const std::vector<Data> &data) {
    int ret = 0;

    for (const Data &d : data) {
        int start = std::max(d.start1, d.start2);
        int end = std::min(d.end1, d.end2);

        if (end >= start) {
            ++ret;
        }
    }

    return ret;
}

void Test() {
    std::string input(R"(2-4,6-8
2-3,4-5
5-7,7-9
2-8,3-7
6-6,4-6
2-6,4-8)");

    std::stringstream ss(input);
    auto data = ParseInput(ss);
    int ret1 = Problem1(data);
    int ret2 = Problem2(data);

    assert(ret1 == 2);
    assert(ret2 == 4);
}

int main() {
    Test();

    auto data = ParseInput(std::cin);
    int ret1 = Problem1(data);
    int ret2 = Problem2(data);

    std::cout << "Problem1: " << ret1 << std::endl;
    std::cout << "Problem2: " << ret2 << std::endl;
    return 0;
}
