#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <sstream>

namespace {

template <typename T>
std::vector<std::deque<std::int64_t>> ParseInput(T &input_stream) {
    std::vector<std::deque<std::int64_t>> ret;
    std::string line;
    std::deque<std::int64_t> q;
    while (std::getline(input_stream, line)) {
        if (line.find("Player") == 0) {
            continue;
        }

        if (line.empty()) {
            ret.push_back(q);
            if (ret.size() == 2) {
                return ret;
            }

            q.clear();
            continue;
        }

        std::int64_t val = std::stoll(line);
        q.push_back(val);
    }

    if (!q.empty()) {
        ret.push_back(q);
    }

    return ret;
}

std::int64_t Solve01(std::vector<std::deque<std::int64_t>> &cards) {
    while (!cards[0].empty() && !cards[1].empty()) {
        int c1 = cards[0].front();
        int c2 = cards[1].front();

        cards[0].pop_front();
        cards[1].pop_front();
        if (c1 > c2) {
            cards[0].push_back(c1);
            cards[0].push_back(c2);
        } else {
            cards[1].push_back(c2);
            cards[1].push_back(c1);
        }
    }

    auto &winner = cards[0].empty() ? cards[1] : cards[0];
    std::int64_t ret = 0;
    size_t limit = winner.size();
    for (size_t i = 0; i < limit; ++i) {
        auto val = winner.front();
        ret += val * (limit - i);
        winner.pop_front();
    }

    return ret;
}

void Test01() {
    std::string input(R"(Player 1:
9
2
6
3
1

Player 2:
5
8
4
7
10)");

    std::istringstream ss(input);
    auto cards = ParseInput(ss);
    assert(cards.size() == 2);
    assert((cards[0] == std::deque<std::int64_t>{9, 2, 6, 3, 1}));
    assert((cards[1] == std::deque<std::int64_t>{5, 8, 4, 7, 10}));

    assert(Solve01(cards) == 306);
}

} // namespace

int main() {
    Test01();

    auto cards = ParseInput(std::cin);
    assert(cards.size() == 2);

    std::cout << "Part01: " << Solve01(cards) << std::endl;
    return 0;
}
