#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <fstream>

struct Order {
    int moves;
    int from;
    int to;
};

struct Data {
    std::vector<std::vector<char>> stacks;
    std::vector<Order> orders;

    void Print() const {
        for (size_t i = 0; i < stacks.size(); ++i) {
            printf("[%zd] = [", i);
            for (char c : stacks[i]) {
                printf(" [%c] ", c);
            }
            printf("]\n");
        }
    }
};

template <typename T>
Data ParseInput(T &it) {
    Data ret;
    std::string line;
    bool initialized = false;

    while (std::getline(it, line)) {
        int len = line.size();
        if (!initialized) {
            ret.stacks.resize((len + 1) / 4);
            initialized = true;
        }

        if (line[1] == '1') {
            break;
        }

        for (int i = 0; i < len; i += 4) {
            ret.stacks[i / 4].push_back(line[i + 1]);
        }
    }

    for (auto &stack : ret.stacks) {
        std::reverse(stack.begin(), stack.end());

        while (!stack.empty() && stack.back() == ' ') {
            stack.pop_back();
        }
    }

    // skip empty line
    (void)std::getline(it, line);

    while (std::getline(it, line)) {
        if (line.empty()) {
            break;
        }

        Order order;
        sscanf(line.c_str(), "move %d from %d to %d", &order.moves, &order.from, &order.to);
        ret.orders.push_back(order);
    }

    return ret;
}

std::string Problem1(Data data) {
    for (const Order &order : data.orders) {
        for (int i = 0; i < order.moves; ++i) {
            int v = data.stacks[order.from - 1].back();
            data.stacks[order.from - 1].pop_back();
            data.stacks[order.to - 1].push_back(v);
        }
    }

    std::string ret;
    for (const auto &stack : data.stacks) {
        ret.push_back(stack.back());
    }

    return ret;
}

std::string Problem2(Data data) {
    for (const Order &order : data.orders) {
        std::vector<char> tmp;
        for (int i = 0; i < order.moves; ++i) {
            tmp.push_back(data.stacks[order.from - 1].back());
            data.stacks[order.from - 1].pop_back();
        }

        for (auto it = tmp.rbegin(); it != tmp.rend(); ++it) {
            data.stacks[order.to - 1].push_back(*it);
        }
    }

    std::string ret;
    for (const auto &stack : data.stacks) {
        ret.push_back(stack.back());
    }

    return ret;
}

void Test() {
    std::ifstream is("test.txt");
    auto data = ParseInput(is);
    auto ret1 = Problem1(data);
    auto ret2 = Problem2(data);

    assert(ret1 == "CMZ");
    assert(ret2 == "MCD");
}

int main() {
    Test();

    auto data = ParseInput(std::cin);
    auto ret1 = Problem1(data);
    auto ret2 = Problem2(data);

    std::cout << "Problem1: " << ret1 << std::endl;
    std::cout << "Problem2: " << ret2 << std::endl;
    return 0;
}
