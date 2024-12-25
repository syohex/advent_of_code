open System
open System.IO

let parse (input: string list) : Map<string, string list> * string =
    let rec parse' (input: string list) table =
        match input with
        | [] -> failwith "never reach here"
        | h :: t ->
            if String.IsNullOrEmpty(h) then
                table, List.head t
            else
                let parts = h.Split(" => ")

                let table =
                    match Map.tryFind parts.[0] table with
                    | Some(v) -> Map.add parts.[0] (parts.[1] :: v) table
                    | None -> Map.add parts.[0] [ parts.[1] ] table

                parse' t table

    parse' input Map.empty

let problem1 (input: string) (table: Map<string, string list>) : int =
    let rec f i acc =
        if i >= input.Length then
            Set.count acc
        else
            let prev = input.Substring(0, i)
            let substr = input.Substring(i)

            let acc =
                table
                |> Map.fold
                    (fun acc k v ->
                        if substr.StartsWith(k) then
                            let ss = substr.Substring(k.Length)

                            v
                            |> List.fold
                                (fun acc replaced ->
                                    let s = prev + replaced + ss
                                    Set.add s acc)
                                acc
                        else
                            acc)
                    acc

            f (i + 1) acc

    f 0 Set.empty

let testInput =
    """H => HO
H => OH
O => HH

HOH
"""
    |> fun s -> s.Split('\n') |> Array.toList

let testTable, testStr = parse testInput
// 4
problem1 testStr testTable

let input = File.ReadLines("../input/day19.txt") |> Seq.toList
let table, inputStr = parse input

let ret1 = problem1 inputStr table
// 576
printfn "ret1 = %d" ret1
