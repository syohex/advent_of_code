#include <iostream>
#include <cassert>
#include <string>
#include <sstream>
#include <vector>

namespace {

struct Data {
    std::vector<std::string> first;
    std::vector<std::string> second;
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

        d.first = SplitString(tmp1);
        d.second = SplitString(tmp2);

        ret.push_back(d);
    }

    return ret;
}

int Part1(const std::vector<Data> &data) {
    int ret = 0;
    for (const auto &d : data) {
        for (const auto &str : d.second) {
            size_t size = str.size();
            if (size == 2 || size == 4 || size == 3 || size == 7) {
                ++ret;
            }
        }
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
}

} // namespace

int main() {
    Test();

    auto data = ParseInput(std::cin);
    auto part1 = Part1(data);

    std::cout << "Part1: " << part1 << std::endl;
    return 0;
}
