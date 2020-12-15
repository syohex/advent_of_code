#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cstdio>

namespace {

struct PasswordData {
    explicit PasswordData(int min, int max, char chr, std::string password)
        : min(min), max(max), chr(chr), password(std::move(password)) {
    }

    bool Valid01() const {
        int count = 0;
        for (auto c : password) {
            if (c == chr) {
                ++count;
            }
        }

        return count >= min && count <= max;
    }

    bool Valid02() const {
        return (password[min - 1] == chr && password[max - 1] != chr) || (password[min - 1] != chr && password[max - 1] == chr);
    }

    bool operator==(const PasswordData &other) const {
        return min == other.min && max == other.max && chr == other.chr && password == other.password;
    }

    int min;
    int max;
    char chr;
    std::string password;
};

std::vector<PasswordData> ParseInput(const std::vector<std::string> &inputs) {
    std::vector<PasswordData> ret;
    char password[256];
    for (const auto &input : inputs) {
        int min, max;
        char ch;
        sscanf(input.c_str(), "%d-%d %c: %s", &min, &max, &ch, password);

        ret.emplace_back(min, max, ch, password);
    }

    return ret;
}

int Solve01(const std::vector<PasswordData> &passwords) {
    int ret = 0;
    for (const auto &password : passwords) {
        if (password.Valid01()) {
            ++ret;
        }
    }

    return ret;
}

int Solve02(const std::vector<PasswordData> &passwords) {
    int ret = 0;
    for (const auto &password : passwords) {
        if (password.Valid02()) {
            ++ret;
        }
    }

    return ret;
}

void Test01() {
    // clang-format off
    std::vector<std::string> inputs {
        "1-3 a: abcde",
        "1-3 b: cdefg",
        "2-9 c: ccccccccc",
    };
    // clang-format on

    auto passwords = ParseInput(inputs);
    assert(passwords.size() == 3);
    assert((passwords[0] == PasswordData(1, 3, 'a', "abcde")));
    assert((passwords[1] == PasswordData(1, 3, 'b', "cdefg")));
    assert((passwords[2] == PasswordData(2, 9, 'c', "ccccccccc")));

    assert(passwords[0].Valid01());
    assert(!passwords[1].Valid01());
    assert(passwords[2].Valid01());

    assert(Solve01(passwords) == 2);
}

void Test02() {
    // clang-format off
    std::vector<std::string> inputs {
        "1-3 a: abcde",
        "1-3 b: cdefg",
        "2-9 c: ccccccccc",
    };
    // clang-format on

    auto passwords = ParseInput(inputs);
    assert(passwords[0].Valid02());
    assert(!passwords[1].Valid02());
    assert(!passwords[2].Valid02());

    assert(Solve02(passwords) == 1);
}

} // namespace

int main() {
    Test01();
    Test02();

    std::vector<std::string> inputs;
    std::string line;
    while (std::getline(std::cin, line)) {
        inputs.push_back(line);
    }

    auto passwords = ParseInput(inputs);

    auto answer01 = Solve01(passwords);
    assert(answer01 == 580);
    std::cout << "Part01: " << answer01 << std::endl;

    auto answer02 = Solve02(passwords);
    assert(answer02 == 611);
    std::cout << "Part02: " << answer02 << std::endl;

    return 0;
}
