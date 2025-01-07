#!/usr/bin/env python3


def part01(input: str) -> int:
    s: set[tuple[int, int]] = set()
    s.add((0, 0))

    x = 0
    y = 0
    for c in input:
        if c == "^":
            y += 1
        elif c == ">":
            x += 1
        elif c == "v":
            y -= 1
        elif c == "<":
            x -= 1

        s.add((x, y))

    return len(s)


def part02(input: str) -> int:
    s: set[tuple[int, int]] = set()
    s.add((0, 0))

    is_human = True
    x1 = 0
    y1 = 0
    x2 = 0
    y2 = 0
    for c in input:
        if is_human:
            if c == "^":
                y1 += 1
            elif c == ">":
                x1 += 1
            elif c == "v":
                y1 -= 1
            elif c == "<":
                x1 -= 1
            is_human = False
            s.add((x1, y1))
        else:
            if c == "^":
                y2 += 1
            elif c == ">":
                x2 += 1
            elif c == "v":
                y2 -= 1
            elif c == "<":
                x2 -= 1
            is_human = True
            s.add((x2, y2))

    return len(s)


with open("../input/day03.txt") as f:
    input = f.readline()

    ret1 = part01(input)
    assert ret1 == 2572

    ret2 = part02(input)
    assert ret2 == 2631

    print(f"ret1={ret1}")
    print(f"ret2={ret2}")
