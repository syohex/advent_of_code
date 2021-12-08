#include <iostream>
#include <cassert>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <cstdint>
#include <algorithm>

namespace {

struct Data {
    std::vector<std::string> candidates;
    std::vector<std::string> output;

    std::int64_t Decode() const {
        std::vector<std::string> numbers(10, "");
        std::vector<bool> checked(10, false);
        size_t one_index = FindOne();
        size_t four_index = FindFour();
        size_t seven_index = FindSeven();
        size_t eight_index = FindEight();

        numbers[1] = candidates[one_index];
        numbers[4] = candidates[four_index];
        numbers[7] = candidates[seven_index];
        numbers[8] = candidates[eight_index];

        checked[one_index] = true;
        checked[four_index] = true;
        checked[seven_index] = true;
        checked[eight_index] = true;

        const auto just_match = [](const std::string &target, const std::string &source) -> bool {
            for (char c : source) {
                if (target.find(c) == std::string::npos) {
                    return false;
                }
            }

            return true;
        };

        // find 9
        for (size_t i = 0; i < candidates.size(); ++i) {
            if (checked[i]) {
                continue;
            }

            if (candidates[i].size() == 6) {
                if (just_match(candidates[i], numbers[4])) {
                    checked[i] = true;
                    numbers[9] = candidates[i];
                    break;
                }
            }
        }

        std::vector<size_t> five_segments;
        for (size_t i = 0; i < candidates.size(); ++i) {
            if (checked[i]) {
                continue;
            }

            if (candidates[i].size() == 5) {
                five_segments.push_back(i);
            }
        }

        for (size_t i : five_segments) {
            if (just_match(candidates[i], numbers[1])) {
                checked[i] = true;
                numbers[3] = candidates[i];
            }
        }

        for (size_t i : five_segments) {
            if (checked[i]) {
                continue;
            }
            if (just_match(numbers[9], candidates[i])) {
                checked[i] = true;
                numbers[5] = candidates[i];
                break;
            }
        }

        for (size_t i : five_segments) {
            if (checked[i]) {
                continue;
            }

            checked[i] = true;
            numbers[2] = candidates[i];
        }

        for (size_t i = 0; i < candidates.size(); ++i) {
            if (checked[i]) {
                continue;
            }

            if (candidates[i].size() == 6) {
                if (just_match(candidates[i], numbers[1]) && just_match(numbers[8], candidates[i])) {
                    checked[i] = true;
                    numbers[0] = candidates[i];
                    break;
                }
            }
        }

        for (size_t i = 0; i < candidates.size(); ++i) {
            if (checked[i]) {
                continue;
            }

            if (candidates[i].size() == 6) {
                checked[i] = true;
                numbers[6] = candidates[i];
                break;
            }
        }

        std::map<std::string, int> m;
        for (size_t i = 0; i < numbers.size(); ++i) {
            std::string tmp(numbers[i]);
            std::sort(tmp.begin(), tmp.end());
            m[tmp] = static_cast<int>(i);
        }

        std::int64_t ret = 0;
        for (const auto &str : output) {
            std::string tmp(str);
            std::sort(tmp.begin(), tmp.end());
            if (m.find(tmp) == m.end()) {
                throw "never reach here";
            }
            ret = ret * 10 + m[tmp];
        }

        return ret;
    }

    size_t FindOne() const {
        return FindByLength(2);
    }
    size_t FindFour() const {
        return FindByLength(4);
    }
    size_t FindSeven() const {
        return FindByLength(3);
    }
    size_t FindEight() const {
        return FindByLength(7);
    }

    size_t FindByLength(size_t len) const {
        for (size_t i = 0; i < candidates.size(); ++i) {
            if (candidates[i].size() == len) {
                return i;
            }
        }

        throw "never reach here";
    }
};

std::vector<std::string> SplitString(const std::string &str) {
    std::stringstream ss(str);
    std::string tmp;
    std::vector<std::string> ret;
    while (std::getline(ss, tmp, ' ')) {
        if (tmp.empty()) {
            continue;
        }

        ret.push_back(tmp);
    }

    return ret;
}

std::vector<Data> ParseInput(std::istream &ss) {
    std::vector<std::string> lines;
    std::string tmp;

    while (std::getline(ss, tmp)) {
        lines.push_back(tmp);
    }

    std::vector<Data> ret;
    for (const auto &line : lines) {
        std::stringstream ss_line(line);
        Data d;

        std::string tmp1, tmp2;
        std::getline(ss_line, tmp1, '|');
        std::getline(ss_line, tmp2, '|');

        d.candidates = SplitString(tmp1);
        d.output = SplitString(tmp2);

        ret.push_back(d);
    }

    return ret;
}

int Part1(const std::vector<Data> &data) {
    int ret = 0;
    for (const auto &d : data) {
        for (const auto &str : d.output) {
            size_t size = str.size();
            if (size == 2 || size == 4 || size == 3 || size == 7) {
                ++ret;
            }
        }
    }

    return ret;
}

std::int64_t Part2(const std::vector<Data> &data) {
    std::int64_t ret = 0;
    for (const auto &d : data) {
        ret += d.Decode();
    }

    return ret;
}

void Test() {
    std::string test(R"(be cfbegad cbdgef fgaecd cgeb fdcge agebfd fecdb fabcd edb | fdgacbe cefdb cefbgd gcbe
edbfga begcd cbg gc gcadebf fbgde acbgfd abcde gfcbed gfec | fcgedb cgb dgebacf gc
fgaebd cg bdaec gdafb agbcfd gdcbef bgcad gfac gcb cdgabef | cg cg fdcagb cbg
fbegcd cbd adcefb dageb afcb bc aefdc ecdab fgdeca fcdbega | efabcd cedba gadfec cb
aecbfdg fbg gf bafeg dbefa fcge gcbea fcaegb dgceab fcbdga | gecf egdcabf bgf bfgea
fgeab ca afcebg bdacfeg cfaedg gcfdb baec bfadeg bafgc acf | gebdcfa ecba ca fadegcb
dbcfg fgd bdegcaf fgec aegbdf ecdfab fbedc dacgb gdcebf gf | cefg dcbef fcge gbcadfe
bdfegc cbegaf gecbf dfcage bdacg ed bedf ced adcbefg gebcd | ed bcgafe cdgba cbgef
egadfb cdbfeg cegd fecab cgb gbdefca cg fgcdab egfdb bfceg | gbdfcae bgc cg cgb
gcafb gcf dcaebfg ecagb gf abcdeg gaef cafbge fdbac fegbdc | fgae cfgab fg bagce)");

    std::stringstream ss(test);
    auto data = ParseInput(ss);
    auto part1 = Part1(data);
    assert(part1 == 26);

    auto part2 = Part2(data);
    assert(part2 == 61229);
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
