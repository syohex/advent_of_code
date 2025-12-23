open System.IO

let toFreq (s: string) : int[] =
    s
    |> Seq.fold
        (fun (acc: int[]) c ->
            let index = int c - int 'a'
            acc.[index] <- acc.[index] + 1
            acc)
        (Array.zeroCreate 26)

let problem1 (input: string list) : int =
    let hasTwo = Array.exists ((=) 2)
    let hasThree = Array.exists ((=) 3)

    input
    |> List.fold
        (fun (twos, threes) s ->
            let freq = toFreq s
            let twos = if hasTwo freq then twos + 1 else twos
            let threes = if hasThree freq then threes + 1 else threes
            twos, threes)
        (0, 0)
    |> (fun (twos, threes) -> twos * threes)

let maskNthChar (pos: int) (s: string) : string =
    s |> String.mapi (fun i c -> if i = pos then '*' else c)


let problem2 (input: string list) : string =
    let len = input |> List.head |> _.Length

    let rec f pos input =
        if pos = len then
            failwith "never reach here"
        else
            let masked = input |> List.map (maskNthChar pos)
            let counts = List.countBy id masked

            match List.tryFind (fun (_, count) -> count = 2) counts with
            | Some(v, _) -> v |> String.filter ((<>) '*')
            | None -> f (pos + 1) input

    f 0 input

// test
let testData1 =
    [ "abcdef"; "bababc"; "abbcde"; "abcccd"; "aabcdd"; "abcdee"; "ababab" ]
// 12
problem1 testData1

let testData2 = [ "abcde"; "fghij"; "klmno"; "pqrst"; "fguij"; "axcye"; "wvxyz" ]
// "fgij"
problem2 testData2

let input = "../input/day02.txt" |> File.ReadLines |> Seq.toList
let ret1 = problem1 input
let ret2 = problem2 input

printfn "problem1 = %d" ret1
printfn "problem2 = %s" ret2
