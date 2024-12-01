#!/usr/bin/env python3
import sys


def problem1(s: str) -> int:
    ret = 0
    for c in s:
        if c == "(":
            ret += 1
        else:
            ret -= 1
    return ret


def problem2(s: str) -> int:
    pos = 0
    for i, c in enumerate(s):
        if c == "(":
            pos += 1
        else:
            pos -= 1

        if pos == -1:
            return i + 1

    raise Exception("never reach here")


def test():
    def check(s: str, got: int, expected: int):
        try:
            assert got == expected
        except AssertionError:
            print(f"input={s} got={got}, expected={expected}")
            sys.exit(1)

    test_data1 = [
        ("(())", 0),
        ("()()", 0),
        ("(((", 3),
        ("(()(()(", 3),
        ("))(((((", 3),
        ("())", -1),
        ("))(", -1),
        (")))", -3),
        (")())())", -3),
    ]
    for s, expected in test_data1:
        check(s, problem1(s), expected)

    test_data2 = [(")", 1), ("()())", 5)]
    for s, expected in test_data2:
        check(s, problem2(s), expected)


test()


with open("../input/day01.txt") as f:
    s = f.readline()
    ret1 = problem1(s)
    print(f"ret1={ret1} expected=232")

    ret2 = problem2(s)
    print(f"ret2={ret2}, expected=1783")
