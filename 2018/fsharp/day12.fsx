open System
open System.IO
open System.Text.RegularExpressions

let patternToInt pattern : int =
    pattern |> Seq.fold (fun acc c -> if c = '#' then acc * 2 + 1 else acc * 2) 0

let parseInput (input: string list) : string * Map<int, char> =
    let parsePattern s =
        let m = Regex.Match(s, @"^([.#]+) => ([.#])$")

        if m.Success then
            patternToInt m.Groups.[1].Value, m.Groups.[2].Value.[0]
        else
            failwithf "cannot parse the pattern line: '%s'" s


    let m = Regex.Match(List.head input, @"^initial state: (.+)$")

    if m.Success then
        let initialState = m.Groups.[1].Value
        let patterns = input |> List.skip 2 |> List.map parsePattern
        initialState, Map.ofList patterns
    else
        failwithf "cannot parse state line '%s'" (List.head input)

let adjustState (state: (int * char) list) : (int * char) list * int =
    let f state =
        let pos = List.findIndex (fun (_, v) -> v = '#') state

        if pos < 4 then
            let diff = 4 - pos
            seq { 1..diff } |> Seq.fold (fun acc _ -> (-1, '.') :: acc) state, diff
        else
            state, 0

    let state, diff = f state
    let state', _ = f (List.rev state)
    List.rev state', diff

let problem1 (state: string) (table: Map<int, char>) (count: int) : int =
    let rec f i (state: (int * char) list) basePos =
        if i >= count then
            let zeroPos = List.tryFindIndex (fun (i, _) -> i = 0) state |> Option.defaultValue 0

            state
            |> List.indexed
            |> List.fold (fun acc (i, (_, v)) -> if v = '#' then acc + i - zeroPos else acc) 0
        else
            let state, diff = adjustState state

            let nextState =
                state
                |> List.windowed 5
                |> List.fold
                    (fun acc v ->
                        let p = v |> List.map snd |> patternToInt
                        let c = Map.tryFind p table |> Option.defaultValue '.'
                        let hasOrigin = v |> List.item 2 |> fst |> ((=) 0)
                        if hasOrigin then (0, c) :: acc else (-1, c) :: acc)
                    []
                |> List.rev

            f (i + 1) nextState (basePos + diff)

    f 0 (Seq.toList state |> List.indexed) 0

let testInput =
    "initial state: #..#.#..##......###...###

...## => #
..#.. => #
.#... => #
.#.#. => #
.#.## => #
.##.. => #
.#### => #
#.#.# => #
#.### => #
##.#. => #
##.## => #
###.. => #
###.# => #
####. => #"
        .Split('\n')
    |> Array.toList

let testState, testTable = parseInput testInput
problem1 testState testTable 20

let state, table = "../input/day12.txt" |> File.ReadAllLines |> Seq.toList |> parseInput
// not 3189
let ret1 = problem1 state table 20
printfn "problem1 = %d" ret1
