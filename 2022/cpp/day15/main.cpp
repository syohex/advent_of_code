#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include <set>
#include <map>
#include <cmath>
#include <algorithm>

struct Sensor {
    int row;
    int col;

    int beacon_row;
    int beacon_col;

    int BeaconDistance() const {
        return std::abs(row - beacon_row) + std::abs(col - beacon_col);
    }
};

template <typename T>
std::vector<Sensor> ParseInput(T &it) {
    std::vector<Sensor> ret;
    std::string line;
    while (std::getline(it, line)) {
        Sensor s;
        sscanf(line.c_str(), "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &s.col, &s.row, &s.beacon_col, &s.beacon_row);
        ret.push_back(s);
    }

    return ret;
}

struct Line {
    int start;
    int end;
};

int Problem1(const std::vector<Sensor> &data, int row) {
    std::map<int, std::vector<Line>> not_exist;

    for (const auto &sensor : data) {
        int distance = sensor.BeaconDistance();
        for (int row = sensor.row - distance; row <= sensor.row + distance; ++row) {
            int d = std::abs(row - sensor.row);
            int len = std::abs(distance - d);
            not_exist[row].push_back({sensor.col - len, sensor.col + len});
        }
    }

    const auto cmp = [](const Line &a, const Line &b) {
        if (a.start == b.start) {
            return a.end < b.end;
        }

        return a.start < b.start;
    };

    auto lines = not_exist[row];
    std::sort(lines.begin(), lines.end(), cmp);

    int len = lines.size();
    int range_start = lines[0].start;
    int range_end = lines[0].end;

    std::vector<Line> ranges;
    for (int i = 1; i < len; ++i) {
        if (lines[i].start <= range_end) {
            range_end = std::max(range_end, lines[i].end);
        } else {
            ranges.push_back({range_start, range_end});
            range_start = lines[i].start;
            range_end = lines[i].end;
        }
    }
    ranges.push_back({range_start, range_end});

    std::set<int> beacon_cols;
    for (const auto &sensor : data) {
        if (sensor.beacon_row == row) {
            beacon_cols.insert(sensor.beacon_col);
        }
    }

    int ret = 0;
    for (const auto &range : ranges) {
        ret += range.end - range.start + 1;
    }

    for (int col : beacon_cols) {
        for (const auto &range : ranges) {
            if (range.start <= col && col <= range.end) {
                --ret;
                break;
            }
        }
    }

    return ret;
}

void Test() {
    std::string input(R"(Sensor at x=2, y=18: closest beacon is at x=-2, y=15
Sensor at x=9, y=16: closest beacon is at x=10, y=16
Sensor at x=13, y=2: closest beacon is at x=15, y=3
Sensor at x=12, y=14: closest beacon is at x=10, y=16
Sensor at x=10, y=20: closest beacon is at x=10, y=16
Sensor at x=14, y=17: closest beacon is at x=10, y=16
Sensor at x=8, y=7: closest beacon is at x=2, y=10
Sensor at x=2, y=0: closest beacon is at x=2, y=10
Sensor at x=0, y=11: closest beacon is at x=2, y=10
Sensor at x=20, y=14: closest beacon is at x=25, y=17
Sensor at x=17, y=20: closest beacon is at x=21, y=22
Sensor at x=16, y=7: closest beacon is at x=15, y=3
Sensor at x=14, y=3: closest beacon is at x=15, y=3
Sensor at x=20, y=1: closest beacon is at x=15, y=3)");

    std::stringstream ss(input);
    auto data = ParseInput(ss);
    int ret1 = Problem1(data, 10);
    assert(ret1 == 26);
}

int main() {
    Test();

    auto data = ParseInput(std::cin);
    int ret1 = Problem1(data, 2000000);

    std::cout << "Problem1: " << ret1 << std::endl;
    return 0;
}
