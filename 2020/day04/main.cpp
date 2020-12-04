#include <cassert>
#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

namespace {

bool IsValidPassport(const std::map<std::string, std::string> &passport) {
    auto mandatory_fields = {
        "byr", // Birth Year
        "iyr", // Issue Year
        "eyr", // Expiration Year
        "hgt", // Height
        "hcl", // Hair color
        "ecl", // Eye color
        "pid", // Passport ID
    };

    for (const auto field : mandatory_fields) {
        if (passport.find(field) == passport.end()) {
            return false;
        }
    }
    return true;
}

std::vector<std::string> SplitBySpace(const std::string &str) {
    std::vector<std::string> v;
    size_t i = 0;
    while (i < str.size()) {
        while (i < str.size() && str[i] == ' ') {
            ++i;
        }

        size_t start = i;
        while (i < str.size() && str[i] != ' ') {
            ++i;
        }

        v.push_back(str.substr(start, i - start));
    }
    return v;
}

template <typename T>
int CountValidPassport(T &stream) {
    std::string line;
    std::map<std::string, std::string> passport;
    int ret = 0;
    std::string key, value;
    while (std::getline(stream, line)) {
        if (line.empty()) {
            if (IsValidPassport(passport)) {
                ++ret;
            }
            passport.clear();
            continue;
        }

        auto words = SplitBySpace(line);
        for (const auto &word : words) {
            auto pos = word.find(':');
            key = word.substr(0, pos);
            value = word.substr(pos + 1);
            passport[key] = value;
        }
    }

    if (!passport.empty()) {
        if (IsValidPassport(passport)) {
            ++ret;
        }
    }

    return ret;
}

void test() {
    std::istringstream ss(R"(ecl:gry pid:860033327 eyr:2020 hcl:#fffffd
byr:1937 iyr:2017 cid:147 hgt:183cm

iyr:2013 ecl:amb cid:350 eyr:2023 pid:028048884
hcl:#cfa07d byr:1929

hcl:#ae17e1 iyr:2013
eyr:2024
ecl:brn pid:760753108 byr:1931
hgt:179cm

hcl:#cfa07d eyr:2025 pid:166559648
iyr:2011 ecl:brn hgt:59in
)");

    assert(CountValidPassport(ss) == 2);
}

} // namespace

int main() {
    test();

    int part01 = CountValidPassport(std::cin);
    std::cout << "Part01: " << part01 << std::endl;
    return 0;
}
