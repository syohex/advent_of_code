open System.IO

let rec howManyHouses (cs: char list) ((x, y): int * int) (s: Set<int * int>) : int =
    match cs with
    | [] -> Set.count s
    | h :: t ->
        let x, y =
            match h with
            | '^' -> x, y + 1
            | 'v' -> x, y - 1
            | '>' -> x + 1, y
            | '<' -> x - 1, y
            | _ -> failwithf "invalid character %c" h

        howManyHouses t (x, y) (Set.add (x, y) s)

let problem1 (s: string) : int =
    howManyHouses (Seq.toList s) (0, 0) (Set.empty |> Set.add (0, 0))

// 2
problem1 ">"

// 4
problem1 "^>v<"

// 2
problem1 "^v^v^v^v^v"

let input = File.ReadLines("../../input/day03.txt") |> Seq.head

problem1 input

