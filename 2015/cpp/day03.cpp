#include <cassert>
#include <string>
#include <set>
#include <fstream>
#include <cstdio>

using namespace std;

auto Problem1(const std::string &input) -> int {
    set<pair<int, int>> s;
    s.insert({0, 0});

    int row = 0;
    int col = 0;
    for (char c : input) {
        switch (c) {
        case '^':
            row -= 1;
            break;
        case '>':
            col += 1;
            break;
        case 'v':
            row += 1;
            break;
        case '<':
            col -= 1;
            break;
        default:
            assert(!"never reach here");
            break;
        }
        s.insert({row, col});
    }

    return s.size();
}

auto Problem2(const std::string &input) -> int {
    set<pair<int, int>> s;
    s.insert({0, 0});

    int rows[2] = {0, 0};
    int cols[2] = {0, 0};
    int index = 0;
    for (char c : input) {
        switch (c) {
        case '^':
            rows[index] -= 1;
            break;
        case '>':
            cols[index] += 1;
            break;
        case 'v':
            rows[index] += 1;
            break;
        case '<':
            cols[index] -= 1;
            break;
        default:
            assert(!"never reach here");
            break;
        }

        s.insert({rows[index], cols[index]});
        index = index == 0 ? 1 : 0;
    }

    return s.size();
}

void Test() {
    assert(Problem1(">") == 2);
    assert(Problem1("^>v<") == 4);
    assert(Problem1("^v^v^v^v^v") == 2);

    assert(Problem2("^v") == 3);
    assert(Problem2("^>v<") == 3);
    assert(Problem2("^v^v^v^v^v") == 11);
}

int main() {
    Test();

    ifstream f("../input/day03.txt");
    string input;
    f >> input;

    int ret1 = Problem1(input);
    assert(ret1 == 2572);

    int ret2 = Problem2(input);
    assert(ret2 == 2631);

    printf("ret1 = %d\n", ret1);
    printf("ret2 = %d\n", ret2);
    return 0;
}
