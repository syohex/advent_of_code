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

void Problem2(const std::vector<Instruction> &insns) {
    int clock = 0;
    std::vector<std::vector<char>> crt(6, std::vector<char>(40, '.'));
    int row = 0;
    int col = 0;
    int sprite_pos = 0;

    for (const auto &insn : insns) {
        if (insn.type == InstructionType::kNoop) {
            if (col >= sprite_pos && col < sprite_pos + 3) {
                crt[row][col] = '#';
            }

            clock += 1;
            col += 1;
            if (clock % 40 == 0) {
                row += 1;
                col = 0;
            }
        } else {
            if (col >= sprite_pos && col < sprite_pos + 3) {
                crt[row][col] = '#';
            }

            clock += 1;
            col += 1;
            if (clock % 40 == 0) {
                row += 1;
                col = 0;
            }

            clock += 1;
            if (col >= sprite_pos && col < sprite_pos + 3) {
                crt[row][col] = '#';
            }
            sprite_pos += insn.val;
            col += 1;

            if (clock % 40 == 0) {
                row += 1;
                col = 0;
            }
        }
    }

    for (const auto &row : crt) {
        for (char c : row) {
            printf("%c", c);
        }
        printf("\n");
    }
}

void Test() {
    std::ifstream fs("test.txt");
    auto data = ParseInput(fs);

    int ret1 = Problem1(data);
    assert(ret1 == 13140);

    Problem2(data);
}

int main() {
    Test();

    auto data = ParseInput(std::cin);
    int ret1 = Problem1(data);

    std::cout << "Problem1: " << ret1 << std::endl;

    Problem2(data);
    return 0;
}
