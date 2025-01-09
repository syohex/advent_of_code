open System
open System.IO

let problem1 (input: string list) : string =
    let move row col c =
        match c with
        | 'U' -> max 0 (row - 1), col
        | 'D' -> min 2 (row + 1), col
        | 'L' -> row, max 0 (col - 1)
        | 'R' -> row, min 2 (col + 1)
        | _ -> failwith "never reach here"

    let toNum row col = row * 3 + col + 1

    let rec f input row col acc =
        match input with
        | [] -> acc |> List.rev |> List.fold (fun acc d -> acc + string d) ""
        | h :: t ->
            let row, col = h |> Seq.fold (fun (x, y) c -> move x y c) (row, col)
            f t row col ((toNum row col) :: acc)

    f input 1 1 []

let problem2 (matrix: char[,]) (input: string list) : string =
    let steps =
        Map.empty
        |> Map.add 'U' (-1, 0)
        |> Map.add 'D' (1, 0)
        |> Map.add 'L' (0, -1)
        |> Map.add 'R' (0, 1)

    let isValid row col =
        row >= 0 && row < 5 && col >= 0 && col < 5 && matrix.[row, col] <> '0'

    let rec f input row col (acc: char list) =
        match input with
        | [] -> acc |> List.rev |> String.Concat
        | h :: t ->
            let row, col =
                h
                |> Seq.fold
                    (fun (x, y) c ->
                        let a, b = Map.find c steps
                        let x', y' = x + a, y + b
                        if isValid x' y' then x', y' else x, y)
                    (row, col)

            f t row col (matrix.[row, col] :: acc)

    f input 2 0 []

let testInput = [ "ULL"; "RRDDD"; "LURDL"; "UUUUD" ]
// 1985
problem1 testInput

let input = File.ReadLines("../input/day02.txt") |> Seq.toList
let ret1 = problem1 input
// 56983
printfn "ret1=%s" ret1

let matrix =
    array2D
        [ [ '0'; '0'; '1'; '0'; '0' ]
          [ '0'; '2'; '3'; '4'; '0' ]
          [ '5'; '6'; '7'; '8'; '9' ]
          [ '0'; 'A'; 'B'; 'C'; '0' ]
          [ '0'; '0'; 'D'; '0'; '0' ] ]

// 5DB3
problem2 matrix testInput

let ret2 = problem2 matrix input
printfn "ret2=%s" ret2