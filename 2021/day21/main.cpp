#include <iostream>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <sstream>
#include <map>
#include <tuple>
#include <functional>
#include <algorithm>

namespace {

struct Player {
    int position;
    int score;

    explicit Player(int position) : position(position), score(0) {
    }

    bool operator<(const Player &other) const {
        return std::tie(position, score) < std::tie(other.position, other.score);
    }
};

struct GameState {
    Player player1;
    Player player2;
    bool is_player1;

    explicit GameState(const Player &p1, const Player &p2) : player1(p1), player2(p2), is_player1(true) {
    }

    bool operator<(const GameState &other) const {
        return std::tie(player1, player2, is_player1) < std::tie(other.player1, other.player2, other.is_player1);
    }
};

GameState ParseInput(const std::string &input) {
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
    return GameState(Player(p1), Player(p2));
}

std::int64_t Part1(const GameState &init_state) {
    GameState state = init_state;
    int dice = 1;
    std::int64_t roll_count = 0;

    while (true) {
        auto &current_player = state.is_player1 ? state.player1 : state.player2;
        auto &other_player = state.is_player1 ? state.player2 : state.player1;

        int position = current_player.position;
        for (int i = 0; i < 3; ++i) {
            position += dice;
            ++dice;
            if (dice >= 101) {
                dice = 1;
            }
        }
        roll_count += 3;

        int mod = position % 10;
        position = mod == 0 ? 10 : mod;

        current_player.score += position;
        current_player.position = position;

        if (current_player.score >= 1000) {
            return other_player.score * roll_count;
        }

        state.is_player1 = !state.is_player1;
    }

    assert(!"never reach here");
    return -1;
}

std::map<int, int> AllSteps() {
    std::map<int, int> ret;
    std::function<void(int count, int acc)> f;
    f = [&f, &ret](int count, int acc) {
        if (count == 3) {
            ++ret[acc];
            return;
        }

        for (int i = 1; i <= 3; ++i) {
            f(count + 1, acc + i);
        }
    };

    f(0, 0);
    return ret;
}

std::int64_t Part2(const GameState &init_state) {
    auto steps = AllSteps();

    std::map<GameState, std::int64_t> all_state;
    all_state.insert({init_state, 1});

    constexpr int GOAL_SCORE = 21;
    std::int64_t player1_wins = 0;
    std::int64_t player2_wins = 0;
    while (!all_state.empty()) {
        decltype(all_state) tmp;
        for (const auto &it : all_state) {
            const auto &current_state = it.first;
            auto state_count = it.second;

            for (const auto &it : steps) {
                int step = it.first;
                int step_count = it.second;

                GameState next_state = current_state;
                next_state.is_player1 = !current_state.is_player1;

                Player &player = current_state.is_player1 ? next_state.player1 : next_state.player2;
                player.position += step;
                if (player.position > 10) {
                    player.position -= 10;
                }

                player.score += player.position;

                std::int64_t universes = step_count * state_count;
                if (player.score >= GOAL_SCORE) {
                    if (current_state.is_player1) {
                        player1_wins += universes;
                    } else {
                        player2_wins += universes;
                    }
                    continue;
                }

                tmp[next_state] += universes;
            }
        }

        all_state = tmp;
    }

    return std::max(player1_wins, player2_wins);
}

void Test() {
    std::string input(R"(Player 1 starting position: 4
Player 2 starting position: 8)");
    const auto init_state = ParseInput(input);
    auto part1 = Part1(init_state);
    assert(part1 == 739785);

    auto part2 = Part2(init_state);
    assert(part2 == 444356092776315);
}

} // namespace

int main() {
    Test();
    std::string input(R"(Player 1 starting position: 4
Player 2 starting position: 5)");
    const auto init_state = ParseInput(input);
    auto part1 = Part1(init_state);
    auto part2 = Part2(init_state);

    std::cout << "Part1: " << part1 << std::endl;
    std::cout << "Part2: " << part2 << std::endl;

    assert(part1 == 864900);
    assert(part2 == 575111835924670);
    return 0;
}