#include <cassert>
#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <optional>

namespace {

bool IsValidPassport01(const std::map<std::string, std::string> &passport) {
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

std::optional<int> ParseInteger(const std::string &str) {
    std::optional<int> ret;
    try {
        int num = std::stoi(str);
        ret = num;
    } catch (...) {
    }

    return ret;
}

bool IsValidDigitString(const std::string &str, size_t digits, int min, int max) {
    if (str.size() != 4) {
        return false;
    }

    auto birth_year = ParseInteger(str);
    if (!birth_year.has_value()) {
        return false;
    }

    return *birth_year >= min && *birth_year <= max;
}

bool StringEndsWith(const std::string &str, const std::string &suffix) {
    return str.rfind(suffix) == str.size() - suffix.size();
}

bool IsValidHeightString(const std::string &str) {
    if (StringEndsWith(str, "cm")) {
        auto height_cm = ParseInteger(str.substr(0, str.size() - 2));
        if (!height_cm.has_value()) {
            return false;
        }

        return *height_cm >= 150 && *height_cm <= 193;
    }

    if (StringEndsWith(str, "in")) {
        auto height_in = ParseInteger(str.substr(0, str.size() - 2));
        if (!height_in.has_value()) {
            return false;
        }

        return *height_in >= 59 && *height_in <= 76;
    }

    return false;
}

bool IsValidHairColor(const std::string &color) {
    if (color.size() != 7) {
        return false;
    }

    if (color[0] != '#') {
        return false;
    }

    for (size_t i = 1; i < color.size(); ++i) {
        char c = color[i];
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))) {
            return false;
        }
    }

    return true;
}

bool IsValidEyeColor(const std::string &color) {
    const auto valid_colors = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
    for (const auto valid_color : valid_colors) {
        if (color == valid_color) {
            return true;
        }
    }

    return 0;
}

bool IsValidPassportID(const std::string &id) {
    if (id.size() != 9) {
        return false;
    }

    for (const auto c : id) {
        if (!(c >= '0' && c <= '9')) {
            return false;
        }
    }

    return true;
}

bool IsValidPassport02(const std::map<std::string, std::string> &passport) {
    if (!IsValidPassport01(passport)) {
        return false;
    }

    if (!IsValidDigitString(passport.at("byr"), 4, 1920, 2002)) {
        return false;
    }
    if (!IsValidDigitString(passport.at("iyr"), 4, 2010, 2020)) {
        return false;
    }
    if (!IsValidDigitString(passport.at("eyr"), 4, 2020, 2030)) {
        return false;
    }
    if (!IsValidHeightString(passport.at("hgt"))) {
        return false;
    }
    if (!IsValidHairColor(passport.at("hcl"))) {
        return false;
    }
    if (!IsValidEyeColor(passport.at("ecl"))) {
        return false;
    }

    return IsValidPassportID(passport.at("pid"));
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
int CountValidPassport01(T &stream) {
    std::string line;
    std::map<std::string, std::string> passport;
    int ret = 0;
    std::string key, value;
    while (std::getline(stream, line)) {
        if (line.empty()) {
            if (IsValidPassport01(passport)) {
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
        if (IsValidPassport01(passport)) {
            ++ret;
        }
    }

    return ret;
}

template <typename T>
int CountValidPassport02(T &stream) {
    std::string line;
    std::map<std::string, std::string> passport;
    int ret = 0;
    std::string key, value;
    while (std::getline(stream, line)) {
        if (line.empty()) {
            if (IsValidPassport02(passport)) {
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
        if (IsValidPassport02(passport)) {
            ++ret;
        }
    }

    return ret;
}

void test01() {
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

    assert(CountValidPassport01(ss) == 2);
}

void test02() {
    std::istringstream ss(R"(eyr:1972 cid:100
hcl:#18171d ecl:amb hgt:170 pid:186cm iyr:2018 byr:1926

iyr:2019
hcl:#602927 eyr:1967 hgt:170cm
ecl:grn pid:012533040 byr:1946

hcl:dab227 iyr:2012
ecl:brn hgt:182cm pid:021572410 eyr:2020 byr:1992 cid:277

hgt:59cm ecl:zzz
eyr:2038 hcl:74454a iyr:2023
pid:3556412378 byr:2007

pid:087499704 hgt:74in ecl:grn iyr:2012 eyr:2030 byr:1980
hcl:#623a2f

eyr:2029 ecl:blu cid:129 byr:1989
iyr:2014 pid:896056539 hcl:#a97842 hgt:165cm

hcl:#888785
hgt:164cm byr:2001 iyr:2015 cid:88
pid:545766238 ecl:hzl
eyr:2022

iyr:2010 hgt:158cm hcl:#b6652a ecl:blu byr:1944 eyr:2021 pid:093154719
)");

    assert(CountValidPassport02(ss) == 4);
}

} // namespace

int main() {
    test01();
    test02();

    std::string input;
    std::string line;
    while (std::getline(std::cin, line)) {
        input += line;
        input += "\n";
    }

    std::istringstream part01_ss(input);
    std::istringstream part02_ss(input);

    int part01 = CountValidPassport01(part01_ss);
    std::cout << "Part01: " << part01 << std::endl;

    int part02 = CountValidPassport02(part02_ss);
    std::cout << "Part02: " << part02 << std::endl;
    return 0;
}
