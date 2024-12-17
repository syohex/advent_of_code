#include <cassert>
#include <string>
#include <fstream>
#include <limits>

using namespace std;

auto SumNumbersInJson(const string &s, size_t &i) -> int {
    if (i >= s.size()) {
        return 0;
    }

    int ret = 0;
    switch (s[i]) {
    case '{':
        ++i;
        if (s[i] == '}') {
            ++i;
            return 0;
        }
        while (true) {
            SumNumbersInJson(s, i); // read property
            ++i;                    // colon
            ret += SumNumbersInJson(s, i);
            if (s[i] == '}') {
                ++i;
                break;
            }
            ++i; // comma
        }

        return ret;
    case '[':
        ++i;
        if (s[i] == ']') {
            ++i;
            return 0;
        }
        while (true) {
            ret += SumNumbersInJson(s, i); // read property
            if (s[i] == ']') {
                ++i;
                break;
            }
            ++i; // comma
        }
        return ret;
    case '"':
        ++i;
        while (s[i] != '\"') {
            ++i;
        }
        ++i;
        return 0;
    default: {
        int sign = 1;
        if (s[i] == '-') {
            sign = -1;
            ++i;
        }
        while (s[i] >= '0' && s[i] <= '9') {
            ret = ret * 10 + s[i] - '0';
            ++i;
        }
        return sign * ret;
    }
    }
}

auto Problem1(const string &s) -> int {
    size_t i = 0;
    return SumNumbersInJson(s, i);
}

auto SumNumbersInJsonExceptRed(const string &s, size_t &i) -> int {
    if (i >= s.size()) {
        return 0;
    }

    int ret = 0;
    switch (s[i]) {
    case '{': {
        ++i;
        if (s[i] == '}') {
            ++i;
            return 0;
        }

        bool has_red = false;
        while (true) {
            SumNumbersInJsonExceptRed(s, i); // read property
            ++i;                             // colon

            int val = SumNumbersInJsonExceptRed(s, i);
            if (val == std::numeric_limits<int>::max()) {
                has_red = true;
            } else {
                ret += val;
            }

            if (s[i] == '}') {
                ++i;
                break;
            }
            ++i; // comma
        }

        return has_red ? 0 : ret;
    }
    case '[':
        ++i;
        if (s[i] == ']') {
            ++i;
            return 0;
        }
        while (true) {
            int tmp = SumNumbersInJsonExceptRed(s, i); // read property
            if (tmp != std::numeric_limits<int>::max()) {
                ret += tmp;
            }

            if (s[i] == ']') {
                ++i;
                break;
            }
            ++i; // comma
        }
        return ret;
    case '"': {
        ++i;
        string tmp;
        while (s[i] != '\"') {
            tmp.push_back(s[i]);
            ++i;
        }
        ++i;

        if (tmp == "red") {
            return std::numeric_limits<int>::max();
        }

        return 0;
    }
    default: {
        int sign = 1;
        if (s[i] == '-') {
            sign = -1;
            ++i;
        }
        while (s[i] >= '0' && s[i] <= '9') {
            ret = ret * 10 + s[i] - '0';
            ++i;
        }
        return sign * ret;
    }
    }
}

auto Problem2(const string &s) -> int {
    size_t i = 0;
    return SumNumbersInJsonExceptRed(s, i);
}

auto Test() -> void {
    int ret1 = Problem1("[1,2,3]");
    assert(ret1 == 6);

    int ret2 = Problem1("{\"a\":2,\"b\":4}");
    assert(ret2 == 6);

    int ret3 = Problem1("[[[3]]]");
    assert(ret3 == 3);

    int ret4 = Problem1("{\"a\":{\"b\":4},\"c\":-1}");
    assert(ret4 == 3);

    int ret5 = Problem1("{\"a\":[-1,1]}");
    assert(ret5 == 0);

    int ret6 = Problem1("[-1,{\"a\":1}]");
    assert(ret6 == 0);

    int ret7 = Problem1("[]");
    assert(ret7 == 0);
    int ret8 = Problem1("{}");
    assert(ret8 == 0);

    int ret9 = Problem2("[1,2,3]");
    assert(ret9 == 6);

    int ret10 = Problem2("[1,{\"c\":\"red\",\"b\":2},3]");
    assert(ret10 == 4);

    int ret11 = Problem2("{\"d\":\"red\",\"e\":[1,2,3,4],\"f\":5}");
    assert(ret11 == 0);
    int ret12 = Problem2("[1,\"red\",5]");
    assert(ret12 == 6);
}

int main() {
    Test();

    ifstream f("../input/day12.txt");
    string s;
    f >> s;

    int ret1 = Problem1(s);
    assert(ret1 == 119433);

    int ret2 = Problem2(s);
    assert(ret2 == 68466);

    printf("ret1 = %d\n", ret1);
    printf("ret2 = %d\n", ret2);
    return 0;
}
