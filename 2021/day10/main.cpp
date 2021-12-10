#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdint>
#include <map>
#include <algorithm>

namespace {

std::vector<std::string> ParseInput(std::istream &ss) {
    std::string tmp;
    std::vector<std::string> ret;
    while (std::getline(ss, tmp)) {
        if (tmp.empty()) {
            continue;
        }

        ret.push_back(tmp);
    }

    return ret;
}

std::int64_t Part1(const std::vector<std::string> &data) {
    std::map<char, int> wrong_chars;
    for (const auto &line : data) {
        std::vector<char> stack;
        bool ok = true;
        for (char c : line) {
            switch (c) {
            case '(':
            case '[':
            case '{':
            case '<':
                stack.push_back(c);
                break;
            case ')':
                if (!stack.empty() && stack.back() == '(') {
                    stack.pop_back();
                } else {
                    ++wrong_chars[c];
                    ok = false;
                }
                break;
            case ']':
                if (!stack.empty() && stack.back() == '[') {
                    stack.pop_back();
                } else {
                    ++wrong_chars[c];
                    ok = false;
                }
                break;
            case '}':
                if (!stack.empty() && stack.back() == '{') {
                    stack.pop_back();
                } else {
                    ++wrong_chars[c];
                    ok = false;
                }
                break;
            case '>':
                if (!stack.empty() && stack.back() == '<') {
                    stack.pop_back();
                } else {
                    ++wrong_chars[c];
                    ok = false;
                }
                break;
            default:
                assert(false && "never reach here");
                break;
            }

            if (!ok) {
                break;
            }
        }
    }

    const std::map<char, int> score{
        {')', 3},
        {']', 57},
        {'}', 1197},
        {'>', 25137},
    };

    std::int64_t ret = 0;
    for (const auto &it : wrong_chars) {
        ret += score.at(it.first) * it.second;
    }
    return ret;
}

std::int64_t Part2(const std::vector<std::string> &data) {
    std::vector<std::int64_t> incomplete_scores;
    for (const auto &line : data) {
        std::vector<char> stack;
        bool ok = true;
        for (char c : line) {
            switch (c) {
            case '(':
            case '[':
            case '{':
            case '<':
                stack.push_back(c);
                break;
            case ')':
                if (!stack.empty() && stack.back() == '(') {
                    stack.pop_back();
                } else {
                    ok = false;
                }
                break;
            case ']':
                if (!stack.empty() && stack.back() == '[') {
                    stack.pop_back();
                } else {
                    ok = false;
                }
                break;
            case '}':
                if (!stack.empty() && stack.back() == '{') {
                    stack.pop_back();
                } else {
                    ok = false;
                }
                break;
            case '>':
                if (!stack.empty() && stack.back() == '<') {
                    stack.pop_back();
                } else {
                    ok = false;
                }
                break;
            default:
                assert(false && "never reach here");
                break;
            }

            if (!ok) {
                break;
            }
        }

        if (ok) {
            std::int64_t score = 0;
            while (!stack.empty()) {
                score *= 5;
                switch (stack.back()) {
                case '(':
                    score += 1;
                    break;
                case '[':
                    score += 2;
                    break;
                case '{':
                    score += 3;
                    break;
                case '<':
                    score += 4;
                    break;
                default:
                    assert(false && "never reach here");
                    break;
                }

                stack.pop_back();
            }

            incomplete_scores.push_back(score);
        }
    }

    std::sort(incomplete_scores.begin(), incomplete_scores.end());
    int len = incomplete_scores.size();
    return incomplete_scores[len / 2];
}

void Test() {
    std::string input(R"([({(<(())[]>[[{[]{<()<>>
[(()[<>])]({[<{<<[]>>(
{([(<{}[<>[]}>{[]{[(<()>
(((({<>}<{<{<>}{[]{[]{}
[[<[([]))<([[{}[[()]]]
[{[{({}]{}}([{[{{{}}([]
{<[[]]>}<{[{[{[]{()[[[]
[<(<(<(<{}))><([]([]()
<{([([[(<>()){}]>(<<{{
<{([{{}}[<[[[<>{}]]]>[]])");

    std::stringstream ss(input);
    auto data = ParseInput(ss);
    auto part1 = Part1(data);
    auto part2 = Part2(data);

    assert(part1 == 26397);
    assert(part2 == 288957);
}

} // namespace

int main() {
    Test();

    auto data = ParseInput(std::cin);
    auto part1 = Part1(data);
    auto part2 = Part2(data);

    std::cout << "Part1: " << part1 << std::endl;
    std::cout << "Part2: " << part2 << std::endl;
    return 0;
}
