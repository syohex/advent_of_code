open System.IO

let nextPosition ((x, y): int * int) (dir: char) : int * int =
    match dir with
    | '^' -> x, y + 1
    | 'v' -> x, y - 1
    | '>' -> x + 1, y
    | '<' -> x - 1, y
    | _ -> failwithf "invalid character %c" dir

let rec howManyHouses (cs: char list) ((x, y): int * int) (s: Set<int * int>) : int =
    match cs with
    | [] -> Set.count s
    | h :: t ->
        let x, y = nextPosition (x, y) h
        howManyHouses t (x, y) (Set.add (x, y) s)

let problem1 (s: string) : int =
    howManyHouses (Seq.toList s) (0, 0) (Set.empty |> Set.add (0, 0))

let rec howManyHousesWithRobo
    (cs: char list)
    (isSanta: bool)
    ((x1, y1): int * int)
    ((x2, y2): int * int)
    (s: Set<int * int>)
    : int =
    match cs with
    | [] -> Set.count s
    | h :: t ->
        if isSanta then
            let x1, y1 = nextPosition (x1, y1) h
            howManyHousesWithRobo t false (x1, y1) (x2, y2) (Set.add (x1, y1) s)
        else
            let x2, y2 = nextPosition (x2, y2) h
            howManyHousesWithRobo t true (x1, y1) (x2, y2) (Set.add (x2, y2) s)

let problem2 (s: string) : int =
    howManyHousesWithRobo (Seq.toList s) true (0, 0) (0, 0) (Set.empty |> Set.add (0, 0))

// 2
problem1 ">"

// 4
problem1 "^>v<"

// 2
problem1 "^v^v^v^v^v"

// 3
problem2 "^v"

// 3
problem2 "^>v<"

// 11
problem2 "^v^v^v^v^v"

let input = File.ReadLines("../input/day03.txt") |> Seq.head

problem1 input
problem2 input
