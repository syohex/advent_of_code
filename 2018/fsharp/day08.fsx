open System.IO

type Node =
    { Children: Node list
      MetadataEntries: int list }

let parseInput (s: string) : int list =
    s.Split() |> Array.map int |> Array.toList

let readTree (input: int list) : Node =
    let rec f input =
        match input with
        | []
        | _ :: [] -> failwithf "never reach here: %A" input
        | childCount :: metadataCount :: t ->
            let children, data =
                seq { 1..childCount }
                |> Seq.fold
                    (fun (children, data) _ ->
                        let node, data = f data
                        node :: children, data)
                    ([], t)

            let metadataEntries = List.take metadataCount data

            { Children = List.rev children
              MetadataEntries = metadataEntries },
            List.skip metadataCount data

    f input |> fst

let problem1 (input: int list) : int =
    let rec f (node: Node) =
        let childSum = node.Children |> List.fold (fun acc c -> acc + f c) 0
        childSum + List.sum node.MetadataEntries

    let tree = readTree input
    f tree

let problem2 (input: int list) : int =
    let rec f (node: Node) =
        if List.isEmpty node.Children then
            List.sum node.MetadataEntries
        else
            node.MetadataEntries
            |> List.fold
                (fun (acc, cache) e ->
                    match Map.tryFind e cache with
                    | Some v -> acc + v, cache
                    | None ->
                        match List.tryItem (e - 1) node.Children with
                        | None -> acc, Map.add e 0 cache
                        | Some n ->
                            let v = f n
                            acc + v, Map.add e v cache)
                (0, Map.empty)
            |> fst

    let tree = readTree input
    f tree

let testInput = "2 3 0 3 10 11 12 1 1 0 1 99 2 1 1 2" |> parseInput
// 138
problem1 testInput
// 66
problem2 testInput

let input = "../input/day08.txt" |> File.ReadLines |> Seq.head |> parseInput
let ret1 = problem1 input
let ret2 = problem2 input

printfn "problem1 = %d" ret1
printfn "problem2 = %d" ret2
