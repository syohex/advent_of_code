#include <iostream>
#include <string>
#include <cstdio>

int main() {
    std::string line;
    int first, second;
    char ch;
    char password[256];

    int ret = 0;
    while (std::getline(std::cin, line)) {
        sscanf(line.c_str(), "%d-%d %c: %s", &first, &second, &ch, password);

        std::string pw(password);

        if (password[first - 1] == ch && password[second - 1] == ch) {
            continue;
        }
        if (password[first - 1] == ch || password[second - 1] == ch) {
            ++ret;
        }
    }

    std::cout << ret << std::endl;
    return 0;
}
