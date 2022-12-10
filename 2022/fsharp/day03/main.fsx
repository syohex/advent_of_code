open System
open System.IO

let parseInput (file: string) : string list = File.ReadLines(file) |> Seq.toList

let charToScore (c: char) : int =
    if Char.IsLower(c) then
        (int c - int 'a') + 1
    else
        (int c - int 'A') + 27

let problem1 (data: string list) : int =
    let splitString (s: string) : (char list * char list) =
        let half = s.Length / 2
        let cs = s |> Seq.toList
        List.take half cs, List.skip half cs

    let findUniqChar (cs1, cs2) : char =
        let s1 = cs1 |> Set.ofList
        let s2 = cs2 |> Set.ofList

        Set.intersect s1 s2 |> Seq.toList |> List.head

    data
    |> List.map splitString
    |> List.map findUniqChar
    |> List.map charToScore
    |> List.sum

let toGroup (data: string list) : string list list =
    let rec toGroup' data n acc ret =
        match data with
        | [] -> ret |> List.rev
        | h :: t ->
            let acc' = h :: acc

            if n = 3 then
                toGroup' t 1 [] ((acc' |> List.rev) :: ret)
            else
                toGroup' t (n + 1) acc' ret

    toGroup' data 1 [] []

let findUniqueChar (group: string list) : char =
    let sets = group |> List.map Set.ofSeq

    List.tail sets
    |> List.fold (fun acc s -> Set.intersect acc s) (List.head sets)
    |> Set.toList
    |> List.head

let problem2 (data: string list) : int =
    data
    |> toGroup
    |> List.map findUniqueChar
    |> List.map charToScore
    |> List.sum

let testData = parseInput "test.txt"
// 157
problem1 testData
// 70
problem2 testData

let data = parseInput "input.txt"
// 8109
problem1 data
// 2738
problem2 data
