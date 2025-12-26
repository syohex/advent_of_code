open System
open System.IO

let removeOppositePolarity (s: string) : char list =
    let rec f cs stack =
        match cs with
        | [] -> stack
        | h :: t ->
            match stack with
            | [] -> f t (h :: stack)
            | top :: rest ->
                if h <> top && Char.ToLower(h) = Char.ToLower(top) then
                    f t rest
                else
                    f t (h :: stack)

    f (Seq.toList s) []

let problem1 (input: string) : int =
    input |> removeOppositePolarity |> List.length

let problem2 (input: string) : int =
    seq { 'a' .. 'z' }
    |> Seq.map (fun lower ->
        let upper = Char.ToUpper(lower)

        input
        |> String.filter (fun c -> c <> lower && c <> upper)
        |> removeOppositePolarity
        |> List.length)
    |> Seq.min


// test
let testData1 = "dabAcCaCBAcCcaDA"
// 10
problem1 testData1
// 4
problem2 testData1

let input = "../input/day05.txt" |> File.ReadLines |> Seq.head
let ret1 = problem1 input
let ret2 = problem2 input

printfn "problem1 = %d" ret1
printfn "problem2 = %d" ret2
