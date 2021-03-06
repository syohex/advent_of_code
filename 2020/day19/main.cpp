#include <cassert>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

namespace {

struct Rule {
    std::vector<std::vector<int>> rules;
    std::string str;
    int id;

    std::vector<std::string> Apply(const std::map<int, Rule> &all_rules, const std::string &message,
                                   const std::vector<std::string> &prefixes = std::vector<std::string>{""}) const {
        if (!str.empty()) { // terminal symbol
            return std::vector<std::string>{str};
        }

        std::vector<std::string> ret;
        for (const auto &rule : rules) {
            std::vector<std::string> candidates{""};
            for (int rule_index : rule) {
                std::vector<std::string> new_prefixes;
                for (const auto &prefix : prefixes) {
                    for (const auto &cand : candidates) {
                        auto new_prefix = prefix + cand;
                        if (message.find(new_prefix) == 0) {
                            new_prefixes.emplace_back(std::move(new_prefix));
                        }
                    }
                }

                const auto strs = all_rules.at(rule_index).Apply(all_rules, message, new_prefixes);
                if (strs.empty()) {
                    goto next_rule;
                }

                std::vector<std::string> new_candidates;
                for (const auto &candidate : candidates) {
                    for (const auto &s : strs) {
                        auto new_candidate = candidate + s;
                        for (const auto &prefix : prefixes) {
                            if (message.find(prefix + new_candidate) == 0) {
                                new_candidates.emplace_back(std::move(new_candidate));
                                break;
                            }
                        }
                    }
                }

                if (new_candidates.empty()) {
                    goto next_rule;
                }

                candidates = std::move(new_candidates);
            }

            for (const auto &c : candidates) {
                ret.push_back(c);
            }

        next_rule:;
        }

        return ret;
    }
};

Rule ParseRuleLine(const std::string &line, int &rule_number) {
    size_t pos = 0;
    rule_number = 0;
    while (pos < line.size() && line[pos] >= '0' && line[pos] <= '9') {
        rule_number = rule_number * 10 + line[pos] - '0';
        ++pos;
    }

    ++pos; // skip colon

    Rule rule;
    rule.id = rule_number;
    std::vector<int> rule_numbers;
    while (pos < line.size()) {
        while (pos < line.size() && line[pos] == ' ') {
            ++pos;
        }

        if (line[pos] == '|') {
            rule.rules.push_back(rule_numbers);
            rule_numbers.clear();
            ++pos;
            continue;
        }

        if (line[pos] == '"') {
            ++pos;

            std::string str;
            while (pos < line.size() && line[pos] != '"') {
                str += line[pos];
                ++pos;
            }

            rule.str = str;
            break;
        }

        int index = 0;
        while (pos < line.size() && line[pos] >= '0' && line[pos] <= '9') {
            index = index * 10 + line[pos] - '0';
            ++pos;
        }

        rule_numbers.push_back(index);

        if (pos == line.size()) {
            rule.rules.push_back(rule_numbers);
            break;
        }
    }

    return rule;
}

template <typename T>
std::tuple<std::map<int, Rule>, std::vector<std::string>> ParseInput(T &input_stream) {
    std::map<int, Rule> rules;
    std::vector<std::string> messages;

    std::string line;
    bool rule_part = true;
    while (std::getline(input_stream, line)) {
        if (line.empty()) {
            rule_part = false;
            continue;
        }

        int rule_number;
        if (rule_part) {
            auto rule = ParseRuleLine(line, rule_number);
            rules[rule_number] = rule;
        } else {
            messages.push_back(line);
        }
    }

    return std::make_tuple(rules, messages);
}

int Solve(const std::map<int, Rule> &rules, const std::vector<std::string> &messages, int first_rule) {
    int ret = 0;
    for (const auto &message : messages) {
        const auto candidates = rules.at(first_rule).Apply(rules, message);
        if (std::find(candidates.begin(), candidates.end(), message) != candidates.end()) {
            ++ret;
        }
    }

    return ret;
}

void UpdateRules02(std::map<int, Rule> &rules) {
    rules[8].rules = std::vector<std::vector<int>>{{42}, {42, 8}};
    rules[11].rules = std::vector<std::vector<int>>{{42, 31}, {42, 11, 31}};
}

void Test01() {
    std::string input(R"(0: 4 1 5
1: 2 3 | 3 2
2: 4 4 | 5 5
3: 4 5 | 5 4
4: "a"
5: "b"

ababbb
bababa
abbbab
aaabbb
aaaabbb)");

    std::istringstream iss(input);
    auto [rules, messages] = ParseInput(iss);
    assert(rules.size() == 6);
    assert(messages.size() == 5);

    assert((rules[0].rules == std::vector<std::vector<int>>{{4, 1, 5}}));
    assert((rules[1].rules == std::vector<std::vector<int>>{{2, 3}, {3, 2}}));
    assert((rules[2].rules == std::vector<std::vector<int>>{{4, 4}, {5, 5}}));
    assert((rules[3].rules == std::vector<std::vector<int>>{{4, 5}, {5, 4}}));
    assert(rules[4].str == "a");
    assert(rules[5].str == "b");

    assert(messages[0] == "ababbb");
    assert(messages[1] == "bababa");
    assert(messages[2] == "abbbab");
    assert(messages[3] == "aaabbb");
    assert(messages[4] == "aaaabbb");

    assert(Solve(rules, messages, 0) == 2);
}

void Test02() {
    std::string input(R"(42: 9 14 | 10 1
9: 14 27 | 1 26
10: 23 14 | 28 1
1: "a"
11: 42 31
5: 1 14 | 15 1
19: 14 1 | 14 14
12: 24 14 | 19 1
16: 15 1 | 14 14
31: 14 17 | 1 13
6: 14 14 | 1 14
2: 1 24 | 14 4
0: 8 11
13: 14 3 | 1 12
15: 1 | 14
17: 14 2 | 1 7
23: 25 1 | 22 14
28: 16 1
4: 1 1
20: 14 14 | 1 15
3: 5 14 | 16 1
27: 1 6 | 14 18
14: "b"
21: 14 1 | 1 14
25: 1 1 | 1 14
22: 14 14
8: 42
26: 14 22 | 1 20
18: 15 15
7: 14 5 | 1 21
24: 14 1

abbbbbabbbaaaababbaabbbbabababbbabbbbbbabaaaa
bbabbbbaabaabba
babbbbaabbbbbabbbbbbaabaaabaaa
aaabbbbbbaaaabaababaabababbabaaabbababababaaa
bbbbbbbaaaabbbbaaabbabaaa
bbbababbbbaaaaaaaabbababaaababaabab
ababaaaaaabaaab
ababaaaaabbbaba
baabbaaaabbaaaababbaababb
abbbbabbbbaaaababbbbbbaaaababb
aaaaabbaabaaaaababaa
aaaabbaaaabbaaa
aaaabbaabbaaaaaaabbbabbbaaabbaabaaa
babaaabbbaaabaababbaabababaaab
aabbbbbaabbbaaaaaabbbbbababaaaaabbaaabba)");

    std::istringstream iss(input);
    auto [rules, messages] = ParseInput(iss);
    UpdateRules02(rules);

    assert((rules[8].rules == std::vector<std::vector<int>>{{42}, {42, 8}}));
    assert((rules[11].rules == std::vector<std::vector<int>>{{42, 31}, {42, 11, 31}}));

    assert(Solve(rules, messages, 0) == 12);
}

} // namespace

int main() {
    Test01();
    Test02();

    auto [rules, messages] = ParseInput(std::cin);
    std::cout << "Part01: " << Solve(rules, messages, 0) << std::endl;

    UpdateRules02(rules);
    std::cout << "Part02: " << Solve(rules, messages, 0) << std::endl;
    return 0;
}
