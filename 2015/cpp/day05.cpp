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

auto Test() -> void {
    assert(IsNiceString("ugknbfddgicrmopn"));
    assert(IsNiceString("aaa"));
    assert(!IsNiceString("jchzalrnumimnmhp"));
    assert(!IsNiceString("haegwjzuvuyypxyu"));
    assert(!IsNiceString("dvszwmarrgswjxmb"));
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

    printf("ret1 = %d\n", ret1);
    return 0;
}