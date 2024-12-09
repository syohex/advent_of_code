#include <cassert>
#include <string>
#include <md5.h>

using namespace std;

int FindLeadingZeroMD5(const string &input, int zeros) {
    const string prefix(zeros, '0');
    std::string tmp(MD5_DIGEST_LENGTH, '\0');

    for (int i = 1;; ++i) {
        MD5_CTX ctx;
        string data = input + std::to_string(i);

        MD5Init(&ctx);
        MD5Update(&ctx, reinterpret_cast<const uint8_t *>(data.data()), data.size());
        MD5End(&ctx, &tmp[0]);

        if (tmp.starts_with(prefix)) {
            return i;
        }
    }

    assert(!"never reach here");
    return -1;
}

int Problem1(const std::string &input) {
    return FindLeadingZeroMD5(input, 5);
}

int Problem2(const std::string& input) {
    return FindLeadingZeroMD5(input, 6);
}

void Test() {
    assert(Problem1("abcdef") == 609043);
    assert(Problem1("pqrstuv") == 1048970);
}

int main() {
    Test();

    const string input("yzbqklnj");
    int ret1 = Problem1(input);
    assert(ret1 == 282749);
    int ret2 = Problem2(input);
    assert(ret2 == 9962624);

    printf("ret1 = %d\n", ret1);
    printf("ret2 = %d\n", ret2);
    return 0;
}
