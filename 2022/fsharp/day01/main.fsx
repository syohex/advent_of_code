open System
open System.IO

let parseInput (file: string) : int list list =
    let rec parseInput' lines items acc =
        match lines with
        | [] -> (items :: acc) |> List.rev
        | h :: t ->
            if String.IsNullOrEmpty(h) then
                parseInput' t [] ((items |> List.rev) :: acc)
            else
                let calorie = h |> int
                parseInput' t (calorie :: items) acc

    let lines = File.ReadLines(file) |> Seq.toList
    parseInput' lines [] []

let problem1 (data: int list list) : int =
    data
    |> List.map (fun calories -> calories |> List.sum)
    |> List.max

let problem2 (data: int list list) : int =
    data
    |> List.map (fun calories -> calories |> List.sum)
    |> List.sort
    |> List.rev
    |> List.take 3
    |> List.sum

let testData = parseInput "test.txt"
let testRet1 = problem1 testData
let testRet2 = problem2 testData

if testRet1 <> 24000 then
    failwith $"Unexpected part1 test result: got:{testRet1} expected:24000"

if testRet2 <> 45000 then
    failwith $"Unexpected part2 test result: got:{testRet2} expected:45000"

let data = parseInput "input.txt"
let ret1 = problem1 data
let ret2 = problem2 data

if ret1 <> 70369 then
    failwith $"Unexpected part1 result: got:{ret1} expected:70369"

if ret2 <> 203002 then
    failwith $"Unexpected part1 result: got:{ret1} expected:203002"
