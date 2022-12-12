open System
open System.IO
open System.Text.RegularExpressions

type Operation = { Count: int; From: int; To: int }

let rec splitPart (lines: string list) acc stackPart =
    match lines with
    | [] -> stackPart, acc |> List.rev
    | h :: t ->
        if String.IsNullOrEmpty(h) then
            splitPart t [] (acc |> List.tail |> List.rev)
        else
            splitPart t (h :: acc) stackPart

let toMatrix (lines: string list) =
    let rec toMatrix' (lines: string list) acc =
        match lines with
        | [] -> array2D acc
        | h :: t -> toMatrix' t ((h |> Seq.toList) :: acc)

    toMatrix' lines []

let parseStackLines (lines: string list) =
    let readStack col (matrix: char [,]) =
        let rec readStack' row col (matrix: char [,]) acc =
            let rows = Array2D.length1 matrix

            if row >= rows || matrix.[row, col] = ' ' then
                acc
            else
                readStack' (row + 1) col matrix (matrix.[row, col] :: acc)

        readStack' 0 col matrix []

    let rec parseInput' col limit matrix acc =
        if col >= limit then
            acc |> List.rev
        else
            let stack = readStack col matrix
            parseInput' (col + 4) limit matrix (stack :: acc)

    let matrix = toMatrix lines
    let limit = Array2D.length2 matrix
    parseInput' 1 limit matrix [] |> Array.ofList

let parseOperationLine (s: string) : Operation =
    let regexp = Regex(@"move (\d+) from (\d+) to (\d+)", RegexOptions.Compiled)
    let m = regexp.Match(s)

    { Count = m.Groups[1].Value |> int
      From = m.Groups[2].Value |> int
      To = m.Groups[3].Value |> int }

let parseInput (file: string) =
    let lines = File.ReadLines(file) |> Seq.toList
    let stackPart, operationPart = splitPart lines [] []
    let operations = operationPart |> List.map parseOperationLine
    stackPart |> parseStackLines, operations

let problem1 (stacks: char list []) (operations: Operation list) : string =
    let rec moveStacks i n fromStack toStack =
        if i >= n then
            fromStack, toStack
        else
            moveStacks (i + 1) n (fromStack |> List.tail) ((List.head fromStack) :: toStack)

    let rec problem1' operations (stacks: char list []) =
        match operations with
        | [] -> stacks
        | { Count = c; From = f; To = t } :: rest ->
            let fromStack, toStack = moveStacks 0 c stacks.[f - 1] stacks.[t - 1]
            stacks.[f - 1] <- fromStack
            stacks.[t - 1] <- toStack
            problem1' rest stacks

    let stacks' = problem1' operations stacks
    stacks' |> Array.map List.head |> String.Concat

let problem2 (stacks: char list []) (operations: Operation list) : string =
    let rec problem2' operations (stacks: char list []) =
        match operations with
        | [] -> stacks
        | { Count = c; From = f; To = t } :: rest ->
            let fromStack = List.skip c stacks.[f - 1]
            let toStack = (List.take c stacks.[f - 1]) @ stacks.[t - 1]
            stacks.[f - 1] <- fromStack
            stacks.[t - 1] <- toStack
            problem2' rest stacks

    let stacks' = problem2' operations stacks
    stacks' |> Array.map List.head |> String.Concat

let testStacks, testOperations = parseInput "test.txt"
// "CMZ"
problem1 (Array.copy testStacks) testOperations
// "MCD"
problem2 (Array.copy testStacks) testOperations

let stacks, operations = parseInput "input.txt"
// "TWSGQHNHL"
problem1 (Array.copy stacks) operations
// "JNRSCDWPP"
problem2 (Array.copy stacks) operations
