#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <utility>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <memory>

namespace {

struct Pair;
struct Route {
    Pair *parent;
    bool is_left;
};

struct Pair {
    Pair *left_;
    Pair *right_;
    int value_;

    Pair() : left_(nullptr), right_(nullptr) {
    }

    explicit Pair(Pair *left, Pair *right) : left_(left), right_(right) {
    }

    explicit Pair(int value) : Pair() {
        this->value_ = value;
    }

    ~Pair() {
        delete left_;
        delete right_;
    }

    int Magnitude() const noexcept {
        if (left_ == nullptr && right_ == nullptr) {
            return value_;
        }
        std::function<int(const Pair *node)> f;
        f = [&f](const Pair *node) -> int {
            if (node->left_ == nullptr && node->right_ == nullptr) {
                return node->value_;
            }

            return f(node->left_) * 3 + f(node->right_) * 2;
        };

        return left_->Magnitude() * 3 + right_->Magnitude() * 2;
    }

    bool IsLeaf() const noexcept {
        return left_ == nullptr && right_ == nullptr;
    }

    static void AddLeftNeighbor(const std::vector<Route> &routes, int value) {
        for (auto it = routes.rbegin(); it != routes.rend(); ++it) {
            if (!it->is_left) {
                Pair *p = it->parent->left_;
                while (!p->IsLeaf()) {
                    p = p->right_;
                }

                p->value_ += value;
                return;
            }
        }
    }

    static void AddRightNeighbor(const std::vector<Route> &routes, int value) {
        for (auto it = routes.rbegin(); it != routes.rend(); ++it) {
            if (it->is_left) {
                Pair *p = it->parent->right_;
                while (!p->IsLeaf()) {
                    p = p->left_;
                }

                p->value_ += value;
                return;
            }
        }
    }

    void Explode() {
        std::function<bool(Pair * node, int depth, std::vector<Route> &routes)> f;
        f = [&f](Pair *node, int depth, std::vector<Route> &routes) -> bool {
            if (node->IsLeaf()) {
                return false;
            }

            if (depth >= 4) {
                AddLeftNeighbor(routes, node->left_->value_);
                AddRightNeighbor(routes, node->right_->value_);
                return true;
            }

            routes.push_back({node, true});
            if (f(node->left_, depth + 1, routes)) {
                delete node->left_;
                node->left_ = new Pair(0);
            }

            routes.back().is_left = false;
            if (f(node->right_, depth + 1, routes)) {
                delete node->right_;
                node->right_ = new Pair(0);
            }
            routes.pop_back();

            return false;
        };

        std::vector<Route> routes;
        (void)f(this, 0, routes);
    }

    static Pair *ConcatPair(Pair *p1, Pair *p2) {
        std::function<void(Pair * node, Pair * copied)> f;
        f = [&f](Pair *node, Pair *copied) {
            if (node->IsLeaf()) {
                copied->value_ = node->value_;
                return;
            }

            copied->left_ = new Pair();
            f(node->left_, copied->left_);
            copied->right_ = new Pair();
            f(node->right_, copied->right_);
        };

        Pair *ret = new Pair();
        ret->left_ = new Pair();
        ret->right_ = new Pair();

        f(p1, ret->left_);
        f(p2, ret->right_);
        return ret;
    }

    bool Split() {
        if (IsLeaf()) {
            if (value_ < 10) {
                return false;
            }

            left_ = new Pair(value_ / 2.0);
            right_ = new Pair(std::round(value_ / 2.0));
            return true;
        }

        if (left_->Split()) {
            // change only left most value
            return true;
        }

        return right_->Split();
    }

    Pair *Add(Pair *other) {
        Pair *ret = ConcatPair(this, other);
        while (true) {
            ret->Explode();
            if (!ret->Split()) {
                break;
            }
        }
        return ret;
    }
};

std::unique_ptr<Pair> ParsePair(const std::string &input) {
    std::vector<Pair *> stack;

    Pair *p = new Pair();
    for (char c : input) {
        if (c == '[') {
            if (p->left_ == nullptr) {
                p->left_ = new Pair();
                stack.push_back(p);
                p = p->left_;
            } else {
                p->right_ = new Pair();
                stack.push_back(p);
                p = p->right_;
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
            if (p->left_ == nullptr) {
                p->left_ = new Pair(value);
            } else {
                p->right_ = new Pair(value);
            }
        }
    }

    return std::unique_ptr<Pair>(p->left_);
}

std::vector<std::unique_ptr<Pair>> ParseInput(std::istream &ss) {
    std::vector<std::unique_ptr<Pair>> ret;
    std::string tmp;
    while (std::getline(ss, tmp)) {
        if (tmp.empty()) {
            continue;
        }

        ret.push_back(ParsePair(tmp));
    }

    return ret;
}

void PrintPair(const Pair *root) {
    std::function<void(const Pair *node)> f;
    f = [&f](const Pair *node) {
        if (node->IsLeaf()) {
            printf("%d", node->value_);
            return;
        }

        printf("[");
        f(node->left_);
        printf(",");
        f(node->right_);
        printf("]");
    };

    f(root);
    printf("\n");
}

int Part1(const std::vector<std::unique_ptr<Pair>> &pairs) {
    Pair *base = pairs[0].get();
    for (size_t i = 1; i < pairs.size(); ++i) {
        base = base->Add(pairs[i].get());
    }
    return base->Magnitude();
}

int Part2(const std::vector<std::unique_ptr<Pair>> &pairs) {
    int ret = 0;
    int len = pairs.size();

    for (int i = 0; i < len - 1; ++i) {
        for (int j = i + 1; j < len; ++j) {
            if (i == j) {
                continue;
            }

            std::unique_ptr<Pair> p1(pairs[i]->Add(pairs[j].get()));
            std::unique_ptr<Pair> p2(pairs[j]->Add(pairs[i].get()));
            ret = std::max({ret, p1->Magnitude(), p2->Magnitude()});
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
            auto pair = ParsePair(t.input);
            assert(pair->Magnitude() == t.expected);
        }
    }
    {
        auto p1 = ParsePair("[[[[4,3],4],4],[7,[[8,4],9]]]");
        auto p2 = ParsePair("[1,1]");
        auto p3 = p1->Add(p2.get());
        PrintPair(p3);
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