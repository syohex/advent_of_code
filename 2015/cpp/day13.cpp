#include <cassert>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <functional>
#include <set>
#include <limits>
#include <fstream>

using namespace std;

using Graph = map<string, map<string, int>>;

auto Parse(const vector<string> &input) -> Graph {
    Graph ret;
    for (const auto &line : input) {
        stringstream ss(line);
        vector<string> parts;
        string tmp;

        while (getline(ss, tmp, ' ')) {
            parts.push_back(tmp);
        }
        parts.back().pop_back(); // remove last period

        const auto &from = parts[0];
        const auto &verb = parts[2];
        int score = stoi(parts[3]) * (verb == "gain" ? 1 : -1);
        const auto &to = parts.back();

        ret[from][to] = score;
    }

    return ret;
}

auto DFS(const string &node, const Graph &graph, set<string> &visited, int total_score) -> int {
    visited.insert(node);

    if (visited.size() == graph.size()) {
        visited.erase(node);
        return total_score + graph.at(node).at("Alice") + graph.at("Alice").at(node);
    }

    int ret = numeric_limits<int>::min();
    for (const auto &[next, score] : graph.at(node)) {
        if (!visited.contains(next)) {
            int new_score = total_score + score + graph.at(next).at(node);
            ret = max(ret, DFS(next, graph, visited, new_score));
        }
    }
    visited.erase(node);

    return ret;
}

auto Problem1(const Graph &graph) -> int {
    set<string> visited;
    return DFS("Alice", graph, visited, 0);
}

auto Test() -> void {
    vector<string> input = {"Alice would gain 54 happiness units by sitting next to Bob.",
                            "Alice would lose 79 happiness units by sitting next to Carol.",
                            "Alice would lose 2 happiness units by sitting next to David.",
                            "Bob would gain 83 happiness units by sitting next to Alice.",
                            "Bob would lose 7 happiness units by sitting next to Carol.",
                            "Bob would lose 63 happiness units by sitting next to David.",
                            "Carol would lose 62 happiness units by sitting next to Alice.",
                            "Carol would gain 60 happiness units by sitting next to Bob.",
                            "Carol would gain 55 happiness units by sitting next to David.",
                            "David would gain 46 happiness units by sitting next to Alice.",
                            "David would lose 7 happiness units by sitting next to Bob.",
                            "David would gain 41 happiness units by sitting next to Carol."};

    Graph g = Parse(input);
    int ret1 = Problem1(g);
    assert(ret1 == 330);
}

int main() {
    Test();

    fstream f("../input/day13.txt");
    vector<string> input;
    string tmp;
    while (getline(f, tmp)) {
        input.push_back(tmp);
    }

    const auto graph = Parse(input);
    int ret1 = Problem1(graph);
    assert(ret1 == 664);

    auto graph2 = graph;
    vector<string> names;
    for (auto &[k, v] : graph2) {
        v["myself"] = 0;
        names.push_back(k);
    }

    for (const auto &name : names) {
        graph2["myself"][name] = 0;
    }

    int ret2 = Problem1(graph2);
    assert(ret2 == 640);

    printf("ret1 = %d\n", ret1);
    printf("ret2 = %d\n", ret2);
    return 0;
}