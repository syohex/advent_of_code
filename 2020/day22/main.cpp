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
                break;
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
}

} // namespace

int main() {
    Test01();
    return 0;
}
