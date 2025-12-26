open System
open System.IO

type Record = { Id: int; Row: int; Col: int }

let parseLine (id: int) (s: string) : Record =
    let parts = s.Split(',')

    { Id = id
      Row = int parts.[1]
      Col = int parts.[0] }

let gridSize (records: Record list) : int * int =
    records
    |> List.fold (fun (maxRow, maxCol) r -> max maxRow r.Row, max maxCol r.Col) (-1, -1)

let findNearestId (row: int) (col: int) (records: Record list) : int =
    records
    |> List.fold
        (fun (minId, minDistance) r ->
            let dist = (abs (r.Row - row)) + (abs (r.Col - col))

            if dist < minDistance then r.Id, dist
            elif dist = minDistance then -1, dist
            else minId, minDistance)
        (-1, Int32.MaxValue)
    |> fst

let setNearestId (records: Record list) (grid: int[,]) : unit =
    let rows, cols = Array2D.length1 grid, Array2D.length2 grid

    let rec f row col =
        if row >= rows then
            ()
        elif col >= cols then
            f (row + 1) 0
        elif grid.[row, col] <> 0 then
            f row (col + 1)
        else
            let nearestId = findNearestId row col records
            grid.[row, col] <- nearestId
            f row (col + 1)

    f 0 0

let countAreas (grid: int[,]) : Map<int, int> =
    let rows, cols = Array2D.length1 grid, Array2D.length2 grid

    let rec f row col acc =
        if row >= rows then
            acc
        elif col >= cols then
            f (row + 1) 0 acc
        else
            let id_ = grid.[row, col]
            let v = Map.tryFind id_ acc |> Option.defaultValue 0
            let acc = Map.add id_ (v + 1) acc
            f row (col + 1) acc

    f 0 0 Map.empty

let findInfiniteAreaIds (grid: int[,]) : Set<int> =
    let rows, cols = Array2D.length1 grid, Array2D.length2 grid

    let acc =
        seq { 0 .. (cols - 1) }
        |> Seq.fold (fun acc col -> acc |> Set.add grid.[0, col] |> Set.add grid.[rows - 1, col]) Set.empty

    seq { 0 .. (rows - 1) }
    |> Seq.fold (fun acc row -> acc |> Set.add grid.[row, 0] |> Set.add grid.[row, cols - 1]) acc


let problem1 (input: Record list) : int =
    let rows, cols = gridSize input
    let grid = Array2D.init (rows + 1) (cols + 1) (fun _ _ -> 0)

    input |> List.iter (fun r -> grid.[r.Row, r.Col] <- r.Id)
    setNearestId input grid

    let area = countAreas grid
    let infiniteAreaIds = findInfiniteAreaIds grid

    area
    |> Map.fold
        (fun acc id areas ->
            if id <> -1 && Set.contains id infiniteAreaIds then
                acc
            else
                max acc areas)
        -1

let problem2 (input: Record list) (limit: int) : int =
    let rows, cols = gridSize input
    let grid = Array2D.init (rows + 1) (cols + 1) (fun _ _ -> 0)

    let rec f row col acc =
        if row >= rows then
            acc
        elif col >= cols then
            f (row + 1) 0 acc
        else
            let totalDistance =
                input
                |> List.fold (fun acc r -> acc + (abs (row - r.Row)) + (abs (col - r.Col))) 0

            if totalDistance < limit then
                f row (col + 1) (acc + 1)
            else
                f row (col + 1) acc

    f 0 0 0

let testData1 =
    [ "1, 1"; "1, 6"; "8, 3"; "3, 4"; "5, 5"; "8, 9" ]
    |> List.mapi (fun i s -> parseLine (i + 1) s)

// 17
problem1 testData1
// 16
problem2 testData1 32

let input =
    "../input/day06.txt"
    |> File.ReadLines
    |> Seq.mapi (fun i s -> parseLine (i + 1) s)
    |> Seq.toList

let ret1 = problem1 input
let ret2 = problem2 input 10_000

printfn "problem1 = %d" ret1
printfn "problem2 = %d" ret2
