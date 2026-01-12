open System.IO
open System.Text.RegularExpressions

let parseInput (s: string) : (int * int) =
    let pattern = @"(\d+) players; last marble is worth (\d+) points"
    let m = Regex.Match(s, pattern)

    if m.Success then
        int m.Groups.[1].Value, int m.Groups.[2].Value
    else
        failwithf "invalid input: %s" s

let problem1 (players: int) (points: int) : int =
    let rec f point player pos marbles scores =
        if point > points then
            if Map.isEmpty scores then
                0
            else
                scores |> Map.values |> Seq.max
        else
            let length = List.length marbles

            if point % 23 = 0 then
                let insertPos = (pos + length - 7) % length
                let front = List.take (insertPos + 1) marbles
                let back = List.skip (insertPos + 1) marbles
                let v = Map.tryFind player scores |> Option.defaultValue 0
                let scores = Map.add player (v + point + List.head back) scores
                let marbles = front @ List.tail back
                let player = (player + 1) % players
                f (point + 1) player insertPos marbles scores
            else
                let insertPos = (pos + 2) % length
                let front = List.take (insertPos + 1) marbles
                let back = List.skip (insertPos + 1) marbles
                let marbles = front @ [ point ] @ back
                let player = (player + 1) % players
                f (point + 1) player insertPos marbles scores

    f 1 0 0 [ 0 ] Map.empty

// 32
problem1 9 25
// 8317
problem1 10 1618
// 146373
problem1 13 7999
// 2764
problem1 17 1104
// 54718
problem1 21 6111
// 37305
problem1 30 5807

let players, points =
    "../input/day09.txt" |> File.ReadLines |> Seq.head |> parseInput
let ret1 = problem1 players points

// 394486
printfn "problem1 = %d" ret1
