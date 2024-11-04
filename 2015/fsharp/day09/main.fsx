open System
open System.IO

type Data =
    { From: string
      To: string
      Distance: int }

let parseLine (s: string) : Data =
    let parts = s.Split(' ')

    { From = parts.[0]
      To = parts.[2]
      Distance = int parts.[4] }

let toGraph (data: Data list) : Map<string, (string * int) list> =
    let rec f data acc =
        match data with
        | [] -> acc
        | { From = from; To = to'; Distance = d } :: t ->
            let acc =
                match Map.tryFind from acc with
                | Some(v) -> Map.add from ((to', d) :: v) acc
                | None -> Map.add from [ to', d ] acc

            let acc =
                match Map.tryFind to' acc with
                | Some(v) -> Map.add to' ((from, d) :: v) acc
                | None -> Map.add to' [ from, d ] acc

            f t acc

    f data Map.empty

let problem1 (graph: Map<string, (string * int) list>) : int =
    let rec f node total visited nodes =
        if Set.count visited = nodes then
            total
        else
            match Map.tryFind node graph with
            | None -> Int32.MaxValue
            | Some(nexts) ->
                nexts
                |> List.fold
                    (fun acc (next, distance) ->
                        if Set.contains next visited then
                            acc
                        else
                            min acc <| f next (total + distance) (Set.add next visited) nodes)
                    Int32.MaxValue

    let nodes = graph |> Map.count

    graph
    |> Map.fold (fun acc start _ -> min acc <| f start 0 (Set.empty |> Set.add start) nodes) Int32.MaxValue

let testGraph =
    [ "London to Dublin = 464"
      "London to Belfast = 518"
      "Dublin to Belfast = 141" ]
    |> List.map parseLine
    |> toGraph

// 605
problem1 testGraph

let input = File.ReadLines("../../input/day09.txt") |> Seq.toList
let graph = input |> List.map parseLine |> toGraph
let ret1 = problem1 graph
// 251
printfn "ret1 = %d" ret1
