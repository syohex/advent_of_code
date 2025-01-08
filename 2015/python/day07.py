#!/usr/bin/env python3
from abc import ABC, abstractmethod
from typing import override
import ctypes


class Operand:
    def __init__(self, val=str):
        self.val = int(val) if val.isdecimal() else val

    def can_eval(self, env: dict[str, int]) -> bool:
        if isinstance(self.val, int):
            return True

        return self.val in env

    def eval(self, env: dict[str, int]) -> int:
        if isinstance(self.val, int):
            return self.val

        return env[self.val]


class Instruction(ABC):
    @property
    def dest(self) -> str:
        return self._dest

    @dest.setter
    def dest(self, val: str):
        self._dest = val

    @abstractmethod
    def eval(self, env: dict[str, int]) -> bool:
        pass


class Assignment(Instruction):
    def __init__(self, dest: str, src: Operand):
        super().__init__()
        self.dest = dest
        self.src = src

    @override
    def eval(self, env: dict[str, int]) -> bool:
        if not self.src.can_eval(env):
            return False

        env[self.dest] = self.src.eval(env)
        return True


class BinaryComplement(Instruction):
    def __init__(self, dest: str, src: Operand):
        super().__init__()
        self.dest = dest
        self.src = src

    @override
    def eval(self, env: dict[str, int]) -> bool:
        if not self.src.can_eval(env):
            return False

        v = ctypes.c_uint16(~self.src.eval(env))
        env[self.dest] = v.value
        return True


class BinaryAnd(Instruction):
    def __init__(self, dest: str, src1: Operand, src2: Operand):
        super().__init__()
        self.dest = dest
        self.src1 = src1
        self.src2 = src2

    @override
    def eval(self, env: dict[str, int]) -> bool:
        if not (self.src1.can_eval(env) and self.src2.can_eval(env)):
            return False

        v = self.src1.eval(env) & self.src2.eval(env)
        env[self.dest] = v
        return True


class BinaryOr(Instruction):
    def __init__(self, dest: str, src1: Operand, src2: Operand):
        super().__init__()
        self.dest = dest
        self.src1 = src1
        self.src2 = src2

    @override
    def eval(self, env: dict[str, int]) -> bool:
        if not (self.src1.can_eval(env) and self.src2.can_eval(env)):
            return False

        v = self.src1.eval(env) | self.src2.eval(env)
        env[self.dest] = v
        return True


class LeftShift(Instruction):
    def __init__(self, dest: str, src1: Operand, src2: Operand):
        super().__init__()
        self.dest = dest
        self.src1 = src1
        self.src2 = src2

    @override
    def eval(self, env: dict[str, int]) -> bool:
        if not (self.src1.can_eval(env) and self.src2.can_eval(env)):
            return False

        v = self.src1.eval(env) << self.src2.eval(env)
        env[self.dest] = v
        return True


class RightShift(Instruction):
    def __init__(self, dest: str, src1: Operand, src2: Operand):
        super().__init__()
        self.dest = dest
        self.src1 = src1
        self.src2 = src2

    @override
    def eval(self, env: dict[str, int]) -> bool:
        if not (self.src1.can_eval(env) and self.src2.can_eval(env)):
            return False

        v = self.src1.eval(env) >> self.src2.eval(env)
        env[self.dest] = v
        return True


def parse(s: str) -> Instruction:
    parts = s.split(None)
    if len(parts) == 3:
        return Assignment(parts[2], Operand(parts[0]))
    elif len(parts) == 4:
        return BinaryComplement(parts[3], Operand(parts[1]))
    else:
        match parts[1]:
            case "AND":
                return BinaryAnd(parts[4], Operand(parts[0]), Operand(parts[2]))
            case "OR":
                return BinaryOr(parts[4], Operand(parts[0]), Operand(parts[2]))
            case "LSHIFT":
                return LeftShift(parts[4], Operand(parts[0]), Operand(parts[2]))
            case "RSHIFT":
                return RightShift(parts[4], Operand(parts[0]), Operand(parts[2]))
            case _:
                raise Exception(f"unknown insn: {parts[1]} => {s}")


def problem1(insns: list[Instruction]) -> int:
    env: dict[str, int] = {}
    while True:
        for insn in insns:
            insn.eval(env)

        if "a" in env:
            return env["a"]


def problem2(insns: list[Instruction], init: int) -> int:
    env = {"b": init}
    while True:
        for insn in insns:
            if insn.dest not in env:
                insn.eval(env)

        if "a" in env:
            return env["a"]


with open("../input/day07.txt") as f:
    insns = [parse(s) for s in f.readlines()]
    ret1 = problem1(insns)
    assert ret1 == 16076

    ret2 = problem2(insns, ret1)
    assert ret2 == 2797

    print(f"ret1={ret1}")
    print(f"ret2={ret2}")
