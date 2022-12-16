#include <cassert>
#include <string>
#include <map>
#include <sstream>
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <limits>
#include <queue>

struct GraphInfo {
    std::map<std::string, std::set<std::string>> graph;
    std::map<std::string, int> flows;

    void Print() const {
        for (const auto &it : graph) {
            printf("%s => [", it.first.c_str());
            for (const auto &next : it.second) {
                printf(" %s ", next.c_str());
            }
            printf("]\n");
        }
    }
};

template <typename T>
GraphInfo ParseInput(T &it) {
    GraphInfo g;
    std::string line;

    while (std::getline(it, line)) {
        char name[16];
        int64_t rate;
        sscanf(line.c_str(), "Valve %s has flow rate=%ld", name, &rate);

        g.flows[name] = rate;

        std::string tmp;
        for (char c : line.substr(line.find(';') + sizeof("tunnels lead to valves") + 1)) {
            if (c != ' ') {
                tmp.push_back(c);
            }
        }

        std::stringstream ss(tmp);
        while (std::getline(ss, tmp, ',')) {
            g.graph[name].insert(tmp);
        }
    }

    return g;
}

struct CacheKey1 {
    std::string current;
    int time;

    bool operator<(const CacheKey1 &a) const {
        return std::tie(current, time) < std::tie(a.current, a.time);
    }
};

struct State1 {
    std::string current;
    int time;
    std::set<std::string> released;
    int total_score;
};

int Problem1(const GraphInfo &g, int limit) {
    std::set<std::string> released;
    for (const auto &it : g.flows) {
        if (it.second == 0) {
            released.insert(it.first);
        }
    }

    std::map<CacheKey1, int> cache;

    std::deque<State1> q;
    q.push_back({"AA", 1, released, 0});

    int ret = 0;
    while (!q.empty()) {
        auto state = q.front();
        q.pop_front();

        const CacheKey1 key{state.current, state.time};
        if (cache.find(key) != cache.end() && cache.at(key) >= state.total_score) {
            continue;
        }
        cache[key] = state.total_score;

        if (state.time >= limit) {
            ret = std::max(ret, state.total_score);
            continue;
        }

        int score = 0;
        for (const auto &valve : state.released) {
            score += g.flows.at(valve);
        }

        if (state.released.size() == g.flows.size()) {
            int new_score = state.total_score + ((limit - state.time) * score);
            q.push_back({state.current, limit, state.released, new_score});
            continue;
        }

        int new_score = score + state.total_score;

        // case1: open valve
        if (state.released.find(state.current) == state.released.end()) {
            state.released.insert(state.current);
            q.push_back({state.current, state.time + 1, state.released, new_score + g.flows.at(state.current)});
            state.released.erase(state.current);
        }

        // case2: move valve
        for (const auto &next : g.graph.at(state.current)) {
            q.push_back({next, state.time + 1, state.released, new_score});
        }
    }

    return ret;
}

struct CacheKey2 {
    std::string current;
    std::string elephant;
    int time;

    bool operator<(const CacheKey2 &a) const {
        return std::tie(current, elephant, time) < std::tie(a.current, a.elephant, a.time);
    }
};

struct State2 {
    std::string current;
    std::string elephant;
    int time;
    std::set<std::string> released;
    int total_score;
};

int Problem2(const GraphInfo &g, int limit) {
    std::set<std::string> released;
    for (const auto &it : g.flows) {
        if (it.second == 0) {
            released.insert(it.first);
        }
    }

    std::map<CacheKey2, int> cache;

    std::deque<State2> q;
    q.push_back({"AA", "AA", 1, released, 0});

    int ret = 0;
    while (!q.empty()) {
        auto state = q.front();
        q.pop_front();

        const CacheKey2 key{state.current, state.elephant, state.time};
        if (cache.find(key) != cache.end() && cache.at(key) >= state.total_score) {
            continue;
        }
        cache[key] = state.total_score;

        if (state.time >= limit) {
            ret = std::max(ret, state.total_score);
            continue;
        }

        int score = 0;
        for (const auto &valve : state.released) {
            score += g.flows.at(valve);
        }

        if (state.released.size() == g.flows.size()) {
            int new_score = state.total_score + ((limit - state.time) * score);
            q.push_back({state.current, state.elephant, limit, state.released, new_score});
            continue;
        }

        int new_score = score + state.total_score;

        // case1: open valve
        if (state.released.find(state.current) == state.released.end()) {
            state.released.insert(state.current);

            int new_score1 = new_score + g.flows.at(state.current);

            // case1-1 elephant opens too
            if (state.released.find(state.elephant) == state.released.end()) {
                state.released.insert(state.elephant);
                int new_score2 = new_score1 + g.flows.at(state.elephant);
                q.push_back({state.current, state.elephant, state.time + 1, state.released, new_score2});
                state.released.erase(state.elephant);
            }

            // case1-2 elephant moves
            for (const auto &next : g.graph.at(state.elephant)) {
                q.push_back({state.current, next, state.time + 1, state.released, new_score1});
            }
            state.released.erase(state.current);
        }

        // case2: move valve
        for (const auto &next : g.graph.at(state.current)) {
            // case2-1 elephant opens
            if (state.released.find(state.elephant) == state.released.end()) {
                state.released.insert(state.elephant);
                int new_score1 = new_score + g.flows.at(state.elephant);
                q.push_back({next, state.elephant, state.time + 1, state.released, new_score1});
                state.released.erase(state.elephant);
            }

            // case-2-2 elephant moves too
            for (const auto &next2 : g.graph.at(state.elephant)) {
                q.push_back({next, next2, state.time + 1, state.released, new_score});
            }
        }
    }

    return ret;
}

void Test() {
    std::string input(R"(Valve AA has flow rate=0; tunnels lead to valves DD, II, BB
Valve BB has flow rate=13; tunnels lead to valves CC, AA
Valve CC has flow rate=2; tunnels lead to valves DD, BB
Valve DD has flow rate=20; tunnels lead to valves CC, AA, EE
Valve EE has flow rate=3; tunnels lead to valves FF, DD
Valve FF has flow rate=0; tunnels lead to valves EE, GG
Valve GG has flow rate=0; tunnels lead to valves FF, HH
Valve HH has flow rate=22; tunnel leads to valve GG
Valve II has flow rate=0; tunnels lead to valves AA, JJ
Valve JJ has flow rate=21; tunnel leads to valve II)");

    std::stringstream ss(input);
    auto data = ParseInput(ss);
    auto ret1 = Problem1(data, 30);
    auto ret2 = Problem2(data, 26);
    assert(ret1 == 1651);
    assert(ret2 == 1707);
}

int main() {
    Test();

    auto data = ParseInput(std::cin);
    auto ret1 = Problem1(data, 30);
    auto ret2 = Problem2(data, 26);
    std::cout << "Problem1: " << ret1 << std::endl;
    std::cout << "Problem2: " << ret2 << std::endl;
    return 0;
}
