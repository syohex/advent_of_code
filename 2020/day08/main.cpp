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

int ExecuteInstructions(const std::vector<Instruction> &instructions, bool &finish) {
    std::vector<int> execute_count(instructions.size());
    int value = 0;
    int program_counter = 0;
    while (program_counter >= 0 && program_counter < static_cast<int>(instructions.size())) {
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

    finish = program_counter == static_cast<int>(instructions.size());
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

    bool finish;
    assert(ExecuteInstructions(insns, finish) == 5);
}

int Solve02(std::vector<Instruction> &insns) {
    bool finish;
    int count = 0;
    for (auto &insn : insns) {
        if (insn.type == InstructionType::kJmp) {
            insn.type = InstructionType::kNop;

            int ret = ExecuteInstructions(insns, finish);
            if (finish) {
                return ret;
            }

            insn.type = InstructionType::kJmp;
        } else if (insn.type == InstructionType::kNop) {
            insn.type = InstructionType::kJmp;
            int ret = ExecuteInstructions(insns, finish);
            if (finish) {
                return ret;
            }

            insn.type = InstructionType::kNop;
        }
        ++count;
    }

    assert(!"never reach here");
    return -1;
}

void Test02() {
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
    assert(Solve02(insns) == 8);
}

} // namespace

int main() {
    Test01();
    Test02();

    std::vector<std::string> code;
    std::string line;
    while (std::getline(std::cin, line)) {
        code.push_back(line);
    }

    auto insns = ParseCode(code);
    bool finish;
    std::cout << "Part01: " << ExecuteInstructions(insns, finish) << std::endl;
    std::cout << "Part02: " << Solve02(insns) << std::endl;

    return 0;
}
