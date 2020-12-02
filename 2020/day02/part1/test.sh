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

    got=$($EXE < $input)
    if [[ $got != $expected ]]; then
        echo "Got: $got, Expected: $expected"
        exit 1
    fi
}

test test.txt 2
