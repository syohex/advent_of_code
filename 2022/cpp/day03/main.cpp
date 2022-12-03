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

int Problem2(const std::vector<std::string> &data) {
    int ret = 0;
    int len = data.size();
    for (int i = 0; i < len; i += 3) {
        std::vector<std::vector<bool>> v(3, std::vector<bool>(128, false));
        for (int j = 0; j < 3; ++j) {
            const std::string &items = data[i + j];
            for (char c : items) {
                v[j][c] = true;
            }
        }

        for (int j = 0; j < 128; ++j) {
            if (v[0][j] && v[1][j] && v[2][j]) {
                if (j >= 'a' && j <= 'z') {
                    ret += j - 'a' + 1;
                } else {
                    ret += j - 'A' + 27;
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
    int ret2 = Problem2(data);

    assert(ret1 == 157);
    assert(ret2 == 70);
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
