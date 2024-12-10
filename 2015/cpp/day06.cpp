#include <cassert>
#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <fstream>
#include <array>
#include <functional>
#include <algorithm>

using namespace std;

using OpFunc = function<int(int)>;

enum class Type {
    On = 0,
    Off = 1,
    Toggle = 2,
};

struct Operation {
    Type type;
    pair<int, int> start;
    pair<int, int> end;

    explicit Operation(Type type, pair<int, int> start, pair<int, int> end)
        : type(type), start(std::move(start)), end(std::move(end)) {
    }
};

auto ParseNumbers(const string &s) -> pair<int, int> {
    stringstream ss(s);
    string tmp;
    vector<int> v;

    while (getline(ss, tmp, ',')) {
        v.push_back(stoi(tmp));
    }

    assert(v.size() == 2);

    return {v[0], v[1]};
}

auto Parse(const vector<string> &input) -> vector<Operation> {
    vector<Operation> ret;
    for (const auto &line : input) {
        stringstream ss(line);
        string tmp;
        vector<string> v;

        while (getline(ss, tmp, ' ')) {
            v.push_back(tmp);
        }

        auto type = v[1] == "on" ? Type::On : v[1] == "off" ? Type::Off : Type::Toggle;
        if (type == Type::Toggle) {
            ret.emplace_back(type, ParseNumbers(v[1]), ParseNumbers(v[3]));
        } else {
            ret.emplace_back(type, ParseNumbers(v[2]), ParseNumbers(v[4]));
        }
    }
    return ret;
}

auto CountLight(const vector<Operation> &input, const array<OpFunc, 3> &ops) -> int {
    vector<vector<int>> grid(1000, vector<int>(1000, false));

    for (const auto &op : input) {
        auto &[row1, col1] = op.start;
        auto &[row2, col2] = op.end;
        auto func = ops[static_cast<int>(op.type)];
        for (int i = row1; i <= row2; ++i) {
            for (int j = col1; j <= col2; ++j) {
                grid[i][j] = func(grid[i][j]);
            }
        }
    }

    int ret = 0;
    for (int i = 0; i < 1000; ++i) {
        for (int j = 0; j < 1000; ++j) {
            ret += grid[i][j];
        }
    }
    return ret;
}

auto Problem1(const vector<Operation> &input) -> int {
    const std::array<OpFunc, 3> ops = {
        [](int v) -> int { return 1; },
        [](int v) -> int { return 0; },
        [](int v) -> int { return v == 1 ? 0 : 1; },
    };

    return CountLight(input, ops);
}

auto Problem2(const vector<Operation> &input) -> int {
    const std::array<OpFunc, 3> ops = {
        [](int v) -> int { return v + 1; },
        [](int v) -> int { return max(v - 1, 0); },
        [](int v) -> int { return v + 2; },
    };

    return CountLight(input, ops);
}

void Test() {
    vector<Operation> input1 = {Operation(Type::On, {0, 0}, {999, 999})};
    assert(Problem1(input1) == 1'000'000);

    vector<Operation> input2 = {Operation(Type::Toggle, {0, 0}, {999, 0})};
    assert(Problem1(input2) == 1'000);

    vector<Operation> input3 = {Operation(Type::On, {0, 0}, {0, 0})};
    assert(Problem2(input3) == 1);
    vector<Operation> input4 = {Operation(Type::Toggle, {0, 0}, {999, 999})};
    assert(Problem2(input4) == 2'000'000);
}

int main() {
    Test();

    ifstream f("../input/day06.txt");
    vector<string> v;
    string tmp;
    while (getline(f, tmp)) {
        v.push_back(tmp);
    }

    auto input = Parse(v);
    int ret1 = Problem1(input);
    assert(ret1 == 400410);

    int ret2 = Problem2(input);
    assert(ret2 == 15343601);
    printf("ret1 = %d\n", ret1);
    printf("ret2 = %d\n", ret2);
    return 0;
}
