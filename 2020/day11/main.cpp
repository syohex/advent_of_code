#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

namespace {

std::vector<std::string> StringToSeats(const std::string &str) {
    std::vector<std::string> seats;
    std::istringstream ss(str);
    std::string line;
    while (std::getline(ss, line)) {
        seats.push_back(line);
    }

    return seats;
}

int CountAroundSeats(const std::vector<std::string> &seats, int row, int col, char seat) {
    int count = 0;
    int row_max = static_cast<int>(seats.size() - 1);
    int col_max = static_cast<int>(seats[row].size() - 1);
    if (row >= 1) {
        if (col >= 1 && seats[row - 1][col - 1] == seat) {
            ++count;
        }
        if (seats[row - 1][col] == seat) {
            ++count;
        }
        if (col <= col_max - 1 && seats[row - 1][col + 1] == seat) {
            ++count;
        }
    }

    if (col >= 1 && seats[row][col - 1] == seat) {
        ++count;
    }
    if (col <= col_max - 1 && seats[row][col + 1] == seat) {
        ++count;
    }

    if (row <= row_max - 1) {
        if (col >= 1 && seats[row + 1][col - 1] == seat) {
            ++count;
        }
        if (seats[row + 1][col] == seat) {
            ++count;
        }
        if (col <= col_max - 1 && seats[row + 1][col + 1] == seat) {
            ++count;
        }
    }

    return count;
}

int Solve01(std::vector<std::string> &seats) {
    int prev = -1;

    while (true) {
        auto tmp = seats;
        int count = 0;
        for (int i = 0; i < static_cast<int>(seats.size()); ++i) {
            for (int j = 0; j < static_cast<int>(seats[i].size()); ++j) {
                if (seats[i][j] == 'L') {
                    int occupied = CountAroundSeats(seats, i, j, '#');
                    if (occupied == 0) {
                        ++count;
                        tmp[i][j] = '#';
                    }
                } else if (seats[i][j] == '#') {
                    int empties = CountAroundSeats(seats, i, j, '#');
                    if (empties >= 4) {
                        tmp[i][j] = 'L';
                    } else {
                        ++count;
                    }
                }
            }
        }

        if (count == prev) {
            break;
        }

        seats = tmp;
        prev = count;
    }

    return prev;
}

void Test01() {
    std::string input(R"(L.LL.LL.LL
LLLLLLL.LL
L.L.L..L..
LLLL.LL.LL
L.LL.LL.LL
L.LLLLL.LL
..L.L.....
LLLLLLLLLL
L.LLLLLL.L
L.LLLLL.LL)");

    auto seats = StringToSeats(input);
    assert(Solve01(seats) == 37);
}

} // namespace

int main() {
    Test01();

    std::vector<std::string> seats;
    std::string line;
    while (std::getline(std::cin, line)) {
        seats.push_back(line);
    }

    std::cout << "Part01: " << Solve01(seats) << std::endl;
    return 0;
}
