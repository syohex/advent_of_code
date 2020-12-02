#!/usr/bin/env python
import re
import sys


def main():
    ret = 0
    reg = re.compile(r'^(\d+)-(\d+)\s+(\w):\s+(\S+)')
    for line in sys.stdin:
        match = reg.search(line.strip())
        if match is None:
            raise Exception(f'Got unexpected line: {line}')

        (min_num, max_num, ch, password) = (int(match.group(1)), int(match.group(2)), match.group(3), match.group(4))

        d = {}
        for c in password:
            if c in d:
                d[c] += 1
            else:
                d[c] = 1

        if ch not in d:
            continue

        if d[ch] >= min_num and d[ch] <= max_num:
            ret += 1

    print(ret)


if __name__ == '__main__':
    main()
