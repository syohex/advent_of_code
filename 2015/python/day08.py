#!/usr/bin/env python3


def count_real_chars(s: str) -> int:
    s = s[1 : len(s) - 1]
    i = 0
    ret = 0
    while i < len(s):
        match s[i]:
            case "\\":
                i += 1
                match s[i]:
                    case '"' | "\\":
                        i += 1
                        ret += 1
                    case "x":
                        i += 3
                        ret += 1
            case _:
                i += 1
                ret += 1

    return ret


def problem1(lines: list[str]) -> int:
    return sum([len(s) - count_real_chars(s) for s in lines])


def count_encoded_chars(s: str) -> int:
    ret = 2
    for c in s:
        match c:
            case '"' | "\\":
                ret += 2
            case _:
                ret += 1
    return ret


def problem2(lines: list[str]) -> int:
    return sum([count_encoded_chars(s) - len(s) for s in lines])


with open("../input/day08.txt") as f:
    lines = f.readlines()

    ret1 = problem1(lines)
    assert ret1 == 1371

    ret2 = problem2(lines)
    assert ret2 == 2117

    print(f"ret1={ret1}")
    print(f"ret2={ret2}")
