#include <cassert>
#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <set>
#include <optional>
#include <cmath>

namespace {

struct Position {
    int x;
    int y;
    int z;

    explicit Position(int x, int y, int z) : x(x), y(y), z(z) {
    }

    bool operator<(const Position &p) const {
        if (x != p.x) {
            return x < p.x;
        }
        if (y != p.y) {
            return y < p.y;
        }
        return z < p.z;
    };

    bool operator==(const Position &p) const {
        return x == p.x && y == p.y && z == p.z;
    }
};

using ScannerData = std::set<Position>;

std::vector<ScannerData> ParseInput(std::istream &ss) {
    std::vector<ScannerData> ret;
    std::string tmp;

    while (std::getline(ss, tmp)) { // skip label
        ScannerData sd;
        int x, y, z;
        while (std::getline(ss, tmp)) {
            if (tmp.empty()) {
                break;
            }
#if _WIN32
            sscanf_s(tmp.c_str(), "%d,%d,%d", &x, &y, &z);
#else
            sscanf(tmp.c_str(), "%d,%d,%d", &x, &y, &z);
#endif
            sd.emplace(x, y, z);
        }

        ret.push_back(sd);
    }

    return ret;
}

std::vector<ScannerData> AllOrientations(const ScannerData &scanner_data) {
    std::vector<ScannerData> ret;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                std::vector<ScannerData> s(6);
                for (const auto &beacon : scanner_data) {
                    int x = i == 0 ? -beacon.x : beacon.x;
                    int y = j == 0 ? -beacon.y : beacon.y;
                    int z = k == 0 ? -beacon.z : beacon.z;

                    s[0].emplace(x, y, z);
                    s[1].emplace(x, z, y);
                    s[2].emplace(y, x, z);
                    s[3].emplace(y, z, x);
                    s[4].emplace(z, x, y);
                    s[5].emplace(z, y, x);
                }

                std::copy(s.begin(), s.end(), std::back_inserter(ret));
            }
        }
    }

    return ret;
}

std::optional<Position> FindScannerPosition(const ScannerData &scanner0, const ScannerData &other_scanner) {
    for (const auto &beacon0 : scanner0) {
        std::set<Position> distance0;
        for (const auto &b : scanner0) {
            distance0.emplace(beacon0.x - b.x, beacon0.y - b.y, beacon0.z - b.z);
        }

        for (const auto &beacon1 : other_scanner) {
            std::set<Position> distance1;
            for (const auto &b : other_scanner) {
                distance1.emplace(beacon1.x - b.x, beacon1.y - b.y, beacon1.z - b.z);
            }

            int count = 0;
            for (const auto &dist : distance1) {
                if (distance0.find(dist) != distance0.end()) {
                    ++count;
                    if (count >= 12) {
                        int scanner_x = beacon0.x - beacon1.x;
                        int scanner_y = beacon0.y - beacon1.y;
                        int scanner_z = beacon0.z - beacon1.z;
                        return Position{scanner_x, scanner_y, scanner_z};
                    }
                }
            }
        }
    }

    return std::nullopt;
}

void MergeScannerData(ScannerData &base, const ScannerData &other, const Position &other_origin) {
    for (const auto &d : other) {
        int x = d.x + other_origin.x;
        int y = d.y + other_origin.y;
        int z = d.z + other_origin.z;
        base.emplace(x, y, z);
    }
}

size_t Part1(const std::vector<ScannerData> &scanner_datas) {
    auto scanner0 = scanner_datas[0];
    std::set<size_t> checked;
    checked.insert(0);

    while (true) {
        for (size_t i = 1; i < scanner_datas.size(); ++i) {
            if (checked.find(i) != checked.end()) {
                continue;
            }

            auto candidates = AllOrientations(scanner_datas[i]);
            for (const auto &candidate : candidates) {
                auto scanner_position = FindScannerPosition(scanner0, candidate);
                if (scanner_position) {
                    checked.insert(i);
                    MergeScannerData(scanner0, candidate, scanner_position.value());
                    break;
                }
            }
        }

        if (checked.size() == scanner_datas.size()) {
            break;
        }
    }

    return scanner0.size();
}

int Part2(const std::vector<ScannerData> &scanner_datas) {
    auto scanner0 = scanner_datas[0];
    std::set<size_t> checked;
    checked.insert(0);

    std::vector<Position> scanners;
    scanners.emplace_back(0, 0, 0);

    while (true) {
        for (size_t i = 1; i < scanner_datas.size(); ++i) {
            if (checked.find(i) != checked.end()) {
                continue;
            }

            auto candidates = AllOrientations(scanner_datas[i]);
            for (const auto &candidate : candidates) {
                auto ret = FindScannerPosition(scanner0, candidate);
                if (ret) {
                    checked.insert(i);
                    auto scanner_position = ret.value();
                    MergeScannerData(scanner0, candidate, scanner_position);
                    scanners.emplace_back(scanner_position.x, scanner_position.y, scanner_position.z);
                    break;
                }
            }
        }

        if (checked.size() == scanner_datas.size()) {
            break;
        }
    }

    int ret = 0;
    int len = scanners.size();
    for (int i = 0; i < len; ++i) {
        for (int j = i + 1; j < len; ++j) {
            int dist_x = std::abs(scanners[i].x - scanners[j].x);
            int dist_y = std::abs(scanners[i].y - scanners[j].y);
            int dist_z = std::abs(scanners[i].z - scanners[j].z);
            ret = std::max(ret, dist_x + dist_y + dist_z);
        }
    }

    return ret;
}

void Test() {
    std::ifstream iss;
    iss.open("test.txt");
    auto scanner_datas = ParseInput(iss);
    iss.close();

    auto part1 = Part1(scanner_datas);
    auto part2 = Part2(scanner_datas);
    assert(part1 == 79);
    assert(part2 == 3621);
}

} // namespace

int main() {
    Test();

    auto scanner_datas = ParseInput(std::cin);
    auto part1 = Part1(scanner_datas);
    auto part2 = Part2(scanner_datas);

    std::cout << "Part1: " << part1 << std::endl;
    std::cout << "Part2: " << part2 << std::endl;
    return 0;
}