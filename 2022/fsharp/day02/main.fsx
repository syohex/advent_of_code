open System.IO

type Hand =
    | Rock
    | Paper
    | Scissors

type Result =
    | Lose
    | Draw
    | Win

let handToResult (h: Hand) : Result =
    match h with
    | Rock -> Lose
    | Paper -> Draw
    | Scissors -> Win

let parseInput (file: string) : (Hand * Hand) list =
    let charToHand (c: char) : Hand =
        match c with
        | 'A'
        | 'X' -> Rock
        | 'B'
        | 'Y' -> Paper
        | 'C'
        | 'Z' -> Scissors
        | _ -> failwith "never reach here"

    File.ReadLines(file)
    |> Seq.map (fun line -> charToHand line.[0], charToHand line.[2])
    |> Seq.toList

let score (h1: Hand) (h2: Hand) : int =
    match h2, h1 with
    | Rock, Rock -> 4
    | Rock, Paper -> 1
    | Rock, Scissors -> 7
    | Paper, Rock -> 8
    | Paper, Paper -> 5
    | Paper, Scissors -> 2
    | Scissors, Rock -> 3
    | Scissors, Paper -> 9
    | Scissors, Scissors -> 6

let problem1 (hands: (Hand * Hand) list) : int =
    hands
    |> List.map (fun (h1, h2) -> score h1 h2)
    |> List.sum

let score2 (h: Hand) (r: Result) : int =
    match r, h with
    | Lose, Rock -> 3
    | Lose, Paper -> 1
    | Lose, Scissors -> 2
    | Draw, Rock -> 4
    | Draw, Paper -> 5
    | Draw, Scissors -> 6
    | Win, Rock -> 8
    | Win, Paper -> 9
    | Win, Scissors -> 7

let problem2 (hands: (Hand * Hand) list) : int =
    hands
    |> List.map (fun (h1, h2) -> h1, handToResult h2)
    |> List.map (fun (h, r) -> score2 h r)
    |> List.sum

let testData = parseInput "test.txt"
// 15
problem1 testData
// 12
problem2 testData

let data = parseInput "input.txt"
// 12645
problem1 data
// 11756
problem2 data
