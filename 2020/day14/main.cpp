#include <cassert>
#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <map>

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

} // namespace

int main() {
    Test01();

    std::vector<std::string> inputs;
    std::string line;
    while (std::getline(std::cin, line)) {
        inputs.push_back(line);
    }

    auto insns = ParseInput(inputs);
    std::cout << "Part01: " << Solve01(insns) << std::endl;
    return 0;
}
