#include <cassert>
#include <iostream>
#include <string>
#include <cstdio>
#include <print>
#include <tuple>
#include <set>
#include <vector>
#include <algorithm>

struct Node {
    int val;
    Node *prev;
    Node *next;

    explicit Node(int val) : val{val}, prev{this}, next{this} {
    }

    explicit Node(int val, Node *prev, Node *next) : val{val}, prev{prev}, next{next} {
    }

    Node *insert_after(int val) {
        Node *new_node = new Node(val, this, next);
        next->prev = new_node;
        next = new_node;
        return new_node;
    }

    Node *remove() {
        prev->next = next;
        next->prev = prev;

        Node *tmp = next;
        next = nullptr;
        prev = nullptr;
        delete this;

        return tmp;
    }

    static void finalize(Node *n) {
        std::set<Node *> s;
        Node *p = n;
        while (true) {
            if (s.contains(p)) {
                break;
            }

            s.insert(p);
            Node *tmp = p;
            delete p;
            p = tmp->next;
        }
    }

    static void print(Node *n) {
        std::set<Node *> s;
        Node *p = n;
        std::print("[");
        while (true) {
            if (s.contains(p)) {
                std::println("]");
                break;
            }

            std::print(" {} ", p->val);
            s.insert(p);
            p = p->next;
        }
    }
};

auto parse_input(const std::string &input) -> std::tuple<int, int> {
    // 448 players; last marble is worth 71628 points
    int players;
    int points;
    std::sscanf(input.c_str(), "%d players; last marble is worth %d points", &players, &points);
    return std::tuple<int, int>({players, points});
}

auto problem1(int players, int points) -> int64_t {
    Node *head = new Node(0);

    std::vector<int64_t> player_scores(players, 0);
    int id = 0;

    Node *p = head;
    for (int i = 1; i <= points; ++i) {
        if (i % 23 == 0) {
            for (int j = 0; j < 7; ++j) {
                p = p->prev;
            }

            player_scores[id] += p->val + i;
            p = p->remove();
        } else {
            p = p->next->insert_after(i);
        }

        id = (id + 1) % players;
    }

    Node::finalize(p);
    return *std::max_element(player_scores.begin(), player_scores.end());
}

void test() {
    int ret = problem1(9, 25);
    assert(ret == 32);
    assert(problem1(10, 1618) == 8317);
    assert(problem1(13, 7999) == 146373);
    assert(problem1(17, 1104) == 2764);
    assert(problem1(21, 6111) == 54718);
    assert(problem1(30, 5807) == 37305);
}

int main() {
    test();

    std::string input;
    std::getline(std::cin, input);

    auto [players, points] = parse_input(input);
    auto ret1 = problem1(players, points);
    auto ret2 = problem1(players, points * 100);

    std::println("problem1 = {}", ret1);
    std::println("problem2 = {}", ret2);
    return 0;
}
