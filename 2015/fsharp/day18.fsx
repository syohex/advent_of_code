open System.IO

let neighbors =
    [ (-1, -1); (-1, 0); (-1, 1); (0, -1); (0, 1); (1, -1); (1, 0); (1, 1) ]

let countLights row col (input: char[,]) rows cols =
    neighbors
    |> List.map (fun (x, y) -> row + x, col + y)
    |> List.filter (fun (x, y) -> x >= 0 && x < rows && y >= 0 && y < cols)
    |> List.filter (fun (x, y) -> input.[x, y] = '#')
    |> List.length

let update (input: char[,]) =
    let rows, cols = Array2D.length1 input, Array2D.length2 input
    let ret = Array2D.zeroCreate rows cols

    let rec update' row col =
        if row >= rows then
            ret
        elif col >= cols then
            update' (row + 1) 0
        else
            let lights = countLights row col input rows cols

            if input.[row, col] = '#' then
                ret.[row, col] <- if lights = 2 || lights = 3 then '#' else '.'
            else
                ret.[row, col] <- if lights = 3 then '#' else '.'

            update' row (col + 1)

    update' 0 0

let update2 (input: char[,]) =
    let rows, cols = Array2D.length1 input, Array2D.length2 input
    let ret = Array2D.zeroCreate rows cols
    let corners = [ (0, 0); (0, cols - 1); (rows - 1, 0); (rows - 1, cols - 1) ]

    corners
    |> List.iter (fun (x, y) ->
        input.[x, y] <- '#'
        ret.[x, y] <- '#')

    let rec update2' row col =
        if row >= rows then
            ret
        elif col >= cols then
            update2' (row + 1) 0
        elif List.exists ((=) (row, col)) corners then
            update2' row (col + 1)
        else
            let lights = countLights row col input rows cols

            if input.[row, col] = '#' then
                ret.[row, col] <- if lights = 2 || lights = 3 then '#' else '.'
            else
                ret.[row, col] <- if lights = 3 then '#' else '.'

            update2' row (col + 1)

    update2' 0 0

let problem1 (input: char[,]) (steps: int) : int =
    let input = Array2D.copy input

    seq { 1..steps }
    |> Seq.fold (fun acc _ -> update acc) input
    |> Seq.cast<char>
    |> Seq.filter ((=) '#')
    |> Seq.length

let problem2 (input: char[,]) (steps: int) : int =
    let input = Array2D.copy input

    seq { 1..steps }
    |> Seq.fold (fun acc _ -> update2 acc) input
    |> Seq.cast<char>
    |> Seq.filter ((=) '#')
    |> Seq.length

let testInput =
    [ ".#.#.#"; "...##."; "#....#"; "..#..."; "#.#..#"; "####.." ]
    |> List.map Seq.toList
    |> array2D

// 4
problem1 testInput 4

// 17
problem2 testInput 5

let input =
    File.ReadLines("../input/day18.txt")
    |> Seq.map Seq.toList
    |> Seq.toList
    |> array2D

// 814
problem1 input 100

// 924
problem2 input 100
