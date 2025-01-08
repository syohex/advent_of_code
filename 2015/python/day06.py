#!/usr/bin/env python3
from enum import Enum
import dataclasses


class Action(Enum):
    ON = 1
    OFF = 2
    TOGGLE = 3


@dataclasses.dataclass
class Data:
    action: Action
    start: tuple[int, int]
    end: tuple[int, int]


def parse(s: str) -> Data:
    parts = s.split(None)
    if parts[0] == "turn":
        action = Action.ON if parts[1] == "on" else Action.OFF
        starts = parts[2].split(",")
        ends = parts[4].split(",")
        return Data(
            action, (int(starts[0]), int(starts[1])), (int(ends[0]), int(ends[1]))
        )
    else:
        starts = parts[1].split(",")
        ends = parts[3].split(",")
        return Data(
            Action.TOGGLE,
            (int(starts[0]), int(starts[1])),
            (int(ends[0]), int(ends[1])),
        )


def problem1(actions: list[Data]) -> int:
    matrix = [[False] * 1000 for _ in range(0, 1000)]
    for d in actions:
        match d.action:
            case Action.ON:
                for x in range(d.start[0], d.end[0] + 1):
                    for y in range(d.start[1], d.end[1] + 1):
                        matrix[x][y] = True
            case Action.OFF:
                for x in range(d.start[0], d.end[0] + 1):
                    for y in range(d.start[1], d.end[1] + 1):
                        matrix[x][y] = False
            case Action.TOGGLE:
                for x in range(d.start[0], d.end[0] + 1):
                    for y in range(d.start[1], d.end[1] + 1):
                        matrix[x][y] = not matrix[x][y]

    return sum([len([x for x in row if x]) for row in matrix])


def problem2(actions: list[Data]) -> int:
    matrix = [[0] * 1000 for _ in range(0, 1000)]
    for d in actions:
        match d.action:
            case Action.ON:
                for x in range(d.start[0], d.end[0] + 1):
                    for y in range(d.start[1], d.end[1] + 1):
                        matrix[x][y] += 1
            case Action.OFF:
                for x in range(d.start[0], d.end[0] + 1):
                    for y in range(d.start[1], d.end[1] + 1):
                        matrix[x][y] = max(0, matrix[x][y] - 1)
            case Action.TOGGLE:
                for x in range(d.start[0], d.end[0] + 1):
                    for y in range(d.start[1], d.end[1] + 1):
                        matrix[x][y] += 2

    return sum([sum(row) for row in matrix])


with open("../input/day06.txt") as f:
    input = [parse(line) for line in f.readlines()]
    ret1 = problem1(input)
    assert ret1 == 400410

    ret2 = problem2(input)
    assert ret2 == 15343601

    print(f"ret1={ret1}")
    print(f"ret2={ret2}")
