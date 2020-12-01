#include <iostream>
#include <map>
#include <vector>

int main() {
    int n;
    std::vector<int> v;
    while (std::cin >> n) {
        v.push_back(n);
    }

    std::map<int, std::vector<int>> m;
    for (size_t i = 0; i < v.size() - 1; ++i) {
        for (size_t j = i + 1; j < v.size(); ++j) {
            int sum = v[i] + v[j];
            m[sum] = std::vector<int>{v[i], v[j]};
        }
    }

    for (auto i : v) {
        int n = 2020 - i;
        if (m.find(n) != m.end()) {
            std::cout << i * m[n][0] * m[n][1] << std::endl;
            return 0;
        }
    }

    return 0;
}
