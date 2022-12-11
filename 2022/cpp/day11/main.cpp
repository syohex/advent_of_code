#include <cassert>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>
#include <queue>
#include <cstdint>
#include <functional>
#include <algorithm>
#include <numeric>

struct TestInfo {
    int64_t divisor;
    int true_id;
    int false_id;
};

struct Monkey {
    std::deque<int64_t> items;
    std::function<int64_t(int64_t)> operation;
    TestInfo test_info;
    int64_t inspects;
};

std::deque<int64_t> ParseItemLine(const std::string &s) {
    // Starting items: num1, num2...
    std::string tmp;
    for (char c : s) {
        if (c != ' ') {
            tmp.push_back(c);
        }
    }

    std::string nums = tmp.substr(tmp.find(":") + 1);
    std::deque<int64_t> ret;

    std::stringstream ss(nums);
    while (std::getline(ss, tmp, ',')) {
        ret.push_back(std::stoi(tmp));
    }

    return ret;
}

std::function<int64_t(int64_t)> ParseOperationLine(const std::string &s) {
    // Operation: new = old * 19
    // Operation: new = old * old

    char op;
    std::string buf(16, 0);
    sscanf(s.c_str(), "  Operation: new = old %c %s", &op, &buf[0]);

    if (buf.find("old") == 0) {
        if (op == '+') {
            return [](int64_t a) -> int64_t { return a + a; };
        } else {
            return [](int64_t a) -> int64_t { return a * a; };
        }
    } else {
        std::int64_t val = std::stol(buf);
        if (op == '+') {
            return [val](int64_t a) -> int64_t { return a + val; };
        } else {
            return [val](int64_t a) -> int64_t { return a * val; };
        }
    }
}

TestInfo ParseTestLines(const std::vector<std::string> &lines) {
    //  Test: divisible by 23
    //    If true: throw to monkey 2
    //    If false: throw to monkey 3

    TestInfo t;
    sscanf(lines[0].c_str(), "  Test: divisible by %ld", &t.divisor);
    sscanf(lines[1].c_str(), "    If true: throw to monkey %d", &t.true_id);
    sscanf(lines[2].c_str(), "    If false: throw to monkey %d", &t.false_id);

    return t;
}

template <typename T>
std::vector<Monkey> ParseInput(T &it) {
    std::vector<Monkey> monkeys;
    std::string line;
    std::vector<std::string> test_lines(3);

    while (std::getline(it, line)) {
        Monkey m{};
        // skip Monkey ID line

        // items line
        std::getline(it, line);
        m.items = ParseItemLine(line);

        // operation line
        std::getline(it, line);
        m.operation = ParseOperationLine(line);

        // test line
        for (int i = 0; i < 3; ++i) {
            std::getline(it, test_lines[i]);
        }

        m.test_info = ParseTestLines(test_lines);

        // skip empty line
        std::getline(it, line);

        monkeys.push_back(m);
    }

    return monkeys;
}

int64_t Problem1(const std::vector<Monkey> &data, int turns) {
    auto monkeys = data;

    for (int i = 0; i < turns; ++i) {
        for (auto &monkey : monkeys) {
            size_t len = monkey.items.size();
            monkey.inspects += len;
            for (size_t j = 0; j < len; ++j) {
                int64_t item = monkey.items.front();
                monkey.items.pop_front();

                int64_t new_level = monkey.operation(item) / 3;
                if (new_level % monkey.test_info.divisor == 0) {
                    monkeys[monkey.test_info.true_id].items.push_back(new_level);
                } else {
                    monkeys[monkey.test_info.false_id].items.push_back(new_level);
                }
            }
        }
    }

    std::vector<int64_t> inspects;
    for (const auto &monkey : monkeys) {
        inspects.push_back(monkey.inspects);
    }
    std::sort(inspects.begin(), inspects.end(), std::greater<int>{});
    return inspects[0] * inspects[1];
}

int64_t Problem2(const std::vector<Monkey> &data, int turns) {
    auto monkeys = data;
    int64_t lcm = 1;
    for (const auto &m : monkeys) {
        lcm = std::lcm(lcm, m.test_info.divisor);
    }

    for (int i = 0; i < turns; ++i) {
        for (auto &monkey : monkeys) {
            size_t len = monkey.items.size();
            monkey.inspects += len;
            for (size_t j = 0; j < len; ++j) {
                int64_t item = monkey.items.front();
                monkey.items.pop_front();

                int64_t new_level = monkey.operation(item) % lcm;
                if (new_level % monkey.test_info.divisor == 0) {
                    monkeys[monkey.test_info.true_id].items.push_back(new_level);
                } else {
                    monkeys[monkey.test_info.false_id].items.push_back(new_level);
                }
            }
        }
    }

    std::vector<int64_t> inspects;
    for (const auto &monkey : monkeys) {
        inspects.push_back(monkey.inspects);
    }

    std::sort(inspects.begin(), inspects.end(), std::greater<int>{});
    return inspects[0] * inspects[1];
}

void Test() {
    std::string input(R"(Monkey 0:
  Starting items: 79, 98
  Operation: new = old * 19
  Test: divisible by 23
    If true: throw to monkey 2
    If false: throw to monkey 3

Monkey 1:
  Starting items: 54, 65, 75, 74
  Operation: new = old + 6
  Test: divisible by 19
    If true: throw to monkey 2
    If false: throw to monkey 0

Monkey 2:
  Starting items: 79, 60, 97
  Operation: new = old * old
  Test: divisible by 13
    If true: throw to monkey 1
    If false: throw to monkey 3

Monkey 3:
  Starting items: 74
  Operation: new = old + 3
  Test: divisible by 17
    If true: throw to monkey 0
    If false: throw to monkey 1)");

    std::stringstream ss(input);
    auto data = ParseInput(ss);

    auto ret1 = Problem1(data, 20);
    assert(ret1 == 10605);

    auto ret2 = Problem2(data, 10000);
    assert(ret2 == 2713310158);
}

int main() {
    Test();

    auto data = ParseInput(std::cin);
    auto ret1 = Problem1(data, 20);
    auto ret2 = Problem2(data, 10000);

    std::cout << "Problem1: " << ret1 << std::endl;
    std::cout << "Problem2: " << ret2 << std::endl;
    return 0;
}
