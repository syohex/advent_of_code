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

void Test() {
    assert(Problem1("mjqjpqmgbljsphdztnvjfqwrcgsmlb") == 7);
    assert(Problem1("bvwbjplbgvbhsrlpgdmjqwftvncz") == 5);
    assert(Problem1("nppdvjthqldpwncqszvftbrmjlhg") == 6);
    assert(Problem1("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg") == 10);
    assert(Problem1("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw") == 11);
}

int main() {
    Test();

    std::string input;
    std::cin >> input;
    int ret1 = Problem1(input);

    std::cout << "Problem1: " << ret1 << std::endl;
    return 0;
}
