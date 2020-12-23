#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <map>

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

    bool Equal(const std::vector<int> &values) {
        ListNode *p = this;
        size_t pos = 0;
        while (true) {
            if (p->value != values[pos++]) {
                return false;
            }

            p = p->next;

            if (p == this) {
                break;
            }
        }

        assert(pos == values.size());
        return true;
    }

    std::vector<int> ToVector() {
        std::vector<int> ret;
        ListNode *p = this;
        while (true) {
            ret.push_back(p->value);
            p = p->next;
            if (p == this) {
                break;
            }
        }

        return ret;
    }

    int value;
    ListNode *next;
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

std::string Solve01WithMap(const std::vector<int> &cups, int turns) {
    std::map<int, int> m;

    int min = INT_MAX;
    int max = INT_MIN;
    for (size_t i = 0; i < cups.size(); ++i) {
        min = std::min(min, cups[i]);
        max = std::max(max, cups[i]);
        if (i == cups.size() - 1) {
            m[cups[i]] = cups[0];
        } else {
            m[cups[i]] = cups[i + 1];
        }
    }

    int current_cup = cups[0];
    std::vector<int> pickups(3);
    for (int i = 0; i < turns; ++i) {
        pickups[0] = m[current_cup];
        pickups[1] = m[pickups[0]];
        pickups[2] = m[pickups[1]];

        m[current_cup] = m[pickups[2]];
        int target = current_cup == min ? max : current_cup - 1;
        while (true) {
            bool found = false;
            for (int pickup : pickups) {
                if (pickup == target) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                break;
            }

            do {
                target = target == min ? max : target - 1;
            } while (m.find(target) == m.end());
        }

        m[pickups[2]] = m[target];
        m[target] = pickups[0];
        current_cup = m[current_cup];
    }

    std::string ret;
    int next = m[1];
    while (true) {
        ret.push_back(next + '0');
        next = m[next];
        if (next == 1) {
            break;
        }
    }

    return ret;
}

std::int64_t Solve02(const std::vector<int> &cups, int max_cup, int turns) {
    int max = INT_MIN;
    for (int num : cups) {
        max = std::max(max, num);
    }

    std::map<int, int> m;
    for (size_t i = 0; i < max_cup; ++i) {
        if (i < cups.size() - 1) {
            m[cups[i]] = cups[i + 1];
        } else if (i == cups.size() - 1) {
            m[cups.back()] = max + 1;
        } else {
            m[i + 1] = i + 2;
        }
    }

    m[max_cup] = cups[0];
    int current_cup = cups.front();

    std::vector<int> pickups(3);
    for (int i = 0; i < turns; ++i) {
        pickups[0] = m[current_cup];
        pickups[1] = m[pickups[0]];
        pickups[2] = m[pickups[1]];

        m[current_cup] = m[pickups[2]];

        int target = current_cup == 1 ? max_cup : current_cup - 1;
        while (true) {
            bool found = false;
            for (int pickup : pickups) {
                if (target == pickup) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                break;
            }

            target = target == 1 ? max_cup : target - 1;
        }

        m[pickups[2]] = m[target];
        m[target] = pickups[0];
        current_cup = m[current_cup];
    }

    std::int64_t next = m[1];
    std::int64_t next_next = m[m[1]];
    return next * next_next;
}

void Test01() {
    std::string input("389125467");
    auto cups = ParseInput(input);
    assert((cups == std::vector<int>{3, 8, 9, 1, 2, 5, 4, 6, 7}));

    assert(Solve01(cups, 10) == "92658374");
    assert(Solve01(cups, 100) == "67384529");
    assert(Solve01WithMap(cups, 10) == "92658374");
    assert(Solve01WithMap(cups, 100) == "67384529");
}

void Test02() {
    std::string input("389125467");
    auto cups = ParseInput(input);
    assert(Solve02(cups, 1'000'000, 10'000'000) == 149245887792);
}

} // namespace

int main() {
    Test01();
    Test02();

    std::string input = "137826495";
    auto cups = ParseInput(input);
    std::cout << "Part01: " << Solve01(cups, 100) << std::endl;
    std::cout << "Part02: " << Solve02(cups, 1'000'000, 10'000'000) << std::endl;
    return 0;
}
