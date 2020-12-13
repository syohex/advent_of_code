#include <cassert>
#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

namespace {

struct BusDeparture {
    std::int64_t id;
    std::int64_t offset;

    bool operator==(const BusDeparture &other) const {
        return id == other.id && offset == other.offset;
    }
};

std::vector<std::int64_t> ParseBusID(const std::string &str) {
    std::vector<std::int64_t> ret;
    std::string id;

    std::istringstream ss(str);
    while (std::getline(ss, id, ',')) {
        if (id == "x") {
            continue;
        }

        ret.push_back(std::stoll(id));
    }

    std::sort(ret.begin(), ret.end());
    return ret;
}

std::vector<BusDeparture> ParseBusID2(const std::string &str) {
    std::vector<BusDeparture> ret;
    std::string id;
    std::int64_t offset = 0;

    std::istringstream ss(str);
    while (std::getline(ss, id, ',')) {
        if (id != "x") {
            auto bus_id = std::stoll(id);
            ret.push_back(BusDeparture{bus_id, offset});
        }

        ++offset;
    }

    return ret;
}

std::int64_t Solve01(std::int64_t timeout, const std::vector<std::int64_t> bus_ids) {
    std::int64_t min_id = INT64_MAX;
    std::int64_t min_diff = INT64_MAX;
    for (const auto bus_id : bus_ids) {
        std::int64_t val = bus_id;
        for (std::int64_t i = 1; val < timeout; ++i) {
            val += bus_id;
        }

        auto diff = val - timeout;
        if (diff < min_diff) {
            min_id = bus_id;
            min_diff = diff;
        }
    }

    assert(min_id != INT64_MAX);
    assert(min_diff != INT64_MAX);

    return min_diff * min_id;
}

std::int64_t Solve02(const std::vector<BusDeparture> bus_ids) {
    std::vector<bool> checked(bus_ids.size(), false);
    checked[0] = true;

    auto base = bus_ids[0].id;
    std::int64_t val = 0;
    for (std::int64_t i = 1;; ++i) {
        bool ok = true;
        val += base;
        for (size_t j = 1; j < bus_ids.size(); ++j) {
            if ((val + bus_ids[j].offset) % bus_ids[j].id == 0) {
                if (!checked[j]) {
                    base *= bus_ids[j].id;
                    checked[j] = true;
                }
            } else {
                ok = false;
                break;
            }
        }

        if (ok) {
            return val;
        }
    }

    assert(!"never reach here");
    return 0;
}

void Test01() {
    std::int64_t timeout = 939;
    std::string bus_id_str("7,13,x,x,59,x,31,19");

    auto bus_ids = ParseBusID(bus_id_str);
    assert((bus_ids == std::vector<std::int64_t>{7, 13, 19, 31, 59}));

    auto val = Solve01(timeout, bus_ids);
    assert(val == 295);
}

void Test02() {
    std::string bus_id_str("7,13,x,x,59,x,31,19");

    auto bus_ids = ParseBusID2(bus_id_str);
    assert(bus_ids.size() == 5);
    assert((bus_ids[0] == BusDeparture{7, 0}));
    assert((bus_ids[1] == BusDeparture{13, 1}));
    assert((bus_ids[2] == BusDeparture{59, 4}));
    assert((bus_ids[3] == BusDeparture{31, 6}));
    assert((bus_ids[4] == BusDeparture{19, 7}));

    {
        auto ids = ParseBusID2("17,x,13,19");
        assert(Solve02(ids) == 3417);
    }
    {
        auto ids = ParseBusID2("67,7,59,61");
        assert(Solve02(ids) == 754018);
    }
    {
        auto ids = ParseBusID2("67,x,7,59,61");
        assert(Solve02(ids) == 779210);
    }
    {
        auto ids = ParseBusID2("67,7,x,59,61");
        assert(Solve02(ids) == 1261476);
    }
    {
        auto ids = ParseBusID2("1789,37,47,1889");
        assert(Solve02(ids) == 1202161486);
    }
}

} // namespace

int main() {
    Test01();
    Test02();

    std::int64_t timestamp;
    std::string bus_id_line;
    std::cin >> timestamp >> bus_id_line;

    {
        auto bus_ids = ParseBusID(bus_id_line);
        std::cout << "Part01: " << Solve01(timestamp, bus_ids) << std::endl;
    }
    {
        auto bus_ids = ParseBusID2(bus_id_line);
        std::cout << "Part02: " << Solve02(bus_ids) << std::endl;
    }

    return 0;
}
