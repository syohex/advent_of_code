open System

let sumDivisor (num: int) =
    let limit = Math.Sqrt(double num) |> int

    seq { 1..limit }
    |> Seq.fold
        (fun acc j ->
            if num % j = 0 then
                let div = num / j
                acc + j + (if j = div then 0 else div)
            else
                acc)
        0

let problem1 (input: int) : int =
    let lowest = input / 10

    let rec f i =
        if sumDivisor i >= lowest then i else f (i + 1)

    f 1

problem1 150
let ret1 = problem1 29000000
printfn "ret1 = %d" ret1
