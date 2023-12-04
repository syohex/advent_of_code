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

let calculatePoint (d: Data) : int =
    let rec calculatePoint' nums wins acc =
        match nums with
        | [] -> acc
        | h :: t ->
            if Set.contains h wins then
                if acc = 0 then
                    calculatePoint' t wins 1
                else
                    calculatePoint' t wins (acc * 2)
            else
                calculatePoint' t wins acc

    calculatePoint' d.Cards d.WinningNumber 0

let countWinCards (d: Data) : int =
    let rec countWinCards' nums wins acc =
        match nums with
        | [] -> acc
        | h :: t ->
            if Set.contains h wins then
                countWinCards' t wins (acc + 1)
            else
                countWinCards' t wins acc

    countWinCards' d.Cards d.WinningNumber 0

let problem1 (input: string list) : int =
    input |> List.map parseLine |> List.map calculatePoint |> List.sum

let problem01 (input: string list) : int =
    input
    |> List.map parseLine
    |> List.map countWinCards
    |> List.map (fun n -> Math.Pow(2, double (n - 1)) |> int)
    |> List.sum

let copyCard (ds: Data list) : Map<int, int> =
    let rec copyCard' i ds acc =
        match ds with
        | [] -> acc
        | h :: t ->
            let p = countWinCards h
            let count = Map.find i acc

            let acc' =
                seq { (i + 1) .. (i + p) }
                |> Seq.fold
                    (fun acc j ->
                        let n = Map.find j acc
                        Map.add j (n + count) acc)
                    acc

            copyCard' (i + 1) t acc'

    let cardCount =
        seq { 1 .. ds.Length } |> Seq.fold (fun acc i -> Map.add i 1 acc) Map.empty

    copyCard' 1 ds cardCount

let problem2 (input: string list) : int =
    input |> List.map parseLine |> copyCard |> Map.values |> Seq.sum

let testInput =
    [ "Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53"
      "Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19"
      "Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1"
      "Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83"
      "Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36"
      "Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11" ]

// 13
problem1 testInput
problem01 testInput

// 30
problem2 testInput

let input = readInput "../input.txt"
// 21138
problem1 input
// 7185540
problem2 input
