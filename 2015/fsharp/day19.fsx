open System
open System.IO

let parse (input: string list) : string * (string * string) list =
    let rec parse' (input: string list) table =
        match input with
        | [] -> failwith "never reach here"
        | h :: t ->
            if String.IsNullOrEmpty(h) then
                List.head t, List.rev table
            else
                let parts = h.Split(" => ")
                parse' t ((parts.[0], parts.[1]) :: table)

    parse' input []

let problem1 (molecule: string) (table: (string * string) list) : int =
    let rec f i acc =
        if i >= molecule.Length then
            Set.count acc
        else
            let prev = molecule.Substring(0, i)
            let substr = molecule.Substring(i)

            let acc =
                table
                |> List.fold
                    (fun acc (orig, replaced) ->
                        if substr.StartsWith(orig) then
                            let rest = substr.Substring(orig.Length)
                            let replaced = prev + replaced + rest
                            Set.add replaced acc
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

let testMolecule, testTable = parse testInput
// 4
problem1 testMolecule testTable

let input = File.ReadLines("../input/day19.txt") |> Seq.toList
let molecule, table = parse input

let ret1 = problem1 molecule table
// 576
printfn "ret1 = %d" ret1
