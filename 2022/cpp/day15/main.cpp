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
#include <limits>

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

struct Range {
    int start;
    int end;

    bool operator<(const Range &other) const {
        if (start == other.start) {
            return end < other.end;
        }

        return start < other.start;
    }
};

std::map<int, std::vector<Range>> FindNonBeaconRanges(const std::vector<Sensor> &sensors) {
    std::map<int, std::vector<Range>> non_beacon_range;
    for (const auto &sensor : sensors) {
        int distance = sensor.BeaconDistance();
        for (int row = sensor.row - distance; row <= sensor.row + distance; ++row) {
            int d = std::abs(row - sensor.row);
            int len = std::abs(distance - d);
            non_beacon_range[row].push_back({sensor.col - len, sensor.col + len});
        }
    }

    std::map<int, std::vector<Range>> ret;
    for (const auto &it : non_beacon_range) {
        int row = it.first;
        std::vector<Range> ranges = it.second;
        std::sort(ranges.begin(), ranges.end());

        std::vector<Range> optimized_ranges;
        int range_start = ranges[0].start;
        int range_end = ranges[0].end;

        for (size_t i = 1; i < ranges.size(); ++i) {
            if (ranges[i].start <= range_end + 1) {
                range_end = std::max(range_end, ranges[i].end);
            } else {
                optimized_ranges.push_back({range_start, range_end});
                range_start = ranges[i].start;
                range_end = ranges[i].end;
            }
        }

        optimized_ranges.push_back({range_start, range_end});
        ret[row] = optimized_ranges;
    }

    return ret;
}

int Problem1(const std::vector<Sensor> &sensors, int row) {
    const auto non_beacon_range = FindNonBeaconRanges(sensors);

    std::set<int> beacon_cols;
    for (const auto &sensor : sensors) {
        if (sensor.beacon_row == row) {
            beacon_cols.insert(sensor.beacon_col);
        }
    }

    const std::vector<Range> &ranges = non_beacon_range.at(row);

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

std::int64_t Problem2(const std::vector<Sensor> &sensors) {
    const auto non_beacon_range = FindNonBeaconRanges(sensors);

    int max_sensor_row = std::numeric_limits<int>::min();
    int max_sensor_col = std::numeric_limits<int>::min();
    for (const auto &sensor : sensors) {
        max_sensor_row = std::max(max_sensor_row, sensor.row);
        max_sensor_col = std::max(max_sensor_col, sensor.col);
    }

    for (const auto &it : non_beacon_range) {
        int row = it.first;
        const auto &ranges = it.second;
        if (row < 0 || row > max_sensor_row) {
            continue;
        }
        if (ranges.size() < 2) {
            continue;
        }

        for (const auto &range : ranges) {
            if (range.start <= 0 && range.end >= max_sensor_col) {
                continue;
            }

            return (static_cast<int64_t>(range.end + 1) * 4000000) + row;
        }
    }

    // never reach here
    return std::numeric_limits<int>::min();
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
    auto ret2 = Problem2(data);
    assert(ret1 == 26);
    assert(ret2 == 56000011);
}

int main() {
    Test();

    auto data = ParseInput(std::cin);
    int ret1 = Problem1(data, 2000000);
    auto ret2 = Problem2(data);

    std::cout << "Problem1: " << ret1 << std::endl;
    std::cout << "Problem2: " << ret2 << std::endl;
    return 0;
}
