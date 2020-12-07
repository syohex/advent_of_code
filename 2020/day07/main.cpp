#include <cassert>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <sstream>
#include <set>
#include <map>

namespace {

using WeightInfo = std::map<std::string, std::map<std::string, std::uint64_t>>;

int Solve01(const WeightInfo &weight_info, const std::string &color) {
    std::set<std::string> hold_bags;

    while (true) {
        int checked = 0;
        for (const auto &it : weight_info) {
            if (it.first == color || hold_bags.find(it.first) != hold_bags.end()) {
                continue;
            }

            for (const auto &it2 : it.second) {
                if (it2.first == color || hold_bags.find(it2.first) != hold_bags.end()) {
                    ++checked;
                    hold_bags.insert(it.first);
                    break;
                }
            }
        }

        if (checked == 0) {
            break;
        }
    }

    return static_cast<int>(hold_bags.size());
}

int Solve02(const WeightInfo &weight_info, const std::string &color) {
    std::map<std::string, std::uint64_t> bag_count;
    for (const auto &it : weight_info) {
        if (it.second.empty()) {
            bag_count[it.first] = 1;
        }
    }

    while (bag_count.size() < weight_info.size()) {
        for (const auto &it : weight_info) {
            int resolved = 0;
            std::uint64_t sum = 0;

            for (const auto &it2 : it.second) {
                const auto &element_bag = it2.first;
                const auto &element_count = it2.second;
                if (bag_count.find(element_bag) != bag_count.end()) {
                    sum += element_count * bag_count[element_bag];
                    ++resolved;
                }
            }

            if (resolved == it.second.size()) {
                bag_count[it.first] = sum + 1;
            }
        }
    }

    return bag_count.at(color) - 1; // remove it self
}

WeightInfo ParseBagInfo(const std::string &input) {
    WeightInfo ret;

    std::istringstream ss(input);
    std::string line;

    std::regex r("^(.+?) bags contain");
    std::regex re("(\\d+) (.+?) bags?");
    while (std::getline(ss, line)) {
        std::string bag_color;
        std::smatch match;
        if (std::regex_search(line, match, r)) {
            bag_color = match[1].str();
        }
        assert(!bag_color.empty());

        auto begin = std::sregex_iterator(line.begin(), line.end(), re);
        auto end = std::sregex_iterator();

        std::map<std::string, std::uint64_t> weight;
        for (auto it = begin; it != end; ++it) {
            std::smatch match = *it;

            auto match_it = it->begin();
            std::advance(match_it, 1);
            std::uint64_t count = std::stoll(*match_it);
            std::advance(match_it, 1);
            std::string color = *match_it;

            weight[color] = count;
        }

        ret[bag_color] = std::move(weight);
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

    auto weight_info = ParseBagInfo(input);
    assert(weight_info.size() == 9);
    assert(weight_info["light red"]["bright white"] == 1);
    assert(weight_info["light red"]["muted yellow"] == 2);
    assert(weight_info["dark orange"]["bright white"] == 3);
    assert(weight_info["dark orange"]["muted yellow"] == 4);
    assert(weight_info["bright white"]["shiny gold"] == 1);
    assert(weight_info["muted yellow"]["shiny gold"] == 2);
    assert(weight_info["muted yellow"]["faded blue"] == 9);
    assert(weight_info["shiny gold"]["dark olive"] == 1);
    assert(weight_info["shiny gold"]["vibrant plum"] == 2);
    assert(weight_info["dark olive"]["faded blue"] == 3);
    assert(weight_info["dark olive"]["dotted black"] == 4);
    assert(weight_info["vibrant plum"]["faded blue"] == 5);
    assert(weight_info["vibrant plum"]["dotted black"] == 6);
    assert(weight_info["faded blue"].empty());
    assert(weight_info["dotted black"].empty());

    assert(Solve01(weight_info, "shiny gold") == 4);
}

void Test02() {
    // clang-format off
    std::string input(R"(shiny gold bags contain 2 dark red bags.
dark red bags contain 2 dark orange bags.
dark orange bags contain 2 dark yellow bags.
dark yellow bags contain 2 dark green bags.
dark green bags contain 2 dark blue bags.
dark blue bags contain 2 dark violet bags.
dark violet bags contain no other bags.)");
    // clang-format on

    auto weight_info = ParseBagInfo(input);
    assert(weight_info.size() == 7);

    assert(Solve02(weight_info, "shiny gold") == 126);
}

} // namespace

int main() {
    Test01();
    Test02();

    std::string input;
    std::string line;
    while (std::getline(std::cin, line)) {
        input += line;
        input += '\n';
    }

    WeightInfo weight_info = ParseBagInfo(input);
    std::cout << "Part1: " << Solve01(weight_info, "shiny gold") << std::endl;
    std::cout << "Part2: " << Solve02(weight_info, "shiny gold") << std::endl;
    return 0;
}
