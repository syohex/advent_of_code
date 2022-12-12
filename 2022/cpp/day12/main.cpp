#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>
#include <limits>

template <typename T>
std::vector<std::vector<char>> ParseInput(T &it) {
    std::vector<std::vector<char>> ret;
    std::string line;

    while (std::getline(it, line)) {
        if (line.empty()) {
            break;
        }
        std::vector<char> tmp;
        for (char c : line) {
            tmp.push_back(c);
        }
        ret.push_back(tmp);
    }
    return ret;
}

struct Pos {
    int row;
    int col;
    int steps;

    bool operator<(const Pos &p) const {
        if (row == p.row) {
            return col < p.col;
        }

        return row < p.row;
    }
};

int Problem1(const std::vector<std::vector<char>> &data) {
    int rows = data.size();
    int cols = data[0].size();

    std::vector<std::vector<int>> min_steps(rows, std::vector<int>(cols, rows * cols + 1));
    std::queue<Pos> q;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (data[i][j] == 'S') {
                q.push({i, j, 0});
                min_steps[i][j] = 0;
                break;
            }
        }
    }

    std::vector<std::pair<int, int>> steps{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
    int ret = std::numeric_limits<int>::max();
    while (!q.empty()) {
        auto p = q.front();
        q.pop();

        if (data[p.row][p.col] == 'E') {
            ret = std::min(ret, p.steps);
            continue;
        }

        char current = data[p.row][p.col];
        current = current == 'S' ? 'a' : current;

        for (const auto &step : steps) {
            int r = p.row + step.first;
            int c = p.col + step.second;

            if (!(r >= 0 && r < rows && c >= 0 && c < cols)) {
                continue;
            }

            int next = data[r][c];
            next = next == 'E' ? 'z' : next;
            if (!((current >= next) || (current + 1 == next))) {
                continue;
            }

            if (p.steps + 1 >= min_steps[r][c]) {
                continue;
            }
            min_steps[r][c] = p.steps + 1;
            q.push({r, c, p.steps + 1});
        }
    }

    return ret;
}

void Test() {
    std::string input(R"(Sabqponm
abcryxxl
accszExk
acctuvwj
abdefghi)");
    std::stringstream ss(input);
    auto data = ParseInput(ss);

    auto ret1 = Problem1(data);
    assert(ret1 == 31);
}

int main() {
    Test();

    auto data = ParseInput(std::cin);
    auto ret1 = Problem1(data);

    std::cout << "Problem1: " << ret1 << std::endl;
    return 0;
}
