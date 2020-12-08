#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>

namespace {

enum class InstructionType {
    kAcc,
    kJmp,
    kNop,
};

struct Instruction {
    InstructionType type;
    int value;

    bool operator==(const Instruction &other) const {
        return type == other.type && value == other.value;
    }
};

std::vector<Instruction> ParseCode(const std::vector<std::string> &code) {
    std::vector<Instruction> ret;
    for (const auto &line : code) {
        std::string insn(3, 0);
        int value;
        sscanf(line.c_str(), "%s %d", &insn[0], &value);

        InstructionType type = InstructionType::kNop;
        if (insn == "acc") {
            type = InstructionType::kAcc;
        } else if (insn == "jmp") {
            type = InstructionType::kJmp;
        } else if (insn == "nop") {
            type = InstructionType::kNop;
        } else {
            std::cerr << "Unknown instruction: " << insn << std::endl;
            assert(!"unknown instruction");
        }

        ret.push_back(Instruction{type, value});
    }

    return ret;
}

int ExecuteInstructions(const std::vector<Instruction> &instructions) {
    std::vector<int> execute_count(instructions.size());
    int value = 0;
    int program_counter = 0;
    while (true) {
        ++execute_count[program_counter];
        if (execute_count[program_counter] >= 2) {
            break;
        }

        auto &insn = instructions[program_counter];
        switch (insn.type) {
        case InstructionType::kAcc:
            value += insn.value;
            ++program_counter;
            break;
        case InstructionType::kJmp:
            program_counter += insn.value;
            break;
        case InstructionType::kNop:
            ++program_counter;
            break;
        }
    }

    return value;
}

void Test01() {
    // clang-format off
    std::vector<std::string> code {
        "nop +0",
        "acc +1",
        "jmp +4",
        "acc +3",
        "jmp -3",
        "acc -99",
        "acc +1",
        "jmp -4",
        "acc +6"
    };
    // clang-format on

    auto insns = ParseCode(code);
    assert(insns.size() == 9);
    assert((insns[0] == Instruction{InstructionType::kNop, 0}));
    assert((insns[1] == Instruction{InstructionType::kAcc, 1}));
    assert((insns[2] == Instruction{InstructionType::kJmp, 4}));
    assert((insns[3] == Instruction{InstructionType::kAcc, 3}));
    assert((insns[4] == Instruction{InstructionType::kJmp, -3}));
    assert((insns[5] == Instruction{InstructionType::kAcc, -99}));
    assert((insns[6] == Instruction{InstructionType::kAcc, 1}));
    assert((insns[7] == Instruction{InstructionType::kJmp, -4}));
    assert((insns[8] == Instruction{InstructionType::kAcc, 6}));

    assert(ExecuteInstructions(insns) == 5);
}

} // namespace

int main() {
    Test01();

    std::vector<std::string> code;
    std::string line;
    while (std::getline(std::cin, line)) {
        code.push_back(line);
    }

    auto insns = ParseCode(code);

    std::cout << "Part01: " << ExecuteInstructions(insns) << std::endl;

    return 0;
}
