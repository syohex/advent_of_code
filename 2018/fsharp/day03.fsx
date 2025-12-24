open System.IO
open System.Text.RegularExpressions

type Record =
    { Id: int
      Row: int
      Col: int
      Width: int
      Height: int }

let parseLine (s: string) : Record =
    // #ID @ Row,Col: WidthxHeight
    let pattern = @"#(\d+) @ (\d+),(\d+): (\d+)x(\d+)"
    let m = Regex.Match(s, pattern)

    if m.Success then
        { Id = int m.Groups.[1].Value
          Row = int m.Groups.[2].Value
          Col = int m.Groups.[3].Value
          Width = int m.Groups.[4].Value
          Height = int m.Groups.[5].Value }
    else
        failwithf "failed to parse: %s" s

let updateGrid (record: Record) (grid: int list[,]) : unit =
    for i in 0 .. (record.Width - 1) do
        for j in 0 .. (record.Height - 1) do
            let row, col = record.Row + i, record.Col + j
            grid.[row, col] <- record.Id :: grid.[row, col]

let problem1 (input: Record list) : int =
    let grid = Array2D.init 1000 1000 (fun _ _ -> [])
    input |> List.iter (fun r -> updateGrid r grid)

    grid
    |> Seq.cast<int list>
    |> Seq.filter (fun ids -> List.length ids >= 2)
    |> Seq.length

let problem2 (input: Record list) : int =
    let grid = Array2D.init 1000 1000 (fun _ _ -> [])
    input |> List.iter (fun r -> updateGrid r grid)

    let overrapped =
        grid
        |> Seq.cast<int list>
        |> Seq.filter (fun ids -> List.length ids >= 2)
        |> Seq.fold (fun acc ids -> ids |> List.fold (fun acc id_ -> Set.add id_ acc) acc) Set.empty

    let ids = input |> List.map _.Id

    match List.tryFind (fun id_ -> not <| Set.contains id_ overrapped) ids with
    | Some v -> v
    | None -> failwith "never reach here"

let testData1 = [ "#1 @ 1,3: 4x4"; "#2 @ 3,1: 4x4"; "#3 @ 5,5: 2x2" ]
let testRecords1 = List.map parseLine testData1
// 4
problem1 testRecords1

// 3
problem2 testRecords1

let input = "../input/day03.txt" |> File.ReadLines |> Seq.toList
let records = input |> List.map parseLine
let ret1 = problem1 records
let ret2 = problem2 records

printfn "problem1 = %d" ret1
printfn "problem2 = %d" ret2
