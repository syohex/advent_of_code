#include <cassert>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <set>
#include <algorithm>
#include <limits>
#include <fstream>

using namespace std;

using DistanceInfo = pair<string, int>;
using Graph = map<string, vector<DistanceInfo>>;
using CmpFunc = auto(int, int) -> int;

auto SplitString(const string &s) -> vector<string> {
    stringstream ss(s);
    vector<string> ret;
    string tmp;

    while (getline(ss, tmp, ' ')) {
        ret.push_back(tmp);
    }

    return ret;
}

auto Parse(const vector<string> &input) -> Graph {
    Graph graph;
    for (const auto &s : input) {
        const auto words = SplitString(s);
        graph[words[0]].push_back({words[2], stoi(words[4])});
        graph[words[2]].push_back({words[0], stoi(words[4])});
    }

    return graph;
}

auto GetDistance(const string &location, set<string> &visited, int base, const CmpFunc cmp, const Graph &graph) {
    if (visited.size() == graph.size()) {
        return 0;
    }

    int ret = base;
    for (const auto &[next, distance] : graph.at(location)) {
        if (!visited.contains(next)) {
            visited.insert(next);
            ret = cmp(ret, distance + GetDistance(next, visited, base, cmp, graph));
            visited.erase(next);
        }
    }

    return ret;
}

auto Problem1(const Graph &graph) -> int {
    int ret = numeric_limits<int>::max();
    const auto cmp = [](int a, int b) -> int { return min(a, b); };
    int base = numeric_limits<int>::max();
    for (const auto &it : graph) {
        set<string> visited;
        visited.insert(it.first);
        ret = min(ret, GetDistance(it.first, visited, base, cmp, graph));
    }

    return ret;
}

auto Problem2(const Graph &graph) -> int {
    int ret = 0;
    const auto cmp = [](int a, int b) -> int { return max(a, b); };
    for (const auto &it : graph) {
        set<string> visited;
        visited.insert(it.first);
        ret = max(ret, GetDistance(it.first, visited, 0, cmp, graph));
    }

    return ret;
}

auto Test() -> void {
    vector<string> input{
        "London to Dublin = 464",
        "London to Belfast = 518",
        "Dublin to Belfast = 141",
    };

    const auto &graph = Parse(input);
    int ret1 = Problem1(graph);
    assert(ret1 == 605);

    int ret2 = Problem2(graph);
    assert(ret2 == 982);
}

int main() {
    Test();

    ifstream f("../input/day09.txt");
    vector<string> input;
    string tmp;
    while (getline(f, tmp)) {
        input.push_back(tmp);
    }

    const auto graph = Parse(input);
    int ret1 = Problem1(graph);
    assert(ret1 == 251);

    int ret2 = Problem2(graph);
    assert(ret2 == 898);

    printf("ret1 = %d\n", ret1);
    printf("ret2 = %d\n", ret2);
    return 0;
}
