open System.IO

let problem01 (s: string) : int =
    let cs = s |> Seq.toList
    let cs = List.append cs [ List.head cs ]

    List.tail cs
    |> List.fold (fun (acc, prev) c -> if prev = c then acc + (int c - int '0'), c else acc, c) (0, List.head cs)
    |> fst

let test1 () =
    let inputs = [ ("1122", 3); ("1111", 4); ("1234", 0); ("91212129", 9) ]

    for (input, expected) in inputs do
        let ret = problem01 input

        if ret <> expected then
            failwithf "FAILED input=%s expected=%d got=%d\n" input expected ret

test1 ()

let problem02 (s: string) : int =
    let half = s.Length / 2
    let nums = s |> Seq.map (fun c -> int c - int '0') |> Seq.toList
    let first = List.take half nums
    let second = List.skip half nums

    List.zip first second
    |> List.fold (fun acc (a, b) -> if a = b then acc + a + b else acc) 0

let test2 () =
    let inputs =
        [ ("1212", 6); ("1221", 0); ("123425", 4); ("123123", 12); ("12131415", 4) ]

    for (input, expected) in inputs do
        let ret = problem02 input

        if ret <> expected then
            failwithf "FAILED input=%s expected=%d got=%d" input expected ret

test2

let input = "../input/day01.txt" |> File.ReadAllLines |> Array.head
let ret1 = problem01 input
let ret2 = problem02 input

printfn "ret1 = %d" ret1
printfn "ret2 = %d" ret2
