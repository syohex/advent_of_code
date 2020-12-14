#include <cassert>
#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <functional>

namespace {

enum class Type {
    kMask,
    kAddress,
};

struct Instruction {
    explicit Instruction(std::string mask) : type(Type::kMask), mask(std::move(mask)), addr(0), value(0) {
    }

    explicit Instruction(int addr, int value) : type(Type::kAddress), addr(addr), value(value) {
    }

    Type type;
    std::string mask;
    std::uint64_t addr;
    std::uint64_t value;

    bool operator==(const Instruction &other) const {
        return type == other.type && mask == other.mask && addr == other.addr && value == other.value;
    }
};

std::vector<Instruction> ParseInput(const std::vector<std::string> &inputs) {
    std::vector<Instruction> ret;
    for (const auto &input : inputs) {
        int addr, value;
        char buf[64];
        if (input.find("mask") == 0) {
            sscanf(input.c_str(), "mask = %s", buf);
            ret.emplace_back(buf);
        } else if (input.find("mem") == 0) {
            sscanf(input.c_str(), "mem[%d] = %d\n", &addr, &value);
            ret.emplace_back(addr, value);
        } else {
            assert(!"invalid input never reach here");
        }
    }
    return ret;
}

std::uint64_t Mask(const std::string &mask, std::uint64_t value) {
    std::uint64_t ret = 0;
    for (std::uint64_t i = 0; i < 36; ++i) {
        std::uint64_t v = value & 0x1;
        switch (mask[35 - i]) {
        case '1':
            ret |= (static_cast<std::uint64_t>(1) << i);
            break;
        case '0':
            break;
        case 'X':
            ret |= (v << i);
            break;
        default:
            assert(!"never reach here");
            break;
        }

        value >>= 1;
    }

    return ret;
}

std::string MaskAddress(const std::string &mask, std::uint64_t addr) {
    std::string ret;
    for (std::uint64_t i = 0; i < 36; ++i) {
        std::uint64_t v = addr & 0x1;
        switch (mask[35 - i]) {
        case '1':
            ret.push_back('1');
            break;
        case '0':
            ret.push_back(v + '0');
            break;
        case 'X':
            ret.push_back('X');
            break;
        default:
            assert(!"never reach here");
            break;
        }

        addr >>= 1;
    }

    std::reverse(ret.begin(), ret.end());
    return ret;
}

std::vector<std::uint64_t> CandidateAddresses(const std::string &floating_mask) {
    std::vector<std::uint64_t> ret;
    std::function<void(size_t pos, std::uint64_t address)> f;
    f = [&f, &ret, &floating_mask](size_t pos, std::uint64_t address) {
        if (pos >= 36) {
            ret.push_back(address);
            return;
        }

        switch (floating_mask[35 - pos]) {
        case '0':
            f(pos + 1, address);
            break;
        case '1':
            f(pos + 1, address + (static_cast<std::uint64_t>(1) << pos));
            break;
        case 'X':
            f(pos + 1, address);
            f(pos + 1, address + (static_cast<std::uint64_t>(1) << pos));
            break;
        }
    };

    f(0, 0);

    std::sort(ret.begin(), ret.end());
    return ret;
}

std::uint64_t Solve01(const std::vector<Instruction> &insns) {
    std::string mask;
    std::map<std::uint64_t, std::uint64_t> memory;

    for (const auto &insn : insns) {
        if (insn.type == Type::kMask) {
            mask = insn.mask;
        } else {
            memory[insn.addr] = Mask(mask, insn.value);
        }
    }

    std::uint64_t ret = 0;
    for (const auto &it : memory) {
        ret += it.second;
    }

    return ret;
}

std::uint64_t Solve02(const std::vector<Instruction> &insns) {
    std::string mask;
    std::map<std::uint64_t, std::uint64_t> memory;

    for (const auto &insn : insns) {
        if (insn.type == Type::kMask) {
            mask = insn.mask;
        } else {
            const auto cand_mask = MaskAddress(mask, insn.addr);
            const auto addrs = CandidateAddresses(cand_mask);
            for (const auto addr : addrs) {
                memory[addr] = insn.value;
            }
        }
    }

    std::uint64_t ret = 0;
    for (const auto &it : memory) {
        ret += it.second;
    }

    return ret;
}

void Test01() {
    // clang-format off
    std::vector<std::string> inputs {
        "mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X",
        "mem[8] = 11",
        "mem[7] = 101",
        "mem[8] = 0",
    };
    // clang-format on
    auto insns = ParseInput(inputs);
    assert(insns.size() == 4);
    assert(insns[0] == Instruction("XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X"));
    assert(insns[1] == Instruction(8, 11));
    assert(insns[2] == Instruction(7, 101));
    assert(insns[3] == Instruction(8, 0));

    const std::string mask = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X";
    assert(Mask(mask, 11) == 73);
    assert(Mask(mask, 101) == 101);
    assert(Mask(mask, 0) == 64);

    assert(Solve01(insns) == 165);
}

void Test02() {
    // clang-format off
    std::vector<std::string> inputs {
        "mask = 000000000000000000000000000000X1001X",
        "mem[42] = 100",
        "mask = 00000000000000000000000000000000X0XX",
        "mem[26] = 1",
    };
    // clang-format on

    const auto insns = ParseInput(inputs);
    assert(MaskAddress("000000000000000000000000000000X1001X", 42) == "000000000000000000000000000000X1101X");
    assert(MaskAddress("00000000000000000000000000000000X0XX", 26) == "00000000000000000000000000000001X0XX");

    {
        const auto addrs = CandidateAddresses("000000000000000000000000000000X1101X");
        assert((addrs == std::vector<std::uint64_t>{26, 27, 58, 59}));
    }
    {
        const auto addrs = CandidateAddresses("00000000000000000000000000000001X0XX");
        assert((addrs == std::vector<std::uint64_t>{16, 17, 18, 19, 24, 25, 26, 27}));
    }

    assert(Solve02(insns) == 208);
}

} // namespace

int main() {
    Test01();
    Test02();

    std::vector<std::string> inputs;
    std::string line;
    while (std::getline(std::cin, line)) {
        inputs.push_back(line);
    }

    auto insns = ParseInput(inputs);
    std::cout << "Part01: " << Solve01(insns) << std::endl;
    std::cout << "Part02: " << Solve02(insns) << std::endl;
    return 0;
}
