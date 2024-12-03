open System.IO

let parse (input: string list) : Map<string, Map<string, int>> =
    let parse' (s: string) : (string * string * int) =
        let words = s.Split(' ')
        let person1 = words.[0]
        let last = Array.last words
        let person2 = last.[0 .. last.Length - 2]

        let point =
            if words.[2] = "gain" then
                int words.[3]
            else
                -1 * int words.[3]

        person1, person2, point

    let rec toGraph edges acc =
        match edges with
        | [] -> acc
        | (person1, person2, point) :: t ->
            let acc =
                match Map.tryFind person1 acc with
                | Some(v) -> Map.add person1 (Map.add person2 point v) acc
                | None -> Map.add person1 (Map.empty |> Map.add person2 point) acc

            toGraph t acc

    let edges = input |> List.map parse'
    toGraph edges Map.empty

let problem1 (graph: Map<string, Map<string, int>>) : int =
    let nodes = Map.count graph

    let getPoint person1 person2 graph : int =
        graph |> Map.find person1 |> Map.find person2

    let rec problem1' node graph visited acc =
        if nodes = Set.count visited then
            let point1 = getPoint "Alice" node graph
            let point2 = getPoint node "Alice" graph
            acc + point1 + point2
        else
            let visited = Set.add node visited

            Map.find node graph
            |> Map.fold
                (fun maxPoint next point ->
                    if Set.contains next visited then
                        maxPoint
                    else
                        let point' = acc + point + getPoint next node graph
                        let p = problem1' next graph (Set.add next visited) point'
                        max maxPoint p)
                0

    problem1' "Alice" graph Set.empty 0

let testData =
    """Alice would gain 54 happiness units by sitting next to Bob.
Alice would lose 79 happiness units by sitting next to Carol.
Alice would lose 2 happiness units by sitting next to David.
Bob would gain 83 happiness units by sitting next to Alice.
Bob would lose 7 happiness units by sitting next to Carol.
Bob would lose 63 happiness units by sitting next to David.
Carol would lose 62 happiness units by sitting next to Alice.
Carol would gain 60 happiness units by sitting next to Bob.
Carol would gain 55 happiness units by sitting next to David.
David would gain 46 happiness units by sitting next to Alice.
David would lose 7 happiness units by sitting next to Bob.
David would gain 41 happiness units by sitting next to Carol."""
        .Split('\n')
    |> Array.toList

let testGraph = parse testData
// 330
problem1 testGraph

let data = File.ReadAllLines("../input/day13.txt") |> Array.toList
let graph = parse data

let ret1 = problem1 graph

printfn "problem1 = %d" ret1
