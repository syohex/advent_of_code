open System

let toString (v: (int * char) list) : string =
    let rec f v (acc: string list) =
        match v with
        | [] -> acc |> List.rev |> String.Concat
        | (count, c) :: t -> f t (string c :: string count :: acc)

    f v []

let toLookAndSay (s: string) =
    let rec f cs prev count acc =
        match cs with
        | [] ->
            if count = 0 then
                acc |> List.rev |> toString
            else
                f [] '?' 0 ((count, prev) :: acc)
        | h :: t ->
            if h = prev then
                f t prev (count + 1) acc
            else
                f t h 1 ((count, prev) :: acc)

    match Seq.toList s with
    | [] -> failwith "never reach here"
    | h :: t -> f t h 1 []

let rec problem1 (input: string) (count: int) : int =
    if count = 0 then
        input.Length
    else
        let ret = toLookAndSay input
        problem1 ret (count - 1)

// [11,21,1211,111221,312211]
seq { 1..5 } |> Seq.map (fun i -> problem1 "1" i)

let input = "3113322113"
let ret1 = problem1 input 40
printfn "ret1=%d" ret1
