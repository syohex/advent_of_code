#include <cassert>
#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

namespace {

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

void Test01() {
    std::int64_t timeout = 939;
    std::string bus_id_str("7,13,x,x,59,x,31,19");

    auto bus_ids = ParseBusID(bus_id_str);
    assert((bus_ids == std::vector<std::int64_t>{7, 13, 19, 31, 59}));

    auto val = Solve01(timeout, bus_ids);
    assert(val == 295);
}

} // namespace

int main() {
    Test01();

    std::int64_t timestamp;
    std::string bus_id_line;
    std::cin >> timestamp >> bus_id_line;

    auto bus_ids = ParseBusID(bus_id_line);

    std::cout << "Part01: " << Solve01(timestamp, bus_ids) << std::endl;

    return 0;
}
