open System
open System.IO

let readInput (f: string) : string list = File.ReadLines(f) |> Seq.toList

let parseInput (input: string list) : char[,] =
    let rec parseInput' lines acc =
        match lines with
        | [] -> acc |> List.rev |> array2D
        | h :: t -> parseInput' t ((Seq.toList h) :: acc)

    parseInput' input []

let isSymbol (c: char) : bool = (Char.IsNumber(c) || c = '.') |> not

let hasSymbolInNeighbor (row: int) (startCol: int) (endCol: int) (engine: char[,]) : bool =
    let minRow = Math.Max(0, row - 1)
    let maxRow = Math.Min(Array2D.length1 engine - 1, row + 1)
    let minCol = Math.Max(0, startCol - 1)
    let maxCol = Math.Min(Array2D.length2 engine - 1, endCol + 1)

    seq {
        for i in minRow..maxRow do
            for j in minCol..maxCol do
                engine.[i, j]
    }
    |> Seq.exists isSymbol

let findEndNumberColumn (row: int) (col: int) (engine: char[,]) : int =
    let cols = Array2D.length2 engine

    let rec findEndNumberColumn' row col (engine: char[,]) =
        if col >= cols then
            cols - 1
        else if Char.IsNumber(engine.[row, col]) then
            findEndNumberColumn' row (col + 1) engine
        else
            col - 1

    findEndNumberColumn' row col engine

let charDigitToInt (c: char) : int = int c - int '0'

let extractNumber (row: int) (startCol: int) (endCol: int) (engine: char[,]) : int =
    let rec extractNumber' row col endCol acc =
        if col > endCol then
            acc
        else
            extractNumber' row (col + 1) endCol (10 * acc + charDigitToInt engine.[row, col])

    extractNumber' row startCol endCol 0


let findNumbers (engine: char[,]) : int list =
    let rows, cols = Array2D.length1 engine, Array2D.length2 engine

    let rec findNumbers' row col acc =
        if row >= rows then
            List.rev acc
        elif col >= cols then
            findNumbers' (row + 1) 0 acc
        else if Char.IsNumber(engine.[row, col]) then
            let endCol = findEndNumberColumn row col engine

            if hasSymbolInNeighbor row col endCol engine then
                let num = extractNumber row col endCol engine
                findNumbers' row (endCol + 1) (num :: acc)
            else
                findNumbers' row (endCol + 1) acc

        else
            findNumbers' row (col + 1) acc

    findNumbers' 0 0 []

let problem1 (input: string list) : int =
    input |> parseInput |> findNumbers |> List.sum

let test1 () : int =
    let input =
        [ "467..114.."
          "...*......"
          "..35..633."
          "......#..."
          "617*......"
          ".....+.58."
          "..592....."
          "......755."
          "...$.*...."
          ".664.598.." ]

    let a = input |> parseInput |> findNumbers
    printfn "## %A" a
    a |> List.sum

// 4361
test1 ()

let input = readInput "../input.txt"
problem1 input
