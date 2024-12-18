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

let problem2 (nums: int list) (limit: int) : int =
    let rec f nums limit sum len acc =
        if sum = limit then
            match Map.tryFind len acc with
            | Some(v) -> Map.add len (v + 1) acc
            | None -> Map.add len 1 acc
        elif sum > limit then
            acc
        else
            match nums with
            | [] -> acc
            | h :: t ->
                let acc = f t limit (sum + h) (len + 1) acc
                f t limit sum len acc

    let m = f nums limit 0 0 Map.empty
    let minKey = m |> Map.keys |> Seq.min
    Map.find minKey m

let testData = [ 20; 15; 10; 5; 5 ]
// 4
problem1 testData 25
// 3
problem2 testData 25

let input = File.ReadLines("../input/day17.txt") |> Seq.map int |> Seq.toList

let ret1 = problem1 input 150
// 1638
printfn "ret1 = %d" ret1

let ret2 = problem2 input 150
printfn "ret2 = %d" ret2
