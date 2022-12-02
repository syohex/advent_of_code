#include <cassert>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <iostream>

enum class Shape {
    kRock = 1,
    kPaper = 2,
    kScissor = 3,
};

struct Data {
    Shape opposite;
    Shape me;
};

Shape CharToShape(char c) {
    switch (c) {
    case 'A':
    case 'X':
        return Shape::kRock;
    case 'B':
    case 'Y':
        return Shape::kPaper;
    case 'C':
    case 'Z':
        return Shape::kScissor;
    default:
        assert(!"never reach here");
        exit(1);
    }
}

template <typename T>
std::vector<Data> ParseInput(T &is) {
    std::vector<Data> ret;
    std::string tmp;
    while (std::getline(is, tmp)) {
        if (tmp.empty()) {
            break;
        }

        Data d{CharToShape(tmp[0]), CharToShape(tmp[2])};
        ret.push_back(d);
    }

    return ret;
}

int Problem1(const std::vector<Data> &data) {
    int score = 0;
    for (const Data &d : data) {
        if (d.opposite == d.me) {
            score += 3;
        } else {
            switch (d.opposite) {
            case Shape::kRock:
                if (d.me == Shape::kPaper) {
                    score += 6;
                }
                break;
            case Shape::kPaper:
                if (d.me == Shape::kScissor) {
                    score += 6;
                }
                break;
            case Shape::kScissor:
                if (d.me == Shape::kRock) {
                    score += 6;
                }
                break;
            }
        }

        score += static_cast<int>(d.me);
    }

    return score;
}

int Problem2(const std::vector<Data> &data) {
    int score = 0;
    for (const Data &d : data) {
        switch (d.me) {
        case Shape::kRock: // need to lose
            switch (d.opposite) {
            case Shape::kRock:
                score += 3;
                break;
            case Shape::kPaper:
                score += 1;
                break;
            case Shape::kScissor:
                score += 2;
                break;
            }
            break;
        case Shape::kPaper: // need to draw
            score += static_cast<int>(d.opposite) + 3;
            break;
        case Shape::kScissor: // need to win
            switch (d.opposite) {
            case Shape::kRock:
                score += 2;
                break;
            case Shape::kPaper:
                score += 3;
                break;
            case Shape::kScissor:
                score += 1;
                break;
            }
            score += 6;
            break;
        }
    }

    return score;
}

void test() {
    std::string s(R"(A Y
B X
C Z)");

    std::stringstream ss(s);
    auto data = ParseInput(ss);
    int ret1 = Problem1(data);
    assert(ret1 == 15);

    int ret2 = Problem2(data);
    assert(ret2 == 12);
}

int main() {
    test();

    auto data = ParseInput(std::cin);
    int ret1 = Problem1(data);
    int ret2 = Problem2(data);

    std::cout << "Problem1: " << ret1 << std::endl;
    std::cout << "Problem2: " << ret2 << std::endl;
    return 0;
}
