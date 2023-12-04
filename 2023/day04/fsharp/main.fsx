open System
open System.IO

type Data =
    { WinningNumber: Set<int>
      Cards: int list }

let readInput (f: string) : string list = File.ReadLines(f) |> Seq.toList

let parseLine (s: string) : Data =
    let parts = s.Split(':').[1].Split('|') |> Array.map (fun s -> s.Trim())

    let winNums =
        parts.[0].Split(' ', StringSplitOptions.RemoveEmptyEntries)
        |> Array.map (fun s -> s.Trim())
        |> Array.map int
        |> Set.ofArray

    let cards =
        parts.[1].Split(' ', StringSplitOptions.RemoveEmptyEntries)
        |> Array.map (fun s -> s.Trim())
        |> Array.map int
        |> Array.toList

    { WinningNumber = winNums
      Cards = cards }

let calculatePoint (d: Data) : int64 =
    let rec calculatePoint' nums wins (acc: int64) =
        match nums with
        | [] -> acc
        | h :: t ->
            if Set.contains h wins then
                if acc = 0 then
                    calculatePoint' t wins 1L
                else
                    calculatePoint' t wins (acc * 2L)
            else
                calculatePoint' t wins acc

    calculatePoint' d.Cards d.WinningNumber 0L

let problem1 (input: string list) : int64 =
    input |> List.map parseLine |> List.map calculatePoint |> List.sum

let testInput =
    [ "Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53"
      "Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19"
      "Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1"
      "Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83"
      "Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36"
      "Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11" ]

// 13
problem1 testInput

let input = readInput "../input.txt"
// 21138
problem1 input
