#include <cassert>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

auto HasStraight(const vector<int> &v) -> bool {
    for (size_t i = 2; i < v.size(); ++i) {
        if (v[i - 2] - 2 == v[i] && v[i - 1] - 1 == v[i]) {
            return true;
        }
    }
    return false;
}

auto HasInvalidValue(const vector<int> &v) -> bool {
    int i_num = 'i' - 'a';
    int o_num = 'o' - 'a';
    int l_num = 'l' - 'a';
    for (int n : v) {
        if (n == i_num || n == o_num || n == l_num) {
            return true;
        }
    }

    return false;
}

auto HasTwoDifferntPairs(const vector<int> &v) -> bool {
    int count = 0;
    for (int c = 0; c < 26; ++c) {
        bool ok = false;
        for (size_t i = 1; i < v.size(); ++i) {
            if (v[i - 1] == c && v[i] == c) {
                ok = true;
                break;
            }
        }
        if (ok) {
            ++count;
            if (count == 2) {
                return true;
            }
        }
    }

    return false;
}

auto IsValidPassword(const vector<int> &v) -> bool {
    return HasStraight(v) && !HasInvalidValue(v) && HasTwoDifferntPairs(v);
}

auto Increment(vector<int> &v) -> void {
    ++v[0];

    int carry = 0;
    for (size_t i = 0; i < v.size(); ++i) {
        v[i] += carry;

        if (v[i] >= 26) {
            v[i] = 0;
            carry = 1;
        } else {
            carry = 0;
            break;
        }
    }

    if (carry == 1) {
        v.push_back(1);
    }
}

auto ToString(const vector<int> &v) -> string {
    string ret;
    for (int n : v) {
        ret.push_back(static_cast<char>(n + 'a'));
    }
    reverse(ret.begin(), ret.end());
    return ret;
}

auto Problem1(const string &input) -> string {
    vector<int> v;
    for (char c : input) {
        v.push_back(c - 'a');
    }
    reverse(v.begin(), v.end());

    while (true) {
        Increment(v);

        if (IsValidPassword(v)) {
            break;
        }
    }

    return ToString(v);
}

auto Test() -> void {
    auto ret1 = Problem1("abcdefgh");
    assert(ret1 == "abcdffaa");

    auto ret2 = Problem1("ghijklmn");
    assert(ret2 == "ghjaabcc");
}

int main() {
    Test();

    const string input = "hepxcrrq";
    auto ret1 = Problem1(input);
    assert(ret1 == "hepxxyzz");

    auto ret2 = Problem1(ret1);
    assert(ret2 == "heqaabcc");

    printf("ret1 = %s\n", ret1.data());
    printf("ret2 = %s\n", ret2.data());
    return 0;
}
