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

let adjustState (state: char list) : char list * int =
    let f state =
        let pos = List.findIndex ((=) '#') state

        if pos < 4 then
            let diff = 4 - pos
            seq { 1..diff } |> Seq.fold (fun acc _ -> '.' :: acc) state, diff
        else
            state, 0

    let state, diff = f state
    let state', _ = f (List.rev state)
    List.rev state', diff

let problem1 (state: string) (table: Map<int, char>) (count: int64) : int =
    let rec f i (state: char list) basePos =
        if i >= count then
            state
            |> List.indexed
            |> List.fold (fun acc (i, v) -> if v = '#' then acc + i + basePos else acc) 0
        else
            let state, diff = adjustState state
            let basePos = basePos - diff

            let nextState =
                state
                |> List.windowed 5
                |> List.map patternToInt
                |> List.map (fun p -> Map.tryFind p table |> Option.defaultValue '.')

            let firstHash = List.findIndex ((=) '#') nextState
            let basePos = 2 + basePos + firstHash
            let nextState = List.skipWhile ((=) '.') nextState

            f (i + 1L) nextState basePos

    f 0 (Seq.toList state) 0

let problem2 (state: string) (table: Map<int, char>) (count: int64) : int64 =
    let ret = problem1 state table 100
    let ret1 = problem1 state table 101
    let diff = (ret1 - ret) |> int64
    int64 ret + diff * (count - 100L)

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

let state, table =
    "../input/day12.txt" |> File.ReadAllLines |> Seq.toList |> parseInput
let ret1 = problem1 state table 20L
let ret2 = problem2 state table 50000000000L

printfn "problem1 = %d" ret1
printfn "problem2 = %d" ret2
