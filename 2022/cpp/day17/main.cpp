#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

struct Block {
    int width;
    int height;
    std::vector<std::pair<int, int>> rocks;
};

std::vector<Block> InitBlocks() {
    std::vector<Block> blocks;

    // ####
    {
        int width = 4;
        int height = 1;
        std::vector<std::pair<int, int>> rocks{
            {0, 0},
            {0, 1},
            {0, 2},
            {0, 3},
        };
        blocks.push_back({width, height, rocks});
    }

    // .#.
    // ###
    // .#.
    {
        int width = 3;
        int height = 3;
        std::vector<std::pair<int, int>> rocks{
            {0, 1}, {1, 0}, {1, 1}, {1, 2}, {2, 1},
        };
        blocks.push_back({width, height, rocks});
    }

    // ..#    ###
    // ..# -> ..#
    // ###    ..#
    {
        int width = 3;
        int height = 3;
        std::vector<std::pair<int, int>> rocks{
            {0, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 2},
        };
        blocks.push_back({width, height, rocks});
    }

    // #
    // #
    // #
    // #
    {
        int width = 1;
        int height = 4;
        std::vector<std::pair<int, int>> rocks{
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 0},
        };
        blocks.push_back({width, height, rocks});
    }

    // ##
    // ##
    {
        int width = 2;
        int height = 2;
        std::vector<std::pair<int, int>> rocks{
            {0, 0},
            {0, 1},
            {1, 0},
            {1, 1},
        };
        blocks.push_back({width, height, rocks});
    }

    return blocks;
}

void PrintStage(const std::vector<std::vector<bool>> &stage) {
    int rows = stage.size();
    int cols = stage[0].size();
    printf("#################################\n");
    for (int i = rows - 1; i >= 0; --i) {
        for (int j = 0; j < cols; ++j) {
            printf("%c", stage[i][j] ? '#' : '.');
        }
        printf("\n");
    }
}

bool CanMove(const std::vector<std::vector<bool>> &stage, int row, int col, const Block &block) {
    for (const auto &[rock_row, rock_col] : block.rocks) {
        int r = row + rock_row;
        int c = col + rock_col;

        if (stage[r][c]) {
            return false;
        }
    }

    return true;
}

void ResizeStage(std::vector<std::vector<bool>> &stage, int height) {
    int rows = stage.size();
    int cols = stage[0].size();

    if (rows >= height) {
        return;
    }

    int diff = height - rows;
    for (int i = 0; i < diff; ++i) {
        stage.push_back(std::vector<bool>(cols, false));
    }
}

void AddBlock(std::vector<std::vector<bool>> &stage, int row, int col, const Block &block) {
    for (const auto &[r, c] : block.rocks) {
        stage[row + r][col + c] = true;
    }
}

int Problem1(const std::string &operations, int count, const std::vector<Block> &blocks) {
    std::vector<std::vector<bool>> stage(3, std::vector<bool>(7, false));
    int rocks = 0;

    int highest_rock = 0;
    size_t block_index = 0;
    ResizeStage(stage, highest_rock + 3 + blocks[block_index].height);

    int row = 3;
    int col = 2;

    for (size_t i = 0;;) {
        const Block &current_block = blocks[block_index];
        char op = operations[i];
        i = (i + 1) % operations.size();

        if (op == '>') {
            if (col + current_block.width <= 6) {
                if (CanMove(stage, row, col + 1, current_block)) {
                    col += 1;
                }
            }
        } else {
            if (col >= 1) {
                if (CanMove(stage, row, col - 1, current_block)) {
                    col -= 1;
                }
            }
        }

        if (row >= 1 && CanMove(stage, row - 1, col, current_block)) {
            row -= 1;
        } else {
            for (const auto &[r, c] : current_block.rocks) {
                stage[row + r][col + c] = true;
            }

            highest_rock = std::max(highest_rock, row + current_block.height);

            row = highest_rock + 3;
            col = 2;

            block_index = (block_index + 1) % blocks.size();
            ResizeStage(stage, highest_rock + 3 + blocks[block_index].height);

            ++rocks;
            if (rocks >= count) {
                return highest_rock;
            }
        }
    }

    // never reach here
    return -1;
}

void Test() {
    std::string input = ">>><<><>><<<>><>>><<<>>><<<><<<>><>><<>>";

    std::vector<Block> blocks = InitBlocks();

    int ret1 = Problem1(input, 2022, blocks);
    assert(ret1 == 3068);
}

int main() {
    Test();

    std::vector<Block> blocks = InitBlocks();
    std::string input;
    std::cin >> input;

    int ret1 = Problem1(input, 2022, blocks);

    std::cout << "Problem1: " << ret1 << std::endl;

    return 0;
}
