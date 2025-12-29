open System
open System.IO
open System.Text.RegularExpressions

type Graph = Map<string, Set<string>>

let parseLine (s: string) : (string * string) =
    let m = Regex.Match(s, @"Step (\w+).*step (\w+)")

    if m.Success then
        m.Groups.[1].Value, m.Groups.[2].Value
    else
        failwithf "input is not matched against the pattern: '%s'" s

let toGraphAndRules (edges: (string * string) list) : Graph * Graph =
    edges
    |> List.fold
        (fun (graph, rules) (from, to_) ->
            let v = Map.tryFind from graph |> Option.defaultValue Set.empty
            let graph = Map.add from (Set.add to_ v) graph

            let v' = Map.tryFind to_ rules |> Option.defaultValue Set.empty
            let rules = Map.add to_ (Set.add from v') rules
            graph, rules)
        (Map.empty, Map.empty)

let getAllNodes (graph: Graph) : Set<string> =
    graph |> Map.fold (fun acc k v -> Set.add k acc |> Set.union v) Set.empty

let findInitNodes (graph: Graph) : string list =
    let nodes = getAllNodes graph

    let allDest =
        graph |> Map.values |> Seq.fold (fun acc v -> Set.union acc v) Set.empty

    nodes
    |> Set.filter (fun n -> not <| Set.contains n allDest)
    |> Set.toList
    |> List.sort

let problem1 (input: Graph) (rules: Graph) : string =
    let rec f nodes visited (acc: string list) =
        match nodes with
        | [] -> acc |> List.rev |> String.Concat
        | h :: t ->
            let acc = h :: acc
            let visited = Set.add h visited

            let nexts =
                match Map.tryFind h input with
                | Some v -> Set.toList v
                | None -> []

            let nodes =
                List.append nexts t
                |> List.filter (fun n ->
                    (not <| Set.contains n visited)
                    && Set.forall
                        (fun n -> Set.contains n visited)
                        (Map.tryFind n rules |> Option.defaultValue Set.empty))
                |> List.sort

            f nodes visited acc

    let init = findInitNodes input
    f init Set.empty []


let testData1, testRules =
    [ "Step C must be finished before step A can begin."
      "Step C must be finished before step F can begin."
      "Step A must be finished before step B can begin."
      "Step A must be finished before step D can begin."
      "Step B must be finished before step E can begin."
      "Step D must be finished before step E can begin."
      "Step F must be finished before step E can begin." ]
    |> List.map parseLine
    |> toGraphAndRules

// ['C']
findInitNodes testData1

// "CABDFE"
problem1 testData1 testRules

let input, inputRules =
    "../input/day07.txt"
    |> File.ReadLines
    |> Seq.toList
    |> List.map parseLine
    |> toGraphAndRules

let ret1 = problem1 input inputRules

printfn "problem1 = %s" ret1
