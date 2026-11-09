open System
open System.IO

let problem1 (grid: List<List<int>>) : int =
    grid
    |> List.map (fun v ->
        let minNum, maxNum =
            v
            |> List.fold (fun (minNum, maxNum) n -> min n minNum, max n maxNum) (Int32.MaxValue, Int32.MinValue)

        maxNum - minNum)
    |> List.sum

let rec findDivisibleValues (nums: int list) : int * int =
    match nums with
    | [] -> failwith "never reach here"
    | h :: t ->
        match List.tryFind (fun n -> n % h = 0) t with
        | Some m -> h, m
        | None -> findDivisibleValues t

let problem2 (grid: List<List<int>>) : int =
    grid
    |> List.map (List.sort >> findDivisibleValues)
    |> List.map (fun (a, b) -> b / a)
    |> List.sum

let input =
    "../input/day02.txt"
    |> File.ReadAllLines
    |> Array.map (fun s -> s.Split() |> Array.map int |> Array.toList)
    |> Array.toList

let testData1 = [ [ 5; 1; 9; 5 ]; [ 7; 5; 3 ]; [ 2; 4; 6; 8 ] ]
let test1 = problem1 testData1
printfn "test1 = %d" test1 //  expected 18

let testData2 = [ [ 5; 9; 2; 8 ]; [ 9; 4; 7; 3 ]; [ 3; 8; 6; 5 ] ]
let test2 = problem2 testData2
printfn "test2 = %d" test2 //  expected 9

let ret1 = problem1 input
let ret2 = problem2 input

printfn "problem1 = %d" ret1
printfn "problem2 = %d" ret2
