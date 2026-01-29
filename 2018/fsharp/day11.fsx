open System

let calculatePowerLevel (x: int) (y: int) (gridSerialNumber: int) : int =
    let rackId = x + 10
    let powerLevel = rackId * y
    let powerLevel = powerLevel + gridSerialNumber
    let powerLevel = powerLevel * rackId
    let hundredDigit = (powerLevel / 100) % 10
    hundredDigit - 5

let totalPower ((x, y): int * int) (squareSize: int) (grid: int[,]) : int =
    seq {
        for i in 0 .. (squareSize - 1) do
            for j in 0 .. (squareSize - 1) do
                yield grid.[x + i, y + j]
    }
    |> Seq.sum

let findLargestTotalPower (size: int) (squareSize: int) (grid: int[,]) : int * int * int =
    let rec f row col limit (maxRow, maxCol) maxPower =
        if row >= limit then
            maxRow + 1, maxCol + 1, maxPower
        elif col >= limit then
            f (row + 1) 0 limit (maxRow, maxCol) maxPower
        else
            let power = totalPower (row, col) squareSize grid

            if power > maxPower then
                f row (col + 1) limit (row, col) power
            else
                f row (col + 1) limit (maxRow, maxCol) maxPower

    let limit = size - squareSize
    f 0 0 limit (-1, -1) Int32.MinValue

let gridToPrefixGrid (size: int) (grid: int[,]) : int[,] =
    let acc = Array2D.zeroCreate (size + 1) (size + 1)

    for x in 1..size do
        for y in 1..size do
            acc.[x, y] <- acc.[x - 1, y] + acc.[x, y - 1] - acc.[x - 1, y - 1] + grid[x - 1, y - 1]

    acc

let toSumGrid (squareSize: int) (prefixGrid: int[,]) : int[,] =
    let size = (Array2D.length1 prefixGrid) - 1
    let sumGridSize = size - squareSize + 1

    let ret = Array2D.zeroCreate sumGridSize sumGridSize

    for i in 0 .. (sumGridSize - 1) do
        for j in 0 .. (sumGridSize - 1) do
            let power =
                prefixGrid.[i + squareSize, j + squareSize]
                - prefixGrid.[i, j + squareSize]
                - prefixGrid.[i + squareSize, j]
                + prefixGrid.[i, j]

            ret.[i, j] <- power

    ret

let problem1 (gridSerialNumber: int) : int * int * int =
    let grid = Array2D.zeroCreate 300 300

    for x in 1..300 do
        for y in 1..300 do
            let v = calculatePowerLevel x y gridSerialNumber
            grid.[x - 1, y - 1] <- v

    findLargestTotalPower 300 3 grid

let problem2 (gridSerialNumber: int) : int * int * int * int =
    let grid = Array2D.zeroCreate 300 300

    for x in 1..300 do
        for y in 1..300 do
            let v = calculatePowerLevel x y gridSerialNumber
            grid.[x - 1, y - 1] <- v

    let prefixGrid = gridToPrefixGrid 300 grid

    seq { 1..300 }
    |> Seq.map (fun size ->
        toSumGrid size prefixGrid
        |> Array2D.mapi (fun x y power -> x + 1, y + 1, size, power)
        |> Seq.cast<int * int * int * int>
        |> Seq.sortWith (fun (_, _, _, power1) (_, _, _, power2) -> compare power2 power1)
        |> Seq.head)
    |> Seq.sortWith (fun (_, _, _, power1) (_, _, _, power2) -> compare power2 power1)
    |> Seq.head

let test () : unit =
    // -5
    printfn "# %d" <| calculatePowerLevel 122 79 57
    // 0
    printfn "# %d" <| calculatePowerLevel 217 196 39
    // 4
    printfn "# %d" <| calculatePowerLevel 101 153 71
    // 33, 45, 29
    printfn "# %A" <| problem1 18
    // 21, 61, 30
    printfn "# %A" <| problem1 42
    // 90, 269, 16, 113
    printfn "# %A" <| problem2 18
    // 232, 251, 12, 119
    printfn "# %A" <| problem2 42
    ()

let input = 7689
let x1, y1, ret1 = problem1 input
let x2, y2, size2, ret2 = problem2 input

printfn "problem1 = %d(x=%d, y=%d)" ret1 x1 y1
printfn "problem2 = %d(x=%d, y=%d, size=%d)" ret2 x2 y2 size2
