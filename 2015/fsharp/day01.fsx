open System.IO

let problem1 (s: string) : int =
    let rec f cs acc =
        match cs with
        | [] -> acc
        | '(' :: t -> f t (acc + 1)
        | ')' :: t -> f t (acc - 1)
        | h :: _ -> failwithf "invalid input %c" h

    f (Seq.toList s) 0

let problem2 (s: string) : int =
    let rec f cs steps acc =
        if acc = -1 then
            steps
        else
            match cs with
            | [] -> failwith "never reach here"
            | '(' :: t -> f t (steps + 1) (acc + 1)
            | ')' :: t -> f t (steps + 1) (acc - 1)
            | h :: _ -> failwithf "invalid input %c" h

    f (Seq.toList s) 0 0

let testInput =
    [ "(())"; "()()"; "((("; "(()(()("; "))((((("; "())"; "))("; ")))"; ")())())" ]

// [0, 0, 3, 3, 3, -1, -1, -3, -3]
testInput |> List.map problem1

let input = File.ReadLines("../input/day01.txt") |> Seq.head

// 232
problem1 input

// 1
problem2 ")"

// 5
problem2 "()())"

// 1783
problem2 input
