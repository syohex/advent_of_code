open System.IO

let problem01 (s: string) : int =
    let cs = s |> Seq.toList
    let cs = List.append cs [ List.head cs ]

    List.tail cs
    |> List.fold (fun (acc, prev) c -> if prev = c then acc + (int c - int '0'), c else acc, c) (0, List.head cs)
    |> fst

let test () =
    let inputs = [ ("1122", 3); ("1111", 4); ("1234", 0); ("91212129", 9) ]

    for (input, expected) in inputs do
        let ret = problem01 input

        if ret <> expected then
            failwithf "FAILED input=%s expected=%d got=%d\n" input expected ret

test ()

let input = "../input/day01.txt" |> File.ReadAllLines |> Array.head
let ret1 = problem01 input
