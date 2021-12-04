#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <set>

namespace {
struct BingoBoard {
    std::vector<std::vector<int>> data;
    std::vector<std::vector<bool>> mark;

    BingoBoard() : data(5, std::vector<int>(5, 0)), mark(5, std::vector<bool>(5, false)) {
    }

    void Set(int num) {
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                if (data[i][j] == num) {
                    mark[i][j] = true;
                    return;
                }
            }
        }
    }

    bool Check() const {
        for (int i = 0; i < 5; ++i) {
            auto &row = mark[i];
            if (row[0] && row[1] && row[2] && row[3] && row[4]) {
                return true;
            }
        }
        for (int i = 0; i < 5; ++i) {
            if (mark[0][i] && mark[1][i] && mark[2][i] && mark[2][i] && mark[3][i] && mark[4][i]) {
                return true;
            }
        }

        return false;
    }

    int Score() const {
        int ret = 0;
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                if (!mark[i][j]) {
                    ret += data[i][j];
                }
            }
        }
        return ret;
    }

    void Print() {
        printf("###\n");
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                printf(" %c ", mark[i][j] ? 'o' : 'x');
            }
            printf("\n");
        }
        printf("###\n");
    }
};

struct Bingo {
    std::vector<int> nums;
    std::vector<BingoBoard> boards;

    int Play() {
        for (int num : nums) {
            for (auto &board : boards) {
                board.Set(num);
                // board.Print();
                if (board.Check()) {
                    return num * board.Score();
                }
            }
        }

        // never reach here
        return -1;
    }

    int PlayToTheLast() {
        std::set<size_t> unfinished;
        for (size_t i = 0; i < boards.size(); ++i) {
            unfinished.insert(i);
        }

        for (int num : nums) {
            for (size_t i = 0; i < boards.size(); ++i) {
                if (unfinished.find(i) == unfinished.end()) {
                    continue;
                }

                boards[i].Set(num);
                if (boards[i].Check()) {
                    unfinished.erase(i);
                    if (unfinished.empty()) {
                        return num * boards[i].Score();
                    }
                }
            }
        }

        // never reach here
        return -1;
    }
};

Bingo ParseInput(std::istream &ss) {
    Bingo ret;
    std::string tmp;

    std::getline(ss, tmp);

    std::stringstream ss_num(tmp);
    while (std::getline(ss_num, tmp, ',')) {
        ret.nums.push_back(std::stoi(tmp));
    }

    while (std::getline(ss, tmp)) {
        BingoBoard board;
        for (int i = 0; i < 5; ++i) {
            std::getline(ss, tmp);
            std::stringstream ss_bingo(tmp);
            int j = 0;
            while (std::getline(ss_bingo, tmp, ' ')) {
                if (tmp.empty()) { // for 0-9, they have two spaces prefix
                    continue;
                }

                board.data[i][j] = std::stoi(tmp);
                ++j;
            }
        }
        ret.boards.push_back(board);
    }

    return ret;
}

void Test() {
    std::string input(R"(7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1

22 13 17 11  0
 8  2 23  4 24
21  9 14 16  7
 6 10  3 18  5
 1 12 20 15 19

 3 15  0  2 22
 9 18 13 17  5
19  8  7 25 23
20 11 10 24  4
14 21 16 12  6

14 21 17 24  4
10 16 15  9 19
18  8 23 26 20
22 11 13  6  5
 2  0 12  3  7
)");

    std::istringstream ss(input);
    auto bingo = ParseInput(ss);
    assert(bingo.Play() == 4512);
    assert(bingo.PlayToTheLast() == 1924);
}

} // namespace

int main() {
    Test();

    Bingo bingo = ParseInput(std::cin);
    int part1 = bingo.Play();
    int part2 = bingo.PlayToTheLast();

    std::cout << "Part1: " << part1 << std::endl;
    std::cout << "Part2: " << part2 << std::endl;
    return 0;
}
