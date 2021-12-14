#include <cassert>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <cstdint>
#include <list>
#include <algorithm>
#include <limits>

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
        ++it;
        for (; it != list.end(); ++it) {
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

    std::int64_t min = std::numeric_limits<std::int64_t>::max();
    std::int64_t max = std::numeric_limits<std::int64_t>::min();
    for (const auto &it : freq) {
        min = std::min(min, it.second);
        max = std::max(max, it.second);
    }

    return max - min;
}

std::int64_t Part2(const Data &data, int count) {
    std::map<std::string, std::int64_t> m;
    int len = data.init.size();
    for (size_t i = 0; i < len - 1; ++i) {
        std::string key = data.init.substr(i, 2);
        ++m[key];
    }

    std::map<char, std::int64_t> freq;
    for (char c : data.init) {
        ++freq[c];
    }

    const auto &rule = data.rule;
    for (int i = 0; i < count; ++i) {
        std::map<std::string, std::int64_t> tmp;
        for (const auto &it : m) {
            const auto &key = it.first;
            if (rule.find(key) != rule.end()) {
                std::string first, second;
                first += key[0];
                first += rule.at(key);
                second += rule.at(key);
                second += key[1];

                tmp[first] += it.second;
                tmp[second] += it.second;
                freq[rule.at(key)] += it.second;
            }
        }

        m = tmp;
    }

    std::int64_t min = std::numeric_limits<std::int64_t>::max();
    std::int64_t max = std::numeric_limits<std::int64_t>::min();
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
    auto part2 = Part2(data, 40);
    assert(part1 == 1588);
    assert(Part2(data, 10) == 1588);
    assert(part2 == 2188189693529);
}

} // namespace

int main() {
    Test();

    auto data = ParseInput(std::cin);
    auto part1 = Part1(data, 10);
    auto part2 = Part2(data, 40);

    std::cout << "Part1: " << part1 << std::endl;
    std::cout << "Part2: " << part2 << std::endl;
    return 0;
}
