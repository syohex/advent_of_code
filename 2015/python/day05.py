#!/usr/bin/env python3


def is_vowel(c: str) -> bool:
    return c in ["a", "e", "i", "o", "u"]


def condition1(s: str) -> bool:
    return len([c for c in s if is_vowel(c)]) >= 3


def condition2(s: str) -> bool:
    for i in range(0, len(s) - 1):
        if s[i] == s[i + 1]:
            return True

    return False


def condition3(s: str) -> bool:
    not_oks = ["ab", "cd", "pq", "xy"]
    for not_ok in not_oks:
        if not_ok in s:
            return False

    return True


def problem1(input: list[str]) -> int:
    return len([s for s in input if condition1(s) and condition2(s) and condition3(s)])


def condition4(s: str) -> bool:
    for i in range(0, len(s) - 3):
        pair = s[i : i + 2]
        if s[i + 2 :].find(pair) != -1:
            return True

    return False


def condition5(s: str) -> bool:
    for i in range(0, len(s) - 2):
        if s[i] == s[i + 2]:
            return True

    return False


def problem2(input: list[str]) -> int:
    return len([s for s in input if condition4(s) and condition5(s)])


with open("../input/day05.txt") as f:
    input = f.readlines()

    ret1 = problem1(input)
    assert ret1 == 258
    print(f"ret1={ret1}")

    ret2 = problem2(input)
    assert ret2 == 53
    print(f"ret2={ret2}")
