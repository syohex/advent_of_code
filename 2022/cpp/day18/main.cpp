#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include <set>

struct Position {
    int x;
    int y;
    int z;

    bool operator<(const Position &a) const {
        return std::tie(x, y, z) < std::tie(a.x, a.y, a.z);
    }
};

template <typename T>
std::vector<Position> ParseInput(T &it) {
    std::vector<Position> ret;
    std::string tmp;

    while (std::getline(it, tmp)) {
        std::stringstream ss(tmp);

        std::string tmp2;
        std::vector<int> nums;
        while (std::getline(ss, tmp2, ',')) {
            nums.push_back(std::stoi(tmp2));
        }

        assert(nums.size() == 3);

        ret.push_back({nums[0], nums[1], nums[2]});
    }

    return ret;
}

bool IsAdjacent(const Position &a, const Position &b) {
    return (std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z)) == 1;
}

int Problem1(const std::vector<Position> &positions) {
    int len = positions.size();
    std::vector<int> surface_count(len, 6);

    for (int i = 0; i < len - 1; ++i) {
        for (int j = i + 1; j < len; ++j) {
            if (IsAdjacent(positions[i], positions[j])) {
                --surface_count[i];
                --surface_count[j];
            }
        }
    }

    int sum = 0;
    for (int n : surface_count) {
        sum += n;
    }

    return sum;
}

struct AreaInfo {
    int min_x;
    int max_x;
    int min_y;
    int max_y;
    int min_z;
    int max_z;

    bool IsInArea(int x, int y, int z) const {
        return min_x <= x && x <= max_x && min_y <= y && y <= max_y && min_z <= z && z <= max_z;
    }
};

bool CanMoveOut(int x, int y, int z, const AreaInfo &ai, const std::set<Position> &positions, std::set<Position> &visited) {
    visited.insert({x, y, z});

    std::vector<std::tuple<int, int, int>> steps{{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}};
    for (const auto &[sx, sy, sz] : steps) {
        int xx = x + sx;
        int yy = y + sy;
        int zz = z + sz;

        if (positions.find({xx, yy, zz}) != positions.end()) {
            continue;
        }

        if (!ai.IsInArea(xx, yy, zz)) {
            return true;
        }

        if (visited.find({xx, yy, zz}) == visited.end()) {
            if (CanMoveOut(xx, yy, zz, ai, positions, visited)) {
                return true;
            }
        }
    }

    return false;
}

bool IsTrapped(int x, int y, int z, const AreaInfo &ai, const std::set<Position> &positions, std::set<Position> &cache) {
    if (positions.find({x, y, z}) != positions.end()) {
        return false;
    }

    if (cache.find({x, y, z}) != cache.end()) {
        return true;
    }

    std::set<Position> visited;
    bool canMoveOut = CanMoveOut(x, y, z, ai, positions, visited);
    if (canMoveOut) {
        return false;
    }

    for (const auto &p : visited) {
        cache.insert(p);
    }

    return true;
}

int Problem2(const std::vector<Position> &positions) {
    struct AreaInfo ai;
    ai.min_x = std::numeric_limits<int>::max();
    ai.max_x = std::numeric_limits<int>::min();
    ai.min_y = std::numeric_limits<int>::max();
    ai.max_y = std::numeric_limits<int>::min();
    ai.min_z = std::numeric_limits<int>::max();
    ai.max_z = std::numeric_limits<int>::min();

    for (const auto &p : positions) {
        ai.min_x = std::min(ai.min_x, p.x);
        ai.max_x = std::max(ai.max_x, p.x);
        ai.min_y = std::min(ai.min_y, p.y);
        ai.max_y = std::max(ai.max_y, p.y);
        ai.min_z = std::min(ai.min_z, p.z);
        ai.max_z = std::max(ai.max_z, p.z);
    }

    std::set<Position> cache;
    std::set<Position> ps(positions.begin(), positions.end());
    std::vector<Position> trapped;
    for (int x = ai.min_x; x <= ai.max_x; ++x) {
        for (int y = ai.min_y; y <= ai.max_y; ++y) {
            for (int z = ai.min_z; z <= ai.max_z; ++z) {
                if (IsTrapped(x, y, z, ai, ps, cache)) {
                    trapped.push_back({x, y, z});
                }
            }
        }
    }

    int surfaces = Problem1(positions);
    int trapped_surfaces = Problem1(trapped);
    return surfaces - trapped_surfaces;
}

void Test() {
    std::string input(R"(2,2,2
1,2,2
3,2,2
2,1,2
2,3,2
2,2,1
2,2,3
2,2,4
2,2,6
1,2,5
3,2,5
2,1,5
2,3,5)");
    std::stringstream ss(input);

    auto data = ParseInput(ss);

    int ret0 = Problem1({{1, 1, 1}, {2, 1, 1}});
    assert(ret0 == 10);

    int ret1 = Problem1(data);
    assert(ret1 == 64);

    int ret2 = Problem2(data);
    assert(ret2 == 58);
}

int main() {
    Test();

    auto data = ParseInput(std::cin);
    int ret1 = Problem1(data);
    int ret2 = Problem2(data);

    std::cout << "Problem1: " << ret1 << std::endl;
    std::cout << "Problem2: " << ret2 << std::endl;
    return 0;
}
