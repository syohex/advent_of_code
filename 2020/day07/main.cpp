#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <sstream>

namespace {

struct BagWeightInfo {
    std::string color;
    int count;
};

struct BagInfo {
    std::string color;
    std::vector<BagWeightInfo> weights;
};

std::vector<BagInfo> ParseBagInfo(const std::string &input) {
    std::vector<BagInfo> ret;

    std::istringstream ss(input);
    std::string line;

    std::regex r("^(.+?) bags contain");
    std::regex re("(\\d+) (.+?) bags?");
    while (std::getline(ss, line)) {
        BagInfo info;
        std::smatch match;
        if (std::regex_search(line, match, r)) {
            std::string bag_color = match[1].str();
            info.color = bag_color;
            std::cout << "Bag: " << bag_color << std::endl;
        }

        auto begin = std::sregex_iterator(line.begin(), line.end(), re);
        auto end = std::sregex_iterator();

        for (auto it = begin; it != end; ++it) {
            std::smatch match = *it;

            auto match_it = it->begin();
            std::advance(match_it, 1);
            int count = std::stoi(*match_it);
            std::advance(match_it, 1);
            std::string color = *match_it;

            info.weights.push_back(BagWeightInfo{color, count});

            std::cout << "\t color: " << color << ", count: " << count << std::endl;
        }

        ret.push_back(info);
    }

    return ret;
}

void Test01() {
    // clang-format off
    std::string input(R"(light red bags contain 1 bright white bag, 2 muted yellow bags.
dark orange bags contain 3 bright white bags, 4 muted yellow bags.
bright white bags contain 1 shiny gold bag.
muted yellow bags contain 2 shiny gold bags, 9 faded blue bags.
shiny gold bags contain 1 dark olive bag, 2 vibrant plum bags.
dark olive bags contain 3 faded blue bags, 4 dotted black bags.
vibrant plum bags contain 5 faded blue bags, 6 dotted black bags.
faded blue bags contain no other bags.
dotted black bags contain no other bags.)");
    // clang-format on

    ParseBagInfo(input);
}

} // namespace

int main() {
    Test01();

    std::string input;
    std::string line;
    while (std::getline(std::cin, line)) {
        input += line;
        input += '\n';
    }

    ParseBagInfo(input);
    return 0;
}
