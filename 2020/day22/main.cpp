#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <sstream>
#include <set>

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

std::int64_t CalculateScore(std::deque<std::int64_t> &winner) {
    std::int64_t ret = 0;
    size_t limit = winner.size();
    for (size_t i = 0; i < limit; ++i) {
        auto val = winner.front();
        ret += val * (limit - i);
        winner.pop_front();
    }

    return ret;
}

std::int64_t Solve01(std::deque<std::int64_t> &player1, std::deque<std::int64_t> &player2, int sub = 0) {
    while (!player1.empty() && !player2.empty()) {
        auto c1 = player1.front();
        auto c2 = player2.front();

        player1.pop_front();
        player2.pop_front();
        if (c1 > c2) {
            player1.push_back(c1);
            player1.push_back(c2);
        } else {
            player2.push_back(c2);
            player2.push_back(c1);
        }
    }

    auto &winner = player1.empty() ? player2 : player1;
    return CalculateScore(winner);
}

void WithSubGame(std::deque<std::int64_t> &player1, std::deque<std::int64_t> &player2) {
    std::set<std::vector<std::deque<std::int64_t>>> s;

    while (!player1.empty() && !player2.empty()) {
        auto key = std::vector<std::deque<std::int64_t>>{player1, player2};
        if (s.find(key) != s.end()) {
            player2.clear();
            return;
        }

        s.insert(key);
        auto c1 = player1.front();
        auto c2 = player2.front();

        player1.pop_front();
        player2.pop_front();

        bool player1_win;
        if (player1.size() >= static_cast<size_t>(c1) && player2.size() >= static_cast<size_t>(c2)) {
            // sub game
            std::deque<std::int64_t> subdeck1 = player1;
            std::deque<std::int64_t> subdeck2 = player2;
            while (subdeck1.size() > static_cast<size_t>(c1)) {
                subdeck1.pop_back();
            }
            while (subdeck2.size() > static_cast<size_t>(c2)) {
                subdeck2.pop_back();
            }

            WithSubGame(subdeck1, subdeck2);
            if (subdeck1.empty()) {
                player1_win = false;
            } else {
                player1_win = true;
            }
        } else {
            player1_win = c1 > c2;
        }

        if (player1_win) {
            player1.push_back(c1);
            player1.push_back(c2);
        } else {
            player2.push_back(c2);
            player2.push_back(c1);
        }
    }
}

std::int64_t Solve02(std::deque<std::int64_t> &player1, std::deque<std::int64_t> &player2) {
    WithSubGame(player1, player2);

    auto &winner = player1.empty() ? player2 : player1;
    return CalculateScore(winner);
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
    auto cards01 = ParseInput(ss);
    auto cards02 = cards01;

    assert(cards01.size() == 2);
    assert((cards01[0] == std::deque<std::int64_t>{9, 2, 6, 3, 1}));
    assert((cards01[1] == std::deque<std::int64_t>{5, 8, 4, 7, 10}));

    assert(Solve01(cards01[0], cards01[1]) == 306);
    assert(Solve02(cards02[0], cards02[1]) == 291);
}

} // namespace

int main() {
    Test01();

    auto cards01 = ParseInput(std::cin);
    assert(cards01.size() == 2);
    auto cards02 = cards01;

    std::cout << "Part01: " << Solve01(cards01[0], cards01[1]) << std::endl;
    std::cout << "Part02: " << Solve02(cards02[0], cards02[1]) << std::endl;
    return 0;
}
