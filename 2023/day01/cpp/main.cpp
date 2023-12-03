#include <cassert>
#include <iostream>
#include <vector>
#include <string>

namespace {

int combineFirstAndLastDigits(const std::string &s) {
    int len = s.size();
    int first = -1;
    for (int i = 0; i < len; ++i) {
        if (s[i] >= '1' && s[i] <= '9') {
            first = s[i] - '0';
            break;
        }
    }

    assert(first != -1);

    for (int i = len - 1; i >= 0; --i) {
        if (s[i] >= '1' && s[i] <= '9') {
            return first * 10 + (s[i] - '0');
        }
    }

    assert(!"never reach here");
    return -1;
}

std::string replaceString(const std::string &s) {
    std::string ret;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == 'o') {
            if (s.find("one", i) == i) {
                ret.push_back('1');
                continue;
            }
        } else if (s[i] == 't') {
            if (s.find("two", i) == i) {
                ret.push_back('2');
                continue;
            }
            if (s.find("three", i) == i) {
                ret.push_back('3');
                continue;
            }
        } else if (s[i] == 'f') {
            if (s.find("four", i) == i) {
                ret.push_back('4');
                continue;
            }
            if (s.find("five", i) == i) {
                ret.push_back('5');
                continue;
            }
        } else if (s[i] == 's') {
            if (s.find("six", i) == i) {
                ret.push_back('6');
                continue;
            }
            if (s.find("seven", i) == i) {
                ret.push_back('7');
                continue;
            }
        } else if (s[i] == 'e') {
            if (s.find("eight", i) == i) {
                ret.push_back('8');
                continue;
            }
        } else if (s[i] == 'n') {
            if (s.find("nine", i) == i) {
                ret.push_back('9');
                continue;
            }
        }

        ret.push_back(s[i]);
    }

    return ret;
}

int problem1(const std::vector<std::string> &input) {
    int ret = 0;
    for (const auto &s : input) {
        ret += combineFirstAndLastDigits(s);
    }

    return ret;
}

int problem2(const std::vector<std::string> &input) {
    int ret = 0;
    for (const auto &s : input) {
        auto replaced = replaceString(s);
        ret += combineFirstAndLastDigits(replaced);
    }

    return ret;
}

void test1() {
    std::vector<std::string> input{
        "1abc2",
        "pqr3stu8vwx",
        "a1b2c3d4e5f",
        "treb7uchet",
    };

    int ret = problem1(input);
    assert(ret == 142);
}

void test2() {
    std::vector<std::string> input{
        "two1nine", "eightwothree", "abcone2threexyz", "xtwone3four", "4nineeightseven2", "zoneight234", "7pqrstsixteen",
    };

    int ret = problem2(input);
    assert(ret == 281);
}

} // namespace

int main() {
    std::vector<std::string> input;
    std::string line;
    while (std::cin >> line) {
        if (line.empty()) {
            continue;
        }
        input.push_back(line);
    }

    test1();
    test2();

    int ret1 = problem1(input);
    // 54490
    std::cout << "Problem1: " << ret1 << std::endl;

    int ret2 = problem2(input);
    std::cout << "Problem2: " << ret2 << std::endl;
    return 0;
}
