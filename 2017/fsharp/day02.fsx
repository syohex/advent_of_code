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

let input =
    "../input/day02.txt"
    |> File.ReadAllLines
    |> Array.map (fun s -> s.Split() |> Array.map int |> Array.toList)
    |> Array.toList

let testData1 = [ [ 5; 1; 9; 5 ]; [ 7; 5; 3 ]; [ 2; 4; 6; 8 ] ]
let test1 = problem1 testData1
printfn "test1 = %d" test1 //  expected 18

let ret1 = problem1 input

printfn "problem1 = %d" ret1