#!/usr/bin/env bash

EXE=
if [[ $OSTYPE == "msys" ]]; then
    EXE="./a.exe"
else
    EXE="./a.out"
fi

function test() {
    input=$1
    expected=$2

    got=$(echo -n -e $input | $EXE)
    if [[ $got != $expected ]]; then
        echo "Got: $got, Expected: $expected"
        exit 1
    fi
}

test "1721
979
366
299
675
1456" 514579

test "1
2019
2
3" 2019

