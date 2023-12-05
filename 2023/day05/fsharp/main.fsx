open System
open System.IO

type MapData =
    { Src: int64
      Dest: int64
      Length: int64 }

type Data =
    { Seeds: int64 list
      Maps: MapData list list }

let parseSeedLine (s: string) : int64 list =
    s.Split(':').[1].Split(' ', StringSplitOptions.RemoveEmptyEntries)
    |> Array.map (fun s -> s.Trim())
    |> Array.map int64
    |> Array.toList

let parseMapLine (s: string) : MapData =
    let nums = s.Split(' ', StringSplitOptions.RemoveEmptyEntries) |> Array.map int64

    { Src = nums.[1]
      Dest = nums.[0]
      Length = nums.[2] }

let parseMapLines (lines: string list) : (MapData list * string list) =
    let rec parseMapLines' (lines: string list) acc =
        match lines with
        | [] -> acc, []
        | h :: t ->
            if h = "" then
                acc, t
            else
                let m = parseMapLine h
                parseMapLines' t (m :: acc)

    parseMapLines' lines []


let parseInput (input: string list) : Data =
    let rec parseInput' (lines: string list) acc =
        match lines with
        | [] -> List.rev acc
        | h :: t ->
            if h.EndsWith("map:") then
                let m, t' = parseMapLines t
                parseInput' t' (m :: acc)
            else
                failwith "never reach here"

    let seeds = parseSeedLine (List.head input)
    let maps = parseInput' (List.skip 2 input) []
    { Seeds = seeds; Maps = maps }

let readInput (f: string) : string list = File.ReadLines(f) |> Seq.toList

let rec applyMaps (v: int64) (maps: MapData list list) : int64 =
    let rec findMapData v maps =
        match maps with
        | [] -> v
        | { Src = s; Dest = d; Length = len } :: t ->
            if v >= s && v < s + len then
                d + (v - s)
            else
                findMapData v t

    match maps with
    | [] -> v
    | h :: t ->
        let v' = findMapData v h
        applyMaps v' t

let problem1 (d: Data) : int64 =
    d.Seeds |> List.map (fun seed -> applyMaps seed d.Maps) |> List.min

let testInput = readInput "../test.txt"
let testData = parseInput testInput

// 35
problem1 testData

let input = readInput "../input.txt" |> parseInput
problem1 input
