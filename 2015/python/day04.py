#!/usr/bin/env python3
import hashlib
from typing import assert_never


def find_md5hash(input: str, zeros: int) -> int:
    zeros_str = "0" * zeros
    num = 1
    while True:
        d = input + str(num)
        s = hashlib.md5(d.encode()).hexdigest()
        if s.startswith(zeros_str):
            return num

        num += 1

    assert_never("never reach here")


input = "yzbqklnj"
ret1 = find_md5hash(input, 5)
assert ret1 == 282749

ret2 = find_md5hash(input, 6)
assert ret2 == 9962624

print(f"ret1={ret1}")
print(f"ret2={ret2}")
