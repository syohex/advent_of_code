open System.IO
open System.Text.RegularExpressions

let parseInput (file: string) : ((int * int) * (int * int)) list =
    let regex = Regex(@"(\d+)-(\d+),(\d+)-(\d+)", RegexOptions.Compiled)

    File.ReadLines(file)
    |> Seq.map (fun line ->
        let m = regex.Match(line)

        [| m.Groups[1].Value
           m.Groups[2].Value
           m.Groups[3].Value
           m.Groups[4].Value |])
    |> Seq.map (fun strs -> Array.map int strs)
    |> Seq.map (fun nums -> (nums.[0], nums.[1]), (nums.[2], nums.[3]))
    |> Seq.toList

let problem1 (data: ((int * int) * (int * int)) list) : int =
    let isFullyContained ((a1, b1), (a2, b2)) : bool =
        let s1 = seq { a1..b1 } |> Set.ofSeq
        let s2 = seq { a2..b2 } |> Set.ofSeq

        Set.isSubset s1 s2 || Set.isSubset s2 s1

    data
    |> List.filter isFullyContained
    |> List.length

let problem2 (data: ((int * int) * (int * int)) list) : int =
    let isOverlapped ((a1, b1), (a2, b2)) : bool =
        let s1 = seq { a1..b1 } |> Set.ofSeq
        let s2 = seq { a2..b2 } |> Set.ofSeq

        Set.intersect s1 s2 |> Set.isEmpty |> not

    data |> List.filter isOverlapped |> List.length

let testData = parseInput "test.txt"
// 2
problem1 testData
// 4
problem2 testData

let data = parseInput "input.txt"
// 487
problem1 data
// 849
problem2 data
