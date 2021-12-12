#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <sstream>
#include <functional>

namespace {

using Graph = std::map<std::string, std::set<std::string>>;

Graph ParseInput(std::istream &ss) {
    Graph ret;
    std::string tmp;

    while (std::getline(ss, tmp)) {
        size_t pos = tmp.find('-');
        if (pos == std::string::npos) {
            continue;
        }

        std::string p1 = tmp.substr(0, pos);
        std::string p2 = tmp.substr(pos + 1);

        ret[p1].insert(p2);
        ret[p2].insert(p1);
    }

    return ret;
}

int Part1(const Graph &graph) {
    std::function<int(const std::string &point, std::vector<std::string> &paths, std::set<std::string> &checked)> f;
    f = [&](const std::string &point, std::vector<std::string> &paths, std::set<std::string> &checked) -> int {
        if (point == "end") {
            return 1;
        }

        if (graph.find(point) == graph.end()) {
            return 0;
        }

        int ret = 0;
        for (const auto &p : graph.at(point)) {
            if (checked.find(p) != checked.end()) {
                continue;
            }

            bool is_small = p[0] >= 'a' && p[0] <= 'z';
            if (is_small) {
                checked.insert(p);
            }

            paths.push_back(p);
            ret += f(p, paths, checked);
            paths.pop_back();

            if (is_small) {
                checked.erase(p);
            }
        }

        return ret;
    };

    std::vector<std::string> paths{"start"};
    std::set<std::string> checked{"start"};
    return f("start", paths, checked);
}

void Test() {
    {
        std::string input(R"(start-A
start-b
A-c
A-b
b-d
A-end
b-end)");

        std::stringstream ss(input);
        auto data = ParseInput(ss);
        auto part1 = Part1(data);
        assert(part1 == 10);
    } // namespace

    {
        std::string input(R"(dc-end
HN-start
start-kj
dc-start
dc-HN
LN-dc
HN-end
kj-sa
kj-HN
kj-dc)");
        std::stringstream ss(input);
        auto data = ParseInput(ss);
        auto part1 = Part1(data);
        assert(part1 == 19);
    }
    {
        std::string input(R"(fs-end
he-DX
fs-he
start-DX
pj-DX
end-zg
zg-sl
zg-pj
pj-he
RW-he
fs-DX
pj-RW
zg-RW
start-pj
he-WI
zg-he
pj-fs
start-RW)");

        std::stringstream ss(input);
        auto data = ParseInput(ss);
        auto part1 = Part1(data);
        assert(part1 == 226);
    }
}

} // namespace

int main() {
    Test();

    auto graph = ParseInput(std::cin);
    auto part1 = Part1(graph);

    std::cout << "Part1: " << part1 << std::endl;
    return 0;
}