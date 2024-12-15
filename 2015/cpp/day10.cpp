#include <cassert>
#include <string>
#include <vector>

using namespace std;

int RunLength(const string &input, int count) {
    string s = input;
    for (int i = 0; i < count; ++i) {
        vector<pair<int, char>> v;
        int n = 1;
        char prev = s[0];
        for (size_t i = 1; i < s.size(); ++i) {
            if (s[i] != prev) {
                v.push_back({n, prev});
                prev = s[i];
                n = 1;
            } else {
                ++n;
            }
        }

        v.push_back({n, prev});
        s.clear();

        for (const auto &[n, c] : v) {
            for (char d : to_string(n)) {
                s.push_back(d);
            }
            s.push_back(c);
        }
    }

    return s.size();
}

auto Test() -> void {
    int ret1 = RunLength("1", 5);
    assert(ret1 == 6);
}

int main() {
    Test();

    const string input = "3113322113";
    int ret1 = RunLength(input, 40);
    assert(ret1 == 329356);

    int ret2 = RunLength(input, 50);
    assert(ret2 == 4666278);

    printf("ret1 = %d\n", ret1);
    printf("ret2 = %d\n", ret2);
    return 0;
}
