#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>

namespace {

int Solve01(const std::vector<std::string> &input) {
    int ret = 0;
    std::set<char> s;
    for (const auto &line : input) {
        if (line.empty()) {
            ret += static_cast<int>(s.size());
            s.clear();
            continue;
        }

        for (const char c : line) {
            s.insert(c);
        }
    }

    ret += static_cast<int>(s.size());
    return ret;
}

int Solve02(const std::vector<std::string> &input) {
    int ret = 0;
    std::map<char, int> m;
    int persons = 0;
    for (const auto &line : input) {
        if (line.empty()) {
            int count = 0;

            for (const auto &it : m) {
                if (it.second == persons) {
                    ++count;
                }
            }

            ret += count;
            persons = 0;
            m.clear();
            continue;
        }

        ++persons;
        for (const char c : line) {
            ++m[c];
        }
    }

    int count = 0;
    for (const auto &it : m) {
        if (it.second == persons) {
            ++count;
        }
    }

    ret += count;
    return ret;
}

void Test01() {
    std::vector<std::string> input{"abc",
                                   "", //
                                   "a",   "b",  "c",
                                   "", //
                                   "ab",  "ac",
                                   "", //
                                   "a",   "a",  "a", "a",
                                   "", //
                                   "b"};

    assert(Solve01(input) == 11);
}

void Test02() {
    std::vector<std::string> input{"abc",
                                   "", //
                                   "a",   "b",  "c",
                                   "", //
                                   "ab",  "ac",
                                   "", //
                                   "a",   "a",  "a", "a",
                                   "", //
                                   "b"};

    assert(Solve02(input) == 6);
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

    std::cout << "Part01: " << Solve01(input) << std::endl;
    std::cout << "Part02: " << Solve02(input) << std::endl;
    return 0;
}
