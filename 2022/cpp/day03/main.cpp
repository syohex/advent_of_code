#include <cassert>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <set>
#include <iostream>

template <typename T>
std::vector<std::string> ParseInput(T &it) {
    std::vector<std::string> ret;
    std::string tmp;
    while (std::getline(it, tmp)) {
        if (tmp.empty()) {
            break;
        }

        ret.push_back(tmp);
    }

    return ret;
}

int Problem1(const std::vector<std::string> &data) {
    int ret = 0;
    for (const std::string &items : data) {
        size_t len = items.size() / 2;
        std::set<char> s;
        for (size_t i = 0; i < len; ++i) {
            s.insert(items[i]);
        }

        for (size_t i = len; i < items.size(); ++i) {
            char c = items[i];
            if (s.find(c) != s.end()) {
                if (c >= 'a' && c <= 'z') {
                    ret += c - 'a' + 1;
                } else {
                    ret += c - 'A' + 27;
                }
                break;
            }
        }
    }

    return ret;
}

void Test() {
    std::string input(R"(vJrwpWtwJgWrhcsFMMfFFhFp
jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL
PmmdzqPrVvPwwTWBwg
wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn
ttgJtRGJQctTZtZT
CrZsJsPPZsGzwwsLwLmpwMDw)");
    std::stringstream ss(input);

    auto data = ParseInput(ss);
    int ret1 = Problem1(data);

    assert(ret1 == 157);
}

int main() {
    Test();

    auto data = ParseInput(std::cin);
    int ret1 = Problem1(data);

    std::cout << "Problem1: " << ret1 << std::endl;

    return 0;
}
