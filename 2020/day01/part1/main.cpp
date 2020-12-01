#include <iostream>
#include <set>

int main() {
    std::set<int> s;
    int n;

    while (std::cin >> n) {
        s.insert(n);
    }

    for (const auto num : s) {
        int v = 2020 - num;
        if (s.find(v) != s.end()) {
            std::cout << v * num << std::endl;
            return 0;
        }
    }

    return 0;
}
