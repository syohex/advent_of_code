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
1456" 241861950

test "1
2
2017
4
5" 4034
