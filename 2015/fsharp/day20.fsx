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

let problem2 (input: int) : int =
    let lowest = Math.Ceiling(double input / 11.0) |> int

    let rec f i (dp: int[]) =
        seq { 1..50 }
        |> Seq.iter (fun j ->
            let k = i * j

            if k < lowest then
                dp.[k] <- dp.[k] + i * 11)

        if (i + 1) >= lowest then
            match Array.tryFindIndex (fun n -> n >= input) dp with
            | None -> failwith "never reach here"
            | Some(v) -> v
        else
            f (i + 1) dp

    let dp = Array.zeroCreate lowest
    f 1 dp

problem1 150

let ret1 = problem1 29000000
// 665280
printfn "ret1 = %d" ret1

let ret2 = problem2 29000000
// 705600
printfn "ret2 = %d" ret2
