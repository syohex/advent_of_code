open System
open System.IO

let parse (s: string) : Map<string, int> =
    let rec parse' parts acc =
        match parts with
        | [] -> acc
        | _ :: [] -> failwithf "invalid input %s" s
        | name :: count :: t -> parse' t (Map.add name (int count) acc)

    let parts =
        s
            .Replace(":", "")
            .Replace(",", "")
            .Split(' ', StringSplitOptions.RemoveEmptyEntries)
        |> Array.toList

    parse' (List.skip 2 parts) Map.empty

let isMatched (aunt: Map<string, int>) (compounds: Map<string, int>) : bool =
    compounds
    |> Map.forall (fun k v ->
        match Map.tryFind k aunt with
        | None -> true
        | Some(num) -> num = v)

let isMatched2 (aunt: Map<string, int>) (compounds: Map<string, int>) : bool =
    compounds
    |> Map.forall (fun k v ->
        match Map.tryFind k aunt with
        | None -> true
        | Some(num) ->
            match k with
            | "cat"
            | "trees" -> num > v
            | "pomeranians"
            | "goldfish" -> num < v
            | _ -> num = v)

let problem1 (input: Map<string, int> list) (compounds: Map<string, int>) : int =
    input |> List.findIndex (fun m -> isMatched m compounds) |> ((+) 1)

let problem2 (input: Map<string, int> list) (compounds: Map<string, int>) : int =
    input |> List.findIndex (fun m -> isMatched2 m compounds) |> ((+) 1)

let compounds =
    """children: 3
    cats: 7
    samoyeds: 2
    pomeranians: 3
    akitas: 0
    vizslas: 0
    goldfish: 5
    trees: 3
    cars: 2
    perfumes: 1"""
        .Replace('\n', ' ')
    |> parse

let input =
    File.ReadAllLines("../input/day16.txt") |> Array.map parse |> Array.toList

let ret1 = problem1 input compounds
let ret2 = problem2 input compounds
// 213
printfn "ret1 = %d" ret1
// 323
printfn "ret2 = %A" ret2
