#include <cassert>
#include <iostream>
#include <string>
#include <set>

int Problem1(const std::string &input) {
    int len = input.size();
    for (int i = 0; i < len - 4; ++i) {
        std::set<char> s;
        s.insert(input[i]);
        s.insert(input[i + 1]);
        s.insert(input[i + 2]);
        s.insert(input[i + 3]);

        if (s.size() == 4) {
            return i + 4;
        }
    }

    return -1;
}

int Problem2(const std::string &input) {
    int len = input.size();
    for (int i = 0; i < len - 14; ++i) {
        std::set<char> s;
        for (int j = 0; j < 14; ++j) {
            s.insert(input[i + j]);
        }

        if (s.size() == 14) {
            return i + 14;
        }
    }

    return -1;
}

void Test() {
    assert(Problem1("mjqjpqmgbljsphdztnvjfqwrcgsmlb") == 7);
    assert(Problem1("bvwbjplbgvbhsrlpgdmjqwftvncz") == 5);
    assert(Problem1("nppdvjthqldpwncqszvftbrmjlhg") == 6);
    assert(Problem1("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg") == 10);
    assert(Problem1("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw") == 11);

    assert(Problem2("mjqjpqmgbljsphdztnvjfqwrcgsmlb") == 19);
    assert(Problem2("bvwbjplbgvbhsrlpgdmjqwftvncz") == 23);
    assert(Problem2("nppdvjthqldpwncqszvftbrmjlhg") == 23);
    assert(Problem2("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg") == 29);
    assert(Problem2("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw") == 26);
}

int main() {
    Test();

    std::string input;
    std::cin >> input;
    int ret1 = Problem1(input);
    int ret2 = Problem2(input);

    std::cout << "Problem1: " << ret1 << std::endl;
    std::cout << "Problem2: " << ret2 << std::endl;
    return 0;
}
