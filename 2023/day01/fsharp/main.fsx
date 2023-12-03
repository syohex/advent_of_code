open System
open System.IO

let readLines (file: string) : string list = File.ReadLines(file) |> Seq.toList

let charToDigit (c: char) : int = int c - int '0'

let combineFirstAndLastDigit (s: string) : int =
    let first = Seq.find Char.IsDigit s
    let last = Seq.findBack Char.IsDigit s
    (charToDigit first) * 10 + charToDigit last

let problem01 (input: string list) : int =
    input |> List.map combineFirstAndLastDigit |> List.sum

let replaceNumber (s: string) : string =
    let rec replaceNumber' (cs: char list) (acc: char list) =
        match cs with
        | [] -> acc |> List.rev |> String.Concat
        | 'o' :: 'n' :: 'e' :: _ -> replaceNumber' (List.tail cs) ('1' :: acc)
        | 't' :: 'w' :: 'o' :: _ -> replaceNumber' (List.tail cs) ('2' :: acc)
        | 't' :: 'h' :: 'r' :: 'e' :: 'e' :: _ -> replaceNumber' (List.tail cs) ('3' :: acc)
        | 'f' :: 'o' :: 'u' :: 'r' :: _ -> replaceNumber' (List.tail cs) ('4' :: acc)
        | 'f' :: 'i' :: 'v' :: 'e' :: _ -> replaceNumber' (List.tail cs) ('5' :: acc)
        | 's' :: 'i' :: 'x' :: _ -> replaceNumber' (List.tail cs) ('6' :: acc)
        | 's' :: 'e' :: 'v' :: 'e' :: 'n' :: _ -> replaceNumber' (List.tail cs) ('7' :: acc)
        | 'e' :: 'i' :: 'g' :: 'h' :: 't' :: _ -> replaceNumber' (List.tail cs) ('8' :: acc)
        | 'n' :: 'i' :: 'n' :: 'e' :: _ -> replaceNumber' (List.tail cs) ('9' :: acc)
        | h :: t -> replaceNumber' t (h :: acc)

    replaceNumber' (Seq.toList s) []

let problem02 (input: string list) : int =
    input |> List.map replaceNumber |> List.map combineFirstAndLastDigit |> List.sum

let test1 () : int =
    let input = [ "1abc2"; "pqr3stu8vwx"; "a1b2c3d4e5f"; "treb7uchet" ]

    problem01 input

let test2 () : int =
    let input =
        [ "two1nine"
          "eightwothree"
          "abcone2threexyz"
          "xtwone3four"
          "4nineeightseven2"
          "zoneight234"
          "7pqrstsixteen" ]

    problem02 input

// 142
test1 ()

// 281
test2 ()

let input = readLines "../input.txt"
// 54990
problem01 input

// 54473
problem02 input
