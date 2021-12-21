#include <iostream>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <sstream>
#include <map>
#include <tuple>
#include <functional>

namespace {

struct Position {
    int p1;
    int p2;
};

Position ParseInput(const std::string &input) {
    int p1, p2;

    std::stringstream ss(input);
    std::string tmp;

    std::getline(ss, tmp);
#if _WIN32
    sscanf_s(tmp.c_str(), "Player 1 starting position: %d", &p1);
#else
    sscanf(tmp.c_str(), "Player 1 starting position: %d", &p1);
#endif
    std::getline(ss, tmp);
#if _WIN32
    sscanf_s(tmp.c_str(), "Player 2 starting position: %d", &p2);
#else
    sscanf(tmp.c_str(), "Player 2 starting position: %d", &p2);
#endif
    return Position{p1, p2};
}

std::int64_t Part1(const Position &p) {
    int p1 = p.p1;
    int p2 = p.p2;
    int dice = 1;

    std::int64_t score1 = 0, score2 = 0;
    std::int64_t count = 0;
    std::int64_t ret = 0;

    const auto roll_dice = [](int &dice) {
        ++dice;
        if (dice >= 101) {
            dice = 1;
        }
    };

    while (true) {
        for (int i = 0; i < 3; ++i) {
            p1 += dice;
            roll_dice(dice);
        }
        count += 3;

        int mod = p1 % 10;
        p1 = mod == 0 ? 10 : mod;
        score1 += p1;

        if (score1 >= 1000) {
            ret = score2 * count;
            break;
        }

        for (int i = 0; i < 3; ++i) {
            p2 += dice;
            roll_dice(dice);
        }
        count += 3;

        mod = p2 % 10;
        p2 = mod == 0 ? 10 : mod;
        score2 += p2;

        if (score2 >= 1000) {
            ret = score1 * count;
            break;
        }
    }

    return ret;
}

struct State {
    int p1;
    int p2;
    int score1;
    int score2;
    bool is_player1;

    bool operator<(const State &a) const {
        auto v1 = std::tie(p1, p2, score1, score2, is_player1);
        auto v2 = std::tie(a.p1, a.p2, a.score1, a.score2, a.is_player1);
        return v1 < v2;
    }
};

std::map<int, int> AllSteps() {
    std::map<int, int> ret;
    std::function<void(std::vector<int> & acc)> f;
    f = [&f, &ret](std::vector<int> &acc) {
        if (acc.size() == 3) {
            int sum = 0;
            for (int i : acc) {
                sum += i;
            }
            ++ret[sum];
            return;
        }

        for (int i = 1; i <= 3; ++i) {
            acc.push_back(i);
            f(acc);
            acc.pop_back();
        }
    };

    std::vector<int> acc;
    f(acc);
    return ret;
}

std::int64_t Part2(const Position &p) {
    auto steps = AllSteps();

    std::map<State, std::int64_t> m;
    m.insert({State{p.p1, p.p2, 0, 0, true}, 1});

    constexpr int GOAL_SCORE = 21;
    std::int64_t ret = 0;
    while (!m.empty()) {
        std::map<State, std::int64_t> tmp;
        for (const auto &it : m) {
            const auto &prev_state = it.first;
            auto state_count = it.second;

            for (const auto &it : steps) {
                State s = prev_state;
                int step = it.first;
                int step_count = it.second;

                s.is_player1 = !prev_state.is_player1;
                if (prev_state.is_player1) {
                    s.p1 += step;
                    if (s.p1 > 10) {
                        s.p1 -= 10;
                    }
                    s.score1 += s.p1;
                    if (s.score1 >= GOAL_SCORE) {
                        ret += step_count * state_count;
                        continue;
                    }
                } else {
                    s.p2 += step;
                    if (s.p2 > 10) {
                        s.p2 -= 10;
                    }
                    s.score2 += s.p2;
                    if (s.score2 >= GOAL_SCORE) {
                        continue;
                    }
                }

                tmp[s] += step_count * state_count;
            }
        }

        m = tmp;
    }

    return ret;
}

void Test() {
    std::string input(R"(Player 1 starting position: 4
Player 2 starting position: 8)");
    auto position = ParseInput(input);
    auto part1 = Part1(position);
    assert(part1 == 739785);

    auto part2 = Part2(position);
    assert(part2 == 444356092776315);
}

} // namespace

int main() {
    Test();
    std::string input(R"(Player 1 starting position: 4
Player 2 starting position: 5)");
    auto position = ParseInput(input);
    auto part1 = Part1(position);
    auto part2 = Part2(position);

    std::cout << "Part1: " << part1 << std::endl;
    std::cout << "Part2: " << part2 << std::endl;

    return 0;
}