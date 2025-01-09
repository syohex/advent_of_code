open System
open System.IO

type Turn =
    | Left
    | Right

type Direction =
    | North
    | East
    | South
    | West

let parse (s: string) : (Turn * int) list =
    s.Replace(",", "").Split(' ')
    |> Array.map (fun s ->
        let dir = if s.[0] = 'L' then Left else Right
        dir, int (s.Substring(1)))
    |> Array.toList

let nextDirection current turn =
    match current with
    | North -> if turn = Left then West else East
    | East -> if turn = Left then North else South
    | South -> if turn = Left then East else West
    | West -> if turn = Left then South else North

let doWalk (row, col) direction steps =
    match direction with
    | North -> row + steps, col
    | East -> row, col + steps
    | South -> row - steps, col
    | West -> row, col - steps

let problem1 (input: (Turn * int) list) : int =
    let rec f input (row: int, col: int) direction =
        match input with
        | [] -> Math.Abs(row) + Math.Abs(col)
        | (turn, steps) :: t ->
            let direction = nextDirection direction turn
            let (row, col) = doWalk (row, col) direction steps
            f t (row, col) direction

    f input (0, 0) North

let nextVisits (row, col) direction steps =
    match direction with
    | North -> seq { 1..steps } |> Seq.map (fun i -> row + i, col)
    | East -> seq { 1..steps } |> Seq.map (fun i -> row, col + i)
    | South -> seq { 1..steps } |> Seq.map (fun i -> row - i, col)
    | West -> seq { 1..steps } |> Seq.map (fun i -> row, col - i)

let problem2 (input: (Turn * int) list) : int =
    let rec f input (row: int, col: int) direction visited =
        match input with
        | [] -> failwith "never reach here"
        | (turn, steps) :: t ->
            let direction = nextDirection direction turn
            let nexts = nextVisits (row, col) direction steps

            match Seq.tryFind (fun (x, y) -> Set.contains (x, y) visited) nexts with
            | Some((x, y)) -> Math.Abs(x) + Math.Abs(y)
            | None ->
                let visited = nexts |> Seq.fold (fun acc v -> Set.add v acc) visited
                f t (Seq.last nexts) direction visited

    f input (0, 0) North Set.empty

// 5
parse "R2, L3" |> problem1

// 2
parse "R2, R2, R2" |> problem1

// 12
parse "R5, L5, R5, R3" |> problem1

// 4
parse "R8, R4, R4, R8" |> problem2

let input = File.ReadLines("../input/day01.txt") |> Seq.head |> parse
let ret1 = problem1 input
// 250
printfn "ret1 = %d" ret1

let ret2 = problem2 input
// 151
printfn "ret2 = %d" ret2
