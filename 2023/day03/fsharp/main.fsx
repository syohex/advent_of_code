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

let detectNumberLocation
    (row: int)
    (col: int)
    (engine: char[,])
    (visited: Set<(int * int)>)
    : (int * Set<(int * int)>) =
    let rec startPos col =
        if col < 0 then
            0
        else if Char.IsNumber(engine.[row, col]) then
            startPos (col - 1)
        else
            col + 1

    let rec endPos col =
        if col >= Array2D.length2 engine then
            col - 1
        else if Char.IsNumber(engine.[row, col]) then
            endPos (col + 1)
        else
            col - 1

    let s, e = startPos col, endPos col
    let num = extractNumber row s e engine
    let visited' = seq { s..e } |> Seq.fold (fun acc i -> Set.add (row, i) acc) visited
    num, visited'

let findTwoNumbers (row: int) (col: int) (engine: char[,]) : Option<(int * int)> =
    let minRow = Math.Max(0, row - 1)
    let maxRow = Math.Min(Array2D.length1 engine - 1, row + 1)
    let minCol = Math.Max(0, col - 1)
    let maxCol = Math.Min(Array2D.length2 engine - 1, col + 1)

    let rec findTwoNumbers' row col visited first second =
        if row > maxRow then
            match first, second with
            | Some(a), Some(b) -> Some((a, b))
            | _ -> None
        elif col > maxCol then
            findTwoNumbers' (row + 1) minCol visited first second
        elif Set.contains (row, col) visited then
            findTwoNumbers' row (col + 1) visited first second
        else if Char.IsNumber(engine.[row, col]) then
            let num, visited' = detectNumberLocation row col engine visited

            if Option.isNone first then
                findTwoNumbers' row (col + 1) visited' (Some(num)) second
            else
                findTwoNumbers' row (col + 1) visited' first (Some(num))
        else
            findTwoNumbers' row (col + 1) visited first second

    findTwoNumbers' minRow minCol Set.empty None None

let findGearNumbers (engine: char[,]) : (int * int) list =
    let rows, cols = Array2D.length1 engine, Array2D.length2 engine

    let rec findGearNumbers row col acc =
        if row >= rows then
            List.rev acc
        elif col >= cols then
            findGearNumbers (row + 1) 0 acc
        else if engine.[row, col] = '*' then
            match findTwoNumbers row col engine with
            | Some((a, b)) -> findGearNumbers row (col + 1) ((a, b) :: acc)
            | None -> findGearNumbers row (col + 1) acc
        else
            findGearNumbers row (col + 1) acc

    findGearNumbers 0 0 []

let problem2 (input: string list) : int64 =
    input
    |> parseInput
    |> findGearNumbers
    |> List.map (fun (a, b) -> int64 a * int64 b)
    |> List.sum

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

    problem1 input

let test2 () : int64 =
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

    problem2 input

// 4361
test1 ()

// 467835
test2 ()

let input = readInput "../input.txt"
// 537832
problem1 input

// 81939900
problem2 input
