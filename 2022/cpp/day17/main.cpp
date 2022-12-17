#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>

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

std::vector<int64_t> CurrentShape(const std::vector<std::vector<bool>> &stage, int row) {
    std::vector<int64_t> cols(7, 0);
    for (int col = 0; col < 7; ++col) {
        for (int r = row - 1; r >= 0; --r) {
            if (stage[r][col]) {
                cols[col] = row - r;
                break;
            }
        }
    }
    return cols;
}

struct Data {
    size_t op_index;
    size_t block_index;
    std::vector<int64_t> shape;
    int64_t highest;
};

int64_t Problem2(const std::string &operations, int64_t count, const std::vector<Block> &blocks) {
    std::vector<std::vector<bool>> stage(3, std::vector<bool>(7, false));
    int64_t rocks = 0;

    int64_t highest_rock = 0;
    size_t block_index = 0;
    ResizeStage(stage, highest_rock + 3 + blocks[block_index].height);

    int row = 3;
    int col = 2;

    std::map<std::pair<size_t, size_t>, std::map<std::vector<int64_t>, std::pair<int64_t, int64_t>>> shape_info;
    std::vector<int64_t> shape = CurrentShape(stage, row);
    shape_info[{0, 0}][shape] = {rocks, highest_rock};

    std::vector<Data> phase_info;

    for (size_t i = 0;;) {
        phase_info.push_back({i, block_index, CurrentShape(stage, row), highest_rock});

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

            highest_rock = std::max(highest_rock, static_cast<int64_t>(row) + current_block.height);

            row = highest_rock + 3;
            col = 2;

            block_index = (block_index + 1) % blocks.size();
            ResizeStage(stage, highest_rock + 3 + blocks[block_index].height);

            ++rocks;

            const auto current_shape = CurrentShape(stage, row);
            const std::pair<size_t, size_t> key{i, block_index};
            auto &ss = shape_info[key];
            const auto &it = ss.find(current_shape);
            if (it != ss.end()) {
                auto prev_rocks = it->second.first;
                auto prev_highest = it->second.second;

                auto period = rocks - prev_rocks;
                auto highest_period = highest_rock - prev_highest;

                int64_t diff = count - rocks;
                highest_rock += (diff / period) * highest_period;

                for (size_t p = 0; p < phase_info.size(); ++p) {
                    const auto &d = phase_info[p];
                    if (d.op_index == i && d.block_index == block_index && d.shape == current_shape) {
                        int64_t rest_rocks = diff % period;
                        rocks = 0;
                        for (size_t q = p + 1; rocks < rest_rocks; ++q) {
                            highest_rock += phase_info[q].highest - phase_info[q - 1].highest;
                            if (phase_info[q - 1].block_index != phase_info[q].block_index) {
                                ++rocks;
                            }
                        }
                        break;
                    }
                }

                return highest_rock;
            }

            ss[current_shape] = {rocks, highest_rock};
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

    int64_t ret2 = Problem2(input, 1000000000000L, blocks);
    assert(ret2 == 1514285714288L);
}

int main() {
    Test();

    std::vector<Block> blocks = InitBlocks();
    std::string input;
    std::cin >> input;

    int ret1 = Problem1(input, 2022, blocks);
    int64_t ret2 = Problem2(input, 1000000000000L, blocks);

    std::cout << "Problem1: " << ret1 << std::endl;
    std::cout << "Problem2: " << ret2 << std::endl;

    return 0;
}
