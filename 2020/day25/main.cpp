#include <cassert>
#include <cstdint>
#include <cstdio>
#include <iostream>

namespace {

int LoopSize(int subject_number, std::int64_t public_key) {
    std::int64_t value = 1;
    for (int i = 0;; ++i) {
        value *= subject_number;
        value %= 20201227;

        if (value == public_key) {
            return i + 1;
        }
    }

    assert(!"never reach here");
    return -1;
}

std::int64_t EncryptKey(int subject_number, int loops) {
    std::int64_t value = 1;
    for (int i = 0; i < loops; ++i) {
        value *= subject_number;
        value %= 20201227;
    }

    return value;
}

std::int64_t Solve01(std::int64_t card_pubkey, std::int64_t door_pubkey) {
    int card_loop_size = LoopSize(7, card_pubkey);
    int door_loop_size = LoopSize(7, door_pubkey);

    auto encrypt_key1 = EncryptKey(door_pubkey, card_loop_size);
    auto encrypt_key2 = EncryptKey(card_pubkey, door_loop_size);
    assert(encrypt_key1 == encrypt_key2);
    return encrypt_key1;
}

void Test01() {
    std::int64_t card_pubkey = 5764801;
    std::int64_t door_pubkey = 17807724;

    assert(LoopSize(7, card_pubkey) == 8);
    assert(LoopSize(7, door_pubkey) == 11);

    auto encrypt_key = Solve01(card_pubkey, door_pubkey);
    assert(encrypt_key == 14897079);
}

} // namespace

int main() {
    Test01();

    std::int64_t card_pubkey, door_pubkey;
    std::cin >> card_pubkey >> door_pubkey;

    auto encrypt_key = Solve01(card_pubkey, door_pubkey);
    std::cout << "Part01: " << encrypt_key << std::endl;
    return 0;
}
