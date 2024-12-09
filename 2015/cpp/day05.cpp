#include <cassert>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

using namespace std;

auto HasThreeVowels(const string &s) -> bool {
    int vowels = 0;
    for (char c : s) {
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            ++vowels;
            if (vowels >= 3) {
                return true;
            }
        }
    }
    return false;
}

auto HasDoubleLetter(const string &s) -> bool {
    for (size_t i = 1; i < s.size(); ++i) {
        if (s[i - 1] == s[i]) {
            return true;
        }
    }
    return false;
}

auto HasInvalidWord(const string &s) -> bool {
    const auto invalid_words = {"ab", "cd", "pq", "xy"};
    for (const auto *word : invalid_words) {
        if (s.find(word) != string::npos) {
            return true;
        }
    }

    return false;
}

auto IsNiceString(const string &s) -> bool {
    return HasThreeVowels(s) && HasDoubleLetter(s) && !HasInvalidWord(s);
}

auto Problem1(const vector<string> &input) -> int {
    int ret = 0;
    for (const auto &s : input) {
        if (IsNiceString(s)) {
            ++ret;
        }
    }

    return ret;
}

auto HasTwoLettersPair(const string &s) -> bool {
    size_t len = s.size();
    for (size_t i = 0; i < len - 1; ++i) {
        auto substr = s.substr(i, 2);
        auto pos = s.find(substr, i + 1);
        if (pos != string::npos && pos >= i + 2) {
            return true;
        }
    }

    return false;
}

auto HasWrapping(const string &s) -> bool {
    for (size_t i = 2; i < s.size(); ++i) {
        if (s[i - 2] == s[i]) {
            return true;
        }
    }

    return false;
}

auto IsNiceString2(const string &s) -> bool {
    return HasTwoLettersPair(s) && HasWrapping(s);
}

auto Problem2(const vector<string> &input) -> int {
    int ret = 0;
    for (const auto &s : input) {
        if (IsNiceString2(s)) {
            ++ret;
        }
    }

    return ret;
}

auto Test() -> void {
    assert(IsNiceString("ugknbfddgicrmopn"));
    assert(IsNiceString("aaa"));
    assert(!IsNiceString("jchzalrnumimnmhp"));
    assert(!IsNiceString("haegwjzuvuyypxyu"));
    assert(!IsNiceString("dvszwmarrgswjxmb"));

    assert(IsNiceString2("qjhvhtzxzqqjkmpb"));
    assert(IsNiceString2("xxyxx"));
    assert(!IsNiceString2("uurcxstgmygtbstg"));
    assert(!IsNiceString2("ieodomkazucvgmuy"));
}

int main() {
    Test();

    ifstream f("../input/day05.txt");
    vector<string> input;
    string tmp;
    while (f >> tmp) {
        input.push_back(tmp);
    }

    int ret1 = Problem1(input);
    assert(ret1 == 258);

    int ret2 = Problem2(input);
    assert(ret2 == 53);

    printf("ret1 = %d\n", ret1);
    printf("ret2 = %d\n", ret2);
    return 0;
}