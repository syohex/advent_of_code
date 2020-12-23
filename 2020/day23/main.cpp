#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

namespace {

struct ListNode {
    explicit ListNode(int n) : value(n), next(nullptr) {
    }

    void Dump() {
        ListNode *p = this;
        printf("[");
        while (true) {
            printf(" %d ", p->value);
            p = p->next;
            if (p == this) {
                break;
            }
        }
        printf("]\n");
    }

    int value;
    ListNode *next;
};

struct Cups {
    ListNode *head;
    size_t size;
};

ListNode *CreateCircularList(const std::vector<int> &nums) {
    ListNode *head = new ListNode(nums[0]);
    ListNode *p = head;
    for (size_t i = 1; i < nums.size(); ++i) {
        p->next = new ListNode(nums[i]);
        p = p->next;
    }

    p->next = head;
    return head;
}

ListNode *FindDestination(ListNode *head, size_t size, int target) {
    int min = INT_MAX;
    ListNode *p = head;
    for (size_t i = 0; i < size; ++i) {
        min = std::min(min, p->value);
        p = p->next;
    }

    while (target >= min) {
        p = head;
        for (size_t i = 0; i < size; ++i) {
            if (p->value == target) {
                return p;
            }

            p = p->next;
        }

        --target;
    }

    return nullptr;
}

ListNode *FindMaxValue(ListNode *head, size_t size) {
    int max = INT_MIN;
    ListNode *p = head;
    ListNode *ret = nullptr;
    for (size_t i = 0; i < size; ++i) {
        if (p->value > max) {
            ret = p;
            max = p->value;
        }

        p = p->next;
    }

    assert(ret != nullptr);
    return ret;
}

ListNode *PlayGame(const std::vector<int> &cups, int turns) {
    ListNode *head = CreateCircularList(cups);

    ListNode *current_cup = head;
    size_t length = cups.size();
    size_t remain_size = length - 3 - 1;
    for (int i = 0; i < turns; ++i) {
        int current = current_cup->value;
        int target = current - 1;

        ListNode *p = current_cup->next;
        ListNode *pick_up_head = p;
        ListNode *prev = nullptr;
        for (size_t j = 0; j < 3; ++j) {
            prev = p;
            p = p->next;
        }
        ListNode *pick_up_end = prev;

        ListNode *remain_head = p;
        ListNode *dest = FindDestination(remain_head, remain_size, target);
        if (dest == nullptr) {
            dest = FindMaxValue(remain_head, remain_size);
        }

        current_cup->next = remain_head;
        ListNode *tmp = dest->next;
        dest->next = pick_up_head;
        pick_up_end->next = tmp;

        current_cup = current_cup->next;

        size_t current_cup_index = (i + 1) % length;
        p = head;
        for (int j = 0; j < length; ++j) {
            if (p->value == current_cup->value) {
                for (size_t k = 0; k < length - current_cup_index; ++k) {
                    p = p->next;
                }

                head = p;
                break;
            }
            p = p->next;
        }
    }

    return head;
}

std::vector<int> ParseInput(const std::string &input) {
    std::vector<int> ret;
    for (char c : input) {
        ret.push_back(c - '0');
    }

    return ret;
}

std::string Solve01(const std::vector<int> &cups, int turns) {
    ListNode *head = PlayGame(cups, turns);

    ListNode *one = nullptr;
    ListNode *p = head;
    while (true) {
        if (p->value == 1) {
            one = p;
            p = p->next;
            break;
        }
        p = p->next;
    }

    std::string ret;
    while (p != one) {
        ret.push_back(p->value + '0');
        p = p->next;
    }

    return ret;
}

void Test01() {
    std::string input("389125467");
    auto cups = ParseInput(input);
    assert((cups == std::vector<int>{3, 8, 9, 1, 2, 5, 4, 6, 7}));

    assert(Solve01(cups, 10) == "92658374");
    assert(Solve01(cups, 100) == "67384529");
}

} // namespace

int main() {
    Test01();

    std::string input = "137826495";
    auto cups = ParseInput(input);
    std::cout << "Part01: " << Solve01(cups, 100) << std::endl;
    return 0;
}
