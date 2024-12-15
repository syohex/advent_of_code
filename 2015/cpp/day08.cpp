#include <cassert>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

auto Problem1(const vector<string> &input) -> int {
    int ret = 0;
    for (const auto &s : input) {
        int len = s.size();
        int i = 0;
        int chars = -2; // two double quotes
        while (i < len) {
            switch (s[i]) {
            case '\\':
                ++i;
                switch (s[i]) {
                case '\\':
                    ++i;
                    break;
                case '"':
                    ++i;
                    break;
                case 'x':
                    i += 3;
                    break;
                }
                break;
            default:
                ++i;
                break;
            }
            ++chars;
        }

        ret += len - chars;
    }
    return ret;
}

auto Problem2(const vector<string> &input) -> int {
    int ret = 0;
    for (const auto &s : input) {
        int len = s.size();
        int chars = 2; // two double quotes
        for (char c : s) {
            switch (c) {
            case '"':
            case '\\':
                chars += 2;
                break;
            default:
                chars += 1;
                break;
            }
        }

        ret += chars - len;
    }
    return ret;
}

auto Test() -> void {
    vector<string> input{
        "\"\"",
        "\"abc\"",
        "\"aaa\\\"aaa\"",
        "\"\\x27\"",
    };

    int ret1 = Problem1(input);
    assert(ret1 == 12);

    int ret2 = Problem2(input);
    assert(ret2 == 19);
}

int main() {
    Test();

    ifstream f("../input/day08.txt");
    vector<string> input;
    string tmp;
    while (getline(f, tmp)) {
        input.push_back(tmp);
    }

    int ret1 = Problem1(input);
    assert(ret1 == 1371);

    int ret2 = Problem2(input);
    assert(ret2 == 2117);

    printf("ret1 = %d\n", ret1);
    printf("ret2 = %d\n", ret2);
    return 0;
}
