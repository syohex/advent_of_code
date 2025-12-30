open System
open System.IO
open System.Text.RegularExpressions

type Node =
    { Parents: char list
      Children: char list }

let parseLine (s: string) : (char * char) =
    let m = Regex.Match(s, @"Step (\w).*step (\w)")

    if m.Success then
        m.Groups.[1].Value.[0], m.Groups.[2].Value.[0]
    else
        failwithf "input is not matched against the pattern: '%s'" s

let toGraph (edges: (char * char) list) : Map<char, Node> =
    edges
    |> List.fold
        (fun acc (from, to_) ->
            let fromNode =
                Map.tryFind from acc |> Option.defaultValue { Parents = []; Children = [] }

            let acc =
                Map.add
                    from
                    { fromNode with
                        Children = to_ :: fromNode.Children }
                    acc

            let toNode =
                Map.tryFind to_ acc |> Option.defaultValue { Parents = []; Children = [] }

            Map.add
                to_
                { toNode with
                    Parents = from :: toNode.Parents }
                acc)
        Map.empty

let findRoot (graph: Map<char, Node>) : char list =
    graph
    |> Map.filter (fun _ { Parents = parents } -> List.isEmpty parents)
    |> Map.keys
    |> Seq.toList
    |> List.sort

let canAssignNode (node: char) (visited: Set<char>) (graph: Map<char, Node>) : bool =
    if Set.contains node visited then
        false
    else
        match Map.tryFind node graph with
        | Some v -> List.forall (fun n -> Set.contains n visited) v.Parents
        | None -> true

let problem1 (graph: Map<char, Node>) : string =
    let rec f nodes visited (acc: char list) =
        match nodes with
        | [] -> acc |> List.rev |> String.Concat
        | h :: t ->
            let acc = h :: acc
            let visited = Set.add h visited

            let nexts =
                match Map.tryFind h graph with
                | Some v -> v.Children
                | None -> []

            let nodes =
                List.append nexts t
                |> List.filter (fun n -> canAssignNode n visited graph)
                |> List.sort

            f nodes visited acc

    let roots = findRoot graph
    f roots Set.empty []

let assignTask
    (nodes: char list)
    (workers: Map<char, int>)
    (workerNum: int)
    (visited: Set<char>)
    (graph: Map<char, Node>)
    (scoreTable: int[])
    : Map<char, int> * char list =
    let rec f nodes workers acc =
        if Map.count workers >= workerNum then
            workers, List.append nodes acc |> List.sort
        else
            match nodes with
            | [] -> workers, acc
            | h :: t ->
                if canAssignNode h visited graph then
                    let index = (h - 'A') |> int
                    let workers = Map.add h (scoreTable.[index]) workers
                    f t workers acc
                else
                    f t workers (h :: acc)

    f nodes workers []

let problem2 (graph: Map<char, Node>) (workerNum: int) (scoreTable: int[]) : int =
    let rec f nodes workers visited acc =
        if List.isEmpty nodes && Map.isEmpty workers then
            acc
        else
            let minNode, minValue =
                workers
                |> Map.fold
                    (fun (minNode, minValue) node time -> if time < minValue then node, time else minNode, minValue)
                    ('?', Int32.MaxValue)

            let oldWorker = workers

            let visited, acc, workers, nodes =
                if minNode = '?' then
                    visited, acc, workers, nodes
                else
                    let workers = workers |> Map.remove minNode |> Map.map (fun _ v -> v - minValue)

                    let nodes =
                        match Map.tryFind minNode graph with
                        | Some v -> List.append nodes v.Children |> Set.ofList |> Set.toList |> List.sort
                        | None -> nodes

                    Set.add minNode visited, acc + minValue, workers, nodes

            match nodes with
            | [] -> workers |> Map.values |> Seq.fold (+) acc
            | _ ->
                let workers, restTasks = assignTask nodes workers workerNum visited graph scoreTable
                f restTasks workers visited acc

    let roots = findRoot graph
    f roots Map.empty Set.empty 0

let testGraph1 =
    [ "Step C must be finished before step A can begin."
      "Step C must be finished before step F can begin."
      "Step A must be finished before step B can begin."
      "Step A must be finished before step D can begin."
      "Step B must be finished before step E can begin."
      "Step D must be finished before step E can begin."
      "Step F must be finished before step E can begin." ]
    |> List.map parseLine
    |> toGraph

let testScore = Array.init 26 (fun i -> 1 + i)

// "CABDFE"
problem1 testGraph1

// 15
problem2 testGraph1 2 testScore

let inputGraph =
    "../input/day07.txt"
    |> File.ReadLines
    |> Seq.toList
    |> List.map parseLine
    |> toGraph

let inputScore = Array.init 26 (fun i -> 61 + i)

// "GJFMDHNBCIVTUWEQYALSPXZORK"
let ret1 = problem1 inputGraph
// 1050
let ret2 = problem2 inputGraph 5 inputScore

printfn "problem1 = %s" ret1
printfn "problem2 = %d" ret2
