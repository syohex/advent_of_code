open System.IO

type Operation =
    | On
    | Off
    | Toggle

type Data =
    { Operation: Operation
      From: int * int
      To: int * int }

let parseInput (s: string) : Data =
    let parsePosition (s: string) : int * int =
        let parts = s.Split(',')
        int parts.[0], int parts.[1]

    let parts = s.Split(' ')

    match parts.[0] with
    | "turn" ->
        match parts.[1] with
        | "on" ->
            { Operation = On
              From = parsePosition parts.[2]
              To = parsePosition parts.[4] }
        | "off" ->
            { Operation = Off
              From = parsePosition parts.[2]
              To = parsePosition parts.[4] }
        | _ -> failwithf "unknown operation %s" parts.[1]
    | "toggle" ->
        { Operation = Toggle
          From = parsePosition parts.[1]
          To = parsePosition parts.[3] }
    | _ -> failwithf "unknown operation %s" parts.[0]

let problem1 (input: Data list) : int =
    let grid = Array2D.zeroCreate 1000 1000

    input
    |> List.fold
        (fun
            (acc: int[,])
            { Operation = op
              From = x1, y1
              To = x2, y2 } ->
            match op with
            | On ->
                for i in x1..x2 do
                    for j in y1..y2 do
                        acc.[i, j] <- 1

                acc
            | Off ->
                for i in x1..x2 do
                    for j in y1..y2 do
                        acc.[i, j] <- 0

                acc
            | Toggle ->
                for i in x1..x2 do
                    for j in y1..y2 do
                        acc.[i, j] <- if acc.[i, j] = 1 then 0 else 1

                acc)
        grid
    |> Seq.cast<int>
    |> Seq.filter ((=) 1)
    |> Seq.length

let problem2 (input: Data list) : int =
    let grid = Array2D.zeroCreate 1000 1000

    input
    |> List.fold
        (fun
            (acc: int[,])
            { Operation = op
              From = x1, y1
              To = x2, y2 } ->
            match op with
            | On ->
                for i in x1..x2 do
                    for j in y1..y2 do
                        acc.[i, j] <- acc.[i, j] + 1

                acc
            | Off ->
                for i in x1..x2 do
                    for j in y1..y2 do
                        acc.[i, j] <- max (acc.[i, j] - 1) 0

                acc
            | Toggle ->
                for i in x1..x2 do
                    for j in y1..y2 do
                        acc.[i, j] <- acc.[i, j] + 2

                acc)
        grid
    |> Seq.cast<int>
    |> Seq.reduce (+)

let input =
    File.ReadLines("../input/day06.txt") |> Seq.map parseInput |> Seq.toList

let ret1 = problem1 input
// 400410
printfn $"ret1={ret1}"

let ret2 = problem2 input
// 15343601
printfn $"ret2={ret2}"
