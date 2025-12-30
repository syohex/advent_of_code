open System
open System.IO
open System.Text.RegularExpressions

type Graph = Map<char, Set<char>>

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
                |> List.filter (fun n -> not <| Set.contains n visited)
                |> List.filter (fun n ->
                    let parents =
                        match Map.tryFind n graph with
                        | Some v -> v.Parents
                        | None -> []

                    parents |> List.forall (fun p -> Set.contains p visited))
                |> List.sort

            f nodes visited acc

    let roots = findRoot graph
    f roots Set.empty []

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

// "CABDFE"
problem1 testGraph1

let inputGraph =
    "../input/day07.txt"
    |> File.ReadLines
    |> Seq.toList
    |> List.map parseLine
    |> toGraph

// "GJFMDHNBCIVTUWEQYALSPXZORK"
let ret1 = problem1 inputGraph

printfn "problem1 = %s" ret1
