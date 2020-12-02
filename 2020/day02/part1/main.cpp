#include <iostream>
#include <string>
#include <map>
#include <cstdio>

int main() {
    std::string line;
    int min, max;
    char ch;
    char password[256];

    int ret = 0;
    while (std::getline(std::cin, line)) {
        sscanf(line.c_str(), "%d-%d %c: %s", &min, &max, &ch, password);

        std::map<char, int> m;
        std::string pw(password);
        for (const auto c : pw) {
            ++m[c];
        }

        if (m[ch] >= min && m[ch] <= max) {
            ++ret;
        }
    }

    std::cout << ret << std::endl;
    return 0;
}
