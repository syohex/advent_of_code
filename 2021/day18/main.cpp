#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <utility>
#include <cmath>
#include <sstream>
#include <algorithm>

namespace {

struct Pair {
    Pair *left;
    Pair *right;
    int value;

    Pair() : left(nullptr), right(nullptr) {
    }

    explicit Pair(Pair *left, Pair *right) : left(left), right(right) {
    }

    explicit Pair(int value) : Pair() {
        this->value = value;
    }

    ~Pair() {
        delete left;
        delete right;
    }

    int Magnitude() const {
        if (left == nullptr && right == nullptr) {
            return value;
        }

        int left_value = left->Magnitude();
        int right_value = right->Magnitude();

        return left_value * 3 + right_value * 2;
    }

    bool Explode(int depth, int &count, Pair *head) {
        if (left == nullptr && right == nullptr) {
            count += 1;
            return false;
        }

        if (depth >= 4) {
            int counter = 0;
            head->AddNthElement(count, left->value, counter);
            counter = 0;
            head->AddNthElement(count + 3, right->value, counter);
            return true;
        }

        if (left->Explode(depth + 1, count, head)) {
            delete left;
            left = new Pair(0);
            count += 1;
        }

        if (right->Explode(depth + 1, count, head)) {
            delete right;
            right = new Pair(0);
            count += 1;
        }

        return false;
    }

    void AddNthElement(int n, int val, int &count) {
        if (left == nullptr && right == nullptr) {
            count += 1;
            if (n == count) {
                value += val;
            }
            return;
        }
        if (count > n) {
            return;
        }

        left->AddNthElement(n, val, count);
        right->AddNthElement(n, val, count);
    }

    bool Split() {
        if (left == nullptr && right == nullptr) {
            if (value >= 10) {
                left = new Pair(value / 2.0);
                right = new Pair(std::round(value / 2.0));
                return true;
            }

            return false;
        }

        return left->Split() || right->Split();
    }

    Pair *Add(Pair *other) {
        Pair *ret = new Pair(this, other);

        while (true) {
            int count = 0;
            ret->Explode(0, count, ret);
            if (!ret->Split()) {
                break;
            }
        }

        return ret;
    }

    Pair *Copy() {
        std::function<void(Pair * n, Pair * *p)> f;
        f = [&](Pair *n, Pair **p) {
            if (n->left == nullptr && n->right == nullptr) {
                (*p)->value = n->value;
                return;
            }

            Pair *left = new Pair();
            f(n->left, &left);
            (*p)->left = left;

            Pair *right = new Pair();
            f(n->right, &right);
            (*p)->right = right;
        };

        Pair *p = new Pair();
        f(this, &p);
        return p;
    }

    void Print() {
        if (left == nullptr && right == nullptr) {
            printf(" %d ", value);
            return;
        }

        printf("[");
        left->Print();
        printf(",");
        right->Print();
        printf("]");
    }
};

Pair *ParsePair(const std::string &input) {
    std::vector<Pair *> stack;

    Pair *p = new Pair();
    for (char c : input) {
        if (c == '[') {
            if (p->left == nullptr) {
                p->left = new Pair();
                stack.push_back(p);
                p = p->left;
            } else {
                p->right = new Pair();
                stack.push_back(p);
                p = p->right;
            }

            continue;
        }
        if (c == ']') {
            p = stack.back();
            stack.pop_back();
            continue;
        }
        if (c == ',' || c == ' ') {
            // ignore
            continue;
        }

        if (c >= '0' && c <= '9') {
            int value = c - '0';
            if (p->left == nullptr) {
                p->left = new Pair(value);
            } else {
                p->right = new Pair(value);
            }
        }
    }

    return p->left;
}

std::vector<Pair *> ParseInput(std::istream &ss) {
    std::vector<Pair *> ret;
    std::string tmp;
    while (std::getline(ss, tmp)) {
        if (tmp.empty()) {
            continue;
        }

        ret.push_back(ParsePair(tmp));
    }

    return ret;
}

int Part1(const std::vector<Pair *> &pairs) {
    Pair *base = pairs[0];
    for (size_t i = 1; i < pairs.size(); ++i) {
        base = base->Add(pairs[i]);
    }

    return base->Magnitude();
}

int Part2(const std::vector<Pair *> &pairs) {
    int ret = 0;
    int len = pairs.size();

    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < len; ++j) {
            if (i == j) {
                continue;
            }

            auto *p1 = pairs[i]->Copy();
            auto *p2 = pairs[j]->Copy();
            auto p = p1->Add(p2);
            ret = std::max(ret, p->Magnitude());
        }
    }

    return ret;
}

