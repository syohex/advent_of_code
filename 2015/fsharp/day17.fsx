open System.IO

let problem1 (nums: int list) (limit: int) : int =
    let rec f nums limit acc =
        if acc = limit then
            1
        elif acc > limit then
            0
        else
            match nums with
            | [] -> 0
            | h :: t ->
                let ret1 = f t limit (acc + h)
                let ret2 = f t limit acc
                ret1 + ret2

    f nums limit 0

// 4
problem1 [20;15;10;5;5] 25

let input = File.ReadLines("../input/day17.txt") |> Seq.map int |> Seq.toList

let ret1 = problem1 input 150
// 1638
printfn "ret1 = %d" ret1
