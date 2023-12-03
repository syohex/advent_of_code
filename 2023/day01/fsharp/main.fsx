open System
open System.IO

let readLines (file: string) : string list =
    File.ReadLines(file) |> Seq.toList

let charToDigit (c: char) : int =
    int c - int '0'

let combineFirstAndLastDigit (s: string) : int =
    let first = Seq.find Char.IsDigit s
    let last = Seq.findBack Char.IsDigit s
    (charToDigit first) * 10 + charToDigit last

let problem01 (input: string list) : int =
    input
    |> List.map combineFirstAndLastDigit
    |> List.sum

let test1 () : int =
    let input = [
        "1abc2"
        "pqr3stu8vwx"
        "a1b2c3d4e5f"
        "treb7uchet"
    ]

    problem01 input

// 142
test1 ()

let input = readLines "../input.txt"
// 54990
problem01 input
