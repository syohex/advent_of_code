open System
open System.IO

let parseLine (s: string) : int * int =
    let rec f cs acc =
        match cs with
        | [] -> failwith "never reach here"
        | '"' :: [] -> s.Length, acc
        | '\\' :: '\\' :: t
        | '\\' :: '"' :: t -> f t (acc + 1)
        | '\\' :: 'x' :: c1 :: c2 :: t when Char.IsAsciiHexDigit(c1) && Char.IsAsciiHexDigit(c2) -> f t (acc + 1)
        | _ :: t -> f t (acc + 1)

    f (s |> Seq.toList |> List.tail) 0

let problem1 (lines: string list) : int =
    lines
    |> List.map parseLine
    |> List.fold (fun (acc1, acc2) (total, memory) -> acc1 + total, acc2 + memory) (0, 0)
    |> fun (total, memory) -> total - memory

let testInput = [ "\"\""; "\"abc\""; "\"aaa\\\"aaa\""; "\"\\x27\"" ]
// 23, 11
problem1 testInput

let input = File.ReadLines("../../input/day08.txt") |> Seq.toList

let ret1 = problem1 input
printfn "ret1=%d" ret1
