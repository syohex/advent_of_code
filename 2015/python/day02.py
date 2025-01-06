#!/usr/bin/env python3


def parse(s: str) -> tuple[int, int, int]:
    parts = [int(x) for x in s.split("x")]
    return parts[0], parts[1], parts[2]


def square_feet(a: tuple[int, int, int]) -> int:
    sorted_vals = sorted(a)
    smallest_area = sorted_vals[0] * sorted_vals[1]
    return 2 * a[0] * a[1] + 2 * a[1] * a[2] + 2 * a[2] * a[0] + smallest_area


def part1(input: list[tuple[int, int, int]]) -> int:
    return sum([square_feet(v) for v in input])


def ribbon_length(a: tuple[int, int, int]) -> int:
    sorted_vals = sorted(a)
    ribbon = 2 * sorted_vals[0] + 2 * sorted_vals[1]
    return a[0] * a[1] * a[2] + ribbon


def part2(input: list[tuple[int, int, int]]) -> int:
    return sum([ribbon_length(v) for v in input])


with open("../input/day02.txt") as f:
    lines = f.readlines()
    input = [parse(line) for line in lines]

    ret1 = part1(input)
    assert ret1 == 1598415
    print(f"ret1={ret1}")

    ret2 = part2(input)
    assert ret2 == 3812909
    print(f"ret2={ret2}")