void Test() {
    {
        struct TestData {
            std::string input;
            int expected;
        } tests[] = {{"[[1,2],[[3,4],5]]", 143},
                     {"[[[[0,7],4],[[7,8],[6,0]]],[8,1]]", 1384},
                     {"[[[[1,1],[2,2]],[3,3]],[4,4]]", 445},
                     {"[[[[3,0],[5,3]],[4,4]],[5,5]]", 791},
                     {"[[[[5,0],[7,4]],[5,5]],[6,6]]", 1137},
                     {"[[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]]", 3488}};
        for (const auto &t : tests) {
            auto *pair = ParsePair(t.input);
            assert(pair->Magnitude() == t.expected);
            delete pair;
        }
    }
    {
        std::string input(R"([[[0,[4,5]],[0,0]],[[[4,5],[2,6]],[9,5]]]
[7,[[[3,7],[4,3]],[[6,3],[8,8]]]]
[[2,[[0,8],[3,4]]],[[[6,7],1],[7,[1,6]]]]
[[[[2,4],7],[6,[0,5]]],[[[6,8],[2,8]],[[2,1],[4,5]]]]
[7,[5,[[3,8],[1,4]]]]
[[2,[2,2]],[8,[8,1]]]
[2,9]
[1,[[[9,3],9],[[9,0],[0,7]]]]
[[[5,[7,4]],7],1]
[[[[4,2],2],6],[8,7]])");

        std::stringstream ss(input);
        auto pairs = ParseInput(ss);
        auto part1 = Part1(pairs);
        assert(part1 == 3488);
    }
    {
        std::string input(R"([[[0,[5,8]],[[1,7],[9,6]]],[[4,[1,2]],[[1,4],2]]]
[[[5,[2,8]],4],[5,[[9,9],0]]]
[6,[[[6,2],[5,6]],[[7,6],[4,7]]]]
[[[6,[0,7]],[0,9]],[4,[9,[9,0]]]]
[[[7,[6,4]],[3,[1,3]]],[[[5,5],1],9]]
[[6,[[7,3],[3,2]]],[[[3,8],[5,7]],4]]
[[[[5,4],[7,7]],8],[[8,3],8]]
[[9,3],[[9,9],[6,[4,9]]]]
[[2,[[7,7],7]],[[5,8],[[9,3],[0,2]]]]
[[[[5,2],5],[8,[3,7]]],[[5,[7,5]],[4,4]]])");

        std::stringstream ss(input);
        auto pairs = ParseInput(ss);
        auto part1 = Part1(pairs);
        assert(part1 == 4140);
    }
    {
        std::string input(R"([[[0,[5,8]],[[1,7],[9,6]]],[[4,[1,2]],[[1,4],2]]]
[[[5,[2,8]],4],[5,[[9,9],0]]]
[6,[[[6,2],[5,6]],[[7,6],[4,7]]]]
[[[6,[0,7]],[0,9]],[4,[9,[9,0]]]]
[[[7,[6,4]],[3,[1,3]]],[[[5,5],1],9]]
[[6,[[7,3],[3,2]]],[[[3,8],[5,7]],4]]
[[[[5,4],[7,7]],8],[[8,3],8]]
[[9,3],[[9,9],[6,[4,9]]]]
[[2,[[7,7],7]],[[5,8],[[9,3],[0,2]]]]
[[[[5,2],5],[8,[3,7]]],[[5,[7,5]],[4,4]]])");

        std::stringstream ss(input);
        auto pairs = ParseInput(ss);
        auto part2 = Part2(pairs);
        assert(part2 == 3993);
    }
}

} // namespace

int main() {
    Test();

    std::string input;
    std::string tmp;
    while (std::getline(std::cin, tmp)) {
        input += tmp + "\n";
    }

    std::stringstream ss1(input);
    std::stringstream ss2(input);
    auto pairs1 = ParseInput(ss1);
    auto pairs2 = ParseInput(ss2);
    auto part1 = Part1(pairs1);
    auto part2 = Part2(pairs2);

    std::cout << "Part1: " << part1 << std::endl;
    std::cout << "Part2: " << part2 << std::endl;
    return 0;
}