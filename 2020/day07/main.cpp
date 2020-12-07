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

std::set<std::string> FindBaseBags(const WeightInfo &weight_info) {
    std::set<std::string> ret;
    for (const auto &it : weight_info) {
        if (it.second.empty()) {
            ret.insert(it.first);
        }
    }

    return ret;
}

WeightInfo Normalization(const WeightInfo &weight_info) {
    std::set<std::string> base_bags = FindBaseBags(weight_info);
    WeightInfo resolved_bags;
    for (const auto &bag : base_bags) {
        resolved_bags[bag][bag] = 1;
    }

    size_t resolved = base_bags.size();
    while (resolved < weight_info.size()) {
        for (const auto &it : weight_info) {
            const auto &bag_color = it.first;
            if (resolved_bags.find(bag_color) != resolved_bags.end()) {
                continue;
            }

            auto &bag_weight = it.second;
            size_t resolved_colors = 0;
            std::map<std::string, std::uint64_t> normalized_weight;
            for (const auto &it2 : bag_weight) {
                const auto &color = it2.first;
                const auto &count = it2.second;

                if (resolved_bags.find(color) != resolved_bags.end()) {
                    ++resolved_colors;
                    for (const auto &base_color_it : resolved_bags[color]) {
                        const auto &base_color = base_color_it.first;
                        auto base_count = base_color_it.second;
                        normalized_weight[base_color] += count * base_count;
                    }
                }
            }

            if (resolved_colors == bag_weight.size()) {
                resolved_bags[bag_color] = std::move(normalized_weight);
                ++resolved;
            }
        }
    }

    return resolved_bags;
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

int CountCanHoldBack(const WeightInfo &weight_info, const std::string &color) {
    assert(weight_info.find(color) != weight_info.end());

    int ret = 0;
    const auto &my_weight = weight_info.at(color);
    for (const auto &it : weight_info) {
        if (it.first == color) {
            continue;
        }

        size_t count = 0;
        for (const auto &it2 : it.second) {
            if (it2.second >= my_weight.at(it2.first)) {
                ++count;
            }
        }

        if (count == my_weight.size()) {
            printf("%s -> ", it.first.c_str());
            for (const auto &it2 : it.second) {
                printf(" %s:%lld ", it2.first.c_str(), static_cast<long long int>(it2.second));
            }
            printf("\n");
            ++ret;
        }
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

    auto base_bags = FindBaseBags(weight_info);
    assert((base_bags == std::set<std::string>{"faded blue", "dotted black"}));

    auto normalized = Normalization(weight_info);
    assert(normalized.size() == 9);
    assert(normalized["light red"]["faded blue"] == 83);
    assert(normalized["light red"]["dotted black"] == 80);
    assert(normalized["dark orange"]["faded blue"] == 179);
    assert(normalized["dark orange"]["dotted black"] == 176);
    assert(normalized["bright white"]["faded blue"] == 13);
    assert(normalized["bright white"]["dotted black"] == 16);
    assert(normalized["muted yellow"]["faded blue"] == 35);
    assert(normalized["muted yellow"]["dotted black"] == 32);
    assert(normalized["shiny gold"]["faded blue"] == 13);
    assert(normalized["shiny gold"]["dotted black"] == 16);
    assert(normalized["dark olive"]["faded blue"] == 3);
    assert(normalized["dark olive"]["dotted black"] == 4);
    assert(normalized["vibrant plum"]["faded blue"] == 5);
    assert(normalized["vibrant plum"]["dotted black"] == 6);
    assert(normalized["faded blue"]["faded blue"] == 1);
    assert(normalized["dotted black"]["dotted black"] == 1);

    assert(CountCanHoldBack(normalized, "shiny gold") == 4);
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

    WeightInfo weight_info = ParseBagInfo(input);
    WeightInfo normalized = Normalization(weight_info);
    std::cout << "Part1: " << CountCanHoldBack(normalized, "shiny gold") << std::endl;
    return 0;
}
