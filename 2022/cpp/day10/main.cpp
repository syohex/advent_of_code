#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

enum class InstructionType {
    kAddrx,
    kNoop,
};

struct Instruction {
    InstructionType type;
    int val;
};

template <typename T>
std::vector<Instruction> ParseInput(T &it) {
    std::vector<Instruction> ret;
    std::string tmp;
    char buf[16];
    int val;

    while (std::getline(it, tmp)) {
        if (tmp.empty()) {
            break;
        }

        if (tmp.find("addx") == 0) {
            sscanf(tmp.c_str(), "%s %d", buf, &val);
            ret.push_back({InstructionType::kAddrx, val});
        } else {
            ret.push_back({InstructionType::kNoop, 0});
        }
    }

    return ret;
}

int Problem1(const std::vector<Instruction> &insns) {
    int clock = 1;
    int reg = 1;
    int ret = 0;

    for (const auto &insn : insns) {
        if (insn.type == InstructionType::kNoop) {
            clock += 1;
            if ((clock - 20) % 40 == 0) {
                ret += reg * clock;
            }
        } else {
            clock += 1;
            if ((clock - 20) % 40 == 0) {
                ret += reg * clock;
            }

            clock += 1;
            reg += insn.val;

            if ((clock - 20) % 40 == 0) {
                ret += reg * clock;
            }
        }
    }

    return ret;
}

void Test() {
    std::ifstream fs("test.txt");
    auto data = ParseInput(fs);

    int ret1 = Problem1(data);
    assert(ret1 == 13140);
}

int main() {
    Test();

    auto data = ParseInput(std::cin);
    int ret1 = Problem1(data);

    std::cout << "Problem1: " << ret1 << std::endl;
    return 0;
}
