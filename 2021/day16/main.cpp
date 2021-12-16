#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <limits>

namespace {

std::vector<int> ToBits(const std::string &s) {
    std::vector<int> ret;
    for (char c : s) {
        std::vector<int> bits;
        switch (c) {
        case '0':
            bits = {0, 0, 0, 0};
            break;
        case '1':
            bits = {0, 0, 0, 1};
            break;
        case '2':
            bits = {0, 0, 1, 0};
            break;
        case '3':
            bits = {0, 0, 1, 1};
            break;
        case '4':
            bits = {0, 1, 0, 0};
            break;
        case '5':
            bits = {0, 1, 0, 1};
            break;
        case '6':
            bits = {0, 1, 1, 0};
            break;
        case '7':
            bits = {0, 1, 1, 1};
            break;
        case '8':
            bits = {1, 0, 0, 0};
            break;
        case '9':
            bits = {1, 0, 0, 1};
            break;
        case 'A':
            bits = {1, 0, 1, 0};
            break;
        case 'B':
            bits = {1, 0, 1, 1};
            break;
        case 'C':
            bits = {1, 1, 0, 0};
            break;
        case 'D':
            bits = {1, 1, 0, 1};
            break;
        case 'E':
            bits = {1, 1, 1, 0};
            break;
        case 'F':
            bits = {1, 1, 1, 1};
            break;
        default:
            assert(!"never reach here");
        }

        std::copy(bits.begin(), bits.end(), std::back_inserter(ret));
    }

    return ret;
}

struct Packet {
    int version;
    int type;
    std::int64_t value = 0;
    std::vector<Packet> sub_packets;
};

Packet DecodePacket(const std::vector<int> &bits, size_t &pos) {
    Packet ret;

    // read version
    int version = 0;
    for (int i = 0; i < 3; ++i) {
        version = version * 2 + bits[pos];
        ++pos;
    }

    ret.version = version;

    // read type
    int type = 0;
    for (int i = 0; i < 3; ++i) {
        type = type * 2 + bits[pos];
        ++pos;
    }
    ret.type = type;

    if (type == 4) {
        bool finish = false;
        std::int64_t value = 0;
        while (!finish) {
            int flag = bits[pos];
            ++pos;

            if (flag == 0) {
                finish = true;
            }

            for (int i = 0; i < 4; ++i) {
                value = value * 2 + bits[pos];
                ++pos;
            }
        }

        ret.value = value;
    } else {
        int id = bits[pos];
        ++pos;
        if (id == 0) {
            int length = 0;
            for (int i = 0; i < 15; ++i) {
                length = length * 2 + bits[pos];
                ++pos;
            }

            size_t limit = pos + length;
            while (pos < limit) {
                ret.sub_packets.push_back(DecodePacket(bits, pos));
            }
        } else {
            int count = 0;
            for (int i = 0; i < 11; ++i) {
                count = count * 2 + bits[pos];
                ++pos;
            }

            for (int i = 0; i < count; ++i) {
                ret.sub_packets.push_back(DecodePacket(bits, pos));
            }
        }
    }

    return ret;
}

std::int64_t TotalVersions(const Packet &p) {
    std::int64_t ret = p.version;
    for (const auto &sub_packet : p.sub_packets) {
        ret += TotalVersions(sub_packet);
    }
    return ret;
}

std::int64_t PacketValue(const Packet &p) {
    switch (p.type) {
    case 0: {
        std::int64_t ret = 0;
        for (const auto &sp : p.sub_packets) {
            ret += PacketValue(sp);
        }
        return ret;
    }
    case 1: {
        std::int64_t ret = 1;
        for (const auto &sp : p.sub_packets) {
            ret *= PacketValue(sp);
        }
        return ret;
    }
    case 2: {
        auto ret = std::numeric_limits<std::int64_t>::max();
        for (const auto &sp : p.sub_packets) {
            ret = std::min(ret, PacketValue(sp));
        }
        return ret;
    }
    case 3: {
        auto ret = std::numeric_limits<std::int64_t>::min();
        for (const auto &sp : p.sub_packets) {
            ret = std::max(ret, PacketValue(sp));
        }
        return ret;
    }
    case 4:
        return p.value;
    case 5: {
        assert(p.sub_packets.size() == 2);
        auto v1 = PacketValue(p.sub_packets[0]);
        auto v2 = PacketValue(p.sub_packets[1]);
        return v1 > v2 ? 1 : 0;
    }
    case 6: {
        assert(p.sub_packets.size() == 2);
        auto v1 = PacketValue(p.sub_packets[0]);
        auto v2 = PacketValue(p.sub_packets[1]);
        return v1 < v2 ? 1 : 0;
    }
    case 7: {
        assert(p.sub_packets.size() == 2);
        auto v1 = PacketValue(p.sub_packets[0]);
        auto v2 = PacketValue(p.sub_packets[1]);
        return v1 == v2 ? 1 : 0;
    }
    default:
        break;
    }

    throw "never reach here";
}

std::int64_t Part1(const std::string &input) {
    const std::vector<int> bits = ToBits(input);
    size_t pos = 0;

    auto packet = DecodePacket(bits, pos);
    return TotalVersions(packet);
}

std::int64_t Part2(const std::string &input) {
    const std::vector<int> bits = ToBits(input);
    size_t pos = 0;

    auto packet = DecodePacket(bits, pos);
    return PacketValue(packet);
}

void Test() {
    (void)Part1("D2FE28");
    (void)Part1("38006F45291200");
    assert(Part1("8A004A801A8002F478") == 16);
    assert(Part1("620080001611562C8802118E34") == 12);
    assert(Part1("C0015000016115A2E0802F182340") == 23);
    assert(Part1("A0016C880162017C3686B18A3D4780") == 31);

    assert(Part2("C200B40A82") == 3);
    assert(Part2("04005AC33890") == 54);
    assert(Part2("880086C3E88112") == 7);
    assert(Part2("CE00C43D881120") == 9);
    assert(Part2("D8005AC2A8F0") == 1);
    assert(Part2("F600BC2D8F") == 0);
    assert(Part2("9C005AC2F8F0") == 0);
    assert(Part2("9C0141080250320F1802104A08") == 1);
}

} // namespace

int main() {
    Test();

    std::string input;
    std::cin >> input;

    auto part1 = Part1(input);
    auto part2 = Part2(input);

    std::cout << "Part1: " << part1 << std::endl;
    std::cout << "Part2: " << part2 << std::endl;

    return 0;
}
