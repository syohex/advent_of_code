#include <cassert>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <cstdint>
#include <list>
#include <algorithm>
#include <climits>

namespace {

struct Data {
    std::string init;
    std::map<std::string, char> rule;
};

Data ParseInput(std::istream &ss) {
    Data ret;
    std::string tmp;
    std::getline(ss, ret.init);
    std::getline(ss, tmp); // skip empty line

    while (std::getline(ss, tmp)) {
        if (tmp.empty()) {
            continue;
        }

        size_t pos = tmp.find(" -> ");
        auto key = tmp.substr(0, pos);
        auto value = tmp.substr(pos + 4)[0];

        ret.rule[key] = value;
    }

    return ret;
}

std::int64_t Part1(const Data &data, int count) {
    std::list<char> list(data.init.begin(), data.init.end());

    const auto &rule = data.rule;
    for (int i = 0; i < count; ++i) {
        std::string str;
        auto it = list.begin();
        str += *it;
        auto prev = it;
        ++it;
        for (; it != list.end(); prev = it, ++it) {
            str += *it;
            if (rule.find(str) != rule.end()) {
                list.insert(it, rule.at(str));
            }

            str = str.substr(1);
        }
    }

    std::map<char, std::int64_t> freq;
    for (char c : list) {
        ++freq[c];
    }

    std::int64_t min = INT_MAX;
    std::int64_t max = INT_MIN;
    for (const auto &it : freq) {
        min = std::min(min, it.second);
        max = std::max(max, it.second);
    }

    return max - min;
}

void Test() {
    std::string input(R"(NNCB

CH -> B
HH -> N
CB -> H
NH -> C
HB -> C
HC -> B
HN -> C
NN -> C
BH -> H
NC -> B
NB -> B
BN -> B
BB -> N
BC -> B
CC -> N
CN -> C)");

    std::stringstream ss(input);
    auto data = ParseInput(ss);
    auto part1 = Part1(data, 10);
    assert(part1 == 1588);
}

} // namespace

int main() {
    Test();

    auto data = ParseInput(std::cin);
    auto part1 = Part1(data, 10);

    std::cout << "Part1: " << part1 << std::endl;
    return 0;
}
