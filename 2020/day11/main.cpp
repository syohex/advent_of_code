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

int CountVisibleOccupiedSeats(const std::vector<std::string> &seats, int row, int col) {
    const auto see_occupied_seat = [&seats](int row, int col, int x_inc, int y_inc) {
        int row_max = static_cast<int>(seats.size() - 1);
        int col_max = static_cast<int>(seats[row].size() - 1);
        for (int i = 1;; ++i) {
            int x = row + (x_inc * i);
            int y = col + (y_inc * i);
            if (x < 0 || x > row_max) {
                break;
            }
            if (y < 0 || y > col_max) {
                break;
            }
            // if (!((x >= 0 && x <= row_max) && (y >= 0 && y <= col_max))) {
            //     break;
            // }

            switch (seats[x][y]) {
            case '#':
                return 1;
            case 'L':
                return 0;
            default:
                break;
            }
        }

        return 0;
    };

    // left upper
    int count = 0;
    count += see_occupied_seat(row, col, -1, -1); // left upper
    count += see_occupied_seat(row, col, -1, +0); // upper
    count += see_occupied_seat(row, col, -1, +1); // right upper
    count += see_occupied_seat(row, col, +0, -1); // left
    count += see_occupied_seat(row, col, +0, +1); // right
    count += see_occupied_seat(row, col, +1, -1); // left down
    count += see_occupied_seat(row, col, +1, +0); // down
    count += see_occupied_seat(row, col, +1, +1); // right down
    return count;
}

int Solve01(const std::vector<std::string> &seats) {
    auto prev = seats;
    while (true) {
        auto tmp = prev;
        int count = 0;
        bool changed = false;
        for (int i = 0; i < static_cast<int>(prev.size()); ++i) {
            for (int j = 0; j < static_cast<int>(prev[i].size()); ++j) {
                if (prev[i][j] == 'L') {
                    int occupied = CountAroundSeats(prev, i, j, '#');
                    if (occupied == 0) {
                        ++count;
                        tmp[i][j] = '#';
                        changed = true;
                    }
                } else if (prev[i][j] == '#') {
                    int empties = CountAroundSeats(prev, i, j, '#');
                    if (empties >= 4) {
                        tmp[i][j] = 'L';
                        changed = true;
                    } else {
                        ++count;
                    }
                }
            }
        }

        if (!changed) {
            return count;
        }

        prev = tmp;
    }

    assert(!"never reach here");
    return -1;
}

int Solve02(const std::vector<std::string> &seats) {
    auto prev = seats;
    while (true) {
        auto tmp = prev;
        int count = 0;
        bool changed = false;
        for (int i = 0; i < static_cast<int>(prev.size()); ++i) {
            for (int j = 0; j < static_cast<int>(prev[i].size()); ++j) {
                if (prev[i][j] == 'L') {
                    int occupied = CountVisibleOccupiedSeats(prev, i, j);
                    if (occupied == 0) {
                        ++count;
                        tmp[i][j] = '#';
                        changed = true;
                    }
                } else if (prev[i][j] == '#') {
                    int empties = CountVisibleOccupiedSeats(prev, i, j);
                    if (empties >= 5) {
                        tmp[i][j] = 'L';
                        changed = true;
                    } else {
                        ++count;
                    }
                }
            }
        }

        if (!changed) {
            return count;
        }

        prev = tmp;
    }

    assert(!"never reach here");
    return -1;
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

void Test02() {
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
    assert(Solve02(seats) == 26);
}

} // namespace

int main() {
    Test01();
    Test02();

    std::vector<std::string> seats;
    std::string line;
    while (std::getline(std::cin, line)) {
        seats.push_back(line);
    }

    std::cout << "Part01: " << Solve01(seats) << std::endl;
    std::cout << "Part02: " << Solve02(seats) << std::endl;
    return 0;
}
