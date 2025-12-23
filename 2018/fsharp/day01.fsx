open System.IO

let problem1 (input: string list) : int =
    input |> List.fold (fun acc s -> acc + int s) 0

let problem2 (input: string list) : int =
    let rec f nums visited origNums acc =
        match nums with
        | [] -> f origNums visited origNums acc
        | h :: t ->
            let acc = acc + h

            if Set.contains acc visited then
                acc
            else
                f t (Set.add acc visited) origNums acc

    let nums = input |> List.map int
    f nums (Set.add 0 Set.empty) nums 0

// test
// 3
problem1 [ "+1"; "-2"; "+3"; "+1" ]
// 3
problem1 [ "+1"; "+1"; "+1" ]
// 0
problem1 [ "+1"; "+1"; "-2" ]
// -6
problem1 [ "-1"; "-2"; "-3" ]

// 2
problem2 [ "+1"; "-2"; "+3"; "+1" ]
// 0
problem2 [ "+1"; "-1" ]
// 10
problem2 [ "+3"; "+3"; "+4"; "-2"; "-4" ]
// 5
problem2 [ "-6"; "+3"; "+8"; "+5"; "-6" ]
// 14
problem2 [ "+7"; "+7"; "-2"; "-7"; "-4" ]

let inputData = "../input/day01.txt"
let input = inputData |> File.ReadLines |> Seq.toList

let ret1 = problem1 input
let ret2 = problem2 input

printfn "problem1 = %d" ret1
printfn "problem2 = %d" ret2
